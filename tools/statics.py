"""Renumber `_$E` static-initialiser annotations after their compiled names shift, and place the NetImmerse
registration initialisers in their units.

    python tools/statics.py renumber [<checkout>]      # rewrite shifted _$E annotations; prints what changed
    python tools/statics.py place-all [--write] [--evict] [--units <unit>... | --changed [<base>]]
                                                       # every unit's registrations, placed by their owner
    python tools/statics.py place <unit> <lo> <hi> [--write]   # annotate the unit's NetImmerse registrations there

VC6 numbers a unit's compiler-generated static initialisers (`_$E447`, `_$E448`, ...) in the order it meets them,
and every inline, annotation or class-scope static added to a header the unit includes shifts the numbers. A matched
`_$E` function then loses its match with its source unchanged (AppWrap's 0x64C570/0x64C580 moved by one per lane).

For each annotated `_$E` function that is no longer MATCH (from the checkout's build/report.json, so run a build or
`match` first; the units with such a row are compared afresh with the checkout's `match` before anything is read, since
a slot's build/ can be older than its source), this compares every `_$E` function in the unit's object with retail's bytes at the annotated address,
ignoring the four bytes at each relocation. It applies one shift per unit when the failing annotations agree on it, or
one shift per block of the unit's annotations when they agree block by block (the `<iostream>` quads kept their
numbers, everything after them moved by two). A row that was failing before the shift (not MATCH in
config/baseline.json) doesn't vote: it takes its block's shift and is printed "left failing" (CameraMode's 0x41DFB0).
Rows that really disagree are left for a human, and printed. An address that exactly one of the unit's
initialisers fits, its relocation targets included (the Register a registration calls, the body a `jmp` stub enters),
is settled first and on its own: the registration stubs are byte-identical apart from those targets. Last, each `jmp`
stub of a renumbered unit is named after the body it enters (a stub left at its old number still compiles to a `jmp`,
so only the pairing shows it). An annotation no initialiser of the unit fits isn't a shift, and is left unreported.

The NetImmerse registrations (NiStreamMacros.h: a `jmp` stub and a `call NiXxx::Register; mov [static], eax; ret` body
per streamable class a unit includes) sit among and after their unit's code. `place-all` gives every one its owner:
the linker lays `.text` and `.bss` out in the same object order, so the bodies cut into blocks wherever every earlier
store lies below every later one (`blocks`), and each block is one unit's. A block goes to the one unit with `.bss`
data among its stores (DATA annotations, the statics its matched functions and its other `_$E` initialisers reach),
else the one unit with annotated code among its stubs, else the one unit with data just past its last store, else the
unit whose blocks surround it (`data_first` keeps a data-owned unit out of the others); a block nothing decides takes the default order
(`default_order`: the unit of the code right before it, or the next unit after unannotated code). A unit takes a stub
only when its object fits it, bytes and targets, each initialiser once, and a `jmp` stub goes with its body. Existing
registration annotations move only when the evidence places them elsewhere and the numbers they need are free; one
the evidence can't re-home keeps its place (`--evict` drops it, for a `regress --accept`). Build first: it reads
build/report.json and the objects. `place` annotates one unit's stubs in [lo, hi) when the caller has the evidence.
`--write` rewrites each changed unit's registration block (created after its last `#include`).
"""

import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import coff  # noqa: E402
import comdat  # noqa: E402
import decomp  # noqa: E402

STATIC = re.compile(r"_\$E(\d+)$")
ANNOT = re.compile(r"(//\s*FUNCTION:\s*STBC\s+0x0*%s\s+)_\$E(\d+)")


def masked(code, relocs, start, end):
    """The function's bytes with every relocated dword set to None (relocations hold no comparable value)."""
    out = list(code)
    for off, _, _ in relocs:
        if start <= off < end:
            for k in range(off - start, min(off - start + 4, len(out))):
                out[k] = None
    return out


def same(mask, retail):
    return retail is not None and len(retail) == len(mask) and all(m is None or m == r for m, r in zip(mask, retail))


def static_functions(obj):
    """The object's `_$E` functions, read once per object: (number, section, masked bytes)."""
    found = getattr(obj, "_static_functions", None)
    if found is None:
        found = obj._static_functions = []
        for s in obj.symbols.values():
            m = STATIC.match(s.name)
            if not m or not s.defined:
                continue
            sec = obj.section(s.section)
            if not sec.is_code:
                continue
            start, end = obj.function_extent(s)
            found.append((int(m.group(1)), sec, masked(sec.data[start:end], sec.relocs, start, end)))
    return found


def candidates(obj, img, addr, known=None):
    """Numbers of the unit's `_$E` functions whose bytes fit retail's at addr. With `known` (comdat.known_addresses),
    relocations that name a symbol of known address must reach it too (which Register a registration calls, which
    initialiser body a `jmp` stub enters)."""
    out = []
    for number, sec, mask in static_functions(obj):
        if not same(mask, img.read(addr, len(mask))):
            continue
        if known is not None and not comdat.targets_agree(img, obj, sec, addr, known):
            continue
        out.append(number)
    return out


def unit_source(root, unit):
    for ext in (".cpp", ".c"):
        p = Path(root) / "src" / (unit + ext)
        if p.exists():
            return p
    return None


def failing_static_units(rows):
    """The units with an annotated `_$E` initialiser the rows don't call MATCH: the only units renumber rewrites."""
    return sorted({r["unit"] for r in rows if r.get("static") and r.get("status") != "MATCH"
                   and not r["unit"].startswith("(")})


def match_units(root, units):
    """Compile and compare the units with the checkout's own tools (`match` refreshes their objects and their rows in
    build/report.json). A fresh object comes straight from build/objcache, so this costs little after a build."""
    import subprocess
    root = Path(root)
    subprocess.run([sys.executable, str(root / "tools" / "decomp.py"), "match", *units], cwd=str(root),
                   capture_output=True, text=True, errors="replace")


