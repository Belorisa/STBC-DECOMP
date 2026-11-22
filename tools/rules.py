"""Rule checker for annotated functions (decomp-code-rules, "Making [match] mechanical").

[match] findings make an exact function MATCH-PENDING. Advisory findings are open work that doesn't block a match.
Per-module relaxations come from config/project.json ("relax": rule numbers) and are recorded in AGENTS.md.
A module may name a pristine "upstream" tree and the rules ("upstream_exempt") whose findings are skipped on lines
identical to the upstream file: unchanged surviving source is authentic.
Lines in inactive preprocessor branches (per `cl /E` with the unit's flags) are not checked.
R13 compares each compiled vtable's slot count with config/vtables.txt (a class's functions are flagged when its
vtable has a slot too many or too few). R17's unused-local check covers locals of plain types (numbers, bools,
pointers) declared in a body and never named again; a local initialised by a call is not flagged (the call is real).
R33 (header comment, advisory) looks above the whole stack of consecutive annotation lines (template instances, a
function's copies), and doesn't ask for one on a compiler-generated member (??_G, ??_E, ??_F..., or an annotation above
its class definition) or on a member defined on one line inside its class body.
"""
import copy
import hashlib
import json
import os
import pickle
import re
import sys
import time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

PLACEHOLDER = re.compile(r"\b(?:FUN_|DAT_|PTR_|LAB_|sub_|unk_|unknown[0-9A-F]+\b|VirtualSlot\d|Unknown[0-9A-F]{6}|local_|param_|field_0x|m_Unk|UnkType_|iVar\d|uVar\d|pvVar\d|var\d+\b)\w*")
ADDRESS_LIKE = re.compile(r"\b[A-Za-z_]*?_?(?=[0-9a-fA-F]*[0-9])[0-9a-fA-F]{6,8}\b")
OFFSET_CAST = re.compile(r"\(\s*(?:unsigned\s+)?(?:char|BYTE|byte|uint8_t|int|DWORD)\s*\*\s*\)\s*\(?\s*[\w.\->]+\s*\)?\s*\+\s*(?:0x[0-9A-Fa-f]+|\d+)")
THIS_ARITH = re.compile(r"\(\s*(?:unsigned\s+)?(?:char|BYTE|byte|int|DWORD)\s*\*\s*\)\s*this\b|\bthis\s*\)?\s*\+\s*(?:0x[0-9A-Fa-f]+|\d+)")
ASM = re.compile(r"\b(?:__asm|_asm|_emit|asm\s*\()")
VOLATILE = re.compile(r"\bvolatile\b")
IF_OPEN = re.compile(r"\bif\s*\(")
EMPTY_ELSE = re.compile(r"\belse\s*(?:\{\s*\}|;)")
DO_WHILE0 = re.compile(r"\bwhile\s*\(\s*0\s*\)\s*;")
SELF_ASSIGN = re.compile(r"(?<![=!<>+\-*/%&|^.>\w])\s*([A-Za-z_][\w]*(?:(?:->|\.)[A-Za-z_]\w*)*)\s*=\s*\1\s*;")
PLUS0_TIMES1 = re.compile(r"[+]\s*0(?![\w.])|(?<![(\s,=])\s*\*\s*1(?![\w.])")
PROBE = re.compile(r"\bPROBE\b|//\s*(?i:probe)\b")   # the marker, not an identifier such as a sensor probe
EXTERN = re.compile(r"^\s*extern\b(?!\s*\"(?:C|C\+\+)?\")")   # not a linkage spec (strings are stripped to "")
# A single plain-typed local declaration: numbers, bools and pointers (a class object's constructor is real code).
PLAIN_LOCAL = re.compile(
    r"^\s*(?:const\s+|register\s+)*"
    r"(?:(?:unsigned|signed)\s+)?(?:(?:int|long|short|char|float|double|bool|BOOL|DWORD|WORD|BYTE|UINT|ULONG|size_t|"
    r"unsigned|signed)\b|[A-Za-z_]\w*\s*\*)"
    r"\s*\**\s*([A-Za-z_]\w*)\s*(?:=\s*([^;]*))?;\s*$")
