"""Other builds of the same code as siblings: pair their functions with retail's and compare what each build calls.

    python tools/sibling.py [--build B] [--refresh]         # pair (cached) and print the coverage (B: demo, mpe,
                                                            # damagetool, exporter, or all; default demo)
    python tools/sibling.py fn 0x678C30 [--build B]         # one function: retail and the sibling side by side
                                                            # (`--build all`: every sibling's pair and call set)
    python tools/sibling.py callsets [--open] [--min-score N] [--top N] [--build B]
                                                            # functions whose direct call sets differ, by function
                                                            # and by callee
    python tools/sibling.py new [--top N] [--build B]       # sibling-only functions that paired functions call
    python tools/sibling.py flips [--all] [--build B]       # open functions whose sibling copy differs: only in
                                                            # frame slots/registers, schedule, layout, or in code
    python tools/sibling.py open [--min-score N] [--top N] [--unit RE] [--strong]
                                                            # every open function against every sibling: pair,
                                                            # similarity, and whose side each sibling takes where
                                                            # retail's and ours' call sets disagree
    python tools/sibling.py syms [ADDR ...] [--all]         # DamageTool's decorated names of retail's functions
                                                            # against ours: name, parameter, const, return type

The siblings (each only read: never run, never copied into the tree):
    demo        stbcdemo.exe, 2002-02-09, the DEMO branch of the game, same linker: nearly the same source
    mpe         the SDK's ModelPropertyEditor_s.exe, 2002-02 (stamp 0x3C5AF2ED), release: the TG framework,
                NetImmerse and its own SWIG App module; no symbols, no base relocations (MSVC RTTI only)
    damagetool  the SDK's DamageTool.exe, 2001-04, release, with a COFF symbol table: the damage/OBB/voxel code
                and NetImmerse, named (its linker folded identical bodies, so one address can carry several names)
    exporter    the SDK's STBCExporter.dlu, 2002-03, a release 3ds Max plugin with NetImmerse 3 (MaxImmerse)
AnimOnly (a debug build) is tools/sdkdebug.py's. A sibling's path comes from `--path` (one build), else the
environment variable STBC_<BUILD> (STBC_DEMO, STBC_MPE, STBC_DAMAGETOOL, STBC_EXPORTER), else `"siblings": {build:
path}` in config/project.json (the demo also `"sibling"`), else the default under C:/Recomp.

Where two builds made different inlining decisions, the sibling shows a helper's boundary that retail expands (or
the reverse): evidence for a hand-expanded body or an invented helper, never source. Where retail's and ours' call
sets disagree, a sibling that calls what retail calls sides with retail; two siblings doing so are a strong pointer.

Pairing. Function starts are found in each image the same way: the entry point, the code addresses that data points
at (vtables, function-pointer tables) or that code pushes or stores as an immediate, direct call targets, and the
code after a `ret`/`jmp` that no branch or jump table before it reaches (after `nop` fill to a 16-byte boundary a
`jmp` to it is a stub's tail jump; packed code such as the CRT library has no fill). Retail adds every annotated or
listed address. An image without base relocations (mpe) gets them rebuilt: every operand dword of a linear sweep
that points into the image and is printed as an operand, the jump tables that sweep finds, and every aligned dword
of .data/.rdata that points into the image (on DamageTool and the exporter, whose real tables check it, over 99%
agree). A function runs to the next start; its bytes are hashed with every base-relocated dword and every relative
call/jump out of the function masked. Seeds come first:
    symbol    (damagetool) a decorated name both sides define once: its COFF symbol and our annotated symbol
    rtti      a vtable both sides name: NetImmerse's NiRTTI (both), the MSVC RTTI of a /GR build (mpe,
              damagetool), DamageTool's `??_7` symbols, against retail's NiRTTI and config/vtables.txt; its slots
              pair in order
Then, until nothing changes:
    bytes     the masked bytes are identical and unique on both sides
    calls     in a paired function, the call, jump, code-pointer and data references at the same index (aligned
              by difflib where the pair differs) pair their targets; an unpaired callee also pairs when every
              byte-identical caller (a stub paired by order included) votes for the same sibling function
    vtable    a paired data address whose dwords are relocated code pointers pairs the slots
    string    a string referenced by exactly one function on each side pairs the two
    order     between two paired anchors (the link order is the same), the unpaired functions align on their bytes
              and mapped references, and pair where sizes are within a factor of two, no paired reference
              contradicts them, and (bytes differing) at least half the sibling's paired references agree; an order
              pair a reference later contradicts is dropped
Once that settles, the pairs the link order can't see, and settle again, until nothing changes:
    name      (damagetool) a qualified name (`Class::Method`) both sides give one unpaired function, DamageTool's
              classes renamed as ours (CLASS_RENAMES: the renames the class comments record, completed by the
              classes whose functions two or more pairs found without names put in one class of ours)
    neighbour the unpaired callees (and callers) of a pair, against the sibling's unpaired callees (callers) of its
              counterpart: each the other's single most alike candidate
    refs      the same bytes and the same code references, every one paired, unique on both sides (a run of stubs
              that a template copy's placement cut off from its anchors)
Two functions whose masked bytes differ pair only when their instruction sequences (addresses masked) are at least
40% alike (difflib's ratio): the similarity printed for each pair. Looser where other evidence holds: 30% in step
between two anchors that hold as many functions on each side, 25% for neighbours whose paired references agree
(two thirds of at least two), 20% for name pairs among overloads, 10% for a name only one function carries on each
side. A pair by order or neighbour is refused where both builds name the two as members of different classes
(after the renames); stronger evidence still pairs them, and `syms` shows the clash. The result is cached in build/sibling-retail.json
(retail's functions) and build/sibling-<build>.json until the image or build/report.json changes.

Call sets are read as tools/callset.py reads them (direct calls and import-slot calls; calls through a register
don't count). The sibling's targets are mapped to retail through the pairing (imports by name); a target without a
retail counterpart is printed as `<build>:0x...` (with DamageTool's name when it has one). `open` reads retail's and
ours' call sets from tools/callset.py's cache.
"""

import argparse
import bisect
import difflib
import hashlib
import json
import os
import re
import struct
import sys
from collections import Counter, defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import callset  # noqa: E402
import decomp  # noqa: E402
import pe  # noqa: E402

RETAIL_CACHE = decomp.BUILD / "sibling-retail.json"
FORMAT = 3
DEFAULT_DEMO = "C:/Recomp/bcdemo/stbcdemo.exe"
SDK = "C:/Recomp/bcsdk-1.1/Tools/"
# build: (default path, what it is)
BUILDS = {
    "demo": (DEFAULT_DEMO, "stbcdemo.exe, 2002-02-09, the DEMO branch"),
    "v10": ("C:/Recomp/bc-1.0-cd/Setup/stbc.exe", "stbc.exe 1.0 from the retail CD, 2002-01-29, the CDChecking build"),
    "mpe": (SDK + "ModelPropertyEditor/ModelPropertyEditor_s.exe", "ModelPropertyEditor_s.exe, 2002-02, TG + NI"),
    "damagetool": (SDK + "DamageTool/DamageTool.exe", "DamageTool.exe, 2001-04, COFF symbols"),
    "exporter": (SDK + "STBCExporter.dlu", "STBCExporter.dlu, 2002-03, NetImmerse 3"),
}
FILL = (0x90, 0xCC)
BRANCH = re.compile(r"^(j\w+|call|loop\w*)$")
ADDRESS = re.compile(r"^0x[0-9a-f]+$")
TABLE = re.compile(r"\[(?:\w+\*4 \+ )?(0x[0-9a-f]+)\]")
CODE_KINDS = ("c", "j", "t")   # a direct call, a jump out of the function, a relocated code pointer
MIN_SHAPE = 0.4                # two functions whose bytes differ pair only when this alike
NAME_SHAPE = 0.2               # ... or this alike, when both builds give them one qualified name (overloads)
UNIQUE_NAME_SHAPE = 0.1        # ... or this, when the name is the only one of its kind on each side
CALLEE_SHAPE = 0.25            # ... or this, for neighbours of a pair whose paired references agree
WINDOW_SHAPE = 0.3             # ... or this, in step between two anchors holding as many functions on each side
LOWEST_SHAPE = min(MIN_SHAPE, NAME_SHAPE, UNIQUE_NAME_SHAPE, CALLEE_SHAPE, WINDOW_SHAPE)
WEAK = ("order", "neighbour")   # evidence that a contradiction of the two builds' class names overrules
SHAPE_ADDRESS = re.compile(r"0x[0-9a-f]{5,8}")


def build_path(build="demo", arg=None):
    """The sibling's executable: `arg`, else STBC_<BUILD>, else project.json's "siblings" (the demo also
    "sibling"), else the default."""
    if arg:
        return Path(arg)
    env = os.environ.get("STBC_" + build.upper())
    if env:
        return Path(env)
    listed = (decomp.PROJECT.get("siblings") or {}).get(build)
    if not listed and build == "demo":
        listed = decomp.PROJECT.get("sibling")
    return Path(listed or BUILDS[build][0])


def demo_path(arg=None):
    return build_path("demo", arg)


def cache_path(build):
    return decomp.BUILD / ("sibling-%s.json" % build)


# ---------------------------------------------------------------------------
# reading one image

_cs = None


def disasm_lite(code, addr):
    global _cs
    if _cs is None:
        import capstone
        _cs = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    return _cs.disasm_lite(bytes(code), addr)


def immediate(ops, value):
    """True when the operands carry value as an immediate (`push 0x401230`, `mov dword ptr [eax], 0x401230`),
    not inside a memory operand (`[eax*4 + 0x401230]`)."""
    s = "0x%x" % value
    i = ops.find(s)
    while i >= 0:
        if ops.count("[", 0, i) == ops.count("]", 0, i) and not ops[i + len(s):i + len(s) + 1].isalnum():
            return True
        i = ops.find(s, i + 1)
    return False


def entry_point(img):
    p = struct.unpack_from("<I", img.data, 0x3C)[0]
    return img.base + struct.unpack_from("<I", img.data, p + 24 + 16)[0]


def image_size(img):
    p = struct.unpack_from("<I", img.data, 0x3C)[0]
    return struct.unpack_from("<I", img.data, p + 24 + 56)[0]


JUMP_TABLE = re.compile(r"\[\w+\*4 \+ (0x[0-9a-f]+)\]")
BYTE_TABLE = re.compile(r"byte ptr \[\w+ \+ (0x[0-9a-f]+)\]")