def renumber(root, refresh=match_units):
    """Rewrite the shifted annotations; return (changes, problems) as printable lines.

    The report's rows and the objects can be older than the source: a slot worktree keeps the build/ of its previous
    lane, so after a reset to main its rows and objects describe other headers, and a shift read from them is no
    shift of the current source. So each unit renumber would rewrite is compared afresh first (`refresh`, the
    checkout's `match`); on a fresh build that reproduces the same objects and rows, and changes nothing."""
    root = Path(root)
    report = root / "build" / "report.json"
    if not report.exists():
        return [], ["no build/report.json: run `python tools/decomp.py build` first"]
    rows = decomp.load_json(report).get("functions", [])
    todo = failing_static_units(rows)
    if todo and refresh is not None:
        refresh(root, todo)   # a unit that no longer compiles loses its object, and is skipped below
        rows = decomp.load_json(report).get("functions", [])
    img = decomp.pe.Image(decomp.target_image())
    changes, problems = [], []
    # The report's rows say which annotations fail. `build` and `match` write them with the objects; an object
    # compiled since (by `diff`, or a `match` without a report to refresh) has rows that describe an older source.
    stale = stale_rows(rows, report, root / "build" / "obj")
    for unit in stale:
        problems.append("%s: its object is newer than build/report.json; run `python tools/decomp.py match %s`, "
                        "then renumber" % (unit, unit))
    units = sorted(set(failing_static_units(rows)) - set(stale))
    known = None
    baseline = decomp.load_json(root / "config" / "baseline.json") if (root / "config" / "baseline.json").exists() \
        else {}
    for unit in units:
        path = unit_source(root, unit)
        objp = root / "build" / "obj" / (unit + ".obj")
        if path is None or not objp.exists():
            continue
        text = path.read_bytes().decode("latin-1")   # bytes: keep the file's own line endings
        failing = [r["addr"] for r in rows if r["unit"] == unit and r.get("status") != "MATCH"]
        if not failing:
            continue
        obj = coff.Coff(objp.read_bytes())
        if known is None:
            known = comdat.known_addresses(comdat.all_annotations())
        new, unit_changes, unit_problems = renumber_unit(
            unit, text, failing,
            fit=lambda a, obj=obj: candidates(obj, img, a),
            fit_targets=lambda a, obj=obj: candidates(obj, img, a, known),
            was_failing=lambda a: was_failing(baseline, a),
            repair=lambda t, ch, unit=unit, obj=obj: repair_pairs(unit, t, obj, img, ch))
        changes += unit_changes
        problems += unit_problems
        if new != text:
            path.write_bytes(new.encode("latin-1"))
    return changes, problems


def was_failing(baseline, addr):
    """True when the baseline (the state the lane started from) holds no MATCH for addr: a row that failed before
    any shift, which a shift can't be read from (CameraMode's 0x41DFB0)."""
    row = baseline.get("0x%X" % (addr if isinstance(addr, int) else int(addr, 16)))
    return not row or row.get("status") != "MATCH"


def annotations(text):
    """The text's `_$E` annotations in order: [(address "0x..", number)]."""
    return [("0x%X" % int(m.group(1), 16), int(m.group(2))) for m in re.finditer(
        r"//\s*FUNCTION:\s*STBC\s+(0x[0-9A-Fa-f]+)\s+_\$E(\d+)", text)]


def rename(text, addr, new):
    return re.sub(ANNOT.pattern % addr[2:].lstrip("0"), lambda m: "%s_$E%d" % (m.group(1), new), text, flags=re.I)


def shift_plan(shifters, stuck, order):
    """The shift of each failing annotation, or None when the rows really disagree.

    shifters: {address: set of shifts its bytes allow} for the annotations whose number moved (a non-empty set
    without 0); stuck: the addresses of the failing annotations that were failing before the shift (the baseline)
    and fit no shift, or several; order: every `_$E` annotation's address in the unit's text order.

    One shift every shifter allows applies to all of them. Failing that, a shift per block: the text order cut where
    the shifters' common shift changes (TGObject: the `<iostream>` quads kept their numbers, everything after them
    moved by two). An annotation that was failing already doesn't vote; it takes the shift of its block (the hand
    procedure: the failing row shifts with its neighbours). Returns {address: shift}."""
    voters = {a: d for a, d in shifters.items() if a not in stuck}
    if not voters:
        return None
    common = set.intersection(*voters.values())
    if len(common) == 1:
        delta = common.pop()
        return {a: delta for a in list(shifters) + list(stuck)}
    # Blocks: walk the text order, keeping a running intersection; cut when it would empty.
    blocks, current, members = [], None, []
    for a in order:
        if a not in voters:
            members.append(a)
            continue
        nxt = voters[a] if current is None else current & voters[a]
        if not nxt:
            blocks.append((current, members))
            current, members = set(voters[a]), [a]
        else:
            current = nxt
            members.append(a)
    blocks.append((current, members))
    if any(d is None or len(d) != 1 for d, _ in blocks):
        return None
    plan = {}
    for d, members in blocks:
        delta = next(iter(d))
        for a in members:
            if a in shifters or a in stuck:
                plan[a] = delta
    # A block of one voter is a disagreement, not a block (two annotations pulling apart).
    if any(sum(1 for a in members if a in voters) < 2 for _, members in blocks):
        return None
    return plan


