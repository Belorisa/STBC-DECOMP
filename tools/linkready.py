"""How far a real link of the compiled objects is from retail's layout: a report, no exe built.

    python tools/linkready.py                     # the summary
    python tools/linkready.py --unit game/ShipClass
    python tools/linkready.py --list text|data|order|unresolved [--top N]
    python tools/linkready.py --json build/linkready.json

The byte gate splices each exact function into a copy of retail and places every datum at retail's address, so it
can't see whether the objects would link into retail's layout. LINK 6 lays out each object's non-COMDAT section
(`.text`, `.data`, `.rdata`, `.bss`) as one contiguous contribution, in the order of the objects on its command
line, the same order for every section. This report measures that, after `decomp.py build`:

- **text**: for each unit, the base each of its functions implies (its retail address minus its offset in the
  object's `.text` contribution: the object's `.text` sections, one per function as VC6 emits them here, laid end to
  end in their order at their alignment, as LINK does). One base: the unit's code would link where retail has it, its functions in retail's
  order with retail's sizes. Several: a function out of order, a function whose compiled size differs from retail's
  (an open function shifts every later one), code retail has that the object lacks, or an inline copy the object
  emits where retail kept another unit's (LINK keeps the first object's copy and drops the rest).
- **data**: the same for each non-COMDAT data section of the object, from the retail addresses the exact functions'
  relocations hold (a relocation's retail dword minus its addend, against the target's offset in its section). One
  base: every datum those functions reach sits where retail has it relative to the others. Data no exact function
  reaches is not measured. COMDAT data (string literals, float constants, vtables in /Gy objects) is placed by the
  linker per name and not measured here.
- **order**: the units sorted by their text base against the same units sorted by each data base. LINK uses one
  object order for all sections, so a unit whose data falls out of its code's order needs another object order than
  its code (two retail units in one source file, or one retail unit split in two).
- **unresolved**: tools/linkcheck.py's unresolved externals (weak externals already resolved through their fallback,
  a vftable's `??_E` falling back to `??_G`; references made only by a unit another unit #includes, and `__imp_`
  imports, left out as linkcheck counts them apart), by kind: compiler-generated members, template instances, and
  the rest (functions or data with no source and no symbols.txt claim yet). linkcheck also counts an annotated name
  no object defines as resolved; this count doesn't, so the two differ by linkcheck's "an annotation gives" line.

Not measured: COMDAT selection and folding (which unit's copy LINK keeps, /OPT:ICF), the padding between objects,
imports and resources. Those need a real link.
"""

import argparse
import bisect
import json
import struct
import sys
from collections import Counter, defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import coff  # noqa: E402
import comdat  # noqa: E402
import decomp  # noqa: E402
import linkcheck  # noqa: E402
import units as U  # noqa: E402

EXACT = ("MATCH",)
DIR32 = 6
REL32 = 20


def section_kind(sec):
    if sec.is_code:
        return "text"
    if sec.is_bss or sec.name.startswith(".bss"):
        return "bss"
    if sec.name.startswith(".rdata"):
        return "rdata"
    if sec.name.startswith(".data"):
        return "data"
    return None


def text_offsets(obj, elsewhere=frozenset()):
    """{section number: offset in the object's .text contribution}: LINK concatenates an object's `.text` sections
    (one per function when they are COMDATs) in their order, each at its own alignment. `.text$x` (EH funclets)
    sorts after every object's `.text` and is left out, and so is a COMDAT whose function retail kept from another
    unit (`elsewhere`: the names annotated to other units), which LINK drops when it keeps that unit's copy."""
    names = defaultdict(set)
    for s in obj.symbols.values():
        if s.defined and s.storage == coff.SC_EXTERNAL:
            names[s.section].add(s.name)
    out, at = {}, 0
    for sec in obj.sections:
        if sec.flags & coff.SCN_LNK_COMDAT and names[sec.index] & elsewhere:
            continue
        if sec.is_code and sec.name == ".text":
            align = sec.alignment
            at = (at + align - 1) // align * align
            out[sec.index] = at
            at += sec.size
    return out


def unit_layout(unit, obj, urows, img, elsewhere=frozenset()):
    """{kind: Counter(base: count)} for one unit's contributions, and {kind: object size}: text over the object's
    concatenated `.text` sections, data over its non-COMDAT data sections."""
    bases = defaultdict(Counter)
    sizes = defaultdict(int)
    offsets = text_offsets(obj, elsewhere)
    for sec in obj.sections:
        kind = section_kind(sec)
        if kind == "text" and sec.index in offsets:
            sizes[kind] += sec.size
        elif kind and kind != "text" and not sec.flags & coff.SCN_LNK_COMDAT:
            sizes[kind] += sec.size
    by_name = {}
    for s in obj.symbols.values():
        if s.defined:
            by_name.setdefault(s.name, s)
    for r in urows:
        sym = by_name.get(r.get("symbol"))
        if sym is None or sym.section not in offsets:
            continue
        sec = obj.section(sym.section)
        a = int(r["addr"], 16)
        bases["text"][a - offsets[sym.section] - sym.value] += 1
        if r["status"] not in EXACT or not r.get("size"):
            continue
        start, end = sym.value, sym.value + r["size"]
        for off, idx, typ in sec.relocs:
            if typ != DIR32 or not start <= off < end:
                continue
            target = obj.symbols.get(idx)
            if target is None or not target.defined:
                continue
            tsec = obj.section(target.section)
            kind = section_kind(tsec)
            if kind in (None, "text") or tsec.flags & coff.SCN_LNK_COMDAT:
                continue
            addend = struct.unpack_from("<I", sec.data, off)[0]
            at = img.dword(a + off - start)
            if at is None:
                continue
            # the field holds target + addend; retail's dword holds retail(target) + addend
            bases[kind][(at - addend - target.value) & 0xFFFFFFFF] += 1
    return bases, sizes