def synthetic_relocs(img, image_range=None):
    """The base relocations of an image linked without them (/FIXED), rebuilt: in .text, every dword of an
    instruction (a linear sweep) whose value points into the image and is printed among its operands, and the
    entries of the jump tables the sweep finds (`jmp [reg*4 + table]`, whose entries point into .text; a byte index
    table read by `mov`/`movzx byte ptr [reg + table]` is stepped over while its bytes index the jump table before
    it); in .data and .rdata, every aligned dword that points into the image (`image_range`: [lo, hi), by default
    from the headers)."""
    lo_img, hi_img = image_range or (img.base + 0x1000, img.base + image_size(img))
    _, tlo, tsize, _, traw = img.text
    thi = tlo + min(tsize, traw)
    code = img.read(tlo, thi - tlo)
    out = set()
    tables = {}       # table address -> "d" (dwords) or "b" (byte indices)
    pos, entries = tlo, 0
    while pos < thi:
        kind = tables.pop(pos, None)
        if kind == "d":
            entries = 0
            while pos + 4 <= thi and tlo <= struct.unpack_from("<I", code, pos - tlo)[0] < thi:
                out.add(pos)
                pos += 4
                entries += 1
            continue
        if kind == "b":
            while pos < thi and code[pos - tlo] < max(entries, 1):
                pos += 1
            continue
        stop = min((k for k in tables if k > pos), default=thi)
        start = pos
        for a, n, m, o in disasm_lite(code[pos - tlo:stop - tlo], pos):
            pos = a + n
            for k in range(n - 3):
                v = struct.unpack_from("<I", code, a - tlo + k)[0]
                if lo_img <= v < hi_img and ("0x%x" % v) in o:
                    out.add(a + k)
            hit = (JUMP_TABLE.search(o) if "*4 + 0x" in o else
                   BYTE_TABLE.search(o) if m.startswith("mov") and "byte ptr [" in o else None)
            if hit:
                t = int(hit.group(1), 16)
                if a < t < thi:
                    tables.setdefault(t, "d" if "*4" in hit.group(0) else "b")
                    stop = min(stop, t)
            if pos >= stop:
                pos = stop
                break
        if pos == start or (pos < stop and pos not in tables):
            pos += 1   # bytes that don't decode
    for name, va, vsize, _, rsize in img.sections:
        if name in (".data", ".rdata"):
            data = img.read(va, min(vsize, rsize)) or b""
            for k in range(0, len(data) - 3, 4):
                if lo_img <= struct.unpack_from("<I", data, k)[0] < hi_img:
                    out.add(va + k)
    return out


class Code:
    """One image's .text read as functions: {start: {"size", "hash", "refs", "calls"}}.

    refs: [[kind, target]] in instruction order, kind "c" (direct call), "j" (jump out of the function),
    "i" (call through a dword: an import slot or a function pointer), "t" (relocated pointer into .text),
    "d" (relocated pointer elsewhere). calls: callset.call_targets of the function, {"0x...": n}."""

    def __init__(self, img, extra=()):
        self.img = img
        _, self.lo, size, _, _ = img.text
        self.hi = self.lo + size
        self.relocs = img.relocs
        self.reloc_sorted = sorted(img.relocs)
        self.starts = self.discover(extra)
        self.funcs = {}
        for k, s in enumerate(self.starts):
            end = self.starts[k + 1] if k + 1 < len(self.starts) else self.hi
            self.funcs[s] = self.read_function(s, end)

    @classmethod
    def cached(cls, img, funcs):
        """A Code over an image whose functions were read before (the cache)."""
        self = cls.__new__(cls)
        self.img = img
        _, self.lo, size, _, _ = img.text
        self.hi = self.lo + size
        self.relocs = img.relocs
        self.reloc_sorted = sorted(img.relocs)
        self.funcs = funcs
        self.starts = sorted(funcs)
        return self

    def shape(self, a):
        """The function's instructions with addresses masked: what the similarity of two builds' copies reads."""
        shapes = self.__dict__.setdefault("_shapes", {})
        s = shapes.get(a)
        if s is None:
            code = self.img.read(a, self.funcs[a]["size"]) or b""
            s = shapes[a] = ["%s %s" % (m, SHAPE_ADDRESS.sub("A", o)) for _, _, m, o in disasm_lite(code, a)]
        return s

    def in_text(self, a):
        return self.lo <= a < self.hi

    def target(self, site):
        return self.img.dword(site)

    def sites(self, lo, hi):
        i = bisect.bisect_left(self.reloc_sorted, lo)
        j = bisect.bisect_left(self.reloc_sorted, hi)
        return self.reloc_sorted[i:j]

    def seeds(self, extra):
        out = {entry_point(self.img)} | {a for a in extra if self.in_text(a)}
        for site in self.relocs:
            t = self.target(site)
            if t is not None and self.in_text(t) and not self.in_text(site):
                out.add(t)
        return out

    def walk(self, start, end):
        """(instructions, new starts): the instructions from start up to end (stopping at data: a relocated site at
        an instruction's start, or bytes that don't decode), and the starts they reveal: direct call targets, and
        the code after a `ret`/`jmp` and its fill to a 16-byte boundary that no branch or jump table before it
        reaches."""
        code = self.img.read(start, end - start) or b""
        # reach: the furthest label a conditional branch, a jump table or a local address reaches; jumped: the
        # furthest an unconditional jump reaches. After fill, only reach counts: a `jmp` to the code after the fill
        # is a tail jump to the next function (the `_$E` stubs), since a function has no internal alignment.
        ins, found, reach, jumped, ended, filled = [], set(), start, start, False, False
        for a, size, m, o in disasm_lite(code, start):
            if a in self.relocs:
                break
            # packed code (the CRT library) has no fill: there any code after a `ret`/`jmp` that nothing before
            # it reaches is the next function
            if ended and m not in ("nop", "int3") and a > reach and ((filled and a % 16 == 0) or a > jumped):
                found.add(a)
                break
            if ended and m in ("nop", "int3"):
                filled = True
                continue   # fill after the last instruction
            ended, filled = m in ("ret", "jmp"), False
            ins.append((a, size, m, o))
            if BRANCH.match(m) and ADDRESS.match(o):
                t = int(o, 16)
                if m == "call":
                    if self.in_text(t):
                        found.add(t)
                elif m == "jmp" and start <= t < end:
                    jumped = max(jumped, t)
                elif start <= t < end:
                    reach = max(reach, t)
            elif m == "jmp" and "*4" in o:
                hit = TABLE.search(o)
                if hit:   # a jump table: its entries are this function's labels
                    p = int(hit.group(1), 16)
                    while p in self.relocs:
                        t = self.target(p)
                        if t is None or not start <= t < end:
                            break
                        reach = max(reach, t)
                        p += 4
            for site in self.sites(a, a + size):
                t = self.target(site)
                if t is None or not self.in_text(t):
                    continue
                if not immediate(o, t):
                    if start <= t < end:   # a table of the function's own ([table + eax*4])
                        reach = max(reach, t)
                else:   # `push offset f`, `mov [x], offset f`: a function pointer
                    found.add(t)
        return ins, found

    def discover(self, extra):
        starts = set(self.seeds(extra))
        todo = sorted(starts)
        while todo:
            ordered = sorted(starts)
            new = set()
            for s in todo:
                k = bisect.bisect_right(ordered, s)
                end = ordered[k] if k < len(ordered) else self.hi
                _, found = self.walk(s, end)
                new |= {a for a in found if a not in starts and self.in_text(a)}
            starts |= new
            todo = sorted(new)
        return sorted(starts)

    def read_function(self, start, end):
        ins, _ = self.walk(start, end)
        stop = ins[-1][0] + ins[-1][1] if ins else start
        code = bytearray(self.img.read(start, end - start) or b"")
        n = len(code)
        while n > stop - start and code[n - 1] in FILL:
            n -= 1
        del code[n:]
        end = start + n
        refs = []
        masked = bytearray(code)
        for site in self.sites(start, end):
            masked[site - start:site - start + 4] = b"\0\0\0\0"
        for a, size, m, o in ins:
            for site in self.sites(a, a + size):
                t = self.target(site)
                if t is None or (start <= t < end and self.in_text(t)):
                    continue   # a label of the function's own (a jump table, a local address)
                if m == "call":
                    kind = "i"
                else:
                    kind = "t" if self.in_text(t) else "d"
                refs.append([kind, "0x%X" % t])
            if BRANCH.match(m) and ADDRESS.match(o):
                t = int(o, 16)
                if start <= t < end:
                    continue
                width = 4 if size >= 5 else 1
                masked[a + size - width - start:a + size - start] = b"\0" * width
                if m == "call":
                    refs.append(["c", "0x%X" % t])
                elif m.startswith("j"):
                    refs.append(["j", "0x%X" % t])
        calls = callset.call_targets([(m, o) for _, _, m, o in ins])
        return {"size": n, "hash": hashlib.sha1(bytes(masked)).hexdigest()[:16], "refs": refs,
                "calls": dict(calls)}

    def string_at(self, addr):
        """The printable C string at addr (at least 4 characters), else None."""
        b = self.img.read(addr, 1)
        if b is None:
            return None
        off = self.img.offset(addr)
        end = self.img.data.find(b"\0", off, off + 256)
        if end < 0 or end - off < 4:
            return None
        s = self.img.data[off:end]
        if all(0x20 <= c < 0x7F or c in (9, 10, 13) for c in s):
            return s.decode("latin-1")
        return None


# ---------------------------------------------------------------------------
# pairing

