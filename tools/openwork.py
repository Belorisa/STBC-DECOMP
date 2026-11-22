"""What is open, per class or per retail address range, from the last build.

    python tools/openwork.py class ShipClass           # one class: matched, parked (reason), pending, claimed, unannotated
    python tools/openwork.py range 0x78F1E0 0x790CA0   # a retail range: annotated functions and the gaps between them
    python tools/openwork.py classes                   # every class seen in annotations or symbols.txt, with counts
    python tools/openwork.py phases                    # the campaign's three phases: unwritten bytes, drafts, walls
    python tools/openwork.py gaps [N]                  # phase 1 map: unwritten gap bytes per class before the gap
    python tools/openwork.py drafts [MIN_SCORE]        # the phase 2 queue: drafts and open functions, with card tries
    python tools/openwork.py parked [MIN_SCORE]        # parked and DIFF functions grouped by their PARKED reason
    python tools/openwork.py walls [WALL [MIN_SCORE]]  # open functions by wall class (inline, control-flow, register,
                                                       # referent, structure): the table, or one class's functions
    python tools/openwork.py groups [ID]               # shared-wall groups from the cards: functions, scores, wall,
                                                       # last try (cards.py add --group, cards.py tag)
    python tools/openwork.py headers [HEADER]          # header leads from the cards: per header, the tries that changed
                                                       # it and those that only pointed at it (off-limits, not tried)
    python tools/openwork.py untried [LO HI] [MIN] [--ignore sonnet,merge_lane] [--since 2026-11-05]
                                                       # open functions no strong try has reached yet
    python tools/openwork.py ready [Class]             # claimed functions whose callees all have an identity (ready to
                                                       # write) versus those calling unidentified code (blocked)
    python tools/openwork.py triage [Class|all]        # who takes each open function first: a Sonnet
                                                       # worker or Opus (by score, PARKED note, readiness)
    python tools/openwork.py advisory [RULE] [UNIT|Class]
                                                       # the readability backlog: advisory findings by rule; one rule
                                                       # per unit; a unit's or a class's findings themselves
    python tools/openwork.py finish [N]                # units whose functions all match, ranked by advisory findings:
                                                       # what a finishing lane cleans without touching open work
    python tools/openwork.py thunks                   # check every IMPORT-THUNK row against retail: `FF 25 <slot>`,
                                                       # the slot the import the row names

Reads build/report.json (run `decomp.py build` first), config/symbols.txt and config/exceptions.md. "claimed" means the
function has an identity in symbols.txt but no source yet. The gaps of `range` are retail addresses between annotated
functions that no annotation covers; Ghidra's function list says what lives there. A routine config/exceptions.md lists
as ORIGINAL-ASM without an annotation (a CRT routine retail took from VC6's assembly sources) is neither claimed work
nor unwritten bytes: `range` shows it as `asm`, and `phases` counts it on a line of its own. A function under a
`// PATCHED:` note (binary-patched in retail, written as the original; the build gives it status PATCHED) is likewise
neither matched nor open: `class` and `range` show it as `patched`, `phases` counts it on a line of its own, and the
queues (`drafts`, `walls`, `parked`, `triage`) leave it out. A 6-byte `jmp [__imp_X]` stub config/exceptions.md lists as
IMPORT-THUNK (supplied by an import library, not written) is handled like an ORIGINAL-ASM routine: `range` shows it as
`thunk`, `phases` counts it on a line of its own, and `ready`, `classes` and `triage` don't offer it as claimed work.
A block of a library object's data linked into .text (dinput.lib's DIOBJECTDATAFORMAT tables) that config/exceptions.md
lists as LIBRARY-DATA, its size the row's "<N> bytes", is handled the same way: `range` shows it as `data`.

A function whose card's last wall is `compiler-state` (a worker proved the wall bounded: several spellings A/B'd,
retail's own sites contradicting each other) stays open in every total; `walls`, `drafts`, `parked` and `triage` list
it apart (triage's tier "state"), and `phases` counts it on a line of its own.

The gaps after a function that doesn't match start at retail's end of it, scanned as `ready` scans a claimed body,
not at our compiled end; where the scan finds no end, `range` says the gap may start with that function's tail and
`gaps` counts such gaps on a line of their own.
"""

import bisect
import json
import re
import sys
from collections import Counter, defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import decomp  # noqa: E402


def load():
    rep = decomp.load_report()
    claims = []
    for line in (decomp.CONFIG / "symbols.txt").read_text(encoding="utf-8").splitlines():
        body = line.split("#")[0].split()
        if len(body) >= 2:
            claims.append((body[0], int(body[1], 16), line.split("#", 1)[1].strip() if "#" in line else ""))
    return rep["functions"], claims


def original_asm(text=None, annotated=()):
    """{address: (name, bytes)} of the routines config/exceptions.md lists as ORIGINAL-ASM with no annotation: the
    CRT's assembly routines, counted neither matched nor open. A row's byte count is the last "<N> bytes" of its
    deviation (0 when it gives none). Annotated rows (C with an inline __asm block) are the report's, not these."""
    return decomp.listed_apart("ORIGINAL-ASM", text, annotated)


def import_thunks(text=None, annotated=()):
    """{address: (name, bytes)} of the 6-byte `jmp [__imp_X]` stubs config/exceptions.md lists as IMPORT-THUNK: code
    the import libraries supply, counted neither matched nor open."""
    return decomp.listed_apart("IMPORT-THUNK", text, annotated)


def library_data(text=None, annotated=()):
    """{address: (name, bytes)} of the library objects' data linked into .text that config/exceptions.md lists as
    LIBRARY-DATA (dinput.lib's DIOBJECTDATAFORMAT tables): not source, counted neither matched nor open."""
    return decomp.listed_apart("LIBRARY-DATA", text, annotated)


def apart(annotated=()):
    """{address: (kind, name, bytes)} of everything listed apart without source: `asm`, `thunk` and `data`."""
    out = {a: ("asm",) + v for a, v in original_asm(annotated=annotated).items()}
    out.update((a, ("thunk",) + v) for a, v in import_thunks(annotated=annotated).items())
    out.update((a, ("data",) + v) for a, v in library_data(annotated=annotated).items())
    return out


def class_of_decorated(name):
    # special members (??0 ctor, ??1 dtor, ??_G/??_E deleting dtors, ??4 operators): the class follows the code
    m = re.match(r"\?\?(?:_[0-9A-Z]|[0-9A-Z])(\w+)@@", name) or re.match(r"\?\w+@(\w+)@@", name)
    return m.group(1) if m else None


def class_of_row(row):
    m = re.match(r"(\w+)::", row["name"] or "")
    if m:
        return m.group(1)
    return class_of_decorated(row.get("symbol") or "")


def status_of(row):
    if row["status"] == "PATCHED":
        return "patched"   # binary-patched in retail, written as the original: listed apart, neither matched nor open
    if row["status"] == "MATCH":
        return "pending" if any(r["tier"] == "match" for r in row.get("rules", [])) else "matched"
    if row["status"] in ("NO-OBJ", "NO-SYMBOL"):
        return "broken"
    if row.get("parked"):
        return "parked"
    return "draft" if row.get("draft") else "diff"


