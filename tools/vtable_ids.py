"""Identify retail functions through vtables: each slot of a compiled vtable names its function, and the same slot
of the class's retail vtable (config/vtables.txt) holds that function's retail address.

The compiled objects in build/obj/ define each class's vtable ('??_7Class@@6B@') where the class's first non-inline
virtual function is compiled, with one relocation per slot to the function's decorated name. Slot i of the compiled
vtable and slot i of the retail one are the same function. An identity is proposed only when it is unambiguous:
the address is not annotated or claimed already, no other name lands on the same address (the linker folds
identical bodies, so an empty `{}` may serve several classes), and the name lands on no other address. A slot whose
compiled name is annotated at a different retail address is reported as a conflict: one of the two is wrong.

    python tools/vtable_ids.py              # what would be added, and the conflicts
    python tools/vtable_ids.py --write      # append the identities to config/symbols.txt, with their evidence

Run after `python tools/decomp.py build`.
"""
import argparse
import re
import struct
import sys
from collections import defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import coff  # noqa: E402
import decomp  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
PURECALL = {"__purecall"}


def retail_vtables():
    """{class: (address, slot count)} from config/vtables.txt."""
    out = {}
    for line in (ROOT / "config" / "vtables.txt").read_text(encoding="utf-8").splitlines():
        # "Class 0xADDRESS slots": a template class's name may hold spaces
        m = re.match(r"\s*(.+?)\s+(0x[0-9A-Fa-f]+)\s+(\d+)", line.split("#")[0])
        if m:
            out[m.group(1)] = (int(m.group(2), 16), int(m.group(3)))
    return out


BASIC = {"C": "signed char", "D": "char", "E": "unsigned char", "F": "short", "G": "unsigned short", "H": "int",
         "I": "unsigned int", "J": "long", "K": "unsigned long", "M": "float", "N": "double", "X": "void",
         "_N": "bool", "_J": "__int64", "_K": "unsigned __int64"}


class Unparsed(Exception):
    pass


def demangle_class(text):
    """The C++ name of an MSVC-mangled class name ('?$NiTMap@HH@' -> 'NiTMap<int,int>', 'Inner@Outer@' ->
    'Outer::Inner'). Covers what the project's vtables use: templates over basic types, pointers, const, classes,
    structs and nested templates; a function-pointer argument or anything else raises Unparsed."""
    names = []   # back-reference table: the names met so far, '0'-'9' refer to them

    def name(i):
        if text.startswith("?$", i):
            j = text.index("@", i + 2)
            base, i, args = text[i + 2:j], j + 1, []
            outer = names[:]
            names.clear()   # a template's arguments have their own back-reference table
            while text[i] != "@":
                t, i = typ(i)
                args.append(t)
            names[:] = outer
            n = "%s<%s>" % (base, ",".join(args))
            names.append(n)
            return n, i + 1
        if text[i].isdigit():
            return names[int(text[i])], i + 1
        j = text.index("@", i)
        n = text[i:j]
        names.append(n)
        return n, j + 1

    def qualified(i):
        parts = []
        while text[i] != "@":
            p, i = name(i)
            parts.append(p)
        return "::".join(reversed(parts)), i + 1

    def typ(i):
        for code in sorted(BASIC, key=len, reverse=True):
            if text.startswith(code, i):
                return BASIC[code], i + len(code)
        c = text[i]
        if c in "PQ" and text[i + 1] in "AB":
            t, j = typ(i + 2)
            return ("const " if text[i + 1] == "B" else "") + t + "*", j
        if c == "A" and text[i + 1] in "AB":
            t, j = typ(i + 2)
            return ("const " if text[i + 1] == "B" else "") + t + "&", j
        if c in "VU":
            return qualified(i + 1)
        if text.startswith("W4", i):
            return qualified(i + 2)
        raise Unparsed(text[i:])

    try:
        n, i = qualified(0)
    except (Unparsed, ValueError, IndexError):
        return None
    return n if i == len(text) else None


def name_key(name):
    """A class name compared without spaces and underscores: config/vtables.txt writes 'const_char*' and
    'unsigned long', the demangler 'const char*'."""
    return re.sub(r"[\s_]", "", name)


