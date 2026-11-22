"""Find where retail kept a compiled unit's COMDAT functions (inline and template instances), and lay a unit's code
out against a retail gap.

    python tools/comdat.py game/TGSoundRegion                 # every COMDAT function of the unit, found anywhere in .text
    python tools/comdat.py game/TGSoundRegion 0x70E800 0x70EC00   # only hits in that retail range
    python tools/comdat.py layout game/SetManager             # the unit's code sections in link order, with sizes
    python tools/comdat.py layout game/SetManager 0x417E00    # ... fitted against retail from that address
    python tools/comdat.py order game/SetManager              # the unit's emission order against retail's
    python tools/comdat.py order game/SetManager --all        # ... every section, not only those out of order

Search mode: for each COMDAT function in the unit's object (build/obj/<unit>.obj), the bytes with their relocation
fields masked are searched for in retail's .text at 16-byte function boundaries. A hit is a candidate
`// FUNCTION: STBC <addr> <decorated> @<unit>` annotation in the header that defines the inline or template; the
decorated name is printed so it can be pasted. Several hits mean the linker kept identical copies apart (the names
differ; compare callers).

Layout mode: every function of a VC6 /Gy object sits in its own COMDAT `.text` section, and the linker lays a unit's
sections out in object order, each at its section alignment, leaving out the copies it took from an earlier object.
So the unit's sections in object order (its own functions, the template and inline instances it emits, the `_$E`
static initialisers of the headers it includes) are the order retail shows. Each line gives the kind (`fn`
external, `static`, `comdat` for a selectable copy, `$E` for a static initialiser, `funclet` for `.text$x`), the
size, the name, and the annotated address of that name if any. With a start address, the sections are fitted
greedily: a section whose masked bytes equal retail's at the cursor is placed there (the cursor moves on by its
aligned size); one that differs is printed as `--` (kept from another unit, or a different body) and the cursor
stays. A retail address that is annotated by some other name is shown after `=`. Funclets (`.text$x`) are grouped
after all `.text` by the linker and are skipped in the fit.

Order mode: the linker keeps a unit's sections in object order, so the retail addresses of the unit's functions,
read in its object's order, must rise. Each code section is given the address its annotation bound to this unit
names (its own functions and the `@unit` copies of header inlines and templates), and with `--search` a COMDAT copy
without one is looked up as search mode does (a single hit only). The longest rising run of those addresses is the
order both agree on; every other section is printed as out of order, with the sections retail puts it between. A
COMDAT's selection is read from its section symbol (`any` for an inline, implicit member or template instance,
`nodup` for an out-of-line function), the kind the emission rules of docs/c2-backend.md ("What a unit's object holds,
and in which order") distinguish: out-of-line functions in definition order, the inline copies one needs right after
it, template instances at the end, a `static` function after its first caller. Idea after bw1-decomp's
tools/comdat-scan.py `order` (openblack's Black & White decompilation, CC0), which compares split target objects; ours
reads retail's addresses.

Build the unit first (`decomp.py match <unit>`).
"""

import bisect
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import coff  # noqa: E402
import decomp  # noqa: E402
import pe  # noqa: E402


def unit_name(arg):
    return re.sub(r"\.(c|cpp)$", "", re.sub(r"^src/", "", arg.replace("\\", "/")))


def load_obj(unit):
    obj_path = decomp.BUILD / "obj" / (unit + ".obj")
    if not obj_path.exists():
        sys.exit("no %s: run `python tools/decomp.py match %s` first" % (obj_path, unit))
    return obj_path, coff.Coff(obj_path.read_bytes())


def masked_body(sec, a, b):
    body = sec.data[a:b]
    mask = bytearray(b"\xff" * len(body))
    for off, _, _ in sec.relocs:
        if a <= off < b:
            for k in range(4):
                if off - a + k < len(mask):
                    mask[off - a + k] = 0
    return body, mask


def same(img, addr, body, mask):
    r = img.read(addr, len(body))
    return r is not None and all(mask[i] == 0 or r[i] == body[i] for i in range(len(body)))


def all_annotations():
    """address -> annotated name, over every source and header (no compile)."""
    out = {}
    for name, path in decomp.source_files().items():
        for e in decomp.scan(name, path):
            out[e.addr] = e.decorated or e.name
    for e in decomp.header_entries():
        out[e.addr] = e.decorated or e.name
    return out


def code_sections(obj):
    """The object's code sections in link order: (section, primary function symbol or None)."""
    by_sec = {}
    for s in obj.symbols.values():
        if s.defined and s.is_function and s.value == 0:
            by_sec.setdefault(s.section, s)
    for sec in obj.sections:
        if sec.is_code:
            yield sec, by_sec.get(sec.index)


