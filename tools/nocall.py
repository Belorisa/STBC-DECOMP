"""Retail out-of-line copies of a unit's own helpers that nothing calls: the callers that should call them.

    python tools/nocall.py                     # every candidate with open callers, strongest first
    python tools/nocall.py --unit netimmerse/renderer/NiD3DFrameBuffer
    python tools/nocall.py --all               # also helpers without open callers, and external functions
    python tools/nocall.py --gaps              # also unwritten gap functions nothing calls (unidentified helpers)
    python tools/nocall.py --json build/nocall.json

A retail function is *uncalled* when no `call`, `jmp` or `jcc` rel32 in .text lands on its first byte and no
base-relocated dword in the image holds its address (a vtable, a function-pointer table, a `push offset`). The image's
own relocations are exact; the rel32 scan reads every E8/E9/0F8x byte as an instruction, which only adds targets, so
a function it reports uncalled has no caller at all.

An uncalled external function is dead code the linker kept, except in a unit built with /Ob2 (NetImmerse), where any
function defined before its callers may be inlined (`ob2`). An uncalled *helper* is different: an inline or template
function defined in a header, or a `static` or `inline` one in its unit. Compiler-generated members (deleting
destructors) are left out: no source names them. VC6 decides inlining in the back end, after
the front end has emitted the out-of-line copy for every helper a function of the unit names, so an uncalled copy
means some function of that unit (the unit retail kept it in, under /YX) called the helper and the back end inlined
every call. Where our source writes that body out by hand, the caller should call the helper instead: that lowers
the caller's own cost and moves the inline budget (docs/levers/inline.md, "A retail out-of-line copy of a unit's own
helper with no callers"). A helper whose copy we don't emit at all (NO-SYMBOL) is the strongest lead: nothing of ours
names it yet.

For each candidate the tool prints the unit's open functions (DIFF, NO-SYMBOL, NO-OBJ: score, PARKED/DRAFT, wall),
each marked `names it` when its source already spells the helper's name followed by `(`; the others come first. The
unit is the annotation's binding unit, or for a helper no unit defines, the units of the annotated functions on
either side of retail's copy. `--gaps` adds the functions of unwritten gaps (split as dupescan.py splits them) nothing
calls, with the unit of the annotated function before them: helpers without an identity yet.

Reads build/report.json (run `build` or `match <unit>` first) and the retail image. A report only: it writes no
source and no annotation.
"""
import argparse
import bisect
import json
import re
import sys
from collections import Counter, defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import decomp  # noqa: E402

HEADER_SUFFIXES = (".h", ".hpp", ".inl")
SOURCE_SUFFIXES = (".c", ".cpp")
OPEN = ("DIFF", "NO-SYMBOL", "NO-OBJ", "REFERENT", "WRONG-REFERENT")
HELPER_KINDS = ("header", "static", "inline", "template", "ob2")
GENERATED = re.compile(r"deleting destructor'|^\?\?_[EG]")   # compiler-generated: nothing in source calls them


def auto_inlines(unit):
    """Whether the unit builds with /Ob2 (any function defined earlier in the unit may be inlined): its module's
    flags, then its own overrides, the last /Ob wins."""
    for m in decomp.PROJECT["units"]:
        if ("src/" + unit).startswith(m["path"] + "/"):
            flags = list(m.get("flags", [])) + list(m.get("overrides", {}).get(unit, []))
            obs = [f for f in flags if f.startswith("/Ob")]
            return bool(obs) and obs[-1] == "/Ob2"
    return False


