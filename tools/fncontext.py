"""What a worker is shown about one function: the unit's compiler, the class declaration, a matched example to copy,
the retail assembly with names, the diff cut to the rows that differ with the kind of difference that comes first,
and the docs/levers/ entries closest to its PARKED note. Used by tools/packet.py, tools/openwork.py and
tools/annotate_inlines.py.
"""

import math
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import decomp  # noqa: E402
import funcsource as L  # noqa: E402

ROOT = L.ROOT


# ---------------------------------------------------------------------------------------------------------------------
# the unit and the class

COMPILERS = {
    None: "Microsoft Visual C++ 6.0 SP5 (cl 12.00.8804, c2 build 8966)",
    "rtm": "Microsoft Visual C++ 6.0 RTM (cl 12.00.8168)",
    "sp4": "Microsoft Visual C++ 6.0 SP4 (c1xx 8867, c2 8799)",
    "pp": "Microsoft Visual C++ 6.0 SP5 front end with the Processor Pack back end (c2 13.00.9044)",
}



def compiler_description(unit, path):
    cfg = decomp.unit_config(path.relative_to(ROOT).as_posix())
    flags = list(cfg["flags"]) + list(cfg.get("overrides", {}).get(unit, []))
    return "%s, C++98; flags: %s" % (COMPILERS.get(decomp.compiler_of(path), "unknown"), " ".join(flags))



def unit_functions(unit, rows):
    """{addr: (start, end, text)} of every annotated function defined in the unit's own source file."""
    path = L.SRC / (unit + ".cpp")
    if not path.exists():
        path = L.SRC / (unit + ".c")
    if not path.exists():
        return {}
    lines = path.read_text(encoding="utf-8", errors="replace").splitlines(keepends=True)
    pat = re.compile(r"//\s*FUNCTION:\s*STBC\s+0x0*([0-9A-Fa-f]+)\b")
    out = {}
    for i, line in enumerate(lines):
        m = pat.search(line)
        if not m:
            continue
        try:
            s, e = L.function_span(lines, i)
        except SystemExit:
            continue
        out[int(m.group(1), 16)] = "".join(lines[s:e]).replace("\r\n", "\n")
    return out



def pick_example(rows, unit, addr, target_text):
    """A MATCH function of the same unit for the model to copy: the same class when possible, the closest in
    length among bodies with at least four statements."""
    texts = unit_functions(unit, rows)
    cls = (next((f["name"] for f in rows if int(f["addr"], 16) == addr), "") or "").split("::")[0]
    cands = []
    for f in rows:
        a = int(f["addr"], 16)
        if f["unit"] != unit or f["status"] != "MATCH" or a == addr or a not in texts:
            continue
        body = texts[a]
        if body.count(";") < 4:
            continue
        same = (f["name"] or "").split("::")[0] == cls
        cands.append((0 if same else 1, abs(len(body) - len(target_text)), f["name"], body))
    if not cands:
        return None, None
    cands.sort(key=lambda c: (c[0], c[1]))
    return cands[0][2], cands[0][3]



def method_of_decorated(name):
    """(class, method) of a decorated member name; the constructor and destructor take the class's own name."""
    m = re.match(r"\?(\?[0-9_A-Z]|\w+)@(\w+)@@", name)
    if not m:
        return None, None
    op, cls = m.group(1), m.group(2)
    if op == "?0":
        return cls, cls
    if op == "?1":
        return cls, "~" + cls
    if op.startswith("?"):
        return cls, None   # operators and compiler-generated members: not drafted
    return cls, op



def class_declaration(cls):
    """(header path, the class's declaration text) from the headers under src/."""
    pat = re.compile(r"^(?:class|struct)\s+%s\b[^;]*?\{" % re.escape(cls), re.M)
    for p in sorted(L.SRC.rglob("*.h")):
        text = p.read_text(encoding="utf-8", errors="replace")
        m = pat.search(text)
        if not m:
            continue
        depth, i = 0, m.end() - 1
        while i < len(text):
            depth += {"{": 1, "}": -1}.get(text[i], 0)
            if depth == 0:
                return p, text[m.start():text.find(";", i) + 1]
            i += 1
    return None, None