VTABLES_FILE = "config/vtables.txt"
# PARKED notes whose reason the project has since resolved; a function still parked on one of these needs a retry.
STALE_PARKED = [
    (r"processor pack|build 9044|vcpp\.dll|9044", "the Processor Pack back end is in tools/vc6/_pp"),
    (r"NiPoint2.*copy constructor|copy constructor.*NiPoint2", "NiPoint2 has a copy constructor"),
    (r"NiColorA.*operator=", "NiColorA has a member-wise operator="),
    (r"NiPoint3 declares none|NiPoint3::operator-\(\)", "NiPoint3 declares operator-()"),
    (r"RemoveEnd", "NiTArray::RemoveEnd exists"),
    (r"GetDriver|NiSoundSystem::driver", "NiSoundSystem::GetDriver exists"),
    (r"GetShapeData\(\) is protected", "NiTriShape::GetShapeData is public"),
    (r"needs? (?:an? )?NiTList (?:tail|GetTailPos|GetPrev)", "NiTList has the tail-side API"),
]


def strip_comments(text):
    text = re.sub(r"/\*.*?\*/", lambda m: re.sub(r"[^\n]", " ", m.group(0)), text, flags=re.S)
    return "\n".join(l.split("//")[0] if not l.lstrip().startswith("#") else l for l in text.splitlines())


def strip_strings(text):
    return re.sub(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'', '""', text)


def bug_reason(text):
    """True when the BUG note in text (`// BUG:` or `/* BUG: ... */`) says what is wrong (some words after `BUG:` and
    an optional `redundant:`), so that it can excuse the statement under it."""
    m = re.search(r"BUG:\s*(?:redundant:\s*)?(.*)", text, re.S)
    reason = re.sub(r"//|/\*|\*/", " ", m.group(1)) if m else ""
    return re.search(r"[A-Za-z]{2,}", reason) is not None


BUG_MARK = re.compile(r"(?://|/\*)\s*BUG:|^\s*\*\s*BUG:")


def bug_noted(raw_lines, i):
    """A BUG note on line i or in the comment block directly above it: the statement is the original's own bug, kept
    for the byte-exact preservation build (user's ruling), so R17 doesn't count it as fake code. The note is a `//`
    comment or a C comment (`/* BUG: ... */`, the CRT's style), and must say what is wrong: an empty one (`// BUG:`,
    `/* BUG: redundant: */`) excuses nothing."""
    line = raw_lines[i] if i < len(raw_lines) else ""
    if "BUG:" in line:
        return bug_reason(line)
    j, in_block = i - 1, False
    while j >= 0:
        l = raw_lines[j].strip()
        if in_block or l.endswith("*/"):
            if not in_block and "/*" in l and not l.startswith("/*"):
                break   # a statement with a comment after it
            in_block = "/*" not in l   # the block comment runs up to the line that opens it
        elif not l.startswith("//"):
            break
        if BUG_MARK.search(raw_lines[j]):
            # the reason may run on over the comment lines below the marker
            return bug_reason(" ".join(raw_lines[j:i]))
        j -= 1
    return False


def empty_if(line):
    """An if whose balanced condition is followed by ';' or '{ }' on the same line."""
    for m in IF_OPEN.finditer(line):
        depth, i = 1, m.end()
        while i < len(line) and depth:
            depth += {"(": 1, ")": -1}.get(line[i], 0)
            i += 1
        if depth == 0 and re.match(r"\s*(?:;|\{\s*\})", line[i:]):
            return True
    return bool(EMPTY_ELSE.search(line))


_active_cache = {}
USE_CACHE = True   # check_units(cache=False) turns the disk caches off (the gate)


def active_lines(entry):
    """Line numbers of entry's file that the compiler sees (cl /E), or None when preprocessing fails."""
    import decomp
    key = str(entry.path)
    if key not in _active_cache:
        _active_cache[key] = decomp.preprocessed_lines(entry.path, USE_CACHE)
    return _active_cache[key]


_upstream_cache = {}
_file_lines_cache = {}


def file_lines(path):
    """A source file's lines, read once per run."""
    import decomp
    key = str(path)
    if key not in _file_lines_cache:
        _file_lines_cache[key] = decomp.source_text(path).splitlines()
    return _file_lines_cache[key]


