"""Merge a worker's lane into the main branch through the fixed procedure (orchestrator skill, "Merging a lane").

    python tools/merge_lane.py merge C:\\Recomp\\stbc-wt\\slot1 -m "merge: lane/game-25 (what landed)"

In the lane's slot: rebase onto main (symbols.txt conflicts are resolved by a per-name union; any other conflict
stops the merge), build, renumber shifted `_$E` annotations (tools/statics.py), prune symbols.txt and commit the prune when it dropped real lines, regress (must be 0),
gate. Then in the main checkout: fast-forward (main is then the gated lane commit, so the slot's build output is
copied over rather than built and gated again), update the baseline, the cards' score entries and the README
counts, commit them
with the given message, export the Ghidra names, and park the slot at main with its branch deleted.

Every step stops at the first failure and leaves the slot as it is for a human to look at. It never builds while a
rebase is unfinished.

A worker runs the same checks on its own slot before reporting, without merging:

    python tools/merge_lane.py check

The pieces are also available on their own:

    python tools/merge_lane.py resolve-symbols config/symbols.txt   # during a manual rebase
    python tools/merge_lane.py prune [<checkout>]                    # drop symbols.txt lines annotations now define
    python tools/merge_lane.py counts [<checkout>]                   # refresh the README's per-module counts
"""

import argparse
import json
import re
import shutil
import subprocess
import sys
from collections import Counter
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import cards  # noqa: E402
import statics  # noqa: E402

MAIN = Path(__file__).resolve().parent.parent
TRAILER = "Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>"


class Stop(Exception):
    pass


# ---------------------------------------------------------------------------
# helpers

def git(cwd, *args, check=True):
    p = subprocess.run(["git", *args], cwd=str(cwd), capture_output=True, text=True, errors="replace")
    if check and p.returncode != 0:
        raise Stop("git %s failed in %s:\n%s" % (" ".join(args), cwd, p.stdout + p.stderr))
    return p.stdout


def decomp(cwd, *args):
    """Run tools/decomp.py in cwd; return (exit code, output)."""
    p = subprocess.run([sys.executable, str(Path(cwd) / "tools" / "decomp.py"), *args], cwd=str(cwd),
                       capture_output=True, text=True, errors="replace")
    return p.returncode, p.stdout + p.stderr


def last_line(out, prefix=""):
    lines = [l for l in out.splitlines() if l.startswith(prefix)]
    return lines[-1] if lines else ""


def say(msg):
    print(msg, flush=True)


def unmerged(cwd):
    return git(cwd, "diff", "--name-only", "--diff-filter=U").split()


# What judges a lane: its build, comparison, rule and gate code, the flags it compiles with, and the baseline it is
# compared with. The merge runs them from the lane's slot, so a lane that changed them would be judged by its own
# rules: a lane may change tools/ or project.json only when the merge says it is a tooling merge (--allow-tooling),
# and never the baseline, which only the merge itself writes on main.
JUDGE_PATHS = ("tools", "config/project.json")
NEVER_IN_LANE = ("config/baseline.json",)
IGNORED_OK = re.compile(r"^tools/(?:vc6/?|.*__pycache__/.*|.*\.pyc)$")


def judge_changes(slot, against):
    """(judge paths the lane changes against `against`, baseline paths it changes, ignored files under tools/ that
    could shadow a module: anything but the compiler junction and bytecode caches)."""
    changed = git(slot, "diff", "--name-only", against, "--", *JUDGE_PATHS).split()
    baseline = git(slot, "diff", "--name-only", against, "--", *NEVER_IN_LANE).split()
    ignored = [l[3:] for l in git(slot, "status", "--porcelain", "--ignored", "--", "tools").splitlines()
               if l.startswith("!! ") and not IGNORED_OK.match(l[3:])]
    return changed, baseline, ignored


def require_judges(slot, against, allow_tooling):
    changed, baseline, ignored = judge_changes(slot, against)
    if baseline:
        raise Stop("the lane changes %s: only the merge writes the baseline, on main" % ", ".join(baseline))
    if ignored:
        raise Stop("ignored files under tools/ in the slot (they could shadow the tools): %s" % ", ".join(ignored))
    if changed and not allow_tooling:
        raise Stop("the lane changes what judges it (%s): review that diff, then merge with --allow-tooling"
                   % ", ".join(changed))
    if changed:
        say("  tooling merge: the lane's own %s judge it (--allow-tooling)" % ", ".join(changed))
    else:
        say("  judged by main's tools, flags and baseline (the lane changes none of them)")


