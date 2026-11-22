"""The BC SDK's AnimOnly.exe as evidence: a debug build of NetImmerse 3.1 with its asserts.

    python tools/sdkdebug.py asserts [--file PAT] [--expr PAT]   # every assert: function, file, line, expression
    python tools/sdkdebug.py files [--all] [--objects]           # the source files' function runs, in link order
    python tools/sdkdebug.py map [--json FILE] [--pairs FILE]    # pair AnimOnly's functions with retail's and ours
    python tools/sdkdebug.py fn 0x426171                         # one AnimOnly function: file, asserts, pair, calls
    python tools/sdkdebug.py od [--unit REGEX]                   # our units at /Od against AnimOnly, function by function
    python tools/sdkdebug.py cmp 0x7F74B0 [--debug 0x51128D]     # one function at /Od, ours beside AnimOnly's copy
    python tools/sdkdebug.py search 0x7C3290 [--file RE | --range LO HI]   # the debug functions most like ours at /Od
    python tools/sdkdebug.py search --all-unpaired [--open-only]  # every unpaired NetImmerse function: search.json
                                                                 # (cmp, od, search, find: --no-mask-frame)
    python tools/sdkdebug.py find scratch.cpp [--flags F] [--only REGEX]   # a scratch file's /Od functions in AnimOnly
    python tools/sdkdebug.py --exe OTHER.exe rtti [--prefix RE]  # a /GR build's class hierarchy against our classes

AnimOnly.exe (`C:/Recomp/bcsdk-1.1/Tools/AnimOnly`, VC6 linker, stamped April 2001) is Totally Games' animation
viewer, linked against a DEBUG build of "NetImmerse Version 3.1", the engine version BC ships. Debug code is /Od: an
ebp frame in every function, every inline out of line, statements nearly literal. Its `assert` calls push the
expression, the `__FILE__` path (`C:/TGSharedExternal/NDL3/Src/NiMain/NiNode.cpp`, written with backslashes) and the line, so each assert
names the original source file of the function it sits in, and a debug link lays the functions out object by object,
in each object's definition order. The SDK binary is evidence only, read like the demo executable: never run, never
copied into the tree, its code never pasted. Its path comes from `--exe`, else the environment variable
STBC_ANIMONLY, else `"animonly"` in config/project.json, else the default below.

Functions. A start is the entry point, a target of the incremental-link thunk table, a `push ebp; mov ebp, esp` right
after a `ret`, `ret n` or `int3` fill (library code is packed), or a direct call target that is such a prologue or is
called from two sites. A function runs to the next start.

Asserts. `_assert` is the function most often called right after `push <line>; push <file>; push <expr>` whose file
operand is a path; each such call site gives (expression, file, line, containing function).

Files. A function's file is the `.cpp` (or `.c`) of its own asserts. A header's assert (`NiTArray.inl`) names where
an inline was written, not the object that emitted the copy, so it doesn't place a function. Between two functions of
the same file, every function belongs to it (one object's code is contiguous). `--objects` widens each run to its
object: back to the static initializers just before it (NiRTTI's), forward to the next initializer, run, or int3 fill.

Map. NetImmerse's own RTTI pairs the two builds: each class's static initializer calls `NiRTTI::NiRTTI(name, base)`
on the class's NiRTTI, its GetRTTI returns that NiRTTI's address, and a vtable is a run of code pointers whose first
slot is a GetRTTI (retail's from config/vtables.txt where listed). Both builds' vtables pair by class name, slot by
slot while both agree on which slots the class overrides; retail's slots name our annotated functions
(build/report.json). Then the CreateObject registrations (`push <creation function>; push "<class>"`), strings that
one function uses on each side, and, until nothing changes, calls (between two callees both builds pair, a lone
unpaired callee on each side) and definition order (between two pairs that are neighbours in one of our units, the
unpaired functions in order when both sides hold as many). The result is build/sdkdebug/map.json: the pairs with
their original file, our units against the original files, and member names from asserts (`m_pBuffer != NULL` right
after a test of one offset of `this`: the original's name for our member at that offset, by the debug layout).

/Od. `od` compiles our units with their own includes and defines under AnimOnly's debug flags (/Od /MLd /D_DEBUG; a
second compile adds /GZ, which the NiDirect3D and NiSurface libraries were built with) and looks for every function
of the objects, COMDAT inline copies included, in AnimOnly byte for byte with relocated fields masked
(build/sdkdebug/od.json). A /Od body keeps each statement's own instructions, every inline call and the frame layout of
named locals, so it pins spellings that compile to the same release bytes: a paired function whose /Od copy differs
from AnimOnly's has statements that differ from the original's, and `cmp` shows where. Our source has no asserts,
so a function with one never matches at /Od. `find` does the same for a scratch file (`#line N` before an assert
reproduces the original's line operand).

Comparisons. `cmp`, `od`'s differing pairs and `search` compare listings: callees dropped (the two builds name them
differently), jump targets and addresses blanked (a constant in read-only data shows its value, `[=90.0]`, but
compares as an address), listings cut at the first jump table, and frame slots masked (every ebp-based operand's
displacement: `[ebp - 0x10]` -> `[ebp X]`, `[ebp + eax*4 - 0x98]` -> `[ebp + eax*4 X]`), since a /Od frame orders
the named locals by a hash of their names and /GZ pads it. Two ratios come out: masked (the statements) and with
frame slots (the frame order, which the original's local names decide), the second along the first's alignment.
`search` finds the debug copy of a
function the map hasn't paired by ranking every debug function against our /Od body, best first over upper bounds
(instruction bags, then a bit-parallel longest common subsequence, then difflib's ratio), which gives the result of a
full scan; confirmed hits go in config/sdkdebug_pairs.txt, which `map` adds after its own rounds.

RTTI. `rtti` reads a /GR build's complete object locators and class hierarchy descriptors (the SDK's DamageTool.exe
and ModelPropertyEditor_s.exe; retail is built without /GR) and lists each class's direct bases against ours.
"""

import argparse
import bisect
import json
import math
import os
import re
import struct
import sys
from collections import Counter, defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import pe  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
BUILD = ROOT / "build"
OUT = BUILD / "sdkdebug"
DEFAULT_EXE = "C:/Recomp/bcsdk-1.1/Tools/AnimOnly/AnimOnly.exe"
RETAIL = ROOT / "orig" / "stbc.exe"
SOURCE_SUFFIX = (".cpp", ".c")


def exe_path(arg=None):
    if arg:
        return arg
    if os.environ.get("STBC_ANIMONLY"):
        return os.environ["STBC_ANIMONLY"]
    try:
        project = json.loads((ROOT / "config" / "project.json").read_text())
        if project.get("animonly"):
            return project["animonly"]
    except (OSError, ValueError):
        pass
    return DEFAULT_EXE


def rel32(code, i, va):
    """The target of an E8/E9 rel32 at code[i]."""
    return (va + 5 + struct.unpack_from("<i", code, i + 1)[0]) & 0xFFFFFFFF


def entry_point(img):
    p = struct.unpack_from("<I", img.data, 0x3C)[0]
    return img.base + struct.unpack_from("<I", img.data, p + 24 + 16)[0]


def cstring(img, va, limit=512):
    """The NUL-terminated string at `va`, or None if it isn't printable text."""
    b = img.read(va, 1)
    if b is None:
        return None
    off = img.offset(va)
    end = img.data.find(b"\0", off, off + limit)
    if end < 0:
        return None
    s = img.data[off:end]
    if not s or any(c < 0x20 and c not in (9, 10, 13) or c > 0x7E for c in s):
        return None
    return s.decode("latin-1")


SCN_MEM_WRITE = 0x80000000                                     # a COFF section's IMAGE_SCN_MEM_WRITE
PROLOGUE = re.compile(rb"\x55\x8B\xEC")                        # push ebp; mov ebp, esp
JUMP_TABLE = re.compile(rb"\xFF\x24[\x85\x8D\x95\x9D\xAD\xB5\xBD]")   # jmp dword ptr [reg*4 + disp32]


class Debug:
    """One debug image: its function starts, call sites, asserts and source-file runs."""

    def __init__(self, img):
        self.img = img
        _, self.tva, self.tsize, _, _ = img.text
        self.code = img.read(self.tva, min(img.text[2], img.text[4]))
        self.starts = self._starts()
        self.tables = self._tables()
        self.start_set = set(self.starts)
        self.calls = self._calls()
        self.assert_fn = None
        self.asserts = self._asserts()

    def in_text(self, va):
        return self.tva <= va < self.tva + len(self.code)

    def _starts(self):
        code, tva = self.code, self.tva
        starts = {entry_point(self.img)}
        # incremental-link thunk table: a run of E9 jumps at the start of .text
        i = 0
        while i < len(code) and code[i] == 0xCC:
            i += 1
        self.thunks = {}
        while i + 5 <= len(code) and code[i] == 0xE9:
            t = rel32(code, i, tva + i)
            if self.in_text(t):
                self.thunks[tva + i] = t
                starts.add(t)
            i += 5
        prologues = self.prologues = set()
        for m in PROLOGUE.finditer(code):
            i = m.start()
            prologues.add(tva + i)
            if i == 0 or code[i - 1] in (0xC3, 0xCC) or (i >= 3 and code[i - 3] == 0xC2):
                starts.add(tva + i)
        # an E8 byte may sit inside another instruction: a call target counts when it is a prologue, or when two
        # sites call it (a chance rel32 rarely lands on the same address twice)
        seen = Counter()
        for m in re.finditer(rb"\xE8", code):
            i = m.start()
            if i + 5 <= len(code):
                t = rel32(code, i, tva + i)
                if self.in_text(t):
                    seen[self.thunks.get(t, t)] += 1
        starts.update(t for t, n in seen.items() if t in prologues or n >= 2)
        return sorted(s for s in starts if self.in_text(s))

    def function_of(self, va):
        k = bisect.bisect_right(self.starts, va) - 1
        return self.starts[k] if k >= 0 else None

    def next_start(self, start):
        k = bisect.bisect_right(self.starts, start)
        return self.starts[k] if k < len(self.starts) else self.tva + len(self.code)

    def end_of(self, start):
        """The end of a function's instructions: the next start, or its first jump table where it has one."""
        end = self.next_start(start)
        t = self.tables.get(start)
        return t if t is not None and t < end else end

    def rodata(self, va, n):
        """The `n` bytes at `va` when they lie in read-only data (.rdata: constants and strings), else None."""
        sec = self.img.section_of(va)
        return self.img.read(va, n) if sec and sec[0] == ".rdata" else None

    def table_run(self, start, table):
        """The end of the jump tables at `table`: the run of relocated dwords that point back into the function."""
        p = table
        while p in self.img.relocs and start <= (self.img.dword(p) or 0) < table:
            p += 4
        return p

    def _tables(self):
        """{function start: its first jump table}. /Od puts a switch's tables after the function's last instruction:
        `jmp dword ptr [reg*4 + table]`, the table a run of relocated dwords that point back into the function, then
        any byte index tables. A prologue right after them starts a function the start scan missed (no int3 or ret
        before it): such starts are added until none is new."""
        code, tva, relocs = self.code, self.tva, self.img.relocs
        sites = []
        for m in JUMP_TABLE.finditer(code):
            i = m.start()
            t = struct.unpack_from("<I", code, i + 3)[0] if i + 7 <= len(code) else 0
            if tva + i + 3 in relocs and t in relocs and self.in_text(t) and t > tva + i:
                sites.append((tva + i, t))
        while True:
            tables = {}
            for site, t in sites:
                s = self.function_of(site)
                if s is None or t >= self.next_start(s) or not s <= (self.img.dword(t) or 0) < t:
                    continue
                if s not in tables or t < tables[s]:
                    tables[s] = t
            new = set()
            for s, t in tables.items():
                m = PROLOGUE.search(code, self.table_run(s, t) - tva, self.next_start(s) - tva)
                if m and tva + m.start() in self.prologues:
                    new.add(tva + m.start())
            if not new:
                return tables
            self.starts = sorted(set(self.starts) | new)

    def _calls(self):
        """{caller start: [callee start, ...]} in address order, direct E8 calls only, thunks resolved."""
        out = defaultdict(list)
        for m in re.finditer(rb"\xE8", self.code):
            i = m.start()
            if i + 5 > len(self.code):
                continue
            t = rel32(self.code, i, self.tva + i)
            t = self.thunks.get(t, t)
            if t in self.start_set:
                out[self.function_of(self.tva + i)].append((self.tva + i, t))
        return out

    def _assert_site(self, site):
        """(expression, file, line) pushed before the call at `site`, or None."""
        code, i = self.code, site - self.tva
        if i < 12 or code[i - 5] != 0x68 or code[i - 10] != 0x68:
            return None
        expr_va = struct.unpack_from("<I", code, i - 4)[0]
        file_va = struct.unpack_from("<I", code, i - 9)[0]
        if code[i - 12] == 0x6A:
            line = code[i - 11]
        elif i >= 15 and code[i - 15] == 0x68:
            line = struct.unpack_from("<I", code, i - 14)[0]
        else:
            return None
        path = cstring(self.img, file_va)
        if not path or not re.search(r"\.(cpp|c|h|hpp|inl)$", path, re.I):
            return None
        expr = cstring(self.img, expr_va, 4096)
        if expr is None:
            return None
        return expr, path, line

    def _asserts(self):
        votes = Counter()
        sites = defaultdict(list)
        for caller, pairs in self.calls.items():
            for site, t in pairs:
                a = self._assert_site(site)
                if a:
                    votes[t] += 1
                    sites[t].append((site, caller) + a)
        if not votes:
            return []
        self.assert_fn = votes.most_common(1)[0][0]
        return [dict(site=s, function=f, expr=e, file=p, line=ln)
                for s, f, e, p, ln in sorted(sites[self.assert_fn])]

    def files(self):
        """[(file, first start, last start, [starts])] in link order: the runs of each source file's functions."""
        owner = {}
        for a in self.asserts:
            if a["file"].lower().endswith(SOURCE_SUFFIX):
                owner.setdefault(a["function"], Counter())[a["file"]] += 1
        placed = sorted((f, c.most_common(1)[0][0]) for f, c in owner.items())
        runs = []
        for f, path in placed:
            if runs and runs[-1][0] == path:
                runs[-1][2] = f
            else:
                runs.append([path, f, f])
        out = []
        for path, lo, hi in runs:
            k0 = bisect.bisect_left(self.starts, lo)
            k1 = bisect.bisect_right(self.starts, hi)
            out.append((path, lo, hi, self.starts[k0:k1]))
        return out

    def objects(self, init_functions):
        """The file runs widened to their objects: [(file, first start, last start, [starts])]. An object's code
        begins with its static initializers where it has any (`NiImplementRTTI` sits at the top of a class's .cpp),
        so a run widens back to the nearest initializer before it and forward to the next initializer or run,
        never across another run."""
        runs = self.files()
        inits = sorted(set(init_functions))
        bounds = sorted(set(inits) | {lo for _, lo, _, _ in runs})
        out = []
        for j, (path, lo, hi, _) in enumerate(runs):
            prev_hi = runs[j - 1][2] if j else self.tva
            k = bisect.bisect_right(inits, lo) - 1
            start = inits[k] if k >= 0 and inits[k] > prev_hi else lo
            # an object's initializers come in a group: step back over the adjacent ones
            while k > 0 and inits[k - 1] > prev_hi and self.next_start(inits[k - 1]) == inits[k]:
                k -= 1
                start = inits[k]
            n = bisect.bisect_right(bounds, hi)
            end = bounds[n] if n < len(bounds) else self.tva + len(self.code)
            # library code is packed; int3 fill before a function marks the application's or the CRT's objects
            f = self.next_start(hi)
            while f < end and self.code[f - self.tva - 1] != 0xCC:
                f = self.next_start(f)
            end = min(end, f)
            k0 = bisect.bisect_left(self.starts, start)
            k1 = bisect.bisect_left(self.starts, end)
            out.append((path, start, self.starts[k1 - 1], self.starts[k0:k1]))
        return out