def compiled_vtables():
    """{class: (unit, [decorated name per slot])} of the primary vtables the compiled objects define. A class is
    named as config/vtables.txt names it when the demangled names agree."""
    retail_names = {name_key(k): k for k in retail_vtables()}
    out = {}
    for obj in sorted((ROOT / "build" / "obj").rglob("*.obj")):
        c = coff.Coff(obj.read_bytes())
        syms = c.symbols
        for s in syms.values():
            m = re.match(r"\?\?_7(.+@)6B@$", s.name)   # the primary vtable: '??_7Class@@6B@'
            if not m or not s.defined:
                continue
            cls = demangle_class(m.group(1)) or "::".join(reversed(m.group(1).rstrip("@").split("@")))
            cls = retail_names.get(name_key(cls), cls)
            if cls in out:
                continue
            sec = c.section(s.section)
            slots = {}
            for off, index, _ in sec.relocs:
                if off >= s.value:
                    target = syms.get(index)
                    if target is not None:
                        slots[(off - s.value) // 4] = target.name
            if slots:
                unit = obj.relative_to(ROOT / "build" / "obj").with_suffix("").as_posix()
                out[cls] = (unit, [slots.get(i) for i in range(max(slots) + 1)])
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("--write", action="store_true", help="append the identities to config/symbols.txt")
    args = ap.parse_args()

    img = decomp.pe.Image(decomp.target_image())
    report = decomp.load_report()["functions"]
    annotated = {int(r["addr"], 16): r.get("symbol") or r["name"] for r in report}
    by_symbol = defaultdict(set)     # decorated name -> its annotated addresses (an unfolded inline has several)
    for r in report:
        if r.get("symbol"):
            by_symbol[r["symbol"]].add(int(r["addr"], 16))
    claims = decomp.extra_symbols()
    claimed_at = {a: n for n, a in claims.items()}

    names_at = defaultdict(set)      # retail address -> decorated names the slots give it
    evidence = {}                    # (name, address) -> evidence text
    conflicts = []
    retail = retail_vtables()
    compiled = compiled_vtables()
    for cls, (unit, slots) in sorted(compiled.items()):
        if cls not in retail:
            continue
        base, count = retail[cls]
        for i, name in enumerate(slots[:count]):
            if not name or name in PURECALL:
                continue
            raw = img.read(base + 4 * i, 4)
            if raw is None:
                continue
            (addr,) = struct.unpack("<I", raw)
            if img.section_of(addr) != img.text:
                continue
            known = by_symbol.get(name) or ({claims[name]} if name in claims else set())
            if known and addr not in known:
                conflicts.append("%s slot %d: %s is at %s in the source, retail's slot holds 0x%X" % (
                    cls, i, name, ", ".join("0x%X" % k for k in sorted(known)), addr))
                continue
            names_at[addr].add(name)
            evidence.setdefault((name, addr), "slot %d of %s's vtable 0x%X (the compiled vtable of %s)" % (i, cls, base, unit))

    new, folded = [], 0
    addrs_of = defaultdict(set)
    for addr, names in names_at.items():
        for n in names:
            addrs_of[n].add(addr)
    for addr, names in sorted(names_at.items()):
        if addr in annotated or addr in claimed_at:
            continue
        if len(names) != 1:
            folded += 1
            continue
        (name,) = names
        if len(addrs_of[name]) != 1 or name in claims or name in by_symbol:
            continue
        new.append((name, addr, evidence[(name, addr)]))

    print("compiled vtables matched to retail: %d of %d" % (len([c for c in compiled if c in retail]), len(retail)))
    print("new identities: %d   (addresses several names share, skipped: %d)" % (len(new), folded))
    for name, addr, why in new[:40]:
        print("  0x%06X %s   # %s" % (addr, name, why))
    if len(new) > 40:
        print("  ... %d more" % (len(new) - 40))
    for c in conflicts:
        print("CONFLICT " + c)
    if args.write and new:
        with open(ROOT / "config" / "symbols.txt", "a", encoding="utf-8", newline="\n") as f:
            for name, addr, why in new:
                f.write("%s 0x%08x   # %s\n" % (name, addr, why))
        print("appended %d entries to config/symbols.txt" % len(new))


if __name__ == "__main__":
    main()