class Pairing:
    """retail -> demo pairs of functions (fmap) and data addresses (dmap), each with how it was found."""

    def __init__(self, retail, demo, rcode=None, dcode=None):
        self.r, self.d = retail, demo          # {start: func}
        self.rcode, self.dcode = rcode, dcode  # Code objects, for vtables, strings and shapes (optional in tests)
        self.fmap, self.finv, self.how = {}, {}, {}
        self.dmap, self.dinv = {}, {}
        self.ratios = {}
        self.seed_pairs = []    # [(retail, sibling, how)]: names both builds give one function
        self.rnames = {}        # {retail: qualified name} (ours, from the annotated symbols): see by_names
        self.dnames = {}        # {sibling: [qualified names]} (the sibling's symbols)
        self.renames = {}       # {sibling class: our class}: the known renames, completed by votes
        self.seed_tables = []   # [(retail slots, sibling slots)]: vtables both builds name

    def ratio(self, ra, da):
        """How alike the two functions' instructions are (addresses masked): 1.0 for the same masked bytes; below
        LOWEST_SHAPE only a bound. None without the images (tests, unless the ratio was set in `ratios`)."""
        if self.r[ra]["hash"] == self.d[da]["hash"]:
            return 1.0
        key = (ra, da)
        if key not in self.ratios:
            if self.rcode is None or self.dcode is None:
                return None
            sm = difflib.SequenceMatcher(None, self.rcode.shape(ra), self.dcode.shape(da), autojunk=False)
            bound = min(sm.real_quick_ratio(), sm.quick_ratio())
            self.ratios[key] = sm.ratio() if bound >= LOWEST_SHAPE else bound
        return self.ratios[key]

    def pair(self, ra, da, how, override=False, floor=MIN_SHAPE):
        """Pair ra with da unless either is paired already or their instructions are less alike than `floor`;
        `override` (a byte-identical caller's call index) replaces pairs found only by order."""
        if ra not in self.r or da not in self.d:
            return False
        if self.fmap.get(ra) == da:
            return False
        r = self.ratio(ra, da)
        if r is not None and r < floor:
            return False
        if how in WEAK and self.contradicts(ra, da):
            return False
        if override:
            old_d, old_r = self.fmap.get(ra), self.finv.get(da)
            if (old_d is not None and self.how[ra] != "order") or \
                    (old_r is not None and self.how[old_r] != "order"):
                return False
            if old_d is not None:
                self.unpair(ra)
            if old_r is not None:
                self.unpair(old_r)
        if ra in self.fmap or da in self.finv:
            return False
        self.fmap[ra], self.finv[da], self.how[ra] = da, ra, how
        return True

    def unpair(self, ra):
        da = self.fmap.pop(ra)
        del self.finv[da]
        del self.how[ra]

    def pair_data(self, ra, da):
        if ra in self.dmap or da in self.dinv:
            return False
        self.dmap[ra], self.dinv[da] = da, ra
        return True

    def run(self, rounds=24):
        """Seeds, then the link-order pairing to its fixpoint (settle); then the pairs it can't see (names, the
        neighbours of pairs, stubs placed elsewhere) and settle again, until nothing changes. The order pairs come
        first: a pair found later that the link order puts elsewhere (a template copy) would cut its anchors."""
        self.seeds()
        self.by_bytes()
        self.settle(rounds)
        for _ in range(rounds):
            before = dict(self.fmap)
            self.by_names()
            self.neighbours()
            self.by_keys()
            self.settle(rounds)
            if self.fmap == before:
                break
        return self

    def settle(self, rounds=24):
        """Strong evidence to a fixpoint, then one loose order pass, and again, until nothing changes."""
        for _ in range(rounds):
            before = dict(self.fmap)
            self.prune()
            self.propagate()
            self.votes()
            self.vtables()
            self.strings()
            self.propagate()
            self.order(strict=True)
            self.propagate()
            if self.fmap == before:
                self.order(strict=False)
                self.propagate()
                if self.fmap == before:
                    break

    def seeds(self):
        for ra, da, how in self.seed_pairs:
            self.pair(ra, da, how)
        for rs, ds in self.seed_tables:
            for x, y in zip(rs, ds):
                if x in self.r and y in self.d:
                    self.pair(x, y, "rtti")

    def by_bytes(self):
        rh, dh = defaultdict(list), defaultdict(list)
        for a, f in self.r.items():
            rh[f["hash"]].append(a)
        for a, f in self.d.items():
            dh[f["hash"]].append(a)
        for h, ra in rh.items():
            da = dh.get(h)
            if len(ra) == 1 and da and len(da) == 1 and self.r[ra[0]]["size"] >= 6:
                self.pair(ra[0], da[0], "bytes")

    def mapped(self, kind, t):
        """A demo reference as retail sees it, for aligning: the retail counterpart when paired."""
        a = int(t, 16)
        if kind in CODE_KINDS and a in self.finv:
            return (kind, "0x%X" % self.finv[a])
        if kind in ("d", "i") and a in self.dinv:
            return (kind, "0x%X" % self.dinv[a])
        return (kind, "demo:" + t)

    def similar(self, ra, da):
        fr, fd = self.r[ra], self.d[da]
        if fr["hash"] == fd["hash"]:
            return True
        return 0.5 <= (fr["size"] + 1) / (fd["size"] + 1) <= 2.0

    def compatible(self, ra, da):
        """False when the two have the same bytes but a reference already paired elsewhere contradicts them."""
        rr, dr = self.r[ra]["refs"], self.d[da]["refs"]
        if self.r[ra]["hash"] != self.d[da]["hash"] or len(rr) != len(dr):
            return True
        for (_, tr), (kd, td) in zip(rr, dr):
            m = self.mapped(kd, td)[1]
            if not m.startswith("demo:") and m != tr:
                return False
            if m.startswith("demo:") and int(tr, 16) in self.fmap:
                return False
        return True

    def votes(self):
        """Pair an unpaired callee with the demo function that every byte-identical paired caller (stubs paired by
        order included) calls at the same index, when two callers or one caller paired by stronger evidence agree,
        and no caller votes otherwise."""
        cast = defaultdict(Counter)
        back = defaultdict(set)
        strong = set()
        for ra, da in self.fmap.items():
            rr, dr = self.r[ra]["refs"], self.d[da]["refs"]
            if self.r[ra]["hash"] != self.d[da]["hash"] or len(rr) != len(dr):
                continue
            for (kr, tr), (kd, td) in zip(rr, dr):
                x, y = int(tr, 16), int(td, 16)
                if kr != kd or kr not in CODE_KINDS or x in self.fmap or y in self.finv:
                    continue
                if x in self.r and y in self.d:
                    cast[x][y] += 1
                    back[y].add(x)
                    if self.how[ra] != "order" or self.r[ra]["size"] >= 16:
                        strong.add((x, y))
        for x, ys in cast.items():
            if len(ys) != 1:
                continue
            (y, n), = ys.items()
            if back[y] == {x} and (n >= 2 or (x, y) in strong) and self.similar(x, y):
                self.pair(x, y, "calls")

    def contradicts(self, ra, da):
        """True when both builds name the two functions as members of different classes (the sibling's renamed as
        ours; any of its folded names may agree)."""
        q, names = self.rnames.get(ra), self.dnames.get(da)
        if q is None or len(q) < 2 or not names:
            return False
        theirs = [self.renamed(n) for n in names if n is not None]
        return bool(theirs) and all(len(n) >= 2 for n in theirs) and all(n[:-1] != q[:-1] for n in theirs)

    def class_votes(self):
        """Complete `renames` from the pairs found without names: a sibling class whose functions, each the only
        name at its address, pair with functions of one class of ours (at least twice, never with another class)
        is that class renamed."""
        votes = defaultdict(Counter)
        for ra, da in self.fmap.items():
            if self.how[ra] in ("name", "symbol", "order", "neighbour"):
                continue
            q, names = self.rnames.get(ra), self.dnames.get(da)
            if q is None or not names or len(names) != 1 or names[0] is None:
                continue
            if len(q) > 1 and len(names[0]) > 1:
                votes["::".join(names[0][:-1])]["::".join(q[:-1])] += 1
        for theirs, ours in votes.items():
            if theirs in self.renames or len(ours) != 1:
                continue
            (cls, n), = ours.items()
            if n >= 2 and cls != theirs:
                self.renames[theirs] = cls

    def renamed(self, q):
        """A sibling's qualified name with its classes renamed as ours (the longest renamed scope wins)."""
        for k in range(len(q) - 1, 0, -1):
            cls = self.renames.get("::".join(q[:k]))
            if cls is not None:
                return tuple(cls.split("::")) + q[k:]
        return q

    def by_names(self):
        """Pair the unpaired functions both builds give one qualified name (`Class::Method`, the sibling's classes
        renamed as ours), when their instructions are at least UNIQUE_NAME_SHAPE alike (one function of that name
        on each side) or NAME_SHAPE (overloads, which pair the most alike first). A pair found only by order gives
        way."""
        if not self.rnames or not self.dnames:
            return
        self.class_votes()
        rq, dq = defaultdict(list), defaultdict(set)
        for ra, q in self.rnames.items():
            if q is not None and ra in self.r and not PLACEHOLDER_MEMBER.match(q[-1]) and                     (ra not in self.fmap or self.how[ra] == "order"):
                rq[q].append(ra)
        for da, names in self.dnames.items():
            if da not in self.d or (da in self.finv and self.how[self.finv[da]] != "order"):
                continue
            for q in names:
                if q is not None:
                    dq[self.renamed(q)].add(da)
        for q, ras in rq.items():
            das = dq.get(q)
            if not das:
                continue
            floor = UNIQUE_NAME_SHAPE if len(ras) == 1 and len(das) == 1 else NAME_SHAPE
            cands = sorted(((self.ratio(ra, da) or 0.0, ra, da) for ra in ras for da in das), reverse=True)
            for _, ra, da in cands:
                if self.fmap.get(ra) != da:
                    self.pair(ra, da, "name", override=True, floor=floor)

    def neighbourhood(self):
        """({function: set of callees}, {function: set of callers}) of each side, over code references."""
        if not hasattr(self, "_hood"):
            def index(funcs):
                down, up = defaultdict(set), defaultdict(set)
                for a, f in funcs.items():
                    for kind, t in f["refs"]:
                        x = int(t, 16)
                        if kind in CODE_KINDS and x in funcs and x != a:
                            down[a].add(x)
                            up[x].add(a)
                return down, up
            self._hood = index(self.r), index(self.d)
        return self._hood

    def callees_agree(self, ra, da):
        """At least two of the sibling function's references are paired, and two thirds of those land on retail's
        references: the two call mostly the same things."""
        known = Counter(t for t in (self.mapped(k, t)[1] for k, t in self.d[da]["refs"]) if not t.startswith("demo:"))
        if sum(known.values()) < 2:
            return False
        hits = known & Counter(t for _, t in self.r[ra]["refs"])
        return 3 * sum(hits.values()) >= 2 * sum(known.values())

    def neighbours(self):
        """Pair the unpaired callees (and callers) of paired functions with the sibling's unpaired callees (and
        callers) of their counterparts, wherever the call indexes don't line up: each pair the most alike of its
        candidates on both sides, at least MIN_SHAPE alike, or CALLEE_SHAPE when its paired references agree."""
        (rdown, rup), (ddown, dup) = self.neighbourhood()
        support = Counter()
        for ra, da in list(self.fmap.items()):
            if self.how[ra] == "order" and self.r[ra]["size"] < 16:
                continue
            for rs, ds in ((rdown, ddown), (rup, dup)):
                xs = [x for x in rs.get(ra, ()) if x not in self.fmap]
                ys = [y for y in ds.get(da, ()) if y not in self.finv]
                if xs and ys and len(xs) * len(ys) <= 64:
                    for x in xs:
                        for y in ys:
                            support[x, y] += 1
        scored = []
        for (x, y), n in support.items():
            r = self.ratio(x, y)
            if r is not None and r < MIN_SHAPE and (r < CALLEE_SHAPE or not self.callees_agree(x, y)):
                continue
            scored.append((r if r is not None else 0.0, n, x, y))
        best_r, best_d = defaultdict(list), defaultdict(list)
        for r, n, x, y in scored:
            best_r[x].append(((r, n), y))
            best_d[y].append(((r, n), x))

        def only(cands):   # the single best candidate, None on a tie
            cands.sort(reverse=True)
            return cands[0][1] if len(cands) == 1 or cands[0][0] > cands[1][0] else None
        best_r = {x: only(c) for x, c in best_r.items()}
        best_d = {y: only(c) for y, c in best_d.items()}
        for r, n, x, y in sorted(scored, reverse=True):
            if best_r[x] == y and best_d[y] == x:
                self.pair(x, y, "neighbour", floor=LOWEST_SHAPE)

    def prune(self):
        """Drop the pairs found only by order that a paired reference now contradicts."""
        for ra in [a for a, how in self.how.items() if how == "order"]:
            if not self.compatible(ra, self.fmap[ra]):
                self.unpair(ra)

    def propagate(self):
        todo = [(ra, da) for ra, da in self.fmap.items()
                if self.how[ra] != "order" or self.r[ra]["size"] >= 16]   # a stub's order says little
        seen = set()
        while todo:
            ra, da = todo.pop()
            if (ra, da) in seen:
                continue
            seen.add((ra, da))
            rr, dr = self.r[ra]["refs"], self.d[da]["refs"]
            same = self.r[ra]["hash"] == self.d[da]["hash"] and len(rr) == len(dr)
            if same:
                blocks = [(i, i) for i in range(len(rr))]
            else:
                a = [tuple(x) for x in rr]
                b = [self.mapped(k, t) for k, t in dr]
                blocks = []
                for op, i1, i2, j1, j2 in difflib.SequenceMatcher(None, a, b, autojunk=False).get_opcodes():
                    if op == "replace" and i2 - i1 == j2 - j1:
                        blocks += [(i1 + k, j1 + k) for k in range(i2 - i1)]
            for i, j in blocks:
                (kr, tr), (kd, td) = rr[i], dr[j]
                if kr != kd:
                    continue
                x, y = int(tr, 16), int(td, 16)
                if kr in CODE_KINDS:
                    if x in self.r and y in self.d and (same or self.similar(x, y)) and \
                            self.pair(x, y, "calls", override=same and self.how.get(ra) != "order"):
                        todo.append((x, y))
                elif same or kr == "d":
                    self.pair_data(x, y)

    def vtables(self):
        if self.rcode is None or self.dcode is None:
            return
        for ra, da in list(self.dmap.items()):
            if ra not in self.rcode.relocs or da not in self.dcode.relocs:
                continue
            k = 0
            while k < 512 and ra + 4 * k in self.rcode.relocs and da + 4 * k in self.dcode.relocs:
                x, y = self.rcode.target(ra + 4 * k), self.dcode.target(da + 4 * k)
                if not (x in self.r and y in self.d):
                    break
                if self.fmap.get(x, y) != y or self.finv.get(y, x) != x:
                    break
                self.pair(x, y, "vtable")
                k += 1

    def strings(self):
        if self.rcode is None or self.dcode is None:
            return
        def index(funcs, code):
            out = defaultdict(set)
            for a, f in funcs.items():
                for kind, t in f["refs"]:
                    if kind == "d":
                        s = code.string_at(int(t, 16))
                        if s is not None:
                            out[s].add(a)
            return out
        if not hasattr(self, "_strings"):
            self._strings = index(self.r, self.rcode), index(self.d, self.dcode)
        ri, di = self._strings
        for s, ra in ri.items():
            da = di.get(s)
            if len(ra) == 1 and da and len(da) == 1:
                self.pair(next(iter(ra)), next(iter(da)), "string")

    def anchors(self):
        """The longest run of pairs whose demo addresses rise with retail's: the shared link order."""
        pairs = sorted(self.fmap.items())
        tails, prev, idx = [], [None] * len(pairs), []
        for n, (_, d) in enumerate(pairs):
            k = bisect.bisect_left(tails, d)
            if k == len(tails):
                tails.append(d)
                idx.append(n)
            else:
                tails[k] = d
                idx[k] = n
            prev[n] = idx[k - 1] if k else None
        out, n = [], idx[-1] if idx else None
        while n is not None:
            out.append(pairs[n])
            n = prev[n]
        return out[::-1]

    def key_r(self, a):
        return (self.r[a]["hash"],) + tuple(t for _, t in self.r[a]["refs"])

    def key_d(self, a):
        return (self.d[a]["hash"],) + tuple(self.mapped(k, t)[1] for k, t in self.d[a]["refs"])

    def order(self, strict=False):
        """Between anchors, align the unpaired functions on their hash and mapped references (runs of identical
        stubs differ only in what they call); strict pairs only exact keys, else equal-length runs pair in order
        where the sizes agree (at least WINDOW_SHAPE alike when the anchors hold as many functions on each side,
        else MIN_SHAPE)."""
        rs, ds = sorted(self.r), sorted(self.d)
        chain = [(min(rs[0], ds[0]) - 1, min(rs[0], ds[0]) - 1)] if rs and ds else []
        chain += self.anchors() + [(1 << 32, 1 << 32)]
        for (r0, d0), (r1, d1) in zip(chain, chain[1:]):
            ru = [a for a in rs[bisect.bisect_right(rs, r0):bisect.bisect_left(rs, r1)] if a not in self.fmap]
            du = [a for a in ds[bisect.bisect_right(ds, d0):bisect.bisect_left(ds, d1)] if a not in self.finv]
            if not ru or not du:
                continue
            a = [self.key_r(x) for x in ru]
            b = [self.key_d(y) for y in du]
            for op, i1, i2, j1, j2 in difflib.SequenceMatcher(None, a, b, autojunk=False).get_opcodes():
                if op == "equal":
                    self.pair_run(zip(ru[i1:i2], du[j1:j2]))
                elif strict or op != "replace":
                    continue
                elif i2 - i1 == j2 - j1:
                    # the two sides hold as many functions between their anchors: in step, they are less alike
                    self.pair_run(zip(ru[i1:i2], du[j1:j2]), WINDOW_SHAPE if len(ru) == len(du) else MIN_SHAPE)
                else:   # unequal runs: align them on the bytes alone
                    rb, db = ru[i1:i2], du[j1:j2]
                    sm = difflib.SequenceMatcher(None, [self.r[x]["hash"] for x in rb],
                                                 [self.d[y]["hash"] for y in db], autojunk=False)
                    for op2, p1, p2, q1, q2 in sm.get_opcodes():
                        if op2 == "equal" or (op2 == "replace" and p2 - p1 == q2 - q1):
                            self.pair_run(zip(rb[p1:p2], db[q1:q2]))

    def by_keys(self):
        """Pair the unpaired functions whose bytes and code references, every one of them paired, are the same and
        unique on both sides (data references aside: a guard flag of a `_$E` stub is paired by nothing), wherever
        the link order puts them (a run of stubs that a template copy's placement cut off from its anchors)."""
        rk, dk = defaultdict(list), defaultdict(list)
        for a, f in self.r.items():
            if a not in self.fmap:
                k = tuple(t for kind, t in f["refs"] if kind in CODE_KINDS)
                if k:
                    rk[(f["hash"],) + k].append(a)
        for a, f in self.d.items():
            if a not in self.finv:
                k = tuple(self.mapped(kind, t)[1] for kind, t in f["refs"] if kind in CODE_KINDS)
                if k and not any(t.startswith("demo:") for t in k):
                    dk[(f["hash"],) + k].append(a)
        for k, ds in dk.items():
            rs = rk.get(k)
            if rs and len(rs) == 1 and len(ds) == 1:
                self.pair(rs[0], ds[0], "refs")

    def pair_run(self, pairs, floor=MIN_SHAPE):
        for x, y in pairs:
            if self.similar(x, y) and self.compatible(x, y) and self.agree(x, y):
                self.pair(x, y, "order", floor=floor)

    def agree(self, ra, da):
        """For two functions whose bytes differ: at least half of the demo's references already paired land on
        retail's references (a function and its revision call mostly the same things)."""
        if self.r[ra]["hash"] == self.d[da]["hash"]:
            return True
        known = [self.mapped(k, t)[1] for k, t in self.d[da]["refs"]]
        known = Counter(t for t in known if not t.startswith("demo:"))
        if not known:
            return True
        hits = known & Counter(t for _, t in self.r[ra]["refs"])
        return 2 * sum(hits.values()) >= sum(known.values())


