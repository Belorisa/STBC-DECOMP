"""Function cards: one append-only history per open function, in cards/<addr>.jsonl.

    python tools/cards.py show 0x5AD450                  # the card: summary, then every entry
    python tools/cards.py add 0x5AD450 --by sonnet --how "inlined GetSubsystem" [--wall register] [--group ID]
    python tools/cards.py tag 0x41F350 0x481CC0 --group ID --by opus --how "why these share one wall"
    python tools/cards.py missing [main]                 # open functions this branch changed without a "try" entry
    python tools/cards.py backfill                       # create cards from the baseline's git history
    python tools/cards.py record OLD_BASELINE NEW_BASELINE --lane game-102 --commit abc1234

A card is a data record (like config/baseline.json), not documentation: it keeps what was tried on a function and
what each try did to its score, so the next worker starts where the last one stopped. Only open functions have
cards (anything but a MATCH without a pending rule finding); a card stays when its function matches, closed by a
final entry.

Each line is one JSON entry:
    {"date", "kind": "score", "lane", "commit", "from", "to", "status"}      written by the merge (record)
    {"date", "kind": "try",   "lane", "by", "from", "to", "how", "wall", "group"}   written by the worker (add)
    {"date", "kind": "group", "lane", "by", "group", "how"}                 written by the worker (tag)
"from" is the score before (None for a function first seen), "to" the score after. "wall" is the wall-identifier
class when one was diagnosed: inline, control-flow, register, referent, structure, or compiler-state (a wall proven
bounded: several spellings A/B'd, retail's own sites contradicting each other; still open work, listed apart).
"group" names a shared wall: functions that one fix (a header body, a lever) would close or fail together. A try
carries it when the worker tags the function while trying it (`add --group`); `tag` puts functions in a group
without a try. `openwork.py groups` lists the groups.

`add` on an address no annotation covers is accepted when it lies in an unwritten gap of openwork's scan, or with
`--gap`; such a card has no scores.

`add` takes "from" from config/baseline.json (the score when the lane started) and "to" from build/report.json
(run `decomp.py build` or `match` first). `merge_lane.py check` stops a lane whose changed open functions have no
new "try" entry, and `merge_lane.py merge` records the score moves of every merge.
"""

import argparse
import datetime
import json
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CARDS = "cards"
WALLS = ("inline", "control-flow", "register", "referent", "structure", "compiler-state")
GROUP_ID = re.compile(r"[a-z0-9][a-z0-9.-]{0,47}")   # a short lower-case slug: nitlistblock-base-index


def key(addr):
    """Baseline-style key of an address: 0x5AD450."""
    return "0x%X" % (addr if isinstance(addr, int) else int(str(addr), 16))


def card_path(addr, root=ROOT):
    return Path(root) / CARDS / ("%s.jsonl" % key(addr))


def read(addr, root=ROOT):
    p = card_path(addr, root)
    if not p.exists():
        return []
    return [json.loads(l) for l in p.read_text(encoding="utf-8").splitlines() if l.strip()]


def append(addr, entries, root=ROOT):
    p = card_path(addr, root)
    p.parent.mkdir(exist_ok=True)
    with p.open("a", encoding="utf-8", newline="\n") as f:
        for e in entries:
            f.write(json.dumps(e, sort_keys=False) + "\n")


def is_open(row):
    # a PATCHED function (binary-patched in retail, written as the original) is listed apart, not open work
    return bool(row) and row.get("status") != "PATCHED" and not (row.get("status") == "MATCH" and not row.get("pending"))


def score_of(row):
    if row is None:
        return None
    if isinstance(row, (int, float)):
        return float(row)
    return row.get("score")


def moved(old, new):
    """Whether a baseline row's change is a real step: first sight, its own source changed, or it gained or lost a
    match. A score that moves while the source stays (a neighbour's layout, an unstable referent) is not."""
    if old is None:
        return True
    if not isinstance(old, dict) or not isinstance(new, dict):
        return abs((score_of(new) or 0) - (score_of(old) or 0)) >= 0.5
    if old.get("src") and new.get("src") and old["src"] != new["src"]:
        return score_of(old) != score_of(new) or old.get("status") != new.get("status")
    return is_open(old) != is_open(new)


def git(*args, cwd=ROOT):
    return subprocess.run(["git", *args], cwd=str(cwd), capture_output=True, text=True, errors="replace").stdout