def require_clean(cwd):
    dirty = git(cwd, "status", "--porcelain").strip()
    if dirty:
        raise Stop("%s has uncommitted changes:\n%s" % (cwd, dirty))


def commit(cwd, message, *paths):
    """Commit the paths; False, with nothing committed, when they hold no change (a lane of tools or docs leaves
    the baseline and the README counts as they were)."""
    git(cwd, "add", *paths)
    if subprocess.run(["git", "diff", "--cached", "--quiet", "--", *paths], cwd=str(cwd)).returncode == 0:
        return False
    git(cwd, "commit", "-q", "-m", message + "\n\n" + TRAILER)
    return True


# ---------------------------------------------------------------------------
# symbols.txt

def read_lines(path):
    text = Path(path).read_text(encoding="utf-8")
    return text.splitlines()


def write_lines(path, lines):
    Path(path).write_text("\n".join(lines) + "\n", encoding="utf-8", newline="\n")


def resolve_symbols(path):
    """Resolve rebase conflicts in symbols.txt: a per-name union of both sides. A stray continuation line (no address)
    rejoins the entry above it. Returns the names listed at two different addresses (left for a human)."""
    out, seen, clashes = [], {}, []
    for line in read_lines(path):
        if line.startswith(("<<<<<<<", "=======", ">>>>>>>")):
            continue
        body = line.split("#")[0].strip()
        if body:
            parts = body.split()
            try:
                addr = int(parts[1], 16)
            except (IndexError, ValueError):
                if out:
                    out[-1] = out[-1].rstrip() + " " + line.strip()
                continue
            name = parts[0]
            if name in seen:
                if seen[name] != addr:
                    clashes.append("%s at 0x%X and 0x%X" % (name, seen[name], addr))
                continue
            seen[name] = addr
        out.append(line)
    write_lines(path, out)
    return clashes


def prune(checkout):
    """Drop symbols.txt lines whose name an annotation now defines at the same address, and duplicate lines.
    Reads the checkout's build/obj, so it must follow a build. Returns (dropped, conflicts)."""
    sys.path.insert(0, str(Path(checkout) / "tools"))
    import decomp as d   # the checkout's own copy
    objdir = d.BUILD / "obj"
    all_units = d.source_files()
    objs = {u: objdir / (u + ".obj") for u in all_units if (objdir / (u + ".obj")).exists()}
    annotated = {}
    for u in d.load_units(objs, all_units).values():
        for e in u.entries:
            s = u.symbols.get(e.addr)
            if s is not None:
                annotated[s.name] = e.addr
    path = Path(checkout) / "config" / "symbols.txt"
    keep, seen, dropped, conflicts = [], set(), 0, []
    for line in read_lines(path):
        body = line.split("#")[0].strip()
        if not body:
            keep.append(line)
            continue
        name, addr = body.split()[:2]
        addr = int(addr, 16)
        if name in annotated:
            if annotated[name] != addr:
                conflicts.append("%s listed 0x%X, annotated 0x%X" % (name, addr, annotated[name]))
                keep.append(line)
            else:
                dropped += 1
            continue
        if name in seen:
            dropped += 1
            continue
        seen.add(name)
        keep.append(line)
    write_lines(path, keep)
    return dropped, conflicts


# ---------------------------------------------------------------------------
# README counts

README_ROWS = {
    "Python 1.5.2": "python",
    "SWIG `App` wrappers (`game/AppWrap`)": "game/AppWrap",
    "Totally Games framework (`src/game`, other units)": "game",
    "MSVC6 C runtime (LIBCMT and LIBCPMT C members, `src/crt`)": "crt",
    "MSVC6 C++ runtime (LIBCPMT and LIBCMT C++ members, `src/cpprt`)": "cpprt",
    "NetImmerse 3.1": "netimmerse",
    "GameSpy SDK (`src/gamespy`)": "gamespy",
}


