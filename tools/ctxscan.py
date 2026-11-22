"""Declaration-count scan: does a function's code depend on how many declarations its unit has before it?

    python tools/ctxscan.py 0x554040                    # k = 0..63 dummy declarations directly above the definition
    python tools/ctxscan.py 0x554040 0x59D5B0 --top     # at the top of the unit's file, before its includes
    python tools/ctxscan.py 0x6FA560 --range 0 127 --jobs 8

VC6 decides some frame-slot, register and schedule choices of a large function by the count of front-end entities
(declarations) before it, modulo 64 (docs/levers/walls.md, "decided by a count, modulo 64"). For each k in the range
the tool compiles a scratch copy of the function's unit with k lines `extern int ctxScanProbe<i>;` inserted directly
above the function's annotation (or at the top of the file with --top), and compares the function's bytes with
retail's, relocation fields masked on both sides. It prints the k windows where the bytes are exact, and each distinct
byte state with its k and its kind against retail (decomp.flip_kind: frame/register, schedule, or other).

A report only: the scratch copies live under build/ctxscan/ and are always deleted, src/ is never edited, and a dummy
declaration is never a fix (a count that lands in the window is a clue to the original's declarations, nothing more).
The scratch copy is compiled from build/ with the source's directory first on the include path; k = 0 is checked
against the unit's own object in build/obj when there is one, and a difference is printed as a warning.
"""

import argparse
import concurrent.futures
import hashlib
import json
import os
import shutil
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import coff  # noqa: E402
import decomp  # noqa: E402
import pe  # noqa: E402

SCRATCH = decomp.BUILD / "ctxscan"
PROBE = "extern int ctxScanProbe%d;"


def find_entry(addr):
    """(unit, unit source path, entry): the annotated function at addr. A function annotated in a header is found
    through build/report.json's unit (the one whose object retail kept)."""
    files = decomp.source_files()
    for unit, path in files.items():
        for e in decomp.scan(unit, path):
            if e.kind == "FUNCTION" and e.addr == addr and Path(e.path) == path:
                return unit, path, e
    try:
        rows = json.loads((decomp.BUILD / "report.json").read_text(encoding="utf-8"))["functions"]
    except (OSError, ValueError, KeyError):
        rows = []
    unit = next((r["unit"] for r in rows if int(r["addr"], 16) == addr), None)
    if unit in files:
        e = next((e for e in decomp.header_entries() if e.kind == "FUNCTION" and e.addr == addr), None)
        if e is not None:
            return unit, files[unit], e
    return None, None, None


def insertion_line(lines, entry, top):
    """0-based line the probes go above: the first line of the annotation stack over the definition, or 0."""
    if top:
        return 0
    i = entry.line - 1
    while i > 0 and lines[i - 1].lstrip().startswith("//") and decomp.ANNOTATION.match(lines[i - 1]):
        i -= 1
    return i


def masked_code(obj_path, entry):
    """(our bytes, retail's bytes) of the function with every relocation field zeroed on both sides, or an error."""
    c = coff.Coff(obj_path.read_bytes())
    s, err = decomp.find_symbol(c, entry)
    if s is None:
        return None, err
    sec = c.section(s.section)
    start, end = c.function_extent(s)
    code = bytearray(sec.data[start:end])
    retail = bytearray(IMG.read(entry.addr, len(code)) or b"")
    for off, _, _ in sec.relocs:
        o = off - start
        if 0 <= o < len(code):
            code[o:o + 4] = b"\0" * 4
            retail[o:o + 4] = b"\0" * 4
    return (bytes(code), bytes(retail)), None


def compile_probe(unit, path, entry, lines, at, k, workdir):
    """Compile the unit with k probes at line `at`; (masked our bytes, masked retail) or an error string."""
    d = workdir / ("k%d" % k)
    d.mkdir(parents=True, exist_ok=True)
    src = d / path.name
    with open(src, "w", encoding="latin-1", newline="") as f:
        f.write("\n".join(lines[:at] + [PROBE % j for j in range(k)] + lines[at:]))
    obj = d / "probe.obj"
    args = decomp.cl_args(unit, path, pch=d / "probe.pch")
    # the scratch copy's quoted includes resolve as the original's: its own directory first
    args = args[:1] + ["/I", str(path.parent)] + args[1:-1] + [str(src)]
    pch = decomp.fresh_pch(args)
    rc, out = decomp.vc6.run("cl", ["/c", *args, "/Fo" + str(obj)], cwd=str(d), compiler=decomp.compiler_of(path))
    decomp.drop_pch(pch)
    if rc != 0 or not obj.exists():
        return None, "compile failed: " + out.strip().splitlines()[-1] if out.strip() else "compile failed"
    return masked_code(obj, entry)


