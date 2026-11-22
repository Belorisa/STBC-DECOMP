"""Which compiler decision differs in a function that doesn't match: the wall-identifier skill's first routing step,
read mechanically from the two disassemblies.

`decomp.py build` and `match` call `classify` for every function short of exact and store the class in the row
("wall", with a one-line "wall_note"); `openwork.py walls` groups the open functions by it and the packet shows it.

The classes, in the skill's routing order (a later class is named only when the earlier ones agree):

    inline         the out-of-line call multiset differs (an inline boundary, an incomplete body, another callee)
    control-flow   the calls agree; the branch and return sequence differs (a source construct)
    referent       the instructions agree once addresses are masked: an identity, not codegen
    register       everything above agrees: operand order, spills, colouring or instruction choice differ; the note
                   names a frame-size difference, which `tools/framemap.py` reads slot by slot

The class says where to look first, not that the wall is bounded: "register" is reached here by elimination over
the instructions only, and the skill's source-shape checklist still comes before calling it residue. A worker's own
verdict on the function's card (`cards.py add --wall`, which also has "structure" and "compiler-state")
outranks this one.
"""

import re
from collections import Counter

ADDRESS = re.compile(r"0x[0-9a-f]{5,8}")
REGISTERS = re.compile(r"\b(?:e?[abcd]x|[abcd][lh]|e?[sd]i|e?[sb]p|st\(\d\)|st)\b")
FILL = ("nop", "int3")


def cut_at_end(ins, code, addr, align=16):
    """The instructions up to the function's end: the first `ret` or `jmp` followed by fill bytes that run to the
    next alignment boundary (retail is read past the function when ours is shorter, and a jump table after the last
    return disassembles as junk on both sides)."""
    for k, (a, mnem, ops, size) in enumerate(ins):
        if mnem not in ("ret", "retn", "jmp"):
            continue
        end = a + size - addr
        pad = (-(addr + end)) % align
        if pad and end + pad <= len(code) and all(b in (0x90, 0xCC) for b in code[end:end + pad]):
            return ins[:k + 1]
    return ins


def instructions(disasm_sized, code, addr, unresolved=None):
    """[(mnemonic, operands)] of the function. A direct call of ours whose target didn't resolve (`unresolved`:
    {relocation site: symbol}) carries "?<symbol>" for operands: the bytes at its site are not an address."""
    ins = cut_at_end(disasm_sized(code, addr), code, addr)
    unresolved = unresolved or {}
    return [(m, "?" + unresolved[a + 1] if m == "call" and a + 1 in unresolved else o)
            for a, m, o, _ in ins if m not in FILL]


def call_key(ops):
    """A call's target: the address of a direct call, else its operand with the registers left out."""
    return ops if ADDRESS.fullmatch(ops) else REGISTERS.sub("R", ops)


def pair_unresolved(calls_o, calls_r):
    """Ours' calls without an address stand for retail's calls that nothing else in ours accounts for: when the two
    counts are equal the call sets agree as far as anyone can tell, and the open question is an identity."""
    unknown = Counter({k: n for k, n in calls_o.items() if k.startswith("?")})
    known = calls_o - unknown
    left = calls_r - known
    if unknown and not known - calls_r and sum(left.values()) == sum(unknown.values()):
        return calls_r
    return calls_o


def some(counter, limit=3):
    items = ["%s%s" % (k[1:] + " (no address yet)" if k.startswith("?") else k, " x%d" % n if n > 1 else "")
             for k, n in sorted(counter.items())]
    return ", ".join(items[:limit]) + (", ..." if len(items) > limit else "")


def frame_size(ins):
    """Bytes a function allocates for its locals: `push` of a scratch register at entry, and `sub esp, N` (or
    `mov eax, N` before __chkstk) ahead of the first call or branch, where VC6 schedules it."""
    size, k = 0, 0
    while k < len(ins) and ins[k][0] == "push" and ins[k][1] in ("eax", "ecx", "edx"):
        size += 4
        k += 1
    for m, o in ins[k:]:
        if m == "call" or m.startswith(("j", "ret", "loop")):
            break
        if m == "sub" and o.startswith("esp, "):
            n = o[5:]
            size += int(n, 16) if n.startswith("0x") else int(n) if n.isdigit() else 0
        if m == "mov" and o.startswith("eax, 0x") and int(o[5:], 16) >= 0x1000:
            size += int(o[5:], 16)
            break
    return size


def frame_note(ours, retail):
    """"; frame 0x.. in ours, 0x.. in retail" when the two allocate different frames: a local's slot or scope, the
    reading `tools/framemap.py` gives."""
    a, b = frame_size(ours), frame_size(retail)
    return "" if a == b else "; frame 0x%X in ours, 0x%X in retail (tools/framemap.py)" % (a, b)


def classify(ours, retail, problems=()):
    """(class, note) for two instruction lists [(mnemonic, operands)]."""
    calls_o = Counter(call_key(o) for m, o in ours if m == "call")
    calls_r = Counter(call_key(o) for m, o in retail if m == "call")
    calls_o = pair_unresolved(calls_o, calls_r)
    if calls_o != calls_r:
        only_o, only_r = calls_o - calls_r, calls_r - calls_o
        parts = []
        if only_r:
            parts.append("only retail calls %s" % some(only_r))
        if only_o:
            parts.append("only ours calls %s" % some(only_o))
        return "inline", "%d calls ours, %d retail: %s" % (sum(calls_o.values()), sum(calls_r.values()),
                                                           "; ".join(parts))

    def flow(ins):
        return [m for m, _ in ins if m.startswith(("j", "loop")) or m in ("ret", "retn")]
    flow_o, flow_r = flow(ours), flow(retail)
    if flow_o != flow_r:
        def counts(f):
            return "%d branches, %d returns" % (sum(1 for m in f if not m.startswith("ret")),
                                                sum(1 for m in f if m.startswith("ret")))
        if Counter(flow_o) == Counter(flow_r):
            k = next(i for i, (a, b) in enumerate(zip(flow_o, flow_r)) if a != b)
            return "control-flow", "the same branches in another order: number %d is %s in ours, %s in retail" % (
                k + 1, flow_o[k], flow_r[k])
        if len(flow_o) == len(flow_r):
            k = next(i for i, (a, b) in enumerate(zip(flow_o, flow_r)) if a != b)
            return "control-flow", "branch %d is %s in ours, %s in retail (%s)" % (
                k + 1, flow_o[k], flow_r[k], counts(flow_o))
        return "control-flow", "ours %s; retail %s" % (counts(flow_o), counts(flow_r))

    def masked(ins):
        return ["%s %s" % (m, "X" if o.startswith("?") else ADDRESS.sub("X", o)) for m, o in ins]
    text_o, text_r = masked(ours), masked(retail)
    if text_o == text_r:
        return "referent", (problems[0] if problems else "the instructions agree; an address or a constant differs")
    kinds_o = Counter(REGISTERS.sub("R", t) for t in text_o)
    kinds_r = Counter(REGISTERS.sub("R", t) for t in text_r)
    frame = frame_note(ours, retail)
    if kinds_o == kinds_r:
        return "register", "the same instructions with other registers or in another order (%d instructions)%s" % (
            len(text_o), frame)
    extra, missing = sum((kinds_o - kinds_r).values()), sum((kinds_r - kinds_o).values())
    return "register", ("calls and branches agree; %d instructions only in ours, %d only in retail (%d and %d in all): "
                        "an expression's spelling or the schedule%s" % (extra, missing, len(text_o), len(text_r),
                                                                         frame))
