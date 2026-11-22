"""Link dry-run: what a linker would say about the compiled objects, without producing an exe. A report only; nothing
gates on it.

    python tools/linkcheck.py              # counts, then the top offenders by unit
    python tools/linkcheck.py --top 30     # more units per list
    python tools/linkcheck.py --all        # every duplicate and unresolved name

Reads every compiled unit's object in build/obj (run `decomp.py build` first) with tools/coff.py and reports:

- duplicate definitions: an external symbol defined, not as a COMDAT, in more than one object (LNK2005). The byte
  gate splices functions into retail one by one, so it can't see a function or datum two units both define: one of
  them is in the wrong unit.
- unresolved externals: a symbol objects reference that no object defines (COMDAT, common or plain), and that
  config/symbols.txt doesn't claim and no annotation names (LNK2001). These are the code and data the source still
  lacks, or names that differ between a declaration and its definition. Names an annotation gives that no object
  defines (a header inline no unit emits yet) are counted on a line of their own.

A weak external (storage class 105) is a reference with a fallback: its auxiliary record names the symbol LINK binds
it to when no object defines the name itself. VC6 writes a vftable's slot 0 as the weak `??_E` (vector deleting
destructor) falling back to the `??_G` (scalar deleting destructor) of the same class, and emits a `??_E` body only
in a unit that deletes an array of the class; retail's vftables accordingly hold `??_G` bodies in slot 0
(NiLoadRecord's at 0x88B840 holds 0x4554D0, a scalar deleting destructor). Such a name is resolved when its fallback
is, and is unresolved, shown with its fallback, only when neither is defined.

Two kinds of unresolved name are counted apart, on lines of their own: references made only by a unit whose source
file another unit #includes (crt0.c's console variant references _main; no link takes both), and imports (`__imp__X`)
no symbols.txt claim names, which an import library resolves in a real link (retail's import table lacking one means
retail's link discarded every reference to it).
"""

import argparse
import sys
from collections import Counter, defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import coff  # noqa: E402
import decomp  # noqa: E402


def object_symbols(obj):
    """(strong definitions, COMDAT or common definitions, references) of one object's external symbols. The
    references are a dict {name: the name a weak external falls back to, or None for a plain reference}."""
    strong, merged, refs = set(), set(), {}
    for s in obj.symbols.values():
        if s.storage == coff.SC_WEAK_EXTERNAL:
            d = obj.symbols.get(s.weak_default) if s.weak_default is not None else None
            refs[s.name] = d.name if d else None
            continue
        if s.storage != coff.SC_EXTERNAL:
            continue
        if s.section > 0:
            if obj.section(s.section).flags & coff.SCN_LNK_COMDAT:
                merged.add(s.name)
            else:
                strong.add(s.name)
        elif s.section == 0:
            if s.value:
                merged.add(s.name)          # a value on an undefined external: a common definition
            else:
                refs.setdefault(s.name, None)
    return strong, merged, {n: d for n, d in refs.items() if n not in strong and n not in merged}


def fallback(refs, name):
    """The name a weak reference falls back to (None for a plain reference, or for refs given as a set)."""
    return refs.get(name) if isinstance(refs, dict) else None


def analyse(objects, claimed, included=None):
    """objects: {unit: (strong, merged, refs)}; claimed: names config/symbols.txt gives an address; included: unit ->
    the units that #include its source file (decomp.includers). Returns (duplicates {name: [units]}, unresolved
    {name: [units]}, variants {name: [units]}): a name defined only by a unit and the units that include its file
    (crt0.c in wincrt0.c) is a variant the build compiles twice, which no link would combine, not a duplicate. A weak
    reference whose fallback is defined or claimed is resolved; split() takes out the kinds counted apart."""
    included = included or {}
    strong_by, defined = defaultdict(list), set()
    for unit, (strong, merged, _) in objects.items():
        for n in strong:
            strong_by[n].append(unit)
        defined |= strong | merged
    duplicates, variants = {}, {}
    for n, us in strong_by.items():
        if len(us) < 2:
            continue
        family = any(set(us) <= {u} | included.get(u, set()) for u in us)
        (variants if family else duplicates)[n] = sorted(us)
    unresolved = defaultdict(list)
    known = defined | set(claimed)
    for unit, (_, _, refs) in objects.items():
        for n in refs:
            if n in known or fallback(refs, n) in known:
                continue
            unresolved[n].append(unit)
    return duplicates, {n: sorted(us) for n, us in unresolved.items()}, variants