def upstream_paths(entry, mod):
    """The pristine upstream files of entry: the template files listed for its unit in "upstream_units" (generated
    units), and the mirrored file under the module's "upstream" tree."""
    import decomp
    paths = [decomp.ROOT / p for p in mod.get("upstream_units", {}).get(entry.unit, [])]
    if mod.get("upstream"):
        paths.append(decomp.ROOT / mod["upstream"] / entry.path.relative_to(decomp.ROOT / mod["path"]))
    return paths


def upstream_lines(entry, mod):
    """Lines of entry's pristine upstream (upstream_paths). An empty set when there is none."""
    paths = upstream_paths(entry, mod)
    for path in paths:
        key = str(path)
        if key not in _upstream_cache:
            text = path.read_text(encoding="latin-1") if path.exists() else ""
            _upstream_cache[key] = {l.rstrip() for l in text.splitlines()}
    if len(paths) == 1:
        return _upstream_cache[str(paths[0])]    # read-only: callers only test membership
    lines = set()
    for path in paths:
        lines |= _upstream_cache[str(path)]
    return lines


def module_of(unit, project):
    best = None
    for u in project["units"]:
        p = u["path"]
        if ("src/" + unit).startswith(p + "/") and (best is None or len(p) > len(best["path"])):
            best = u
    return best or {}


def body_of(entry):
    """(first line number, text) of the definition after the annotation and comment lines."""
    lines = entry.text.splitlines()
    i = 1
    while i < len(lines) and (lines[i].strip().startswith("//") or not lines[i].strip()):
        i += 1
    return entry.line + i, "\n".join(lines[i:])


def type_definitions(root):
    """Type name -> set of normalised bodies, across every header and source file."""
    defs = {}
    pat = re.compile(r"\b(struct|class|union)\s+(\w+)\s*(?::[^{;]*)?\{")
    sources = [p for p in (root / "src").rglob("*") if p.is_file() and (
        p.suffix.lower() in (".h", ".hpp", ".inl", ".c", ".cpp") or not p.suffix)]   # C++ headers: <xstring>
    for p in sources:
        text = strip_comments(p.read_text(encoding="latin-1"))
        found = []   # (name, start, body start, body end)
        for m in pat.finditer(text):
            depth, i = 0, m.end() - 1
            while i < len(text):
                if text[i] == "{":
                    depth += 1
                elif text[i] == "}":
                    depth -= 1
                    if depth == 0:
                        break
                i += 1
            found.append((m.group(2), m.start(), m.end(), i))
        for name, start, body_start, body_end in found:
            # A nested type is named by its enclosing classes: basic_istream::sentry is not basic_ostream::sentry.
            outer = [n for n, s, bs, be in found if bs <= start < be]
            qualified = "::".join(outer + [name])
            body = re.sub(r"\s+", " ", text[body_start:body_end])
            defs.setdefault(qualified, {}).setdefault(body, set()).add(p.relative_to(root).as_posix())
    # R1's row: the same type defined in two files with different bodies (variants under #if in one file are not).
    return {name.split("::")[-1]: files for name, files in defs.items()
            if len(files) > 1 and len(set().union(*files.values())) > 1}


def cached_type_definitions(root):
    """type_definitions, from build/typedefs.pkl while no source file's name, timestamp or size has changed (and
    this checker hasn't)."""
    import decomp
    h = hashlib.sha1(Path(__file__).read_bytes())
    for p in sorted(p for p in (root / "src").rglob("*") if p.is_file()):
        st = p.stat()
        h.update(("%s %d %d\n" % (p, st.st_mtime_ns, st.st_size)).encode("utf-8", "replace"))
    key = h.hexdigest()
    path = decomp.BUILD / "typedefs.pkl"
    try:
        with open(path, "rb") as f:
            old = pickle.load(f)
        if old[0] == key:
            return old[1]
    except Exception:
        pass
    defs = type_definitions(root)
    decomp.BUILD.mkdir(exist_ok=True)
    tmp = path.with_suffix(".tmp%d" % os.getpid())
    with open(tmp, "wb") as f:
        pickle.dump((key, defs), f, pickle.HIGHEST_PROTOCOL)
    os.replace(tmp, path)
    return defs