def declaration(decl, method):
    """The match of the method's declaration in a class declaration: one line that names it before any '(' or '='
    (so not a call inside an inline body) and ends in ';' with no body (an inline body needs a header annotation,
    not a draft). None when the class defines it inline or doesn't declare it."""
    return re.search(r"^[^\n;{}()=]*\b%s\s*\([^;{\n]*\)[^;{\n]*;" % re.escape(method), decl, re.M)



def home_unit(cls, rows):
    """The unit that defines most of the class's annotated functions, outside headers. The generated SWIG wrappers
    (game/AppWrap) define inline members of many classes but own none of them: never a home. A free function
    (cls None, a SWIG wrapper's C name) has none either."""
    if not cls:
        return None
    count = {}
    for r in rows:
        if (r["name"] or "").startswith(cls + "::") and not r["unit"].startswith("(") and r["unit"] != "game/AppWrap":
            count[r["unit"]] = count.get(r["unit"], 0) + 1
    return max(count, key=count.get) if count else None



def retail_asm(img, addr, size, names):
    code = img.read(addr, size)
    out = []
    for a, mnem, ops in decomp.disasm(code, addr):
        line = "%06X  %-6s %s" % (a, mnem, ops)
        for t in re.findall(r"0x[0-9a-f]{5,8}", ops):
            if int(t, 16) in names:
                line += "    ; " + names[int(t, 16)]
        out.append(line)
    return "\n".join(out)



def example_function(cls, unit, rows):
    """A matched function of the class from its unit, medium length: the house style to copy."""
    texts = unit_functions(unit, rows)
    cands = []
    for r in rows:
        a = int(r["addr"], 16)
        if r["status"] == "MATCH" and (r["name"] or "").startswith(cls + "::") and a in texts:
            body = texts[a]
            if 2 <= body.count(";") and len(body) < 1800:
                cands.append((abs(len(body) - 700), r["name"], body))
    if not cands:
        return None, None
    cands.sort()
    return cands[0][1], cands[0][2]


# ---------------------------------------------------------------------------------------------------------------------
# the diff: which compiler decision differs, and where

def best_view(addr, limit, diagnosed=True, raw=None):
    """(diff, diagnosis, lever keys) for the function's current compiled state: the diagnosed compact diff, or the
    plain diff cut to `limit` rows (with --no-diagnosis, or when nothing classifies). `raw` is the text of
    `decomp.py diff` when the caller already has it (packet.py's one load for many functions)."""
    if raw is None:
        raw = L.run(["diff", "0x%X" % addr])
    if diagnosed:
        summary, compact, keys = diagnose(raw, limit=min(limit, 40))
        if compact:
            return compact, summary, keys
    return diff_text(addr, limit, raw), None, []



def diff_text(addr, limit, raw=None):
    out = (raw if raw is not None else L.run(["diff", "0x%X" % addr])).splitlines()
    marked = [i for i, l in enumerate(out) if re.search(r"\s[<>|]\s", l) or l.startswith("status")]
    if len(out) <= limit or not marked:
        return "\n".join(out[:limit])
    keep = set()
    for i in marked:
        keep.update(range(max(0, i - 3), min(len(out), i + 4)))
    keep.update(range(0, 2))
    picked, last = [], -2
    for i in sorted(keep):
        if i != last + 1:
            picked.append("   ...")
        picked.append(out[i])
        last = i
    return "\n".join(picked[:limit])



DIFF_ROW = re.compile(r"^(?:([0-9A-F]{8})  (.*?))?\s*([<>|])?\s*(?:([0-9A-F]{8})  (.*))?$")



BRANCH = re.compile(r"^(j\w+|loop\w*)\s+0x[0-9a-f]+$")



REGISTERS = re.compile(r"\b(?:e?[abcd]x|[abcd][lh]|e?[sd]i|e?[sb]p|st\(\d\)|st)\b")



INVERSE = {"je": "jne", "jne": "je", "jz": "jnz", "jnz": "jz", "jl": "jge", "jge": "jl", "jg": "jle", "jle": "jg",
           "jb": "jae", "jae": "jb", "ja": "jbe", "jbe": "ja", "js": "jns", "jns": "js", "jp": "jnp", "jnp": "jp"}



