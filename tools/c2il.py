"""Compare the IL the front end hands to the back end for two spellings of a unit: is an edit byte-neutral everywhere?

    python tools/c2il.py compare game/SetManager edited.cpp              # the unit's source against an edited copy
    python tools/c2il.py compare game/SetManager a.cpp b.cpp             # two edited copies
    python tools/c2il.py compare 0x418510 renamed.cpp                    # a definition variant of one function

cl.exe runs the front end (C1XX.DLL) and the back end (C2.DLL) one after the other, and they share nothing but the IL
files named by the `-il <prefix>` of their command lines (suffixes `ex` the code tuples, `gl` globals, `sy` symbols,
`in` an index, `db` debug data under /Zi). `cl /B2 <batch>` runs the batch in place of C2.DLL, with the pass's command
line in the environment variable MSC_CMD_FLAGS; the batch here copies the IL files away, so a compile yields its IL
and no object. Each side is compiled from the same scratch path (the IL records file paths) with the unit's flags,
and the verdict is one of:

- `identical`: every IL file is equal once the code IL's source-line markers (`4F 01 80 <u16 line>`) are left out:
  the back end gets the same input, so every function of the unit (inline budgets, frame and register choices,
  COMDAT order) comes out the same. Renames of nothing, layout, comments, blank lines;
- `same code`: the code IL is equal and the symbol files differ (a local renamed, a declaration moved): the same
  instructions, though the symbols' order can differ;
- `different`: the code IL differs: the compiler sees another program, even when this unit's bytes don't change (a
  tuple more moves an inline cost, and with it a budget elsewhere). The first differing tuple offset is printed.

With an address the files are definition variants, as `tools/trial.py` takes them: the function's whole definition,
put in place of the annotated one in a copy of its unit's source.

After bw1-decomp's tools/c2-il.py and docs/msvc6_passes.md (openblack's Black & White decompilation, CC0). Unlike
`c2trace --cost`, which counts one function's tuples under a debugger, this compares whole units without one.
"""
import argparse
import os
import re
import shutil
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import decomp  # noqa: E402
import vc6  # noqa: E402

WORK = decomp.BUILD / "c2il"
CODE = "ex"
SYMBOLS = ("gl", "sy", "in")
SUFFIXES = (CODE,) + SYMBOLS
LINE_MARKER = re.compile(rb"\x4f\x01\x80..", re.S)

BATCH = ('@echo off\r\n'
         'for /f "tokens=2" %%P in ("%MSC_CMD_FLAGS%") do (\r\n'
         '  for %%S in (ex gl sy in db) do if exist "%%P%%S" copy /y "%%P%%S" "%C2IL_OUT%\\%%S" >nul\r\n'
         ')\r\n'
         'exit /b 0\r\n')


def strip_lines(code):
    """The code IL without its source-line markers."""
    return LINE_MARKER.sub(b"", code)


def verdict(a, b):
    """('identical' | 'same code' | 'different', detail) for two captures {suffix: bytes}."""
    ca, cb = strip_lines(a.get(CODE, b"")), strip_lines(b.get(CODE, b""))
    if ca != cb:
        n = next((i for i in range(min(len(ca), len(cb))) if ca[i] != cb[i]), min(len(ca), len(cb)))
        return "different", "code IL differs from byte %d (%d against %d bytes, line markers left out)" % (
            n, len(ca), len(cb))
    differ = [s for s in SYMBOLS if a.get(s) != b.get(s)]
    if differ:
        return "same code", "symbol files differ: %s" % ", ".join(differ)
    return "identical", "every IL file is equal (line markers left out)"


def capture(unit, path, text, workdir):
    """{suffix: bytes} of the IL the front end writes for `text` compiled as the unit's source from workdir."""
    workdir.mkdir(parents=True, exist_ok=True)
    out = workdir / "il"
    shutil.rmtree(out, ignore_errors=True)
    out.mkdir()
    bat = workdir / "capture.bat"
    bat.write_bytes(BATCH.encode("ascii"))
    src = workdir / path.name
    with open(src, "w", encoding="latin-1", newline="") as f:
        f.write(text)
    args = decomp.cl_args(unit, path, pch=workdir / "il.pch")
    args = [*args[:1], "/I", str(path.parent), *args[1:-1], str(src)]
    args = [a for a in args if a not in ("/Zi", "/Z7")]
    decomp.fresh_pch(args)
    env = vc6.env()
    env["C2IL_OUT"] = str(out)
    exe = vc6.COMPILERS[decomp.compiler_of(path)] / "CL.EXE"
    p = subprocess.run([str(exe), "/c", "/B2", str(bat), *args], cwd=str(workdir), env=env, capture_output=True,
                       text=True, errors="replace")
    got = {s: (out / s).read_bytes() for s in SUFFIXES if (out / s).exists()}
    if CODE not in got:
        raise SystemExit("no IL captured (cl exit %d): %s" % (p.returncode, (p.stdout + p.stderr).strip()[-400:]))
    return got


def sources(target, files):
    """(unit, path, [(label, text)]) for a unit and whole-file variants, or an address and definition variants."""
    if re.fullmatch(r"0x[0-9A-Fa-f]+", target):
        import trial
        unit, path, entry = trial.find_entry(int(target, 16))
        if entry is None:
            raise SystemExit("%s: no annotated function" % target)
        if Path(entry.path) != path:
            raise SystemExit("%s is annotated in a header: give the unit and whole-file variants" % target)
        lines = decomp.source_text(path).split("\n")
        first, last = trial.definition_span(lines, entry)
        texts = [(f, "\n".join(lines[:first - 1] + [Path(f).read_text(encoding="latin-1").rstrip("\n")]
                               + lines[last:])) for f in files]
    else:
        unit = target.replace("\\", "/").removeprefix("src/")
        unit = re.sub(r"\.(c|cpp)$", "", unit)
        path = decomp.source_files().get(unit)
        if path is None:
            raise SystemExit("no unit %s" % unit)
        texts = [(f, Path(f).read_text(encoding="latin-1")) for f in files]
    if len(texts) == 1:
        texts.insert(0, ("(source)", decomp.source_text(path)))
    return unit, path, texts


def main(argv=None, out=print):
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    sub = ap.add_subparsers(dest="cmd", required=True)
    c = sub.add_parser("compare", help="compare the IL of two spellings")
    c.add_argument("target", help="a unit (game/SetManager) or an annotated function's address")
    c.add_argument("files", nargs="+", help="one or two variants (whole files for a unit, definitions for an address)")
    a = ap.parse_args(argv)
    if len(a.files) > 2:
        ap.error("one or two variants")
    unit, path, texts = sources(a.target, a.files)
    work = WORK / ("%d" % os.getpid())
    try:
        caps = [capture(unit, path, t, work) for _, t in texts]
    finally:
        shutil.rmtree(work, ignore_errors=True)
    v, detail = verdict(*caps)
    out("%s: %s against %s: %s (%s)" % (unit, texts[0][0], texts[1][0], v, detail))
    return 0 if v == "identical" else 1


if __name__ == "__main__":
    sys.exit(main())