def cmd_class(cls, rows, claims):
    mine = [r for r in rows if class_of_row(r) == cls]
    annotated = {int(r["addr"], 16) for r in mine} | set(apart())
    print("%s: %d annotated" % (cls, len(mine)))
    for state in ("matched", "pending", "draft", "parked", "diff", "broken", "patched"):
        sel = sorted((r for r in mine if status_of(r) == state), key=lambda r: int(r["addr"], 16))
        if not sel:
            continue
        print("  %s (%d)" % (state, len(sel)))
        for r in sel:
            extra = ""
            if state in ("parked", "draft", "patched"):
                extra = "  " + (r.get(state) or "")[:90]
            elif state == "pending":
                extra = "  " + "; ".join(x["text"][:60] for x in r.get("rules", []) if x["tier"] == "match")
            if state == "matched":
                print("    %s %s" % (r["addr"], r["name"]))
            else:
                print("    %s %5.1f %s%s" % (r["addr"], r["score"], r["name"], extra))
    claimed = [(n, a, e) for n, a, e in claims if class_of_decorated(n) == cls and a not in annotated]
    if claimed:
        print("  claimed in symbols.txt, no source (%d)" % len(claimed))
        for n, a, e in sorted(claimed, key=lambda c: c[1]):
            print("    0x%X %s  # %s" % (a, n, e[:70]))


EXACT = ("MATCH", "REFERENT", "WRONG-REFERENT")   # statuses whose compiled size is retail's


TABLE_REF = re.compile(r"\[e[a-z]{2}\*4 \+ (0x[0-9a-f]+)\]|byte ptr \[e[a-z]{2} \+ (0x[0-9a-f]+)\]")


def switch_tables(img, start, size, stop):
    """True when the retail body at start..start+size reads, as `[reg*4 + T]` (a jump table) or `byte ptr [reg + T]`
    (its index table), an address T in start+size..stop: the bytes after the last return are the function's own
    switch tables and padding, not a gap."""
    code = img.read(start, size)
    for _, _, ops in decomp.disasm(code, start):
        for m in TABLE_REF.finditer(ops):
            if start + size <= int(m.group(1) or m.group(2), 16) < stop:
                return True
    return False


def retail_extents(rows, img, others=()):
    """{address: retail's size, or None when the scan finds no end} of every annotated function whose compiled size
    may differ from retail's (anything but an exact status). Retail's body is scanned as `ready` scans a claimed
    function (callees()), up to the next annotated or listed address; `others` adds addresses to stop at."""
    starts = sorted({int(r["addr"], 16) for r in rows} | set(others))
    out = {}
    for r in rows:
        if r["status"] in EXACT or r["status"] == "PATCHED":
            continue
        a = int(r["addr"], 16)
        i = bisect.bisect_right(starts, a)
        stop = starts[i] if i < len(starts) else a + SCAN_CAP
        try:
            _, size = callees(img, a, stop)
        except Exception:
            size = None
        out[a] = size if size and a + size < stop else None
        if out[a] and switch_tables(img, a, size, stop):
            out[a] = stop - a   # the switch tables and padding after the last return belong to the function
    return out


def end_of(r, extents):
    """(where the gap after an annotated row may start, the row's name when that is only its compiled end): retail's
    extent when known, else the compiled size, which may stop short of retail's (the gap then begins with its tail)."""
    a = int(r["addr"], 16)
    size = r.get("size", 0)
    tail = None
    if a in extents:
        if extents[a] is None:
            tail = r["name"] or r["addr"]
        else:
            size = max(size, extents[a])
    cursor = a + max(size, 16)
    return cursor + (-cursor) % 16, tail


def cmd_range(lo, hi, rows, claims):
    img = decomp.pe.Image(decomp.target_image())
    inside = sorted((r for r in rows if lo <= int(r["addr"], 16) < hi), key=lambda r: int(r["addr"], 16))
    asm = {a: v for a, v in apart(annotated={int(r["addr"], 16) for r in rows}).items() if lo <= a < hi}
    claimed = {a: n for n, a, _ in claims if lo <= a < hi and a not in asm}
    print("0x%X-0x%X: %d annotated (%d PATCHED), %d claimed without source, %d ORIGINAL-ASM, %d IMPORT-THUNK%s" % (
        lo, hi, len(inside), sum(1 for r in inside if status_of(r) == "patched"),
        len(set(claimed) - {int(r["addr"], 16) for r in inside}),
        sum(1 for v in asm.values() if v[0] == "asm"), sum(1 for v in asm.values() if v[0] == "thunk"),
        "".join(", %d LIBRARY-DATA" % n for n in [sum(1 for v in asm.values() if v[0] == "data")] if n)))
    entries = [(int(r["addr"], 16), r) for r in inside] + [(a, None) for a in asm]
    extents = retail_extents(inside, img, set(asm) | {hi})
    cursor, tail = lo, None
    for a, r in sorted(entries, key=lambda e: e[0]):
        if a - cursor >= 0x40:
            print("  gap   0x%X-0x%X (%d bytes)%s%s" % (cursor, a, a - cursor, "".join(
                "  claimed 0x%X %s" % (c, n[:50]) for c, n in sorted(claimed.items()) if cursor <= c < a),
                "  starts with the tail of %s (retail's extent unknown)" % tail if tail else ""))
        if r is None:
            # an assembly routine, an import thunk or library data: listed, not written; the next one may follow it
            # unaligned
            kind, name, size = asm[a]
            print("  %-7s %5s 0x%X %s (%d bytes)" % (kind, "", a, name, size))
            cursor = max(cursor, a + size)
            continue
        note = r.get("patched") or r.get("parked")
        print("  %-7s %5.1f 0x%X %s%s" % (status_of(r), r["score"], a, r["name"], ("  " + note[:70]) if note else ""))
        cursor, tail = end_of(r, extents)
    if hi - cursor >= 0x40:
        print("  gap   0x%X-0x%X (%d bytes)%s" % (cursor, hi, hi - cursor,
                                                 "  starts with the tail of %s (retail's extent unknown)" % tail
                                                 if tail else ""))


def gaps_of(rows, extents=None):
    """The unwritten gaps between annotated functions (as `range` prints them): (start, end, the annotated row before
    it, that row's name when the gap may begin with its tail). A non-exact function's gap starts at retail's extent
    (`retail_extents`), or at its compiled end when that is unknown."""
    ordered = sorted(rows, key=lambda r: int(r["addr"], 16))
    out = []
    for prev, r in zip(ordered, ordered[1:]):
        cursor, tail = end_of(prev, extents or {})
        a = int(r["addr"], 16)
        if a - cursor >= 0x40:
            out.append((cursor, a, prev, tail))
    return out


def asm_rows(rows):
    """The ORIGINAL-ASM routines, IMPORT-THUNK stubs and LIBRARY-DATA blocks not annotated in `rows`, as rows: they
    bound gaps."""
    annotated = {int(r["addr"], 16) for r in rows}
    return [{"addr": "0x%X" % a, "name": n, "size": size, "asm": True, "status": "MATCH"}
            for a, (_, n, size) in apart(annotated=annotated).items()]