CLASS_NAME = re.compile(r"^[A-Z][A-Za-z0-9_]{1,63}$")
RTTI_SITE = re.compile(rb"(?:\x68(.{4})|\x6A\x00)\x68(.{4})\xB9(.{4})\xE8", re.S)


class Rtti:
    """NetImmerse's RTTI in one image: each class's NiRTTI, its GetRTTI functions and its vtables.

    `classes`: {class name: NiRTTI address}; `base`: {class name: base class name or None}; `vtables`: {class name:
    [vtable address, ...]} (a class with several bases' tables, or a template's copies, has more than one); `slots`:
    {vtable address: [code address, ...]}."""

    def __init__(self, img, thunks=None):
        self.img = img
        self.thunks = thunks or {}
        _, tva, _, _, _ = img.text
        code = img.read(tva, min(img.text[2], img.text[4]))
        votes = Counter()
        sites = []
        for m in RTTI_SITE.finditer(code):
            name = cstring(img, struct.unpack("<I", m.group(2))[0], 80)
            if not name or not CLASS_NAME.match(name):
                continue
            i = m.end() - 1
            ctor = rel32(code, i, tva + i)
            votes[ctor] += 1
            base = struct.unpack("<I", m.group(1))[0] if m.group(1) else 0
            sites.append((ctor, name, base, struct.unpack("<I", m.group(3))[0], tva + m.start()))
        self.ctor = votes.most_common(1)[0][0] if votes else None
        by_addr = {}
        self.classes = {}
        self.init_sites = [site for ctor, _, _, _, site in sites if ctor == self.ctor]
        for ctor, name, base, rtti, _ in sites:
            if ctor == self.ctor and name not in self.classes:
                self.classes[name] = rtti
                by_addr[rtti] = (name, base)
        self.base = {n: by_addr.get(b, (None,))[0] for n, (_, b) in
                     ((n, by_addr[a]) for n, a in self.classes.items())}
        self.vtables, self.slots = self._vtables(code, tva, by_addr)

    def _vtables(self, code, tva, by_addr):
        # a GetRTTI: `mov eax, <rtti>` (B8 imm32) in a function no bigger than a debug frame's
        getrtti = {}
        for m in re.finditer(rb"\xB8(.{4})", code, re.S):
            a = struct.unpack("<I", m.group(1))[0]
            if a not in by_addr:
                continue
            i = m.start()
            if code[i + 5] == 0xC3:                                   # release: mov eax, imm; ret
                getrtti[tva + i] = by_addr[a][0]
            else:                                                     # debug: push ebp ... mov eax, imm ... ret
                k = code.rfind(b"\x55\x8B\xEC", max(0, i - 16), i)
                if k >= 0 and code.find(b"\xC3", i + 5, i + 12) >= 0:
                    getrtti[tva + k] = by_addr[a][0]
        alias = defaultdict(list)
        for t, f in self.thunks.items():
            if f in getrtti:
                alias[t].append(f)
        text_lo, text_hi = tva, tva + len(code)
        vt_starts = {}
        for name, va, vsize, _, rsize in [s for s in self.img.sections if s[0] in (".rdata", ".data")]:
            data = self.img.read(va, min(vsize, rsize))
            for k in range(0, len(data) - 3, 4):
                p = struct.unpack_from("<I", data, k)[0]
                p = self.thunks.get(p, p)
                if p in getrtti:
                    vt_starts[va + k] = getrtti[p]
        vtables = defaultdict(list)
        slots = {}
        ordered = sorted(vt_starts)
        for j, v in enumerate(ordered):
            limit = ordered[j + 1] if j + 1 < len(ordered) else v + 4 * 400
            out = []
            a = v
            while a < limit:
                p = self.img.dword(a)
                if p is None or not text_lo <= p < text_hi:
                    break
                out.append(self.thunks.get(p, p))
                a += 4
            vtables[vt_starts[v]].append(v)
            slots[v] = out
        return dict(vtables), slots


def listed_vtables():
    """{class: (address, slots)} from config/vtables.txt: retail's vtables as the project identified them."""
    out = {}
    try:
        text = (ROOT / "config" / "vtables.txt").read_text(encoding="utf-8")
    except OSError:
        return out
    for line in text.splitlines():
        m = re.match(r"\s*(.+?)\s+(0x[0-9A-Fa-f]+)\s+(\d+)", line.split("#")[0])
        if m:
            out[m.group(1)] = (int(m.group(2), 16), int(m.group(3)))
    return out


CONFIRMED = ROOT / "config" / "sdkdebug_pairs.txt"


def confirmed_pairs(path=None):
    """[(retail, debug)] from config/sdkdebug_pairs.txt: `0x<retail> 0x<debug>  # evidence` per line, a pair
    a `search` hit established (the debug copy read beside ours), which `map` takes as an anchor."""
    try:
        text = Path(path or CONFIRMED).read_text(encoding="utf-8")
    except OSError:
        return []
    out = []
    for line in text.splitlines():
        m = re.match(r"\s*(0x[0-9A-Fa-f]+)\s+(0x[0-9A-Fa-f]+)", line.split("#")[0])
        if m:
            out.append((int(m.group(1), 16), int(m.group(2), 16)))
    return out


class Retail:
    """Retail's side: the annotated functions (build/report.json), their direct calls, and NetImmerse's RTTI."""

    def __init__(self, img, rows):
        self.img = img
        self.rows = {}
        for r in rows:
            if r.get("static") or "size" not in r:
                continue
            self.rows.setdefault(int(r["addr"], 16), r)
        self.starts = sorted(self.rows)
        self.rtti = Rtti(img)
        listed = listed_vtables()
        for cls, vts in self.rtti.vtables.items():
            if cls in listed:
                va, n = listed[cls]
                self.rtti.vtables[cls] = [va]
                self.rtti.slots[va] = [img.dword(va + 4 * k) for k in range(n)]
            elif len(vts) > 1:
                self.rtti.vtables[cls] = []   # which table is the class's own is unknown

    def calls(self, start):
        r = self.rows.get(start)
        if r is None:
            return []
        code = self.img.read(start, r["size"]) or b""
        out = []
        for m in re.finditer(rb"\xE8", code):
            i = m.start()
            if i + 5 <= len(code):
                t = rel32(code, i, start + i)
                if t in self.rows:
                    out.append(t)
        return out


def immediates(code, base, lo, hi):
    """[(site, value)] of the dword immediates of `push imm32` (68) and `mov reg, imm32` (B8-BF) in [lo, hi)."""
    out = []
    for m in re.finditer(rb"[\x68\xB8-\xBF](.{4})", code, re.S):
        v = struct.unpack("<I", m.group(1))[0]
        if lo <= v < hi:
            out.append((base + m.start(), v))
    return out


def string_users(img, function_of, sections=(".data", ".rdata")):
    """{string text: {function}} of the strings pushed or loaded as immediates (six characters or more)."""
    _, tva, _, _, _ = img.text
    code = img.read(tva, min(img.text[2], img.text[4]))
    out = defaultdict(set)
    for name, va, vsize, _, rsize in img.sections:
        if name not in sections:
            continue
        for site, v in immediates(code, tva, va, va + min(vsize, rsize)):
            s = cstring(img, v, 256)
            if s and len(s) >= 6:
                f = function_of(site)
                if f is not None:
                    out[s].add(f)
    return out