# ---------------------------------------------------------------------------
# names both builds give: imports, symbols and vtables

def imports_by_name(img):
    return {v: k for k, v in img.imports().items()}


def coff_symbols(img):
    """{address: [decorated names]} from an image's COFF symbol table (DamageTool's linker kept one): its functions
    and its `??_7` vtables. The linker folded identical bodies, so an address can carry several names. The values
    in this table are relative to the image base."""
    p = struct.unpack_from("<I", img.data, 0x3C)[0]
    psym, nsym = struct.unpack_from("<II", img.data, p + 12)
    out = defaultdict(list)
    if not psym:
        return out
    strtab = psym + 18 * nsym
    i = 0
    while i < nsym:
        raw, val, sec, typ, _, naux = struct.unpack_from("<8sIhHBB", img.data, psym + 18 * i)
        if raw[:4] == b"\0\0\0\0":
            off = strtab + struct.unpack_from("<I", raw, 4)[0]
            name = img.data[off:img.data.index(b"\0", off)].decode("latin-1")
        else:
            name = raw.rstrip(b"\0").decode("latin-1")
        if sec > 0 and (typ == 0x20 or name.startswith("??_7")):
            out[img.base + val].append(name)
        i += 1 + naux
    return out


# DamageTool's classes that retail's code (and so our source) knows under another name: {DamageTool's qualified
# class: ours}. Each is a rename the class comments record (docs/evidence.md, "The SDK's /GR tools"); the pairing adds
# the ones its own pairs vote for (Pairing.class_votes).
CLASS_RENAMES = {
    "BinaryVoxel": "NiBinaryVoxelData",               # the voxel data an NiBinaryVoxelExtraData holds
    "BinaryVoxel::Slice": "VoxelSlice",               # ConvertToPointers/ConvertToOffsets: ToPointers/ToOffsets
    "BinaryVoxel::Dim3": "VoxelSliceRef",             # paired by bytes (ToPointer, ToOffset)
    "BinaryVoxel::Inside": "VoxelRow",                # GetVisualRep: VoxelRow::CreateLines (matched in its form)
    "VertPool": "DamageVertexBlock",                  # its constructor (bool, bool, bool), paired by calls
    "Damage": "VisibleDamage",                        # news the two polygonizers, as VisibleDamage does
    "OBBForest": "OBBModel",                          # OBBModel.h: the forest of OBB trees, one per shape
    "MetaVolumePolygonizer::IndexChunk": "DamageIndexBlock",   # map 0x88's value (DamagePolygonizer.h)
}
PLACEHOLDER_MEMBER = re.compile(r"^(?:Unknown[0-9A-F]{6}|VirtualSlot\d+)$")
SPECIAL_NAMES = {"0": "{ctor}", "1": "{dtor}", "_G": "{scalar deleting}", "_E": "{vector deleting}"}


def qualified(decorated):
    """The qualified name of a decorated function name as a tuple (scopes outermost first, then the member):
    `?Add@InsideData@BinaryVoxelizer@@...` is ("BinaryVoxelizer", "InsideData", "Add"), a constructor's member is
    `{ctor}`, an operator's `{op <code>}`. None for a name this doesn't read (a template scope, a back reference,
    a name that isn't decorated)."""
    if not decorated or not decorated.startswith("?"):
        return None
    rest = decorated[1:]
    member = None
    if rest.startswith("?"):
        m = re.match(r"\?(_[0-9A-Z]|[0-9A-Z])", rest)
        if m is None:
            return None
        code = m.group(1)
        member = SPECIAL_NAMES.get(code, "{op %s}" % code)
        rest = rest[m.end():]
    end = rest.find("@@")
    if end < 0:
        return None
    parts = rest[:end].split("@")
    if not parts or any(not re.match(r"^[A-Za-z_]\w*$", x) for x in parts):
        return None
    if member is None:
        member, parts = parts[0], parts[1:]
    if not parts and member.startswith("{"):
        return None
    return tuple(reversed(parts)) + (member,)


def slots_at(img, va, code_lo, code_hi, limit=512):
    out = []
    while len(out) < limit:
        t = img.dword(va + 4 * len(out))
        if t is None or not code_lo <= t < code_hi:
            break
        out.append(t)
    return out


TYPE_DESCRIPTOR = re.compile(rb"\.\?A[VU][\w@?$<>,]+?@@\0")