def unwritten_gaps(rows, extents=None):
    """(gaps, gaps that may start with a non-exact function's tail, the asm rows, the extents): the bytes no source
    covers, as `gaps` and `range` print them. `extents` defaults to retail_extents() over the retail image."""
    asm = asm_rows(rows)
    if extents is None:
        img = decomp.pe.Image(decomp.target_image())
        extents = retail_extents(rows, img, {int(r["addr"], 16) for r in asm})
    gaps = gaps_of(rows + asm, extents)
    return [g for g in gaps if not g[3]], [g for g in gaps if g[3]], asm, extents


def cmd_gaps(limit, rows):
    """Unwritten gap bytes per class, by the nearest preceding annotated function that names a class: a phase 1 map."""
    last_cls = None
    by, count, largest = Counter(), Counter(), {}
    gaps, tails, asm, _ = unwritten_gaps(rows)
    owner = {}
    for r in sorted(rows + asm, key=lambda r: int(r["addr"], 16)):
        c = None if r.get("asm") else class_of_row(r)
        if c:
            last_cls = c
        owner[r["addr"]] = last_cls
    for lo, hi, prev, _ in gaps:
        c = owner[prev["addr"]] or "(none)"
        by[c] += hi - lo
        count[c] += 1
        if hi - lo > largest.get(c, (0, 0))[0]:
            largest[c] = (hi - lo, lo)
    print("%d gaps, %d bytes between annotated functions" % (len(gaps), sum(by.values())))
    if tails:
        print("and %d gaps, %d bytes, after a non-exact function whose retail extent the scan can't find: they may "
              "start with its tail (`range` marks them)" % (len(tails), sum(hi - lo for lo, hi, _, _ in tails)))
    print("%8s %5s %8s %-10s %s" % ("bytes", "gaps", "largest", "at", "class before the gap"))
    for c, b in by.most_common(limit):
        print("%8d %5d %8d 0x%-8X %s" % (b, count[c], largest[c][0], largest[c][1], c))


def cmd_classes(rows, claims):
    table = defaultdict(lambda: defaultdict(int))
    for r in rows:
        c = class_of_row(r)
        if c:
            table[c][status_of(r)] += 1
    annotated = {int(r["addr"], 16) for r in rows}
    annotated |= set(apart(annotated=annotated))
    for n, a, _ in claims:
        c = class_of_decorated(n)
        if c and a not in annotated:
            table[c]["claimed"] += 1
    def left(t):
        return t["parked"] + t["draft"] + t["diff"] + t["claimed"] + t["pending"]
    print("%-32s %7s %7s %5s %6s %5s %7s" % ("class", "matched", "pending", "draft", "parked", "diff", "claimed"))
    for c in sorted(table, key=lambda c: -left(table[c])):
        t = table[c]
        if left(t) == 0:
            continue
        print("%-32s %7d %7d %5d %6d %5d %7d" % (c, t["matched"], t["pending"], t["draft"], t["parked"], t["diff"],
                                                 t["claimed"]))


def cmd_phases(rows, claims, summary, extents=None):
    """Where the campaign stands in its three phases (orchestrator skill, "Phases"). Phase 1's unwritten bytes are the
    gaps between annotated functions, as `gaps` lists them (padding and runs under 0x40 bytes aside); a written function that
    isn't exact is counted apart, at retail's extent where the scan finds it. `extents`: as unwritten_gaps()."""
    sys.path.insert(0, str(Path(__file__).resolve().parent))
    import cards
    states = Counter(status_of(r) for r in rows)
    annotated = {int(r["addr"], 16) for r in rows}
    asm = original_asm(annotated=annotated)
    thunks = import_thunks(annotated=annotated)
    library = library_data(annotated=annotated)
    text = decomp.pe.Image(decomp.target_image()).text   # (name, va, vsize, ...): symbols.txt also claims data
    claimed = sum(1 for _, a, _ in claims if a not in annotated and a not in asm and a not in thunks
                  and text[1] <= a < text[1] + text[2])
    asm_bytes = sum(size for _, size in asm.values())
    thunk_bytes = sum(size for _, size in thunks.values())
    patched = [r for r in rows if status_of(r) == "patched"]
    patched_bytes = sum(r.get("size", 0) for r in patched)   # the original's size: retail's patched span
    text_bytes = max(summary.get("text_bytes", 1), 1)
    gaps, tails, _, extents = unwritten_gaps(rows, extents)
    unwritten = sum(hi - lo for lo, hi, _, _ in gaps)
    tail_bytes = sum(hi - lo for lo, hi, _, _ in tails)
    print("phase 1, draft the code without source: %d bytes of .text unwritten (%.1f%%) in %d gaps between annotated "
          "functions; %d functions claimed without source" % (unwritten, 100.0 * unwritten / text_bytes, len(gaps),
                                                               claimed))
    if tails:
        print("  and %d bytes in %d gaps after a non-exact function whose retail extent is unknown (they may start "
              "with its tail)" % (tail_bytes, len(tails)))
    inexact = [r for r in rows if r["status"] not in EXACT and status_of(r) != "patched"]
    inexact_bytes = sum(max(r.get("size", 0), extents.get(int(r["addr"], 16)) or 0) for r in inexact)
    print("written but not exact: %d functions, %d bytes (%.1f%% of .text; retail's extent where known)" % (
        len(inexact), inexact_bytes, 100.0 * inexact_bytes / text_bytes))
    drafts = [r for r in rows if status_of(r) in ("draft", "diff")]
    bands = Counter(">=95" if r["score"] >= 95 else ">=80" if r["score"] >= 80 else "<80" for r in drafts)
    print("ORIGINAL-ASM listed apart: %d functions, %d bytes" % (len(asm), asm_bytes))
    print("IMPORT-THUNK listed apart (from the import libraries): %d stubs, %d bytes" % (len(thunks), thunk_bytes))
    if library:
        print("LIBRARY-DATA listed apart (library data in .text): %d blocks, %d bytes" % (
            len(library), sum(size for _, size in library.values())))
    print("PATCHED listed apart (binary-patched in retail, written as the original): %d functions, %d bytes" % (
        len(patched), patched_bytes))
    print("phase 2, refine drafts toward 100%%: %d drafts, %d open without a note (>=95: %d, >=80: %d, <80: %d)"
          % (states["draft"], states["diff"], bands[">=95"], bands[">=80"], bands["<80"]))
    walls = [r for r in rows if status_of(r) == "parked"]
    noted, read = Counter(), Counter()
    for r in walls:
        wall, by = wall_of(r)
        (noted if by == "card" else read)[wall] += 1
    print("phase 3, walls: %d parked (a worker's verdict on the card: %s; read from the diff: %s), %d pending a rule, "
          "%d broken" % (len(walls), ", ".join("%s %d" % kv for kv in noted.most_common()) or "none",
                         ", ".join("%s %d" % kv for kv in read.most_common()) or "none", states["pending"],
                         states["broken"]))
    open_rows = [r for r in rows if status_of(r) in ("draft", "diff", "parked")]
    proven = [r for r in open_rows if bounded(r)]
    print("compiler-state, proven bounded (a worker's verdict on the card; still open, in the counts above): %d "
          "functions" % len(proven))
    statics = [r for r in rows if r.get("static")]
    print("matched: %d written functions; compiler-generated _$E statics counted apart: %d of %d" % (
        sum(1 for r in rows if status_of(r) == "matched" and not r.get("static")),
        sum(1 for r in statics if r["status"] == "MATCH"), len(statics)))