class Mapper:
    """Pairs AnimOnly's functions with retail's: vtable slots, creation functions, unique strings, then calls."""

    def __init__(self, dbg, ret, confirmed=None):
        self.dbg, self.ret = dbg, ret
        self.confirmed = confirmed
        self.conflicts = []    # [(retail, debug, why)]: confirmed pairs the map's own pairs contradict
        self.drtti = Rtti(dbg.img, dbg.thunks)
        self.pairs = {}        # debug start -> retail start
        self.rpairs = {}
        self.how = {}
        self.vtable_notes = {}

    def add(self, d, r, how):
        if d in self.pairs or r in self.rpairs or r not in self.ret.rows:
            return False
        self.pairs[d], self.rpairs[r], self.how[d] = r, d, how
        return True

    def by_vtables(self):
        """Slot by slot, over the classes both builds name. A slot pairs when both builds agree on whether the class
        overrides it (both inherit the base's function, or both define their own); from the first slot where they
        disagree on, the class's later slots are left out (a virtual added or removed between the versions)."""
        dr, rr = self.drtti, self.ret.rtti
        for cls in sorted(set(dr.classes) & set(rr.classes)):
            dv, rv = dr.vtables.get(cls, []), rr.vtables.get(cls, [])
            if len(dv) != 1 or len(rv) != 1:
                self.vtable_notes[cls] = "tables: %d debug, %d retail" % (len(dv), len(rv))
                continue
            ds, rs = dr.slots[dv[0]], rr.slots[rv[0]]
            dbase = dr.slots.get((dr.vtables.get(dr.base.get(cls)) or [None])[0], [])
            rbase = rr.slots.get((rr.vtables.get(rr.base.get(cls)) or [None])[0], [])
            n = min(len(ds), len(rs))
            for k in range(n):
                dinh = k < len(dbase) and ds[k] == dbase[k]
                rinh = k < len(rbase) and rs[k] == rbase[k]
                if dinh != rinh:
                    self.vtable_notes[cls] = "slot %d: %s overrides, %s inherits" % (
                        k, "debug" if rinh else "retail", "retail" if rinh else "debug")
                    break
                if not dinh:
                    self.add(ds[k], rs[k], "vtable %s[%d]" % (cls, k))
            if len(ds) < len(rs) and cls not in self.vtable_notes:
                self.vtable_notes[cls] = "debug table shorter: %d of %d slots" % (len(ds), len(rs))

    def by_creation(self):
        """`push <creation function>; push "<class>"`: each class's CreateObject registration, in both builds."""
        def creators(img, starts_ok):
            _, tva, _, _, _ = img.text
            code = img.read(tva, min(img.text[2], img.text[4]))
            out = defaultdict(set)
            for m in re.finditer(rb"\x68(.{4})\x68(.{4})", code, re.S):
                fn, s = struct.unpack("<II", m.group(1) + m.group(2))
                name = cstring(img, s, 80)
                if name and CLASS_NAME.match(name) and starts_ok(fn):
                    out[name].add(fn)
            return out
        dc = creators(self.dbg.img, lambda a: a in self.dbg.start_set)
        rc = creators(self.ret.img, lambda a: a in self.ret.rows)
        for name in sorted(set(dc) & set(rc)):
            if len(dc[name]) == 1 and len(rc[name]) == 1:
                self.add(next(iter(dc[name])), next(iter(rc[name])), "creation %s" % name)

    def by_strings(self):
        rf = lambda va: (lambda k: self.ret.starts[k] if k >= 0 else None)(
            bisect.bisect_right(self.ret.starts, va) - 1)
        du = string_users(self.dbg.img, self.dbg.function_of)
        ru = string_users(self.ret.img, rf)
        for s in sorted(set(du) & set(ru)):
            if len(du[s]) == 1 and len(ru[s]) == 1:
                self.add(next(iter(du[s])), next(iter(ru[s])), "string %r" % s[:40])

    def by_calls(self, rounds=20):
        """A paired function's calls: between two consecutive callees both builds pair, a lone unpaired callee on
        each side pairs (a debug build also calls the inlines release expanded, so only such one-to-one gaps count).
        Every vote must agree."""
        for _ in range(rounds):
            votes = defaultdict(Counter)
            for d, r in list(self.pairs.items()):
                dseq = [t for _, t in self.dbg.calls.get(d, [])]
                rseq = self.ret.calls(r)
                for dgap, rgap in gaps(dseq, rseq, self.pairs):
                    if len(set(dgap)) == 1 and len(set(rgap)) == 1:
                        votes[dgap[0]][rgap[0]] += 1
            rvotes = defaultdict(set)
            for dt, c in votes.items():
                for rt in c:
                    rvotes[rt].add(dt)
            added = 0
            for dt, c in votes.items():
                if len(c) == 1:
                    rt = next(iter(c))
                    if len(rvotes[rt]) == 1 and self.add(dt, rt, "calls"):
                        added += 1
            if not added:
                break


    def by_order(self):
        """Definition order: between two consecutive pairs whose retail functions are neighbours in one unit (no
        other pair between them on either side), the unpaired functions pair in order when both sides hold the
        same number. A debug link lays an object's functions out in definition order, as retail's units are."""
        unit = lambda r: self.ret.rows[r]["unit"]
        anchors = sorted(self.pairs.items())
        added = 0
        for (d0, r0), (d1, r1) in zip(anchors, anchors[1:]):
            if not r0 < r1 or unit(r0) != unit(r1):
                continue
            k0, k1 = bisect.bisect_right(self.dbg.starts, d0), bisect.bisect_left(self.dbg.starts, d1)
            ds = [f for f in self.dbg.starts[k0:k1] if f not in self.pairs]
            j0, j1 = bisect.bisect_right(self.ret.starts, r0), bisect.bisect_left(self.ret.starts, r1)
            rs = [r for r in self.ret.starts[j0:j1] if unit(r) == unit(r0)]
            if any(r in self.rpairs for r in rs) or not ds or len(ds) != len(rs):
                continue
            for d, r in zip(ds, rs):
                ratio = (self.dbg.end_of(d) - d) / max(1, self.ret.rows[r]["size"])
                if not 0.5 <= ratio <= 12:
                    break
            else:
                for d, r in zip(ds, rs):
                    added += self.add(d, r, "order")
        return added

    def by_confirmed(self):
        """The pairs config/sdkdebug_pairs.txt (or `map --pairs`) confirms (`search` hits read side by side). They
        come after the map's own evidence and add to it: one that disagrees with a pair already made is left out and
        listed (`conflicts`), for a reader to settle."""
        for r, d in confirmed_pairs(self.confirmed):
            if self.pairs.get(d) == r:
                continue
            if d not in self.dbg.start_set or r not in self.ret.rows:
                self.conflicts.append((r, d, "not a function start on both sides"))
            elif d in self.pairs or r in self.rpairs:
                held = ("debug 0x%06X is paired with %s" % (d, self.ret.rows[self.pairs[d]]["name"]) if d in self.pairs
                        else "retail is paired with debug 0x%06X" % self.rpairs[r])
                self.conflicts.append((r, d, held))
            else:
                self.add(d, r, "confirmed")

    def fixpoint(self):
        while True:
            before = len(self.pairs)
            self.by_calls()
            self.by_order()
            if len(self.pairs) == before:
                break

    def run(self):
        self.by_vtables()
        self.by_creation()
        self.by_strings()
        self.fixpoint()
        # the confirmed pairs anchor a second round: a vote they would split earlier stays the map's own
        self.by_confirmed()
        self.fixpoint()
        return self


def gaps(dseq, rseq, pairs):
    """The unpaired callees between the same two paired anchors in both sequences: [(debug gap, retail gap)]. The
    anchors are the paired callees that occur once on each side, in the same order."""
    rpos = {}
    for i, t in enumerate(rseq):
        rpos.setdefault(t, []).append(i)
    anchors = [(-1, -1)]
    last = -1
    dcount = Counter(dseq)
    for i, t in enumerate(dseq):
        r = pairs.get(t)
        if r is None or dcount[t] != 1 or len(rpos.get(r, ())) != 1:
            continue
        j = rpos[r][0]
        if j > last:
            anchors.append((i, j))
            last = j
    anchors.append((len(dseq), len(rseq)))
    rpaired = set(pairs.values())
    out = []
    for (i0, j0), (i1, j1) in zip(anchors, anchors[1:]):
        dg = [t for t in dseq[i0 + 1:i1] if t not in pairs]
        rg = [t for t in rseq[j0 + 1:j1] if t not in rpaired]
        if dg and rg:
            out.append((dg, rg))
    return out


OWN_MEMBER = re.compile(r"(?<![\w>.])(m_\w+)")
THIS_SLOT = re.compile(r"^mov dword ptr \[ebp - (0x[0-9a-f]+)\], ecx$")
LOAD_THIS = re.compile(r"^mov (e[a-d]x|e[sd]i), dword ptr \[ebp - (0x[0-9a-f]+)\]$")
MEMBER_OP = re.compile(r"\[(e[a-d]x|e[sd]i)(?: \+ (0x[0-9a-f]+))?\]")


def member_offsets(dbg):
    """[(function, member name, offset, assert)]: an assert whose expression names one member of its own object
    (`m_pBuffer != NULL`, not `pkObj->m_x`), where the test right before it reads exactly one offset of `this`
    (the slot the prologue stored ECX in). The offset is the debug build's layout."""
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    out = []
    by_fn = defaultdict(list)
    for a in dbg.asserts:
        by_fn[a["function"]].append(a)
    for f, asserts in by_fn.items():
        ins = list(md.disasm_lite(dbg.img.read(f, dbg.end_of(f) - f), f))
        at = {i[0]: k for k, i in enumerate(ins)}
        this_slot = next((m.group(1) for i in ins[:40] for m in [THIS_SLOT.match("%s %s" % (i[2], i[3]))] if m),
                         None)
        if this_slot is None:
            continue
        for a in asserts:
            names = set(OWN_MEMBER.findall(a["expr"]))
            k = at.get(a["site"])
            if len(names) != 1 or k is None:
                continue
            # back over the three pushes to the jcc that skips the call, then back to the previous control transfer
            j = k - 1
            while j > 0 and ins[j][2] == "push":
                j -= 1
            if not ins[j][2].startswith("j"):
                continue
            offs, held = set(), set()
            for i in ins[max(0, j - 12):j]:
                text = "%s %s" % (i[2], i[3])
                if i[2] in ("call", "ret") or i[2].startswith("j"):
                    offs, held = set(), set()
                    continue
                lt = LOAD_THIS.match(text)
                for m in MEMBER_OP.finditer(i[3]):
                    if m.group(1) in held:
                        offs.add(int(m.group(2) or "0", 16))
                if lt:
                    (held.add if lt.group(2) == this_slot else held.discard)(lt.group(1))
            if len(offs) == 1:
                out.append((f, names.pop(), offs.pop(), a))
    return out


def plain_name(member):
    """NetImmerse's Hungarian member name in the project's style: `m_iRefCount` -> `refCount`, `m_pkChild` ->
    `child`, `ms_pLoaders` -> `loaders` (AGENTS.md: lowerCamel, no m_, no Hungarian)."""
    rest = re.sub(r"^ms?_", "", member)
    m = re.match(r"[a-z]+(?=[A-Z])", rest)
    if m:
        rest = rest[m.end():]
    return rest[:1].lower() + rest[1:]


def field_at(types, cls, off, depth=0):
    """Our member of `cls` at byte `off` (bases entered; a nested struct's own members are not): (owner, name)."""
    t = types.get(cls)
    if not t or depth > 12:
        return None
    for fld in t.get("fields", []):
        if fld["offset"] == off:
            return cls, fld["name"]
    for b in t.get("bases", []):
        hit = field_at(types, b["name"], off - b["offset"], depth + 1) if off >= b["offset"] else None
        if hit:
            return hit
    return None


PLACEHOLDER = re.compile(r"^(unknown[0-9A-Fa-f]+|Unknown[0-9A-F]{6}|VirtualSlot\d+|field_0x\w+)$")


def stem(path):
    """`NiNode` from `C:\\TGSharedExternal\\NDL3\\Src\\NiMain\\NiNode.cpp`."""
    return re.split(r"[\\/]", path)[-1].rsplit(".", 1)[0]


def short(path):
    """`NiMain\\NiNode.cpp` from `C:\\TGSharedExternal\\NDL3\\Src\\NiMain\\NiNode.cpp`."""
    parts = re.split(r"[\\/]", path)
    return "\\".join(parts[-2:]) if len(parts) >= 2 else path


def load(exe=None):
    with open(exe_path(exe), "rb") as f:
        return Debug(pe.Image(f.read()))


def cmd_asserts(args):
    dbg = load(args.exe)
    n = 0
    for a in dbg.asserts:
        if args.file and not re.search(args.file, a["file"], re.I):
            continue
        if args.expr and not re.search(args.expr, a["expr"]):
            continue
        print("0x%06X  %-34s %5d  %s" % (a["function"], short(a["file"]), a["line"], a["expr"]))
        n += 1
    print("%d asserts (_assert at 0x%X, %d call sites in all)" % (n, dbg.assert_fn or 0, len(dbg.asserts)))


def objects(dbg, rtti=None):
    """The debug image's file runs widened to their objects (Debug.objects), with NiRTTI's initializers."""
    rtti = rtti or Rtti(dbg.img, dbg.thunks)
    return dbg.objects([dbg.function_of(s) for s in rtti.init_sites])


