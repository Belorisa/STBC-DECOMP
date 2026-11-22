"""Name evidence for placeholders: a report, it never renames anything.

    python tools/names.py ShipClass              # one class: its placeholders and the evidence for a name
    python tools/names.py game/ShipClass         # one unit: the placeholders its .cpp and .h use
    python tools/names.py ShipClass --direct     # only the placeholders with direct evidence
    python tools/names.py --summary              # the whole tree: placeholders by kind and by best evidence
    python tools/names.py --summary --list direct   # ... and every placeholder with that confidence, by class

Placeholders (AGENTS.md, R7): `Unknown<ADDR>` functions, `unknown<OFFSET>` members, `VirtualSlot<NN>` slots and
`UnknownClass<ADDR>` classes. Each is a (class, name) pair: `unknown1C` of ShipClass is not `unknown1C` of TGObject.

Evidence, read mechanically (docs/evidence.md says how far each source is trusted):
- wrapper   direct   a SWIG wrapper in src/game/AppWrap.cpp calls the placeholder method or slot as its one game call
                     (`_wrap_ShipClass_DamageSystem` calls `arg0->Unknown5AF4A0(...)`), or a member wrapper
                     (`_wrap_<Class>_<member>_get`/`_set`) reads or writes the placeholder member;
- accessor  direct   a one-statement accessor `GetX`/`IsX` (`return unknown1C;`) or `SetX` (`unknown1C = x;`) whose own
                     name a wrapper or a script evidences names the member `x` (the project's convention: GetAlertLevel
                     reads alertLevel); `weak` when the accessor's name has no evidence of its own;
- string    direct   a string literal in the function's own body names `<Class>::<Name>`; other literals that read
                     as log or error text are listed as `weak`, without a candidate;
- lineage   lineage  the Gamebryo 1.1.2 headers (C:/Recomp/gamebryo-1.1.2) declare the same class: the placeholder
                     sits between two members or virtuals whose names both declarations share, with as many entries
                     between them on each side, so it takes the Gamebryo entry at its place (`m_kWorldDir` reads as
                     `worldDir`, the project's naming style). Fitted to retail, never taken as is (docs/evidence.md);
- scripts            corroboration only: how many game scripts (C:/Recomp/Star Trek Bridge Commander/scripts, the
                     BC SDK 1.1 .py sources) call `.<candidate>(`; a script alone can't say which placeholder it is.

A candidate that the class (or its bases) already declares under that name is reported as a conflict and drops to
`weak`. Confidence: direct, lineage, weak; `none` when nothing was found.
"""

import argparse
import collections
import hashlib
import os
import pickle
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import decomp  # noqa: E402

SRC = decomp.ROOT / "src"
APPWRAP = SRC / "game" / "AppWrap.cpp"
SCRIPT_DIRS = [Path("C:/Recomp/Star Trek Bridge Commander/scripts"), Path("C:/Recomp/bcsdk-1.1")]
BSIM = Path("C:/Recomp/gamebryo-1.1.2/bsim_stbc.tsv")   # retail functions against the 1.1.2 libraries (BSim)
GAMEBRYO_DIRS = [Path("C:/Recomp/gamebryo-1.1.2/sdk"), Path("C:/Recomp/gamebryo-1.1.2/AppFrameworks")]
CACHE = decomp.BUILD / "names.pkl"

TOKEN = re.compile(r"\b(?:\w*?(Unknown(?:Class)?[0-9A-F]{6}\w*)|(unknown[0-9A-F]+)\b|(VirtualSlot\d+)\w*)")
SOURCE_SUFFIXES = (".h", ".hpp", ".inl", ".c", ".cpp")
CONFIDENCE = ("direct", "lineage", "weak", "none")


def kind_of(token):
    if token.startswith("UnknownClass"):
        return "class"
    if token.startswith("Unknown") or "Unknown" in token:
        return "function"
    if token.startswith("VirtualSlot"):
        return "slot"
    return "member"


