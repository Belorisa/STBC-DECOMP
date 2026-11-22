"""Frame slots and callee-saved registers of one function, retail's beside ours, ours labelled with the source's
variable names: a reading of a frame or register wall instead of a guess from two disassemblies.

    python tools/framemap.py 0x712A60                 # an annotated function, by address or name
    python tools/framemap.py CloseRedbookCD --rev HEAD~3   # the unit's file as it was at a revision
    python tools/framemap.py 0x712A60 --source old.cpp     # the unit compiled from another file

It is a diagnosis tool: it reads, it never writes or permutes source.

How each side is read:

- Frame slots. Every esp- and ebp-based memory operand is normalised to the function's entry: offset 0 is the return
  address, parameters sit at +4 and up, locals below 0. ESP is tracked along the control flow through pushes, pops,
  `sub/add esp` and calls; a call pops what its callee's `ret N` says (retail's callee, read from the image; ours
  takes the pop of the retail call it pairs with), else everything pushed since the last call when no `add esp`
  follows (thiscall, stdcall and imports), else nothing (cdecl). Each slot records its size, how it is accessed (int,
  byte, word, float, address taken, `this` of a call, pushed as an argument, EH state), its first write, its reads
  and the instruction range where it is used.
- Names (ours). The unit is compiled a second time, for diagnosis only, with /Z7 into build/framemap/: the CodeView
  S_BPREL32 records name the frame slots, S_REGISTER the few register locals it records, and the COFF line numbers
  map each instruction to its source line. That copy is never gated. Its code bytes are compared with the gated
  object's (build/obj, compiled as `match` does); when they differ the tool says so, since the names then come from
  a different layout. A register value is named from the source line of the instruction that sets it: the local
  assigned there, else the line's text.
- Pairing. The two instruction lists are aligned with the frame offsets, registers and addresses left out (what
  stays equal when only the frame or the registers differ); each aligned pair votes for "our slot X is retail's slot
  Y" and "our register value is retail's register value". Unpaired slots are paired by order of first use.

The output: a one-line reading when the tables show a pattern the levers know (docs/levers/locals.md and walls.md),
the frame table, then the callee-saved registers (push order, the values each holds, the swaps).
"""

import argparse
import bisect
import difflib
import json
import os
import re
import shutil
import struct
import subprocess
import sys
from collections import Counter, defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import coff  # noqa: E402
import decomp  # noqa: E402
import vc6  # noqa: E402

ROOT = decomp.ROOT
OUT = decomp.BUILD / "framemap"

SAVED = ("ebx", "esi", "edi", "ebp")
REG32 = {"al": "eax", "ah": "eax", "ax": "eax", "bl": "ebx", "bh": "ebx", "bx": "ebx", "cl": "ecx", "ch": "ecx",
         "cx": "ecx", "dl": "edx", "dh": "edx", "dx": "edx", "si": "esi", "di": "edi", "bp": "ebp", "sp": "esp"}
REG = re.compile(r"\b(e?[abcd]x|[abcd][lh]|e?[sd]i|e?[sb]p)\b")
SIZES = {"byte": 1, "word": 2, "dword": 4, "qword": 8, "tbyte": 10, "xword": 10}
MEM = re.compile(r"(?:(byte|word|dword|qword|tbyte|xword) ptr )?(?:[a-z]s:)?\[([^\]]*)\]")
CV_REGS = {17: "eax", 18: "ecx", 19: "edx", 20: "ebx", 21: "esp", 22: "ebp", 23: "esi", 24: "edi",
           1: "al", 2: "cl", 3: "dl", 4: "bl", 9: "ax", 10: "cx", 11: "dx", 12: "bx", 13: "sp", 14: "bp", 15: "si",
           16: "di"}
WRITES = ("mov", "lea", "movzx", "movsx", "pop", "xor", "sub", "add", "and", "or", "inc", "dec", "neg", "not",
          "shl", "shr", "sar", "imul", "sbb", "adc", "setne", "sete")
PURE_WRITES = ("mov", "lea", "movzx", "movsx", "pop")


def reg32(r):
    return REG32.get(r, r)


# ---------------------------------------------------------------------------
# instructions

class Ins:
    __slots__ = ("addr", "mnem", "ops", "size", "reloc", "delta", "ebp_frame", "slots", "reached")

    def __init__(self, addr, mnem, ops, size, reloc=False):
        self.addr, self.mnem, self.ops, self.size, self.reloc = addr, mnem, ops, size, reloc
        self.delta = None      # esp relative to the entry esp, before this instruction
        self.ebp_frame = None  # ebp relative to the entry esp, when ebp is the frame pointer
        self.slots = []        # [(offset, size, access)] frame operands of this instruction
        self.reached = False   # on a path from the entry (jump tables followed); the rest is data read as code

    def __repr__(self):
        return "%08X %s %s" % (self.addr, self.mnem, self.ops)


def disassemble(code, addr, reloc_sites=()):
    """[Ins] of a function's bytes, cut at its end (walls.cut_at_end). reloc_sites: offsets of relocation fields
    (ours), so that an instruction holding one has its constants read as addresses."""
    import walls
    sized = walls.cut_at_end(decomp.disasm_sized(code, addr), code, addr)
    sites = sorted(reloc_sites)
    out = []
    for a, m, o, n in sized:
        if m in walls.FILL:
            continue
        k = bisect.bisect_left(sites, a - addr)
        hit = k < len(sites) and sites[k] < a - addr + n
        out.append(Ins(a, m, o, n, hit))
    return out


def normal(ins):
    """An instruction with addresses, registers and frame displacements left out (decomp.frame_free's view) plus the
    relocated constants of ours: what stays equal when only the frame layout or the register picks differ."""
    o = ins.ops
    if ins.mnem in ("call", "jmp") or ins.mnem.startswith("j"):
        if not REG.search(o) and "[" not in o:
            return ins.mnem + " X"
    if ins.reloc:
        o = MEM.sub(lambda m: m.group(0) if REG.search(m.group(2)) else "[X]", o)
        o = re.sub(r"(?<![\w\[+-] )\b0x[0-9a-f]+\b|(?<=, )\d+\b", "X", o) if "[" not in o else o
    o = re.sub(r"0x[0-9a-f]{5,8}", "X", o)
    o = MEM.sub(lambda m: "%s[F]" % (m.group(1) + " ptr " if m.group(1) else "")
                if re.match(r"e[sb]p\b", m.group(2)) else m.group(0), o)   # a frame slot, wherever it is
    o = REG.sub("R", o)
    o = MEM.sub(lambda m: "%s[%s]" % (m.group(1) + " ptr " if m.group(1) else "",
                                       re.sub(r"0x[0-9a-f]+|\b\d+\b", "N", m.group(2)) if "R" in m.group(2)
                                       else m.group(2)), o)
    return "%s %s" % (ins.mnem, o)


# ---------------------------------------------------------------------------
# the frame: esp tracking and slots

def _imm(text):
    text = text.strip()
    try:
        return int(text, 16) if text.startswith("0x") else int(text)
    except ValueError:
        return None


def first_flow(ins):
    """Index of the first call, branch or return: VC6 schedules the prologue's allocation and saves before it."""
    return next((k for k, i in enumerate(ins) if i.mnem == "call" or i.mnem.startswith(("j", "ret", "loop"))),
                len(ins))


def push_roles(ins):
    """{index: "save" | "alloc" | "eh"} for the pushes that aren't arguments: the saves of callee-saved registers,
    scratch registers pushed at entry to allocate a slot (`push ecx`), and the EH registration's pushes."""
    roles = {k: "save" for k, _ in saves_of(ins)}
    k = 0
    while k < len(ins) and ins[k].mnem == "push" and ins[k].ops in ("eax", "ecx", "edx"):
        roles[k] = "alloc"
        k += 1
    framed = next((k for k, i in enumerate(ins[:12]) if i.mnem == "mov" and i.ops == "ebp, esp"), None)
    end = next((k for k, i in enumerate(ins[:16]) if i.mnem == "mov" and i.ops.startswith("dword ptr fs:[0], ")),
               None)
    if end is not None:
        for j in range(framed + 1 if framed is not None else 0, end):
            if ins[j].mnem == "push" and j not in roles:
                roles[j] = "eh"
    return roles


