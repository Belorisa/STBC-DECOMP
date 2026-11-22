"""Annotate claimed functions whose body is already inline in their class header: no model, the build decides.

    python tools/annotate_inlines.py              # list the candidates
    python tools/annotate_inlines.py --apply      # annotate, build, keep what matches, commit

A function claimed in config/symbols.txt with no annotation, whose method has an inline body in its class's header,
needs only a `// FUNCTION: STBC <addr> <decorated>` line directly above that body: the build binds it to the unit
whose object retail kept, and compares it. --apply writes every annotation and removes their claims, runs one build,
keeps the functions that are MATCH with no [match] rule finding, puts the claims and headers back for the rest, runs
the build again to confirm, and commits the kept ones in one commit (headers and symbols.txt only). Then run the gate
(`merge_lane.py check`, or the merge) as for any lane.

Skipped: a method with more than one inline body of that name in the class (overloads: which is which needs the
decorated name read by hand), a class declared in no header, a header with uncommitted changes. Run in a slot of
its own after `decomp.py build`; it refuses a dirty src/ or config/.
"""

import argparse
import json
import re
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import decomp  # noqa: E402
import funcsource as L  # noqa: E402
import fncontext as F  # noqa: E402
import openwork  # noqa: E402

SYMBOLS = decomp.CONFIG / "symbols.txt"


def inline_sites(decl, method):
    """Offsets in the class declaration of each inline body of the method (a line naming it with '(' then '{')."""
    pat = re.compile(r"^[^\n;{}()=]*\b%s\s*\([^;{\n]*\)[^;{\n]*\{" % re.escape(method), re.M)
    return [m.start() for m in pat.finditer(decl)]


def candidates(rows, claims):
    annotated = {int(r["addr"], 16) for r in rows}
    decls, out, skipped = {}, [], []
    for decorated, addr, _ in claims:
        if addr in annotated:
            continue
        cls, method = F.method_of_decorated(decorated)
        if not cls or not method:
            continue
        if cls not in decls:
            decls[cls] = F.class_declaration(cls)
        header, decl = decls[cls]
        if not decl:
            continue
        sites = inline_sites(decl, method)
        if not sites:
            continue
        if len(sites) > 1:
            skipped.append((addr, decorated, "%d inline bodies named %s" % (len(sites), method)))
            continue
        text = header.read_text(encoding="utf-8", errors="surrogateescape")
        line = text[:text.index(decl) + sites[0]].count("\n")   # 0-based line of the definition
        out.append((addr, decorated, header, line))
    return out, skipped


def git(*args):
    return subprocess.run(["git", *args], cwd=str(L.ROOT), capture_output=True, text=True).stdout


def annotate(cands):
    """Insert the annotations, bottom-up per header so line numbers stay valid."""
    by_header = {}
    for addr, decorated, header, line in cands:
        by_header.setdefault(header, []).append((line, addr, decorated))
    for header, items in by_header.items():
        with open(header, encoding="utf-8", errors="surrogateescape", newline="") as f:
            lines = f.read().splitlines(keepends=True)
        newline = "\r\n" if lines and lines[0].endswith("\r\n") else "\n"
        for line, addr, decorated in sorted(items, reverse=True):
            indent = re.match(r"[ \t]*", lines[line]).group(0)
            lines.insert(line, "%s// FUNCTION: STBC 0x%X %s%s" % (indent, addr, decorated, newline))
        with open(header, "w", encoding="utf-8", errors="surrogateescape", newline="") as f:
            f.write("".join(lines))


def drop_claims(addrs):
    with open(SYMBOLS, encoding="utf-8", newline="") as f:
        lines = f.read().splitlines(keepends=True)
    keep = []
    for l in lines:
        body = l.split("#")[0].split()
        if len(body) >= 2 and int(body[1], 16) in addrs:
            continue
        keep.append(l)
    with open(SYMBOLS, "w", encoding="utf-8", newline="") as f:
        f.write("".join(keep))


def build():
    p = subprocess.run([sys.executable, str(L.ROOT / "tools" / "decomp.py"), "build"], cwd=str(L.ROOT),
                       capture_output=True, text=True, errors="replace")
    return json.loads((decomp.BUILD / "report.json").read_text(encoding="utf-8"))["functions"], p.stdout


def outcome(rows, addr):
    r = next((r for r in rows if int(r["addr"], 16) == addr), None)
    if r is None:
        return "NO-ROW"
    pending = any(f.get("tier") == "match" for f in r.get("rules", []))
    return "MATCH" if r["status"] == "MATCH" and not pending else ("MATCH-PENDING" if pending else r["status"])


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("--apply", action="store_true")
    args = ap.parse_args()
    rows, claims = openwork.load()
    cands, skipped = candidates(rows, claims)
    for addr, decorated, header, line in cands:
        print("0x%X %-60s %s:%d" % (addr, decorated[:60], header.relative_to(L.ROOT).as_posix(), line + 1))
    for addr, decorated, why in skipped:
        print("skip 0x%X %s: %s" % (addr, decorated[:60], why))
    print("%d candidates, %d skipped" % (len(cands), len(skipped)))
    if not args.apply or not cands:
        return 0
    if git("status", "--porcelain", "--", "src", "config").strip():
        sys.exit("src/ or config/ has changes: --apply needs a clean tree")

    annotate(cands)
    drop_claims({c[0] for c in cands})
    rows, _ = build()
    kept = [c for c in cands if outcome(rows, c[0]) == "MATCH"]
    failed = [(c, outcome(rows, c[0])) for c in cands if outcome(rows, c[0]) != "MATCH"]
    for (addr, decorated, header, line), why in failed:
        print("not kept 0x%X %s: %s" % (addr, decorated[:60], why))
    if failed:
        # Put everything back, then apply only the kept ones: simpler than unpicking lines.
        git("checkout", "--", "src", "config")
        if not kept:
            print("nothing matched")
            return 1
        annotate(kept)
        drop_claims({c[0] for c in kept})
        rows, _ = build()
        lost = [c for c in kept if outcome(rows, c[0]) != "MATCH"]
        if lost:
            git("checkout", "--", "src", "config")
            sys.exit("the kept set no longer matches on its own (%s): nothing committed"
                     % ", ".join("0x%X" % c[0] for c in lost))
    files = sorted({c[2].relative_to(L.ROOT).as_posix() for c in kept}) + ["config/symbols.txt"]
    names = ", ".join(F.method_of_decorated(c[1])[0] + "::" + F.method_of_decorated(c[1])[1] for c in kept[:6])
    msg = ("match: %d header inlines annotated (%s%s)\n\nThe claimed functions' bodies were already inline in their "
           "class headers; tools/annotate_inlines.py annotated them and the build kept the %d that match."
           % (len(kept), names, ", ..." if len(kept) > 6 else "", len(kept)))
    git("add", "--", *files)
    r = subprocess.run(["git", "commit", "-q", "-m", msg, "--", *files], cwd=str(L.ROOT), capture_output=True,
                       text=True)
    print(r.stdout + r.stderr or "committed %d of %d" % (len(kept), len(cands)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
