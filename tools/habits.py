"""Measure the original programmers' habits in matched source, per module.

    python tools/habits.py              # the table, from build/report.json and the source
    python tools/habits.py --json       # the same as JSON

Only functions that MATCH are read: their spelling is proven to compile to retail's bytes, so a habit counted there
is the original team's (or, for surviving upstream code, upstream's). Generated SWIG wrappers are left out. Each
habit is a pair of spellings of the same thing; the table gives both counts and the preferred side's share, which
the matcher uses as a prior: the spelling to try first. The bytes still decide.
"""

import json
import re
import sys
from collections import defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import decomp  # noqa: E402
import funcsource as L  # noqa: E402

ANNOT = re.compile(r"//\s*FUNCTION:\s*STBC\s+0x([0-9A-Fa-f]+)")
OPERAND = r"[A-Za-z_][\w.]*(?:->\w+)*(?:\[[^\[\]]+\])?"

# Each habit: (name, the first spelling, the second spelling), as regexes over a body without comments and strings.
HABITS = [
    ("this on the left of ==/!=", r"\bthis\s*[=!]=", r"[=!]=\s*this\b"),
    ("pointer test: !p / p versus p == NULL / p != NULL",
     r"\bif\s*\(\s*!?\s*%s\s*\)" % OPERAND, r"(?:==|!=)\s*(?:NULL|0)\s*\)"),
    ("constant on the left (0 == x)", r"\(\s*(?:NULL|0|\d+)\s*[=!]=", r"[=!]=\s*(?:NULL|\d+)\s*\)"),
    ("else if chain versus separate ifs that return",
     r"\belse\s+if\b", r"\breturn\b[^;]*;\s*\}?\s*\n\s*if\s*\("),
    ("|| in one condition versus two ifs with the same return",
     r"\bif\s*\([^)]*\|\|", r"\bif\s*\([^)]*\)\s*\{?\s*return\s+(\w+)\s*;\s*\}?\s*\n\s*if\s*\([^)]*\)\s*\{?\s*return\s+\1\s*;"),
    ("&& in one condition versus a nested if",
     r"\bif\s*\([^)]*&&", r"\bif\s*\([^)]*\)\s*\{\s*\n\s*if\s*\("),
    ("ternary ?: versus if/else assigning one value",
     r"\?[^:;]+:", r"\bif\s*\([^)]*\)\s*\{?\s*\n?\s*(\w+)\s*=[^;]+;\s*\}?\s*else\s*\{?\s*\n?\s*\1\s*="),
    ("return cond ? true : false versus return cond",
     r"return\s+[^;]*\?\s*true\s*:\s*false", r"return\s+[^;?]*(?:==|!=|<|>|&&|\|\|)[^;?]*;"),
    ("pre-increment ++i versus post-increment i++ (statements and for headers)",
     r"(?:^|[;(]\s*)(?:\+\+|--)[A-Za-z_]", r"[A-Za-z_\]]\s*(?:\+\+|--)\s*[;)]"),
    ("for loop versus while loop", r"\bfor\s*\(", r"\bwhile\s*\("),
    ("compound assignment x += y versus x = x + y",
     r"\b(%s)\s*[+\-*/|&]=" % OPERAND, r"\b(%s)\s*=\s*\1\s*[+\-*/|&]" % OPERAND),
]


def strip(text):
    text = re.sub(r"//[^\n]*", "", text)
    text = re.sub(r"/\*.*?\*/", "", text, flags=re.S)
    return re.sub(r'"(\\.|[^"\\])*"', '""', text)


def module_of(path):
    rel = path.relative_to(decomp.SRC).as_posix()
    if rel.startswith("game/AppWrap"):
        return None
    return rel.split("/")[0]


def bodies():
    """(module, body) of every matched function, its annotation found by address."""
    report = decomp.load_report()
    matched = {int(f["addr"], 16) for f in report["functions"] if f["status"] == "MATCH"}
    out = []
    for p in sorted(decomp.SRC.rglob("*")):
        if p.suffix.lower() not in (".c", ".cpp", ".h", ".hpp", ".inl"):
            continue
        module = module_of(p)
        if module is None:
            continue
        lines = p.read_text(encoding="latin-1").splitlines(keepends=True)
        for i, line in enumerate(lines):
            m = ANNOT.search(line)
            if not m or int(m.group(1), 16) not in matched:
                continue
            try:
                s, e = L.function_span(lines, i)
            except SystemExit:
                continue
            out.append((module, strip("".join(lines[s:e]))))
    return out


def measure():
    counts = defaultdict(lambda: defaultdict(lambda: [0, 0]))
    funcs = defaultdict(int)
    for module, body in bodies():
        funcs[module] += 1
        for name, first, second in HABITS:
            counts[module][name][0] += len(re.findall(first, body, re.M))
            counts[module][name][1] += len(re.findall(second, body, re.M))
        returns = len(re.findall(r"\breturn\b", body))
        counts[module]["several returns versus one (functions)"][0 if returns > 1 else 1] += 1
    return funcs, counts


def main(argv):
    funcs, counts = measure()
    if "--json" in argv:
        print(json.dumps({m: {h: v for h, v in hs.items()} for m, hs in counts.items()}, indent=1))
        return 0
    for module in sorted(funcs, key=lambda m: -funcs[m]):
        print("%s  (%d matched functions)" % (module, funcs[module]))
        for name, (a, b) in counts[module].items():
            total = a + b
            if total < 5:
                continue
            first, second = name.split(" versus ") if " versus " in name else (name, "the other spelling")
            lead = first if a >= b else second
            print("  %-80s %5d : %-5d  -> %s %.0f%%" % (name[:80], a, b, lead[:40], 100.0 * max(a, b) / total))
        print()
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