# An annotation line, or a note that sits under one (decomp.py's ANNOTATION, PARKED, DRAFT, PATCHED and the rest):
# a stack of these is walked over when looking for the header comment above it.
ANNOTATION_LINE = re.compile(r"^\s*//\s*(?:(?:FUNCTION|DATA):\s*STBC\b|"
                             r"(?:PARKED|DRAFT|PATCHED|MATCH-PENDING)\s*:|ORIGINAL-ASM\b)")
CLASS_HEAD = re.compile(r"^\s*(?:template\s*<[^>]*>\s*)?(?:class|struct)\s+\w+\s*(?::[^;(]*)?\{?\s*$")
TEMPLATE_ONLY = re.compile(r"^\s*template\s*<[^>]*>\s*$")
# Compiler-generated members: vcall thunk, vbase destructor, vector and scalar deleting destructors, default-argument
# and copy constructor closures.
COMPILER_MEMBER = re.compile(r"\?\?_[9DEFGO]")
HEADER_WINDOW = 3   # lines above an annotation stack that may hold (the end of) its header comment


def stack_top(lines, i):
    """0-based index of the first line of the run of annotation lines that holds line i (0-based)."""
    while i > 0 and ANNOTATION_LINE.match(lines[i - 1]):
        i -= 1
    return i


def header_window(entry):
    """The lines R33 reads for entry's header comment: those above its whole stack of consecutive annotation lines
    (template instances, a function's copies), so a comment over the stack documents each annotation in it."""
    lines = file_lines(entry.path)
    top = stack_top(lines, entry.line - 1)
    return lines[max(0, top - HEADER_WINDOW):top]


def definition_line(entry):
    """The first line of entry's definition, below the annotation and its notes, or None (a template instance's
    annotation in a stack has no definition under it)."""
    lines = file_lines(entry.path)
    k = entry.line
    while k < len(lines) and lines[k].strip().startswith("//"):
        k += 1
    if k >= len(lines) or not lines[k].strip():
        return None
    if TEMPLATE_ONLY.match(lines[k]) and k + 1 < len(lines):
        return lines[k] + " " + lines[k + 1].strip()   # `template <class T>` over its class or function head
    return lines[k]


def header_comment_missing(entry):
    """R33: no comment above the definition's annotation stack. A compiler-generated member (annotated above its class
    definition) and a member defined on one line inside its class body need none."""
    if entry.decorated and COMPILER_MEMBER.match(entry.decorated):
        return False   # no source of its own: the class's declaration is the documentation
    d = definition_line(entry)
    if d is not None:
        if CLASS_HEAD.match(strip_comments(d)):
            return False   # an implicit destructor or ??_G, annotated above its class (template)
        body = strip_strings(strip_comments(d))
        if d[:1] in (" ", "\t") and "{" in body and body.rstrip().endswith("}") and \
                body.count("{") == body.count("}"):
            return False   # an in-class one-liner: `int GetX() { return x; }`
    return not any(x.strip().startswith(("/*", "*", "//")) and not ANNOTATION_LINE.match(x)
                   for x in header_window(entry))