def lane_name(text):
    m = re.search(r"lane/([\w.-]+)", text or "")
    return m.group(1) if m else None


def today():
    return datetime.date.today().isoformat()


def load_json(path):
    try:
        return json.loads(Path(path).read_text(encoding="utf-8"))
    except (OSError, ValueError):
        return {}


def report_rows(root=ROOT):
    rows = load_json(Path(root) / "build" / "report.json").get("functions", [])
    return {key(r["addr"]): r for r in rows}


# ---------------------------------------------------------------------------
# the merge's record and the backfill

def score_entries(old, new, lane, commit, date, root=ROOT):
    """Score entries for every function whose score moved between two baselines: open functions, and functions
    that already have a card (a match closes the card)."""
    out = {}
    for k, row in new.items():
        before, after = score_of(old.get(k)), score_of(row)
        if not moved(old.get(k), row):
            continue
        if not (is_open(row) if isinstance(row, dict) else after < 100.0) and not card_path(k, root).exists():
            continue
        status = row.get("status") if isinstance(row, dict) else None
        if isinstance(row, dict) and row.get("pending"):
            status = "MATCH-PENDING"
        out[k] = {"date": date, "kind": "score", "lane": lane, "commit": commit, "from": before, "to": after,
                  "status": status}
    return out


def record(old_path, new_path, lane, commit, root=ROOT):
    entries = score_entries(load_json(old_path), load_json(new_path), lane, commit, today(), root)
    for k, e in entries.items():
        append(k, [e], root)
    return len(entries)


def backfill(root=ROOT):
    """Cards for the functions open in the current baseline, from every committed version of the baseline. A
    function that already has a card is left alone."""
    current = load_json(Path(root) / "config" / "baseline.json")
    wanted = {k for k, r in current.items() if is_open(r) and not card_path(k, root).exists()}
    log = git("log", "--reverse", "--format=%H%x09%ad%x09%s", "--date=short", "--", "config/baseline.json",
              cwd=root).splitlines()
    cards = {k: [] for k in wanted}
    last = {}   # each function's row at its last recorded entry
    for line in log:
        sha, date, subject = line.split("\t", 2)
        text = git("show", "%s:config/baseline.json" % sha, cwd=root)
        try:
            base = json.loads(text)
        except ValueError:
            continue
        lane = lane_name(subject) or subject.split(":")[0]
        for k in wanted:
            if k not in base:
                continue
            row = base[k]
            if not moved(last.get(k), row):
                continue
            before, after = score_of(last.get(k)), score_of(row)
            last[k] = row
            status = row.get("status") if isinstance(row, dict) else None
            if isinstance(row, dict) and row.get("pending"):
                status = "MATCH-PENDING"
            cards[k].append({"date": date, "kind": "score", "lane": lane, "commit": sha[:10], "from": before,
                             "to": after, "status": status})
    made = 0
    for k, entries in cards.items():
        if entries:
            append(k, entries, root)
            made += 1
    return made


# ---------------------------------------------------------------------------
# the worker's side

def group_id(text):
    """A group id as given, checked: lower-case letters, digits, '-' and '.'."""
    if not GROUP_ID.fullmatch(text or ""):
        raise SystemExit("group id %r: use a short lower-case slug (letters, digits, '-', '.')" % text)
    return text


def in_gap(addr):
    """True when addr starts or lies inside an unwritten gap of openwork's scan (the bytes between annotated functions,
    a gap after a non-exact function's tail included): where a lane has findings on a function nobody has written."""
    sys.path.insert(0, str(Path(__file__).resolve().parent))
    import openwork   # here, not at the top: the card commands don't need the build's module
    rows, _ = openwork.load()
    gaps, tails, _, _ = openwork.unwritten_gaps(rows)
    a = int(str(addr), 16)
    return any(lo <= a < hi for lo, hi, _, _ in gaps + tails)