def is_chkstk(ins, k):
    """`mov eax, N; call __chkstk` among the first instructions: an allocation of N bytes."""
    p = ins[k - 1] if k else None
    return (ins[k].mnem == "call" and p is not None and p.mnem == "mov" and p.ops.startswith("eax, 0x") and k < 12
            and (_imm(p.ops[5:]) or 0) >= 0x1000)


def saves_of(ins):
    """[(index, register)] of the pushes that save a callee-saved register: a push of ebx/esi/edi/ebp before the
    function writes it (in instruction order), whose value is the caller's."""
    written = set()
    out = []
    for k, i in enumerate(ins):
        if i.mnem == "push" and i.ops in SAVED and i.ops not in written:
            out.append((k, i.ops))
            written.add(i.ops)   # a second push of it is an argument
            continue
        dst = i.ops.split(",")[0].strip()
        if i.mnem in WRITES and reg32(dst) in SAVED:
            written.add(reg32(dst))
    return out


def track(ins, pops, table=None):
    """Set ins[k].delta (esp before the instruction, relative to entry) along the control flow. pops: {call index:
    bytes its callee pops}; table(ins) gives a switch jump's targets. Returns (frame size: the bytes allocated below
    the saves, ebp-framed)."""
    index = {i.addr: k for k, i in enumerate(ins)}
    flow = first_flow(ins)
    roles = push_roles(ins)
    ebp_frame = any(i.mnem == "mov" and i.ops == "ebp, esp" for i in ins[:12])
    ebp_at = [None]
    alloc = [0]

    def step(k, delta, pending):
        i = ins[k]
        m, o = i.mnem, i.ops
        if m == "push":
            n = 2 if o.startswith("word") or o in ("ax", "bx", "cx", "dx") else 4
            arg = k not in roles
            if roles.get(k) == "alloc":
                alloc[0] += n   # `push ecx`: a 4-byte slot allocated at entry
            return delta - n, pending + ([n] if arg else [])
        if m == "pop":
            return delta + 4, pending[:-1]
        if m == "pushal":
            return delta - 32, pending
        if m == "popal":
            return delta + 32, pending
        if m in ("sub", "add") and o.startswith("esp, "):
            n = _imm(o[5:])
            if n is None:
                return delta, pending
            if k < flow and m == "sub":
                alloc[0] += n
            if m == "add":
                left = n
                while pending and left > 0:
                    left -= pending.pop()
            return delta - n if m == "sub" else delta + n, pending
        if m == "mov" and o == "ebp, esp":
            ebp_at[0] = delta
            return delta, pending
        if m == "mov" and o.startswith("esp, ebp") and ebp_at[0] is not None:
            return ebp_at[0], []
        if m == "lea" and o.startswith("esp, ") and ebp_at[0] is not None:
            mm = MEM.search(o)
            if mm and mm.group(2).startswith("ebp"):
                return ebp_at[0] + (_disp(mm.group(2)) or 0), []
        if m == "call":
            if is_chkstk(ins, k):
                n = _imm(ins[k - 1].ops[5:])   # __chkstk allocates eax bytes
                alloc[0] += n
                return delta - n, pending
            if k in pops:
                n = pops[k]
            elif k + 1 < len(ins) and ins[k + 1].mnem == "add" and ins[k + 1].ops.startswith("esp, "):
                n = 0                       # cdecl: the caller's `add esp` follows
            else:
                n = sum(pending)            # the callee pops what was pushed for it
            left = n
            while pending and left > 0:
                left -= pending.pop()
            return delta + n, []
        return delta, pending

    seen = {}
    work = [(0, 0, ())]
    while work:
        k, delta, pending = work.pop()
        while k < len(ins):
            if k in seen:
                break
            seen[k] = delta
            i = ins[k]
            i.delta = delta
            i.ebp_frame = ebp_at[0]
            i.reached = True
            delta, p = step(k, delta, list(pending))
            pending = tuple(p)
            m = i.mnem
            if m in ("ret", "retn"):
                break
            if m.startswith("j") or m.startswith("loop"):
                t = _imm(i.ops)
                if t is not None and t in index:
                    work.append((index[t], delta, pending))
                elif m == "jmp" and "*4" in i.ops and table is not None:
                    work += [(index[t], delta, pending) for t in table(i) if t in index]
                if m == "jmp":
                    break
            k += 1
    last = 0
    for i in ins:   # unreached code (behind a jump table): carry the last known delta
        if i.delta is None:
            i.delta = last
            i.ebp_frame = ebp_at[0]
        last = i.delta
    return alloc[0], ebp_frame


def _disp(expr):
    """The constant part of a memory expression like `esp + 0x10` or `ebp - 4` (None with an index register)."""
    total = 0
    for sign, tok in re.findall(r"([+-]?)\s*(0x[0-9a-f]+|\d+)\b(?!\*)", expr.replace(" ", "")):
        n = int(tok, 16) if tok.startswith("0x") else int(tok)
        total += -n if sign == "-" else n
    return total


def access_of(ins, k, size):
    i = ins[k]
    m = i.mnem
    if m == "lea":
        dst = i.ops.split(",")[0].strip()
        nxt = ins[k + 1] if k + 1 < len(ins) else None
        if nxt is not None and nxt.mnem == "push" and nxt.ops == dst:
            return "&arg"
        if dst == "ecx":
            return "this"
        return "address"
    if m.startswith("f"):
        return "float" if size in (4, 8, 10, None) else "fpu"
    if m == "push":
        return "arg"
    return {1: "byte", 2: "word", 4: "int", 8: "qword"}.get(size, "int")


def collect_slots(ins):
    """{offset: Slot} of a function's frame, from esp- and ebp-based operands."""
    slots = {}
    for k, i in enumerate(ins):
        if not i.reached:
            continue
        for mm in MEM.finditer(i.ops):
            expr = mm.group(2)
            base = re.match(r"(esp|ebp)\b", expr)
            if not base or "fs:" in mm.group(0):
                continue
            if base.group(1) == "ebp":
                if i.ebp_frame is None:
                    continue
                off = i.ebp_frame + _disp(expr)
            else:
                d = i.delta
                if i.mnem == "push" and "[" in i.ops:
                    pass   # push [esp+x] reads before esp moves
                off = d + _disp(expr)
            size = SIZES.get(mm.group(1)) if mm.group(1) else None
            access = access_of(ins, k, size)
            if re.search(r"\b(e?[abcd]x|e[sd]i)\*?\d?\s*[+*]", expr.split(base.group(1), 1)[1]) or "*" in expr:
                access += "[i]"
            write = _is_write(i, mm)
            s = slots.setdefault(off, Slot(off))
            s.add(k, size, access, write)
            i.slots.append((off, size, access))
    return slots


def _is_write(i, mm):
    m = i.mnem
    first = i.ops.split(",")[0]
    if m.startswith("fst") or m.startswith("fist") or m.startswith("fnst") or m in ("fbstp",):
        return True
    if m in ("mov", "movzx", "movsx", "pop", "setne", "sete", "inc", "dec", "add", "sub", "and", "or", "xor", "shl",
             "shr", "neg", "not", "stosd", "rep stosd") and mm.group(0) in first:
        return m not in ("cmp", "test")
    return False