def check_function(entry, mod, dup_types):
    import decomp
    if entry.decorated and decomp.STATIC_INIT.match(entry.decorated):
        # A compiler-generated `_$E` initialiser is annotated by a bare comment line with no definition under it:
        # there is no source to check, and the text below the annotation is another definition's. Only a reviewer's
        # `// MATCH-PENDING:` line in the annotation's own stack is read (user's ruling, 2026-11-22: HullClass's
        # size-pragma quad).
        lines = entry.text.splitlines()
        out = []
        if lines:
            try:
                raw = entry.path.read_text(encoding="latin-1").splitlines()
            except OSError:
                raw = []
            i = entry.line - 1
            if 0 <= i < len(raw):
                top = stack_top(raw, i)
                k = top
                while k < len(raw) and ANNOTATION_LINE.match(raw[k]):
                    m = re.match(r"\s*//\s*MATCH-PENDING:\s*(R\d+)\s*[,:]?\s*(.*)", raw[k])
                    if m:
                        out.append({"rule": m.group(1), "tier": "match",
                                    "text": "marked MATCH-PENDING: " + m.group(2).strip()})
                    k += 1
        return out
    relax = set(mod.get("relax", []))
    advisory_off = set(mod.get("advisory_off", [])) | set(mod.get("unit_advisory_off", {}).get(entry.unit, []))
    out = []

    upstream = upstream_lines(entry, mod)
    upstream_exempt = set(mod.get("upstream_exempt", []))
    raw_line = ""
    noted_bug = False   # the line being checked carries a BUG note (see bug_noted)

    def add(rule, tier, text):
        if rule in relax and tier == "match":
            return
        if rule == "R17" and noted_bug:
            return   # the original's documented bug, kept for the preservation build

        if tier == "advisory" and rule in advisory_off:
            return
        if rule in upstream_exempt and raw_line.strip() and raw_line.rstrip() in upstream:
            return   # the line is unchanged surviving source
        out.append({"rule": rule, "tier": tier, "text": text})

    first, raw = body_of(entry)
    raw_lines = raw.splitlines()
    code = strip_strings(strip_comments(raw))
    lines = code.splitlines()
    active = active_lines(entry)
    if active is not None:
        # Lines in inactive #if branches aren't compiled, so they aren't checked.
        lines = [l if first + k in active or l.lstrip().startswith("#") else "" for k, l in enumerate(lines)]
        code = "\n".join(lines)
    original_asm = "ORIGINAL-ASM" in entry.text.splitlines()[0] or re.search(r"//\s*ORIGINAL-ASM", entry.text)

    if ASM.search(code) and not original_asm:
        add("R18", "match", "inline assembly in the body")

    for i, l in enumerate(lines):
        raw_line = raw_lines[i] if i < len(raw_lines) else ""
        noted_bug = bug_noted(raw_lines, i)
        if VOLATILE.search(l) and "volatile:" not in raw_line:
            add("R17", "match", "volatile without a '// volatile: <evidence>' note: " + l.strip())
        if empty_if(l):
            add("R17", "match", "empty if/else body: " + l.strip())
        if l.strip() == ";":
            prev = next((x for x in reversed(lines[:i]) if x.strip()), "")
            if not re.match(r"^\s*(for|while)\b", prev) and not re.search(r"(case\b.*|\w+)\s*:\s*$", prev):
                add("R17", "match", "bare ';' statement")
        if DO_WHILE0.search(l) and not l.rstrip().endswith("\\"):
            add("R17", "match", "do { } while (0) outside a macro")
        m = SELF_ASSIGN.search(l)
        if m and "==" not in l:
            add("R17", "match", "self-assignment: " + l.strip())
        if PLUS0_TIMES1.search(l):
            add("R17", "match", "'+ 0' or '* 1': " + l.strip())
        if PROBE.search(raw_line):
            add("R17", "match", "probe statement")
        if OFFSET_CAST.search(l) or THIS_ARITH.search(l):
            add("R4", "match", "offset arithmetic through a cast: " + l.strip())
        if EXTERN.match(l):
            add("R2", "match", "local extern declaration: " + l.strip())
        if re.match(r"^\s*#\s*if\s+0\b", l):
            add("R37", "advisory", "#if 0 block")
    # R17: a plain-typed local that is never named again after its declaration.
    # Only the function's own braces: an inline member's text runs on into its class's member declarations.
    body_start = next((k for k, l in enumerate(lines) if "{" in l), len(lines))
    body_end, depth = len(lines), 0
    for k in range(body_start, len(lines)):
        depth += lines[k].count("{") - lines[k].count("}")
        if depth <= 0:
            body_end = k + 1
            break
    head = " ".join(lines[:body_start + 1])
    is_class = re.search(r"\b(?:class|struct|union)\s+\w+[^(]*\{", head) is not None
    for i in range(body_start + 1, body_end if not is_class else body_start + 1):
        # A compiler-generated member's annotation sits above its class: the braces hold members, not locals.
        m = PLAIN_LOCAL.match(lines[i])
        if not m or re.match(r"^\s*(?:return|delete|case|goto|else|throw|sizeof|typedef)\b", lines[i]):
            continue
        name, init = m.group(1), m.group(2) or ""
        if "(" in init:
            continue   # the call in the initialiser is real code, whatever happens to its result
        rest = "\n".join(lines[:i] + lines[i + 1:body_end])
        if not re.search(r"\b%s\b" % re.escape(name), rest):
            raw_line = raw_lines[i] if i < len(raw_lines) else ""
            noted_bug = bug_noted(raw_lines, i)
            add("R17", "match", "unused local %s: %s" % (name, lines[i].strip()))
    for cls, why in vtable_mismatches().items():
        if entry.name and entry.name.startswith(cls + "::"):
            add("R13", "match", why)
    raw_line = ""
    noted_bug = False
    # A reviewer's verdict on what the checker can't see (an unused local kept to reproduce a frame slot, ...).
    for m in re.finditer(r"//\s*MATCH-PENDING:\s*(R\d+)\s*[,:]?\s*(.*)", entry.text):
        add(m.group(1), "match", "marked MATCH-PENDING: " + m.group(2).strip())
    for name in dup_types:
        if re.search(r"\b%s\b" % re.escape(name), code):
            files = sorted(f for fs in dup_types[name].values() for f in fs)
            add("R1", "match", "type %s defined differently in %s" % (name, ", ".join(files)))
    names = set(PLACEHOLDER.findall(code)) | {x for x in ADDRESS_LIKE.findall(code) if not re.fullmatch(r"\d+", x)}
    names = {n for n in names if not re.fullmatch(r"0x[0-9A-Fa-f]+|[0-9A-Fa-f]+[LlUu]*", n)}
    for n in sorted(names):
        add("R7", "advisory", "placeholder or address-like name " + n)
    # R21: a PARKED note that names a problem the project no longer has (advisory: the note has rotted).
    if entry.parked:
        for pat, why in STALE_PARKED:
            if re.search(pat, entry.parked, re.I):
                add("R21", "advisory", "stale PARKED note (%s): %s" % (why, entry.parked[:100]))
                break
    if header_comment_missing(entry):
        add("R33", "advisory", "no header comment")
    # De-duplicate repeated findings.
    seen, uniq = set(), []
    for f in out:
        k = (f["rule"], f["text"])
        if k not in seen:
            seen.add(k)
            uniq.append(f)
    return uniq