def key_kind(key, src):
    """kind_of, with `unknown<N>` split into a data member of the class and a parameter or local (`unknown1`)."""
    k = kind_of(key[1])
    if k == "member" and ("data", key[1]) not in src["order"].get(key[0], ()):
        return "parameter"
    return k


def placeholder_tokens(text):
    """Every placeholder identifier in text: the whole identifier (`Py_Unknown74BBF0`, `VirtualSlot12`)."""
    out = []
    for m in re.finditer(r"\b\w*(?:Unknown(?:Class)?[0-9A-F]{6}|unknown[0-9A-F]+\b|VirtualSlot\d+)\w*", text):
        tok = m.group(0)
        if re.fullmatch(r"\w*Unknown(?:Class)?[0-9A-F]{6}\w*|unknown[0-9A-F]+|VirtualSlot\d+\w*", tok):
            out.append((m.start(), tok))
    return out


def blank(text):
    """text with comments and string/char literals blanked to spaces, lengths and newlines kept."""
    def keep_lines(m):
        return re.sub(r"[^\n]", " ", m.group(0))
    return re.sub(r'//[^\n]*|/\*.*?\*/|"(?:\\.|[^"\\\n])*"|\'(?:\\.|[^\'\\\n])*\'', keep_lines, text, flags=re.S)


CLASS_HEAD = re.compile(r"\b(class|struct|union)\s+(?:\w+_ENTRY\s+|__declspec\s*\([^)]*\)\s*)?(\w+)\s*(:[^{;()]*)?\{")


def match_brace(text, i):
    """Index of the '}' closing the '{' at i."""
    depth = 0
    while i < len(text):
        c = text[i]
        if c == "{":
            depth += 1
        elif c == "}":
            depth -= 1
            if depth == 0:
                return i
        i += 1
    return len(text) - 1


def class_bodies(code):
    """[(name, bases, body start, body end)] of the class/struct/union definitions in blanked code."""
    out = []
    for m in CLASS_HEAD.finditer(code):
        end = match_brace(code, m.end() - 1)
        bases = re.findall(r"(?:public|protected|private|virtual)?\s*(\w+)\s*(?:<[^>]*>)?\s*(?:,|$)",
                           (m.group(3) or ":")[1:].strip())
        out.append((m.group(2), [b for b in bases if b], m.end(), end))
    return out


def members(body):
    """The declarations of a class body at its own depth, in order: [(kind, name, statement)] with kind "data",
    "virtual", "method"; static data, typedefs, friends, enums and nested types are left out."""
    out = []
    i, start, depth, paren = 0, 0, 0, 0
    stmts = []
    while i < len(body):
        c = body[i]
        if c == "(":
            paren += 1
        elif c == ")":
            paren -= 1
        elif c == "{" and paren == 0:
            j = match_brace(body, i)
            head = body[start:i]
            if re.search(r"\)\s*(?:const\s*)?(?::[^;]*)?$", head.strip()) or "(" in head:
                stmts.append(head + "{}")   # an inline function body ends the statement
                i, start = j + 1, j + 1
                continue
            i = j + 1   # a nested type or an initialiser: runs on to its ';'
            continue
        elif c == ";" and paren == 0:
            stmts.append(body[start:i])
            start = i + 1
        elif c == ":" and paren == 0 and re.search(r"\b(?:public|protected|private)\s*$", body[start:i]):
            start = i + 1
        i += 1
    for s in stmts:
        s = " ".join(s.split())
        if not s or re.match(r"(?:typedef|friend|enum|using|template|static|class|struct|union)\b", s):
            continue
        if "(" in s:
            name = re.search(r"(~?\w+|operator\s*\S+?)\s*\(", s)
            if not name:
                continue
            out.append(("virtual" if s.startswith("virtual") else "method", name.group(1), s))
            continue
        decl = re.sub(r"\[[^\]]*\]|=.*$|:\s*\d+\s*$", "", s)   # arrays, initialisers, bit-field widths
        for part in decl.split(","):
            m = re.search(r"(\w+)\s*$", part)
            if m and not re.fullmatch(r"const|unsigned|int|char|long|short|float|double|bool|void", m.group(1)):
                out.append(("data", m.group(1), s))
    return out