def cmd_drafts(min_score, rows):
    """Drafts and open functions by score, with their tries so far: the phase 2 queue."""
    sys.path.insert(0, str(Path(__file__).resolve().parent))
    import cards
    sel = sorted((r for r in rows if status_of(r) in ("draft", "diff") and r["score"] >= min_score),
                 key=lambda r: -r["score"])
    proven = [r for r in sel if bounded(r)]
    for part in (r for r in sel if not bounded(r)), proven:
        if part is proven and proven:
            print("compiler-state, proven bounded (still open, listed apart):")
        for r in part:
            tries = sum(1 for e in cards.read(r["addr"]) if e["kind"] == "try")
            print("  %5.1f %s %-6s tries %d  %s  %s" % (r["score"], r["addr"], status_of(r), tries, r["name"],
                                                        (r.get("draft") or "")[:70]))
    print("%d functions%s" % (len(sel), ", %d of them proven bounded" % len(proven) if proven else ""))


def wall_of(row):
    """(wall class, where it comes from) of an open function: a worker's verdict on its card ("card"), else the
    class the build read from the diff (tools/walls.py, "diff"), else ("undiagnosed", None)."""
    import cards
    noted = [e["wall"] for e in cards.read(row["addr"]) if e.get("wall")]
    if noted:
        return noted[-1], "card"
    if row.get("wall"):
        return row["wall"], "diff"
    return "undiagnosed", None


BOUNDED = "compiler-state"   # a worker's verdict: a wall proven bounded (cards.py add --wall compiler-state)


def bounded(row):
    """Whether a worker proved the function's wall bounded: several spellings A/B'd, retail's own sites contradicting
    each other. Such a function stays open in every total; the queues list it apart."""
    return wall_of(row) == (BOUNDED, "card")


def cmd_walls(wall, min_score, rows):
    """The open functions by wall class: what a phase 3 lane is planned from (one class, one lever family)."""
    import cards
    open_rows = [r for r in rows if r["status"] != "MATCH" and status_of(r) not in ("broken", "patched")
                 and r["score"] >= min_score]
    by_wall = defaultdict(list)
    for r in open_rows:
        by_wall[wall_of(r)[0]].append(r)
    if wall is None:
        print("%-13s %6s %6s %6s %6s   %5s %5s %5s" % ("wall", "all", "parked", "draft", "open", ">=95", ">=80", "<80"))
        for w in sorted(by_wall, key=lambda w: (w == BOUNDED, -len(by_wall[w]))):
            sel = by_wall[w]
            if w == BOUNDED:
                print("proven bounded (a worker's verdict; still open):")
            states = Counter(status_of(r) for r in sel)
            print("%-13s %6d %6d %6d %6d   %5d %5d %5d" % (
                w, len(sel), states["parked"], states["draft"], states["diff"],
                sum(1 for r in sel if r["score"] >= 95), sum(1 for r in sel if 80 <= r["score"] < 95),
                sum(1 for r in sel if r["score"] < 80)))
        return
    for r in sorted(by_wall.get(wall, []), key=lambda r: -r["score"]):
        _, by = wall_of(r)
        tries = sum(1 for e in cards.read(r["addr"]) if e.get("kind") == "try")
        print("%5.1f %s %-7s %-44s %s%s" % (
            r["score"], r["addr"], status_of(r), (r["name"] or "?")[:44],
            "tries %d; " % tries if tries else "",
            "worker's verdict on the card" if by == "card" else r.get("wall_note") or (r.get("problems") or [""])[0]))


def cmd_parked(min_score, rows):
    groups = defaultdict(list)
    proven = "(compiler-state, proven bounded: listed apart, still open)"
    for r in rows:
        if r["status"] == "DIFF" and r["score"] >= min_score:
            key = proven if bounded(r) else (r.get("parked") or "(no PARKED note)").split(":")[0][:60]
            groups[key].append(r)
    for key, sel in sorted(groups.items(), key=lambda kv: (kv[0] == proven, -len(kv[1]))):
        print("%d  %s" % (len(sel), key))
        for r in sorted(sel, key=lambda r: -r["score"])[:8]:
            print("    %5.1f %s %s" % (r["score"], r["addr"], r["name"]))


def last_try(addrs):
    """(address, entry) of the latest "try" among the cards of addrs, or (None, None)."""
    import cards
    best = (None, None)
    for a in addrs:
        for e in cards.read(a):
            if e.get("kind") == "try" and (best[1] is None or e["date"] >= best[1]["date"]):
                best = (a, e)
    return best


def cmd_groups(gid, rows, table=None):
    """Shared-wall groups (cards.py add --group, cards.py tag): the functions one fix would close or fail together."""
    import cards
    table = cards.groups() if table is None else table
    by_addr = {r["addr"]: r for r in rows}
    if gid is None:
        if not table:
            print("no groups yet: tag them with `cards.py add <addr> ... --group ID` or `cards.py tag`")
            return 0
        print("%-28s %5s %5s  %-28s %-14s %s" % ("group", "funcs", "open", "scores", "wall", "last try"))
        for g in sorted(table, key=lambda g: (-sum(1 for a in table[g] if is_open_row(by_addr.get(a))), g)):
            members = sorted(table[g], key=lambda a: -(by_addr.get(a) or {}).get("score", 0))
            open_ = [a for a in members if is_open_row(by_addr.get(a))]
            walls = Counter(wall_of(by_addr[a])[0] for a in open_)
            scores = " ".join("%.1f" % by_addr[a]["score"] if a in by_addr else "-" for a in members[:5])
            a, e = last_try(members)
            print("%-28s %5d %5d  %-28s %-14s %s" % (
                g[:28], len(members), len(open_), scores + (" ..." if len(members) > 5 else ""),
                walls.most_common(1)[0][0] if walls else "-",
                "%s %s %s: %s" % (e["date"], e.get("by") or "", a, e.get("how", "")[:60]) if e else "-"))
        return 0
    if gid not in table:
        print("no group %s (groups: %s)" % (gid, ", ".join(sorted(table)) or "none"))
        return 1
    members = table[gid]
    print("group %s: %d functions" % (gid, len(members)))
    for a in sorted(members, key=lambda a: -(by_addr.get(a) or {}).get("score", 0)):
        r = by_addr.get(a)
        if r is None:
            print("  %s (not in build/report.json)" % a)
            continue
        print("  %5.1f %-9s %-7s %-44s wall %s" % (r["score"], a, status_of(r), (r["name"] or "?")[:44],
                                                   wall_of(r)[0] if is_open_row(r) else "-"))
        for e in members[a]:
            print("        %s %s %s: %s" % (e["date"], e.get("by") or "", e["kind"], e.get("how", "")[:100]))
    a, e = last_try(members)
    if e:
        print("last try: %s %s %s: %s" % (e["date"], e.get("by") or "", a, e.get("how", "")))
    return 0