def readme_counts(checkout):
    """Refresh the README's per-module table and the .text line from build/report.json."""
    report = json.loads((Path(checkout) / "build" / "report.json").read_text())
    ruled, exact = Counter(), Counter()
    for f in report["functions"]:
        if f.get("static"):
            continue   # compiler-generated static initialisers are counted apart (the line under the table)
        unit = f["unit"]
        module = "game/AppWrap" if unit == "game/AppWrap" else unit.split("/")[0]
        if f["status"] == "MATCH":
            exact[module] += 1
            if not any(r["tier"] == "match" for r in f.get("rules", [])):
                ruled[module] += 1
    path = Path(checkout) / "README.md"
    t = path.read_text(encoding="utf-8")
    # the fourth column is derived, never typed: the exact functions a [match] rule still holds back
    for label, m in README_ROWS.items():
        row = "| %s | %d | %d | %d |" % (label, ruled[m], exact[m], exact[m] - ruled[m])
        t = re.sub(r"\| %s \| \d+ \| \d+ \|[^\n]*" % re.escape(label), lambda _: row, t)
    total = "| **total** | **%d** | **%d** | **%d** |" % (
        sum(ruled.values()), sum(exact.values()), sum(exact.values()) - sum(ruled.values()))
    t = re.sub(r"\| \*\*total\*\* \| \*\*\d+\*\* \| \*\*\d+\*\* \|[^\n]*", lambda _: total, t)
    s = report["summary"]
    t = re.sub(r"`\.text` built from source: [\d,]+ of [\d,]+ bytes \([\d.]+%\)\.",
               "`.text` built from source: {:,} of {:,} bytes ({:.3f}%).".format(
                   s["source_bytes"], s["text_bytes"], 100 * s["source_bytes"] / s["text_bytes"]), t)
    pending = len(s.get("statics_pending") or [])
    line = "Compiler-generated static initialisers (`_$E`), counted apart: {:,} of {:,} matched{}.".format(
        s.get("statics_matched", 0), s.get("statics", 0), ", %d MATCH-PENDING" % pending if pending else "")
    if "counted apart:" in t:
        t = re.sub(r"Compiler-generated static initialisers \(`_\$E`\), counted apart: [\d,]+ of [\d,]+ matched"
                   r"(?:, \d+ MATCH-PENDING)?\.", lambda m: line, t)
    else:
        t = re.sub(r"(`\.text` built from source: [^\n]*\n)", lambda m: m.group(1) + line + "\n", t, count=1)
    path.write_text(t, encoding="utf-8", newline="\n")


# ---------------------------------------------------------------------------
# the procedure

DIFF3_HUNK = re.compile(r"<<<<<<< [^\n]*\n(.*?)\|\|\|\|\|\|\| [^\n]*\n(.*?)=======\r?\n(.*?)>>>>>>> [^\n]*\n", re.S)


def resolve_additions(slot, name):
    """Resolve a conflicted file whose every hunk is two pure additions at the same place (the common ancestor had
    nothing there): keep main's lines, then the lane's. Returns False, leaving the file alone, if any hunk changed or
    removed an existing line, or if both sides created the file (two versions of one new unit would be glued
    together), which needs a human."""
    stages = {line.split()[2] for line in git(slot, "ls-files", "-u", "--", name).splitlines() if line.strip()}
    if "1" not in stages:
        return False
    git(slot, "checkout", "--conflict=diff3", "--", name)
    path = slot / name
    with open(path, encoding="latin-1", newline="") as f:
        text = f.read()
    hunks = DIFF3_HUNK.findall(text)
    if not hunks or any(base.strip() for _, base, _ in hunks):
        return False
    # two lanes adding the same function, datum or macro is a duplicate, not two additions
    def keys(side):
        return set(re.findall(r"//\s*(?:FUNCTION|DATA):\s*STBC\s+0x0*([0-9A-Fa-f]+)", side)) | \
               set(re.findall(r"#define\s+(\w+)", side))
    if any(keys(ours) & keys(theirs) for ours, _, theirs in hunks):
        return False
    merged = DIFF3_HUNK.sub(lambda m: m.group(1) + m.group(3), text)
    if any(marker in merged for marker in ("<<<<<<<", "|||||||", ">>>>>>>")):
        return False
    with open(path, "w", encoding="latin-1", newline="") as f:
        f.write(merged)
    return True


STATIC_ANNOTATION = re.compile(r"^\s*//\s*FUNCTION:\s*STBC\s+0x[0-9A-Fa-f]+\s+_\$E\d+\s*$")
CONFLICT = re.compile(r"<<<<<<< [^\n]*\n(.*?)(?:\|{7} [^\n]*\n(.*?))?=======\n(.*?)>>>>>>> [^\n]*\n", re.S)