def renumber_unit(unit, text, failing, fit, fit_targets, was_failing, repair=lambda t, ch: t):
    """One unit's renumbering: (new text, changes, problems).

    failing: the addresses of its non-MATCH rows; fit(addr) and fit_targets(addr): the numbers of the unit's
    initialisers whose bytes fit retail's at addr (fit_targets: their known relocation targets too); was_failing(addr):
    the row failed before the shift (the baseline); repair(text, changes): the `jmp` stub pairing (repair_pairs).

    The targets check reads the unit's own annotations, which are the stale ones while it renumbers: a `jmp` stub can
    then fit one initialiser alone and wrongly (TGObject's 0x6F08B0 entered a byte-identical twin of its body). When
    an address settled alone takes a number another annotation ends with, it is distrusted and joins the shift
    instead, and the unit is planned again."""
    distrust = set()
    for _ in range(4):
        result, clash = _renumber_unit(unit, text, failing, fit, fit_targets, was_failing, repair, distrust)
        if not clash - distrust:
            break
        distrust |= clash
    return result


def _renumber_unit(unit, text, failing, fit, fit_targets, was_failing, repair, distrust):
    """renumber_unit's one pass: ((text, changes, problems), the addresses settled alone whose number is doubled)."""
    original, changes, problems = text, [], []
    annotated = dict(annotations(text))
    fails = [("0x%X" % int(a, 16), annotated["0x%X" % int(a, 16)]) for a in failing
             if "0x%X" % int(a, 16) in annotated]
    if not fails:
        return (text, changes, problems), set()
    settled = set()

    def done(text):
        """The renumbered text, unless it gives one number to two annotations it didn't before."""
        text = repair(text, changes)
        twice = sorted(set(doubled(text)) - set(doubled(original)))
        if twice:
            problems.append("%s: renumbering would give %s to two annotations; left as it was" % (
                unit, ", ".join("_$E%d" % n for n in twice)))
            del changes[:]
            now = dict(annotations(text))
            return (original, changes, problems), {a for a in settled if now.get(a) in twice}
        return (text, changes, problems), set()

    # A registration stub or its jmp is told apart only by its targets: settle each address alone when exactly one of
    # the unit's initialisers fits it, bytes and targets both.
    for addr, old in fails:
        if addr in distrust:
            continue
        c = fit_targets(int(addr, 16))
        if len(c) == 1 and c[0] != old:
            text = rename(text, addr, c[0])
            changes.append("%s %s: _$E%d -> _$E%d" % (unit, addr, old, c[0]))
            settled.add(addr)
    fails = [f for f in fails if f[0] not in settled]
    if not fails:
        return done(text)
    shifters, twins, stuck = {}, {}, []
    for addr, old in fails:
        d = {n - old for n in fit(int(addr, 16))}
        if d and 0 not in d:
            shifters[addr] = d
        elif 0 in d and len(d) > 1 and not was_failing(addr):
            # Its own name still fits its bytes, and so do others: byte-identical twins (exit destructors). It fails
            # either for another reason or because the run shifted onto its twin's number; it votes only when the
            # shifters alone can't settle the run (VisibleDamageBuilder's reverse-numbered 0x4B5620/0x4B5630).
            twins[addr] = d
        if was_failing(addr) and (not d or 0 in d or len(d) > 1):
            stuck.append(addr)   # failing before the shift: no evidence of its own, it moves with its block
        # otherwise: no initialiser fits, or its own name still does: it fails for another reason, not a shift
    olds = dict(fails)
    if not set(shifters) - set(stuck):
        # Only rows that failed before: nothing shifted, and they fail for their own reasons.
        for addr in stuck:
            problems.append("%s: left failing: %s _$E%d (failing before; no shift to apply)" % (unit, addr, olds[addr]))
        return done(text)
    order = [a for a, _ in annotations(text)]
    plan = shift_plan(shifters, set(stuck), order)
    if plan is None and twins:
        # One offset for the whole run, whatever order its numbers run in: each twin allows its own name and its
        # twins' names, and with the shifters only the shift they share is left.
        plan = shift_plan(dict(shifters, **twins), set(stuck), order)
        if plan is not None:
            plan = {a: delta for a, delta in plan.items() if delta or a not in twins}
    if plan is None:
        problems.append("%s: no single shift fits %s" % (unit, ", ".join(
            "%s _$E%d" % (a, olds[a]) for a in sorted(shifters, key=lambda x: int(x, 16)))))
        return done(text)
    # A passing annotation whose name a shifted one is about to take passes only because its bytes equal another
    # static's (exit destructors are byte-identical): it shifted too, when its bytes fit the shifted name, by the
    # shift of the block it sits in.
    everyone = annotations(text)
    position = {a: k for k, (a, _) in enumerate(everyone)}
    planned = sorted(plan, key=lambda a: position[a])

    def block_shift(addr):
        before = [a for a in planned if position[a] <= position[addr]]
        return plan[before[-1]] if before else plan[planned[0]]
    taken = {olds[a] + plan[a] for a in plan}
    grown = True
    while grown:
        grown = False
        for addr, old in everyone:
            if addr in plan or old not in taken:
                continue
            delta = block_shift(addr)
            if old + delta in fit(int(addr, 16)):
                plan[addr], olds[addr] = delta, old
                taken.add(old + delta)
                grown = True
    for addr in sorted(plan, key=lambda a: position[a]):
        text = rename(text, addr, olds[addr] + plan[addr])
        changes.append("%s %s: _$E%d -> _$E%d" % (unit, addr, olds[addr], olds[addr] + plan[addr]))
    for addr in stuck:
        if addr in plan and olds[addr] + plan[addr] not in fit(int(addr, 16)):
            problems.append("%s: left failing: %s (failing before the shift; moved with its neighbours, _$E%d -> "
                            "_$E%d)" % (unit, addr, olds[addr], olds[addr] + plan[addr]))
    return done(text)