def referenced(img):
    """{address: uses} of every address some code or data could reach: the rel32 targets of each E8 (call), E9 (jmp)
    and 0F 80-8F (jcc) byte sequence in .text, read at every offset, and the dwords at the image's base-relocation
    sites. The use counts tell a common referent (operator new, a shared constant) from a distinctive one."""
    _, va, size, off, raw = img.text
    text = img.data[off:off + min(size, raw)]
    out = Counter()
    for opcode in (b"\xe8", b"\xe9"):
        i = text.find(opcode)
        while i != -1:
            if i + 5 <= len(text):
                out[(va + i + 5 + int.from_bytes(text[i + 1:i + 5], "little", signed=True)) & 0xFFFFFFFF] += 1
            i = text.find(opcode, i + 1)
    i = text.find(b"\x0f")
    while i != -1:
        if i + 6 <= len(text) and 0x80 <= text[i + 1] <= 0x8F:
            out[(va + i + 6 + int.from_bytes(text[i + 2:i + 6], "little", signed=True)) & 0xFFFFFFFF] += 1
        i = text.find(b"\x0f", i + 1)
    for site in img.relocs:
        word = img.read(site, 4)
        if word:
            out[int.from_bytes(word, "little")] += 1
    return out


def entry_point(img):
    """The image's entry point (AddressOfEntryPoint): reached without a caller."""
    pe = int.from_bytes(img.data[0x3C:0x40], "little")
    return img.base + int.from_bytes(img.data[pe + 40:pe + 44], "little")


def definition_kind(path, text):
    """How the annotated definition links: `header` (defined in a header: an inline or template member), `static`,
    `inline` or `template` (by the keywords before its name in a .c/.cpp), else `extern`."""
    if Path(path).suffix.lower() in HEADER_SUFFIXES:
        return "header"
    head = []
    for line in text.splitlines():
        s = line.strip()
        if s.startswith("//") or not s:
            continue
        head.append(s.split("//")[0])
        if "(" in s or "{" in s:
            break
    head = " ".join(head)
    head = head.split("(")[0]
    if re.search(r"\bstatic\b", head):
        return "static"
    if re.search(r"\b(?:__forceinline|__inline|inline)\b", head):
        return "inline"
    if re.search(r"\btemplate\b", head):
        return "template"
    return "extern"


def annotated_sources():
    """{address: (path, definition text)} of every FUNCTION annotation in src/ (headers and included files too)."""
    out = {}
    for p in sorted(decomp.SRC.rglob("*")):
        if p.suffix.lower() not in HEADER_SUFFIXES + SOURCE_SUFFIXES:
            continue
        rel = p.relative_to(decomp.SRC).with_suffix("").as_posix()
        for e in decomp.scan(rel, p):
            if e.kind == "FUNCTION":
                out.setdefault(e.addr, (p.relative_to(decomp.ROOT).as_posix(), e.text))
    decomp.save_scan_cache()
    return out


def short_name(name):
    """The name a call spells: the last component of `Class::Method`; a constructor's is `Class::` and its class
    (`new Class`, `Class object;` spell it without parentheses)."""
    name = (name or "").split("(")[0]
    parts = name.split("::")
    if len(parts) >= 2 and parts[-1] == parts[-2]:
        return parts[-1] + "::"
    return parts[-1].lstrip("~") if name else ""


def names_it(text, helper):
    """Whether a caller's source spells the helper's name as a call (`Name (` or `Name(`), or for a constructor
    (`Class::`) its class name."""
    if helper.endswith("::"):
        return re.search(r"(?<![\w~])%s\b" % re.escape(helper[:-2]), text or "") is not None
    return bool(helper) and re.search(r"(?<![\w~])%s\s*\(" % re.escape(helper), text or "") is not None


def neighbour_units(addr, ordered):
    """The units of the annotated functions before and after addr (`ordered`: [(address, unit)] sorted), skipping
    rows without a compiled unit."""
    i = bisect.bisect_left(ordered, (addr, ""))
    out = []
    for j in (i - 1, i + 1 if i < len(ordered) and ordered[i][0] == addr else i):
        if 0 <= j < len(ordered) and not ordered[j][1].startswith("("):
            out.append(ordered[j][1])
    return list(dict.fromkeys(out))


COMMON = 40   # uses: a referent reached from more places than this is too common to trace an inlined copy by


def distinctive(helper_refs, uses):
    """The helper's referents that few places reach (at most COMMON uses): the ones whose presence in a caller says
    something."""
    return frozenset(t for t in helper_refs if t[0] != "disp" and uses.get(t[1], 0) <= COMMON)