def line_of(text, pos):
    return text.count("\n", 0, pos) + 1


def scan_source():
    """The tree's view of placeholders: declarations by (class, token), uses by token, class heads, the accessors
    over placeholder members, function bodies' literals."""
    decls = {}                          # (class, token) -> (kind, file, line)
    uses = collections.defaultdict(set)  # token -> files
    bases = {}                          # class -> [bases]
    declared = collections.defaultdict(set)   # class -> every member/method name it declares
    order = {}                          # class -> [(kind, name)] of its own body, in order
    accessors = collections.defaultdict(list)  # (class, member token) -> [(accessor, file, line)]
    unit_tokens = collections.defaultdict(set)  # unit (path without suffix) -> {(class or None, token)}
    bodies = {}                         # (class, function token) -> (file, line, raw body text)
    for p in sorted(SRC.rglob("*")):
        if p.suffix.lower() not in SOURCE_SUFFIXES or not p.is_file():
            continue
        raw = p.read_text(encoding="latin-1")
        code = blank(raw)
        rel = p.relative_to(decomp.ROOT).as_posix()
        unit = p.relative_to(SRC).with_suffix("").as_posix()
        toks = placeholder_tokens(code)
        for _, tok in toks:
            uses[tok].add(rel)
        spans = class_bodies(code)
        for name, b, s, e in spans:
            if b and name not in bases:
                bases[name] = b
            mem = members(code[s:e])
            if name not in order or len(mem) > len(order[name]):
                order[name] = [(k, n) for k, n, _ in mem]
            declared[name].update(n for _, n, _ in mem)
        for pos, tok in toks:
            inner = [sp for sp in spans if sp[2] <= pos < sp[3]]
            before = code[max(0, pos - 80):pos]
            q = re.search(r"(\w+)\s*::\s*$", before)
            if kind_of(tok) == "class":
                cls = None
            elif q:
                cls = q.group(1)   # `Class::Unknown5AF4A0`: a definition, or a call naming its class
            elif inner and not re.search(r"(?:->|\.)\s*$", before):
                cls = min(inner, key=lambda sp: sp[3] - sp[2])[0]   # declared in (or used bare by) the class body
            else:
                cls = None   # a use through an object (`ship->unknown1C`): its class is the object's type
            unit_tokens[unit].add((cls, tok))
            if cls is not None or kind_of(tok) in ("function", "class"):
                key = (cls, tok)
                # where it is declared: inside the class body, else the first qualified mention
                # (a member's own declaration line, `TGObject *unknown134;`, wins over an accessor's mention)
                declares = bool(inner) and re.match(r"\s*(?:;|\[|,|\()", code[pos + len(tok):pos + len(tok) + 4])                     is not None and not re.search(r"(?:=|return|\(|,)\s*$", before)
                old = decls.get(key)
                if old is None or (inner and not old[3]) or (declares and not old[4]):
                    decls[key] = (kind_of(tok), rel, line_of(code, pos), bool(inner), declares)
        # function bodies of placeholder functions (their literals are evidence), qualified definitions only
        for m in re.finditer(r"\b(\w+)\s*::\s*(\w*Unknown[0-9A-F]{6}\w*)\s*\([^;{]*\)\s*(?:const\s*)?\{", code):
            end = match_brace(code, m.end() - 1)
            bodies[(m.group(1), m.group(2))] = (rel, line_of(code, m.start()), raw[m.end():end])
        # accessors over placeholder members, inline in a class or qualified out of line
        for m in re.finditer(r"\b(?:(\w+)\s*::\s*)?((?:Get|Is|Set)\w+)\s*\(([^()]*)\)\s*(?:const\s*)?\{([^{}]*)\}", code):
            body = " ".join(m.group(4).split())
            g = re.fullmatch(r"return\s+(?:this\s*->\s*)?(unknown[0-9A-F]+)\s*;", body)
            s = re.fullmatch(r"(?:this\s*->\s*)?(unknown[0-9A-F]+)\s*=\s*(\w+)\s*;", body)
            if not (g or s):
                continue
            if g and m.group(2).startswith("Set") or s and not m.group(2).startswith("Set"):
                continue
            if s and not re.search(r"\b%s\s*$" % re.escape(s.group(2)), m.group(3).strip()):
                continue
            cls = m.group(1)
            if cls is None:
                inner = [sp for sp in spans if sp[2] <= m.start() < sp[3]]
                cls = min(inner, key=lambda sp: sp[3] - sp[2])[0] if inner else None
            if cls:
                accessors[(cls, (g or s).group(1))].append((m.group(2), rel, line_of(code, m.start())))
    owned = {t for c, t in decls if c is not None}
    decls = {k: v for k, v in decls.items() if k[0] is not None or k[1] not in owned}   # a method used via an object
    return dict(decls=decls, uses=dict(uses), bases=bases, declared=dict(declared), order=order,
                accessors=dict(accessors), unit_tokens=dict(unit_tokens), bodies=bodies)