def split(unresolved, included=None):
    """(unresolved, variant references, imports): the names referenced only by units whose file another unit
    #includes, and the `__imp_` names, taken out of `unresolved`."""
    included = included or {}
    rest, variant, imports = {}, {}, {}
    for n, us in unresolved.items():
        if n.startswith("__imp_"):
            imports[n] = us
        elif all(included.get(u) for u in us):
            variant[n] = us
        else:
            rest[n] = us
    return rest, variant, imports


def weak_fallbacks(objects, names):
    """{name: sorted fallbacks} of the weak references among `names`."""
    out = defaultdict(set)
    for _, _, refs in objects.values():
        for n in names:
            d = fallback(refs, n) if n in refs else None
            if d:
                out[n].add(d)
    return {n: sorted(ds) for n, ds in out.items()}


def load_objects(objdir=None):
    objdir = Path(objdir) if objdir else decomp.BUILD / "obj"
    out = {}
    for unit in decomp.units_to_compile(decomp.source_files()):
        p = objdir / (unit + ".obj")
        if p.exists():
            out[unit] = object_symbols(coff.Coff(p.read_bytes()))
    return out


def by_unit(table):
    c = Counter()
    for units in table.values():
        for u in set(units):
            c[u] += 1
    return c


def report(duplicates, unresolved, top=15, every=False, variants=None, variant_refs=None, imports=None,
           fallbacks=None):
    fallbacks = fallbacks or {}
    lines = ["duplicate definitions (non-COMDAT, in more than one object): %d names" % len(duplicates),
             "  (and %d names defined by a unit and the units that #include its file: variants, not counted)"
             % len(variants or {}),
             "unresolved externals (no object defines them or a weak reference's fallback, no symbols.txt claim): "
             "%d names, %d references" % (len(unresolved), sum(len(u) for u in unresolved.values()))]
    apart = (("  (and %d referenced only by a unit whose file another unit #includes: variants, not counted)",
              variant_refs),
             ("  (and %d imports no symbols.txt claim names, which an import library resolves: not counted)",
              imports))
    for title, table in apart:
        if table:
            lines.append(title % len(table) + ": " + ", ".join("%s (%s)" % (n, ", ".join(us))
                                                               for n, us in sorted(table.items())))
    for title, table in (("duplicate definitions", duplicates), ("unresolved externals", unresolved)):
        if not table:
            continue
        lines.append("")
        lines.append("%s, top units:" % title)
        for unit, n in by_unit(table).most_common(top):
            lines.append("  %5d  %s" % (n, unit))
        lines.append("%s, most widely referenced:" % title if title.startswith("unres") else "%s, names:" % title)
        names = sorted(table, key=lambda n: (-len(table[n]), n))
        for n in names if every else names[:top]:
            units = table[n]
            weak = " [weak, falls back to %s]" % ", ".join(fallbacks[n]) if n in fallbacks else ""
            lines.append("  %-60s %s%s" % (n, ", ".join(units[:4]) + (" (+%d)" % (len(units) - 4) if len(units) > 4
                                                                        else ""), weak))
        if not every and len(names) > top:
            lines.append("  ... and %d more (--all)" % (len(names) - top))
    return lines


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("--top", type=int, default=15)
    ap.add_argument("--all", action="store_true")
    args = ap.parse_args(argv)
    objects = load_objects()
    if not objects:
        sys.exit("no objects in build/obj: run `python tools/decomp.py build` first")
    import comdat
    annotated = set(comdat.all_annotations().values())
    included = decomp.includers(decomp.source_files())
    duplicates, unresolved, variants = analyse(objects, set(decomp.extra_symbols()) | annotated, included)
    unresolved, variant_refs, imports = split(unresolved, included)
    _, open_annotated, _ = analyse(objects, set(decomp.extra_symbols()))
    open_annotated = [n for n in open_annotated if n in annotated]
    print("objects read: %d" % len(objects))
    print("referenced names an annotation gives but no object defines: %d (not counted below)" % len(open_annotated))
    claim_only = analyse(objects, annotated, included)[1]
    claim_only = {n: us for n, us in claim_only.items() if n not in unresolved and n not in variant_refs
                  and n not in imports}
    imp = [n for n in claim_only if n.startswith("__imp_")]
    print("referenced names only a symbols.txt claim resolves: %d, %d references (%d imports an import library "
          "resolves; %d a CRT or system library, or source the tree lacks, must supply: not counted below)"
          % (len(claim_only), sum(len(us) for us in claim_only.values()), len(imp), len(claim_only) - len(imp)))
    print("\n".join(report(duplicates, unresolved, args.top, args.all, variants, variant_refs, imports,
                           weak_fallbacks(objects, unresolved))))
    return 0


if __name__ == "__main__":
    sys.exit(main())