_vtable_cache = None


def vtable_mismatches():
    """{class: problem} for classes whose compiled vtable (a COMDAT ??_7Class@@6B@ in build/obj) has another slot
    count than config/vtables.txt records for retail."""
    global _vtable_cache
    if _vtable_cache is not None:
        return _vtable_cache
    import coff
    import decomp
    _vtable_cache = {}
    path = decomp.ROOT / VTABLES_FILE
    if not path.exists():
        return _vtable_cache
    want = {}
    for line in path.read_text(encoding="utf-8").splitlines():
        m = re.match(r"^\s*(.+?)\s+(0x[0-9A-Fa-f]+)\s+(\d+)\s*(?:#|$)", line)
        if m:
            want[m.group(1)] = int(m.group(3))
    objdir = decomp.BUILD / "obj"
    for obj in objdir.rglob("*.obj") if objdir.exists() else ():
        o = decomp.load_coff(obj)
        for s in o.symbols.values():
            m = re.fullmatch(r"\?\?_7(\w+)@@6B@", s.name)
            if not m or not s.defined or m.group(1) not in want:
                continue
            sec = o.section(s.section)
            if not sec.flags & coff.SCN_LNK_COMDAT:
                continue   # only a COMDAT section holds exactly one vtable, so only it gives the slot count
            slots = (sec.size - s.value) // 4
            if slots != want[m.group(1)]:
                _vtable_cache[m.group(1)] = "vtable has %d slots, retail %d (%s, %s)" % (
                    slots, want[m.group(1)], VTABLES_FILE, obj.relative_to(objdir).as_posix())
    return _vtable_cache


def warm_active_lines(paths):
    """Preprocess every file the check will ask active_lines about, in parallel: one cl /E per file, run one after
    another, took over a minute of a full build."""
    import concurrent.futures
    import os
    import decomp
    todo = sorted({str(p): p for p in paths if str(p) not in _active_cache}.items())
    with concurrent.futures.ThreadPoolExecutor(max_workers=os.cpu_count()) as ex:
        for (key, _), lines in zip(todo, ex.map(lambda kv: decomp.preprocessed_lines(kv[1], USE_CACHE), todo)):
            _active_cache[key] = lines


RESULT_CACHE = "rulecache.pkl"   # in build/: check_function's findings by a digest of everything it reads
_digest_memo = {}


def _memo_digest(key, make):
    if key not in _digest_memo:
        _digest_memo[key] = hashlib.sha1(make().encode("latin-1", "replace")).hexdigest()
    return _digest_memo[key]