def doubled(text):
    """The `_$E` numbers two or more of the text's annotations hold."""
    seen = {}
    for n in re.findall(r"//\s*FUNCTION:\s*STBC\s+0x[0-9A-Fa-f]+\s+_\$E(\d+)", text):
        seen[int(n)] = seen.get(int(n), 0) + 1
    return [n for n, k in seen.items() if k > 1]


def stale_rows(rows, report, objdir):
    """The units of rows whose object in objdir was written after the report: their rows are older than the code."""
    import os
    when = os.stat(report).st_mtime_ns
    out = []
    for unit in sorted({r["unit"] for r in rows}):
        objp = Path(objdir) / (unit + ".obj")
        if objp.exists() and os.stat(objp).st_mtime_ns > when:
            out.append(unit)
    return out


def jmp_stubs(obj):
    """The object's `jmp` stubs: initialiser number -> the number of the initialiser it enters."""
    found = getattr(obj, "_jmp_stubs", None)
    if found is None:
        found = obj._jmp_stubs = {}
        for s in obj.symbols.values():
            m = STATIC.match(s.name)
            if not m or not s.defined or not obj.section(s.section).is_code:
                continue
            sec = obj.section(s.section)
            start, end = obj.function_extent(s)
            if sec.data[start:start + 1] != b"\xe9" or sec.data[start + 5:end].strip(b"\x90\xcc"):
                continue
            for off, symidx, _ in sec.relocs:
                t = STATIC.match(obj.symbols[symidx].name) if off == start + 1 else None
                if t:
                    found[int(m.group(1))] = int(t.group(1))
    return found


def pair_renames(annotated, retail_target, stubs):
    """The stub annotations a renumbering left behind: each `jmp` stub is named after the body it enters.

    annotated: address -> number, the unit's `_$E` annotations as they now read; retail_target: stub address -> the
    address retail's `jmp` enters; stubs: the object's stub number -> body number (jmp_stubs). A stub is renamed when
    retail's target is annotated in the unit and exactly one of the object's stubs enters that body. Returns
    [(address, old number, new number)]."""
    by_body = {}
    for stub, body in stubs.items():
        by_body.setdefault(body, []).append(stub)
    out = []
    for addr in sorted(retail_target):
        body = annotated.get(retail_target[addr])
        named = by_body.get(body, []) if body is not None else []
        if addr in annotated and len(named) == 1 and named[0] != annotated[addr]:
            out.append((addr, annotated[addr], named[0]))
    return out


def jmp_target(img, addr):
    """Where retail's 5-byte `jmp` at addr goes, or None when addr holds no `jmp`."""
    c = img.read(addr, 5)
    if not c or c[0] != 0xE9:
        return None
    return (addr + 5 + int.from_bytes(c[1:5], "little", signed=True)) & 0xFFFFFFFF


def repair_pairs(unit, text, obj, img, changes):
    """Rename the unit's `jmp` stubs after the bodies they enter (pair_renames), once the bodies are renumbered: a
    stub left at its old number still compiles to some `jmp`, so the shift steps can't tell it from a fit."""
    annotated = {int(m.group(1), 16): int(m.group(2)) for m in re.finditer(
        r"//\s*FUNCTION:\s*STBC\s+0x([0-9A-Fa-f]+)\s+_\$E(\d+)", text)}
    retail = {}
    for addr in annotated:
        t = jmp_target(img, addr)
        if t is not None and t in annotated:
            retail[addr] = t
    for addr, old, new in pair_renames(annotated, retail, jmp_stubs(obj)):
        text = re.sub(ANNOT.pattern % ("%X" % addr), lambda m, n=new: "%s_$E%d" % (m.group(1), n), text, flags=re.I)
        changes.append("%s 0x%X: _$E%d -> _$E%d" % (unit, addr, old, new))
    return text


REGISTRATION_BLOCK = "// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h)."


def registrations(img, lo, hi):
    """Retail's registration stubs in [lo, hi): (address, kind) with kind "body" (call Register; mov [static], eax;
    ret) or "jmp" (the stub that enters a body)."""
    out = []
    for a in range(lo, hi, 16):
        c = img.read(a, 11)
        if c and c[0] == 0xE8 and c[5] == 0xA3 and c[10] == 0xC3:
            out.append((a, "body"))
        elif c and c[0] == 0xE9:
            t = (a + 5 + int.from_bytes(c[1:5], "little", signed=True)) & 0xFFFFFFFF
            d = img.read(t, 11)
            if lo <= t < hi + 16 and d and d[0] == 0xE8 and d[5] == 0xA3 and d[10] == 0xC3:
                out.append((a, "jmp"))
    return out


def place(unit, lo, hi, write):
    """Annotate the unit's registration initialisers that sit in retail's [lo, hi)."""
    objp = decomp.BUILD / "obj" / (unit + ".obj")
    path = unit_source(decomp.ROOT, unit)
    if not objp.exists() or path is None:
        sys.exit("no object or source for %s: run `python tools/decomp.py match %s` first" % (unit, unit))
    img = decomp.pe.Image(decomp.target_image())
    obj = coff.Coff(objp.read_bytes())
    names = comdat.all_annotations()
    known = comdat.known_addresses(names)
    lines, problems, used = [], [], set()
    for addr, kind in registrations(img, lo, hi):
        c = [n for n in candidates(obj, img, addr, known) if n not in used]
        if addr in names:
            problems.append("0x%X already annotated as %s" % (addr, names[addr]))
        elif len(c) != 1:
            problems.append("0x%X (%s): %s" % (addr, kind, "no initialiser of the unit fits" if not c else
                                                "several fit: " + ", ".join("_$E%d" % n for n in c)))
        else:
            used.add(c[0])
            lines.append("// FUNCTION: STBC 0x%08x _$E%d" % (addr, c[0]))
    for line in lines:
        print(line)
    for p in problems:
        print("LEFT " + p)
    if write and lines:
        write_block(path, lines)
    return len(lines), len(problems)