def cmd_headers(name, rows, table=None):
    """Header leads across the cards: for each header the cards name, the tries that changed or A/B'd it ("worked")
    and those that only pointed at it ("noted": off-limits, not tried), with the open functions that cite it. One
    function's card can't show that another lane already settled its header question, or that no lane ever took it."""
    import cards
    table = cards.header_tries() if table is None else table
    by_addr = {r["addr"]: r for r in rows}

    def open_of(entries):
        return {a for a, _ in entries if is_open_row(by_addr.get(a))}

    if name is None:
        if not table:
            print("no card names a header")
            return 0
        print("%-26s %5s %6s %6s %9s  %s" % ("header", "open", "worked", "noted", "unworked", "last worked try"))
        def unworked(h):
            return open_of(table[h]["noted"]) - open_of(table[h]["worked"])
        for h in sorted(table, key=lambda h: (-len(unworked(h)), -len(open_of(table[h]["worked"] + table[h]["noted"])), h)):
            t = table[h]
            cited = open_of(t["worked"] + t["noted"])
            if not cited:
                continue
            last = max(t["worked"], key=lambda ae: ae[1].get("date", ""), default=None)
            print("%-26s %5d %6d %6d %9d  %s" % (
                h[:26], len(cited), len({a for a, _ in t["worked"]}), len({a for a, _ in t["noted"]}),
                len(unworked(h)),
                "%s %s %s: %s" % (last[1].get("date", ""), last[1].get("lane") or last[1].get("by") or "", last[0],
                                  last[1]["how"][:70]) if last else "-"))
        print("open: open functions whose card names the header; worked / noted: functions with a try that changed it /"
              " only pointed at it; unworked: open functions with only noted tries (a lead no lane took). The split"
              " reads free text: check the entries (`openwork.py headers NAME`).")
        return 0
    matches = [h for h in table if h.lower() == name.lower()] or [h for h in table if name.lower() in h.lower()]
    if not matches:
        print("no card names %s" % name)
        return 1
    for h in sorted(matches):
        t = table[h]
        print("== %s" % h)
        for side, title in (("worked", "tries that changed or A/B'd it"), ("noted", "tries that only pointed at it")):
            print("%s (%d):" % (title, len(t[side])))
            for a, e in sorted(t[side], key=lambda ae: ae[1].get("date", "")):
                r = by_addr.get(a)
                print("  %s %-14s %s %5s %-7s %s" % (
                    e.get("date", ""), (e.get("lane") or e.get("by") or "")[:14], a,
                    "%.1f" % r["score"] if r else "-", status_of(r) if r else "-", e["how"]))
    return 0


def header_lines(addr, table=None):
    """The packet's lines for the headers addr's card names: what other cards record about each one, so a worker
    sees a settled or untaken header lead before diagnosing it again."""
    import cards
    table = cards.header_tries() if table is None else table
    mine = sorted(h for h, t in table.items() if any(a == addr for a, _ in t["worked"] + t["noted"]))
    out = []
    for h in mine:
        t = table[h]
        worked = [(a, e) for a, e in t["worked"] if a != addr]
        last = max(worked, key=lambda ae: ae[1].get("date", ""), default=None)
        out.append("%s: %d other cards with a try that changed or A/B'd it, %d that only pointed at it%s" % (
            h, len({a for a, _ in worked}), len({a for a, _ in t["noted"] if a != addr}),
            "; last: %s %s %s: %s" % (last[1].get("date", ""), last[1].get("lane") or "", last[0], last[1]["how"][:160])
            if last else "; no lane has changed it for these functions"))
        settled = [(a, e) for a, e in worked
                   if e.get("wall") == "compiler-state" or re.search(r"compiler[ -]state|A/B over", e["how"], re.I)]
        if settled:
            a, e = min(settled, key=lambda ae: ae[1].get("date", ""))
            out.append("    settled: %s %s %s: %s" % (e.get("date", ""), e.get("lane") or "", a, e["how"][:300]))
    return out


def is_open_row(row):
    return row is not None and status_of(row) in ("draft", "diff", "parked", "pending")


UNTRIED_SINCE = "2026-11-05"
UNTRIED_IGNORE = ("sonnet", "merge_lane")


def untried(rows, lo=0, hi=1 << 32, min_score=0.0, ignore=UNTRIED_IGNORE, since=UNTRIED_SINCE):
    """[(row, old tries)] of the open written functions no strong try has reached: no "try" on the card, or only
    tries by the authors in `ignore` or dated before `since`."""
    import cards
    out = []
    for r in rows:
        if r["status"] != "DIFF" or r.get("static") or status_of(r) == "patched":
            continue
        a = int(r["addr"], 16)
        if not lo <= a < hi or r["score"] < min_score:
            continue
        tries = [e for e in cards.read(r["addr"]) if e.get("kind") == "try"]
        if any(e.get("by") not in ignore and e.get("date", "") >= since for e in tries):
            continue
        out.append((r, len(tries)))
    return sorted(out, key=lambda t: (-t[0]["score"], t[0]["addr"]))


def cmd_untried(argv, rows):
    import argparse
    ap = argparse.ArgumentParser(prog="openwork.py untried")
    ap.add_argument("bounds", nargs="*", help="[LO HI] [MIN_SCORE]")
    ap.add_argument("--ignore", default=",".join(UNTRIED_IGNORE),
                    help="authors whose tries don't count (comma-separated; default %(default)s)")
    ap.add_argument("--since", default=UNTRIED_SINCE, help="tries before this date don't count (default %(default)s)")
    args = ap.parse_args(argv)
    b = args.bounds
    lo, hi = (int(b[0], 16), int(b[1], 16)) if len(b) >= 2 else (0, 1 << 32)
    min_score = float(b[2]) if len(b) >= 3 else float(b[0]) if len(b) == 1 else 0.0
    sel = untried(rows, lo, hi, min_score, tuple(x for x in args.ignore.split(",") if x), args.since)
    for r, n in sel:
        print("%5.1f %-9s %-6s %-60s old tries %d" % (r["score"], r["addr"], status_of(r), (r["name"] or "?")[:60], n))
    print("%d functions" % len(sel))


LARGE = 0x600     # bytes: a function bigger than this is flagged "large" by `ready`
# Out-of-line copies of small inlines (docs/levers/inline.md, "The inline budget"): a call to one is the inline budget, not
# unidentified code, so it doesn't block a function.
INLINE_COPIES = {0x414540: "NiPoint3::NiPoint3", 0x414560: "NiPoint3::operator=", 0x40CFE0: "NiObject::DecRefCount",
                 0x46C280: "NiColorA::NiColorA"}