def result_key(entry, mod, dup):
    """A digest of every input of check_function for this entry: the checker itself, the definition's text and
    place, the module's settings, the pristine upstream, the active lines over the definition, the comment lines
    above it, the duplicated types and the vtable findings of its class."""
    import decomp
    active = active_lines(entry)
    last = entry.line + entry.text.count("\n") + 1
    parts = (
        _memo_digest("rules.py", lambda: Path(__file__).read_text(encoding="latin-1")),
        entry.text, entry.name, entry.unit, entry.parked, entry.line,
        _memo_digest(("mod", mod.get("path")), lambda: json.dumps(mod, sort_keys=True)),
        [_memo_digest(("upstream", str(p)), lambda: decomp.source_text(p) if p.exists() else "")
         for p in upstream_paths(entry, mod)],
        None if active is None else [n for n in range(entry.line, last + 1) if n in active],
        header_window(entry), definition_line(entry),
        _memo_digest("dup", lambda: repr(sorted((n, sorted(f for fs in files.values() for f in fs))
                                                for n, files in dup.items()))),
        sorted((c, w) for c, w in vtable_mismatches().items() if entry.name and entry.name.startswith(c + "::")),
    )
    return hashlib.sha1(repr(parts).encode("latin-1", "replace")).hexdigest()