def traceable(helper_refs, uses):
    """The helper's referents another function can share: those reached from more than one place (a referent only the
    helper reaches, its own EH handler table, can't be in a caller)."""
    return frozenset(t for t in helper_refs if t[0] == "disp" or uses.get(t[1], 0) > 1)


MIN_SHAPE = 1   # member displacements: a helper that reaches no referent is traced by at least this many


def features(code, addr, disasm):
    """("disp", value) of every member displacement and vtable slot in a body (`[reg + 0x168]`, `call [edx + 0x94]`),
    the stack-based ones left out (they move with the frame): what an inlined copy of a referent-free helper keeps."""
    out = set()
    for _, _, ops, _ in disasm(code, addr):
        for base, disp in re.findall(r"\[(\w+)(?: \+ \w+\*\d)? \+ (0x[0-9a-f]+)\]", ops):
            if base not in ("esp", "ebp"):
                value = int(disp, 16)
                if 0x8 <= value < 0x10000:
                    out.add(("disp", value))
    return out


def holds(helper_refs, caller_refs):
    """Whether a caller's retail body reaches every referent (callee, global, constant) the helper's own body does:
    the trace an inlined copy leaves. None when the helper reaches nothing (pure arithmetic: no trace to look for)."""
    if not helper_refs:
        return None
    return helper_refs <= caller_refs


def smaller(refs_of, helper, caller):
    """Whether the helper is at most half the caller's size (True when either size is unknown): an inlined copy is
    part of its caller, a sibling function of the same size only looks alike."""
    size = getattr(refs_of, "size", None)
    if size is None:
        return True
    h, c = size(helper), size(caller)
    return not h or not c or 2 * h <= c


def open_callers(units, rows_by_unit, sources, helper, exclude, helper_refs=frozenset(), refs_of=None):
    """The open functions of `units`: those whose retail body holds the helper's referents first, then those that
    don't name the helper, then by score (highest first)."""
    out = []
    for u in units:
        for r in rows_by_unit.get(u, ()):
            a = int(r["addr"], 16)
            if a == exclude or r["status"] not in OPEN or r.get("static"):
                continue
            text = sources.get(a, (None, ""))[1]
            held = holds(helper_refs, refs_of(a)) if refs_of else None
            if held and not smaller(refs_of, exclude, a):
                held = False   # a sibling as large as the helper shares its callees without having inlined it
            out.append(dict(addr=r["addr"], name=r["name"], unit=u, status=r["status"], score=r["score"],
                            names_it=names_it(text, helper), holds=held, note=r.get("parked") or r.get("draft") or "",
                            noted="parked" if r.get("parked") else "draft" if r.get("draft") else "",
                            wall=wall_of(r)))
    out.sort(key=lambda c: (c["holds"] is not True, c["names_it"], -c["score"]))
    return out


def wall_of(row):
    try:
        import openwork
        wall, src = openwork.wall_of(row)
        return wall if src else ""
    except Exception:
        return row.get("wall", "")