SCAN_CAP = 0x4000  # bytes: the longest body `ready` will scan


def callees(img, start, stop):
    """Direct call targets (and tail-jump targets outside the body) of the retail function at start, scanning up to
    stop or to the first return followed by alignment padding. Returns (targets, body size)."""
    code = img.read(start, min(stop, start + SCAN_CAP) - start)
    if not code:
        return set(), 0   # outside the image's sections
    targets = set()
    end = start + len(code)
    reach = start   # the furthest address a jump inside the body has targeted so far
    insns = list(decomp.disasm(code, start))
    for i, (addr, mnem, ops) in enumerate(insns):
        nxt = insns[i + 1][0] if i + 1 < len(insns) else end
        direct = re.fullmatch(r"0x[0-9a-f]+", ops)
        if mnem in ("call", "jmp") and direct:
            t = int(ops, 16)
            if mnem == "call" or not start <= t < end:
                targets.add(t)
        if mnem.startswith("j") and direct and start <= int(ops, 16) < end:
            reach = max(reach, int(ops, 16))
        if mnem not in ("ret", "retn", "jmp"):
            continue
        # A return or an unconditional jump that no earlier jump reaches past can end the function. A return ends
        # it there, padding or not (the next function may start right after a return that ends on a 16-byte
        # boundary). A jump ends it when alignment padding follows: blocks placed after the last return often end
        # in a jump back into the body. A switch's table jumps are indirect and not seen; the padding test covers
        # those bodies.
        pad = code[nxt - start:(nxt + (-nxt) % 16) - start]
        padded = bool(pad) and all(b in (0x90, 0xCC) for b in pad)
        if nxt > reach and (mnem != "jmp" or padded):
            return targets, nxt - start
        if mnem != "jmp" and padded:
            return targets, nxt - start
    return targets, len(code)


def readiness(rows, claims):
    """(class, address, name, size, unknown callees) for every function claimed in symbols.txt with no source
    (claims outside .text, the import slots and data, are not functions to write)."""
    img = decomp.pe.Image(decomp.target_image())
    annotated = {int(r["addr"], 16) for r in rows}
    asm = set(apart(annotated=annotated))
    known = annotated | asm | {a for _, a, _ in claims} | set(INLINE_COPIES)
    starts = sorted(known)
    out = []
    for name, addr, _ in claims:
        if addr in annotated or addr in asm:
            continue   # written, or an assembly routine or import thunk listed apart
        section = img.section_of(addr)
        if section is None or section[0] != ".text":
            continue   # an import slot or a datum, not a function to write
        cls = class_of_decorated(name)
        if not cls:
            # SWIG wrappers and C functions have no class: group them so the tables read
            cls = "(AppWrap)" if name.startswith("__wrap_") else "(C)" if not name.startswith("?") else None
        i = bisect.bisect_right(starts, addr)
        stop = starts[i] if i < len(starts) else addr + SCAN_CAP
        try:
            targets, size = callees(img, addr, stop)
        except Exception:
            continue
        unknown = sorted(t for t in targets if t not in known and img.section_of(t) == img.section_of(addr))
        out.append((cls, addr, name, size, unknown))
    return out


def cmd_ready(cls, rows, claims):
    """Which claimed functions a lane can write now: every function they call already has an identity."""
    table = readiness(rows, claims)
    if cls:
        sel = sorted((t for t in table if t[0] == cls), key=lambda t: t[1])
        ready = [t for t in sel if not t[4]]
        print("%s: %d claimed without source, %d ready, %d blocked" % (cls, len(sel), len(ready), len(sel) - len(ready)))
        for c, addr, name, size, unknown in sel:
            tag = "BLOCKED" if unknown else "ready  "
            big = "  large" if size > LARGE else ""
            why = ("  calls unidentified " + ", ".join("0x%X" % u for u in unknown[:6])
                   + (" ..." if len(unknown) > 6 else "")) if unknown else ""
            print("  %s 0x%X %5d bytes%s %s%s" % (tag, addr, size, big, name[:60], why))
        return
    per = defaultdict(lambda: [0, 0, 0])
    for c, _, _, size, unknown in table:
        if not c:
            continue
        per[c][0 if not unknown and size <= LARGE else 1 if not unknown else 2] += 1
    print("%-32s %6s %6s %8s" % ("class", "ready", "large", "blocked"))
    for c in sorted(per, key=lambda c: (-per[c][0], c)):
        r, big, blocked = per[c]
        print("%-32s %6d %6d %8d" % (c, r, big, blocked))


# Words of a PARKED note that name a structural wall: a layout, a type or a helper to recover, which a cheap worker
# can't settle by respelling the body.
STRUCTURAL_WORDS = ("layout", "member", "offset", "vtable", "slot", "helper", "inline", "header", "type", "struct",
                    "template", "comdat", "identity", "unknown callee", "unidentified", "eh ", "funclet", "budget")
SPELLING_WORDS = ("return", "epilogue", "exit", "loop", "branch", "order", "accessor", "spelling", "ternary", "?:",
                  "if/else", "guard", "early", "declar", "local", "temporar", "condition", "operand")
REGISTER_WORDS = ("register", "allocat", "x87", "fpu", "spill", "frame slot", "schedul")


def tier_of_open(row):
    """(tier, reason) of an annotated function that doesn't match: who should take it first.
    sonnet: an open function with no wall noted yet, or a near-miss whose note names a spelling wall: a worker with
    a packet, docs/levers/ and tools/trial.py;
    opus: a structural wall (layout, helper, vtable, inline budget), a broken unit, a body far from retail, or a
    parked spelling wall further off (Sonnet retrying parked functions with known levers closed almost nothing and
    cost about 50k tokens a function, game-85);
    park: a register, x87 or scheduling residue, the bounded walls of docs/levers/walls.md: nobody, until a new lever."""
    note = (row.get("parked") or "").lower()
    score = row["score"]
    if row["status"] in ("NO-OBJ", "NO-SYMBOL"):
        return "opus", "broken: %s" % row["status"]
    if any(w in note for w in STRUCTURAL_WORDS):
        return "opus", "structural note"
    if score < 60:
        return "opus", "far from retail (%.1f)" % score
    spelling = any(w in note for w in SPELLING_WORDS)
    if score >= 80 and (spelling or not note):
        return "sonnet", "near-miss, %s" % ("spelling wall" if spelling else "no note")
    if any(w in note for w in REGISTER_WORDS):
        return "park", "register/x87 residue"
    if note:
        return "opus", "parked spelling wall" if spelling else "parked (%.1f)" % score
    return "sonnet", "open (%.1f)" % score


