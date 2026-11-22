"""Fast variant trials: compile a few hand-written spellings of one function against retail, about 0.3 s each.

    python tools/trial.py 0x59D5B0 build/trial/forcetogroup-a.cpp            # one variant first: does it move?
    python tools/trial.py 0x59D5B0 a.cpp b.cpp c.cpp                         # then a set (at most 20)
    python tools/trial.py 0x59D5B0 a.cpp --show                              # with the diff of each against retail

A variant is a file the worker writes by hand: the function's whole definition as it would stand in the source (from
its return type to its closing brace; comment lines before it are kept). The tool preprocesses the function's unit
once with its own flags (`cl /E`, cached in build/trial/ by the unit's fingerprint), puts each variant in place of the
definition in the preprocessed text, compiles that with the unit's flags, and compares the function's bytes with
retail's, relocation fields masked on both sides. A variant that uses a macro (the preprocessed text has none) is
compiled from a scratch copy of the unit's source instead (a variant naming an identifier the preprocessed text never
spells goes there directly), at the cost of the unit's own compile. The unmodified preprocessed text is
compiled too, as the base; a base that differs from build/obj's object is reported (the fast path then doesn't
reproduce the unit and its results mean nothing). A function defined in a header (an inline, a class member) is
compiled from a scratch copy of the header with the variant in place, searched first on the include path (the
header's other files hard-linked beside it), the unit's own source unchanged. A unit built with /YX (its module's "auto_pch" switch) compiles every
variant, and the base, from the scratch copy: its code depends on where the precompiled include block ends.

The bounds are the user's (docs/rulings.md, "Fast variant trials are a bounded tool", 2026-11-18), and the tool
enforces them:

- variants are hand-written files: there is no generator, and a variant file that repeats another is refused;
- a set runs only after one variant was shown to move the function's output: a run with a single variant records,
  in build/trial/<address>.json, whether it moved the bytes, keyed by the function's current source; a run with more
  than one is refused until such a record exists for the same source;
- at most 20 variants a run;
- the output ranks the variants by score, and an exact one is marked as a clue, not an answer: the spelling still
  needs its evidence (docs/evidence.md), and the source change is made and matched through `decomp.py match`.

Method after C:/Recomp/x87-fast-variant-compile.py (an x87 lane's one-off script) and bw1-decomp's
tools/tiebreak-probe.py and variant-sweep.py (openblack's Black & White decompilation, CC0), which compile hand-made
variants of a unit; this one is bounded and compiles from the preprocessed unit.
"""
import argparse
import concurrent.futures
import difflib
import hashlib
import json
import os
import re
import shutil
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import coff  # noqa: E402
import decomp  # noqa: E402
import pe  # noqa: E402

WORK = decomp.BUILD / "trial"
MAX_VARIANTS = 20


# --- pure parts (tools/tests/test_trial.py) ---------------------------------------------------------------------------

def definition_span(lines, entry):
    """(first, last) 1-based source lines of the definition under an annotation: from the first line after the
    annotation's comment stack to the definition's end."""
    i = entry.line          # 0-based index of the line after the annotation
    while i <= entry.end and lines[i].lstrip().startswith("//"):
        i += 1
    return i + 1, entry.end + 1


def braced_end(lines, first):
    """The 1-based line where the braces opened from line `first` (1-based) close, or None. A definition inside a
    class ends there, where the annotation scan's end (the next line starting with a brace) runs on to the class's
    end. Strings, character literals and comments are skipped."""
    depth, opened = 0, False
    in_comment = False
    for n in range(first - 1, len(lines)):
        l, i = lines[n], 0
        while i < len(l):
            c = l[i]
            if in_comment:
                if l.startswith("*/", i):
                    in_comment, i = False, i + 1
            elif l.startswith("//", i):
                break
            elif l.startswith("/*", i):
                in_comment, i = True, i + 1
            elif c in "\"'":
                j = i + 1
                while j < len(l) and l[j] != c:
                    j += 2 if l[j] == "\\" else 1
                i = j
            elif c == "{":
                depth, opened = depth + 1, True
            elif c == "}":
                depth -= 1
                if opened and depth == 0:
                    return n + 1
            i += 1
    return None


def preprocessed_span(text, path, first, last):
    """Indices of the lines of preprocessed `text` that came from lines first..last of path (cl /E's #line
    directives), or [] when none did."""
    want = os.path.normcase(str(path))
    out, current, n = [], None, 0
    for i, l in enumerate(text.split("\n")):
        m = decomp.LINE_DIRECTIVE.match(l) if l.startswith("#") else None
        if m:
            n = int(m.group(1))
            current = os.path.normcase(m.group(2).replace("\\\\", "\\"))
            continue
        if current == want and first <= n <= last:
            out.append(i)
        n += 1
    return out