def layouts(rows, img):
    by_unit = defaultdict(list)
    for r in rows:
        if r.get("unit") and not r["unit"].startswith("("):
            by_unit[r["unit"]].append(r)
    owner = defaultdict(set)   # symbol -> the units it is annotated to
    for r in rows:
        if r.get("symbol") and r.get("unit"):
            owner[r["symbol"]].add(r["unit"])
    out = {}
    for unit in decomp.units_to_compile(decomp.source_files()):
        path = decomp.BUILD / "obj" / (unit + ".obj")
        if not path.exists() or unit not in by_unit:
            continue
        elsewhere = frozenset(s for s, us in owner.items() if unit not in us)
        out[unit] = unit_layout(unit, coff.Coff(path.read_bytes()), by_unit[unit], img, elsewhere)
    return out


def kept_annotations(units):
    """{unit: {address: annotated name}} of the entries bound to each unit that retail kept from it: its own file's
    and the header entries naming it by `@unit` (comdat.unit_annotations' kept_only, for every unit in one load)."""
    files = decomp.source_files()
    objs = {u: decomp.BUILD / "obj" / (u + ".obj") for u in units}
    out = defaultdict(dict)
    for u in decomp.load_units(objs, files).values():
        own = files.get(u.name)
        for e in u.entries:
            if Path(e.path) != own and getattr(e, "bind", None) != u.name:
                continue
            out[u.name][e.addr] = e.decorated or e.name
    return out


def unit_order(unit, own):
    """(sections with a retail address, of them in retail's order, out of order: own functions, inline/template
    copies) for one unit's code sections in object order, as `comdat.py order` reads them: LINK places an object's
    COMDAT sections in its section order, so a section off the longest run in retail's address order would land
    elsewhere than retail has it. `_$E` stubs and EH funclets are left out."""
    obj_path = decomp.BUILD / "obj" / (unit + ".obj")
    obj = coff.Coff(obj_path.read_bytes())
    sel = comdat.section_selections(obj_path.read_bytes())
    by_name = {}
    for addr, n in own.items():
        by_name.setdefault(n, []).append(addr)
    secs = []
    for sec, sym in comdat.code_sections(obj):
        kind = comdat.kind_of(sec, sym)
        if kind in ("funclet", "$E") or sec.size == 0 or sym is None:
            continue
        addrs = by_name.get(sym.name) or by_name.get(decomp.qualified(sym.name)) or []
        secs.append((comdat.SELECTIONS.get(sel.get(sec.index), "-"), min(addrs) if addrs else None))
    run = set(comdat.rising_run([a for _, a in secs]))
    known = [i for i, (_, a) in enumerate(secs) if a is not None]
    out_own = sum(1 for i in known if i not in run and secs[i][0] == "nodup")
    out_copy = sum(1 for i in known if i not in run and secs[i][0] != "nodup")
    return len(known), len(run), out_own, out_copy


def main_base(counter):
    return counter.most_common(1)[0][0] if counter else None


def order_breaks(lay, kind):
    """The units whose `kind` base falls outside the longest run already in their text bases' order."""
    seq = sorted((main_base(b["text"]), main_base(b[kind]), u) for u, (b, _) in lay.items()
                 if b["text"] and b[kind])
    funcs = [(d, u) for _, d, u in seq]
    return [u for _, u in U.out_of_place(funcs)]


def unresolved_kind(name):
    if name.startswith(("??_E", "??_G")):
        return "deleting destructor"
    if name.startswith("??_7"):
        return "vtable"
    if "?$" in name:
        return "template instance"
    if name.startswith(("??0", "??1")):
        return "constructor/destructor"
    return "function or data"


def unresolved():
    """linkcheck's unresolved externals against the symbols.txt claims: linkcheck.analyse resolves a weak external
    through its fallback (a vftable's slot 0 names a weak `??_E` that falls back to `??_G`, docs/c2-backend.md), and
    linkcheck.split takes out the variant references and the imports."""
    objects = linkcheck.load_objects()
    claimed = {n for n, _, _ in openwork_claims()}
    included = decomp.includers(decomp.source_files())
    _, unres, _ = linkcheck.analyse(objects, claimed, included)
    return linkcheck.split(unres, included)[0]