def resolve_statics(slot, rel):
    """A conflict whose every side holds only `_$E` annotation lines for the same addresses (two lanes renumbered
    the same static initialisers) takes main's side; fix_statics renumbers after the build. True when resolved."""
    path = Path(slot) / rel
    if not path.exists():
        return False
    raw = path.read_bytes()
    crlf = b"\r\n" in raw
    text = raw.decode("latin-1").replace("\r\n", "\n")
    blocks = CONFLICT.findall(text)
    if not blocks:
        return False
    for block in blocks:
        sides = [[l for l in side.splitlines() if l.strip()] for side in block]
        if not all(STATIC_ANNOTATION.match(l) for side in sides for l in side):
            return False
        addrs = {tuple(sorted(l.split()[3] for l in side)) for side in sides if side}
        if len(addrs) != 1:
            return False
    text = CONFLICT.sub(lambda m: m.group(1), text)
    path.write_bytes((text.replace("\n", "\r\n") if crlf else text).encode("latin-1"))
    return True


def rebase(slot):
    say("rebase onto main")
    p = subprocess.run(["git", "rebase", "main"], cwd=str(slot), capture_output=True, text=True, errors="replace")
    for _ in range(200):
        files = unmerged(slot)
        if not files:
            break
        hard = [f for f in files if f != "config/symbols.txt" and not resolve_additions(slot, f)
                and not resolve_statics(slot, f)]
        if hard:
            raise Stop("rebase stopped on conflicts a human must resolve: %s (the slot is mid-rebase)" % hard)
        for f in files:
            if f != "config/symbols.txt":
                say("  %s: both sides only added lines; kept both" % f)
                git(slot, "add", f)
        if "config/symbols.txt" in files:
            clashes = resolve_symbols(slot / "config" / "symbols.txt")
            if clashes:
                raise Stop("symbols.txt lists names at two addresses (the slot is mid-rebase):\n  "
                           + "\n  ".join(clashes))
            git(slot, "add", "config/symbols.txt")
        p = subprocess.run(["git", "-c", "core.editor=true", "rebase", "--continue"], cwd=str(slot),
                           capture_output=True, text=True, errors="replace")
    for state in ("rebase-merge", "rebase-apply"):
        path = Path(git(slot, "rev-parse", "--git-path", state).strip())
        if not path.is_absolute():
            path = slot / path
        if path.exists():
            raise Stop("the rebase did not finish:\n" + p.stdout + p.stderr)
    for line in read_lines(slot / "config" / "symbols.txt"):
        if line.startswith(("<<<<<<<", "=======", ">>>>>>>")):
            raise Stop("conflict markers left in symbols.txt")


def fix_statics(slot):
    """Renumber `_$E` annotations whose compiled names shifted (tools/statics.py), commit, and compare again, until a pass
    changes nothing (a renamed neighbour can displace a byte-identical exit destructor). A shift is mechanical (any
    header a unit includes moves them), so it is fixed here rather than reported as a regression."""
    for _ in range(8):
        if not renumber_once(slot):
            return


def renumber_once(slot):
    changes, problems = statics.renumber(slot)
    for p in problems:
        say("  statics: " + p)
    if not changes:
        return False
    for c in changes:
        say("  statics: renumbered " + c)
    paths = sorted({"src/" + c.split()[0] + ext for c in changes for ext in (".cpp", ".c")
                    if (Path(slot) / "src" / (c.split()[0] + ext)).exists()})
    branch = git(slot, "rev-parse", "--abbrev-ref", "HEAD").strip()
    for c in changes:   # the renumbering is a try on the card of a function still open (a parked referent wait)
        addr = c.split()[1].rstrip(":")
        if cards.card_path(addr, slot).exists():
            cards.append(addr, [{"date": cards.today(), "kind": "try", "lane": cards.lane_name(branch) or branch,
                                 "by": "merge_lane", "from": None, "to": None, "how": c}], slot)
    commit(slot, "match: renumber shifted _$E static-initialiser annotations (tools/statics.py)", *paths,
           cards.CARDS)
    # Only the renumbered units changed, and only in their annotations: compare those again (`match` refreshes
    # their rows in build/report.json, which the next pass and regress read). The gate builds everything afterwards.
    units = sorted({c.split()[0] for c in changes})
    rc, out = decomp(slot, "match", *units)
    say("  match %s: %s" % (" ".join(units), last_line(out, "matched under the rules")))
    if rc != 0:
        raise Stop("match failed in the lane, after the renumbering:\n%s" % out[-2000:])
    return True


FLIP_LINE = re.compile(r"^declaration-sensitive (0x[0-9A-Fa-f]+) (.*) lost MATCH \((\S+) ([\d.]+), ([^)]+)\)")


def sensitive_flips(regress_out):
    """[(address, name, status, score, kind)] of the lost matches regress accepted by itself: functions listed in
    config/declaration_sensitive.txt, source unchanged, diff only of an allowed kind (decomp.sensitive_flip)."""
    out = []
    for l in regress_out.splitlines():
        m = FLIP_LINE.match(l)
        if m:
            out.append((m.group(1), m.group(2), m.group(3), float(m.group(4)), m.group(5)))
    return out