def scan_wrappers():
    """[(wrapper name, arg0 type, line, [(token, statement)])] of the SWIG wrappers that use a placeholder, and the
    set of every wrapper name (each one evidences `<Class>_<Method>`)."""
    raw = APPWRAP.read_text(encoding="latin-1")
    code = blank(raw)
    out, names = [], set()
    for m in re.finditer(r"PyObject\s*\*\s*_wrap_(\w+)\s*\(PyObject\s*\*self,\s*PyObject\s*\*args\)\s*\{", code):
        names.add(m.group(1))
        end = match_brace(code, m.end() - 1)
        body = code[m.end():end]
        toks = placeholder_tokens(body)
        if not toks:
            continue
        arg0 = re.search(r"\b(\w+)\s*\*\s*arg0\s*;", body)
        lines = []
        for pos, tok in toks:
            ls = body.rfind("\n", 0, pos) + 1
            le = body.find("\n", pos)
            lines.append((tok, " ".join(body[ls:le].split())))
        out.append((m.group(1), arg0.group(1) if arg0 else None, line_of(code, m.start()), lines))
    return out, names


def scan_scripts():
    """{identifier: number of script files calling `.identifier(` (.py) or holding it as a name (.pyc)}."""
    count = collections.Counter()
    for d in SCRIPT_DIRS:
        if not d.exists():
            continue
        for root, _, files in os.walk(d):
            for f in files:
                p = Path(root) / f
                if f.endswith(".py"):
                    found = set(re.findall(r"\.(\w+)\s*\(", p.read_text(encoding="latin-1", errors="replace")))
                elif f.endswith(".pyc"):
                    found = {x.decode() for x in re.findall(rb"[A-Za-z_][A-Za-z0-9_]{2,}", p.read_bytes())}
                else:
                    continue
                count.update(found)
    return dict(count)


def gamebryo_name(name):
    """A Gamebryo member name in the project's style: `m_kWorldDir` -> worldDir, `m_fLODAdjust` -> lodAdjust."""
    m = re.match(r"m_(?:[a-z]+)?([A-Z]\w*)$", name) or re.match(r"m_(\w+)$", name)
    if not m:
        return name
    s = m.group(1)
    head = re.match(r"[A-Z]+(?=[A-Z][a-z]|\d|$)", s)
    if head and len(head.group(0)) > 1:
        return head.group(0).lower() + s[len(head.group(0)):]
    return s[0].lower() + s[1:]