def add(addr, how, by, wall=None, root=ROOT, group=None, gap=False):
    k = key(addr)
    base = load_json(Path(root) / "config" / "baseline.json").get(k)
    row = report_rows(root).get(k)
    if row is None:
        if not (gap or in_gap(addr)):
            raise SystemExit("%s is not in build/report.json: annotate it and run decomp.py build or match first "
                             "(an address in an unwritten gap, or --gap, records a card without a function)" % k)
        row = {}   # a gap address: no score, the card holds the findings
    branch = git("rev-parse", "--abbrev-ref", "HEAD", cwd=root).strip()
    e = {"date": today(), "kind": "try", "lane": lane_name(branch) or branch, "by": by,
         "from": score_of(base), "to": row.get("score"), "how": how}
    if wall:
        e["wall"] = wall
    if group:
        e["group"] = group_id(group)
    append(k, [e], root)
    return e


def tag(addrs, group, how, by, root=ROOT):
    """Put functions in a shared-wall group without a try: one "group" entry on each card."""
    group = group_id(group)
    branch = git("rev-parse", "--abbrev-ref", "HEAD", cwd=root).strip()
    out = []
    for a in addrs:
        e = {"date": today(), "kind": "group", "lane": lane_name(branch) or branch, "by": by, "group": group,
             "how": how}
        append(key(a), [e], root)
        out.append(key(a))
    return out


def groups(root=ROOT):
    """{group: {address: [the card's entries that name the group]}} over every card."""
    out = {}
    folder = Path(root) / CARDS
    if not folder.is_dir():
        return out
    for p in sorted(folder.glob("0x*.jsonl")):
        for l in p.read_text(encoding="utf-8").splitlines():
            if '"group"' not in l:
                continue
            e = json.loads(l)
            if e.get("group"):
                out.setdefault(e["group"], {}).setdefault(key(p.stem), []).append(e)
    return out


HEADER = re.compile(r"\b([A-Za-z_][A-Za-z0-9_]*\.(?:h|inl|INL))\b")
# a try that names a header without changing it: the lane didn't own it, or only pointed at it
HEADER_NOTED = re.compile(r"off-limits|another lane|other lane|slot\d's header|not tried|not touched|not changed|"
                          r"header-level lead|would sit in|the fix (?:is|sits) in|needs [A-Za-z0-9_]+\.(?:h|inl)|"
                          r"read only|skipped", re.I)
HEADER_GROUP = "hdr-"   # a group slug naming a header change: hdr-nitlistblock-ctor


def header_tries(root=ROOT):
    """{header: {"worked": [(address, entry)], "noted": [(address, entry)]}} over every card's tries. A try that
    names a header (`NiTList.h`) or carries a `hdr-` group is "worked" when it changed or A/B'd the header, "noted"
    when it only pointed at it (off-limits, not tried: HEADER_NOTED). The text is free, so the split is a reading,
    not a record: the full entries are printed for the reader to judge."""
    out = {}
    folder = Path(root) / CARDS
    if not folder.is_dir():
        return out
    for p in sorted(folder.glob("0x*.jsonl")):
        for l in p.read_text(encoding="utf-8").splitlines():
            e = json.loads(l)
            how = e.get("how") or ""
            if e.get("kind") != "try" or not how:
                continue
            names = set(HEADER.findall(how))
            if (e.get("group") or "").startswith(HEADER_GROUP):
                names.add(e["group"])
            side = "noted" if HEADER_NOTED.search(how) else "worked"
            for h in names:
                out.setdefault(h, {"worked": [], "noted": []})[side].append((key(p.stem), e))
    return out


def touched_open(root=ROOT):
    """Open functions whose source this checkout changed against the baseline it started from (new ones too)."""
    base = load_json(Path(root) / "config" / "baseline.json")
    sys.path.insert(0, str(Path(__file__).resolve().parent))
    import decomp   # here, not at the top: the card commands don't need the build's module
    ledger = Path(root) / "config" / "exceptions.md"
    # an import thunk (IMPORT-THUNK) or library data (LIBRARY-DATA) is listed apart, never open work, even when
    # something annotates its address
    text = ledger.read_text(encoding="utf-8") if ledger.exists() else ""
    thunks = {key(a) for status in ("IMPORT-THUNK", "LIBRARY-DATA") for a in decomp.listed_apart(status, text)}
    out = []
    for k, row in report_rows(root).items():
        if not is_open(row) or k in thunks:
            continue
        b = base.get(k)
        if b is None or not isinstance(b, dict) or b.get("src") != row.get("src"):
            out.append((k, row))
    return out