def flip_entry(flip, lane, date):
    addr, name, status, score, kind = flip
    return {"date": date, "kind": "try", "lane": lane, "by": "merge_lane", "from": 100.0, "to": score,
            "how": "declaration-sensitive flip accepted (config/declaration_sensitive.txt): %s, source unchanged"
                   % kind, "wall": "register"}


def record_flips(slot, flips):
    """Print the accepted flips under their own heading, write each one's card entry (once per lane), and commit the
    cards, so the gate after it sees a clean tree."""
    if not flips:
        say("  declaration-sensitive flips accepted: none")
        return
    say("  declaration-sensitive flips accepted: %d (listed in config/declaration_sensitive.txt, source unchanged, "
        "diff only frame slots, registers or schedule)" % len(flips))
    branch = git(slot, "rev-parse", "--abbrev-ref", "HEAD").strip()
    lane = cards.lane_name(branch) or branch
    for flip in flips:
        say("    %s %s (%s %.1f, %s)" % (flip[0], flip[1], flip[2], flip[3], flip[4]))
        e = flip_entry(flip, lane, cards.today())
        if not any(x.get("how") == e["how"] and x.get("lane") == lane for x in cards.read(flip[0], slot)):
            cards.append(flip[0], [e], slot)
    commit(slot, "cards: entries for the declaration-sensitive flips accepted (config/declaration_sensitive.txt)",
           cards.CARDS)


def check_build(cwd, where):
    if unmerged(cwd):
        raise Stop("refusing to build %s: unmerged paths" % where)
    rc, out = decomp(cwd, "build")
    say("  build: " + last_line(out, "matched under the rules"))
    if rc != 0:   # a unit that doesn't compile, or a resolution or data problem
        raise Stop("build failed in %s:\n%s" % (where, out[-2000:]))
    return out


def gate(cwd, where):
    rc, out = decomp(cwd, "gate")
    if rc != 0:
        raise Stop("gate failed in %s:\n%s" % (where, out[-3000:]))
    say("  " + last_line(out, "GATE OK"))


def merge(slot, message, accept=(), allow_tooling=False):
    slot = Path(slot).resolve()
    branch = git(slot, "rev-parse", "--abbrev-ref", "HEAD").strip()
    if branch in ("HEAD", "main"):
        raise Stop("%s is not on a lane branch (%s)" % (slot, branch))
    require_clean(slot)
    require_clean(MAIN)
    say("merging %s from %s" % (branch, slot))

    rebase(slot)
    # After the rebase, an unchanged path is main's very file: the slot then judges the lane with main's tools.
    require_judges(slot, "main", allow_tooling)
    check_build(slot, "the lane")

    dropped, conflicts = prune(slot)
    for c in conflicts:
        say("  symbols.txt: " + c)
    changed = git(slot, "diff", "--ignore-cr-at-eol", "--numstat", "--", "config/symbols.txt").strip()
    if changed:
        say("  prune dropped %d entries" % dropped)
        commit(slot, "match: drop symbols.txt entries now defined by annotations", "config/symbols.txt")
    else:
        git(slot, "checkout", "--", "config/symbols.txt")

    fix_statics(slot)
    rc, out = decomp(slot, "regress", *(["--accept", *accept] if accept else []))
    say("  " + last_line(out, "regressions"))
    for l in out.splitlines():
        if l.startswith(("REGRESSION", "accepted")):
            say("  " + l)
    if rc != 0 or not re.search(r"regressions: 0\b", out):
        raise Stop("regress found regressions (fix in the lane or have the user adjudicate):\n" + out[-2000:])
    record_flips(slot, sensitive_flips(out))
    gate(slot, "the lane")
    report_notes(slot)

    say("fast-forward main")
    before = git(MAIN, "rev-parse", "HEAD").strip()
    git(MAIN, "merge", "--ff-only", "-q", branch)
    # main now holds the very commit the slot just built and gated, so building and gating it again proves
    # nothing; take the slot's build output instead. A tree mismatch falls back to the full build and gate.
    if git(MAIN, "rev-parse", "HEAD^{tree}") == git(slot, "rev-parse", "HEAD^{tree}"):
        shutil.rmtree(MAIN / "build" / "obj", ignore_errors=True)
        shutil.copytree(slot / "build" / "obj", MAIN / "build" / "obj")
        shutil.copy2(slot / "build" / "report.json", MAIN / "build" / "report.json")
        say("  main is the gated lane commit; build output taken from the slot")
    else:
        check_build(MAIN, "main")
        gate(MAIN, "main")
    old_baseline = cards.load_json(MAIN / "config" / "baseline.json")
    decomp(MAIN, "report", "--update-baseline")
    mark_accepted_unannotated(accept)
    moves = cards.score_entries(old_baseline, cards.load_json(MAIN / "config" / "baseline.json"),
                                cards.lane_name(branch) or branch, git(MAIN, "rev-parse", "--short=10", "HEAD").strip(),
                                cards.today(), MAIN)
    for k, e in moves.items():
        cards.append(k, [e], MAIN)
    say("  cards: %d score entries" % len(moves))
    readme_counts(MAIN)
    if not commit(MAIN, message, "config/baseline.json", "README.md", cards.CARDS):
        say("  baseline and README counts unchanged: main stays at the lane's own commits")
    # what ApplyStbcAll.java reads: types, function names and signatures, globals
    # The type export reads only the headers (about half a minute): skip it when the merge changed none.
    headers = git(MAIN, "diff", "--name-only", before, "HEAD", "--", "*.h", "*.hpp", "*.inl").strip()
    exports = ("export_types.py", "export_names.py", "export_data.py") if headers else ("export_names.py",
                                                                                     "export_data.py")
    for export in exports:
        subprocess.run([sys.executable, str(MAIN / "tools" / "ghidra" / export)], cwd=str(MAIN), capture_output=True)
    say("  Ghidra exports refreshed: run tools/ghidra/ApplyStbcAll.java in Ghidra and save")
    say("  " + git(MAIN, "log", "--oneline", "-1").strip())

    git(slot, "checkout", "-q", "--detach", "main")
    git(slot, "branch", "-D", branch)
    say("slot parked at main; %s deleted" % branch)