def cmd_files(args):
    dbg = load(args.exe)
    runs = objects(dbg) if args.objects else dbg.files()
    seen = Counter(r[0] for r in runs)
    for path, lo, hi, fns in runs:
        split = "  (split: %d runs)" % seen[path] if seen[path] > 1 else ""
        print("0x%06X-0x%06X  %4d functions  %s%s" % (lo, dbg.next_start(hi), len(fns), short(path), split))
        if args.all:
            for f in fns:
                own = [a for a in dbg.asserts if a["function"] == f]
                print("    0x%06X  %s" % (f, "; ".join("%d %s" % (a["line"], a["expr"][:60]) for a in own[:3])))
    print("%d runs, %d files, %d functions placed" % (len(runs), len(seen), sum(len(r[3]) for r in runs)))


def retail_image():
    with open(RETAIL, "rb") as f:
        return pe.Image(f.read())


def report_rows():
    try:
        return json.loads((BUILD / "report.json").read_text())["functions"]
    except (OSError, ValueError, KeyError):
        sys.exit("build/report.json is missing: run `python tools/decomp.py build` first")


def file_of(runs):
    """{debug start: source file} for the functions inside each file's runs."""
    out = {}
    for path, _, _, fns in runs:
        for f in fns:
            out[f] = path
    return out


def load_types():
    import pickle
    try:
        with open(BUILD / "layoutcheck_types.pkl", "rb") as f:
            return pickle.load(f)["types"]
    except (OSError, ValueError, KeyError, EOFError):
        return {}


def build_map(dbg, ret, mapper, types):
    """The map's records: pairs, units against files, member names."""
    runs = objects(dbg, mapper.drtti)
    fof = file_of(runs)
    strong = file_of(dbg.files())
    asserts_of = defaultdict(list)
    for a in dbg.asserts:
        asserts_of[a["function"]].append(a)
    pairs = []
    unit_files = defaultdict(Counter)
    for d, r in sorted(mapper.pairs.items()):
        row = ret.rows[r]
        path = fof.get(d)
        if path:
            unit_files[row["unit"]][path] += 1
        pairs.append({"debug": "0x%06X" % d, "retail": row["addr"], "name": row["name"], "unit": row["unit"],
                      "symbol": row.get("symbol"),
                      "status": row["status"], "how": mapper.how[d], "file": path, "in_run": d in strong,
                      "debug_size": dbg.end_of(d) - d, "retail_size": row.get("size"),
                      "asserts": ["%d: %s" % (a["line"], a["expr"]) for a in asserts_of.get(d, [])]})
    file_units = defaultdict(Counter)
    for u, c in unit_files.items():
        for path, n in c.items():
            file_units[path][u] += n
    members = []
    classes = {re.split(r"[\\/]", p)[-1].rsplit(".", 1)[0] for p, _, _, _ in runs}
    for f, name, off, a in member_offsets(dbg):
        r = mapper.pairs.get(f)
        if r is not None:
            cls, via = ret.rows[r]["name"].split("::")[0], "pair"
        else:
            cls, via = re.split(r"[\\/]", a["file"])[-1].rsplit(".", 1)[0], "file"
            if cls not in classes or cls not in types:
                continue
        hit = field_at(types, cls, off)
        want = plain_name(name)
        verdict = ("none" if hit is None else "placeholder" if PLACEHOLDER.match(hit[1])
                   else "agrees" if hit[1].lower() == want.lower() else "differs")
        members.append({"class": cls, "offset": "0x%X" % off, "original": name, "candidate": want,
                        "ours": "%s::%s" % hit if hit else None, "verdict": verdict, "via": via,
                        "debug": "0x%06X" % f, "assert": "%s(%d): %s" % (short(a["file"]), a["line"], a["expr"])})
    return {"pairs": pairs, "unit_files": {u: dict(c) for u, c in unit_files.items()},
            "file_units": {p: dict(c) for p, c in file_units.items()}, "members": members,
            "vtable_notes": mapper.vtable_notes, "runs": [[p, "0x%06X" % lo, "0x%06X" % dbg.next_start(hi), len(fns)]
                                                          for p, lo, hi, fns in runs]}


def summarize(result, ret):
    ni_rows = [r for r in ret.rows.values() if r["unit"].startswith("netimmerse")]
    ni_units = {r["unit"] for r in ni_rows}
    pairs = [p for p in result["pairs"] if p["unit"].startswith("netimmerse")]
    named_units = [u for u in result["unit_files"] if u.startswith("netimmerse")]
    ph = [p for p in pairs if PLACEHOLDER.match(p["name"].split("::")[-1])]
    lines = [
        "pairs: %d in all, %d of our %d NetImmerse functions (%s)" % (
            len(result["pairs"]), len(pairs), len(ni_rows),
            ", ".join("%s %d" % kv for kv in Counter(p["how"].split()[0] for p in result["pairs"]).most_common())),
        "NetImmerse units with an original file: %d of %d" % (len(named_units), len(ni_units)),
        "NetImmerse functions inside a file run: %d (%d open)" % (
            sum(1 for p in pairs if p["file"]), sum(1 for p in pairs if p["file"] and p["status"] != "MATCH")),
        "placeholder functions paired: %d (%d with asserts)" % (len(ph), sum(1 for p in ph if p["asserts"])),
        "member asserts placed: %d (%s)" % (len(result["members"]), ", ".join(
            "%s %d" % kv for kv in Counter(m["verdict"] for m in result["members"]).most_common())),
    ]
    renamed, copies = [], []
    main_file = {u: max(c, key=c.get) for u, c in result["unit_files"].items()}
    for u, path in sorted(main_file.items()):
        if stem(path) != u.split("/")[-1]:
            renamed.append("  unit %s is %s (%d of its %d paired functions)" % (
                u, short(path), result["unit_files"][u][path], sum(result["unit_files"][u].values())))
    split = defaultdict(set)
    for u, path in main_file.items():
        split[path].add(u)
    for path, us in sorted(split.items()):
        if len(us) > 1:
            renamed.append("  file %s is split over %s" % (short(path), ", ".join(sorted(us))))
    for p in result["pairs"]:
        if p["file"] and p["unit"] in main_file and p["file"] != main_file[p["unit"]]:
            copies.append("  %s %s: debug copy 0x%s in %s%s (%s)" % (
                p["retail"], p["name"], p["debug"][2:], short(p["file"]), "" if p["in_run"] else ", widened",
                p["how"]))
    renamed += ["functions whose debug copy sits in another object (an inline of the original's headers, or a "
                "unit boundary):"] + copies
    return lines, renamed


def cmd_map(args):
    dbg = load(args.exe)
    ret = Retail(retail_image(), report_rows())
    mapper = Mapper(dbg, ret, args.pairs).run()
    result = build_map(dbg, ret, mapper, load_types())
    out = Path(args.json) if args.json else OUT / "map.json"
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(json.dumps(result, indent=1))
    lines, renamed = summarize(result, ret)
    for r, d, why in mapper.conflicts:
        lines.append("confirmed pair 0x%X / 0x%06X left out: %s" % (r, d, why))
    print("\n".join(lines))
    print("units and files that disagree with ours:")
    print("\n".join(renamed) or "  none")
    print("member names (the original's, from asserts; `ours` is the member at that offset):")
    seen = set()
    for m in result["members"]:
        key = (m["class"], m["offset"], m["original"])
        if m["verdict"] in ("placeholder", "differs") and key not in seen:
            seen.add(key)
            print("  %-26s %-6s ours %-38s original %-24s -> %s (%s)" % (
                m["class"], m["offset"], m["ours"], m["original"], m["candidate"], m["via"]))
    print("written to %s" % out)


def cmd_fn(args):
    dbg = load(args.exe)
    a = int(args.addr, 16)
    f = dbg.function_of(a)
    ret = Retail(retail_image(), report_rows())
    mapper = Mapper(dbg, ret).run()
    fof = file_of(objects(dbg, mapper.drtti))
    print("0x%06X-0x%06X  file %s" % (f, dbg.end_of(f), short(fof[f]) if f in fof else "-"))
    r = mapper.pairs.get(f)
    if r is not None:
        row = ret.rows[r]
        print("pairs with retail %s %s (%s, %s) by %s" % (row["addr"], row["name"], row["unit"], row["status"],
                                                           mapper.how[f]))
    for x in dbg.asserts:
        if x["function"] == f:
            print("  %s(%d): %s" % (short(x["file"]), x["line"], x["expr"]))
    names = debug_names()      # od.json's /Od matches name the inline copies retail expanded
    for _, t in dbg.calls.get(f, []):
        rt = mapper.pairs.get(t)
        name = ret.rows[rt]["name"] if rt else names.get(t)
        print("  calls 0x%06X%s" % (t, "  = %s" % name if name else ""))


DEBUG_FLAGS = "/Od /GX /W3 /MLd /D WIN32 /D _DEBUG /D _WINDOWS"


def compile_debug(source, flags, compiler=None, includes=()):
    """Compile a scratch file with VC6 under `flags`; the object's path."""
    import shlex
    import vc6
    source = Path(source).resolve()
    obj = OUT / "find" / (source.stem + ".obj")
    obj.parent.mkdir(parents=True, exist_ok=True)
    args = ["/nologo", "/c", *shlex.split(flags), *("/I%s" % i for i in includes), "/Fo%s" % obj, str(source)]
    code, out = vc6.run("cl", args, cwd=str(obj.parent), compiler=compiler)   # vc60.pdb/.idb land there
    if code:
        sys.exit("cl failed:\n%s" % out)
    return obj


def coff_table(o, sym, lo, hi):
    """The offset (from `lo`) of a function's first jump table in our object, or None: the lowest place in the
    function that one of its own relocated fields points forward to (`jmp dword ptr [reg*4 + $L]`, a byte index
    table's load). Its instructions end there; the tables run to the end of the function."""
    import coff
    sec = o.section(sym.section)
    best = None
    for off, si, typ in sec.relocs:
        t = o.symbols[si]
        if not lo <= off < hi or t.section != sym.section or typ != coff.IMAGE_REL_I386_DIR32:
            continue
        target = t.value + int.from_bytes(sec.data[off:off + 4], "little")
        if off < target < hi and (best is None or target < best):
            best = target
    return None if best is None else best - lo


def object_functions(obj, lines=None, tables=None):
    """[(decorated name, bytes, masked offsets)] of the functions an object defines, its COMDAT copies included.
    `lines`, a dict, receives each function's assert line pushes (their instruction offsets) by name; `tables` each
    function's jump table offset (coff_table)."""
    import coff
    o = coff.Coff(obj.read_bytes())
    out = []
    for sym in o.symbols.values():
        if not (sym.defined and sym.is_function and sym.storage in (coff.SC_EXTERNAL, coff.SC_STATIC)):
            continue
        sec = o.section(sym.section)
        lo, hi = o.function_extent(sym)
        masked = {off - lo for off, _, _ in sec.relocs if lo <= off < hi}
        # An assert's line operand (`push <line>; push <file>; push <expr>; call __assert`) is layout, not code:
        # the source's line numbers are free (no line information shipped), so it is masked like a relocation.
        pushes = set()
        for off, si, _ in sec.relocs:
            if not (lo <= off < hi and o.symbols[si].name == "__assert"):
                continue
            line = off - lo - 15
            if line >= 1 and sec.data[lo + line - 1] == 0x68 and sec.data[lo + line + 4] == 0x68:
                masked.add(line)
                pushes.add(line - 1)
            elif off - lo >= 13 and sec.data[off - 13] == 0x6A and sec.data[off - 11] == 0x68:
                pushes.add(off - lo - 13)
        if lines is not None:
            lines[sym.name] = pushes
        if tables is not None:
            tables[sym.name] = coff_table(o, sym, lo, hi)
        out.append((sym.name, sec.data[lo:hi], masked))
    return out


