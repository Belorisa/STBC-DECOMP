"""One annotated function's source: find it, cut it out, compare signatures, recompile and score it.

Shared by the tools that read one annotated function's source (packet.py, fncontext.py, habits.py, framemap.py,
similar.py). The scoring runs `decomp.py match <unit>` in the checkout this file belongs to.
"""

import json
import re
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
SRC = ROOT / "src"
DECOMP = [sys.executable, str(ROOT / "tools" / "decomp.py")]
ROW = re.compile(r"^\s*(\S+)\s+([\d.]+)\s+(0x[0-9A-Fa-f]+)\s")


def find_annotation(addr):
    """(path, line index) of the FUNCTION annotation for addr."""
    pat = re.compile(r"//\s*FUNCTION:\s*STBC\s+0x0*%x\b" % addr, re.I)
    for p in sorted(SRC.rglob("*")):
        if p.suffix.lower() not in (".c", ".cpp", ".h", ".hpp", ".inl"):
            continue
        lines = p.read_text(encoding="utf-8", errors="replace").splitlines(keepends=True)
        for i, line in enumerate(lines):
            if pat.search(line):
                return p, i
    sys.exit("no FUNCTION annotation for 0x%X under src/" % addr)


def function_span(lines, ann):
    """(start, end) line indices, end exclusive: the annotation block and the definition through its last brace."""
    start = ann
    while start > 0 and lines[start - 1].lstrip().startswith("//") and "FUNCTION:" not in lines[start - 1]:
        start -= 1
    i = ann + 1
    while i < len(lines) and lines[i].lstrip().startswith("//"):
        i += 1
    depth, opened = 0, False
    for j in range(i, len(lines)):
        code = re.sub(r"//.*", "", lines[j])
        code = re.sub(r'"(\\.|[^"\\])*"', '""', code)
        code = re.sub(r"'(\\.|[^'\\])*'", "''", code)
        for ch in code:
            if ch == "{":
                depth += 1
                opened = True
            elif ch == "}":
                depth -= 1
        if opened and depth <= 0:
            return start, j + 1
    sys.exit("could not find the end of the function body")


def signature(text):
    """The definition's text before its first brace, without comments, spacing normalised."""
    code = "\n".join(l for l in text.splitlines() if not l.lstrip().startswith("//"))
    sig = " ".join(code.split("{", 1)[0].split())
    return re.sub(r"\s*([*&(),])\s*", r"\1", sig)   # 'TGStream* stream' and 'TGStream *stream' are one signature


def unit_of(addr):
    """The compiled unit that holds the function, from build/report.json."""
    report = ROOT / "build" / "report.json"
    if not report.exists():
        sys.exit("build/report.json is missing: run `python tools/decomp.py build` first")
    for f in json.loads(report.read_text())["functions"]:
        if int(f["addr"], 16) == addr:
            return f["unit"]
    sys.exit("0x%X is not in build/report.json: run a build" % addr)


class CheckoutLock:
    """An exclusive lock on the checkout's build/, taken by every decomp.py command (its main()). `build`, `match`,
    `diff`, `gate` and `rules` compile into build/obj and read every unit's object there, so two commands in one
    checkout (a worker's build started while another runs) take turns instead of reading an
    object the other is rewriting. The lock is a byte of build/compile.lock, held through the OS (released if the
    process dies)."""

    def __enter__(self):
        path = ROOT / "build" / "compile.lock"
        path.parent.mkdir(exist_ok=True)
        self.f = open(path, "a+b")
        if sys.platform == "win32":
            import msvcrt
            self.f.seek(0)
            try:
                msvcrt.locking(self.f.fileno(), msvcrt.LK_NBLCK, 1)
                return self
            except OSError:
                print("waiting for another decomp.py command in this checkout to finish", file=sys.stderr)
            while True:
                try:
                    self.f.seek(0)
                    msvcrt.locking(self.f.fileno(), msvcrt.LK_LOCK, 1)   # retries for 10 s, then raises
                    break
                except OSError:
                    continue
        else:
            import fcntl
            fcntl.flock(self.f.fileno(), fcntl.LOCK_EX)
        return self

    def __exit__(self, *exc):
        if sys.platform == "win32":
            import msvcrt
            self.f.seek(0)
            msvcrt.locking(self.f.fileno(), msvcrt.LK_UNLCK, 1)
        self.f.close()


def run(args):
    """A decomp.py command's output; the command takes the checkout's lock itself."""
    p = subprocess.run(DECOMP + args, cwd=str(ROOT), capture_output=True, text=True, errors="replace")
    return p.stdout + p.stderr


def score(unit, addr):
    """(status, score, output) of the function after recompiling its unit. run() holds the checkout's lock, so a
    parallel run can't rewrite an object while this one reads it; a failure without a compile error is still
    retried (a run that predates the lock), and only a real compile failure (or a third failure) is an ERROR."""
    for attempt in range(3):
        out = run(["match", unit])
        for line in out.splitlines():
            m = ROW.match(line)
            if m and int(m.group(3), 16) == addr:
                return m.group(1), float(m.group(2)), out
        if "COMPILE FAILED" in out:
            break
        time.sleep(2)
    return "ERROR", 0.0, out