# What each kind of difference usually means in VC6 code, and the kinds of lever that reach it.
DIFF_KINDS = {
    "call": ("a different function is called, or a call is added or missing: an inline accessor versus its body, "
             "or a helper that is called instead of written out (fix this first)", ["accessor", "local"]),
    "inverted": ("a conditional jump is inverted: the if and else bodies are in the other order, or the test is "
                 "negated (if ( !x ) versus if ( x ), an early return versus a nested block)", ["return", "guard", "ternary"]),
    "moved": ("the same instructions in another order: the statements or the operands that compute them are in "
              "another order", ["order", "operand", "declare"]),
    "extra": ("an instruction only in retail: retail keeps or computes a value ours doesn't (a local holding a "
              "pointer or a member's address, a value read again instead of kept)", ["local", "accessor", "order"]),
    "missing": ("an instruction only in ours: ours keeps or computes a value retail doesn't (a local retail writes "
                "in place, an expression ours reads twice)", ["local", "accessor", "order"]),
    "register": ("only the registers differ: the same instructions with other registers; this follows the order "
                 "in which locals and subexpressions are first used", ["declare", "local", "order", "operand"]),
    "other": ("another instruction is chosen: usually another spelling of the same expression (a different type, a "
              "comparison written the other way, ?: versus if/else)", ["ternary", "operand", "local"]),
}



def parse_diff(text):
    """(marker, ours, retail) per instruction row of `decomp.py diff`; ours or retail is None for a one-sided row."""
    rows = []
    for line in text.splitlines()[1:]:
        if line.startswith(("status", "   ...")) or not line.strip():
            continue
        m = DIFF_ROW.match(line.rstrip())
        if not m or not (m.group(1) or m.group(4)):
            continue
        ours = re.sub(r"\s+", " ", m.group(2)).strip() if m.group(1) else None
        retail = re.sub(r"\s+", " ", m.group(5)).strip() if m.group(4) else None
        rows.append((m.group(3) or " ", ours, retail, m.group(1), m.group(4)))
    return rows



def classify_row(ours, retail):
    """The kind of one differing row, or None when it differs only by a branch target (a shift caused elsewhere)."""
    if (ours or retail) in ("nop", "int3") and (ours is None or retail is None):
        return None   # alignment fill after a shorter body
    if ours is None:
        return "call" if retail.startswith("call") else "extra"
    if retail is None:
        return "call" if ours.startswith("call") else "missing"
    om, rm = ours.split(" ")[0], retail.split(" ")[0]
    if BRANCH.match(ours) and BRANCH.match(retail):
        if om == rm:
            return None
        return "inverted" if INVERSE.get(om) == rm else "other"
    if om == "call" or rm == "call":
        return "call"
    if om == rm and REGISTERS.sub("R", ours) == REGISTERS.sub("R", retail):
        return "register"
    return "other"