def same_body(dbg, d, body, masked, pushes=(), table=None):
    """Whether AnimOnly's function at `d` is our /Od `body` instruction for instruction: the same bytes outside the
    relocated fields, branches to the same instruction (a short or near encoding alike), an assert's line pushed in
    either encoding (`pushes`: the line pushes' offsets), its jump tables (from `table`, coff_table's offset) right
    after its instructions on both sides and alike outside the relocated entries, and nothing but int3 padding after
    it. Finds the copies a byte search can't: a line under 128 pushed as an imm8, a function start the image cut
    missed, a body under 8 bytes."""
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    code = body if table is None else body[:table]
    ours = list(md.disasm(code, 0))
    if not ours or sum(i.size for i in ours) != len(code):
        return False
    end = dbg.end_of(d)
    if end <= d:
        return False
    theirs = list(md.disasm(dbg.img.read(d, end - d), d))[:len(ours)]
    if len(theirs) < len(ours):
        return False
    tail = theirs[-1].address + theirs[-1].size
    if table is not None:
        rest = body[table:]
        if dbg.tables.get(d) != tail or dbg.next_start(d) < tail + len(rest):
            return False
        got = dbg.img.read(tail, len(rest))
        if any(got[j] != rest[j] for j in range(len(rest)) if not any(m <= table + j < m + 4 for m in masked)):
            return False
        tail, end = tail + len(rest), dbg.next_start(d)
    if any(b != 0xCC for b in dbg.img.read(tail, end - tail)):
        return False
    oi = {i.address: k for k, i in enumerate(ours)}
    ti = {i.address: k for k, i in enumerate(theirs)}

    def relocated(i):
        return any(m < i.address + i.size and i.address < m + 4 for m in masked)

    for a, b in zip(ours, theirs):
        if a.address in pushes:
            if b.mnemonic != "push" or not re.fullmatch(r"(0x)?[0-9a-f]+", b.op_str):
                return False
            continue
        if (a.mnemonic.startswith("j") or a.mnemonic == "call") and re.fullmatch(r"0x[0-9a-f]+", a.op_str) \
                and not relocated(a):
            if a.mnemonic != b.mnemonic or not re.fullmatch(r"0x[0-9a-f]+", b.op_str):
                return False
            if oi.get(int(a.op_str, 16)) != ti.get(int(b.op_str, 16)):
                return False
            continue
        if a.size != b.size:
            return False
        for j in range(a.size):
            if not any(m <= a.address + j < m + 4 for m in masked) and a.bytes[j] != b.bytes[j]:
                return False
    return True


def find_body(dbg, body, masked):
    """The addresses in the debug image whose bytes equal `body` (relocated dwords masked) at a function start."""
    k = 0
    parts = []
    while k < len(body):
        if k in masked:
            parts.append(b".{4}")
            k += 4
        else:
            parts.append(re.escape(body[k:k + 1]))
            k += 1
    pattern = re.compile(b"".join(parts), re.S)
    starts = dbg.start_set
    return [dbg.tva + m.start() for m in pattern.finditer(dbg.code) if dbg.tva + m.start() in starts]


def cmd_find(args):
    """Compile a scratch file under debug flags and look for each of its functions in AnimOnly, byte for byte."""
    dbg = load(args.exe)
    obj = compile_debug(args.source, args.flags, args.compiler, [ROOT / "src" / "netimmerse"] + args.include)
    found = frame_only = 0
    texts, dl = None, None
    for name, body, masked in sorted(object_functions(obj)):
        if args.only and not re.search(args.only, name):
            continue
        hits = find_body(dbg, body, masked)
        found += bool(hits)
        note = ""
        if not hits and not args.no_mask_frame:
            # the same instructions but for frame slots: the statements are the original's, a local's name or
            # declaration order (or /GZ's padding) differs
            if dl is None:
                texts, dl = object_listings(obj), DebugListings(dbg)
            same = dl.index(True).get(tuple(normalize(t) for t in texts.get(name, ())), [])
            if same:
                frame_only += 1
                note = "  frame slots differ: " + ", ".join("0x%06X" % h for h in same[:6])
        print("%-60s %4d bytes  %s%s" % (name, len(body), ", ".join("0x%06X" % h for h in hits[:6]) or "-", note))
        if args.show:
            print("    " + body.hex())
    print("%d functions found in AnimOnly, %d more identical but for frame slots (flags: %s)" % (
        found, frame_only, args.flags))


RELEASE_ONLY = re.compile(r"^/(O[12xgityb]\w*|G[56B]|Gy|MT|ML|MD|YX|Fp.*|DNDEBUG)$")
OD_JSON = OUT / "od.json"


def od_args(unit, path, gz):
    """A unit's own arguments (includes, defines, forced includes) under AnimOnly's debug flags: /Od, /MLd, _DEBUG,
    and /GZ for the libraries built with it (NiDirect3D, NiSurface)."""
    import decomp
    args = [a for a in decomp.cl_args(unit, path, pch=False) if not RELEASE_ONLY.match(a)]
    args = [a for a in args if a not in ("/D", "NDEBUG")]
    return ["/Od", "/MLd", "/D_DEBUG"] + (["/GZ"] if gz else []) + args


# The back end each of AnimOnly's libraries was built with, where it is not the unit's own: NiAnimation's float
# compares read `test ah, 5; jp` and `and eax, 0x4100`, the VC6 Processor Pack's C2 (NiLightColorController::Update,
# NiVisController::ResetTimeExtrema), where NiMain's read SP5's `test ah, 1` and `test ah, 0x41`.
LIBRARY_BACKEND = {"NiAnimation": "pp"}


def od_backends(unit, pairs=None):
    """The back ends a unit is compiled with at /Od: its own (None), and that of every AnimOnly library one of its
    paired functions comes from."""
    if pairs is None:
        try:
            pairs = json.loads((OUT / "map.json").read_text())["pairs"]
        except (OSError, ValueError, KeyError):
            pairs = []
    libs = {part for p in pairs if p.get("unit") == unit for part in re.split(r"[\\/]", p.get("file") or "")}
    return [None] + sorted({LIBRARY_BACKEND[lib] for lib in libs if lib in LIBRARY_BACKEND})


def od_compile(unit, gz, back=None):
    """Compile one unit under the debug flags into build/sdkdebug/od/ (with `back`, another compiler's back end);
    the object's path, or None."""
    import decomp
    import vc6
    path = next((p for p in (decomp.SRC / (unit + ".cpp"), decomp.SRC / (unit + ".c")) if p.exists()), None)
    if path is None:
        return None
    obj = OUT / "od" / (("gz" if gz else "plain") + ("-" + back if back else "")) / (unit + ".obj")
    obj.parent.mkdir(parents=True, exist_ok=True)
    args = od_args(unit, path, gz)
    code, _ = vc6.run("cl", ["/c", *args[:-2], "/Fo" + str(obj), *args[-2:]], cwd=str(obj.parent),
                      compiler=back or decomp.compiler_of(path))
    return obj if code == 0 and obj.exists() else None


def cmd_od(args):
    """Compile our NetImmerse units under AnimOnly's debug flags and find each function's body in AnimOnly."""
    from concurrent.futures import ThreadPoolExecutor
    dbg = load(args.exe)
    rows = report_rows()
    units = sorted({r["unit"] for r in rows if r["unit"].startswith(args.module)
                    and (not args.unit or re.search(args.unit, r["unit"]))})
    try:
        map_pairs = json.loads((OUT / "map.json").read_text())["pairs"]
    except (OSError, ValueError, KeyError):
        map_pairs = []
    jobs = [(u, gz, back) for u in units for gz in (False, True) for back in od_backends(u, map_pairs)]
    with ThreadPoolExecutor(max_workers=os.cpu_count() or 4) as pool:
        objs = list(pool.map(lambda j: (j, od_compile(*j)), jobs))
    found = defaultdict(set)      # decorated name -> debug addresses
    sizes = {}
    failed = []
    bodies = defaultdict(list)    # (unit, decorated name) -> [(body, masked, line pushes)]
    for (unit, gz, back), obj in objs:
        if obj is None:
            failed.append("%s%s%s" % (unit, " /GZ" if gz else "", " (%s)" % back if back else ""))
            continue
        lines, tables = {}, {}
        for name, body, masked in object_functions(obj, lines, tables):
            bodies[(unit, name)].append((body, masked, lines.get(name, ()), tables.get(name)))
            if name.startswith("_$E") or len(body) < 8:
                continue
            sizes[name] = len(body)
            for h in find_body(dbg, body, masked):
                found[name].add(h)
    # a paired function the byte search missed, compared with its pair instruction for instruction
    for p in map_pairs:
        sym, d = p.get("symbol"), int(p["debug"], 16)
        if sym and d not in found.get(sym, ()) and any(
                same_body(dbg, d, body, masked, pushes, table)
                for body, masked, pushes, table in bodies.get((p["unit"], sym), ())):
            found[sym].add(d)
    out = {"found": {n: ["0x%06X" % h for h in sorted(a)] for n, a in sorted(found.items())},
           "failed": failed, "units": units}
    # set against the map: a paired function whose own /Od copy is exact has the original's statements; one that
    # differs is compared with its pair as listings, masked (the statements) and with frame slots
    mask = not args.no_mask_frame
    exact, differing, open_exact = 0, [], []
    for p in map_pairs:
        sym = p.get("symbol")
        if sym is None:
            continue
        if p["debug"] in out["found"].get(sym, []):
            exact += 1
            if p["status"] != "MATCH":
                open_exact.append(p)
        else:
            differing.append(p)
    want = defaultdict(set)
    for p in differing:
        want[p["unit"]].add(p["symbol"])
    ours = defaultdict(dict)
    for (unit, gz, back), obj in objs:
        if obj is not None and unit in want:
            ours[unit][(gz, back)] = object_listings(obj, want[unit])
    dl = DebugListings(dbg)
    rows = []
    for p in differing:
        theirs = dl.raw(int(p["debug"], 16))
        best = None
        for variant, fns in ours[p["unit"]].items():
            if p["symbol"] in fns:
                masked, unmasked, _, _ = compare(fns[p["symbol"]], theirs)
                score = (masked, unmasked) if mask else (unmasked, masked)
                if best is None or score > best[0]:
                    best = (score, masked, unmasked, variant)
        rec = {"retail": p["retail"], "name": p["name"], "status": p["status"], "debug": p["debug"],
               "masked": None, "unmasked": None, "variant": None}
        if best:
            rec.update(masked=round(best[1], 4), unmasked=round(best[2], 4), variant=variant_name(*best[3]))
        rows.append(rec)
    out["differs"] = rows
    OD_JSON.parent.mkdir(parents=True, exist_ok=True)
    OD_JSON.write_text(json.dumps(out, indent=1))
    frame_only = [r for r in rows if r["masked"] == 1.0]
    print("%d of our functions compiled at /Od have a byte-identical copy in AnimOnly (%d debug addresses named)" % (
        len(found), sum(len(a) for a in found.values())))
    if map_pairs:
        print("paired functions: %d exact at /Od, %d differ" % (exact, len(rows)))
        if mask:
            print("  of those, identical but for frame slots: %d (open %d); statements differ: %d; not compiled: %d"
                  % (len(frame_only), sum(1 for r in frame_only if r["status"] != "MATCH"),
                     sum(1 for r in rows if r["masked"] is not None and r["masked"] < 1.0),
                     sum(1 for r in rows if r["masked"] is None)))
        print("open functions whose /Od body already equals AnimOnly's (the statements are the original's):")
        for p in open_exact:
            print("  %s %-50s %s" % (p["retail"], p["name"], p["status"]))
        odiff = [r for r in rows if r["status"] != "MATCH"]
        print("open functions whose /Od body differs (`cmp` shows where): %d" % len(odiff))
        for r in odiff:
            print("  %s %-50s %-14s debug %s  %s" % (
                r["retail"], r["name"], r["status"].split()[0], r["debug"],
                "%5.1f%% alike, %5.1f%% with frame slots" % (pct(r["masked"]), pct(r["unmasked"]))
                if r["masked"] is not None else "not compiled"))
    if failed:
        print("did not compile: %s" % ", ".join(failed))
    print("written to %s" % OD_JSON)


def debug_names():
    """{debug address: name} from build/sdkdebug/od.json (our functions found at /Od, a name only where one fits)
    and the map's pairs."""
    names = defaultdict(set)
    try:
        for n, addrs in json.loads(OD_JSON.read_text())["found"].items():
            if len(addrs) <= 4:
                for a in addrs:
                    names[int(a, 16)].add(n)
    except (OSError, ValueError, KeyError):
        pass
    try:
        for p in json.loads((OUT / "map.json").read_text())["pairs"]:
            names[int(p["debug"], 16)] = {p["name"]}
    except (OSError, ValueError, KeyError):
        pass
    return {a: " | ".join(sorted(n)[:2]) for a, n in names.items()}