def substitute(text, span, variant):
    """The preprocessed text with the lines of span replaced by the variant (on the first of them; the rest blank, so
    the line count stays)."""
    lines = text.split("\n")
    for k, i in enumerate(span):
        lines[i] = variant if k == 0 else ""
    return "\n".join(lines)


def masked(code, relocs, start, retail):
    """(ours, retail's) bytes with every relocation field of ours zeroed on both sides."""
    ours, theirs = bytearray(code), bytearray(retail or b"")
    for off, _, _ in relocs:
        o = off - start
        if 0 <= o < len(ours):
            ours[o:o + 4] = b"\0" * 4
            theirs[o:o + 4] = b"\0" * 4
    return bytes(ours), bytes(theirs[:len(ours)])


def normalised(ins):
    """Instructions with addresses left out: a branch or call target, an absolute address and a zero relocation field
    (ours, unrelocated) all read alike, so ours and retail's compare without their relocations."""
    out = []
    for _, m, o in ins:
        if m.startswith("j") or m == "call":
            o = "T" if re.fullmatch(r"0x[0-9a-f]+", o) else o
        o = re.sub(r"0x[0-9a-f]{5,}", "A", o)
        o = re.sub(r"\[(0x[0-9a-f]+|\d+)\]", "[A]", o)
        o = re.sub(r"\[(e\w\w\*\d)( \+ 0)?\]", r"[\g<1> + A]", o)
        o = re.sub(r"(^|, )0$", r"\g<1>A", o)   # an immediate that is a relocation field in ours
        out.append("%s %s" % (m, o))
    return out


def nav_score(ours, retail, addr):
    """The navigation score of ours (relocation fields zero) against retail's bytes, addresses normalised."""
    a = normalised(decomp.disasm(ours, addr))
    b = normalised(decomp.disasm(retail, addr))
    return round(100.0 * difflib.SequenceMatcher(None, a, b, autojunk=False).ratio(), 1)


def check_gate(state, src_digest, n_variants):
    """None when a run of n variants may go ahead, else the refusal. A set needs a recorded variant that moved the
    output of this source."""
    if n_variants > MAX_VARIANTS:
        return "%d variants: at most %d a run (docs/rulings.md, \"Fast variant trials are a bounded tool\")" % (
            n_variants, MAX_VARIANTS)
    if n_variants <= 1:
        return None
    if not state or state.get("src") != src_digest or not state.get("moved"):
        return ("run one variant first and show that it moves the output (a set runs only after that, for the same "
                "source of the function)")
    return None


def rank(results):
    """Results sorted by score, best first; failures last."""
    return sorted(results, key=lambda r: (r["score"] is None, -(r["score"] or 0), r["name"]))


# --- compiling ---------------------------------------------------------------------------------------------------------

def find_entry(addr):
    import ctxscan
    return ctxscan.find_entry(addr)


def preprocess(unit, path):
    """The unit's preprocessed text, cached by its fingerprint."""
    WORK.mkdir(parents=True, exist_ok=True)
    cached = WORK / (decomp.unit_fingerprint(unit, path) + ".i")
    if cached.exists():
        return cached.read_text(encoding="latin-1")
    rc, out = decomp.vc6.run("cl", ["/E", *decomp.cl_args(unit, path, pch=False)], compiler=decomp.compiler_of(path))
    if rc != 0:
        raise SystemExit("cl /E failed for %s" % unit)
    # vc6.run joins stdout and stderr: drop the compiler's own lines (the file name, D4025 warnings)
    text = "\n".join(l for l in out.split("\n") if not re.match(r"^(\S+\.(c|cpp)|Command line warning .*)$", l))
    tmp = cached.with_suffix(".tmp%d" % os.getpid())
    tmp.write_text(text, encoding="latin-1")
    os.replace(tmp, cached)
    return text


def i_flags(unit, path):
    """The unit's flags for compiling its preprocessed text: no /I, /D or /FI (already applied), no /YX (the text
    has no #include left to precompile), no source."""
    args = decomp.cl_args(unit, path, pch=False)[:-2]
    out, skip = [], False
    for a in args:
        if skip:
            skip = False
            continue
        if a == "/I":
            skip = True
            continue
        if a.startswith(("/I", "/D", "/FI")):
            continue
        out.append(a)
    return out


HEADER_SUFFIXES = (".h", ".hpp", ".inl")


def include_root(args, header):
    """The /I directory of args that holds the header (the deepest one), or its own directory."""
    best = None
    for i, a in enumerate(args[:-1]):
        if a == "/I":
            r = Path(args[i + 1])
            try:
                header.resolve().relative_to(r.resolve())
            except ValueError:
                continue
            if best is None or len(r.parts) > len(best.parts):
                best = r
    return best if best is not None else header.parent


def link_or_copy(src, dst):
    try:
        os.link(src, dst)
    except OSError:
        shutil.copy2(src, dst)