def cmd_triage(cls, rows, claims):
    """Who takes each open function first: a Sonnet worker or Opus."""
    items = []   # (tier, class, address, name, reason)
    for r in rows:
        if r["status"] in ("MATCH", "PATCHED") or (r["unit"] or "").startswith("python/") or r["unit"] == "game/AppWrap":
            continue
        tier, why = tier_of_open(r)
        wall = wall_of(r)[0]
        if wall == BOUNDED and wall_of(r)[1] == "card":
            tier, why = "state", "compiler-state, proven bounded"
        items.append((tier, class_of_row(r) or "-", int(r["addr"], 16), r["name"] or "?",
                      why if wall == "undiagnosed" else "%s; wall %s" % (why, wall)))
    import fncontext
    import packet
    decls = {}

    def inline(cls_name, decorated):
        """Whether the claimed function's body is already inline in its class header: an annotation-only job."""
        _, method = fncontext.method_of_decorated(decorated)
        if not cls_name or not method:
            return False
        if cls_name not in decls:
            decls[cls_name] = fncontext.class_declaration(cls_name)[1]
        return bool(decls[cls_name] and packet.inline_definition(decls[cls_name], method))

    for c, addr, name, size, unknown in readiness(rows, claims):
        if unknown:
            items.append(("opus", c or "-", addr, name, "claimed, calls %d unidentified" % len(unknown)))
        elif size > LARGE:
            items.append(("opus", c or "-", addr, name, "claimed, large (%d bytes)" % size))
        elif inline(c, name):
            items.append(("sonnet", c or "-", addr, name, "claimed, inline in the header: annotate it (%d bytes)" % size))
        else:
            items.append(("sonnet", c or "-", addr, name, "claimed, ready (%d bytes)" % size))
    tiers = ("sonnet", "opus", "park", "state")
    if cls == "all":
        for tier, c, addr, name, why in sorted(items, key=lambda t: (tiers.index(t[0]), t[1], t[2])):
            print("  %-6s %-28s 0x%X %-50s %s" % (tier, c[:28], addr, name[:50], why))
        return
    if cls:
        sel = sorted((t for t in items if t[1] == cls), key=lambda t: (tiers.index(t[0]), t[2]))
        for tier, _, addr, name, why in sel:
            print("  %-6s 0x%X %-50s %s" % (tier, addr, name[:50], why))
        return
    per = defaultdict(Counter)
    for tier, c, *_ in items:
        per[c][tier] += 1
    total = Counter(t[0] for t in items)
    print("open work: sonnet %d, opus %d, parked walls %d; proven bounded (compiler-state, listed apart) %d"
          % (total["sonnet"], total["opus"], total["park"], total["state"]))
    print("%-32s %6s %6s %6s %6s" % ("class", "sonnet", "opus", "park", "state"))
    for c in sorted(per, key=lambda c: (-per[c]["sonnet"], c)):
        print("%-32s %6d %6d %6d %6d" % (c, per[c]["sonnet"], per[c]["opus"], per[c]["park"], per[c]["state"]))


# The advisory rules the checker reports (tools/rules.py), with what each asks and its tier (decomp-code-rules):
# "free" can't change the bytes; "checked" can, so the fix runs the unit's match and the gate.
ADVISORY_RULES = {
    "R7": ("name things for what they mean: no placeholders",
           "checked: a name reaches the bytes only through RTTI or stringizing; evidence for each name"),
    "R11": ("name meaningful constants: a SWIG constant once, in its class header", "free (#define, enum)"),
    "R16": ("restore helpers: one inline in a shared header, not static copies", "checked"),
    "R21": ("comments say what the code means: a stale PARKED note", "free"),
    "R33": ("every function has a header comment", "free"),
    "R37": ("conditional compilation models real builds only: #if 0", "checked"),
}
PLACEHOLDER_KINDS = ((r"\w*Unknown(?:Class)?[0-9A-F]{6}\w*", "Unknown<ADDR>"), (r"unknown[0-9A-F]+", "unknown<OFF>"),
                     (r"VirtualSlot\d+\w*", "VirtualSlot<NN>"))


def placeholder_kind(name):
    """The placeholder family of an R7 finding's name: what a names lane works through (tools/names.py)."""
    return next((kind for pat, kind in PLACEHOLDER_KINDS if re.fullmatch(pat, name)), "other")


def advisory_findings(rows, file_level=True):
    """[(row or None, unit, rule, text)] of every advisory finding of a written function (the rows' rule-cache
    findings; compiler-generated _$E initialisers have no source and are left out), plus the file-level ones (R11,
    R16) with row None."""
    out = []
    for r in rows:
        if r.get("static"):
            continue
        for f in r.get("rules", []):
            if f["tier"] != "match":
                out.append((r, r["unit"], f["rule"], f["text"]))
    if file_level:
        import rules
        for path, line, rule, text in rules.file_findings(decomp.ROOT, set()):
            unit = path.relative_to(decomp.ROOT / "src").with_suffix("").as_posix()
            out.append((None, unit, rule, "%s:%d %s" % (path.relative_to(decomp.ROOT).as_posix(), line, text)))
    return out


def finished_units(rows):
    """{unit: rows} of the units whose written functions all match under the rules: nothing open, nothing pending.
    PATCHED functions and _$E initialisers don't count either way."""
    by = defaultdict(list)
    for r in rows:
        if not r.get("static") and r["unit"] != "(headers)":
            by[r["unit"]].append(r)
    return {u: rs for u, rs in by.items()
            if all(status_of(r) in ("matched", "patched") for r in rs) and any(status_of(r) == "matched" for r in rs)}


def r7_name(text):
    return text.split()[-1]


def print_findings(target, rule, found, finished):
    """A unit's (target with a '/') or a class's advisory findings, by function."""
    if "/" in target:
        sel = [f for f in found if f[1] == target]
    else:
        sel = [f for f in found if f[0] is not None and class_of_row(f[0]) == target]
    sel = [f for f in sel if rule is None or f[2] == rule]
    if not sel:
        print("no advisory findings for %s%s" % (target, " under " + rule if rule else ""))
        return
    by_fn = defaultdict(list)
    for r, unit, ru, text in sel:
        key = (int(r["addr"], 16), r["name"] or "?", status_of(r)) if r else (0, unit, "file")
        by_fn[key].append((ru, text))
    for (addr, name, state), fs in sorted(by_fn.items()):
        print("%-9s %-8s %s" % ("0x%X" % addr if addr else "(file)", state, name))
        for ru, text in sorted(fs):
            print("    %-4s %s" % (ru, text))
    print("%s: %d advisory findings in %d functions%s" % (
        target, len(sel), len(by_fn), "; every function of the unit matches" if target in finished else ""))


def print_rule_by_unit(rule, found, finished):
    """One rule's findings per unit, most first."""
    kinds = [k for _, k in PLACEHOLDER_KINDS] + ["other"]
    per = defaultdict(lambda: [0, set(), Counter()])
    for r, unit, ru, text in found:
        if ru != rule:
            continue
        per[unit][0] += 1
        per[unit][1].add(r["addr"] if r else None)
        if ru == "R7":
            per[unit][2][placeholder_kind(r7_name(text))] += 1
    asks, tier = ADVISORY_RULES.get(rule, ("?", "?"))
    print("%s: %s [%s]" % (rule, asks, tier))
    print("%6s %6s %-8s%s  %s" % ("finds", "funcs", "finished", "".join(" %15s" % k for k in kinds)
                                  if rule == "R7" else "", "unit"))
    for unit, (n, fns, kn) in sorted(per.items(), key=lambda kv: (-kv[1][0], kv[0])):
        print("%6d %6d %-8s%s  %s" % (n, len(fns - {None}), "yes" if unit in finished else "",
                                      "".join(" %15d" % kn[k] for k in kinds) if rule == "R7" else "", unit))
    print("%d findings in %d units, %d of them finished" % (
        sum(v[0] for v in per.values()), len(per), sum(1 for u in per if u in finished)))