_MD = []


def disassembler():
    """One capstone x86-32 disassembler, made on first use."""
    if not _MD:
        import capstone
        _MD.append(capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32))
    return _MD[0]


# A memory operand's size, as capstone prints it (`xword` is x87's ten-byte real).
OPERAND_SIZE = {"byte": 1, "word": 2, "dword": 4, "qword": 8, "tbyte": 10, "xword": 10}
SIZED = re.compile(r"\b(byte|word|dword|qword|tbyte|xword) ptr \[")
# x87 instructions whose memory operand is not a real: integers (fild, fiadd...), the control and status words, the
# environment, BCD
NOT_REAL = re.compile(r"^(fi|fn?stcw|fldcw|fn?stsw|fn?stenv|fldenv|fn?save|frstor|fbld|fbstp)")


def real_text(v):
    """A real as a short literal: `90.0`, `0.5`, `1e-06`."""
    if v != v:
        return "nan"
    if v in (float("inf"), float("-inf")):
        return "-inf" if v < 0 else "inf"
    text = repr(v)
    return text if any(c in text for c in ".en") else text + ".0"


def const_text(mnemonic, size, raw):
    """The value an instruction reads from read-only data (`raw`, the operand's bytes): a real for an x87 operand
    (float, double or ten-byte), an integer otherwise; None when there is nothing to show."""
    if raw is None or len(raw) != size:
        return None
    if mnemonic.startswith("f") and not NOT_REAL.match(mnemonic):
        if size == 4:
            v = struct.unpack("<f", raw)[0]
            if v == v:
                for digits in range(1, 10):           # the shortest text that reads back as the same float
                    t = float("%.*g" % (digits, v))
                    try:
                        if struct.pack("<f", t) == raw:
                            return real_text(t)
                    except OverflowError:       # FLT_MAX rounded up
                        pass
            return real_text(v)
        if size == 8:
            return real_text(struct.unpack("<d", raw)[0])
        if size == 10:
            mant, se = int.from_bytes(raw[:8], "little"), int.from_bytes(raw[8:], "little")
            exp = se & 0x7FFF
            if exp == 0x7FFF:
                return "nan" if mant & ((1 << 63) - 1) else ("-inf" if se >> 15 else "inf")
            v = mant * 2.0 ** (exp - 16383 - 63) if mant else 0.0
            return real_text(-v if se >> 15 else v)
        return None
    if size in (1, 2, 4):
        v = int.from_bytes(raw, "little", signed=True)
        return str(v) if -0x10000 < v < 0x10000 else hex(v & ((1 << 8 * size) - 1))
    return None


def listing(code, addr, names=None, relocs=None, data=None):
    """A function's instructions as text that compares across the two builds: jump targets and addresses blanked,
    calls by name (`relocs`, {offset: symbol}, names our object's; `names` the debug image's). `data(where, n)`
    reads the `n` bytes of read-only data an absolute operand points to (where: the debug image's address, or the
    offset of our object's relocated field), or None: such an operand shows its value, `fdiv dword ptr [=90.0]`,
    which `normalize` reads as `[<addr>]` again."""
    md = disassembler()
    out = []
    for ins in md.disasm(code, addr):
        text = "%s %s" % (ins.mnemonic, ins.op_str)
        sized = SIZED.search(text)
        size = OPERAND_SIZE[sized.group(1)] if sized else 4
        if ins.mnemonic == "call":
            off = ins.address - addr + 1
            if relocs is not None and off in relocs:
                text = "call " + relocs[off]
            elif ins.op_str.startswith("0x"):
                text = "call " + (names or {}).get(int(ins.op_str, 16), "?")
        elif ins.mnemonic.startswith("j"):
            text = ins.mnemonic + " <label>"
        else:
            if relocs is None and data is not None:
                m = re.search(r"\[(0x[0-9a-f]{5,})\]", text)
                value = m and const_text(ins.mnemonic, size, data(int(m.group(1), 16), size))
                if value:
                    text = text.replace(m.group(0), "[=%s]" % value, 1)
            text = re.sub(r"0x[0-9a-f]{5,}", "<addr>", text)
            # an object's relocated field reads 0 or a section offset: blank it like an address (the exception
            # list, fs:[0], reads 0 in both builds)
            for off in sorted(relocs or ()):
                start = ins.address - addr
                if not start <= off < start + ins.size or relocs[off] == "__except_list":
                    continue
                v = int.from_bytes(code[off:off + 4], "little")
                for form in ("[%s]" % hex(v), "[%d]" % v):
                    if form in text:
                        value = data and const_text(ins.mnemonic, size, data(off, size))
                        text = text.replace(form, "[=%s]" % value if value else "[<addr>]", 1)
                        break
                else:
                    text = re.sub(r"(?<![\w\[])(%s|%d)(?![\w\]])" % (hex(v), v), "<addr>", text, count=1)
        out.append(text)
    # An assert's line operand is layout (see object_functions): `push <line>` three instructions before the call.
    for i, text in enumerate(out):
        if i >= 3 and text in ("call __assert", "call _assert") and out[i - 3].startswith("push 0x"):
            out[i - 3] = "push <line>"
    return out


# A frame slot: `[ebp - 0x10]`, `[ebp + 8]`, an array in the frame `[ebp + eax*4 - 0x98]` or `[ebp + ecx - 0x10]`.
# /Od keeps every named local in the frame, so two texts with the same statements read different slots when their
# locals are declared in another order or under other names (the slot order follows a hash of the names), or when
# /GZ pads the frame: masked (the displacement of every ebp-based operand), a comparison sees the statements.
FRAME_SLOT = re.compile(r"\[ebp((?: \+ e[a-z]{2}(?:\*[248])?)?) [-+] (?:0x[0-9a-f]+|[0-9]+)\]")
# a constant's value, shown by `listing`, compares as the address it was read from
CONST_VALUE = re.compile(r"\[=[^\]]*\]")


def normalize(text, mask_frame=True):
    """One listing line as the comparison reads it: a call without its callee (our object names a decorated symbol,
    the debug image an address or a pair's name), a constant's value as `[<addr>]` and, under `mask_frame`, frame
    slots as `[ebp X]` (`[ebp + eax*4 X]`)."""
    if text.startswith("call "):
        return "call"
    if "[=" in text:
        text = CONST_VALUE.sub("[<addr>]", text)
    return FRAME_SLOT.sub(r"[ebp\1 X]", text) if mask_frame else text


class FrameAligned:
    """The frame order compared along the statements' alignment: `sm`, the matcher of the masked listings, aligns
    the two, and along each run it aligns the lines whose frame slots differ (`a`, `b`: the unmasked listings) are
    replaced. A matcher of the unmasked listings aligns on its own and, after locals moved, pairs lines across
    statements and shows swaps that aren't there."""

    def __init__(self, sm, a, b):
        self.a, self.b = a, b
        self.opcodes = []
        for tag, a0, a1, b0, b1 in sm.get_opcodes():
            if tag != "equal":
                self.opcodes.append((tag, a0, a1, b0, b1))
                continue
            k = 0
            while k < a1 - a0:
                same = a[a0 + k] == b[b0 + k]
                j = k
                while j < a1 - a0 and (a[a0 + j] == b[b0 + j]) == same:
                    j += 1
                self.opcodes.append(("equal" if same else "replace", a0 + k, a0 + j, b0 + k, b0 + j))
                k = j

    def get_opcodes(self):
        return self.opcodes

    def ratio(self):
        n = len(self.a) + len(self.b)
        same = sum(a1 - a0 for tag, a0, a1, _, _ in self.opcodes if tag == "equal")
        return 2.0 * same / n if n else 1.0


def compare(ours, theirs):
    """(masked ratio, unmasked ratio, masked matcher, unmasked alignment) of two listings: the masked ratio measures
    the statements, the unmasked one the frame order as well, along the statements' alignment (FrameAligned)."""
    import difflib
    sm = difflib.SequenceMatcher(None, [normalize(t) for t in ours], [normalize(t) for t in theirs],
                                 autojunk=False)
    framed = FrameAligned(sm, [normalize(t, False) for t in ours], [normalize(t, False) for t in theirs])
    return sm.ratio(), framed.ratio(), sm, framed


def object_listings(obj, only=None):
    """{decorated name: listing} of the functions an object defines (COMDAT copies included); `only`, a set of
    names, limits it to those."""
    import coff
    o = coff.Coff(obj.read_bytes())
    out = {}
    for sym in o.symbols.values():
        if not sym.defined or sym.name in out:
            continue
        if only is not None:
            if sym.name not in only:
                continue
        elif not (sym.is_function and sym.storage in (coff.SC_EXTERNAL, coff.SC_STATIC)):
            continue
        lo, hi = o.function_extent(sym)
        sec = o.section(sym.section)
        table = coff_table(o, sym, lo, hi)
        if table is not None:
            hi = lo + table
        relocs = {off - lo: o.symbols[si].name for off, si, _ in sec.relocs if lo <= off < hi}
        targets = {off - lo: si for off, si, _ in sec.relocs if lo <= off < hi}

        def data(off, n, sec=sec, lo=lo, targets=targets):
            """The read-only bytes our relocated field at `off` points to (a constant), or None."""
            t = o.symbols[targets[off]] if off in targets else None
            if t is None or t.section <= 0:
                return None
            ts = o.section(t.section)
            if ts.is_code or ts.is_bss or ts.flags & SCN_MEM_WRITE:
                return None
            base = t.value + int.from_bytes(sec.data[lo + off:lo + off + 4], "little")
            return ts.data[base:base + n] if base + n <= len(ts.data) else None
        out[sym.name] = listing(sec.data[lo:hi], 0, relocs=relocs, data=data)
    return out


def od_variants(unit, pairs=None):
    """[(gz, back end)]: each way `od` compiles a unit (without and with /GZ, each back end of od_backends)."""
    return [(gz, back) for gz in (False, True) for back in od_backends(unit, pairs)]


def variant_name(gz, back):
    return "/Od" + (" /GZ" if gz else "") + (" (%s back end)" % back if back else "")


def our_listings(unit, symbols, pairs=None):
    """{(gz, back): {symbol: listing}} for each /Od variant of the unit that compiles."""
    out = {}
    for gz, back in od_variants(unit, pairs):
        obj = od_compile(unit, gz, back)
        if obj is not None:
            out[(gz, back)] = object_listings(obj, set(symbols))
    return out


def load_pairs():
    try:
        return json.loads((OUT / "map.json").read_text())["pairs"]
    except (OSError, ValueError, KeyError):
        return []


class DebugListings:
    """AnimOnly's functions as listings, made on first use: `raw(a)` the text, `key(a, mask)` the normalized tuple,
    `index(mask)` every start by its normalized listing."""

    def __init__(self, dbg, names=None):
        self.dbg = dbg
        self.names = dict(names if names is not None else debug_names())
        if dbg.assert_fn:
            self.names[dbg.assert_fn] = "_assert"
        self._raw, self._key, self._index = {}, {}, {}

    def raw(self, a):
        if a not in self._raw:
            self._raw[a] = listing(self.dbg.img.read(a, self.dbg.end_of(a) - a), a, names=self.names,
                                   data=self.dbg.rodata)
        return self._raw[a]

    def key(self, a, mask=True):
        k = (a, mask)
        if k not in self._key:
            self._key[k] = tuple(normalize(t, mask) for t in self.raw(a))
        return self._key[k]

    BUCKETS = 1024

    def bound(self, mine, candidates, mask=True):
        """An upper bound of each candidate's ratio against the normalized listing `mine`: the instruction bags'
        overlap (difflib's quick_ratio) over hashed buckets, which can only overstate it, for all candidates at once."""
        import numpy as np
        if ("bags", mask) not in self._index:
            pos = {a: k for k, a in enumerate(self.dbg.starts)}
            bags = np.zeros((len(self.dbg.starts), self.BUCKETS), dtype=np.uint16)
            for a, k in pos.items():
                for t in self.key(a, mask):
                    bags[k, hash(t) % self.BUCKETS] += 1
            self._index[("bags", mask)] = (pos, bags, bags.sum(1, dtype=np.int64))
        pos, bags, sizes = self._index[("bags", mask)]
        v = np.zeros(self.BUCKETS, dtype=np.uint16)
        for t in mine:
            v[hash(t) % self.BUCKETS] += 1
        rows = np.fromiter((pos[a] for a in candidates), dtype=np.int64, count=len(candidates))
        common = np.minimum(bags[rows], v).sum(1, dtype=np.int64)
        total = sizes[rows] + len(mine)
        return np.where(total > 0, 2.0 * common / np.maximum(total, 1), 0.0)

    def index(self, mask=True):
        if mask not in self._index:
            idx = defaultdict(list)
            for a in self.dbg.starts:
                idx[self.key(a, mask)].append(a)
            self._index[mask] = idx
        return self._index[mask]