NOTE_KINDS = ("BUG:", "RULE-EXCEPTION:")


def note_marked(line, kind):
    """True when the source line carries a note of the kind: a `//` comment, a C comment (`/* BUG: ... */`, the
    CRT's style), or a continuation line of one (` * BUG: ...`)."""
    return re.search(r"(?://|/\*)\s*" + re.escape(kind), line) is not None or         re.match(r"\s*\*\s*" + re.escape(kind), line) is not None


def comment_of(line):
    """The comment part of a source line: after `//` or `/*`, or a C comment's continuation line (` * ...`)."""
    k = [i for i in (line.find("//"), line.find("/*")) if i >= 0]
    if k:
        return line[min(k):]
    return line if re.match(r"\s*\*", line) else ""


def invented_marked(line, in_block=False):
    """True when the line's comment says what it carries is invented: `(invented)`, "an invented name", ...
    in_block: the line lies inside a `/* */` comment (a function banner's text lines carry no comment marker)."""
    return "invented" in (line if in_block else comment_of(line)).lower()


def block_comment_lines(text):
    """The 1-based numbers of the lines that lie wholly or partly inside a `/* */` comment (string literals aside),
    so a banner's plain text lines count as comment."""
    inside, out = False, set()
    for n, l in enumerate(text.splitlines(), 1):
        i, quote, touched = 0, None, inside
        while i < len(l):
            c = l[i]
            if inside:
                if l.startswith("*/", i):
                    inside, i = False, i + 2
                    continue
            elif quote:
                if c == "\\":
                    i += 1
                elif c == quote:
                    quote = None
            elif c in "\"'":
                quote = c
            elif l.startswith("//", i):
                break
            elif l.startswith("/*", i):
                inside = touched = True
                i += 2
                continue
            i += 1
        if touched:
            out.add(n)
    return out


def added_notes(slot):
    """[(file, line, kind, text)] for each BUG and RULE-EXCEPTION note (`//` or `/* */`) the lane adds under src/, and
    each `// MATCH-PENDING:` it removes. A BUG note silences R17 on its statement, a RULE-EXCEPTION waives a style
    rule and a removed MATCH-PENDING lets a function count; no tool checks any of them, so the merge lists each one
    for the reviewer. A removed note's line is its line in main."""
    out, path, old_path, line, old_line = [], None, None, 0, 0
    blocks = set()
    for l in git(slot, "diff", "-U0", "main...HEAD", "--", "src").splitlines():
        if l.startswith("+++ "):
            path = l[6:] if l.startswith("+++ b/") else None
            blocks = block_comment_lines(git(slot, "show", "HEAD:" + path, check=False)) if path else set()
        elif l.startswith("--- "):
            old_path = l[6:] if l.startswith("--- a/") else None
        elif l.startswith("@@"):
            m = re.search(r"-(\d+)(?:,\d+)? \+(\d+)", l)
            old_line, line = (int(m.group(1)), int(m.group(2))) if m else (0, 0)
        elif l.startswith("+") and path:
            for kind in NOTE_KINDS:
                if note_marked(l[1:], kind):
                    out.append((path, line, kind.rstrip(":"), l[1:].strip()))
            if invented_marked(l[1:], line in blocks):
                out.append((path, line, "invented", l[1:].strip()))
            line += 1
        elif l.startswith("-") and old_path:
            if note_marked(l[1:], "MATCH-PENDING:"):
                out.append((old_path, old_line, "removed MATCH-PENDING", l[1:].strip()))
            old_line += 1
    return out