class Slot:
    def __init__(self, off):
        self.off = off
        self.sizes = Counter()
        self.access = Counter()
        self.first = None      # index of the first access
        self.first_write = None
        self.reads = 0
        self.writes = 0
        self.last = None
        self.written_as = Counter()   # the access kinds of the writes: "float" for fstp, "int" for mov

    def add(self, k, size, access, write):
        if write:
            self.written_as[access] += 1
        if size:
            self.sizes[size] += 1
        self.access[access] += 1
        if self.first is None:
            self.first = k
        if write and self.first_write is None:
            self.first_write = k
        self.last = k
        if write:
            self.writes += 1
        else:
            self.reads += 1

    @property
    def size(self):
        return self.sizes.most_common(1)[0][0] if self.sizes else None

    def kinds(self):
        return "/".join(a for a, _ in self.access.most_common(3))


def frame_of(ins, pops, table=None):
    alloc, ebp_frame = track(ins, pops, table)
    slots = collect_slots(ins)
    return Frame(ins, alloc, ebp_frame, slots)


class Frame:
    def __init__(self, ins, alloc, ebp_frame, slots):
        self.ins, self.alloc, self.ebp_frame, self.slots = ins, alloc, ebp_frame, slots
        self.saves = [r for _, r in saves_of(ins)]
        self.eh = sum(1 for r in push_roles(ins).values() if r == "eh")
        # the locals and spills lie between the saves (and EH registration) and the outgoing arguments
        self.top = -4 * (self.eh + (1 if ebp_frame else 0))
        self.bottom = -(4 * len(self.saves) + 4 * self.eh + alloc)
        # the EH state: the registration's first push (-1), right above the handler and the link
        self.state = (-8 if ebp_frame else -4) if self.eh else None
        self.state_stores = sum(s.writes for o, s in slots.items() if o == self.state)

    def locals(self):
        """The slots of the locals and spills: below the saved registers and the EH frame, above the outgoing
        arguments."""
        return {o: s for o, s in self.slots.items()
                if self.bottom <= o < self.top and "EH state" not in s.access}

    def outgoing(self):
        return sorted(o for o in self.slots if o < self.bottom)


# ---------------------------------------------------------------------------
# retail callee pops

_pop_memo = {}


def callee_pop(img, target):
    """Bytes the function at target pops on return (its first `ret N`), or None when it can't be read."""
    if target in _pop_memo:
        return _pop_memo[target]
    code = img.read(target, 0x1800) or b""
    n = None
    for a, m, o, sz in decomp.disasm_sized(code, target):
        if m in ("ret", "retn"):
            n = _imm(o) if o else 0
            break
        if m == "jmp" and not o.startswith("0x"):
            continue
        if m in ("int3",):
            break
    _pop_memo[target] = n
    return n


def symbol_pop(name):
    """Bytes a callee pops, from its decorated name where the name says: 0 for __cdecl (`_name`, `?f@@YA...`), N for
    a C __stdcall `_name@N`; None when the name doesn't say (__thiscall and C++ __stdcall: their argument bytes)."""
    if not name:
        return None
    name = name[6:] if name.startswith("__imp_") else name
    if name.startswith("?"):
        m = re.search(r"@@(?:[YCKS]|[AEIMQU][A-Z])([AEGI])", name)
        if m and m.group(1) == "A":
            return 0
        return None
    m = re.fullmatch(r"_[^@]+@(\d+)", name)
    if m:
        return int(m.group(1))
    return 0 if name.startswith("_") and "@" not in name else None


def retail_pops(img, ins):
    pops = {}
    imports = img.imports()
    for k, i in enumerate(ins):
        if i.mnem != "call":
            continue
        t = _imm(i.ops)
        if t is not None:
            n = callee_pop(img, t)
            if n is not None:
                pops[k] = n
        else:
            mm = re.fullmatch(r"dword ptr \[(0x[0-9a-f]+)\]", i.ops)
            if mm and int(mm.group(1), 16) in imports:
                continue   # an import: stdcall or cdecl, read from what follows the call
    return pops


# ---------------------------------------------------------------------------
# alignment and pairing

def align(ours, retail):
    """[(our index, retail index)] of aligned instructions: equal blocks of the normalised lists, and the pairs of
    replaced blocks of equal length."""
    a, b = [normal(i) for i in ours], [normal(i) for i in retail]
    pairs = []
    for op, i1, i2, j1, j2 in difflib.SequenceMatcher(None, a, b, autojunk=False).get_opcodes():
        if op == "equal" or (op == "replace" and i2 - i1 == j2 - j1):
            pairs += [(i1 + n, j1 + n, op == "equal") for n in range(i2 - i1)]
    return pairs