def scan_gamebryo():
    """{class: [(kind, name, original name, statement)]} of the Gamebryo 1.1.2 headers' class bodies."""
    out = {}
    for d in GAMEBRYO_DIRS:
        if not d.exists():
            continue
        for root, _, files in os.walk(d):
            for f in files:
                if not f.lower().endswith(".h"):
                    continue
                code = blank((Path(root) / f).read_text(encoding="latin-1", errors="replace"))
                for name, _, s, e in class_bodies(code):
                    if not name.startswith("Ni") or name in out:
                        continue
                    out[name] = [(k, gamebryo_name(n) if k == "data" else n, n, st) for k, n, st in members(code[s:e])]
    return out


def scan_bsim():
    """{retail address: [(Gamebryo function, similarity, significance)]} by rank, from the BSim run over the
    Gamebryo 1.1.2 libraries (docs/evidence.md; the run is described in the Gamebryo BSim notes)."""
    import csv
    out = collections.defaultdict(list)
    if not BSIM.exists():
        return {}
    with open(BSIM, encoding="utf-8", errors="replace") as f:
        for r in sorted(csv.DictReader(f, delimiter="	"), key=lambda r: int(r["rank"])):
            out[int(r["address"], 16)].append((r["match_name"], float(r["similarity"]), float(r["significance"])))
    return dict(out)


def stamp(paths, deep=True):
    """A key over the paths' timestamps and sizes: every file under a directory when deep, else the directory's own
    (the external trees, the scripts and the Gamebryo SDK, don't change between runs)."""
    h = [Path(__file__).stat().st_mtime_ns]
    for p in paths:
        if p.is_dir() and deep:
            for root, _, files in os.walk(p):
                for f in files:
                    st = (Path(root) / f).stat()
                    h.append((f, st.st_mtime_ns, st.st_size))
        elif p.exists():
            st = p.stat()
            h.append((str(p), st.st_mtime_ns, st.st_size))
    return hashlib.sha1(repr(h).encode("utf-8", "replace")).hexdigest()   # hash() of a str varies per process


def load_index():
    """Everything the report reads, cached in build/names.pkl: the source part by src/'s timestamps, the scripts and
    Gamebryo part by theirs."""
    try:
        with open(CACHE, "rb") as f:
            cache = pickle.load(f)
    except Exception:
        cache = {}
    src_key = stamp([SRC])
    ext_key = stamp(SCRIPT_DIRS + GAMEBRYO_DIRS + [BSIM], deep=False)
    touched = False
    if cache.get("src_key") != src_key:
        cache.update(src_key=src_key, source=scan_source(), wrappers=scan_wrappers())
        touched = True
    if cache.get("ext_key") != ext_key:
        cache.update(ext_key=ext_key, scripts=scan_scripts(), gamebryo=scan_gamebryo(), bsim=scan_bsim())
        touched = True
    if touched:
        decomp.BUILD.mkdir(exist_ok=True)
        tmp = CACHE.with_suffix(".tmp%d" % os.getpid())
        with open(tmp, "wb") as f:
            pickle.dump(cache, f, pickle.HIGHEST_PROTOCOL)
        os.replace(tmp, CACHE)
    return cache


def ancestors(cls, bases):
    out, todo = [], [cls]
    while todo:
        c = todo.pop(0)
        if c in out:
            continue
        out.append(c)
        todo.extend(bases.get(c, []))
    return out


def owner_of(token, cls, src):
    """The class among cls and its bases that declares token (the nearest), or None."""
    for c in ancestors(cls, src["bases"]):
        if (c, token) in src["decls"]:
            return c
    return None


def lower_first(s):
    head = re.match(r"[A-Z]+(?=[A-Z][a-z]|\d|$)", s)
    if head and len(head.group(0)) > 1:
        return head.group(0).lower() + s[len(head.group(0)):]
    return s[:1].lower() + s[1:]