def bag_bound(mine, candidates, key, mask=True):
    """difflib's quick_ratio of `mine` against each candidate's listing: the overlap of the instruction bags, an
    upper bound of the ratio (exact, one candidate at a time; DebugListings.bound does it for all at once)."""
    have = Counter(mine)
    out = []
    for a in candidates:
        theirs = key(a, mask)
        common = sum(min(n, have[t]) for t, n in Counter(theirs).items())
        total = len(mine) + len(theirs)
        out.append(2.0 * common / total if total else 0.0)
    return out


def lcs_masks(seq):
    """{item: bitmask of its positions in `seq`}, for lcs_length."""
    masks = defaultdict(int)
    for i, t in enumerate(seq):
        masks[t] |= 1 << i
    return masks


def lcs_length(masks, n, other):
    """The length of the longest common subsequence of a sequence of `n` items (`masks` = lcs_masks of it) and
    `other`, bit-parallel (Hyyro 2004): one big-integer step per item of `other`. 2 * LCS / (n + m) bounds difflib's
    ratio from above, since difflib's matching blocks are one common subsequence."""
    full = (1 << n) - 1
    v = full
    for t in other:
        u = v & masks.get(t, 0)
        v = ((v + u) | (v - u)) & full
    return n - bin(v).count("1")


def rank(ours, candidates, key, top=4, mask_frame=True, bound=None):
    """The `top` candidates most like `ours`, best first: [(ratio, unmasked ratio, address)]. `candidates` are
    addresses, `key(a, mask)` their normalized listings. Ranked by difflib's ratio of the masked listings (the
    statements; with `mask_frame` off, the frame order along their alignment: FrameAligned), ties by the unmasked
    ratio, then the address.

    Best first over upper bounds: every candidate starts at its instruction bags' overlap (`bound(mine, candidates,
    mask)`, else bag_bound), tightened to its longest common subsequence, then to the ratio itself, always refining
    the candidate with the highest bound, until `top` exact ratios lead every bound left. The frame order's ratio
    counts a common subsequence of the unmasked listings, so the bounds of those hold for it too."""
    import difflib
    import heapq
    mine = [normalize(t, mask_frame) for t in ours]
    candidates = list(candidates)
    bounds = (bound or (lambda m, c, k: bag_bound(m, c, key, k)))(mine, candidates, mask_frame)
    sm = difflib.SequenceMatcher(None, autojunk=False)
    # ours first, as `compare` and `cmp` read it (the ratio is not quite symmetric); always the masked listings, which
    # align the frame order too
    sm.set_seq1(mine if mask_frame else [normalize(t) for t in ours])
    masks, n = lcs_masks(mine), len(mine)

    def ratio(a):
        sm.set_seq2(key(a, True))
        return sm.ratio() if mask_frame else FrameAligned(sm, mine, key(a, False)).ratio()
    # (-bound, address, level): level 0 the bag bound, 1 the LCS bound, 2 difflib's ratio itself
    heap = [(-float(b), a, 0) for b, a in zip(bounds, candidates)]
    heapq.heapify(heap)
    done = []                  # [(ratio, address)] in the order they come out: best first
    while heap and len(done) < top:
        nb, a, level = heapq.heappop(heap)
        if level == 2:
            done.append((-nb, a))
            # an exact ratio tied with this one may still sit under a bound equal to it
            continue
        theirs = key(a, mask_frame)
        if level == 0:
            r = 2.0 * lcs_length(masks, n, theirs) / (n + len(theirs)) if n + len(theirs) else 0.0
        else:
            r = ratio(a)
        heapq.heappush(heap, (-r, a, level + 1))
    # candidates tied with the last one kept (equal ratios break by the unmasked ratio)
    last = done[-1][0] if done else None
    while heap and last is not None and -heap[0][0] >= last:
        nb, a, level = heapq.heappop(heap)
        r = ratio(a) if level < 2 else -nb
        if r >= last:
            done.append((r, a))
    out = []
    for r, a in done:
        _, unmasked, _, _ = compare(ours, list(key(a, False))) if mask_frame else (r, r, None, None)
        out.append((r, unmasked, a))
    out.sort(key=lambda x: (-x[0], -x[1], x[2]))
    return out[:top]


def pct(ratio, digits=1):
    """A ratio as a percentage rounded down to `digits` decimals: 100.0 only for an identical pair."""
    scale = 10 ** digits
    return math.floor(ratio * 100 * scale + 1e-6) / scale


def print_diff(ours, theirs, sm, show_all=False):
    for tag, a0, a1, b0, b1 in sm.get_opcodes():
        if tag == "equal" and not show_all:
            print("    ... %d alike" % (a1 - a0))
            continue
        for k in range(max(a1 - a0, b1 - b0)):
            o = ours[a0 + k] if a0 + k < a1 else ""
            t = theirs[b0 + k] if b0 + k < b1 else ""
            print("  %s %-52s | %s" % (" " if tag == "equal" else "*", o[:52], t))


def retail_row(addr):
    """The report row of our annotated function at retail `addr`, or None."""
    return next((r for r in report_rows() if int(r["addr"], 16) == addr and r.get("symbol")), None)


def cmd_cmp(args):
    """One function: our unit compiled at /Od against AnimOnly's copy (its map pair, or `--debug`), side by side.
    Each /Od variant is compiled; the one with the best masked ratio is shown."""
    dbg = load(args.exe)
    want = int(args.addr, 16)
    pairs = load_pairs()
    p = next((x for x in pairs if int(x["retail"], 16) == want or int(x["debug"], 16) == want), None)
    if args.debug:
        if p is None or int(p["retail"], 16) != want:
            row = retail_row(want)
            if row is None:
                sys.exit("0x%X is not an annotated function of ours" % want)
            p = dict(retail=row["addr"], name=row["name"], unit=row["unit"], symbol=row["symbol"], file=None)
        d = dbg.function_of(int(args.debug, 16))
    elif p is None or not p.get("symbol"):
        sys.exit("0x%X is not paired (run `sdkdebug.py map`, or name a debug function with --debug)" % want)
    else:
        d = int(p["debug"], 16)
    mask = not args.no_mask_frame
    theirs = DebugListings(dbg).raw(d)
    best = None
    for (gz, back), fns in our_listings(p["unit"], [p["symbol"]], pairs).items():
        ours = fns.get(p["symbol"])
        if ours is None:
            continue
        masked, unmasked, smm, smu = compare(ours, theirs)
        score = (masked, unmasked) if mask else (unmasked, masked)
        if best is None or score > best[0]:
            best = (score, gz, back, ours, masked, unmasked, smm if mask else smu)
    if best is None:
        sys.exit("%s does not compile at /Od, or doesn't define %s" % (p["unit"], p["symbol"]))
    _, gz, back, ours, masked, unmasked, sm = best
    print("%s %s: debug 0x%06X (%s), ours at %s: %.1f%% alike, %.1f%% with frame slots%s" % (
        p["retail"], p["name"], d, short(p["file"]) if p.get("file") else "-", variant_name(gz, back),
        pct(masked), pct(unmasked), "" if mask else " (shown with frame slots)"))
    print_diff(ours, theirs, sm, args.all)
    differ = constants_differ(ours, theirs, sm)
    if differ:
        print("constants that differ on lines the comparison reads alike (both read from read-only data):")
        for o, t in differ:
            print("  * %-52s | %s" % (o[:52], t))


def constants_differ(ours, theirs, sm):
    """[(our line, their line)] along the alignment's equal runs whose constants' values differ: the comparison reads
    every constant as `[<addr>]`, so `fdiv [=90.0]` against `fdiv [=180.0]` sits in a run it counts alike."""
    out = []
    for tag, a0, a1, b0, b1 in sm.get_opcodes():
        if tag != "equal":
            continue
        for o, t in zip(ours[a0:a1], theirs[b0:b1]):
            if "[=" in o and "[=" in t and CONST_VALUE.findall(o) != CONST_VALUE.findall(t):
                out.append((o, t))
    return out


def search_range(dbg, args, runs=None):
    """The debug starts a search compares against: `--range LO HI`, else the object runs whose file matches
    `--file`, else every start."""
    if getattr(args, "range", None):
        lo, hi = (int(x, 16) for x in args.range)
        return [a for a in dbg.starts if lo <= a < hi]
    if getattr(args, "file", None):
        runs = runs if runs is not None else objects(dbg)
        got = sorted({f for path, _, _, fns in runs if re.search(args.file, path, re.I) for f in fns})
        if not got:
            sys.exit("no debug object's file matches %r (see `sdkdebug.py files --objects`)" % args.file)
        return got
    return dbg.starts


def unit_files(unit, pairs):
    """The original files our unit's paired functions come from."""
    return {p["file"] for p in pairs if p["unit"] == unit and p.get("file")}


def hit_record(r, u, a, variant, fof, files, paired):
    return {"debug": "0x%06X" % a, "masked": round(r, 4), "unmasked": round(u, 4), "variant": variant_name(*variant),
            "file": short(fof[a]) if a in fof else None, "in_unit_file": fof.get(a) in files,
            "paired_with": paired.get(a)}


def search_one(ours_by_variant, symbol, candidates, key, top, mask, bound=None):
    """Rank `candidates` against each variant's listing of `symbol`; the best `top` over all variants:
    [(ratio, unmasked, address, variant)]."""
    found = {}
    for variant, fns in ours_by_variant.items():
        ours = fns.get(symbol)
        if ours is None:
            continue
        for r, u, a in rank(ours, candidates, key, top, mask, bound):
            if a not in found or (r, u) > found[a][:2]:
                found[a] = (r, u, a, variant)
    return sorted(found.values(), key=lambda x: (-x[0], -x[1], x[2]))[:top]


def cmd_search(args):
    """Our function compiled at /Od against every debug function in a range: the best candidates for its copy."""
    if args.all_unpaired:
        return search_all(args)
    dbg = load(args.exe)
    pairs = load_pairs()
    if len(args.target) == 1 and re.fullmatch(r"(0x)?[0-9A-Fa-f]+", args.target[0]):
        row = retail_row(int(args.target[0], 16))
        if row is None:
            sys.exit("%s is not an annotated function of ours" % args.target[0])
        unit = row["unit"]
        ours = our_listings(unit, [row["symbol"]], pairs)
    elif len(args.target) == 2:
        unit, sub = args.target
        ours = {}
        for gz, back in od_variants(unit, pairs):
            obj = od_compile(unit, gz, back)
            if obj is not None:
                ours[(gz, back)] = {n: t for n, t in object_listings(obj).items() if sub in n}
    else:
        sys.exit("search: a retail address, a unit and a symbol substring, or --all-unpaired")
    symbols = sorted({n for fns in ours.values() for n in fns})
    if not symbols:
        sys.exit("%s does not compile at /Od, or defines no such function" % unit)
    runs = objects(dbg)
    fof = file_of(runs)
    candidates = search_range(dbg, args, runs)
    dl = DebugListings(dbg)
    paired = {int(p["debug"], 16): "%s %s" % (p["retail"], p["name"]) for p in pairs}
    files = unit_files(unit, pairs)
    for sym in symbols:
        n = max(len(f[sym]) for f in ours.values() if sym in f)
        print("%s (%s, %d instructions) against %d debug functions:" % (sym, unit, n, len(candidates)))
        for r, u, a, variant in search_one(ours, sym, candidates, dl.key, args.top, not args.no_mask_frame, dl.bound):
            h = hit_record(r, u, a, variant, fof, files, paired)
            print("  0x%06X  %5.1f%%  %5.1f%% with frame slots  %4d instructions  %-30s %s%s%s" % (
                a, pct(r), pct(u), len(dl.raw(a)), h["file"] or "-", h["variant"],
                "  (our unit's file)" if h["in_unit_file"] else "",
                "  paired with %s" % h["paired_with"] if h["paired_with"] else ""))