def pair_slots(of, rf, pairs):
    """({our offset: retail offset}, {our unpaired offset: the retail offset it shares}): votes of aligned
    instructions, then first-use order for the rest."""
    votes = defaultdict(Counter)
    for i, j, eq in pairs:
        so, sr = of.ins[i].slots, rf.ins[j].slots
        if len(so) == len(sr):
            for (o1, _, _), (o2, _, _) in zip(so, sr):
                votes[o1][o2] += 2 if eq else 1
    out = {}
    taken = set()
    for o1, c in sorted(votes.items(), key=lambda kv: -sum(kv[1].values())):
        for o2, _ in c.most_common():
            if o2 not in taken:
                out[o1] = o2
                taken.add(o2)
                break
    lo = sorted((s.first, o) for o, s in of.locals().items() if o not in out)
    lr = sorted((s.first, o) for o, s in rf.locals().items() if o not in taken)
    for (_, o1), (_, o2) in zip(lo, lr):
        if (of.slots[o1].size or 4) == (rf.slots[o2].size or 4):
            out[o1] = o2
            taken.add(o2)
    # an unpaired slot of ours whose instructions align with a retail slot another of ours took: retail keeps both
    # values in that one slot
    shared = {o1: c.most_common(1)[0][0] for o1, c in votes.items() if o1 not in out and o1 < 0}
    # a slot of ours whose instructions pair with several retail slots holds several values in turn, and retail
    # places them elsewhere: {our offset: [(retail offset, share)]}
    split = {}
    for o1, c in votes.items():
        total = sum(c.values())
        if len(c) > 1 and c.most_common(1)[0][1] * 4 < total * 3:
            split[o1] = [(o2, n * 100 // total) for o2, n in c.most_common(3)]
    pair_slots.split = split
    return out, shared


def pair_calls(ours, retail, pairs):
    return {i: j for i, j, _ in pairs if ours[i].mnem == "call" and retail[j].mnem == "call"}


# ---------------------------------------------------------------------------
# registers

class Value:
    """One value a callee-saved register holds: from the instruction that sets it to the next one."""

    def __init__(self, reg, start):
        self.reg, self.start, self.end = reg, start, start
        self.uses = 0
        self.name = None

    def span(self, ins):
        return "%X-%X" % (ins[self.start].addr, ins[min(self.end, len(ins) - 1)].addr)


def register_values(ins, frame):
    """{register: [Value]} for ebx/esi/edi (and ebp when it isn't the frame pointer)."""
    regs = [r for r in SAVED if not (r == "ebp" and frame.ebp_frame)]
    out = {r: [] for r in regs}
    save_idx = {k for k, _ in saves_of(ins)}
    for k, i in enumerate(ins):
        if k in save_idx:
            continue
        ops = [p.strip() for p in i.ops.split(",")] if i.ops else []
        if not ops:
            continue
        dst = reg32(ops[0]) if REG.fullmatch(ops[0]) else None
        if i.mnem == "pop" and dst in out:
            continue   # the epilogue's restore
        sets = dst in out and (i.mnem in PURE_WRITES or (i.mnem == "xor" and len(ops) == 2 and ops[0] == ops[1]))
        if sets:
            out[dst].append(Value(dst, k))
        for r in {reg32(x) for x in REG.findall(i.ops)}:
            if r in out and out[r] and not (sets and r == dst and i.mnem != "lea" and r not in " ".join(ops[1:])):
                out[r][-1].end = k
                out[r][-1].uses += 1
    return {r: v for r, v in out.items() if v}


def value_text(ins, v):
    i = ins[v.start]
    src = i.ops.split(",", 1)[1].strip() if "," in i.ops else ""
    if i.mnem == "xor":
        return "0"
    if i.mnem == "lea":
        return "&" + src
    return src


def pair_values(ov, rv, pairs, ours, retail):
    """{(our reg, value index): Counter((retail reg, value index): votes)} by the registers of aligned
    instructions."""
    def at(values, reg, k):
        for n, v in enumerate(values.get(reg, ())):
            if v.start <= k <= v.end:
                return n
        return None
    votes = defaultdict(Counter)
    for i, j, eq in pairs:
        ro = [reg32(x) for x in REG.findall(ours[i].ops)]
        rr = [reg32(x) for x in REG.findall(retail[j].ops)]
        if len(ro) != len(rr):
            continue
        for a, b in zip(ro, rr):
            if a in ov:
                n = at(ov, a, i)
                if n is not None:
                    m = at(rv, b, j) if b in rv else None
                    votes[(a, n)][(b, m)] += 2 if eq else 1
    return votes


# ---------------------------------------------------------------------------
# our side: the object, the diagnosis copy and its debug information

class Ours:
    pass


def find_entry(target):
    """(unit, entry) of an annotated function, by address or name."""
    rows = decomp.load_report()["functions"] if (decomp.BUILD / "report.json").exists() else []
    addr = int(target, 16) if target.lower().startswith("0x") else None
    cand = [r for r in rows if (addr is not None and int(r["addr"], 16) == addr) or
            (addr is None and (r["name"] == target or r["name"].split("::")[-1] == target))]
    units = decomp.source_files()
    tried = set()
    for r in cand + [None]:
        if r is None:
            scan_units = [u for u in units if u not in tried]
        elif r["unit"] in units:
            scan_units = [r["unit"]]
        else:
            continue
        for u in scan_units:
            tried.add(u)
            for e in decomp.scan(u, units[u]):
                if e.kind == "FUNCTION" and ((addr is not None and e.addr == addr) or
                                             (addr is None and e.name and (e.name == target or
                                                                           e.name.split("::")[-1] == target))):
                    return u, e
        if r is None:
            break
    for e in decomp.header_entries():
        if e.kind == "FUNCTION" and ((addr is not None and e.addr == addr) or e.name == target):
            hit = next((r for r in rows if int(r["addr"], 16) == e.addr and r["unit"] in units), None)
            if hit:
                return hit["unit"], e
    return None, None


def compile_copy(unit, path, out, debug):
    out.parent.mkdir(parents=True, exist_ok=True)
    if out.exists():
        out.unlink()
    args = ["/c", *decomp.cl_args(unit, path, pch=out.with_suffix(".pch")), *(["/Z7"] if debug else []),
            "/Fo" + str(out)]
    pch = decomp.fresh_pch(args)
    rc, text = vc6.run("cl", args, cwd=str(out.parent), compiler=decomp.compiler_of(path))
    decomp.drop_pch(pch)
    if rc != 0 or not out.exists():
        raise FramemapError("compile failed:\n" + text[-3000:])
    return out


def source_at(unit, rev, source):
    """The path to compile for a unit: its file, a revision of it, or another file. Copies sit beside the unit's own
    file under a hidden name, so quoted includes resolve as they do for the original."""
    path = decomp.source_files()[unit]
    if not rev and not source:
        return path, None
    text = Path(source).read_bytes() if source else subprocess.run(
        ["git", "show", "%s:%s" % (rev, path.relative_to(ROOT).as_posix())], cwd=ROOT, capture_output=True,
        check=True).stdout
    copy = path.with_name(".framemap-" + path.name)
    copy.write_bytes(text)
    return copy, text.decode("latin-1")


def raw_symbols(data):
    """[(index, name, value, section, storage, aux bytes)] including aux records, for .bf line numbers."""
    _, nsec, _, symptr, nsym, _, _ = struct.unpack_from("<HHIIIHH", data, 0)
    out = {}
    i = 0
    while i < nsym:
        raw = data[symptr + 18 * i:symptr + 18 * (i + 1)]
        value, secnum, typ, storage, naux = struct.unpack_from("<IhHBB", raw, 8)
        aux = data[symptr + 18 * (i + 1):symptr + 18 * (i + 1 + naux)]
        out[i] = (raw[:8], value, secnum, storage, aux)
        i += 1 + naux
    return out


def line_table(data, obj, sym):
    """[(offset in the function, source line)] from the COFF line numbers of the function's section."""
    _, nsec, _, symptr, nsym, opthdr, _ = struct.unpack_from("<HHIIIHH", data, 0)
    hdr = 20 + opthdr + 40 * (sym.section - 1)
    lnptr, nl = struct.unpack_from("<I", data, hdr + 28)[0], struct.unpack_from("<H", data, hdr + 34)[0]
    if not nl:
        return []
    syms = raw_symbols(data)
    out, base, func = [], None, None
    for k in range(nl):
        va, ln = struct.unpack_from("<IH", data, lnptr + 6 * k)
        if ln == 0:
            func = va
            base = None
            aux = syms[va][4]
            if len(aux) >= 4:
                bf = struct.unpack_from("<I", aux, 0)[0]   # TagIndex: the .bf symbol
                if bf in syms and len(syms[bf][4]) >= 6:
                    base = struct.unpack_from("<H", syms[bf][4], 4)[0]
            continue
        if func == sym.index and base is not None:
            out.append((va - sym.value, base + ln))
    return out


def debug_locals(obj, sym):
    """[(name, kind, where, type index)] of a function from the CodeView records of its .debug$S: ("bp", offset) for
    S_BPREL32, ("reg", register) for S_REGISTER, in record order (nested blocks flattened)."""
    for s in obj.sections:
        if s.name != ".debug$S":
            continue
        site = {off: obj.symbols[si].name for off, si, t in s.relocs if t == 0x0B}
        d = s.data
        o = 4 if len(d) >= 4 and struct.unpack_from("<I", d, 0)[0] in (1, 2) else 0
        out, inside = [], False
        depth = 0
        while o + 4 <= len(d):
            ln, ty = struct.unpack_from("<HH", d, o)
            body = d[o + 4:o + 2 + ln]
            if ty in (0x100A, 0x100B):   # S_LPROC32_ST, S_GPROC32_ST
                if inside:
                    depth += 1
                elif site.get(o + 4 + 28) == sym.name:
                    inside, depth = True, 0
            elif inside and ty == 0x0207:   # S_BLOCK32
                depth += 1
            elif inside and ty == 0x0006:   # S_END
                if depth == 0:
                    return out
                depth -= 1
            elif inside and ty == 0x1006:   # S_BPREL32_ST
                off, typ = struct.unpack_from("<iI", body, 0)
                out.append((_pascal(body, 8), "bp", off, typ))
            elif inside and ty == 0x1001:   # S_REGISTER_ST
                typ, reg = struct.unpack_from("<IH", body, 0)
                out.append((_pascal(body, 6), "reg", CV_REGS.get(reg, "r%d" % reg), typ))
            o += 2 + ln
        if inside:
            return out
    return []


PRIMITIVE = {0x10: ("char", 1), 0x20: ("unsigned char", 1), 0x11: ("short", 2), 0x21: ("unsigned short", 2),
             0x12: ("long", 4), 0x22: ("unsigned long", 4), 0x74: ("int", 4), 0x75: ("unsigned int", 4),
             0x30: ("bool", 1), 0x40: ("float", 4), 0x41: ("double", 8), 0x42: ("long double", 10),
             0x68: ("char", 1), 0x69: ("unsigned char", 1), 0x70: ("char", 1), 0x71: ("wchar_t", 2),
             0x72: ("short", 2), 0x73: ("unsigned short", 2), 0x03: ("void", 0)}


class Types:
    """Names and sizes of the CodeView types in a .debug$T (VC6's 32-bit-index leaves): enough to label a slot."""

    def __init__(self, data):
        self.recs = {}
        o = 4 if len(data) >= 4 and struct.unpack_from("<I", data, 0)[0] in (1, 2) else 0
        idx = 0x1000
        while o + 4 <= len(data):
            ln, leaf = struct.unpack_from("<HH", data, o)
            self.recs[idx] = (leaf, data[o + 4:o + 2 + ln])
            o += 2 + ln
            idx += 1
        self.full = {}
        for k, (leaf, b) in self.recs.items():
            if leaf in (0x1004, 0x1005, 0x1006):
                name, size, fwd = self._aggregate(leaf, b)
                if not fwd:
                    self.full.setdefault(name, size)

    @staticmethod
    def _numeric(b, at):
        v = struct.unpack_from("<H", b, at)[0]
        if v < 0x8000:
            return v, at + 2
        fmt = {0x8000: "<b", 0x8001: "<h", 0x8002: "<H", 0x8003: "<i", 0x8004: "<I"}.get(v)
        if fmt is None:
            return 0, at + 2
        return struct.unpack_from(fmt, b, at + 2)[0], at + 2 + struct.calcsize(fmt)

    def _aggregate(self, leaf, b):
        prop = struct.unpack_from("<H", b, 2)[0]
        at = 16 if leaf in (0x1004, 0x1005) else 8
        size, at = self._numeric(b, at)
        return _pascal(b, at), size, bool(prop & 0x80)

    def describe(self, t, depth=0):
        """(name, size) of a type index."""
        if t < 0x1000:
            if t & 0x700:
                base = PRIMITIVE.get(t & 0xFF, ("?", 4))[0]
                return base + " *", 4
            return PRIMITIVE.get(t, ("?", None))
        rec = self.recs.get(t)
        if rec is None or depth > 8:
            return "?", None
        leaf, b = rec
        if leaf in (0x1004, 0x1005, 0x1006):
            name, size, fwd = self._aggregate(leaf, b)
            return name, self.full.get(name, size) if fwd else size
        if leaf == 0x1002:   # LF_POINTER
            name, _ = self.describe(struct.unpack_from("<I", b, 0)[0], depth + 1)
            mode = (struct.unpack_from("<I", b, 4)[0] >> 5) & 7
            return name + (" &" if mode == 1 else " *"), 4
        if leaf == 0x1001:   # LF_MODIFIER
            return self.describe(struct.unpack_from("<I", b, 0)[0], depth + 1)
        if leaf == 0x1003:   # LF_ARRAY
            name, _ = self.describe(struct.unpack_from("<I", b, 0)[0], depth + 1)
            size, at = self._numeric(b, 8)
            return name + "[]", size
        if leaf == 0x1007:   # LF_ENUM
            return _pascal(b, 12), 4
        return "?", None


def _pascal(b, at):
    n = b[at] if at < len(b) else 0
    return b[at + 1:at + 1 + n].decode("latin-1")


def function_bytes(obj, sym):
    sec = obj.section(sym.section)
    start, end = obj.function_extent(sym)
    sites = [off - start for off, _, _ in sec.relocs if start <= off < end]
    return sec.data[start:end], sites


def table_disp(i):
    mm = re.search(r"\*4(?: \+ (0x[0-9a-f]+|\d+))?\]", i.ops)
    return (_imm(mm.group(1)) if mm.group(1) else 0) if mm else None


def retail_table(img, addr, size):
    """A switch jump's targets in retail: the table's dwords while they point into the function."""
    def targets(i):
        t, out = table_disp(i), []
        while t is not None and len(out) < 512:
            v = img.dword(t + 4 * len(out))
            if v is None or not addr <= v < addr + size:
                break
            out.append(v)
        return out
    return targets


def our_table(code, start, addr, bases):
    """A switch jump's targets in ours: the table sits in the function's section, its dwords section offsets.
    bases: {relocation site (function offset): the section offset of its symbol} for the table's label."""
    def targets(i):
        t, out = table_disp(i), []
        site = next((b for off, b in bases.items() if i.addr - addr <= off < i.addr - addr + i.size), None)
        if t is not None and site is not None:
            t += site
        while t is not None and len(out) < 512:
            at = t - start + 4 * len(out)
            if not 0 <= at <= len(code) - 4:
                break
            v = struct.unpack_from("<I", code, at)[0] + bases.get(at, 0)   # each entry is a relocation itself
            if not start <= v < start + len(code):
                break
            out.append(addr + v - start)
        return out
    return targets


class FramemapError(Exception):
    pass


_compiled = {}   # (unit, rev, source) -> (plain Coff, debug Coff, debug bytes, source text): one compile per run


def compiled(unit, rev, source):
    key = (unit, rev, source)
    if key in _compiled:
        return _compiled[key]
    path, text = source_at(unit, rev, source)
    try:
        OUT.mkdir(parents=True, exist_ok=True)
        base = OUT / ("%s.%d" % (unit.replace("/", "_"), os.getpid()))   # one name per run: runs can overlap
        dbg = compile_copy(unit, path, base.with_suffix(".z7.obj"), True)
        if rev or source:
            plain = compile_copy(unit, path, base.with_suffix(".obj"), False)
        else:
            _, plain, err = decomp.compile_unit(unit, path, decomp.BUILD / "obj")
            if plain is None:
                raise FramemapError("compile failed:\n" + err[-3000:])
    finally:
        if path.name.startswith(".framemap-"):
            path.unlink()
    data = dbg.read_bytes()
    _compiled[key] = (coff.Coff(plain.read_bytes()), coff.Coff(data), data, text)
    dbg.unlink()
    if rev or source:
        plain.unlink()
    return _compiled[key]


def load_ours(target, rev=None, source=None):
    unit, entry = find_entry(target)
    if entry is None:
        raise FramemapError("no annotated function %s" % target)
    o = Ours()
    o.unit, o.entry, o.warnings = unit, entry, []
    pobj, dobj, data, text = compiled(unit, rev, source)
    psym, err = decomp.find_symbol(pobj, entry)
    dsym, err2 = decomp.find_symbol(dobj, entry)
    if psym is None or dsym is None:
        raise FramemapError(err or err2)
    o.code, o.sites = function_bytes(pobj, psym)
    o.start = pobj.function_extent(psym)[0]
    o.call_names = {off - o.start: pobj.symbols[si].name for off, si, _ in pobj.section(psym.section).relocs
                    if o.start <= off < o.start + len(o.code)}
    sec = pobj.section(psym.section)
    o.bases = {off - o.start: pobj.symbols[si].value for off, si, _ in sec.relocs
               if o.start <= off < o.start + len(o.code) and pobj.symbols[si].section == psym.section}
    o.align = pobj.section(psym.section).alignment
    dcode, dsites = function_bytes(dobj, dsym)
    if dcode != o.code:
        o.warnings.append("the /Z7 diagnosis copy's code differs from the gated object's (%d against %d bytes): its "
                          "names follow its own layout and may be off" % (len(dcode), len(o.code)))
    o.locals = debug_locals(dobj, dsym)
    o.types = Types(next((x.data for x in dobj.sections if x.name == ".debug$T"), b""))
    o.lines = line_table(data, dobj, dsym)
    if text is None or entry.path != decomp.source_files()[unit]:
        text = decomp.source_text(entry.path)   # the unit's file, or the header the function is defined in
    lines = text.splitlines()
    o.source_lines = lines
    o.body = function_body(lines, entry)
    o.rev = rev or source
    return o


def function_body(lines, entry):
    """{line number: text} of the function's definition, found again by its name (the line moves with a revision)."""
    name = (entry.name or "").split("::")[-1]
    start = entry.line
    if name:
        for k, l in enumerate(lines):
            if re.search(r"\b%s\s*\(" % re.escape(name), l) and not l.strip().startswith(("//", "*")) \
                    and not l.rstrip().endswith(";") and abs(k + 1 - entry.line) < 400 + abs(len(lines) - len(
                        entry.lines)) and (k == 0 or "FUNCTION:" in lines[k - 1] or "FUNCTION:" in " ".join(
                            lines[max(0, k - 4):k])):
                start = k + 1
                break
    out = {}
    depth, opened = 0, False
    for k in range(start - 1, len(lines)):
        out[k + 1] = lines[k]
        depth += lines[k].count("{") - lines[k].count("}")
        opened = opened or "{" in lines[k]
        if opened and depth <= 0:
            break
    return out


DECL = re.compile(r"(?:^\s*|[;{(,]\s*)((?:const\s+|unsigned\s+|signed\s+|struct\s+|class\s+)*[A-Za-z_][\w:<>]*"
                  r"(?:\s*[*&]+\s*|\s+)(?:const\s+)?[*&]*\s*)([A-Za-z_]\w*)\s*(?=[=;\[,)(])")
KEYWORDS = {"return", "delete", "new", "else", "goto", "case", "sizeof", "typedef", "throw"}


def declarations(body):
    """{name: (type, line, depth)} of the locals and parameters declared in a function's text; depth 0 for the
    parameters, 1 for function scope, more for blocks."""
    out = {}
    depth = 0
    for n, line in sorted(body.items()):
        code = re.sub(r"//.*|/\*.*?\*/|\"(?:\\.|[^\"])*\"", "", line)
        pos = 0
        for m in DECL.finditer(code):
            typ, name = m.group(1).strip(), m.group(2)
            t0 = typ.split()[0]
            if t0 in KEYWORDS or name in KEYWORDS or t0 in ("if", "while", "for", "switch") or name in out:
                continue
            here = depth + code[:m.start(2)].count("{") - code[:m.start(2)].count("}")
            out[name] = (typ, n, max(here, 0))
            pos = m.end()
        depth += code.count("{") - code.count("}")
    return out


# ---------------------------------------------------------------------------
# naming our side

def line_at(o, offset):
    k = bisect.bisect_right([a for a, _ in o.lines], offset) - 1
    return o.lines[k][1] if k >= 0 else None


class Var:
    def __init__(self, name, off, typ, size, decl):
        self.name, self.off, self.typ, self.size, self.decl = name, off, typ, size, decl

    @property
    def scope(self):
        """"block, line n" for a local declared in a nested block, "function" at function scope, "parameter"."""
        if self.off > 0:
            return "parameter"
        if self.decl and self.decl[2] > 1:
            return "block, line %d" % self.decl[1]
        return "function"


def our_vars(o, of, decls):
    """[Var] of the frame from the BPREL records, made entry-relative (an ebp frame's offsets are from ebp)."""
    shift = -4 if of.ebp_frame else 0
    out = []
    for nm, kind, where, typ in o.locals:
        if kind != "bp":
            continue
        tname, size = o.types.describe(typ)
        out.append(Var(nm, where + shift, tname, size or 4, decls.get(nm)))
    return out


def label(vars_, off):
    """(text, Var) naming a frame offset of ours: the variable whose extent holds it."""
    hits = [v for v in vars_ if v.off <= off < v.off + max(v.size, 1)]
    if not hits:
        return "", None
    names = "/".join(v.name for v in hits if v.off == off)
    if names:
        return names, next(v for v in hits if v.off == off)
    v = max(hits, key=lambda v: v.off)
    return "%s+0x%X" % (v.name, off - v.off), v


def name_value(o, ins, v, decls, vars_):
    """(name, line) of the local a register value stands for: one assigned on the source line of the instruction
    that sets it, `this` copied from ecx at entry, a constant, a reload of a named slot, else the line's text."""
    i = ins[v.start]
    n = line_at(o, i.addr - ins[0].addr)
    text = o.source_lines[n - 1].strip() if n and n - 1 < len(o.source_lines) else ""
    src = i.ops.split(",", 1)[1].strip() if "," in i.ops else ""
    if i.mnem == "mov" and src == "ecx" and "this" in [x[0] for x in o.locals] and not any(
            reg32(j.ops.split(",")[0].strip()) == "ecx" and j.mnem in WRITES for j in ins[:v.start]) and not any(
            j.mnem == "call" for j in ins[:v.start]):
        return "this", n
    for name in sorted(decls, key=len, reverse=True):
        if re.search(r"(?<![\w.>])%s\s*(=[^=]|\+\+|--|\+=|-=|\|=|&=)" % re.escape(name), text) or \
                re.search(r"(\+\+|--)\s*%s\b" % re.escape(name), text):
            return name, n
    if i.mnem == "xor" or (i.mnem == "mov" and re.fullmatch(r"-?0x[0-9a-f]+|\d+", src)):
        return "constant %s" % ("0" if i.mnem == "xor" else src), n
    mm = MEM.search(src)
    if mm and re.match(r"(esp|ebp)\b", mm.group(2)) and i.slots:
        lab, _ = label(vars_, i.slots[0][0])
        if lab:
            return "reload of %s" % lab, n
    reg_locals = [nm for nm, kind, where, _ in o.locals if kind == "reg" and where == v.reg]
    if reg_locals:
        return reg_locals[0], n
    if text and text not in ("{", "}"):
        return "`%s`" % (text[:34] + ("..." if len(text) > 34 else "")), n
    return "?", n


# ---------------------------------------------------------------------------
# the report

def signed(off):
    return "%s0x%X" % ("-" if off < 0 else "+", abs(off))


def build(target, rev=None, source=None):
    o = load_ours(target, rev, source)
    img = decomp.pe.Image(decomp.target_image())
    addr = o.entry.addr
    rows = decomp.load_report()["functions"] if (decomp.BUILD / "report.json").exists() else []
    starts = sorted({int(r["addr"], 16) for r in rows})
    k = bisect.bisect_right(starts, addr)
    stop = starts[k] if k < len(starts) else addr + 4 * len(o.code) + 256
    rcode = img.read(addr, max(len(o.code), min(stop - addr, 4 * len(o.code) + 256))) or b""
    if o.align < 16:
        rcode = rcode[:len(o.code)]   # a packed section (the CRT library): no fill marks retail's end
    retail = disassemble(rcode, addr)
    ours = disassemble(o.code, addr, o.sites)
    pairs = align(ours, retail)
    rpops = retail_pops(img, retail)
    rf = frame_of(retail, rpops, retail_table(img, addr, len(rcode)))
    calls = pair_calls(ours, retail, pairs)
    opops = {k: n for k, i in enumerate(ours) if i.mnem == "call"
             for n in [symbol_pop(o.call_names.get(i.addr - addr + 1 if i.size == 5 else i.addr - addr + 2))]
             if n is not None}
    opops.update({i: rpops[j] for i, j in calls.items() if j in rpops})
    of = frame_of(ours, opops, our_table(o.code, o.start, addr, o.bases))
    return o, ours, retail, of, rf, pairs


def refine_pairs(slotmap, vars_, of, rf):
    """Pair the inside of an aggregate by its base: a member of ours goes with the same member of retail's copy."""
    taken = {r: o for o, r in slotmap.items()}
    for v in vars_:
        if v.size <= 4 or v.off not in slotmap:
            continue
        rb = slotmap[v.off]
        for off in list(of.locals()):
            if v.off < off < v.off + v.size:
                want = rb + off - v.off
                if want in rf.slots and slotmap.get(off) != want:
                    other = taken.get(want)
                    if other is not None and other != off:
                        del slotmap[other]
                    old = slotmap.get(off)
                    if old is not None:
                        taken.pop(old, None)
                    slotmap[off] = want
                    taken[want] = off
    return slotmap


def copy_source(rf, roff):
    """For a slot of retail's whose first write stores a register just loaded from another frame slot: that slot."""
    s = rf.slots.get(roff)
    if s is None or s.first_write is None:
        return None
    ins = rf.ins
    w = ins[s.first_write]
    if w.mnem != "mov" or not REG.fullmatch(w.ops.split(",")[-1].strip()):
        return None
    reg = reg32(w.ops.split(",")[-1].strip())
    for k in range(s.first_write - 1, max(-1, s.first_write - 4), -1):
        i = ins[k]
        if i.mnem == "mov" and reg32(i.ops.split(",")[0].strip()) == reg:
            return i.slots[0][0] if i.slots else None
    return None


class Analysis:
    pass


def analyse(target, rev=None, source=None):
    o, ours, retail, of, rf, pairs = build(target, rev, source)
    a = Analysis()
    a.o, a.ours, a.retail, a.of, a.rf, a.pairs = o, ours, retail, of, rf, pairs
    a.decls = declarations(o.body)
    for nm, kind, where, typ in o.locals:
        a.decls.setdefault(nm, (o.types.describe(typ)[0], 0, 1))
    a.vars = our_vars(o, of, a.decls)
    slotmap, shared_r = pair_slots(of, rf, pairs)
    a.split = pair_slots.split
    a.slotmap = refine_pairs(slotmap, a.vars, of, rf)
    back = {v: k for k, v in a.slotmap.items()}
    olocals, rlocals = of.locals(), rf.locals()

    def describe(s):
        if s is None:
            return ""
        return "%s%s r%d w%d" % (s.kinds(), " %db" % s.size if s.size else "", s.reads, s.writes)

    rows = []
    a.only_r, a.only_o, a.shared_ours, a.shared_retail, a.copies = [], [], [], [], []
    a.write_kinds = []
    a.merges = []   # (ours X, ours Y, retail slot): some of X's values sit in the retail slot of Y
    for roff in sorted(rlocals):
        ooff = back.get(roff)
        lab, var = label(a.vars, ooff) if ooff is not None else ("", None)
        note = []
        if ooff is None:
            note.append("retail has one more %d-byte slot here" % (rlocals[roff].size or 4))
            src = copy_source(rf, roff)
            if src is not None and src in back:
                slab, _ = label(a.vars, back[src])
                if slab:
                    note.append("written once from %s: a copy of it" % slab)
                    a.copies.append((roff, slab))
            a.only_r.append(roff)
        elif ooff != roff:
            note.append("ours at %s" % signed(ooff))
        s_o = of.slots.get(ooff) if ooff is not None else None
        if s_o is not None and rlocals[roff].written_as and s_o.written_as and                 set(rlocals[roff].written_as) != set(s_o.written_as):
            wr, wo = "/".join(sorted(rlocals[roff].written_as)), "/".join(sorted(s_o.written_as))
            note.append("written as %s in retail, as %s in ours" % (wr, wo))
            a.write_kinds.append((lab or signed(ooff), wr, wo))
        if ooff in a.split:
            note.append("holds several values: they pair with retail %s" % ", ".join(
                "%s %d%%" % (signed(r), n) for r, n in a.split[ooff]))
            for r, n in a.split[ooff]:
                other = back.get(r)
                olab = label(a.vars, other)[0] if other is not None and other != ooff else ""
                if olab and lab and "+" not in olab and "+" not in lab:
                    a.merges.append((lab, olab, signed(r)))
        rows.append([roff, ooff, lab, var, describe(rlocals[roff]), describe(s_o), note])
    for ooff in sorted(set(olocals) - set(back.values())):
        lab, var = label(a.vars, ooff)
        note = ["ours has one more %d-byte slot here" % (olocals[ooff].size or 4)]
        if ooff in shared_r and shared_r[ooff] in back:
            other, _ = label(a.vars, back[shared_r[ooff]])
            other = other or signed(back[shared_r[ooff]])
            note.append("retail keeps it in %s with %s" % (signed(shared_r[ooff]), other))
            a.shared_retail.append((lab or signed(ooff), other, shared_r[ooff]))
        a.only_o.append((ooff, lab))
        rows.append([None, ooff, lab, var, "", describe(olocals[ooff]), note])
    for row in rows:
        if row[1] is not None:
            hits = [v.name for v in a.vars if v.off == row[1]]
            if len(hits) > 1:
                row[6].append("ours shares this slot between %s" % " and ".join(hits))
                a.shared_ours.append(hits)

    # declaration-order inversions among the named variables both sides place
    named = [(v, a.slotmap[v.off]) for v in a.vars if v.off < 0 and v.off in a.slotmap]
    a.inversions = []
    for x in range(len(named)):
        for y in range(x + 1, len(named)):
            (v1, r1), (v2, r2) = named[x], named[y]
            if v1.off != r1 and v2.off != r2 and (v1.off - v2.off) * (r1 - r2) < 0:
                a.inversions.append((v1.name, v2.name))
    for v1, v2 in a.inversions:
        for row in rows:
            if row[2] in (v1, v2):
                row[6].append("swapped with %s" % (v2 if row[2] == v1 else v1))
    a.rows = rows

    # callee-saved registers
    a.rvals, a.ovals = register_values(retail, rf), register_values(ours, of)
    vmap = pair_values(a.ovals, a.rvals, pairs, ours, retail)
    groups = {}
    for reg in SAVED:
        for n, v in enumerate(a.ovals.get(reg, ())):
            v.name, v.line = name_value(o, ours, v, a.decls, a.vars)
            g = groups.setdefault((reg, v.name), {"spans": 0, "uses": 0, "lines": set(), "retail": Counter()})
            g["spans"] += 1
            g["uses"] += v.uses
            if v.line:
                g["lines"].add(v.line)
            for (rreg, _), votes in vmap.get((reg, n), {}).items():
                g["retail"][rreg] += votes
    a.regs = groups
    a.reg_swaps = []
    for (reg, name), g in groups.items():
        if g["retail"]:
            best, votes = g["retail"].most_common(1)[0]
            if best != reg and votes * 10 >= 6 * sum(g["retail"].values()) and not name.startswith(("`", "?")):
                a.reg_swaps.append((name, reg, best))
    a.reading = read_pattern(a)
    return a


def read_pattern(a):
    """The one-line reading: a pattern of the levers, stated only when the tables show it."""
    rf, of = a.rf, a.of
    if rf.alloc > of.alloc:
        if a.shared_ours:
            x, y = a.shared_ours[0][:2]
            return ("retail's frame is 0x%X larger, and ours shares one slot between %s and %s where retail keeps "
                    "two: in the original their scopes overlap (one of them is declared at function scope or in an "
                    "enclosing block)" % (rf.alloc - of.alloc, x, y))
        if a.copies:
            roff, src = a.copies[0]
            return ("one extra slot: retail copies %s into a local of its own (%s) and reads that: a named local "
                    "the original has and ours lacks" % (src, signed(roff)))
        if a.only_r:
            first = a.rf.slots[a.only_r[0]]
            if any(k in first.access for k in ("address", "&arg", "this")):
                return ("retail's frame is 0x%X larger, and retail takes the address of a slot ours lacks (%s): an "
                        "object of up to 0x%X bytes (a local or a temporary passed by address) the original has"
                        % (rf.alloc - of.alloc, signed(a.only_r[0]), rf.alloc - of.alloc))
            return ("one extra slot: retail has a %d-byte local at %s that ours lacks (a local retail has, or one "
                    "ours keeps in a register)" % (first.size or 4, signed(a.only_r[0])))
        return ("retail reserves 0x%X more bytes and no instruction reads or writes them: locals the original "
                "declared and never used (or whose only use was optimised away)" % (rf.alloc - of.alloc))
    if rf.alloc < of.alloc:
        if a.shared_retail:
            x, y, at = a.shared_retail[0]
            return ("retail keeps %s and %s in one slot (%s) where ours gives each its own: in the original their "
                    "lifetimes do not overlap (one declared in the block that uses it), or one is a register "
                    "variable spilled there" % (y, x, signed(at)))
        if a.only_o:
            off, lab = a.only_o[0]
            return ("ours has one more slot (%s): retail keeps that value in a register, does not have it, or "
                    "shares its slot with another local" % (lab or signed(off)))
        return ("ours reserves 0x%X more bytes and no instruction of ours reads or writes them: a local ours "
                "declares and never uses, which retail lacks" % (of.alloc - rf.alloc))
    if a.write_kinds:
        lab, wr, wo = a.write_kinds[0]
        if "float" in (wr + wo):
            return ("same frame; %s is written as %s in retail and as %s in ours: one side copies "
                    "the value through the FPU (fld/fstp), the other with integer moves (walls.md, FPU copies)"
                    % (lab, wr, wo))
        return "same frame; %s is written as %s in retail and as %s in ours" % (lab, wr, wo)
    if a.merges:
        x, y, at = a.merges[0]
        return ("retail keeps some of %s's values in the slot of %s (%s): in the original they are one variable "
                "reused (or locals whose scopes let them share a slot)" % (x, y, at))
    split_names = {r[2] for r in a.rows if r[1] in a.split}
    if a.inversions and not split_names & set(a.inversions[0]):
        return "two slots swapped: declaration order of %s and %s" % a.inversions[0]
    if a.split:
        return ("same frame size; %d slot(s) of ours hold several values in turn that retail places in other slots "
                "(see the notes): which values share a slot differs, so scope or the reuse of one variable across "
                "loops, not declaration order" % len(a.split))
    if a.inversions:
        return "two slots swapped: declaration order of %s and %s" % a.inversions[0]
    moved = [r for r in a.rows if r[0] is not None and r[1] is not None and r[0] != r[1]]
    if len(moved) > 1 and len({r[0] - r[1] for r in moved}) == 1 and not (a.only_r or a.only_o):
        names = ", ".join(r[2] or signed(r[1]) for r in moved)
        return ("same frame size; %d slots (%s) sit 0x%X bytes %s in retail together: their block is laid out "
                "differently (one array where ours has separate locals, or a local more or less beside them)"
                % (len(moved), names, abs(moved[0][0] - moved[0][1]), "lower" if moved[0][0] < moved[0][1]
                   else "higher"))
    if moved or a.only_r or a.only_o:
        return "same frame size, %d slot(s) at other offsets and %d unpaired: see the table" % (
            len(moved), len(a.only_r) + len(a.only_o))
    if rf.state_stores != of.state_stores:
        return ("same frame; the EH state is stored %d times in retail and %d in ours: an object with a destructor "
                "(a local or a temporary) lives over another stretch of code, or one is constructed in only one of them"
                % (rf.state_stores, of.state_stores))
    if a.reg_swaps:
        return "same frame, register roles swapped: no frame evidence (%s)" % "; ".join(
            "ours keeps %s in %s, retail in %s" % s for s in a.reg_swaps[:3])
    if rf.saves != of.saves:
        return "same frame and register roles; the push order differs (retail %s, ours %s)" % (
            " ".join(rf.saves), " ".join(of.saves))
    return ("frame and callee-saved registers agree: the difference is in the scratch registers, the schedule or "
            "an expression")


def report(target, rev=None, source=None):
    a = analyse(target, rev, source)
    o, rf, of = a.o, a.rf, a.of
    out = ["framemap %s (0x%X, unit %s)%s" % (o.entry.name, o.entry.addr, o.unit, " at %s" % o.rev if o.rev else "")]
    out += ["warning: " + w for w in o.warnings]
    out.append("reading: " + a.reading)
    fr = "frame: locals 0x%X retail, 0x%X ours" % (rf.alloc, of.alloc)
    if rf.alloc != of.alloc:
        fr += " (retail %s by 0x%X)" % ("larger" if rf.alloc > of.alloc else "smaller", abs(rf.alloc - of.alloc))
    if rf.ebp_frame or of.ebp_frame:
        fr += "; ebp frame: retail %s, ours %s" % ("yes" if rf.ebp_frame else "no", "yes" if of.ebp_frame else "no")
    if rf.eh or of.eh:
        fr += "; EH state stores: retail %d, ours %d" % (rf.state_stores, of.state_stores)
    out.append(fr)
    out.append("")
    out.append("%-7s %-7s %-20s %-18s %-22s %-22s %s" % ("retail", "ours", "our variable", "type, scope",
                                                         "retail access", "our access", "note"))
    for roff, ooff, lab, var, ra, oa, note in sorted(a.rows, key=lambda r: r[0] if r[0] is not None else r[1]):
        ts = ""
        if var is not None and lab == var.name:
            ts = var.typ + ("" if var.scope == "function" else ", " + var.scope.replace(", line ", " "))
        out.append("%-7s %-7s %-20s %-18s %-22s %-22s %s" % (
            signed(roff) if roff is not None else "-", signed(ooff) if ooff is not None else "-", lab[:20], ts[:18],
            ra[:22], oa[:22], "; ".join(note)))
    params = [v for v in a.vars if v.off > 0]
    if params:
        out.append("parameter slots: " + ", ".join("%s %s" % (signed(v.off), v.name) for v in params))
    n_out = (len(rf.outgoing()), len(of.outgoing()))
    if any(n_out):
        out.append("outgoing-argument slots left out: %d retail, %d ours" % n_out)
    out.append("")
    out.append("callee-saved registers: push order retail %s; ours %s" % (" ".join(rf.saves) or "-",
                                                                       " ".join(of.saves) or "-"))
    out.append("%-5s %-38s %-6s %-5s %-12s %s" % ("ours", "value (our variable)", "spans", "uses", "lines",
                                                  "retail register (aligned votes)"))
    for (reg, name), g in sorted(a.regs.items(), key=lambda kv: (SAVED.index(kv[0][0]), min(kv[1]["lines"] or {0}))):
        lines = sorted(g["lines"])
        ltxt = ",".join(str(x) for x in lines[:3]) + ("..." if len(lines) > 3 else "")
        votes = g["retail"]
        total = sum(votes.values())
        rtxt = ", ".join("%s %d%%" % (r, 100 * c // total) for r, c in votes.most_common(2)) if total else "-"
        out.append("%-5s %-38s %-6d %-5d %-12s %s" % (reg, name[:38], g["spans"], g["uses"], ltxt, rtxt))
    for name, reg, best in a.reg_swaps:
        out.append("swap: ours keeps %s in %s, retail in %s" % (name, reg, best))
    out.append("")
    out.append("access: kinds (int, byte, word, float, address = address taken, &arg = address passed, this = "
               "address in ecx, arg = pushed), size, reads r and writes w; scope: the line of a block-scoped "
               "declaration")
    return "\n".join(out)


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("function", help="address (0x...) or name of an annotated function")
    ap.add_argument("--rev", help="compile the unit's file as it was at this git revision (headers stay current)")
    ap.add_argument("--source", help="compile the unit from this file instead")
    args = ap.parse_args(argv)
    import funcsource
    try:
        with funcsource.CheckoutLock():   # the gated object is recompiled into build/obj, as `match` does
            text = report(args.function, args.rev, args.source)
        print(text)
    except FramemapError as e:
        sys.exit(str(e))
    return 0


if __name__ == "__main__":
    sys.exit(main())