INVENTED_SHOWN = 40   # invented markers printed in full; the rest are counted per file


def report_notes(slot):
    for line in notes_lines(added_notes(slot)):
        say(line)


def notes_lines(notes, shown=INVENTED_SHOWN):
    """The block report_notes prints: the BUG and RULE-EXCEPTION notes added and the MATCH-PENDING notes removed, each
    in full; then the added lines marked invented (helpers, names, text: AGENTS.md, "Policies"), grouped by file and
    cut after `shown` lines with a count of the rest."""
    out = []
    reviewed = [n for n in notes if n[2] != "invented"]
    invented = [n for n in notes if n[2] == "invented"]
    if not reviewed:
        out.append("  BUG / RULE-EXCEPTION notes added, MATCH-PENDING notes removed: none")
    else:
        out.append("  BUG / RULE-EXCEPTION notes added, MATCH-PENDING notes removed: %d (review each: does retail's code"
                   " show the defect, the rule the bytes defeat, the fix of the pending finding?)" % len(reviewed))
        for path, line, kind, text in reviewed:
            out.append("    %s:%d  %s%s" % (path, line, "[removed] " if kind.startswith("removed") else "", text[:150]))
    if not invented:
        out.append("  invented helpers, names and text added: none")
        return out
    by_file = {}
    for path, line, _, text in invented:
        by_file.setdefault(path, []).append((line, text))
    out.append("  invented helpers, names and text added: %d in %d files (review each: is it marked, and is there "
               "no evidence for the original?)" % (len(invented), len(by_file)))
    left = shown
    for path in sorted(by_file):
        items = by_file[path]
        out.append("    %s (%d)" % (path, len(items)))
        for line, text in items[:max(left, 0)]:
            out.append("      :%d  %s" % (line, text[:150]))
        if len(items) > max(left, 0):
            out.append("      ... and %d more" % (len(items) - max(left, 0)))
        left -= len(items)
    return out


def mark_accepted_unannotated(accept):
    """The baseline keeps rows whose annotation is gone, so a loss accepted at one merge would come back as a
    regression at every later one. Mark each accepted address the build no longer annotates as an accepted loss
    (status ACCEPTED-LOST, scores and history kept), so `regress` stops counting it."""
    if not accept:
        return
    path = MAIN / "config" / "baseline.json"
    base = cards.load_json(path)
    present = {r["addr"] for r in cards.load_json(MAIN / "build" / "report.json")["functions"]}
    wanted = {int(a, 16) for a in accept}
    n = 0
    for addr, row in base.items():
        if int(addr, 16) in wanted and addr not in present and row.get("status") == "MATCH":
            row["status"] = "ACCEPTED-LOST"
            n += 1
    if n:
        with open(path, "w", encoding="utf-8", newline="\n") as f:
            json.dump(base, f, indent=1)
            f.write("\n")
        say("  baseline: %d accepted unannotated losses marked ACCEPTED-LOST" % n)