def check_units(units, cache=True):
    """{address: findings} for every annotated function of the units. With cache, a function whose inputs are all
    unchanged (result_key) takes its findings from build/rulecache.pkl, and the active lines come from
    build/objcache; the gate checks everything afresh."""
    global USE_CACHE
    import decomp
    USE_CACHE = cache
    dup = cached_type_definitions(decomp.ROOT) if cache else type_definitions(decomp.ROOT)
    warm_active_lines(e.path for u in units.values() for e in u.entries if e.kind == "FUNCTION")
    store_path = decomp.BUILD / RESULT_CACHE
    store = {}
    if cache and store_path.exists():
        try:
            with open(store_path, "rb") as f:
                store = pickle.load(f)
        except Exception:
            store = {}   # unreadable: check everything again
    today = int(time.time() // 86400)
    touched = False
    findings = {}
    src = decomp.ROOT / "src"
    for u in units.values():
        unit_mod = module_of(u.name, decomp.PROJECT)
        for e in u.entries:
            if e.kind != "FUNCTION":
                continue
            # the file's own module, not the binding unit's: a header COMDAT bound with @unit to another module's
            # unit (cpprt's <xstring> kept by game/SetClass) keeps its module's upstream exemptions
            try:
                mod = module_of(e.path.relative_to(src).as_posix(), decomp.PROJECT) or unit_mod
            except ValueError:
                mod = unit_mod
            if cache:
                key = result_key(e, mod, dup)
                hit = store.get(key)
                if hit is None:
                    hit = store[key] = [today, check_function(e, mod, dup)]
                    touched = True
                elif hit[0] != today:
                    hit[0] = today
                    touched = True
                f = copy.deepcopy(hit[1])   # the rows carry their findings; the store keeps its own
            else:
                f = check_function(e, mod, dup)
            if f:
                findings[e.addr] = f
    if cache and touched:
        store = {k: v for k, v in store.items() if v[0] >= today - 14}   # the cache prunes by last use
        tmp = store_path.with_suffix(".tmp%d" % os.getpid())
        with open(tmp, "wb") as f:
            pickle.dump(store, f, pickle.HIGHEST_PROTOCOL)
        os.replace(tmp, store_path)
    return findings


SWIG_CONSTANT = re.compile(r"^\s*#\s*define\s+((?:ET|WC|MWT|GET)_[A-Z0-9_]+)\b", re.M)
STATIC_HELPER = re.compile(r"^static\s+(?:inline\s+)?[\w\s\*&:<>,]+?\b(\w+)\s*\([^)]*\)\s*(?:const\s*)?\{", re.M)


def file_findings(root, selected_units):
    """Findings that belong to files rather than functions: (path, line, rule, text), all advisory.

    - R11: a SWIG constant (ET_, WC_, MWT_, GET_) defined in a .cpp instead of its class header, or defined in more
      than one file.
    - R16: a file-static helper whose body is identical in two units: the original had one inline in a header."""
    files = [p for p in (root / "src" / "game").rglob("*") if p.suffix in (".h", ".cpp")]
    files += [p for p in (root / "src" / "netimmerse").rglob("*") if p.suffix in (".h", ".cpp")]

    def unit_of(p):
        return p.relative_to(root / "src").with_suffix("").as_posix()

    def wanted(p):
        return not selected_units or unit_of(p) in selected_units

    out, constants, helpers = [], {}, {}
    for p in files:
        if unit_of(p) == "game/AppWrap":
            continue   # the generated SWIG table defines every constant by design
        text = p.read_text(encoding="utf-8", errors="replace")
        for m in SWIG_CONSTANT.finditer(text) if unit_of(p).startswith("game/") else ():   # SWIG names the game's
            line = text.count("\n", 0, m.start()) + 1
            constants.setdefault(m.group(1), []).append((p, line))
            if p.suffix == ".cpp" and wanted(p):
                out.append((p, line, "R11", "SWIG constant %s defined in a .cpp: declare it in its class header"
                            % m.group(1)))
        if p.suffix != ".cpp":
            continue
        for m in STATIC_HELPER.finditer(text):
            i, depth = m.end(), 1
            while depth and i < len(text):
                depth += {"{": 1, "}": -1}.get(text[i], 0)
                i += 1
            body = re.sub(r"\s+", "", strip_comments(text[m.end():i]))
            if len(body) > 60:
                helpers.setdefault(body, []).append((p, text.count("\n", 0, m.start()) + 1, m.group(1)))
    for name, where in constants.items():
        if len({p for p, _ in where}) > 1:
            for p, line in where:
                if wanted(p):
                    others = ", ".join(sorted({q.relative_to(root).as_posix() for q, _ in where if q != p}))
                    out.append((p, line, "R11", "%s is also defined in %s: one definition, in its owner's header"
                                % (name, others)))
    for where in helpers.values():
        if len({p for p, _, _ in where}) > 1:
            for p, line, name in where:
                if wanted(p):
                    others = ", ".join(sorted({q.relative_to(root).as_posix() for q, _, _ in where if q != p}))
                    out.append((p, line, "R16", "static helper %s has the same body in %s: one inline in a shared "
                                "header" % (name, others)))
    return sorted(out, key=lambda f: (str(f[0]), f[1]))


def main(selected):
    import decomp

    class U:
        pass

    units = {}
    for name, path in decomp.source_files().items():
        if selected and name not in [re.sub(r"\.(c|cpp)$", "", re.sub(r"^src/", "", s.replace("\\", "/"))) for s in selected]:
            continue
        u = U()
        u.name, u.entries = name, decomp.scan(name, path)
        if u.entries:
            units[name] = u
    findings = check_units(units)
    nmatch = nadv = npatched = 0
    patched = []
    for u in sorted(units.values(), key=lambda x: x.name):
        for e in u.entries:
            # A PATCHED function (binary-patched in retail, written as the original) is checked like any other and
            # listed apart: its findings are printed under its tag and counted neither as open nor in the exit code.
            tag = " (PATCHED)" if getattr(e, "patched", None) else ""
            if tag:
                patched.append(e)
            for f in findings.get(e.addr, []):
                print("%s:%d: 0x%X %s%s: %s [%s] %s" % (e.path.relative_to(decomp.ROOT).as_posix(), e.line, e.addr,
                                                        e.name, tag, f["rule"], f["tier"], f["text"]))
                if tag:
                    npatched += 1
                    continue
                nmatch += f["tier"] == "match"
                nadv += f["tier"] != "match"
    for e in patched:
        print("%s:%d: 0x%X %s: PATCHED, listed apart: %s" % (e.path.relative_to(decomp.ROOT).as_posix(), e.line,
                                                             e.addr, e.name, e.patched[:120]))
    for path, line, rule, text in file_findings(decomp.ROOT, set(units) if selected else set()):
        print("%s:%d: (file) %s [advisory] %s" % (path.relative_to(decomp.ROOT).as_posix(), line, rule, text))
        nadv += 1
    print("rule findings: %d [match], %d advisory%s" % (
        nmatch, nadv, "; PATCHED listed apart: %d functions, %d findings" % (len(patched), npatched) if patched else ""))
    return 1 if nmatch else 0