def openwork_claims():
    out = []
    for line in (decomp.CONFIG / "symbols.txt").read_text(encoding="utf-8").splitlines():
        body = line.split("#")[0].split()
        if len(body) >= 2:
            out.append((body[0], int(body[1], 16), ""))
    return out


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--unit", help="one unit's bases per section")
    ap.add_argument("--list", choices=("text", "data", "order", "unresolved"), help="the units or names behind a line")
    ap.add_argument("--top", type=int, default=40)
    ap.add_argument("--json", help="write the per-unit table to this file")
    args = ap.parse_args(argv)
    rows = decomp.load_report()["functions"]
    img = decomp.pe.Image(decomp.target_image())
    lay = layouts(rows, img)

    if args.unit:
        b, sizes = lay.get(args.unit, ({}, {}))
        for kind in ("text", "data", "rdata", "bss"):
            c = b.get(kind) or Counter()
            print("%-6s object %6d bytes, %d base(s): %s" % (kind, sizes.get(kind, 0), len(c),
                  ", ".join("0x%X x%d" % kv for kv in c.most_common(6))))
        return 0

    open_units = {r["unit"] for r in rows if r.get("unit") and r["status"] not in EXACT + ("PATCHED",)}
    text_ok = [u for u, (b, _) in lay.items() if len(b["text"]) == 1]
    text_bad = sorted(((len(b["text"]), u) for u, (b, _) in lay.items() if len(b["text"]) > 1), reverse=True)
    text_bad_exact = [u for _, u in text_bad if u not in open_units]
    modules = defaultdict(Counter)
    for u, (b, _) in lay.items():
        modules[u.split("/")[0]]["one" if len(b["text"]) == 1 else "open" if u in open_units else "exact"] += 1
    data_stats = {}
    for kind in ("data", "rdata", "bss"):
        have = [u for u, (b, _) in lay.items() if b[kind]]
        bad = sorted(((len(lay[u][0][kind]), u) for u in have if len(lay[u][0][kind]) > 1), reverse=True)
        data_stats[kind] = (have, bad, order_breaks(lay, kind))
    unres = unresolved()
    kinds = Counter(unresolved_kind(n) for n in unres)

    table = {u: {k: {"0x%X" % base: n for base, n in b[k].items()} for k in ("text", "data", "rdata", "bss")}
             for u, (b, _) in lay.items()}
    if args.json:
        Path(args.json).write_text(json.dumps(table, indent=1), encoding="utf-8")

    if args.list == "text":
        for n, u in text_bad[:args.top]:
            print("  %-44s %4d bases%s" % (u, n, "" if u in open_units else "   (every function exact)"))
        return 0
    if args.list == "data":
        for kind, (have, bad, _) in data_stats.items():
            for n, u in bad[:args.top]:
                print("  %-6s %-44s %d bases" % (kind, u, n))
        return 0
    if args.list == "order":
        for kind, (_, _, breaks) in data_stats.items():
            for u in breaks[:args.top]:
                print("  %-6s %s" % (kind, u))
        return 0
    if args.list == "unresolved":
        for n in sorted(unres, key=lambda n: (-len(unres[n]), n))[:args.top]:
            print("  %-24s %-70s %d units" % (unresolved_kind(n), n[:70], len(unres[n])))
        return 0

    print("linkready: %d units with code in the report" % len(lay))
    kept = kept_annotations(list(lay))
    order = {u: unit_order(u, kept.get(u, {})) for u in lay}
    in_order = [u for u, o in order.items() if o[0] and o[1] == o[0]]
    print("  text order (sections in object order against retail's addresses, as comdat.py order reads them):")
    print("         units fully in retail's order: %d of %d; sections out of place: %d own functions, %d inline or "
          "template copies" % (len(in_order), sum(1 for o in order.values() if o[0]),
                               sum(o[2] for o in order.values()), sum(o[3] for o in order.values())))
    print("  text offsets (one base: every section where retail has it, sizes included): %d units; several: %d, of "
          "which %d with every function exact" % (len(text_ok), len(text_bad), len(text_bad_exact)))
    for m, c in sorted(modules.items()):
        oo = [order[u] for u in lay if u.split("/")[0] == m]
        print("         %-11s one base %4d | several: open functions %4d, every function exact %4d | out of "
              "place: own %5d, copies %5d" % (m, c["one"], c["open"], c["exact"], sum(o[2] for o in oo),
                                              sum(o[3] for o in oo)))
    for kind, (have, bad, breaks) in data_stats.items():
        print("  %-6s measured in %d units; one base: %d; several: %d; out of the code's object order: %d"
              % (kind, len(have), len(have) - len(bad), len(bad), len(breaks)))
    print("  unresolved externals: %d names (%s)" % (len(unres), ", ".join("%s %d" % kv for kv in kinds.most_common())))
    print("not measured: COMDAT selection and folding, padding between objects, imports, resources")
    return 0


if __name__ == "__main__":
    sys.exit(main())