def check(slot):
    """A worker's pre-merge check: everything the merge will test, without touching main or the branch.

    Clean tree, build, the prune (committed when it drops lines), regress at 0, the [match] rule findings in the lane's units, the gate,
    a list of the PARKED notes the lane added, and a card entry for every open function the lane changed. Prints the lines a final report should quote."""
    slot = Path(slot).resolve()
    branch = git(slot, "rev-parse", "--abbrev-ref", "HEAD").strip()
    say("checking %s in %s" % (branch, slot))
    require_clean(slot)
    if unmerged(slot):
        raise Stop("unmerged paths")
    behind = git(slot, "rev-list", "--count", "HEAD..main").strip()
    if behind != "0":
        say("  note: main is %s commits ahead; the merge will rebase" % behind)
    changed, baseline, ignored = judge_changes(slot, "main...HEAD")
    if baseline or ignored:
        require_judges(slot, "main...HEAD", True)   # stops: the merge would refuse these whatever its flags
    if changed:
        say("  note: the lane changes what judges it (%s); the merge needs --allow-tooling, and the orchestrator "
            "reviews that diff first" % ", ".join(changed))
    out = check_build(slot, "the lane")
    problems = [l for l in out.splitlines() if l.startswith(("COMPILE FAILED", "SYMBOL CONFLICT", "DATA:"))]
    for p in problems:
        say("  " + p)
    # The prune is part of the lane: commit it here, so what this check gates is the tree the merge will see.
    dropped, conflicts = prune(slot)
    if git(slot, "diff", "--ignore-cr-at-eol", "--numstat", "--", "config/symbols.txt").strip():
        commit(slot, "match: drop symbols.txt entries now defined by annotations", "config/symbols.txt")
        say("  prune dropped %d symbols.txt entries (committed)" % dropped)
    else:
        git(slot, "checkout", "--", "config/symbols.txt")
        say("  prune: nothing to drop")
    for c in conflicts:
        say("  symbols.txt: " + c)
    fix_statics(slot)
    rc, out = decomp(slot, "regress")
    say("  " + last_line(out, "regressions"))
    for l in out.splitlines():
        if l.startswith("REGRESSION"):
            say("  " + l)
    record_flips(slot, sensitive_flips(out))
    changed_units = sorted({Path(f).with_suffix("").as_posix()[4:] for f in
                            git(slot, "diff", "--name-only", "main...HEAD").split()
                            if f.startswith("src/") and f.endswith((".c", ".cpp"))})
    if changed_units:
        rc, out = decomp(slot, "rules", *changed_units)
        for l in out.splitlines():
            if "[match]" in l:
                say("  " + l)
        say("  " + last_line(out, "rule findings"))
    diff = git(slot, "diff", "main...HEAD", "--", "src")
    parked = [l[1:].strip() for l in diff.splitlines() if l.startswith("+") and "PARKED:" in l]
    if parked:
        say("  PARKED notes added: %d" % len(parked))
        for p in parked:
            say("    " + p[:160])
    report_notes(slot)
    miss = cards.missing("main", slot)
    for k, row in miss:
        say("  NO-TRY %s %s (%s %.1f)" % (k, row.get("name"), row.get("status"), row.get("score") or 0))
    if miss:
        raise Stop("%d open functions changed without a card entry: record each try with\n"
                   "  python tools/cards.py add <addr> --by <model> --how \"<what changed, what it showed>\" "
                   "[--wall <class>]\nand commit cards/" % len(miss))
    gate(slot, "the lane")
    say("ready to merge: %s" % branch)


def build_parser():
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    sub = ap.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("merge")
    p.add_argument("slot")
    p.add_argument("-m", "--message", required=True, help="the merge commit's subject, e.g. 'merge: lane/x (...)'")
    p.add_argument("--accept", nargs="+", action="extend", default=[], metavar="ADDR",
                   help="lost matches the user adjudicated as expected (passed to decomp.py regress); repeatable, "
                        "and every list given is kept")
    p.add_argument("--allow-tooling", action="store_true",
                   help="the lane changes tools/ or config/project.json, and the orchestrator reviewed that diff: "
                        "let the lane's own tools and flags judge it")
    p = sub.add_parser("check", help="a worker's pre-merge check of its slot (no merge)")
    p.add_argument("slot", nargs="?", default=".")
    p = sub.add_parser("resolve-symbols")
    p.add_argument("path", nargs="?", default="config/symbols.txt")
    p = sub.add_parser("prune")
    p.add_argument("checkout", nargs="?", default=".")
    p = sub.add_parser("counts")
    p.add_argument("checkout", nargs="?", default=".")
    return ap


def main():
    args = build_parser().parse_args()
    try:
        if args.cmd == "merge":
            merge(args.slot, args.message, args.accept, args.allow_tooling)
        elif args.cmd == "check":
            check(args.slot)
        elif args.cmd == "resolve-symbols":
            for c in resolve_symbols(args.path):
                print("CONFLICT " + c)
        elif args.cmd == "prune":
            dropped, conflicts = prune(Path(args.checkout).resolve())
            print("dropped %d" % dropped)
            for c in conflicts:
                print("CONFLICT " + c)
        elif args.cmd == "counts":
            readme_counts(Path(args.checkout).resolve())
    except Stop as e:
        print("STOP: %s" % e, file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