def make_shadow(root, header, text, sd):
    """A scratch include directory in place of root: the headers of root (and of the header's own directory when that
    is a subdirectory of it) hard-linked, the header written afresh with `text`. Returns sd."""
    root, header = root.resolve(), header.resolve()
    dirs = [(root, sd)]
    if header.parent != root:
        dirs.append((header.parent, sd / header.parent.relative_to(root)))
    for src_dir, dst_dir in dirs:
        dst_dir.mkdir(parents=True, exist_ok=True)
        for f in src_dir.iterdir():
            if f.is_file() and f.suffix.lower() in HEADER_SUFFIXES and f != header:
                link_or_copy(f, dst_dir / f.name)
    with open(sd / header.relative_to(root), "w", encoding="latin-1", newline="") as f:
        f.write(text)
    return sd


def compile_text(unit, path, entry, text, d, preprocessed, header=None):
    """Compile `text` (preprocessed, or a whole source file) in directory d; (code, relocs, start) or an error.
    header = (path, text) compiles the unit's source with that header's text in place of the file's, from a scratch
    copy searched first (text is then the unit's source, normally unchanged)."""
    d.mkdir(parents=True, exist_ok=True)
    obj = d / "t.obj"
    if preprocessed:
        src = d / "t.cpp"
        src.write_text(text, encoding="latin-1")
        args = ["/c", *i_flags(unit, path), "/Tp" if path.suffix.lower() == ".cpp" else "/Tc", str(src)]
    else:
        args = decomp.cl_args(unit, path, pch=d / "t.pch")
        inc, src_dir = [], d
        if header is not None:
            root = include_root(args, header[0])
            sd = make_shadow(root, header[0], header[1], d / "shadow")
            inc = ["/I", str(sd)]
            if path.parent.resolve() == root.resolve():
                src_dir = sd
        src = src_dir / path.name
        with open(src, "w", encoding="latin-1", newline="") as f:
            f.write(text)
        args = ["/c", args[0], *inc, "/I", str(path.parent), *args[1:-1], str(src)]
    pch = decomp.fresh_pch(args)
    rc, out = decomp.vc6.run("cl", [*args, "/Fo" + str(obj)], cwd=str(d), compiler=decomp.compiler_of(path))
    decomp.drop_pch(pch)
    if rc != 0 or not obj.exists():
        errs = [l for l in out.splitlines() if " error " in l]
        return None, (errs[0] if errs else (out.strip().splitlines() or ["compile failed"])[-1])
    c = coff.Coff(obj.read_bytes())
    s, err = decomp.find_symbol(c, entry)
    if s is None:
        return None, err
    sec = c.section(s.section)
    start, end = c.function_extent(s)
    return (sec.data[start:end], [r for r in sec.relocs if start <= r[0] < end], start), None


IDENT = re.compile(r"[A-Za-z_]\w*")


def unknown_names(variant, known):
    """The identifiers of a variant that the preprocessed unit never spells: macros (NULL, Py_DECREF), which only
    the source path expands, or a name the variant introduces."""
    return set(IDENT.findall(variant)) - known


def run_variant(img, unit, path, entry, pre, known, span, source_lines, first, last, name, variant, d):
    got, err, via = None, None, "i"
    # a /YX unit's code depends on where its precompiled include block ends, which the preprocessed text no longer
    # shows: under the switch every variant (and the base) compiles from the scratch copy of the source
    if not decomp.auto_pch(path) and (variant is None or not unknown_names(variant, known)):
        text = substitute(pre, span, variant) if variant is not None else pre
        got, err = compile_text(unit, path, entry, text, d, True)
    if got is None and (variant is not None or decomp.auto_pch(path)):
        # a macro the preprocessed text no longer has (or a /YX unit): the scratch copy of the source
        body = [variant] if variant is not None else source_lines[first - 1:last]
        src = "\n".join(source_lines[:first - 1] + body + source_lines[last:])
        if Path(entry.path).resolve() == path.resolve():
            got, err = compile_text(unit, path, entry, src, d / "src", False)
        else:
            # defined in a header: the variant goes into the header's text, the unit's source stays as it is
            unit_src = path.read_text(encoding="latin-1")
            got, err = compile_text(unit, path, entry, unit_src, d / "src", False,
                                    header=(Path(entry.path), src))
        via = "source"
    if got is None:
        return {"name": name, "score": None, "error": err}
    code, relocs, start = got
    while code and code[-1] in (0x90, 0xCC):
        code = code[:-1]
    retail = img.read(entry.addr, len(code)) or b""
    ours, theirs = masked(code, relocs, start, retail)
    exact = ours == theirs
    return {"name": name, "score": 100.0 if exact else min(99.9, nav_score(ours, retail, entry.addr)),
            "exact": exact, "bytes": ours, "retail": retail, "via": via}