def evidence(index):
    """{(class, token): [(confidence, candidate, source, text)]} for every placeholder with some evidence."""
    src, (wrappers, wrapper_names), scripts, gb = index["source"], index["wrappers"], index["scripts"], index["gamebryo"]
    ev = collections.defaultdict(list)

    def declared_in(cls, name):
        return any(name in src["declared"].get(c, ()) for c in ancestors(cls, src["bases"]))

    def add(key, conf, cand, source, text):
        if cand and conf != "weak" and declared_in(key[0], cand):
            conf, text = "weak", "conflict: %s already declares %s; %s" % (key[0], cand, text)
        if cand:
            n = scripts.get(cand, 0)
            if n:
                text += "  [scripts: %d files]" % n
        ev[key].append((conf, cand, source, text))

    # SWIG wrappers
    for wname, arg0, line, toks in wrappers:
        distinct = {t for t, _ in toks}
        getset = re.match(r"(\w+?)_(\w+)_([gs]et)$", wname)
        for tok, stmt in toks:
            cls = None
            if arg0:
                cls = owner_of(tok, arg0, src)
            if cls is None:
                q = re.search(r"(\w+)\s*::\s*%s" % re.escape(tok), stmt)
                cls = q.group(1) if q else None
            if cls is None:
                continue
            key = (cls, tok)
            where = "AppWrap.cpp:%d _wrap_%s" % (line, wname)
            prefix = (arg0 or cls) + "_"
            if getset and kind_of(tok) == "member" and getset.group(1) == (arg0 or cls):
                add(key, "direct", getset.group(2), "wrapper", "%s: %s" % (where, stmt))
                continue
            method = wname[len(prefix):] if wname.startswith(prefix) else None
            call = re.search(r"(?:->|::|\.)\s*%s\s*\(" % re.escape(tok), stmt) or re.match(r"(?:result\s*=\s*\([^)]*\)\s*)?%s\s*\(" % re.escape(tok), stmt)
            if method and call and len(distinct) == 1 and kind_of(tok) in ("function", "slot") \
                    and not wname.startswith(("new_", "delete_")):
                add(key, "direct", method, "wrapper", "%s: %s" % (where, stmt))
            else:
                add(key, "weak", None, "wrapper", "%s uses it: %s" % (where, stmt))
    # accessors over placeholder members
    for (cls, tok), accs in src["accessors"].items():
        for acc, rel, line in accs:
            m = re.match(r"(?:Get|Is|Set)(\w+)", acc)
            if not m or re.match(r"Unknown[0-9A-F]", m.group(1)):
                continue
            named = any(("%s_%s" % (c, acc)) in wrapper_names for c in [cls] + sorted(sub_classes(cls, src["bases"])))
            conf = "direct" if named or scripts.get(acc, 0) else "weak"
            why = "wrapper %s_%s" % (cls, acc) if named else "scripts call .%s(" % acc if scripts.get(acc) else \
                "no wrapper or script names %s" % acc
            add((cls, tok), conf, lower_first(m.group(1)), "accessor", "%s:%d %s (%s)" % (rel, line, acc, why))
    # string literals of placeholder functions' own bodies
    for (cls, tok), (rel, line, body) in src["bodies"].items():
        for lit in re.findall(r'"((?:\\.|[^"\\])*)"', body):
            q = re.search(r"\b%s::(\w+)" % re.escape(cls), lit)
            if q and not re.match(r"Unknown[0-9A-F]", q.group(1)):
                add((cls, tok), "direct", q.group(1), "string", '%s:%d "%s"' % (rel, line, lit[:80]))
            elif re.search(r"[A-Za-z]{3,}\s+[A-Za-z]{2,}", lit):
                add((cls, tok), "weak", None, "string", '%s:%d "%s"' % (rel, line, lit[:80]))
    # Gamebryo lineage: members and virtuals of the same Ni class, aligned on the names both share
    for cls, mine in src["order"].items():
        if cls not in gb or not any(placeholder_tokens(n) for _, n in mine):
            continue
        theirs = gb[cls]
        for kind in ("data", "virtual"):
            a = [n for k, n in mine if k == kind]
            g = [(n, orig, st) for k, n, orig, st in theirs if k == kind]
            gnames = [n for n, _, _ in g]
            for i, n in enumerate(a):
                if not placeholder_tokens(n):
                    continue
                p = next((j for j in range(i - 1, -1, -1) if a[j] in gnames), -1)
                q = next((j for j in range(i + 1, len(a)) if a[j] in gnames), len(a))
                gp = gnames.index(a[p]) if p >= 0 else -1
                gq = gnames.index(a[q]) if q < len(a) else len(g)
                if gq <= gp or q - p != gq - gp or (p < 0 and q >= len(a)):
                    continue
                cand, orig, st = g[gp + (i - p)]
                if placeholder_tokens(cand):
                    continue
                anchors = "between %s and %s" % (a[p] if p >= 0 else "the start", a[q] if q < len(a) else "the end")
                add((cls, n), "lineage", cand, "lineage", "Gamebryo 1.1.2 %s::%s (%s), %s" % (cls, orig, st[:60],
                                                                                            anchors))
    # BSim against the Gamebryo 1.1.2 libraries: the top candidate of the same class, clearly ahead of any rival
    # (the thresholds of the Gamebryo BSim notes: similarity >= 0.8 and significance >= 15 here, a rival 0.05 behind)
    for (cls, tok) in src["decls"]:
        m = re.search(r"Unknown([0-9A-F]{6})", tok)
        if not cls or not m or kind_of(tok) != "function":
            continue
        cands = index.get("bsim", {}).get(int(m.group(1), 16), [])
        if not cands:
            continue
        name, sim, sig = cands[0]
        rival = next((c for c in cands[1:] if c[0] != name), None)
        if not name.startswith(cls + "::") or sim < 0.8 or sig < 15 or (rival and sim - rival[1] < 0.05):
            continue
        add((cls, tok), "lineage", name.split("::")[-1], "bsim", "BSim %s similarity %.2f significance %.1f%s" % (
            name, sim, sig, "; next %s %.2f" % (rival[0], rival[1]) if rival else ""))
    return ev