def candidates(rows, sources, refs, show_all=False, unit=None, refs_of=None):
    """[candidate dict] over the report's annotated functions, strongest first: NO-SYMBOL helpers, then by the
    number of open callers whose retail body holds the helper's referents and whose source doesn't name it.

    `refs_of(addr)` gives the referents of retail's body at addr (callees, globals, constants: `RetailRefs`). Without
    --all a candidate is kept when it is NO-SYMBOL, when an open caller whose source doesn't name it holds its
    distinctive referents (a caller that names it already calls it), or when it reaches nothing (no trace to test)
    and is a declared helper (not merely `ob2`) with open callers."""
    rows = [r for r in rows if not r.get("static") and r["status"] != "PATCHED"]
    rows_by_unit = defaultdict(list)
    for r in rows:
        rows_by_unit[r["unit"]].append(r)
    ordered = sorted((int(r["addr"], 16), r["unit"]) for r in rows)
    out = []
    for r in rows:
        a = int(r["addr"], 16)
        if a in refs:
            continue
        path, text = sources.get(a, ("", ""))
        kind = definition_kind(path, text) if path else "extern"
        units = [r["unit"]] if not r["unit"].startswith("(") else neighbour_units(a, ordered)
        if kind == "extern" and units and all(auto_inlines(u) for u in units):
            kind = "ob2"   # an external function /Ob2 may inline into every caller of its unit
        if not show_all and (kind not in HELPER_KINDS or GENERATED.search(r["name"] or "")
                             or GENERATED.search(r.get("symbol") or "")):
            continue
        if unit and unit not in units:
            continue
        own = traceable(refs_of(a), refs) if refs_of else frozenset()
        callers = open_callers(units, rows_by_unit, sources, short_name(r["name"]), a, own, refs_of)
        rare = distinctive(own, refs)
        shape = sum(1 for t in own if t[0] == "disp")
        if not show_all and r["status"] != "NO-SYMBOL":
            traced = (rare or (len(own) == shape and shape >= MIN_SHAPE)) and                 any(k["holds"] and not k["names_it"] for k in callers)
            if not traced:
                continue
        out.append(dict(addr=r["addr"], name=r["name"], kind=kind, path=path, status=r["status"], score=r["score"],
                        units=units, referents=len(own) - shape, distinctive=len(rare), shape=shape, traces=sorted(t[1] for t in rare),
                        callers=callers))
    out.sort(key=lambda c: (c["status"] != "NO-SYMBOL", -c["distinctive"],
                            -sum(1 for k in c["callers"] if k["holds"] and not k["names_it"]),
                            -sum(1 for k in c["callers"] if not k["names_it"]), int(c["addr"], 16)))
    return out


class RetailRefs:
    """refs_of(addr): the referents of retail's body at addr (dupescan's relocation-masked signature, the targets
    outside the body), scanned up to the next annotated address."""

    def __init__(self, img, rows):
        import dupescan
        self.retail = dupescan.Retail(img)
        self.starts = sorted(int(r["addr"], 16) for r in rows)
        self.memo = {}
        self.sizes = {}

    def __call__(self, addr):
        if addr not in self.memo:
            import openwork
            i = bisect.bisect_right(self.starts, addr)
            stop = min(self.starts[i] if i < len(self.starts) else addr + openwork.SCAN_CAP, addr + openwork.SCAN_CAP)
            import dupescan
            sig = self.retail.body(addr, stop)
            refs = set(t for t in sig[1] if t[0] != "self") if sig else set()
            if sig:
                code = self.retail.img.read(addr, len(sig[0]))
                if code:
                    refs |= features(code, addr, dupescan.disasm_lite)
            self.memo[addr] = frozenset(refs)
            self.sizes[addr] = len(sig[0]) if sig else 0
        return self.memo[addr]

    def size(self, addr):
        """Retail's body size at addr (0 when unknown)."""
        self(addr)
        return self.sizes[addr]

    def save(self):
        self.retail.save()


def aligned_start(img, start, floor):
    """Where a gap's first function begins: a gap after a non-exact function starts at its compiled end, rounded up,
    which can fall inside retail's next function when ours is shorter. Backs up in 16-byte steps (no lower than
    floor) until the byte before is padding or the last byte of a `ret`."""
    a = start
    while a - 16 > floor:
        before = img.read(a - 1, 1)
        if before is None or before[0] in (0x90, 0xCC, 0xC3):
            break
        ret = img.read(a - 3, 3)
        if ret and ret[0] == 0xC2:
            break
        a -= 16
    return a


def gap_candidates(rows, refs, img, unit=None, refs_of=None):
    """The functions of unwritten gaps nothing calls whose referents an open caller of the unit before the gap holds."""
    import dupescan
    import openwork
    rows = [r for r in rows if r["status"] != "PATCHED"]
    gaps, _, _, _ = openwork.unwritten_gaps(rows)
    rows_by_unit = defaultdict(list)
    for r in rows:
        if not r.get("static"):
            rows_by_unit[r["unit"]].append(r)
    out = []
    for start, end, prev, _ in gaps:
        u = prev.get("unit") or ""
        if not u or u.startswith("(") or (unit and u != unit):
            continue
        for a, size in dupescan.gap_functions(img, aligned_start(img, start, int(prev["addr"], 16)), end):
            if a in refs or size < 4:
                continue
            own = traceable(refs_of(a), refs) if refs_of else frozenset()
            callers = open_callers([u], rows_by_unit, {}, "", a, own, refs_of)
            rare = distinctive(own, refs)
            shape = sum(1 for t in own if t[0] == "disp")
            if not (rare or (len(own) == shape and shape >= MIN_SHAPE)) or not any(k["holds"] for k in callers):
                continue   # a gap function has no name to spell: any holder counts
            out.append(dict(addr="0x%X" % a, name=None, kind="gap", path="", status="UNWRITTEN", score=0.0,
                            size=size, units=[u], referents=len(own) - shape, distinctive=len(rare), shape=shape,
                            traces=sorted(t[1] for t in rare), callers=callers))
    return out