def kind_of(sec, sym):
    if sec.name != ".text":
        return "funclet"
    if sym is None:
        return "?"
    if sym.name.startswith("_$E"):
        return "$E"
    if sym.storage == coff.SC_STATIC:
        return "static"
    # A COMDAT section's selection (aux record) isn't parsed; template and inline names are what the linker
    # can drop, and those are the decorated names with '$' template args or that the unit doesn't define first.
    return "fn"


def unit_annotations(unit, obj_path, kept_only=False):
    """address -> annotated name, for the entries bound to this unit (its own source and its @unit header copies).
    kept_only: only the copies retail kept from this unit, its own file's entries and those naming it by `@unit`
    (a header entry bound to the unit by default may be another unit's copy)."""
    out = {}
    own = decomp.source_files().get(unit)
    for u in decomp.load_units({unit: obj_path}, decomp.source_files()).values():
        if u.name == unit:
            for e in u.entries:
                if kept_only and Path(e.path) != own and getattr(e, "bind", None) != unit:
                    continue
                out[e.addr] = e.decorated or e.name
    return out


def known_addresses(names):
    """name -> address for every annotated function or datum (decorated and qualified spellings) and every claim."""
    out = dict(decomp.extra_symbols())
    for addr, n in names.items():
        out.setdefault(n, addr)
        q = decomp.qualified(n) if n.startswith("?") else None
        if q:
            out.setdefault(q, addr)
    return out


def targets_agree(img, obj, sec, addr, known, depth=2):
    """True unless a relocation names a symbol with a known address that retail's operand doesn't reach. A target
    defined in the unit's own code (a `jmp` stub's initialiser body) must match retail's bytes where retail points."""
    for off, symidx, typ in sec.relocs:
        sym = obj.symbols.get(symidx)
        if sym is None:
            continue
        raw = img.read(addr + off, 4)
        if raw is None:
            return False
        value = int.from_bytes(raw, "little")
        if typ == 0x14:     # IMAGE_REL_I386_REL32
            value = (addr + off + 4 + value) & 0xFFFFFFFF
        target = None
        if sym.storage != coff.SC_STATIC and not sym.name.startswith("_$E"):
            # unit-local names (initialisers, statics) repeat across units: never look them up globally
            target = known.get(sym.name)
            if target is None:
                q = decomp.qualified(sym.name)
                target = known.get(q) if q else None
        if target is not None:
            if value != target:
                return False
            continue
        if depth and sym.defined and sym.value == 0 and obj.section(sym.section).is_code:
            other = obj.section(sym.section)
            body, mask = masked_body(other, 0, other.size)
            if not same(img, value, body, mask) or not targets_agree(img, obj, other, value, known, depth - 1):
                return False
    return True


def cmd_layout(argv):
    unit = unit_name(argv[0])
    obj_path, obj = load_obj(unit)
    img = pe.Image(decomp.target_image())
    own = unit_annotations(unit, obj_path)
    own_by_name = {}
    for addr, n in own.items():
        own_by_name.setdefault(n, []).append(addr)
    secs = []
    for sec, sym in code_sections(obj):
        kind = kind_of(sec, sym)
        name = sym.name if sym else sec.name
        if kind == "funclet" or sec.size == 0:
            continue
        body, mask = masked_body(sec, 0, sec.size)
        anno = own_by_name.get(name) or own_by_name.get(decomp.qualified(name)) or []
        secs.append((kind, name, sec, body, mask, anno))
    if len(argv) < 2:
        for kind, name, sec, body, mask, anno in secs:
            tag = " [0x%s]" % ", 0x".join("%X" % a for a in sorted(anno)) if anno else ""
            print("%-7s %5X  %s%s" % (kind, sec.size, name, tag))
        return 0
    lo = int(argv[1], 16)
    hi = int(argv[2], 16) if len(argv) > 2 else lo + 0x1000
    names = all_annotations()
    known = known_addresses(names)
    placed = set()
    cursor, unknown = lo, None

    def flush(upto):
        if unknown is not None:
            inside = ["0x%X %s" % (a, names[a]) for a in sorted(names) if unknown <= a < upto]
            print("%08X ??      %5X  %s" % (unknown, upto - unknown, "; ".join(inside) if inside else "(not this unit)"))

    while cursor < hi:
        hits = [x for x in secs if x[3][:1] == (img.read(cursor, 1) or b"") and same(img, cursor, x[3], x[4])
                and targets_agree(img, obj, x[2], cursor, known)]
        if not hits:
            if unknown is None:
                unknown = cursor
            cursor += 16
            continue
        flush(cursor)
        unknown = None
        # Prefer the section annotated here, then the longest body (a short jmp stub matches anywhere).
        hits.sort(key=lambda x: (cursor not in x[5], -len(x[3])))
        kind, name, sec, body, mask, anno = hits[0]
        placed.add(name)
        other = names.get(cursor)
        note = ""
        if cursor in anno:
            note = " [annotated]"
        elif other:
            note = " = %s" % other
        alts = "" if len(hits) == 1 else "  (also %s)" % ", ".join(h[1] for h in hits[1:4])
        print("%08X %-7s %5X  %s%s%s" % (cursor, kind, sec.size, name, note, alts))
        cursor += (sec.size + sec.alignment - 1) // sec.alignment * sec.alignment
    flush(cursor)
    rest = [x for x in secs if x[1] not in placed]
    if rest:
        print("not in 0x%X-0x%X:" % (lo, hi))
        for kind, name, sec, body, mask, anno in rest:
            tag = " [0x%s]" % ", 0x".join("%X" % a for a in sorted(anno)) if anno else ""
            print("         %-7s %5X  %s%s" % (kind, sec.size, name, tag))
    return 0