def diagnose(text, window=3, limit=40):
    """(summary, compact diff, lever keys): what kind of difference comes first and how often each kind occurs, with the levers
    it points to; and the diff cut to the rows that really differ (a branch whose only change is its target, shifted
    by a difference elsewhere, is not one), each with `window` rows around it."""
    rows = parse_diff(text)
    kinds, first = [], None
    only_ours = {}
    only_retail = {}
    for i, (mark, ours, retail, oa, ra) in enumerate(rows):
        if mark == " ":
            continue
        k = classify_row(ours, retail)
        if k is None:
            continue
        if ours is not None and retail is None:
            only_ours.setdefault(REGISTERS.sub("R", ours), []).append(i)
        if retail is not None and ours is None:
            only_retail.setdefault(REGISTERS.sub("R", retail), []).append(i)
        kinds.append((i, k))
    if not kinds:
        return None, None, []
    # An instruction only in ours and the same one only in retail, elsewhere: moved, not added.
    moved = set()
    for norm, idx in only_ours.items():
        if norm in only_retail:
            moved.update(idx + only_retail[norm])
    kinds = [(i, "moved" if i in moved else k) for i, k in kinds]
    first_i, first_k = kinds[0]
    mark, ours, retail, oa, ra = rows[first_i]
    where = ("ours %s `%s`" % (oa, ours) if ours else "ours: nothing") + ", " + \
            ("retail %s `%s`" % (ra, retail) if retail else "retail: nothing")
    counts = {}
    for _, k in kinds:
        counts[k] = counts.get(k, 0) + 1
    order = sorted(counts, key=lambda k: (k != first_k, -counts[k]))
    lines = ["The first real difference (%s): %s." % (where, DIFF_KINDS[first_k][0])]
    if len(counts) > 1 or counts[first_k] > 1:
        lines.append("Differing rows by kind: %s. Branches whose only change is their target are left out: they "
                     "follow from a size change elsewhere." % ", ".join("%s %d" % (k, counts[k]) for k in order))
    keep = set()
    for i, _ in kinds:
        keep.update(range(max(0, i - window), min(len(rows), i + window + 1)))
    out, last = [], -2
    header = text.splitlines()[0] if text else ""
    for i in sorted(keep):
        if i != last + 1:
            out.append("   ...")
        mark, ours, retail, oa, ra = rows[i]
        if mark != " " and classify_row(ours, retail) is None:
            mark = "~"   # differs only by its branch target
        out.append("%-44s %s %s" % ("%s  %s" % (oa, ours) if ours else "", mark, "%s  %s" % (ra, retail) if retail else ""))
        last = i
        if len(out) >= limit:
            out.append("   ... (more rows differ)")
            break
    keys = []   # the levers the kinds point to, the first difference's kind first
    for k in order:
        keys += [x for x in DIFF_KINDS[k][1] if x not in keys]
    return "\n".join(lines), header + "\n" + "\n".join(out), keys


# ---------------------------------------------------------------------------------------------------------------------
# the closest documented levers

LEVERS_DIR = ROOT / "docs" / "levers"



STOPWORDS = set("""the and for with this that than from into retail build source when where which their there
these those what while each only one two not but are was were has have its it's onto over after before then them
they this code here gives give given closed match matched wall walls open park parked""".split())



def lever_entries():
    """(section, text) of every bullet of the docs/levers/ files' sections on levers and bounded walls (README.md,
    the index with the house habits and the evidence sources, is left out)."""
    entries, section, current = [], "", None
    paths = sorted(p for p in LEVERS_DIR.glob("*.md") if p.name != "README.md") if LEVERS_DIR.is_dir() else []
    text = "\n\n".join(p.read_text(encoding="utf-8") for p in paths)
    for line in text.splitlines():
        if line.startswith("#"):
            if current:
                entries.append((section, current))
            section, current = line.lstrip("#").strip(), None
        elif line.startswith("- "):
            if current:
                entries.append((section, current))
            current = line[2:].strip()
        elif current is not None and line.startswith("  ") and line.strip():
            current += " " + line.strip()
        elif not line.strip() and current:
            entries.append((section, current))
            current = None
    if current:
        entries.append((section, current))
    return [(s, t) for s, t in entries if "Evidence sources" not in s and "House habits" not in s]



def words(text):
    return {w for w in re.findall(r"[a-z][a-z0-9_?:/]{3,}", text.lower()) if w not in STOPWORDS}



def documented_levers(note, diff, limit_chars):
    """The docs/levers/ entries closest to a function's PARKED note, best first, within limit_chars. Words are weighed
    by their rarity in the levers (a word every entry uses says nothing); the diff's words are too generic to help."""
    want = words(note or "")
    entries = lever_entries()
    if not want or not entries:
        return None
    bags = [words(text) for _, text in entries]
    df = {}
    for bag in bags:
        for w in bag:
            df[w] = df.get(w, 0) + 1
    scored = []
    for (section, text), bag in zip(entries, bags):
        score = sum(math.log(len(entries) / df[w]) for w in want & bag)
        if score >= 6:   # about two rare words in common
            scored.append((score + (0 if "Walls" in section else 1), text))
    scored.sort(key=lambda t: -t[0])
    out, used = [], 0
    for _, text in scored[:5]:
        if used + len(text) > limit_chars:
            break
        out.append("- " + text)
        used += len(text)
    return "\n".join(out) or None