def print_candidates(cands, limit=8, names=None):
    """The candidates, each with its distinctive referents (`names`: {address: name}) and its open callers."""
    names = names or {}
    for c in cands:
        # a lead is a caller whose retail body holds the helper's referents and whose source doesn't call it; the
        # unit's other open functions are listed for context only
        loose = sum(1 for k in c["callers"] if k["holds"] and not k["names_it"])
        print("%s %-9s %-8s %s  [%s]  %d referents (%d distinctive), %d displacements, %s%s%s" % (
            c["addr"], c["status"], c["kind"], c["name"] or "(gap, %d bytes)" % c.get("size", 0),
            ", ".join(c["units"]) or "no unit", c["referents"], c["distinctive"], c.get("shape", 0),
            "%d open functions in the unit" % len(c["callers"]),
            ", %d hold its referents without calling it" % loose if loose else "",
            "  (extern outside /Ob2: an uncalled copy is dead code, not an inlined helper)"
            if c["kind"] == "extern" else ""))
        if c.get("traces"):
            print("    traced by: %s" % ", ".join("0x%X %s" % (t, names.get(t, "")) for t in c["traces"][:6]).rstrip())
        for k in c["callers"][:limit]:
            print("    %s %5.1f %-9s %s%s%s%s%s" % (
                k["addr"], k["score"], k["status"], k["name"], "  (holds its referents)" if k["holds"] else "",
                "  (names it)" if k["names_it"] else "",
                "  wall: %s" % k["wall"] if k["wall"] else "",
                "  %s: %s" % (k["noted"].upper(), k["note"][:60]) if k["noted"] else ""))
        if len(c["callers"]) > limit:
            print("    ... %d more" % (len(c["callers"]) - limit))


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("--unit", help="only candidates whose callers' unit is this one")
    ap.add_argument("--all", action="store_true", help="also candidates without open callers, and external functions")
    ap.add_argument("--gaps", action="store_true", help="also unwritten gap functions nothing calls")
    ap.add_argument("--limit", type=int, default=8, help="open callers printed per candidate (default 8)")
    ap.add_argument("--json", help="write the candidates to this file")
    args = ap.parse_args(argv)
    rows = decomp.load_report()["functions"]
    img = decomp.pe.Image(decomp.target_image())
    refs = referenced(img)
    refs[entry_point(img)] += 1
    refs_of = RetailRefs(img, rows)
    cands = candidates(rows, annotated_sources(), refs, args.all, args.unit, refs_of)
    if args.gaps:
        cands += gap_candidates(rows, refs, img, args.unit, refs_of)
    refs_of.save()
    names = {int(r["addr"], 16): r["name"] for r in rows}
    for name, addr in decomp.extra_symbols().items():
        names.setdefault(addr, name)
    print_candidates(cands, args.limit, names)
    print("%d candidates: %d NO-SYMBOL, %d held by an open caller that doesn't name the helper%s" % (
        len(cands), sum(1 for c in cands if c["status"] == "NO-SYMBOL"),
        sum(1 for c in cands if any(k["holds"] and not k["names_it"] for k in c["callers"])),
        ", %d gap functions" % sum(1 for c in cands if c["kind"] == "gap") if args.gaps else ""))
    if args.json:
        Path(args.json).write_text(json.dumps(cands, indent=1), encoding="utf-8")
    return 0


if __name__ == "__main__":
    sys.exit(main())