def sub_classes(cls, bases):
    """Every class derived from cls, directly or not."""
    subs = {}
    for c, bs in bases.items():
        for b in bs:
            subs.setdefault(b, set()).add(c)
    out, todo = set(), [cls]
    while todo:
        for c in subs.get(todo.pop(), ()):
            if c not in out:
                out.add(c)
                todo.append(c)
    return out


def best(items):
    if not items:
        return "none"
    return min((x[0] for x in items), key=CONFIDENCE.index)


def placeholders_of(target, src):
    """[(class, token)] the target names: a class's own placeholders, or every placeholder a unit's files use."""
    if "/" in target:
        keys = set()
        for unit, toks in src["unit_tokens"].items():
            if unit == target:
                keys |= toks
        # resolve a member/slot token used in a unit to the class that declares it, when only one does
        by_tok = collections.defaultdict(list)
        for c, t in src["decls"]:
            by_tok[t].append(c)
        out = set()
        for c, t in keys:
            if c is not None and (c, t) in src["decls"]:
                out.add((c, t))
            elif len(by_tok.get(t, [])) == 1:
                out.add((by_tok[t][0], t))
            elif c is None and kind_of(t) == "class":
                out.add((None, t))
        return sorted(out, key=lambda k: (k[0] or "", k[1]))
    return sorted((c, t) for c, t in src["decls"] if c == target)