def show_diff(r, addr, out):
    a = normalised(decomp.disasm(r["bytes"], addr))
    b = normalised(decomp.disasm(r["retail"], addr))
    for tag, i1, i2, j1, j2 in difflib.SequenceMatcher(None, a, b, autojunk=False).get_opcodes():
        if tag != "equal":
            out("      ours:   " + " ; ".join(a[i1:i2]))
            out("      retail: " + " ; ".join(b[j1:j2]))


def main(argv=None, out=print):
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("addr", help="the annotated function (0x...)")
    ap.add_argument("variants", nargs="+", help="hand-written variant files: the function's whole definition each")
    ap.add_argument("--show", action="store_true", help="print each variant's differences from retail")
    ap.add_argument("--jobs", type=int, default=os.cpu_count() or 4)
    a = ap.parse_args(argv)
    addr = int(a.addr, 16)
    unit, path, entry = find_entry(addr)
    if entry is None:
        raise SystemExit("0x%X: no annotated function" % addr)
    texts = {}
    for v in a.variants:
        t = Path(v).read_text(encoding="latin-1").rstrip("\n")
        if not t.strip():
            raise SystemExit("%s is empty" % v)
        if t in texts.values():
            raise SystemExit("%s repeats another variant: each one is its own hand-written spelling" % v)
        texts[v] = t
    src_digest = hashlib.sha1(entry.text.encode("latin-1")).hexdigest()[:12]
    state_path = WORK / ("%X.json" % addr)
    try:
        state = json.loads(state_path.read_text(encoding="utf-8"))
    except (OSError, ValueError):
        state = None
    refusal = check_gate(state, src_digest, len(texts))
    if refusal:
        raise SystemExit(refusal)

    lines = Path(entry.path).read_text(encoding="latin-1").split("\n")
    first, last = definition_span(lines, entry)
    if Path(entry.path).resolve() != path.resolve():
        last = braced_end(lines, first) or last     # a member defined in its class: the scan's end is the class's
    pre = preprocess(unit, path)
    span = preprocessed_span(pre, entry.path, first, last)
    if not span:
        raise SystemExit("the definition's lines aren't in the preprocessed unit")
    img = pe.Image(decomp.target_image())
    d = WORK / ("%X_%d" % (addr, os.getpid()))
    try:
        jobs = [("(base)", None)] + list(texts.items())
        with concurrent.futures.ThreadPoolExecutor(a.jobs) as ex:
            known = set(IDENT.findall(pre))
            futs = [ex.submit(run_variant, img, unit, path, entry, pre, known, span, lines, first, last, n, t,
                              d / str(i))
                    for i, (n, t) in enumerate(jobs)]
            results = [f.result() for f in futs]
    finally:
        shutil.rmtree(d, ignore_errors=True)
    base, results = results[0], results[1:]
    if base["score"] is None:
        raise SystemExit("the unmodified preprocessed unit doesn't compile: %s" % base.get("error"))
    out("0x%X %s (%s): base %.1f%s" % (addr, entry.name or entry.decorated, unit, base["score"],
                                       "  exact" if base["exact"] else ""))
    obj = decomp.BUILD / "obj" / (unit + ".obj")
    if obj.exists():
        c = coff.Coff(obj.read_bytes())
        s, _ = decomp.find_symbol(c, entry)
        if s is not None:
            st, en = c.function_extent(s)
            ours, _ = masked(c.section(s.section).data[st:en].rstrip(b"\x90\xcc"),
                             [r for r in c.section(s.section).relocs if st <= r[0] < en], st, b"")
            if ours != base["bytes"]:
                out("  warning: the base differs from build/obj's object: the fast path doesn't reproduce the unit")
    moved = []
    out("  rank  score  moved  variant")
    for i, r in enumerate(rank(results), 1):
        if r["score"] is None:
            out("  -       -    -      %s  compile failed: %s" % (r["name"], r["error"]))
            continue
        mv = r["bytes"] != base["bytes"]
        if mv:
            moved.append(r["name"])
        note = "  EXACT (masked): a clue, not an answer; the spelling still needs its evidence" if r["exact"] else ""
        out("  %-4d %5.1f  %-5s  %s%s%s" % (i, r["score"], "yes" if mv else "no", r["name"],
                                          " (from the source)" if r["via"] == "source" else "", note))
        if a.show and not r["exact"]:
            show_diff(r, addr, out)
    if len(texts) == 1:
        WORK.mkdir(parents=True, exist_ok=True)
        state_path.write_text(json.dumps({"src": src_digest, "moved": moved}), encoding="utf-8")
        out("  %s" % ("moved the output: a set of up to %d variants may run for this source" % MAX_VARIANTS
                      if moved else "did not move the output: no set runs until one variant does"))
    return 0


if __name__ == "__main__":
    sys.exit(main())