def missing(base_ref="main", root=ROOT):
    """Touched open functions with no "try" entry added since base_ref."""
    added = git("diff", "%s...HEAD" % base_ref, "--", CARDS, cwd=root)
    tried, current = set(), None
    for l in added.splitlines():
        if l.startswith("+++ "):
            m = re.search(r"(0x[0-9A-Fa-f]+)\.jsonl", l)
            current = key(m.group(1)) if m else None
        elif l.startswith("+") and current and '"kind": "try"' in l:
            tried.add(current)
    return [(k, row) for k, row in touched_open(root) if k not in tried]


# ---------------------------------------------------------------------------
# reading a card

def summary(addr, root=ROOT):
    """One line: score now and best, tries, last wall; None when there is no card."""
    entries = read(addr, root)
    if not entries:
        return None
    scores = [e["to"] for e in entries if e.get("to") is not None]
    tries = [e for e in entries if e["kind"] == "try"]
    walls = [e["wall"] for e in entries if e.get("wall")]
    line = "card: %d entries, %d tries, score %s, best %s" % (
        len(entries), len(tries), fmt(scores[-1] if scores else None), fmt(max(scores) if scores else None))
    if walls:
        line += ", wall %s" % walls[-1]
    return line


def fmt(s):
    return "-" if s is None else "%.1f" % s


def lines(addr, root=ROOT):
    out = []
    for e in read(addr, root):
        move = "%6s -> %-6s" % (fmt(e.get("from")), fmt(e.get("to")))
        tags = "".join("  [%s: %s]" % (t, e[t]) for t in ("wall", "group") if e.get(t))
        if e["kind"] == "try":
            out.append("%s  %-14s %-8s try    %s  %s%s" % (e["date"], e.get("lane") or "", e.get("by") or "", move,
                                                           e.get("how", ""), tags))
        elif e["kind"] == "group":
            out.append("%s  %-14s %-8s group  %s  %s" % (e["date"], e.get("lane") or "", e.get("by") or "",
                                                         e.get("group"), e.get("how", "")))
        else:
            out.append("%s  %-14s %-8s merge  %s  %s %s" % (e["date"], e.get("lane") or "", "", move,
                                                           e.get("status") or "", e.get("commit") or ""))
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    sub = ap.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("show")
    p.add_argument("addr", nargs="+")
    p = sub.add_parser("add")
    p.add_argument("addr")
    p.add_argument("--how", required=True, help="what this try changed, and what it showed")
    p.add_argument("--by", required=True, help="who tried: opus, sonnet, user")
    p.add_argument("--wall", choices=WALLS)
    p.add_argument("--group", help="the shared-wall group this function belongs to (a short slug)")
    p.add_argument("--gap", action="store_true", help="record a card for an address no annotation covers (a gap "
                   "address inside openwork's scan is accepted without it)")
    p = sub.add_parser("tag")
    p.add_argument("addr", nargs="+")
    p.add_argument("--group", required=True)
    p.add_argument("--by", required=True)
    p.add_argument("--how", required=True, help="why these functions share one wall")
    p = sub.add_parser("missing")
    p.add_argument("base", nargs="?", default="main")
    sub.add_parser("backfill")
    p = sub.add_parser("record")
    p.add_argument("old")
    p.add_argument("new")
    p.add_argument("--lane", required=True)
    p.add_argument("--commit", required=True)
    args = ap.parse_args()
    if args.cmd == "show":
        for a in args.addr:
            s = summary(a)
            print("# %s  %s" % (key(a), s or "no card"))
            for l in lines(a):
                print("  " + l)
    elif args.cmd == "add":
        e = add(args.addr, args.how, args.by, args.wall, group=args.group, gap=args.gap)
        print("%s: try %s -> %s" % (key(args.addr), fmt(e["from"]), fmt(e["to"])))
    elif args.cmd == "tag":
        print("%s: %s" % (args.group, ", ".join(tag(args.addr, args.group, args.how, args.by))))
    elif args.cmd == "missing":
        miss = missing(args.base)
        for k, row in miss:
            print("NO-TRY %s %s (%s %.1f)" % (k, row.get("name"), row.get("status"), row.get("score") or 0))
        print("missing try entries: %d" % len(miss))
        return 1 if miss else 0
    elif args.cmd == "backfill":
        print("cards created: %d" % backfill())
    elif args.cmd == "record":
        print("score entries: %d" % record(args.old, args.new, args.lane, args.commit))
    return 0


if __name__ == "__main__":
    sys.exit(main())