SELECTIONS = {1: "nodup", 2: "any", 3: "same", 4: "exact", 5: "assoc", 6: "largest"}


def section_selections(data):
    """{section number: COMDAT selection} from a COFF object's section symbols (aux byte 14 of a section definition)."""
    import struct
    _, nsec, _, symptr, nsym, _, _ = struct.unpack_from("<HHIIIHH", data, 0)
    out, i = {}, 0
    while i < nsym:
        raw = data[symptr + 18 * i:symptr + 18 * (i + 1)]
        value, secnum, _, storage, naux = struct.unpack_from("<IhHBB", raw, 8)
        if storage == coff.SC_STATIC and value == 0 and naux and 0 < secnum <= nsec:
            aux = data[symptr + 18 * (i + 1):symptr + 18 * (i + 2)]
            if len(aux) == 18 and aux[14]:
                out.setdefault(secnum, aux[14])
        i += 1 + naux
    return out


def rising_run(addrs):
    """Indices of a longest strictly rising subsequence of addrs (None entries skipped)."""
    tails, tail_idx, prev = [], [], {}
    for i, a in enumerate(addrs):
        if a is None:
            continue
        k = bisect.bisect_left(tails, a)
        if k == len(tails):
            tails.append(a)
            tail_idx.append(i)
        else:
            tails[k] = a
            tail_idx[k] = i
        prev[i] = tail_idx[k - 1] if k else None
    out, i = [], tail_idx[-1] if tail_idx else None
    while i is not None:
        out.append(i)
        i = prev[i]
    return sorted(out)


def show(name):
    """A decorated name as `Class::Method` when it undecorates, else as it is."""
    return (decomp.qualified(name) if name.startswith("?") else None) or name


def order_rows(secs, run):
    """[(mark, kind, selection, name, address, note)]: each section in object order, `!` when it is off the run, with
    the run's neighbours retail places it between."""
    on = set(run)
    placed = sorted((secs[i][4], secs[i][3]) for i in run)
    rows = []
    for i, (kind, sel, sec, name, addr) in enumerate(secs):
        note = ""
        if addr is not None and i not in on:
            k = bisect.bisect_left(placed, (addr, ""))
            before = placed[k - 1][1] if k else "(start)"
            after = placed[k][1] if k < len(placed) else "(end)"
            note = "retail: after %s, before %s" % (show(before), show(after))
        rows.append(("!" if note else " ", kind, sel, name, addr, note))
    return rows