def shadows(cls, tok, cand, src):
    """[(file, lines)] of the files using the placeholder and naming its class (or declaring it) where the candidate
    already occurs as a word: renaming there can be captured by a local or a parameter of that name."""
    out = []
    for rel in sorted(src["uses"].get(tok, ())):
        text = (decomp.ROOT / rel).read_text(encoding="latin-1")
        if cls and not re.search(r"\b%s\b" % re.escape(cls), text):
            continue
        n = sum(1 for line in blank(text).splitlines() if re.search(r"\b%s\b" % re.escape(cand), line))
        if n:
            out.append((rel, n))
    return out


def report(target, index, ev, direct_only):
    src = index["source"]
    keys = placeholders_of(target, src)
    if not keys:
        print("no placeholders found for %s" % target)
        return 1
    tally = collections.Counter()
    for key in keys:
        items = ev.get(key, [])
        conf = best(items)
        tally[conf] += 1
        if direct_only and conf != "direct":
            continue
        cls, tok = key
        d = src["decls"].get(key)
        where = "%s:%d" % (d[1], d[2]) if d else "?"
        print("%s::%s  %s  (%s; the spelling occurs in %d files)  best: %s" % (
            cls, tok, key_kind(key, src), where, len(src["uses"].get(tok, ())), conf))
        for c, cand, source, text in sorted(items, key=lambda x: CONFIDENCE.index(x[0])):
            print("    %-7s %-28s %-8s %s" % (c, cand or "-", source, text[:150]))
            if cand and c != "weak":
                for rel, n in shadows(cls, tok, cand, src):
                    print("            the name %s already occurs on %d lines of %s: a local or parameter there "
                          "would shadow it (write this->%s)" % (cand, n, rel, cand))
    print("%s: %d placeholders; best evidence: %s" % (target, len(keys), ", ".join(
        "%s %d" % (c, tally[c]) for c in CONFIDENCE if tally[c])))
    return 0


def summary(index, ev, listing):
    src = index["source"]
    table = collections.defaultdict(collections.Counter)
    for key in src["decls"]:
        table[key_kind(key, src)][best(ev.get(key, []))] += 1
    print("%-9s %7s %7s %7s %7s %7s" % (("kind",) + ("all",) + CONFIDENCE))
    total = collections.Counter()
    for k in ("function", "member", "slot", "parameter", "class"):
        t = table[k]
        total.update(t)
        print("%-9s %7d %7d %7d %7d %7d" % ((k, sum(t.values())) + tuple(t[c] for c in CONFIDENCE)))
    print("%-9s %7d %7d %7d %7d %7d" % (("all", sum(total.values())) + tuple(total[c] for c in CONFIDENCE)))
    by_source = collections.Counter(s for items in ev.values() for c, _, s, _ in items if c == best(items))
    print("evidence behind the best confidence: " + ", ".join("%s %d" % kv for kv in by_source.most_common()))
    if listing:
        per = collections.defaultdict(list)
        for key in sorted(src["decls"], key=lambda k: (k[0] or "", k[1])):
            items = ev.get(key, [])
            if best(items) == listing:
                per[key[0]].append((key[1], [x for x in items if x[0] == listing][0]))
        for cls in sorted(per, key=lambda c: (-len(per[c]), c or "")):
            print("%s (%d)" % (cls, len(per[cls])))
            for tok, (_, cand, source, text) in per[cls]:
                print("    %-24s %-28s %-8s %s" % (tok, cand or "-", source, text[:110]))


def main(argv):
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("targets", nargs="*", help="classes or units (game/ShipClass)")
    ap.add_argument("--direct", action="store_true", help="only placeholders with direct evidence")
    ap.add_argument("--summary", action="store_true", help="the whole tree, by kind and best evidence")
    ap.add_argument("--list", choices=CONFIDENCE, help="with --summary: list every placeholder of this confidence")
    args = ap.parse_args(argv)
    if not args.targets and not args.summary:
        ap.print_help()
        return 2
    index = load_index()
    ev = evidence(index)
    if args.summary:
        summary(index, ev, args.list)
    rc = 0
    for t in args.targets:
        rc |= report(t, index, ev, args.direct)
    return rc


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
