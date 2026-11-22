"""Write build/ghidra_data.csv ("address,name,decorated") for every global the project knows: the DATA annotations
under src/ and the data entries of config/symbols.txt, for tools/ghidra/ApplyStbcData.java.

A C++ global's decorated name ('?g_kLocalizationManager@@3VTGLocalizationManager@@A') is taken from the compiled
objects in build/obj/ when the annotation doesn't give it: Ghidra's demangler reads the global's type from it.

    python tools/decomp.py build              (once, for build/obj/)
    python tools/ghidra/export_data.py        then run ApplyStbcData.java in Ghidra on build/ghidra_data.csv
"""
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import coff  # noqa: E402
import decomp  # noqa: E402


def decorated_data_names():
    """{qualified name: decorated name} of the C++ data the compiled objects define or reference."""
    out = {}
    for obj in (ROOT / "build" / "obj").rglob("*.obj"):
        for s in coff.Coff(obj.read_bytes()).symbols.values():
            n = s.name
            # '3' after the scope is a global variable, '2' a static member: '?name@scope@@2TYPE'
            if n.startswith("?") and not n.startswith("??") and ("@@3" in n or "@@2" in n):
                q = decomp.qualified(n)
                if q:
                    out.setdefault(q, n)
    return out


def main():
    img = decomp.pe.Image(decomp.target_image())
    text = img.text
    decorated = decorated_data_names()
    rows = {}
    entries = decomp.header_entries()
    for unit, path in decomp.source_files().items():
        entries += decomp.scan(unit, path)
    for e in entries:
        if e.kind != "DATA" or not e.name:
            continue
        dec = e.decorated if e.decorated and e.decorated.startswith("?") else decorated.get(e.name, "")
        rows[e.addr] = (decomp.qualified(e.decorated) if e.decorated and not e.name else e.name, dec)
    for name, addr in decomp.extra_symbols().items():
        sec = img.section_of(addr)
        # imports keep the names Ghidra's PE loader gave them
        if addr in rows or sec is None or sec == text or name.startswith("__imp_") or name.startswith("_imp_"):
            continue
        rows[addr] = (decomp.qualified(name) or name, name if name.startswith("?") else "")
    out = ROOT / "build" / "ghidra_data.csv"
    with open(out, "w", encoding="utf-8", newline="\n") as f:
        f.write("# address,name,decorated: globals of stbc-decomp\n")
        for addr in sorted(rows):
            name, dec = rows[addr]
            f.write("0x%X,%s,%s\n" % (addr, name, dec))
    print("%d globals written to %s (%d with a decorated name)" % (len(rows), out, sum(1 for _, d in rows.values() if d)))


if __name__ == "__main__":
    main()