def msvc_vtables(img):
    """{class: vtable address} from a /GR build's RTTI: the complete object locator stored before a class's own
    (offset 0) vtable names its class."""
    import sdkdebug
    names, cols = {}, {}
    for name, va, vsize, _, rsize in img.sections:
        if name in (".data", ".rdata"):
            data = img.read(va, min(vsize, rsize)) or b""
            for m in TYPE_DESCRIPTOR.finditer(data):
                names[va + m.start() - 8] = m.group()[:-1].decode("latin-1")
    rdata = [s for s in img.sections if s[0] == ".rdata"]
    for _, va, vsize, _, rsize in rdata:
        data = img.read(va, min(vsize, rsize)) or b""
        for k in range(0, len(data) - 20, 4):
            sig, off, _, td, _ = struct.unpack_from("<5I", data, k)
            if sig == 0 and off == 0 and td in names:
                cols[va + k] = sdkdebug.undecorate_type(names[td])
    found = defaultdict(set)
    for _, va, vsize, _, rsize in rdata:
        data = img.read(va, min(vsize, rsize)) or b""
        for k in range(0, len(data) - 3, 4):
            col = struct.unpack_from("<I", data, k)[0]
            if col in cols:
                found[cols[col]].add(va + k + 4)
    return {cls: next(iter(v)) for cls, v in found.items() if len(v) == 1}


VTABLE_SYMBOL = re.compile(r"^\?\?_7(\w+)@@6B@$")


def named_vtables(img, symbols=None, listed=None):
    """{class: [slot targets]}: the vtables an image names. NetImmerse's NiRTTI (a class with one table), the MSVC
    RTTI of a /GR build, `??_7` symbols, and (retail) config/vtables.txt, which wins."""
    import sdkdebug
    _, lo, size, _, _ = img.text
    hi = lo + size
    out = {}
    rt = sdkdebug.Rtti(img)
    for cls, tables in rt.vtables.items():
        if len(tables) == 1:
            out[cls] = rt.slots[tables[0]]
    tables = dict(msvc_vtables(img))
    for va, names in (symbols or {}).items():
        for n in names:
            m = VTABLE_SYMBOL.match(n)
            if m:
                tables.setdefault(m.group(1), va)
    for cls, va in tables.items():
        out.setdefault(cls, slots_at(img, va, lo, hi))
    for cls, (va, n) in (listed or {}).items():
        out[cls] = [img.dword(va + 4 * k) for k in range(n)]
    return out


def unique_names(by_address):
    """{name: address} of the names one address alone carries (no `_$E` stub: its number is the unit's count)."""
    seen = defaultdict(set)
    for a, names in by_address.items():
        for n in names:
            if not n.startswith("_$E") and not n.startswith("??_7"):
                seen[n].add(a)
    return {n: next(iter(a)) for n, a in seen.items() if len(a) == 1}


# ---------------------------------------------------------------------------
# the cached result

def stamp(*paths):
    return ";".join("%d:%d" % (p.stat().st_mtime_ns, p.stat().st_size) for p in paths)


def annotated_starts():
    rep = decomp.load_report()
    out = {int(r["addr"], 16) for r in rep["functions"]}
    out |= set(decomp.extra_symbols().values())
    return out


_retail = {}


def retail_code(rimg, refresh=False):
    """Retail's functions as Code, read once for every sibling (build/sibling-retail.json)."""
    report = decomp.BUILD / "report.json"
    key = "%d;%s" % (FORMAT, stamp(report, decomp.ROOT / decomp.PROJECT["target"]))
    if _retail.get("key") == key:
        return _retail["code"], key
    code, key = read_retail_code(rimg, key, refresh)
    _retail.update(key=key, code=code)
    return code, key


def read_retail_code(rimg, key, refresh):
    """Retail's Code from build/sibling-retail.json, else read and cached."""
    if not refresh and RETAIL_CACHE.exists():
        try:
            data = json.loads(RETAIL_CACHE.read_text(encoding="utf-8"))
            if data.get("stamp") == key:
                return Code.cached(rimg, {int(a, 16): f for a, f in data["funcs"].items()}), key
        except ValueError:
            pass
    code = Code(rimg, annotated_starts())
    RETAIL_CACHE.write_text(json.dumps({"stamp": key, "funcs": {"0x%X" % a: f for a, f in code.funcs.items()}}),
                            encoding="utf-8")
    return code, key


def collect(path, rcode):
    import sdkdebug
    rimg = rcode.img
    simg = pe.Image(path.read_bytes())
    if not simg.relocs:
        simg.relocs = synthetic_relocs(simg)
    scode = Code(simg)
    p = Pairing(rcode.funcs, scode.funcs, rcode, scode)
    symbols = coff_symbols(simg)
    rtables = named_vtables(rimg, listed=sdkdebug.listed_vtables())
    stables = named_vtables(simg, symbols)
    p.seed_tables = [(rtables[c], stables[c]) for c in sorted(set(rtables) & set(stables))]
    if symbols:
        ours = unique_names({int(r["addr"], 16): [r["symbol"]] for r in decomp.load_report()["functions"]
                             if r.get("symbol") and not r.get("static")})
        theirs = unique_names(symbols)
        p.seed_pairs = [(ours[n], theirs[n], "symbol") for n in sorted(set(ours) & set(theirs))]
        p.rnames = {int(r["addr"], 16): qualified(r["symbol"]) for r in decomp.load_report()["functions"]
                    if r.get("symbol") and not r.get("static")}
        p.dnames = {a: [qualified(n) for n in names if not n.startswith("??_7")] for a, names in symbols.items()}
        p.renames = dict(CLASS_RENAMES)
    p.run()
    rnames = imports_by_name(rimg)
    imports = {"0x%X" % slot: "0x%X" % rnames[name] for slot, name in simg.imports().items() if name in rnames}
    ratios = {}
    for a, d in p.fmap.items():
        r = p.ratio(a, d)
        if r is not None and r < 1.0:
            ratios["0x%X" % a] = round(r, 3)
    return {"sibling": {"0x%X" % a: f for a, f in scode.funcs.items()},
            "pairs": {"0x%X" % a: ["0x%X" % d, p.how[a]] for a, d in p.fmap.items()},
            "data": {"0x%X" % a: "0x%X" % d for a, d in p.dmap.items()},
            "imports": imports, "ratios": ratios,
            "symbols": {"0x%X" % a: n for a, n in symbols.items()},
            "renames": {k: v for k, v in sorted(p.renames.items()) if CLASS_RENAMES.get(k) != v}}


def load(build="demo", path=None, refresh=False):
    """The pairing of retail with one sibling: {"build", "retail", "sibling", "pairs", "data", "imports", "ratios",
    "symbols"}, from the caches when neither image nor build/report.json changed."""
    report = decomp.BUILD / "report.json"
    if not report.exists():
        sys.exit("no build/report.json: run build first")
    path = path or build_path(build)
    if not path.exists():
        sys.exit("no %s executable at %s (--path PATH, or STBC_%s)" % (build, path, build.upper()))
    rimg = _retail["code"].img if "code" in _retail else pe.Image(decomp.target_image())
    rcode, rkey = retail_code(rimg, refresh)
    key = "%s;%s;%s;%s" % (rkey, path.resolve(), stamp(path), stamp(Path(__file__)))   # the pairing's own code too
    cache = cache_path(build)
    data = None
    if not refresh and cache.exists():
        try:
            data = json.loads(cache.read_text(encoding="utf-8"))
        except ValueError:
            data = None
        if data is not None and data.get("stamp") != key:
            data = None
    if data is None:
        data = collect(path, rcode)
        data["stamp"] = key
        cache.write_text(json.dumps(data), encoding="utf-8")
    data["build"] = build
    data["retail"] = {"0x%X" % a: f for a, f in rcode.funcs.items()}
    return data


class View:
    """One sibling's cached pairing with the report's rows and names."""

    def __init__(self, data, rows=None, extra=None):
        self.data = data
        self.build = data.get("build", "demo")
        self.pairs = {int(a, 16): (int(d, 16), how) for a, (d, how) in data["pairs"].items()}
        self.inv = {d: a for a, (d, _) in self.pairs.items()}
        self.dmap = {int(d, 16): int(a, 16) for a, d in data["data"].items()}   # sibling -> retail data
        self.imports = {int(d, 16): int(a, 16) for d, a in data["imports"].items()}
        self.ratios = {int(a, 16): r for a, r in data.get("ratios", {}).items()}
        self.symbols = {int(a, 16): n for a, n in data.get("symbols", {}).items()}
        if rows is None:
            rep = decomp.load_report()
            rows = rep["functions"]
            self.pending = {int(a, 16) for a in rep["summary"].get("match_pending", [])}
        else:
            self.pending = set()
        self.rows = {int(r["addr"], 16): r for r in rows}
        self.names = {a: r["name"] for a, r in self.rows.items()}
        for decorated, a in (decomp.extra_symbols() if extra is None else extra).items():
            if a not in self.names:
                q = callset.pretty(decorated)
                if decorated.startswith("__imp_"):
                    q = "[import] " + (decomp.import_name(decorated) or decorated)
                self.names[a] = q or decorated

    def status(self, a):
        r = self.rows.get(a)
        if r is None:
            return None
        if a in self.pending:
            return "MATCH-PENDING"
        return r["status"]

    def is_open(self, a):
        return self.status(a) in ("DIFF", "MATCH-PENDING")

    def name(self, a):
        return self.names.get(a)

    def own(self, t):
        """True for a call target that is the sibling's own (no retail counterpart)."""
        return t.startswith(self.build + ":")

    def label(self, t):
        """A retail-side call target as printed: `0x... Name`, or `0x... (no identity)`, or `<build>:0x...` (with
        the sibling's symbol when it has one)."""
        if self.own(t):
            names = self.symbols.get(int(t.split(":", 1)[1], 16))
            return "%s %s" % (t, callset.pretty(names[0]) or names[0]) if names else t
        if t.startswith("?"):
            return callset.pretty(t[1:]) or t[1:]
        n = self.name(int(t, 16))
        return "%s %s" % (t, n) if n else "%s (no identity)" % t

    def map_sibling(self, t):
        """A sibling target (int) as the retail key it stands for."""
        if t in self.inv:
            return "0x%X" % self.inv[t]
        if t in self.imports:
            return "0x%X" % self.imports[t]
        if t in self.dmap:
            return "0x%X" % self.dmap[t]
        return "%s:0x%X" % (self.build, t)

    map_demo = map_sibling

    def local_map(self, a):
        """{sibling callee: retail callee} read from the pair's own direct calls: when the masked bytes are
        identical, every call at the same index (the same code calls the same functions, whatever the pairing of a
        folded or look-alike callee says); else, for the callees the pairing left unpaired, where difflib aligns
        runs of equal length."""
        d, _ = self.pairs[a]
        rc = [t for k, t in self.data["retail"]["0x%X" % a]["refs"] if k == "c"]
        dc = [t for k, t in self.data["sibling"]["0x%X" % d]["refs"] if k == "c"]
        if self.identical(a) and len(rc) == len(dc):
            blocks = list(zip(rc, dc))
        else:
            mapped = [self.map_sibling(int(t, 16)) for t in dc]
            blocks = []
            for op, i1, i2, j1, j2 in difflib.SequenceMatcher(None, rc, mapped, autojunk=False).get_opcodes():
                if op == "replace" and i2 - i1 == j2 - j1:
                    blocks += list(zip(rc[i1:i2], dc[j1:j2]))
        same = self.identical(a) and len(rc) == len(dc)
        out = {}
        for x, y in blocks:
            yi = int(y, 16)
            if same or (self.own(self.map_sibling(yi)) and int(x, 16) not in self.pairs):
                out[yi] = x
        return out

    def sibling_calls(self, a):
        """The paired sibling function's calls, mapped to retail (through the pairing, else through the pair's own
        call order), as a Counter."""
        d, _ = self.pairs[a]
        local = self.local_map(a)
        out = Counter()
        for t, n in self.data["sibling"]["0x%X" % d]["calls"].items():
            ti = int(t, 16)
            out[local[ti] if ti in local else self.map_sibling(ti)] += n
        return out

    def calls(self, a):
        """(retail calls, sibling calls mapped to retail) as Counters, for a paired retail function."""
        return Counter(self.data["retail"]["0x%X" % a]["calls"]), self.sibling_calls(a)

    def identical(self, a):
        d, _ = self.pairs[a]
        return self.data["retail"]["0x%X" % a]["hash"] == self.data["sibling"]["0x%X" % d]["hash"]

    def ratio(self, a):
        """How alike the pair's instructions are: 1.0 for the same masked bytes."""
        return 1.0 if self.identical(a) else self.ratios.get(a, 0.0)