def print_rule_table(found, finished):
    """The backlog by rule: findings, units, functions, how many sit in finished units, and what fixing asks."""
    by = defaultdict(lambda: [0, set(), set(), 0])
    names = defaultdict(Counter)
    for r, unit, ru, text in found:
        b = by[ru]
        b[0] += 1
        b[1].add(unit)
        b[2].add(r["addr"] if r else None)
        b[3] += unit in finished
        if ru == "R7":
            names[placeholder_kind(r7_name(text))][r7_name(text)] += 1
    print("%-4s %6s %5s %6s %8s  %-62s %s" % ("rule", "finds", "units", "funcs", "finished", "asks", "fix"))
    for ru in sorted(by, key=lambda x: -by[x][0]):
        n, units, fns, fin = by[ru]
        asks, tier = ADVISORY_RULES.get(ru, ("?", "?"))
        print("%-4s %6d %5d %6d %8d  %-62s %s" % (ru, n, len(units), len(fns - {None}), fin, asks[:62], tier))
    print("%d advisory findings; `finished`: in the %d units whose functions all match (`openwork.py finish`)" % (
        sum(v[0] for v in by.values()), len(finished)))
    if names:
        print("R7 by placeholder family (uses in function bodies; distinct spellings; `tools/names.py` gives the "
              "evidence by class):")
        for kind, c in sorted(names.items(), key=lambda kv: -sum(kv[1].values())):
            print("  %-16s %6d uses %6d spellings" % (kind, sum(c.values()), len(c)))


def cmd_advisory(argv, rows):
    """The readability backlog. No argument: the findings by rule. A rule: its findings per unit, ranked. A unit
    (with a '/') or a class: the findings themselves, of one rule when one is given too."""
    rule = next((a.upper() for a in argv if re.fullmatch(r"[Rr]\d+", a)), None)
    target = next((a for a in argv if not re.fullmatch(r"[Rr]\d+", a)), None)
    found = advisory_findings(rows)
    finished = finished_units(rows)
    if target:
        print_findings(target, rule, found, finished)
    elif rule:
        print_rule_by_unit(rule, found, finished)
    else:
        print_rule_table(found, finished)
    return 0


def cmd_finish(limit, rows):
    """The units a finishing lane can clean without touching open work (every written function matches), most
    advisory findings first."""
    finished = finished_units(rows)
    per = defaultdict(Counter)
    for r, unit, ru, _ in advisory_findings(rows):
        if unit in finished:
            per[unit][ru] += 1
    ranked = sorted(finished, key=lambda u: (-sum(per[u].values()), u))
    print("%5s %6s %6s %6s %6s  %s" % ("funcs", "finds", "R7", "R33", "other", "unit"))
    for u in ranked[:limit]:
        c = per[u]
        print("%5d %6d %6d %6d %6d  %s" % (len(finished[u]), sum(c.values()), c["R7"], c["R33"],
                                           sum(c.values()) - c["R7"] - c["R33"], u))
    clean = sum(1 for u in finished if not per[u])
    print("%d finished units (%d functions): %d with advisory findings (%d findings), %d clean" % (
        len(finished), sum(len(v) for v in finished.values()), len(finished) - clean,
        sum(sum(c.values()) for c in per.values()), clean))
    return 0


def cmd_thunks(text=None, img=None, claims=None):
    """Check each IMPORT-THUNK row: retail's bytes are `FF 25 <slot>`, and the slot is the import the row names (by
    the name symbols.txt claims at that address, else the row's own). Returns the number of problems."""
    img = img or decomp.pe.Image(decomp.target_image())
    if text is None:
        text = (decomp.CONFIG / "exceptions.md").read_text(encoding="utf-8")
    deviations = {}
    for line in text.splitlines():
        cells = [c.strip() for c in line.strip().strip("|").split("|")]
        if len(cells) >= 5 and cells[-1] == "IMPORT-THUNK" and re.fullmatch(r"0x[0-9A-Fa-f]+", cells[0]):
            deviations[int(cells[0], 16)] = cells[3]
    claimed = {a: n for n, a, _ in (claims if claims is not None else load()[1])}
    problems = 0
    for addr, (name, size) in sorted(import_thunks(text).items()):
        name = claimed.get(addr, name)
        why = decomp.check_import_thunk(img, addr, name, deviations.get(addr, ""))
        if size != decomp.THUNK_BYTES:
            why = why or "0x%X: %d bytes, not %d" % (addr, size, decomp.THUNK_BYTES)
        problems += bool(why)
        print("  %s 0x%X %s%s" % ("BAD" if why else "ok ", addr, name, ("  " + why) if why else ""))
    print("%d IMPORT-THUNK rows, %d problems" % (len(import_thunks(text)), problems))
    return problems


def main(argv):
    if not argv:
        sys.exit(__doc__)
    rows, claims = load()
    cmd = argv[0]
    if cmd == "phases":
        cmd_phases(rows, claims, decomp.load_report().get("summary", {}))
        return 0
    if cmd == "drafts":
        cmd_drafts(float(argv[1]) if len(argv) > 1 else 0.0, rows)
        return 0
    if cmd == "ready":
        cmd_ready(argv[1] if len(argv) > 1 else None, rows, claims)
    elif cmd == "class" and len(argv) > 1:
        cmd_class(argv[1], rows, claims)
    elif cmd == "range" and len(argv) > 2:
        cmd_range(int(argv[1], 16), int(argv[2], 16), rows, claims)
    elif cmd == "gaps":
        cmd_gaps(int(argv[1]) if len(argv) > 1 else 40, rows)
    elif cmd == "classes":
        cmd_classes(rows, claims)
    elif cmd == "triage":
        cmd_triage(argv[1] if len(argv) > 1 else None, rows, claims)
    elif cmd == "parked":
        cmd_parked(float(argv[1]) if len(argv) > 1 else 0.0, rows)
    elif cmd == "advisory":
        return cmd_advisory(argv[1:], rows)
    elif cmd == "finish":
        return cmd_finish(int(argv[1]) if len(argv) > 1 else 30, rows)
    elif cmd == "thunks":
        return 1 if cmd_thunks(claims=claims) else 0
    elif cmd == "groups":
        return cmd_groups(argv[1] if len(argv) > 1 else None, rows)
    elif cmd == "headers":
        return cmd_headers(argv[1] if len(argv) > 1 else None, rows)
    elif cmd == "untried":
        cmd_untried(argv[1:], rows)
    elif cmd == "walls":
        cmd_walls(argv[1] if len(argv) > 1 else None, float(argv[2]) if len(argv) > 2 else 0.0, rows)
    else:
        sys.exit(__doc__)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