def ranges(ks):
    """`0-8,60-63` for a sorted list of ints."""
    out = []
    for k in ks:
        if out and k == out[-1][1] + 1:
            out[-1][1] = k
        else:
            out.append([k, k])
    return ",".join("%d" % a if a == b else "%d-%d" % (a, b) for a, b in out) or "none"


def kind_of(code, retail, addr):
    if code == retail:
        return "exact"
    ours = [(m, o) for _, m, o in decomp.disasm(code, addr)]
    theirs = [(m, o) for _, m, o in decomp.disasm(retail, addr)]
    return decomp.flip_kind(ours, theirs) or "other"


def scan(addr, top, lo, hi, jobs, out=sys.stdout):
    unit, path, entry = find_entry(addr)
    if entry is None:
        print("0x%X: no annotated function at this address" % addr, file=out)
        return 1
    in_unit = Path(entry.path) == path
    if not in_unit and not top:
        print("0x%X: annotated in %s, not in its unit's file: probing at the top of %s" % (
            addr, Path(entry.path).relative_to(decomp.ROOT).as_posix(), path.relative_to(decomp.ROOT).as_posix()),
            file=out)
        top = True
    lines = decomp.source_text(path).split("\n")
    at = insertion_line(lines, entry, top)
    workdir = SCRATCH / ("%X_%d" % (addr, os.getpid()))
    results = {}
    try:
        with concurrent.futures.ThreadPoolExecutor(jobs) as ex:
            futures = {ex.submit(compile_probe, unit, path, entry, lines, at, k, workdir): k for k in range(lo, hi + 1)}
            for f in concurrent.futures.as_completed(futures):
                results[futures[f]] = f.result()
    finally:
        shutil.rmtree(workdir, ignore_errors=True)
        try:
            SCRATCH.rmdir()   # only when no other scan is running
        except OSError:
            pass
    print("0x%X %s (%s; k=%d..%d %s)" % (addr, entry.name or entry.decorated, unit, lo, hi,
                                        "at the top of the file" if top else "above the definition"), file=out)
    errors = {k: err for k, (_, err) in results.items() if err}
    if errors:
        k0 = min(errors)
        print("  %d compile(s) failed, first k=%d: %s" % (len(errors), k0, errors[k0]), file=out)
    states = {}
    for k in sorted(results):
        pair, err = results[k]
        if pair is not None:
            states.setdefault(hashlib.sha1(pair[0]).hexdigest(), []).append(k)
    base = decomp.BUILD / "obj" / (unit + ".obj")
    if lo == 0 and 0 in results and results[0][0] is not None and base.exists():
        own, err = masked_code(base, entry)
        if own is not None and own[0] != results[0][0][0]:
            print("  warning: k=0 differs from build/obj's object: the scratch compile is not the unit's", file=out)
    exact = sorted(k for k, (pair, _) in results.items() if pair is not None and pair[0] == pair[1])
    print("  exact at k: %s" % ranges(exact), file=out)
    print("  %d distinct byte state%s" % (len(states), "" if len(states) == 1 else "s"), file=out)
    for ks in sorted(states.values(), key=lambda ks: ks[0]):
        code, retail = results[ks[0]][0]
        print("    %-15s k=%s" % (kind_of(code, retail, addr), ranges(ks)), file=out)
    return 0


def main(argv=None):
    global IMG
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("addrs", nargs="+", help="annotated function addresses (0x...)")
    ap.add_argument("--top", action="store_true", help="insert at the top of the unit's file, not above the function")
    ap.add_argument("--range", nargs=2, type=int, default=(0, 63), metavar=("LO", "HI"), help="k range (0 63)")
    ap.add_argument("--jobs", type=int, default=os.cpu_count() or 4, help="parallel compiles")
    args = ap.parse_args(argv)
    lo, hi = args.range
    if lo < 0 or hi < lo:
        ap.error("--range needs 0 <= LO <= HI")
    IMG = pe.Image(decomp.target_image())
    rc = 0
    for a in args.addrs:
        rc |= scan(int(a, 16), args.top, lo, hi, args.jobs)
    return rc


IMG = None

if __name__ == "__main__":
    sys.exit(main())