# ---------------------------------------------------------------------------
# reports

def coverage(v, out=sys.stdout):
    nr, nd = len(v.data["retail"]), len(v.data["sibling"])
    hows = Counter(how for _, how in v.pairs.values())
    same = sum(1 for a in v.pairs if v.identical(a))
    print("retail: %d functions read, %s: %d; %d paired (%s), %d identical when masked" % (
        nr, v.build, nd, len(v.pairs), ", ".join("%s %d" % kv for kv in hows.most_common()), same), file=out)
    by = defaultdict(Counter)
    for a, r in v.rows.items():
        if "0x%X" % a not in v.data["retail"]:
            continue
        st = v.status(a)
        if r.get("static"):
            st = "static initialiser"
        c = by[st]
        c["annotated"] += 1
        if a in v.pairs:
            c["paired"] += 1
            c["identical" if v.identical(a) else "differ"] += 1
            rc, dc = v.calls(a)
            if rc != dc:
                c["call set differs"] += 1
    total = Counter()
    for c in by.values():
        total.update(c)
    print("%-20s %9s %9s %9s %9s %9s" % ("annotated", "count", "paired", "identical", "differ", "callset"), file=out)
    for st, c in sorted(by.items(), key=lambda kv: -kv[1]["annotated"]) + [("all", total)]:
        print("%-20s %9d %9d %9d %9d %9d" % (st, c["annotated"], c["paired"], c["identical"], c["differ"],
                                             c["call set differs"]), file=out)


def norm(m, o, label):
    def sub(mt):
        return label(int(mt.group(0), 16)) or mt.group(0)
    return "%s %s" % (m, re.sub(r"0x[0-9a-f]{5,8}", sub, o))


def operand_addresses(img, x, size, m, o):
    """The values in an instruction's operands that are addresses: its relocated dwords and a branch target.
    Any other number (an event type such as 0x8000B7) is a constant, however much it looks like an address."""
    out = {img.dword(s) for s in range(x, x + size) if s in img.relocs}
    if BRANCH.match(m) and ADDRESS.match(o):
        out.add(int(o, 16))
    return out


def labelled(img, start, size, label):
    """[(address, normalised text)] of the function's instructions, addresses named by label(target)."""
    out = []
    for x, n, m, o in disasm_lite(img.read(start, size), start):
        addrs = operand_addresses(img, x, n, m, o)
        out.append((x, norm(m, o, lambda t: label(t) if t in addrs else None)))
    return out


def sym_note(v, d):
    """`, ?Name@...` for a sibling address its symbol table names (the first name: folded bodies carry several)."""
    names = v.symbols.get(d)
    return ", %s%s" % (names[0], " +%d" % (len(names) - 1) if len(names) > 1 else "") if names else ""


def report_fn(v, query, out=sys.stdout, demo_img=None, retail_img=None):
    """Retail and the sibling side by side, both sides' addresses named through the pairing."""
    a = int(query, 16)
    if a not in v.pairs:
        side = "retail" if "0x%X" % a in v.data["retail"] else None
        if a in v.inv:
            a = v.inv[a]
        else:
            print("0x%X has no %s counterpart%s" % (a, v.build, "" if side else " (and is no function start retail's "
                                                       "scan found)"), file=out)
            return 1
    d, how = v.pairs[a]
    fr, fd = v.data["retail"]["0x%X" % a], v.data["sibling"]["0x%X" % d]
    print("retail 0x%X %s (%d bytes, %s)  <->  %s 0x%X (%d bytes%s), paired by %s, %s" % (
        a, v.name(a) or "(no identity)", fr["size"], v.status(a) or "not annotated", v.build, d, fd["size"],
        sym_note(v, d), how, "identical when masked" if fr["hash"] == fd["hash"] else
        "differs, %.0f%% alike" % (100 * v.ratio(a))), file=out)
    rc, dc = v.calls(a)
    for t in sorted(set(rc) | set(dc)):
        if rc[t] != dc[t]:
            print("  calls differ: retail %d, %s %d  %s" % (rc[t], v.build, dc[t], v.label(t)), file=out)
    retail_img = retail_img or pe.Image(decomp.target_image())
    demo_img = demo_img or pe.Image(build_path(v.build).read_bytes())

    def rlabel(t, own=True):
        if own and a <= t < a + fr["size"]:
            return "$+0x%X" % (t - a)   # a label of the function's own
        n = v.name(t)
        return "0x%X<%s>" % (t, n) if n else "0x%X" % t

    def dlabel(t):
        if d <= t < d + fd["size"]:
            return "$+0x%X" % (t - d)
        k = v.map_demo(t)
        return v.label(k) if v.own(k) else rlabel(int(k, 16), own=False)

    ri = labelled(retail_img, a, fr["size"], rlabel)
    di = labelled(demo_img, d, fd["size"], dlabel)
    sm = difflib.SequenceMatcher(None, [t for _, t in ri], [t for _, t in di], autojunk=False)
    w = 58
    for op, i1, i2, j1, j2 in sm.get_opcodes():
        for k in range(max(i2 - i1, j2 - j1)):
            left = ri[i1 + k] if i1 + k < i2 else None
            right = di[j1 + k] if j1 + k < j2 else None
            mark = " " if op == "equal" else "|"
            ls = "%06X %s" % left if left else ""
            rs = "%06X %s" % right if right else ""
            print("%-*s %s %s" % (w, ls[:w], mark, rs), file=out)
    return 0


def differences(v, only_open=False, min_score=0.0):
    """{retail address: (only retail, only the sibling)} for paired annotated functions whose call sets differ."""
    out = {}
    for a in v.pairs:
        if a not in v.rows:
            continue
        if only_open and not v.is_open(a):
            continue
        if v.rows[a].get("score", 0) < min_score:
            continue
        rc, dc = v.calls(a)
        if rc != dc:
            out[a] = (rc - dc, dc - rc)
    return out


def some(counter, v, limit=3):
    items = ["%s%s" % (v.label(t), " x%d" % n if n > 1 else "") for t, n in sorted(counter.items())]
    return "; ".join(items[:limit]) + ("; ..." if len(items) > limit else "")


def report_callsets(v, only_open=False, min_score=0.0, top=25, out=sys.stdout):
    diff = differences(v, only_open, min_score)
    sel = [a for a in v.pairs if a in v.rows and (not only_open or v.is_open(a))
           and v.rows[a].get("score", 0) >= min_score]
    print("%d paired %sfunctions%s, %d whose call sets differ from the %s's" % (
        len(sel), "open " if only_open else "annotated ", " scoring >= %g" % min_score if min_score else "",
        len(diff), v.build), file=out)
    print("\nby function (score, status):", file=out)
    order = sorted(diff, key=lambda a: (-v.rows[a].get("score", 0), a))
    for a in order[:top]:
        only_r, only_d = diff[a]
        print("%6.1f %-13s 0x%X %s (%s 0x%X, paired by %s)" % (
            v.rows[a].get("score", 0), v.status(a), a, v.name(a), v.build, v.pairs[a][0], v.pairs[a][1]), file=out)
        if only_r:
            print("         retail only: %s" % some(only_r, v), file=out)
        if only_d:
            print("         %-12s %s" % (v.build + " only:", some(only_d, v)), file=out)
    if len(order) > top:
        print("  ... %d more (--top N)" % (len(order) - top), file=out)
    for side, title in ((1, "%s calls X out of line where retail doesn't (retail expands X, or lacks the call)"
                         % v.build),
                        (0, "retail calls X out of line where the %s doesn't (it expands X, or lacks it)" % v.build)):
        table = defaultdict(list)
        for a, pair in diff.items():
            for t, n in pair[side].items():
                table[t].append((a, n))
        ranked = sorted(table, key=lambda t: (-len(table[t]), t))
        print("\n%s: %d callees" % (title, len(ranked)), file=out)
        print("%6s %6s %5s  %s" % ("funcs", "sites", "open", "callee"), file=out)
        for t in ranked[:top]:
            nopen = sum(1 for a, _ in table[t] if v.is_open(a))
            print("%6d %6d %5d  %s" % (len(table[t]), sum(n for _, n in table[t]), nopen, v.label(t)), file=out)
        if len(ranked) > top:
            print("  ... %d more (--top N)" % (len(ranked) - top), file=out)
    return diff


def report_new(v, top=40, out=sys.stdout):
    """Sibling functions without a retail counterpart that paired sibling functions call."""
    callers = defaultdict(list)
    for a, (d, _) in v.pairs.items():
        for t, n in v.data["sibling"]["0x%X" % d]["calls"].items():
            ti = int(t, 16)
            if "0x%X" % ti in v.data["sibling"] and ti not in v.inv:
                callers[ti].append(a)
    rhash = defaultdict(list)
    for k, f in v.data["retail"].items():
        rhash[f["hash"]].append(int(k, 16))
    print("%d %s functions without a retail counterpart are called by paired functions" % (len(callers), v.build),
          file=out)
    print("%8s %5s %6s  %s" % (v.build[:8], "size", "calls", "callers (retail name) / same bytes as retail"), file=out)
    ranked = sorted(callers, key=lambda t: (-len(callers[t]), t))
    for t in ranked[:top]:
        f = v.data["sibling"]["0x%X" % t]
        same = [x for x in rhash.get(f["hash"], [])]
        who = sorted(set(callers[t]))
        names = ", ".join("0x%X %s" % (a, v.name(a) or "") for a in who[:3]) + (" ..." if len(who) > 3 else "")
        print("%08X %5d %6d  %s" % (t, f["size"], len(callers[t]), names), file=out)
        if t in v.symbols:
            print("%22s %s's name: %s" % ("", v.build, ", ".join(v.symbols[t][:2])), file=out)
        if same:
            print("%22s same bytes as retail %s" % ("", ", ".join(v.label("0x%X" % x) for x in same[:3])), file=out)
    if len(ranked) > top:
        print("  ... %d more (--top N)" % (len(ranked) - top), file=out)