def cmd_order(argv):
    import argparse
    ap = argparse.ArgumentParser(prog="comdat.py order")
    ap.add_argument("unit")
    ap.add_argument("--all", action="store_true", help="list every section, not only those out of order")
    ap.add_argument("--statics", action="store_true", help="also list the `_$E` stubs (ordered as their own group)")
    ap.add_argument("--search", action="store_true", help="find unannotated COMDAT copies in retail by their bytes")
    a = ap.parse_args(argv)
    unit = unit_name(a.unit)
    obj_path, obj = load_obj(unit)
    sel = section_selections(obj_path.read_bytes())
    own = unit_annotations(unit, obj_path, kept_only=True)
    own_by_name = {}
    for addr, n in own.items():
        own_by_name.setdefault(n, []).append(addr)
    img = pe.Image(decomp.target_image()) if a.search else None
    secs = []
    for sec, sym in code_sections(obj):
        kind = kind_of(sec, sym)
        if kind == "funclet" or sec.size == 0 or sym is None:
            continue
        name = sym.name
        addrs = own_by_name.get(name) or own_by_name.get(decomp.qualified(name)) or []
        addr = min(addrs) if addrs else None
        if addr is None and img is not None and sec.flags & coff.SCN_LNK_COMDAT and not name.startswith("_$E"):
            hits = find_copies(img, sec)
            addr = hits[0] if len(hits) == 1 else None
        secs.append((kind, SELECTIONS.get(sel.get(sec.index), "-"), sec, name, addr))
    # the `_$E` stubs trail the unit's functions in retail (docs/levers/statics.md): each group is ordered on its own
    groups = (("functions", [x for x in secs if x[0] != "$E"]), ("static initialisers", [x for x in secs if x[0] == "$E"]))
    print("%s: %d code sections" % (unit, len(secs)))
    for label, group in groups:
        if not group:
            continue
        run = rising_run([x[4] for x in group])
        rows = order_rows(group, run)
        print("  %s: %d, %d with a retail address, %d of them in retail's order" % (
            label, len(group), sum(1 for x in group if x[4] is not None), len(run)))
        if label != "functions" and not a.statics:
            continue
        for mark, kind, s_, name, addr, note in rows:
            if a.all or mark == "!":
                print("%s %-7s %-6s %-10s %s%s" % (mark, kind, s_, "0x%X" % addr if addr is not None else "-", show(name),
                                                "  " + note if note else ""))
    return 0


def find_copies(img, sec):
    """Retail addresses (16-byte aligned) whose bytes equal the section's, relocation fields masked."""
    body, mask = masked_body(sec, 0, sec.size)
    while body and body[-1] in (0x90, 0xCC) and mask[-1]:
        body, mask = body[:-1], mask[:-1]
    if len(body) < 8:
        return []
    lo, hi = img.text[1], img.text[1] + img.text[2]
    blob = img.read(lo, hi - lo)
    pattern = re.compile(b"".join(re.escape(body[i:i + 1]) if mask[i] else b"." for i in range(len(body))), re.S)
    return [lo + m.start() for m in pattern.finditer(blob) if m.start() % 16 == 0]


def cmd_search(argv):
    unit = unit_name(argv[0])
    obj_path, obj = load_obj(unit)
    img = pe.Image(decomp.target_image())
    text_lo, text_hi = img.text[1], img.text[1] + img.text[2]
    lo = int(argv[1], 16) if len(argv) > 1 else text_lo
    hi = int(argv[2], 16) if len(argv) > 2 else text_hi
    annotated = {e.addr for u in decomp.load_units({unit: obj_path}, decomp.source_files()).values() for e in u.entries}

    blob = img.read(lo, hi - lo) if hi > lo else None   # None when the range leaves the image's file bytes
    rows = []
    for s in obj.symbols.values():
        if not s.defined or not s.is_function:
            continue
        sec = obj.section(s.section)
        if not sec.is_code or not sec.flags & coff.SCN_LNK_COMDAT:
            continue
        a, b = obj.function_extent(s)
        if b - a < 8:
            continue
        body, mask = masked_body(sec, a, b)
        first = next((i for i, m in enumerate(mask) if m), None)
        if first is None:
            continue
        if blob is not None:
            # one pass over the range: the body as a pattern, any byte where a relocation sits
            pattern = b"".join(re.escape(body[i:i + 1]) if mask[i] else b"." for i in range(len(body)))
            pattern = re.compile(pattern, re.S)
            hits, m = [], pattern.search(blob)
            while m:   # from the byte after each hit: bodies can overlap
                if m.start() % 16 == 0 and lo + m.start() < hi - len(body):
                    hits.append(lo + m.start())
                m = pattern.search(blob, m.start() + 1)
        else:
            hits = []
            for start in range(lo, hi - len(body), 16):
                r = img.read(start, len(body))
                if r is None or r[first] != body[first]:
                    continue
                if all(mask[i] == 0 or r[i] == body[i] for i in range(len(body))):
                    hits.append(start)
        if hits:
            rows.append((s.name, len(body), hits))

    for name, size, hits in sorted(rows, key=lambda r: r[2][0]):
        marks = ["0x%X%s" % (h, " (annotated)" if h in annotated else "") for h in hits]
        print("%-6d %s" % (size, name))
        print("       " + ", ".join(marks))
    if not rows:
        print("no COMDAT function of %s found in 0x%X-0x%X" % (unit, lo, hi))
    return 0


def main(argv):
    if not argv:
        sys.exit(__doc__)
    if argv[0] == "order":
        return cmd_order(argv[1:])
    if argv[0] == "layout":
        if len(argv) < 2:
            sys.exit(__doc__)
        return cmd_layout(argv[1:])
    return cmd_search(argv)


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
