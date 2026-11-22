"""Write build/ghidra_names.csv ("address,name,decorated") for every annotated function, from build/report.json.
The decorated name of a C++ function lets ApplyStbcNames.java apply its signature through Ghidra's demangler.

Run `python tools/decomp.py build` first, then `python tools/ghidra/export_names.py`, then run
tools/ghidra/ApplyStbcNames.java in Ghidra's Script Manager on that CSV and save the program.
"""
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import decomp  # noqa: E402

# A definition the name scan misread (a K&R return type on the name line, for example) falls back to the symbol.
NOT_NAMES = {"int", "void", "char", "long", "short", "unsigned", "float", "double", "static", "const", "struct"}


def ghidra_name(row):
    name = row.get("name")
    if not name or name in NOT_NAMES:
        symbol = row.get("symbol")
        name = (decomp.qualified(symbol) if symbol else None) or symbol
    return name


def main():
    rows = json.load(open(ROOT / "build" / "report.json", encoding="utf-8"))["functions"]
    out = ROOT / "build" / "ghidra_names.csv"
    entries = {}
    for row in rows:
        name = ghidra_name(row)
        if name:
            symbol = row.get("symbol") or ""
            entries[int(row["addr"], 16)] = (name, symbol if symbol.startswith("?") else "")
    annotated = len(entries)
    # Functions claimed in symbols.txt without source yet: their identity is evidence too. A name the simple
    # reading can't give (an operator, a template) is left empty, and the demangler's own name stands.
    img = decomp.pe.Image(decomp.target_image())
    for symbol, addr in decomp.extra_symbols().items():
        if addr not in entries and img.section_of(addr) == img.text:
            entries[addr] = (decomp.qualified(symbol) or "", symbol if symbol.startswith("?") else "")
    with open(out, "w", encoding="utf-8", newline="\n") as f:
        f.write("# address,name,decorated: annotated and claimed functions of stbc-decomp\n")
        for addr in sorted(entries):
            name, symbol = entries[addr]
            f.write("0x%X,%s,%s\n" % (addr, name, symbol))
    print("%d names written to %s (%d annotated, %d claimed in symbols.txt)" % (len(entries), out, annotated,
                                                                                 len(entries) - annotated))


if __name__ == "__main__":
    main()