DISPLACEMENT = re.compile(r" [+-] (?:0x[0-9a-f]+|\d+)\]")


def flip_class(retail_ins, demo_ins):
    """How two builds' copies of a function differ, from their [(mnemonic, operands)] with addresses already
    normalised: "frame/register" (only stack slots and register picks), "schedule" (the same such instructions in
    another order), "layout" (also other displacements: member offsets, vtable slots), else "code"."""
    import walls

    def frame(ins):
        bases = decomp.frame_bases(ins)   # decomp.flip_kind's frame slots: esp, and ebp only as a frame pointer
        return ["%s %s" % (m, walls.REGISTERS.sub("R", decomp.frame_slot_free(o, bases))) for m, o in ins]

    a, b = frame(retail_ins), frame(demo_ins)
    if a == b:
        return "frame/register"
    if Counter(a) == Counter(b):
        return "schedule"
    if [DISPLACEMENT.sub("]", x) for x in a] == [DISPLACEMENT.sub("]", x) for x in b]:
        return "layout"
    return "code"


def report_flips(v, demo_img, only_open=True, out=sys.stdout):
    """Annotated functions whose bytes differ from the sibling's, classed by flip_class: a frame/register or schedule
    flip between two builds of one source is a wall that moves with the declarations a unit sees."""
    retail_img = pe.Image(decomp.target_image())
    rows = []
    for a, (d, _) in sorted(v.pairs.items()):
        if a not in v.rows or v.identical(a) or (only_open and not v.is_open(a)) or v.rows[a].get("static"):
            continue
        fr, fd = v.data["retail"]["0x%X" % a], v.data["sibling"]["0x%X" % d]
        ri = [tuple(t.split(" ", 1) + [""])[:2] for _, t in labelled(retail_img, a, fr["size"],
                                                                     lambda t: "0x%X" % t)]
        di = [tuple(t.split(" ", 1) + [""])[:2] for _, t in labelled(
            demo_img, d, fd["size"], lambda t: v.map_demo(t) if not d <= t < d + fd["size"]
            else "0x%X" % (a + t - d))]
        rows.append((flip_class(ri, di), a))
    kinds = Counter(k for k, _ in rows)
    print("%d %sannotated functions differ from the %s's copy: %s" % (
        len(rows), "open " if only_open else "", v.build, ", ".join("%s %d" % kv for kv in kinds.most_common())), file=out)
    for k, a in sorted(rows, key=lambda r: (r[0], -v.rows[r[1]].get("score", 0))):
        print("  %-15s %6.1f %-13s 0x%X %s" % (k, v.rows[a].get("score", 0), v.status(a), a, v.name(a)), file=out)


# ---------------------------------------------------------------------------
# every sibling against the open functions

def load_views(builds, refresh=False, out=sys.stderr):
    """{build: View} for the builds whose executable exists, the report read once."""
    rep = decomp.load_report()
    rows = rep["functions"]
    pending = {int(a, 16) for a in rep["summary"].get("match_pending", [])}
    extra = decomp.extra_symbols()
    views = {}
    for b in builds:
        if not build_path(b).exists():
            print("%s: no executable at %s, left out" % (b, build_path(b)), file=out)
            continue
        v = View(load(b, refresh=refresh), rows, extra)
        v.pending = pending
        views[b] = v
    return views


def sides(retail, ours, sibling, keys, unexplained=0):
    """For each callee in keys (where retail's and ours' counts differ): "retail" when the sibling calls it as
    often as retail, "ours" when as often as ours, else "neither". A side that rests on the sibling calling it less
    often is "unknown" while the sibling makes calls nothing maps (`unexplained`): one of them may be it."""
    out = {}
    for t in keys:
        for side, count in (("retail", retail[t]), ("ours", ours[t])):
            if sibling[t] == count:
                other = ours[t] if side == "retail" else retail[t]
                out[t] = side if count > other or not unexplained else "unknown"
                break
        else:
            out[t] = "neither"
    return out


def verdict(side):
    """One build's verdict over a function's disagreeing callees."""
    kinds = set(side.values())
    if not kinds:
        return "-"
    if kinds == {"retail"}:
        return "retail"
    if kinds == {"ours"}:
        return "ours"
    if kinds == {"unknown"}:
        return "unknown"
    return "mixed"


def open_functions(views, cs):
    """[{addr, row, retail, ours, differ, builds: {build: {pair, how, ratio, side, verdict, extra}}}] for every open
    (DIFF) function of tools/callset.py's cache."""
    out = []
    for k, f in cs["functions"].items():
        a = int(k, 16)
        retail, ours = Counter(f["retail"]), Counter(f["ours"])
        differ = sorted(set(f["only_retail"]) | set(f["only_ours"]))
        entry = {"addr": a, "f": f, "retail": retail, "ours": ours, "differ": differ, "builds": {}}
        for b, v in views.items():
            if a not in v.pairs:
                continue
            sib = v.sibling_calls(a)
            unexplained = sum(n for t, n in sib.items() if v.own(t))
            # retail's calls as this tool's cut reads them, which can end before callset.py's reading (a tail the
            # scan took for another function): the sibling, cut the same way, is compared on the same span
            cut = Counter(v.data["retail"]["0x%X" % a]["calls"])
            for t in set(retail) | set(cut):
                sib[t] = sib[t] + retail[t] - cut[t]
            side = sides(retail, ours, sib, differ, unexplained)
            entry["builds"][b] = {
                "pair": v.pairs[a][0], "how": v.pairs[a][1], "ratio": v.ratio(a), "side": side,
                "verdict": verdict(side),
                "extra": sorted(t for t in sib if v.own(t) and retail[t] == 0 and ours[t] == 0)}
        entry["for_retail"] = sorted(b for b, e in entry["builds"].items() if e["verdict"] == "retail")
        entry["for_ours"] = sorted(b for b, e in entry["builds"].items() if e["verdict"] == "ours")
        out.append(entry)
    return out


def cs_label(t, cs, views):
    if t.startswith("?"):
        return callset.pretty(t[1:]) or t[1:]
    for v in views.values():
        if v.own(t):
            return v.label(t)
    return callset.label(t, cs["names"])


def summary(entries, views, out=sys.stdout):
    total = len(entries)
    differ = [e for e in entries if e["differ"]]
    print("%d open functions; %d whose call sets differ from retail's (tools/callset.py)" % (total, len(differ)),
          file=out)
    print("%-11s %7s %9s %9s %10s %9s %9s %7s" % ("sibling", "paired", "identical", "differing", "callset-", "retail",
                                                    "ours", "mixed"), file=out)
    print("%-11s %7s %9s %9s %10s %9s %9s %7s" % ("", "", "", "", "differs", "side", "side", ""), file=out)
    for b, v in views.items():
        mine = [e for e in entries if b in e["builds"]]
        same = sum(1 for e in mine if e["builds"][b]["ratio"] == 1.0)
        dmine = [e for e in mine if e["differ"]]
        c = Counter(e["builds"][b]["verdict"] for e in dmine)
        print("%-11s %7d %9d %9d %10d %9d %9d %7d" % (b, len(mine), same, len(mine) - same, len(dmine), c["retail"],
                                                    c["ours"], c["mixed"]), file=out)
    covered = Counter(len(e["builds"]) for e in entries)
    print("covered by at least one sibling: %d; by two or more: %d" % (
        sum(n for k, n in covered.items() if k >= 1), sum(n for k, n in covered.items() if k >= 2)), file=out)
    print("call set differs and siblings side with retail: %d by one or more, %d by two or more (strong); "
          "a sibling sides with ours: %d" % (
              sum(1 for e in differ if e["for_retail"]), sum(1 for e in differ if len(e["for_retail"]) >= 2),
              sum(1 for e in differ if e["for_ours"])), file=out)
    sdk = [e for e in differ if any(b != "demo" for b in e["for_retail"])]
    print("  of them sided with retail by an SDK build (not the demo): %d; by one whose copy differs from retail's: "
          "%d" % (len(sdk), sum(1 for e in differ if any(e["builds"][b]["ratio"] < 1.0 for b in e["for_retail"]))),
          file=out)


def report_open(views, cs, min_score=0.0, top=30, unit=None, strong=False, show_all=False, out=sys.stdout):
    """Every open function against every sibling, the ones the siblings point at first."""
    entries = open_functions(views, cs)
    summary(entries, views, out)
    sel = []
    for e in entries:
        f = e["f"]
        if f["score"] < min_score or (unit and not re.search(unit, f["unit"] or "")):
            continue
        if not show_all and not (e["differ"] and (e["for_retail"] or e["for_ours"])):
            continue
        if strong and len(e["for_retail"]) < 2:
            continue
        sel.append(e)
    sel.sort(key=lambda e: (-len(e["for_retail"]), -e["f"]["score"], e["addr"]))
    print("\n%d open functions %s, by siblings siding with retail, then score:" % (
        len(sel), "listed" if show_all else "where a sibling takes a side on the callees retail and ours disagree on"),
        file=out)
    for e in sel[:top]:
        f = e["f"]
        tag = ("retail side: %s" % ", ".join(e["for_retail"])) if e["for_retail"] else ""
        if e["for_ours"]:
            tag += ("; " if tag else "") + "ours side: %s" % ", ".join(e["for_ours"])
        print("%5.1f 0x%X %s (%s)  %s" % (f["score"], e["addr"], f["name"], f["unit"], tag), file=out)
        only_r = ["%s x%d" % (cs_label(t, cs, views), n) if n > 1 else cs_label(t, cs, views)
                  for t, n in sorted(f["only_retail"].items())]
        only_o = ["%s x%d" % (cs_label(t, cs, views), n) if n > 1 else cs_label(t, cs, views)
                  for t, n in sorted(f["only_ours"].items())]
        if only_r:
            print("      retail only: %s" % "; ".join(only_r[:4]) + (" ..." if len(only_r) > 4 else ""), file=out)
        if only_o:
            print("      ours only:   %s" % "; ".join(only_o[:4]) + (" ..." if len(only_o) > 4 else ""), file=out)
        for b in views:
            x = e["builds"].get(b)
            if x is None:
                print("      %-11s -" % b, file=out)
                continue
            line = "      %-11s 0x%X %-6s %s  %s" % (b, x["pair"], x["how"], "same bytes" if x["ratio"] == 1.0 else
                                                     "%3.0f%% alike" % (100 * x["ratio"]), x["verdict"])
            if x["verdict"] == "mixed":
                line += " (%s)" % ", ".join("%s: %s" % (cs_label(t, cs, views), sd) for t, sd in x["side"].items())
            if x["extra"]:
                line += "; also calls %s" % ", ".join(cs_label(t, cs, views) for t in x["extra"][:3])
            print(line, file=out)
    if len(sel) > top:
        print("  ... %d more (--top N)" % (len(sel) - top), file=out)
    return entries