def write_block(path, lines):
    """Add annotation lines to the unit's registration block, creating it after the last `#include`."""
    text = path.read_bytes().decode("latin-1")
    nl = "\r\n" if "\r\n" in text else "\n"
    block = nl.join(lines) + nl
    if REGISTRATION_BLOCK in text:
        i = text.index(REGISTRATION_BLOCK) + len(REGISTRATION_BLOCK) + len(nl)
        while text.startswith("// FUNCTION: STBC 0x", i):
            i = text.index(nl, i) + len(nl)
        text = text[:i] + block + text[i:]
    else:
        last = [m.end() for m in re.finditer(r"^#include[^\n]*\n", text, re.M)]
        i = last[-1] if last else 0
        text = text[:i] + nl + REGISTRATION_BLOCK + nl + block + text[i:]
    path.write_bytes(text.encode("latin-1"))


def register_target(img, addr):
    c = img.read(addr, 5)
    return (addr + 5 + int.from_bytes(c[1:5], "little", signed=True)) & 0xFFFFFFFF


def bss_range(img):
    """Retail's uninitialised data: the tail of .data past its raw bytes."""
    for name, va, vsize, _, rawsize in img.sections:
        if name == ".data":
            return va + rawsize, va + vsize
    return 0, 0


def trimmed(addrs, spread=0x2000):
    """A unit's .bss points without those far from their median: a function annotated in the wrong unit (a header
    copy, a misplaced definition) reaches another unit's block."""
    addrs = sorted(addrs)
    if not addrs:
        return addrs
    mid = addrs[len(addrs) // 2]
    return [a for a in addrs if abs(a - mid) <= spread]


# the <iostream> objects its quads construct: generic, and the quads were often placed by layout alone, so they
# are no evidence of a unit's .bss
IOSTREAM_OBJECTS = {"__Ios_init", "__Wios_init"}


def bss_evidence(img, rows, entries, load, skip=()):
    """unit -> sorted retail .bss addresses of the unit's own data: its DATA annotations there and the uninitialised
    statics its matched functions reach (functions of its own source only, the registration initialisers at `skip`
    excluded: those are what gets placed; a unit's other `_$E` initialisers count (their guard bytes and the statics they
    construct are its own .bss; the `<iostream>` quads don't: BoneStateController's NiRTTI guard 0x991704 lies among
    the stores of its run at 0x65FCC0). COMDAT data is left out: every includer defines it and the linker keeps one copy."""
    lo, hi = bss_range(img)
    own = {(e.unit, e.addr) for e in entries if e.kind == "FUNCTION" and not e.bind}
    found = {}
    for e in entries:
        if e.kind == "DATA" and not e.bind and lo <= e.addr < hi:
            found.setdefault(e.unit, []).append(e.addr)
    for r in rows:
        if r.get("status") != "MATCH" or (r.get("static") and int(r["addr"], 16) in skip)                 or (r["unit"], int(r["addr"], 16)) not in own:
            continue
        obj = load(r["unit"])
        sym = obj and next((s for s in obj.symbols.values() if s.defined and s.name == r.get("symbol")), None)
        if sym is None:
            continue
        sec = obj.section(sym.section)
        start, end = obj.function_extent(sym)
        addr = int(r["addr"], 16)
        for off, symidx, typ in sec.relocs:
            t = obj.symbols.get(symidx)
            if not start <= off < end or typ != coff.IMAGE_REL_I386_DIR32 or t is None or not t.defined:
                continue
            data = obj.section(t.section)
            if not data.is_bss or data.flags & coff.SCN_LNK_COMDAT:
                continue
            if r.get("static") and t.name in IOSTREAM_OBJECTS:
                continue
            raw = img.read(addr + off - start, 4)
            value = int.from_bytes(raw, "little") if raw else 0
            if lo <= value < hi:
                found.setdefault(r["unit"], []).append(value)
    return {u: trimmed(a) for u, a in found.items()}


def blocks(stores):
    """Cut retail's registration bodies, in code order with their stores, where a unit boundary can fall.

    The linker lays out `.text` and `.bss` in the same object order, so a boundary between two units after the k-th
    body needs every store before it below every store after it. Between two such cuts the bodies are one unit's:
    the compiler orders a unit's statics by name, so its stores interleave however its stubs sit in its code.
    Returns [(i, j)] index ranges."""
    n = len(stores)
    low = [0] * (n + 1)
    low[n] = float("inf")
    for i in range(n - 1, -1, -1):
        low[i] = min(stores[i], low[i + 1])
    cuts, high = [0], -1
    for k in range(1, n):
        high = max(high, stores[k - 1])
        if high < low[k]:
            cuts.append(k)
    cuts.append(n)
    return [(cuts[i], cuts[i + 1]) for i in range(len(cuts) - 1) if cuts[i] < cuts[i + 1]]


def units_between(sorted_pairs, lo, hi):
    """The units of the (address, unit) pairs with lo <= address <= hi."""
    import bisect
    keys = [a for a, _ in sorted_pairs]
    return {u for _, u in sorted_pairs[bisect.bisect_left(keys, lo):bisect.bisect_right(keys, hi)]}


def block_owner(code_units, data_units):
    """A block's owner from what lies inside it: the one unit with .bss data among its stores, else (no data) the one
    unit with annotated code among its stubs. Data outranks code: a source file of this project can hold code that
    retail compiled in another unit (TopWindow.cpp's windows). Returns (unit, how) or (None, None)."""
    if len(data_units) == 1:
        return next(iter(data_units)), "bss"
    if len(data_units) > 1:
        both = data_units & code_units
        return (next(iter(both)), "bss") if len(both) == 1 else (None, None)
    if len(code_units) == 1:
        return next(iter(code_units)), "code"
    return None, None


def data_first(owners):
    """A unit that owns a block by its .bss data owns no other block by code alone or by data in a gap: its code inside
    that other block is a header copy bound to it or a misplaced definition (Game's TGObjPtrEvent copies at 0x403290
    sat in the run before LoadMissionAction's, while Game's DATA lies in the run after Episode's), and its data before
    its own stores is no evidence for the block before (BoneStateController's NiRTTI at 0x9916E0, after BoneState's
    stores, with its guard among its own run's). Those blocks are left undecided."""
    by_data = {unit for unit, how in owners if how == "bss"}
    return [(None, None) if how in ("code", "gap") and unit in by_data else (unit, how) for unit, how in owners]


def fill_between(owners):
    """Undecided blocks between two blocks of one unit are that unit's: a unit is contiguous in code order."""
    out = list(owners)
    last = None
    for k, (unit, how) in enumerate(owners):
        if unit is None:
            continue
        if last is not None and owners[last][0] == unit:
            for m in range(last + 1, k):
                if out[m][0] is None:
                    out[m] = (unit, "between")
        last = k
    return out


def default_order(start, before, before_end, after, tight=0x100):
    """The units to try for a block nothing decides. A unit's stubs follow its code (73% of the data-decided blocks
    with no code inside; 85% when the block starts within 0x100 bytes of the code before it), so the unit of the
    code right before comes first; after unannotated code, which is as often the next unit's, the next unit does."""
    if before is not None and start - before_end <= tight:
        return [(before, "trailing"), (after, "leading")]
    return [(after, "leading"), (before, "trailing")]


def registration_lines(text):
    """The unit's `_$E` annotations: address -> number."""
    return {int(m.group(1), 16): int(m.group(2)) for m in re.finditer(
        r"//\s*FUNCTION:\s*STBC\s+0x([0-9A-Fa-f]+)\s+_\$E(\d+)", text)}


def drop_lines(text, addrs):
    """The text without the `_$E` annotation lines of addrs, and without the registration block's heading when no
    annotation is left under it."""
    nl = "\r\n" if "\r\n" in text else "\n"
    line_re = re.compile(r"//\s*FUNCTION:\s*STBC\s+0x([0-9A-Fa-f]+)\s+_\$E\d+\s*$")
    out = []
    for line in text.split(nl):
        m = line_re.match(line)
        if not (m and int(m.group(1), 16) in addrs):
            out.append(line)
    if REGISTRATION_BLOCK in out:
        i = out.index(REGISTRATION_BLOCK)
        if not (i + 1 < len(out) and out[i + 1].startswith("// FUNCTION: STBC 0x")):
            del out[i]
            if i and not out[i - 1].strip() and i < len(out) and not out[i].strip():
                del out[i - 1]
    return nl.join(out)


def stale_units(rows, registration_addrs, units):
    """The units among `units` with a registration annotation (at one of registration_addrs) whose report row isn't
    MATCH: their numbers shifted, and `renumber` settles them before anything is placed around them."""
    return sorted({r["unit"] for r in rows if r.get("static") and r.get("status") != "MATCH"
                   and int(r["addr"], 16) in registration_addrs and r["unit"] in units})


def place_all(write, evict=False, scope=None):
    """Every unit's NetImmerse registrations, placed by their owner (see `blocks`, `block_owner`).

    `scope`, a set of units, limits what changes to those units: the evidence is read from every unit, but a stub
    moves only between units of the scope and only they are written; every other unit's annotations stay as they are
    and keep their addresses. A unit with a `_$E` annotation that no longer matches (its numbers shifted: run
    `renumber` first) is left out of the scope too, since its stale numbers would block the right ones.

    Each block of bodies goes to the one unit with .bss data among its stores, else the one unit with code among its
    stubs, else the unit whose blocks surround it. The evidence moves an annotated body, with its `jmp` stubs, when the
    numbers it needs in its owner are free (or freed by another move); an annotation it can't re-home stays, and with
    `evict` is dropped instead (a lost match for `regress --accept`). Bodies nobody holds then take the default order
    (default_order), for a unit that fits most of the block's free bodies. A unit takes a stub only when its object fits
    it (bytes and Register target), each initialiser once. Returns {address: (unit, number, how)}."""
    import bisect
    img = decomp.pe.Image(decomp.target_image())
    names = comdat.all_annotations()
    known = comdat.known_addresses(names)
    rows = decomp.load_json(decomp.BUILD / "report.json").get("functions", [])
    size = {int(r["addr"], 16): r.get("size") or 0 for r in rows}
    sources = decomp.source_files()
    entries = [e for unit, path in sources.items() for e in decomp.scan(unit, path)]
    objs = {}

    def load(unit):
        if unit not in objs:
            p = decomp.BUILD / "obj" / (unit + ".obj")
            objs[unit] = coff.Coff(p.read_bytes()) if p.exists() and unit_source(decomp.ROOT, unit) else None
        return objs[unit]

    # code positions: the unit's own functions, and header copies bound to it near them
    own = [(e.addr, e.unit) for e in entries if e.kind == "FUNCTION" and not e.bind
           and not (e.decorated or "").startswith("_$E")]
    bound = [(e.addr, e.bind) for e in entries + decomp.header_entries() if e.kind == "FUNCTION" and e.bind
             and not (e.decorated or "").startswith("_$E")]
    near = {}
    for addr, unit in own:
        near.setdefault(unit, []).append(addr)
    code = sorted(own + [(a, u) for a, u in bound if any(abs(a - b) < 0x8000 for b in near.get(u, ()))])
    code_keys = [a for a, _ in code]

    regs = registrations(img, img.text[1], img.text[1] + img.text[2])
    kind_of = dict(regs)
    bodies = [a for a, k in regs if k == "body"]
    stores = [int.from_bytes(img.read(a + 6, 4), "little") for a in bodies]
    evidence = bss_evidence(img, rows, entries, load, set(kind_of))
    points = sorted((a, u) for u, addrs in evidence.items() for a in addrs)

    # what is annotated now: registration lines (re-placed) and every other `_$E` (kept, its number taken)
    texts, old, used = {}, {}, {}
    for unit in sources:
        path = unit_source(decomp.ROOT, unit)
        if path is None:
            continue
        text = texts[unit] = path.read_bytes().decode("latin-1")
        for addr, n in registration_lines(text).items():
            if addr in kind_of:
                old[addr] = (unit, n)
            else:
                used.setdefault(unit, set()).add(n)

    # a unit whose registration annotations no longer all match has shifted numbers: `renumber` settles those, and
    # placing around them adds stubs under the wrong numbers (TGTimerManager, shifted by 6, went from 140 annotations
    # to 174, none right), so it stays as it is
    stale = stale_units(rows, kind_of, texts)
    scope = set(texts if scope is None else scope) - set(stale)
    for unit in stale:
        print("left %s: its registration annotations don't all match; run `statics.py renumber` (after a build or match) "
              "first" % unit)

    # every number a unit's `_$E` annotations hold: (unit, number) -> address (None for the unit's other
    # initialisers), and address -> (unit, number, how) for the registrations, starting from what is annotated
    placed = {a: (u, n, "kept") for a, (u, n) in old.items()}
    holder = {(u, n): None for u, ns in used.items() for n in ns}
    holder.update({(u, n): a for a, (u, n) in old.items()})
    jmps_of = {}
    for addr, kind in regs:
        if kind == "jmp":
            jmps_of.setdefault(jmp_target(img, addr), []).append(addr)

    def plan(unit, body):
        """The numbers unit's object gives a body and the `jmp` stubs that enter it: [(address, number)], or None."""
        obj = load(unit) if unit in scope else None
        c = candidates(obj, img, body, known) if obj is not None else []
        if len(c) != 1:
            return None
        enter = [s for s, b in jmp_stubs(obj).items() if b == c[0]]
        steps = [(body, c[0])]
        for j in jmps_of.get(body, []):
            if len(enter) != 1:
                return None
            steps.append((j, enter[0]))
        return steps

    def free(unit, steps):
        """The numbers are free in unit, and no stub of steps belongs to a unit outside the scope."""
        return all(holder.get((unit, n), a) == a and placed.get(a, (unit,))[0] in scope for a, n in steps)

    evicted = []

    def move(unit, steps, how):
        for a, n in steps:
            if a in placed:
                holder.pop(placed[a][:2], None)
            other = holder.get((unit, n))
            if other is not None and other != a:
                evicted.append((other,) + placed.pop(other)[:2])
            holder[(unit, n)] = a
            placed[a] = (unit, n, how)

    cut = blocks(stores)
    owners = []
    for i, j in cut:
        inside_code = units_between(code, bodies[i], bodies[j - 1])
        inside_data = units_between(points, min(stores[i:j]), max(stores[i:j]))
        owner = block_owner(inside_code, inside_data)
        if owner[0] is None and not inside_data and j < len(stores):
            # a unit's own statics follow its registration stores: data between the block's last store and the
            # next block's first is the block's (122 of 122 blocks decided by their data agree; FloatRangeWatcher's
            # RID_INVALID at 0x98039c)
            gap = units_between(points, max(stores[i:j]) + 1, min(stores[j:]) - 1)
            # unless that unit registers nothing there: a unit without registrations of its own has no block, and
            # its data follows the previous unit's stores (MSJExceptionHandler's after 0x716130's)
            unit = next(iter(gap)) if len(gap) == 1 else None
            obj = unit and load(unit)
            if obj is not None and any(candidates(obj, img, b, known) for b in bodies[i:j]):
                owner = (unit, "gap")
        owners.append(owner)
    owners = fill_between(data_first(owners))

    # the evidence moves a body (and its stubs) when the numbers it needs are free, or freed by another move
    pending = []
    for (i, j), (unit, how) in zip(cut, owners):
        for b in bodies[i:j] if unit else ():
            steps = plan(unit, b)
            if steps is None:
                continue
            if placed.get(b, (None,))[0] == unit and all(placed.get(a, (None, None))[:2] == (unit, n)
                                                         for a, n in steps):
                for a, n in steps:
                    placed[a] = (unit, n, how)
                continue
            pending.append((b, unit, how, steps))
    progress = True
    while progress and pending:
        progress, rest = False, []
        for b, unit, how, steps in pending:
            if free(unit, steps):
                move(unit, steps, how)
                progress = True
            else:
                rest.append((b, unit, how, steps))
        pending = rest
    blocked = pending
    if evict:
        for b, unit, how, steps in blocked:
            if all(placed.get(a, (unit,))[0] in scope for a, _ in steps):
                move(unit, steps, how)
        blocked = [x for x in blocked if not all(placed.get(a, (None,))[0] == x[1] for a, _ in x[3])]

    # what nobody holds yet: the default order, for a unit that takes most of the block's free bodies
    for (i, j), (unit, how) in zip(cut, owners):
        if unit is not None:
            continue
        k = bisect.bisect_left(code_keys, bodies[i])
        before = code[k - 1] if k else (None, None)
        k = bisect.bisect_right(code_keys, bodies[j - 1])
        after = code[k] if k < len(code) else (None, None)
        end = before[0] + size.get(before[0], 0) if before[0] is not None else 0
        loose = [a for a in bodies[i:j] if a not in placed]
        for unit, how in default_order(bodies[i], before[1], end, after[1]):
            ok = [(a, s) for a, s in ((a, plan(unit, a)) for a in loose) if s and free(unit, s)]
            if unit and loose and 2 * len(ok) >= len(loose):
                for a, s in ok:
                    if free(unit, s):
                        move(unit, s, how)
                break

    # a stub whose body is placed and that nobody holds: the unit's stub entering it; a stub left apart from its
    # body (an eviction) goes
    for addr, kind in regs:
        if kind != "jmp":
            continue
        t = jmp_target(img, addr)
        if t in placed:
            unit, body, how = placed[t]
            enter = [s for s, b in jmp_stubs(load(unit)).items() if b == body]
            if addr not in placed and unit in scope and len(enter) == 1 and holder.get((unit, enter[0])) is None \
                    and (unit, enter[0]) not in holder:
                holder[(unit, enter[0])] = addr
                placed[addr] = (unit, enter[0], how)
        if addr in placed and placed[addr][0] in scope and (t not in placed or placed[t][0] != placed[addr][0]
                               or jmp_stubs(load(placed[addr][0])).get(placed[addr][1]) != placed[t][1]):
            evicted.append((addr,) + placed.pop(addr)[:2])
            holder.pop(evicted[-1][1:], None)

    # report and write
    by_unit = {}
    for addr, (unit, n, how) in placed.items():
        by_unit.setdefault(unit, []).append((addr, n, how))
    hows = {}
    for _, _, how in placed.values():
        hows[how] = hows.get(how, 0) + 1
    moved = sum(1 for a, v in placed.items() if a in old and old[a] != v[:2])
    new = [a for a in placed if a not in old]
    dropped = [a for a in old if a not in placed]
    for a, unit, n in sorted(evicted):
        print("dropped 0x%X %s _$E%d" % (a, unit, n))
    for unit in sorted(set(by_unit) | {u for u, _ in old.values()}):
        if unit not in scope:
            continue
        mine = sorted(by_unit.get(unit, []))
        before = {a: n for a, (u, n) in old.items() if u == unit}
        after = {a: n for a, n, _ in mine}
        if before == after:
            continue
        print("%-45s %4d -> %4d" % (unit, len(before), len(after)))
        if write:
            path = unit_source(decomp.ROOT, unit)
            text = drop_lines(texts[unit], set(before))
            path.write_bytes(text.encode("latin-1"))
            if mine:
                write_block(path, ["// FUNCTION: STBC 0x%08x _$E%d" % (a, n) for a, n, _ in mine])
    print("registration stubs in retail: %d (%d blocks); annotated before: %d, after: %d (%s); moved: %d, new: %d, "
          "dropped: %d; evidence placements a kept annotation blocks: %d" % (
              len(regs), len(cut), len(old), len(placed), ", ".join("%s %d" % kv for kv in sorted(hows.items())),
              moved, len(new), len(dropped), len(blocked)))
    return placed


def unit_name(arg):
    return re.sub(r"\.(c|cpp)$", "", re.sub(r"^src/", "", arg.replace("\\", "/")))


def changed_units(root, base="main"):
    """The units whose .c/.cpp source differs from `base`'s merge base: committed on the branch, or in the working
    tree."""
    import subprocess

    def git(*args):
        p = subprocess.run(["git", *args], cwd=str(root), capture_output=True, text=True, errors="replace")
        if p.returncode:
            sys.exit("git %s: %s" % (" ".join(args), p.stderr.strip()))
        return p.stdout
    fork = git("merge-base", base, "HEAD").strip()
    names = git("diff", "--name-only", fork).splitlines() + [
        l[3:].split(" -> ")[-1] for l in git("status", "--porcelain").splitlines()]
    return units_of(names)


def units_of(paths):
    """The units of the source paths among paths (repository-relative)."""
    out = set()
    for p in paths:
        p = p.strip().strip('"').replace("\\", "/")
        if p.startswith("src/") and re.search(r"\.(c|cpp)$", p):
            out.add(unit_name(p))
    return out


def place_all_args(argv):
    """(write, evict, scope) from place-all's arguments: `--units U...` and `--changed [BASE]` limit the scope."""
    write, evict, scope = "--write" in argv, "--evict" in argv, None
    rest = [a for a in argv[1:] if a not in ("--write", "--evict")]
    k = 0
    while k < len(rest):
        if rest[k] == "--units":
            k += 1
            scope = set() if scope is None else scope
            while k < len(rest) and not rest[k].startswith("--"):
                scope.add(unit_name(rest[k]))
                k += 1
        elif rest[k] == "--changed":
            base = "main"
            if k + 1 < len(rest) and not rest[k + 1].startswith("--"):
                base = rest[k + 1]
                k += 1
            k += 1
            scope = (set() if scope is None else scope) | changed_units(decomp.ROOT, base)
        else:
            sys.exit("unknown place-all argument %s\n\n%s" % (rest[k], __doc__))
    return write, evict, scope


def main(argv):
    if argv and argv[0] == "place-all":
        write, evict, scope = place_all_args(argv)
        if scope is not None:
            print("scope: %s" % (", ".join(sorted(scope)) or "no units"))
        place_all(write, evict, scope)
        return 0
    if argv and argv[0] == "place" and len(argv) >= 4:
        unit = unit_name(argv[1])
        placed, left = place(unit, int(argv[2], 16), int(argv[3], 16), "--write" in argv)
        print("placed: %d, left: %d" % (placed, left))
        return 0
    if not argv or argv[0] != "renumber":
        sys.exit(__doc__)
    changes, problems = renumber(argv[1] if len(argv) > 1 else ".")
    for c in changes:
        print("renumbered " + c)
    for p in problems:
        print("LEFT " + p)
    print("renumbered: %d, left: %d" % (len(changes), len(problems)))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