SEARCH_JSON = OUT / "search.json"
SEARCH_PAIRS = OUT / "search_pairs.txt"


def candidate_pairs(results, rows, od_found, paired, least=0.95, margin=0.05):
    """config/sdkdebug_pairs.txt lines for the clear hits of a search: the best candidate at least `least` alike
    (masked) and `margin` ahead of the next, a debug function no pair holds and no other searched function's best
    hit; and each unpaired function whose /Od body od.json found byte for byte at one debug address only."""
    best = Counter(x["hits"][0]["debug"] for x in results if x["hits"])
    out = []
    taken = set()
    for x in results:
        h = x["hits"]
        if not h or h[0]["masked"] < least or h[0]["paired_with"] or best[h[0]["debug"]] > 1:
            continue
        nxt = h[1]["masked"] if len(h) > 1 else 0.0
        if h[0]["masked"] - nxt < margin:
            continue
        taken.add(h[0]["debug"])
        out.append("%s %s  # %s: search %.0f%% masked, %.0f%% with frame slots, next %.0f%%%s" % (
            x["retail"], h[0]["debug"], x["name"], pct(h[0]["masked"], 0), pct(h[0]["unmasked"], 0), pct(nxt, 0),
            ", in our unit's file" if h[0]["in_unit_file"] else ""))
    for r in rows:
        addrs = od_found.get(r["symbol"], [])
        if len(addrs) == 1 and int(addrs[0], 16) not in paired and addrs[0] not in taken:
            taken.add(addrs[0])
            out.append("%s %s  # %s: /Od body byte-identical (od.json), one copy" % (r["addr"], addrs[0], r["name"]))
    return out


def search_all(args):
    """Every NetImmerse function without a map pair that is open, or whose /Od body od.json found nowhere in
    AnimOnly, ranked against the debug image (or `--file`/`--range`); candidates to build/sdkdebug/search.json."""
    from concurrent.futures import ThreadPoolExecutor
    import time
    t0 = time.time()
    dbg = load(args.exe)
    pairs = load_pairs()
    paired_r = {int(p["retail"], 16) for p in pairs}
    try:
        od_found = json.loads(OD_JSON.read_text())["found"]
    except (OSError, ValueError, KeyError):
        od_found = {}
    rows = [r for r in report_rows() if r["unit"].startswith(args.module) and r.get("symbol") and "size" in r
            and not r.get("static") and int(r["addr"], 16) not in paired_r
            and (not args.unit or re.search(args.unit, r["unit"]))]
    want = [r for r in rows if r["status"] != "MATCH" or (not args.open_only and r["symbol"] not in od_found)]
    by_unit = defaultdict(list)
    for r in want:
        by_unit[r["unit"]].append(r)
    jobs = [(u, gz, back) for u in sorted(by_unit) for gz, back in od_variants(u, pairs)]
    with ThreadPoolExecutor(max_workers=os.cpu_count() or 4) as pool:
        objs = list(pool.map(lambda j: (j, od_compile(*j)), jobs))
    ours = defaultdict(dict)
    for (u, gz, back), obj in objs:
        if obj is not None:
            ours[u][(gz, back)] = object_listings(obj, {r["symbol"] for r in by_unit[u]})
    t1 = time.time()
    runs = objects(dbg)
    fof = file_of(runs)
    dl = DebugListings(dbg)
    paired = {int(p["debug"], 16): "%s %s" % (p["retail"], p["name"]) for p in pairs}
    mask = not args.no_mask_frame
    candidates = search_range(dbg, args, runs)
    out = []
    for u in sorted(by_unit):
        files = unit_files(u, pairs)
        for r in sorted(by_unit[u], key=lambda r: int(r["addr"], 16)):
            n = max((len(f[r["symbol"]]) for f in ours[u].values() if r["symbol"] in f), default=0)
            rec = {"retail": r["addr"], "name": r["name"], "unit": u, "symbol": r["symbol"], "status": r["status"],
                   "instructions": n, "hits": []}
            if n:
                rec["hits"] = [hit_record(x, y, a, v, fof, files, paired)
                               for x, y, a, v in search_one(ours[u], r["symbol"], candidates, dl.key, args.top, mask,
                                                          dl.bound)]
            out.append(rec)
    SEARCH_JSON.parent.mkdir(parents=True, exist_ok=True)
    SEARCH_JSON.write_text(json.dumps(out, indent=1))
    lines = candidate_pairs(out, rows, od_found, paired)
    SEARCH_PAIRS.write_text("".join(line + "\n" for line in lines))
    t2 = time.time()
    strong = [x for x in out if x["hits"] and x["hits"][0]["masked"] >= 0.9]
    print("%d unpaired functions searched (%d open) over %d debug functions; compile %.0fs, search %.0fs" % (
        len(out), sum(1 for x in out if x["status"] != "MATCH"), len(candidates), t1 - t0, t2 - t1))
    print("best candidate 90%% alike or more (masked): %d, %d of them identical but for frame slots" % (
        len(strong), sum(1 for x in strong if x["hits"][0]["masked"] == 1.0)))
    print("open functions (retail, best debug candidates: masked / with frame slots, in-file = in our unit's file):")
    for x in out:
        if x["status"] == "MATCH":
            continue
        hits = "; ".join("%s %.0f/%.0f%s" % (h["debug"], pct(h["masked"], 0), pct(h["unmasked"], 0),
                                             " in-file" if h["in_unit_file"] else "") for h in x["hits"][:3])
        print("  %s %-48s %s" % (x["retail"], x["name"][:48], hits or "(not compiled at /Od)"))
    print("written to %s; read a hit with `cmp <retail> --debug <addr>`, then list it in %s for `map`" % (
        SEARCH_JSON, CONFIRMED.relative_to(ROOT)))
    print("%d candidate pairs (clear hits and unique /Od copies) in %s, in that file's format (`map --pairs`)" % (
        len(lines), SEARCH_PAIRS))


MPE_DEFAULT = "C:/Recomp/bcsdk-1.1/Tools/ModelPropertyEditor/ModelPropertyEditor_s.exe"


def undecorate_type(name):
    """`Foo` from `.?AVFoo@@`; a nested or template name keeps its decorated tail (`.?AV?$NiTArray@...`)."""
    m = re.match(r"^\.\?A[VU](\w+)@@$", name)
    return m.group(1) if m else name


def msvc_rtti(img):
    """{class: [direct bases]} from the compiler's RTTI (/GR): each complete object locator names a type descriptor
    and its class hierarchy descriptor, whose base class array lists the class, then its bases depth first, each with
    the number of bases it contains (so a direct base is one the walk reaches from the class itself)."""
    names = {}
    for sec in img.sections:
        if sec[0] not in (".data", ".rdata"):
            continue
        data = img.read(sec[1], min(sec[2], sec[4]))
        for m in re.finditer(rb"\.\?A[VU][\w@?$<>,]+?@@\0", data):
            names[sec[1] + m.start() - 8] = m.group()[:-1].decode("latin-1")   # descriptor: vftable, spare, name
    out = {}
    for sec in img.sections:
        if sec[0] != ".rdata":
            continue
        data = img.read(sec[1], min(sec[2], sec[4]))
        for k in range(0, len(data) - 20, 4):
            sig, off, cd, td, chd = struct.unpack_from("<5I", data, k)
            if sig != 0 or td not in names:
                continue
            hdr = img.read(chd, 16)
            if hdr is None:
                continue
            _, attrs, nbases, arr = struct.unpack("<4I", hdr)
            if not 0 < nbases < 64:
                continue
            bcds = [img.dword(arr + 4 * i) for i in range(nbases)]
            entries = []
            for b in bcds:
                raw = img.read(b, 24) if b else None
                if raw is None:
                    break
                btd, contained = struct.unpack_from("<II", raw)
                entries.append((names.get(btd, "?"), contained))
            if len(entries) != nbases:
                continue
            cls = undecorate_type(entries[0][0])
            direct, i = [], 1
            while i < len(entries):
                direct.append(undecorate_type(entries[i][0]))
                i += 1 + entries[i][1]
            out.setdefault(cls, direct)
    return out


def cmd_rtti(args):
    """The class hierarchy a /GR build of the code base records, against our classes."""
    with open(args.exe or MPE_DEFAULT, "rb") as f:
        img = pe.Image(f.read())
    tree = msvc_rtti(img)
    types = load_types()
    rows = []
    for cls in sorted(tree):
        if args.prefix and not re.match(args.prefix, cls):
            continue
        ours = types.get(cls)
        our_bases = [b["name"] for b in ours.get("bases", [])] if ours else None
        if ours is None:
            verdict = "missing"
        elif our_bases == tree[cls]:
            verdict = "agrees"
        else:
            verdict = "bases differ"
        rows.append((cls, tree[cls], our_bases, verdict))
    for cls, bases, our_bases, verdict in rows:
        if args.all or verdict != "agrees":
            print("%-36s : %-40s %s%s" % (cls, ", ".join(bases) or "-", verdict,
                                         "" if verdict != "bases differ" else " (ours: %s)" % (
                                             ", ".join(our_bases) or "-")))
    print("%d classes in the RTTI (%s)" % (len(rows), ", ".join("%s %d" % kv for kv in Counter(
        r[3] for r in rows).most_common())))


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--exe", help="AnimOnly.exe (default: STBC_ANIMONLY, project.json's animonly, %s)" % DEFAULT_EXE)
    sub = ap.add_subparsers(dest="cmd")
    p = sub.add_parser("asserts")
    p.add_argument("--file")
    p.add_argument("--expr")
    p = sub.add_parser("files")
    p.add_argument("--all", action="store_true")
    p.add_argument("--objects", action="store_true", help="widen each run to its object (its initializers on)")
    p = sub.add_parser("fn")
    p.add_argument("addr")
    p = sub.add_parser("map")
    p.add_argument("--json")
    p.add_argument("--pairs", help="confirmed pairs to anchor the map (default config/sdkdebug_pairs.txt)")
    p = sub.add_parser("find")
    p.add_argument("source")
    p.add_argument("--flags", default=DEBUG_FLAGS)
    p.add_argument("--compiler", choices=("rtm", "sp4", "pp"))
    p.add_argument("--include", action="append", default=[])
    p.add_argument("--only")
    p.add_argument("--show", action="store_true")
    p.add_argument("--no-mask-frame", action="store_true", help="byte hits only, no frame-masked lookup")
    p = sub.add_parser("cmp")
    p.add_argument("addr")
    p.add_argument("--all", action="store_true")
    p.add_argument("--debug", help="the debug function to compare against (one the map hasn't paired)")
    p.add_argument("--no-mask-frame", action="store_true", help="rank and show with frame slots")
    p = sub.add_parser("search")
    p.add_argument("target", nargs="*", help="a retail address, or a unit and a symbol substring")
    p.add_argument("--file", help="a regex over the debug objects' files: search their runs only")
    p.add_argument("--range", nargs=2, metavar=("LO", "HI"), help="search the debug starts in [LO, HI)")
    p.add_argument("--top", type=int, default=4)
    p.add_argument("--no-mask-frame", action="store_true", help="rank with frame slots")
    p.add_argument("--all-unpaired", action="store_true",
                   help="every unpaired function that is open or has no /Od copy: build/sdkdebug/search.json")
    p.add_argument("--open-only", action="store_true", help="with --all-unpaired: the open functions only")
    p.add_argument("--module", default="netimmerse/")
    p.add_argument("--unit", help="with --all-unpaired: a regex over unit names")
    p = sub.add_parser("rtti")
    p.add_argument("--prefix", help="a regex the class names start with (TG|Ni)")
    p.add_argument("--all", action="store_true", help="also the classes that agree")
    p = sub.add_parser("od")
    p.add_argument("--module", default="netimmerse/")
    p.add_argument("--unit", help="a regex over unit names")
    p.add_argument("--no-mask-frame", action="store_true", help="compare the differing pairs with frame slots")
    args = ap.parse_args(argv)
    {"asserts": cmd_asserts, "files": cmd_files, "fn": cmd_fn, "map": cmd_map, "find": cmd_find, "od": cmd_od, "cmp": cmd_cmp, "search": cmd_search, "rtti": cmd_rtti}.get(args.cmd or "files")(args)


if __name__ == "__main__":
    main()