def report_fn_all(views, query, out=sys.stdout):
    """One retail function against every sibling: the pair, its similarity and the call-set differences."""
    a = int(query, 16)
    for b, v in views.items():
        if a not in v.pairs:
            print("%-11s no counterpart" % b, file=out)
            continue
        d, how = v.pairs[a]
        rc, dc = v.calls(a)
        print("%-11s 0x%X (%d bytes%s), paired by %s, %.0f%% alike" % (
            b, d, v.data["sibling"]["0x%X" % d]["size"], sym_note(v, d), how, 100 * v.ratio(a)), file=out)
        for t in sorted(set(rc) | set(dc)):
            if rc[t] != dc[t]:
                print("            calls differ: retail %d, %s %d  %s" % (rc[t], b, dc[t], v.label(t)), file=out)
    return 0


# ---------------------------------------------------------------------------
# DamageTool's names against ours

_undname = None


def undecorate(name):
    """The full undecorated text of a decorated name (dbghelp's UnDecorateSymbolName), or None."""
    global _undname
    if _undname is None:
        try:
            import ctypes
            dll = ctypes.windll.dbghelp
            buf = ctypes.create_string_buffer(4096)

            def und(n):
                k = dll.UnDecorateSymbolName(n.encode("latin-1"), buf, len(buf), 0)
                return buf.value.decode("latin-1") if k else None
            _undname = und
        except (ImportError, AttributeError, OSError):
            _undname = lambda n: None   # noqa: E731
    return _undname(name) if name.startswith("?") else None


SIGNATURE = re.compile(r"^(?:(?P<access>public|protected|private): )?(?P<kind>(?:virtual|static) )?(?P<ret>.*?) ?"
                       r"(?P<cc>__thiscall|__cdecl|__stdcall|__fastcall) (?P<name>[^(]+)\((?P<params>.*)\)"
                       r"(?P<const> ?const)? *$")


def split_params(text):
    out, depth, cur = [], 0, ""
    for ch in text:
        if ch in "<(":
            depth += 1
        elif ch in ">)":
            depth -= 1
        if ch == "," and depth == 0:
            out.append(cur.strip())
            cur = ""
        else:
            cur += ch
    if cur.strip():
        out.append(cur.strip())
    return [] if out == ["void"] else out


def signature(decorated):
    """{access, kind, ret, cc, name, params, const} of a decorated function name, or None."""
    text = undecorate(decorated)
    m = SIGNATURE.match(text) if text else None
    if m is None:
        return None
    g = m.groupdict()
    g["kind"] = (g["kind"] or "").strip()
    g["const"] = bool(g["const"])
    g["params"] = split_params(g["params"])
    return g


def compare_signatures(ours, theirs):
    """[what differs] between our decorated name and the sibling's: name, return, parameter N, const, virtual or
    static, access, calling convention."""
    if ours == theirs:
        return []
    a, b = signature(ours), signature(theirs)
    if a is None or b is None:
        return ["name"]
    out = []
    if a["name"] != b["name"]:
        out.append("name")
    if a["ret"] != b["ret"]:
        out.append("return %s / %s" % (a["ret"] or "-", b["ret"] or "-"))
    if len(a["params"]) != len(b["params"]):
        out.append("%d / %d parameters" % (len(a["params"]), len(b["params"])))
    else:
        for i, (x, y) in enumerate(zip(a["params"], b["params"])):
            if x != y:
                out.append("parameter %d %s / %s" % (i + 1, x, y))
    if a["const"] != b["const"]:
        out.append("const %s / %s" % ("yes" if a["const"] else "no", "yes" if b["const"] else "no"))
    if a["kind"] != b["kind"]:
        out.append("%s / %s" % (a["kind"] or "member", b["kind"] or "member"))
    if a["access"] != b["access"]:
        out.append("%s / %s" % (a["access"] or "free", b["access"] or "free"))
    if a["cc"] != b["cc"]:
        out.append("%s / %s" % (a["cc"], b["cc"]))
    return out


def best_name(ours, names):
    """Of the names the sibling's linker gave one address (folded bodies), the one closest to ours."""
    if ours in names:
        return ours
    so = signature(ours) if ours else None
    if so:
        for n in names:
            sn = signature(n)
            if sn and sn["name"] == so["name"]:
                return n
        method = so["name"].split("::")[-1]
        for n in names:
            sn = signature(n)
            if sn and sn["name"].split("::")[-1] == method:
                return n
    return names[0]


PLACEHOLDER = re.compile(r"::(?:Unknown[0-9A-F]{6}|VirtualSlot\d+)(?!\w)|^Unknown[0-9A-F]{6}$")


def syms_rows(v, sel, explicit):
    """[(retail address, row, ours, the sibling's name, [what differs])] over the selected functions."""
    by_name = defaultdict(list)
    for d, names in v.symbols.items():
        for n in names:
            by_name[n].append(d)
    rows = []
    for a in sel:
        r = v.rows.get(a, {})
        ours = r.get("symbol")
        pair = v.pairs.get(a)
        names = v.symbols.get(pair[0]) if pair else None
        if not names:
            if ours and ours in by_name:
                rows.append((a, r, ours, None, ["named only: %s defines it at 0x%X, unpaired" % (
                    v.build, by_name[ours][0])]))
            elif explicit:
                rows.append((a, r, ours, None, ["no named %s counterpart" % v.build]))
            continue
        theirs = best_name(ours, names)
        what = compare_signatures(ours, theirs) if ours else ["no symbol of ours"]
        if what and what[0] == "name" and PLACEHOLDER.search(r.get("name", "")):
            what[0] = "placeholder name"
        rows.append((a, r, ours, theirs, what))
    return rows


def difference_kind(what):
    """The class of one difference compare_signatures or syms_rows printed, for the summary."""
    first = what.split(" ")[0]
    if first in ("public", "protected", "private", "free"):
        return "access"
    if first in ("member", "static", "virtual"):
        return "virtual/static"
    if first[0].isdigit():
        return "parameter count"
    if what.startswith("placeholder name"):
        return "placeholder name"
    if what.startswith("named only"):
        return "named but unpaired"
    if what.startswith("no "):
        return "no name"
    return first


def report_syms(v, addrs=None, show_all=False, top=200, out=sys.stdout):
    """Retail functions paired with a named function of the sibling (DamageTool): its decorated name against ours,
    and what differs (ours / the sibling's). Default: the open ones; --all: every annotated one; the ones that
    differ are printed, all of them for the addresses given."""
    if not v.symbols:
        print("%s has no symbol table" % v.build, file=out)
        return 1
    if addrs:
        sel = [int(x, 16) for x in addrs]
    else:
        sel = sorted(a for a in v.rows if not v.rows[a].get("static") and (show_all or v.is_open(a)))
    rows = syms_rows(v, sel, bool(addrs))
    kinds = Counter(difference_kind(w) for *_, what in rows for w in what)
    shown = [x for x in rows if x[4] or addrs]
    for a, r, ours, theirs, what in shown[:top]:
        print("0x%X %s  %s %.1f (%s)" % (a, r.get("name"), v.status(a) or "", r.get("score", 0), r.get("unit")),
              file=out)
        if ours:
            print("    ours:       %s" % (undecorate(ours) or ours), file=out)
        if theirs:
            print("    %-11s %s%s" % (v.build + ":", undecorate(theirs) or theirs,
                                      "" if theirs == ours else "  [%s]" % theirs), file=out)
        print("    %s" % ("; ".join(what) if what else "same name"), file=out)
    if len(shown) > top:
        print("  ... %d more (--top N)" % (len(shown) - top), file=out)
    print("%d %s functions with a named %s counterpart, %d the same, %d differ: %s" % (
        len(rows), "selected" if addrs else "annotated" if show_all else "open", v.build,
        sum(1 for x in rows if not x[4]), sum(1 for x in rows if x[4]),
        ", ".join("%s %d" % kv for kv in kinds.most_common()) or "none"), file=out)
    return 0


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("cmd", nargs="*", help="nothing (coverage), `fn <addr>`, `callsets`, `new`, `flips`, `open`, "
                                           "`syms [ADDR ...]`")
    ap.add_argument("--build", default=None, help="the sibling: %s, or all (default: demo; open: all; syms: "
                                                  "damagetool)" % ", ".join(BUILDS))
    ap.add_argument("--path", help="the sibling's executable (one build; default: STBC_<BUILD>, project.json "
                                   "\"siblings\", the default under C:/Recomp)")
    ap.add_argument("--demo", help="the demo executable (same as --build demo --path PATH)")
    ap.add_argument("--open", action="store_true", help="callsets: only open (DIFF and MATCH-PENDING) functions")
    ap.add_argument("--min-score", type=float, default=0.0, help="callsets, open: only functions scoring at least this")
    ap.add_argument("--top", type=int, default=None, help="rows per table (default 25; open: 30; syms: 200)")
    ap.add_argument("--unit", help="open: only units matching this regular expression")
    ap.add_argument("--strong", action="store_true", help="open: only functions two or more siblings side with "
                                                          "retail on")
    ap.add_argument("--all", action="store_true", help="flips: every annotated function; open: every open "
                                                       "function; syms: every annotated function (differences)")
    ap.add_argument("--refresh", action="store_true", help="ignore the caches")
    args = ap.parse_args(argv)
    cmd = args.cmd[0] if args.cmd else None
    build = args.build or ("all" if cmd == "open" else "damagetool" if cmd == "syms" else "demo")
    path = args.path
    if args.demo:
        build, path = "demo", args.demo
    if build != "all" and build not in BUILDS:
        ap.error("--build: one of %s, or all" % ", ".join(BUILDS))
    if path and build == "all":
        ap.error("--path names one build's executable")
    builds = list(BUILDS) if build == "all" else [build]
    if cmd == "open" and len(args.cmd) == 1:
        views = load_views(builds, args.refresh)
        report_open(views, callset.load(), args.min_score, 30 if args.top is None else args.top, args.unit, args.strong, args.all)
        return 0
    if build == "all":
        views = load_views(builds, args.refresh)
        if cmd == "fn" and len(args.cmd) == 2:
            return report_fn_all(views, args.cmd[1])
        if cmd is None:
            for b, v in views.items():
                print("== %s (%s)" % (b, BUILDS[b][1]))
                coverage(v)
            return 0
        ap.error("--build all: coverage, fn <addr> or open")
    path = build_path(build, path)
    v = View(load(build, path, args.refresh))
    top = 25 if args.top is None else args.top
    if cmd is None:
        coverage(v)
    elif cmd == "fn" and len(args.cmd) == 2:
        return report_fn(v, args.cmd[1], demo_img=pe.Image(path.read_bytes()))
    elif cmd == "callsets" and len(args.cmd) == 1:
        report_callsets(v, args.open, args.min_score, top)
    elif cmd == "new" and len(args.cmd) == 1:
        report_new(v, top)
    elif cmd == "flips" and len(args.cmd) == 1:
        report_flips(v, pe.Image(path.read_bytes()), only_open=not args.all)
    elif cmd == "syms":
        return report_syms(v, args.cmd[1:], args.all, 200 if args.top is None else args.top)
    else:
        ap.error("subcommands: fn <addr>, callsets, new, flips, open, syms [ADDR ...]")
    return 0


if __name__ == "__main__":
    sys.exit(main())
