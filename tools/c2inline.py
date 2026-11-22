"""The inline driver's arithmetic (docs/c2-backend.md, "Inline expansion"), on a trace recorded by tools/c2trace.py.

c2trace records every site the driver visits as a tree (functions -> sites -> the sites of each expanded copy). This
module works on that record without running the compiler again:

- `margin_text`: what a site's verdict needs to flip, from the share formula (a nested level gets the budget left
  at its parent site, after the parent's own charge, divided by the parent level's sites not yet visited);
- `simulate`: replays the driver's arithmetic over a (possibly edited) record, and `whatif` reports which verdicts
  change with one more or one fewer site at a position, or a different own cost;
- `retail_compare`: sets the trace's out-of-line sites against retail's out-of-line calls per callee
  (tools/callset.py's data) and names the sites of ours that can account for a count mismatch;
- `statements` / `worth`: the front end's tuples cut into statements, and those (or adjacent pairs) worth N tuples.

A record is plain JSON: {"name", "raw", "own", "budget", "sites": [site]}, a site being {"callee", "raw", "cost",
"budget" (left when it was visited), "force", "limit", "depth", "params", "args", "verdict" ("inline", "reject" or
"late"), "why" (the budget/depth/total reasons of a rejection), "share" (an expanded site: its copy's budget),
"nested" (an expanded site: its copy's sites, else null)}.
"""
import copy
import re

FREE = 40          # a callee of cost <= 40 is never charged and never fails on the budget
TOTAL_CAP = 35000  # no site is expanded once the running total exceeds this
FLOOR = 1000       # the starting budget is max(1000, 2 * own cost)


def start_budget(own):
    return max(FLOOR, 2 * own)


def cdiv(a, b):
    """C's integer division (idiv): truncates toward zero."""
    q = abs(a) // abs(b)
    return q if (a < 0) == (b < 0) else -q


def charged(site):
    return not site["force"] and site["cost"] > FREE


# --- margins ---------------------------------------------------------------------------------------------------------

def fewer_sites_for(L, R, E, C):
    """Smallest k (1 <= k < R) with L // (R - k) - E >= C: sites to remove at the level above so this share fits."""
    if C + E <= 0:
        return 1
    q = L // (C + E)
    if q < 1:
        return None
    k = R - q
    return k if 1 <= k < R else None


def more_sites_for(L, R, E, C):
    """Smallest k >= 1 with L // (R + k) - E < C: sites to add at the level above so this share no longer fits."""
    T = C + E - 1          # the share must drop to T or below
    if T < 0:
        return None
    return max(1, L // (T + 1) + 1 - R)


def own_cost_for(budget0, own, target):
    """Own-cost change that moves the starting budget max(1000, 2 * own) to `target` (None when it can't)."""
    if target > budget0:
        return (target + 1) // 2 - own
    if target < FLOOR:
        return None
    return target // 2 - own


def margin_text(site, parent=None, fn=None, margin=10):
    """One line stating what flips the site's verdict, or None. `parent` (a nested site): {"L": budget left at the
    parent site after its charge, "R": the parent level's sites not yet visited, "pos": "[i/n]", "share": S}.
    `fn` (a top-level site): {"own", "budget"}."""
    C, B = site["cost"], site["budget"]
    if site["force"] or C <= FREE:
        return None
    if site["verdict"] == "reject":
        if not any(w.startswith("cost ") for w in site.get("why", ())) or len(site.get("why", ())) != 1:
            return None
        N = C - B
        text = "rejected: needs >= %d more units here" % N
        if parent:
            L, R, S = parent["L"], parent["R"], parent["share"]
            E = S - B
            X = (S + N) * R - L
            k = fewer_sites_for(L, R, E, C)
            text += " (share %d -> %d: >= %d more left at %s above%s)" % (
                S, S + N, X, parent["pos"],
                ", or %d fewer sites from there on" % k if k else "")
        elif fn:
            d = own_cost_for(fn["budget"], fn["own"], fn["budget"] + N)
            text += " (own cost +%d, or %d fewer units spent before it)" % (d, N)
        return text
    if site["verdict"] == "inline":
        m = B - C
        if m >= margin:
            return None
        N = m + 1
        text = "inlined: flips with >= %d fewer units here" % N
        if parent:
            L, R, S = parent["L"], parent["R"], parent["share"]
            E = S - B
            X = L - (S - m) * R + 1
            k = more_sites_for(L, R, E, C)
            text += " (share %d -> %d: >= %d fewer left at %s above, or %d more sites after it there)" % (
                S, S - N, X, parent["pos"], k)
        elif fn:
            d = own_cost_for(fn["budget"], fn["own"], fn["budget"] - N)
            text += " (%s, or %d more units spent before it)" % (
                "own cost %d" % d if d is not None else "own cost can't: the budget is at the 1000 floor", N)
        return text
    return None


# --- replay ----------------------------------------------------------------------------------------------------------

def templates(functions):
    """{callee raw name: nested sites} from every expansion in the record: a callee's copy has the same sites
    wherever it is expanded (its IL is copied), so a rejected site can borrow them."""
    out = {}

    def walk(sites):
        for s in sites:
            if s.get("nested") is not None:
                out.setdefault(s["raw"], s["nested"])
                walk(s["nested"])
    for f in functions:
        walk(f["sites"])
    return out


def simulate(fn, tmpl=None):
    """Replay the driver over fn's sites; each site gets "sim" = {"verdict", "budget", "share", "unknown"}.
    Static outcomes are kept from the record: a depth or argument rejection, a late rejection after the gate."""
    tmpl = tmpl or {}
    state = {"total": fn["own"]}
    if 2 * fn["own"] > TOTAL_CAP:
        return fn

    def static_reject(s):
        if any(w.startswith("depth ") for w in s.get("why", ())):
            return True
        if s.get("args") is not None and s.get("params") is not None and s["args"] != s["params"]:
            return True
        return s["verdict"] == "reject" and not s.get("why")

    def level(sites, budget):
        start = budget
        n = len(sites)
        for i, s in enumerate(sites):
            sim = {"budget": budget, "share": None, "unknown": False}
            s["sim"] = sim
            if static_reject(s):
                sim["verdict"] = "reject"
                continue
            if not s["force"] and ((s["cost"] > budget and s["cost"] > FREE) or state["total"] > TOTAL_CAP):
                sim["verdict"] = "reject"
                continue
            if charged(s):
                budget -= s["cost"]
            if not s["force"]:
                state["total"] += s["cost"]
            if s["verdict"] == "late":
                sim["verdict"] = "late"
                continue
            sim["verdict"] = "inline"
            nested = s.get("nested")
            if nested is None:
                nested = tmpl.get(s["raw"])
                if nested is not None:
                    nested = copy.deepcopy(nested)
                    s["nested"] = nested
            if nested is None:
                sim["unknown"] = True
                continue
            share = cdiv(budget, n - i)
            sim["share"] = share
            used = level(nested, share)
            if not s["force"]:
                budget -= used
                state["total"] += used
        return start - budget

    level(fn["sites"], start_budget(fn["own"]))
    return fn


def walk(sites, path=()):
    for i, s in enumerate(sites):
        p = path + (i + 1,)
        yield p, s, len(sites)
        if s.get("nested"):
            yield from walk(s["nested"], p)


def path_text(p):
    return ".".join(str(x) for x in p)


def check_replay(fn):
    """Sites where replaying the record disagrees with what the compiler did (the arithmetic is incomplete there)."""
    f = simulate(copy.deepcopy(fn))
    out = []
    for p, s, _ in walk(f["sites"]):
        sim = s.get("sim")
        if sim is None:
            continue
        if sim["verdict"] != s["verdict"] or sim["budget"] != s["budget"] or (
                s.get("share") is not None and sim["share"] != s["share"]):
            out.append((p, s, sim))
    return out


EDIT = re.compile(r"^(?:site=(?P<path>\d+(?:\.\d+)*),(?P<delta>[+-]1)(?:,cost=(?P<cost>\d+))?|own=(?P<own>[+-]\d+))$")


def parse_edit(text):
    m = EDIT.match(text.strip())
    if not m:
        raise ValueError("--whatif takes site=K[.J...],+1|-1[,cost=N] or own=+N|-N, not %r" % text)
    if m.group("own"):
        return {"own": int(m.group("own"))}
    return {"path": tuple(int(x) for x in m.group("path").split(".")), "delta": int(m.group("delta")),
            "cost": int(m.group("cost")) if m.group("cost") else 24}


def apply_edit(fn, e):
    if "own" in e:
        fn["own"] += e["own"]
        fn["budget"] = start_budget(fn["own"])
        return
    sites = fn["sites"]
    for k in e["path"][:-1]:
        if not (1 <= k <= len(sites)) or sites[k - 1].get("nested") is None:
            raise ValueError("no expanded site at %s" % path_text(e["path"][:-1]))
        sites = sites[k - 1]["nested"]
    k = e["path"][-1]
    if e["delta"] > 0:
        if not (1 <= k <= len(sites) + 1):
            raise ValueError("position %d is outside 1..%d" % (k, len(sites) + 1))
        depth = sites[0]["depth"] if sites else len(e["path"])
        sites.insert(k - 1, {"callee": "(added site, cost %d)" % e["cost"], "raw": "(added)", "cost": e["cost"],
                             "budget": 0, "force": False, "limit": 8, "depth": depth, "params": 0, "args": 0,
                             "verdict": "inline", "why": [], "added": True, "nested": []})
    else:
        if not (1 <= k <= len(sites)):
            raise ValueError("no site %d (1..%d)" % (k, len(sites)))
        sites[k - 1]["removed"] = True
        del sites[k - 1]


def whatif(fn, edits, functions=()):
    """[(callee, before, after)] for the sites whose verdict, budget or share change under the edits, keyed by the
    recorded site (sites borrowed from a template or added are reported by their new position)."""
    tmpl = templates(list(functions) or [fn])
    base = simulate(copy.deepcopy(fn), tmpl)
    before = {}
    for p, s, n in walk(base["sites"]):
        s["_id"] = p
        before[p] = (s, n)
    f = copy.deepcopy(base)
    for e in edits:
        apply_edit(f, e)
    for p, s, n in walk(f["sites"]):
        s.pop("sim", None)
    simulate(f, tmpl)
    rows = []
    for p, s, n in walk(f["sites"]):
        sim = s.get("sim")
        if sim is None:
            continue
        old = before.get(s.get("_id")) if s.get("_id") else None
        if old is None:
            rows.append((p, n, s, None, sim))
            continue
        os_ = old[0]["sim"]
        if os_["verdict"] != sim["verdict"] or os_["share"] != sim["share"] or os_["budget"] != sim["budget"]:
            rows.append((p, n, s, os_, sim))
    return base, f, rows


def verdict_word(v):
    return {"inline": "inlined", "reject": "not inlined", "late": "late reject"}.get(v, v)


def report_whatif(fn, edits, functions, say):
    base, f, rows = whatif(fn, edits, functions)
    say("")
    say("what if %s, in %s (own cost %d, budget %d -> own cost %d, budget %d):"
        % (", ".join(edit_text(e) for e in edits), fn["name"], fn["own"], start_budget(fn["own"]),
           f["own"], start_budget(f["own"])))
    flips = 0
    for p, n, s, old, new in rows:
        tag = ""
        if old is None:
            tag = "  (added)" if s.get("added") else "  (copy's sites, borrowed from another expansion)"
            if not s.get("added"):
                continue
        if old is not None and old["verdict"] != new["verdict"]:
            flips += 1
            say("  [%s] %-46s cost %4d  budget %5d -> %5d  %s -> %s%s"
                % (path_text(p), s["callee"], s["cost"], old["budget"], new["budget"], verdict_word(old["verdict"]),
                   verdict_word(new["verdict"]), "  (its copy's sites are unknown)" if new.get("unknown") else ""))
        elif old is None:
            say("  [%s] %-46s cost %4d  budget %5d  %s%s" % (path_text(p), s["callee"], s["cost"], new["budget"],
                                                           verdict_word(new["verdict"]), tag))
    budget_moves = sum(1 for _, _, _, old, new in rows if old is not None and old["verdict"] == new["verdict"])
    say("  %d verdicts flip; %d other sites keep their verdict with another budget or share"
        % (flips, budget_moves))
    return flips


def edit_text(e):
    if "own" in e:
        return "own cost %+d" % e["own"]
    return "%s site at %s%s" % ("one more" if e["delta"] > 0 else "one fewer", path_text(e["path"]),
                                " (cost %d)" % e["cost"] if e["delta"] > 0 else "")


# --- retail ----------------------------------------------------------------------------------------------------------

def out_of_line(fn):
    """{callee raw: [(path, site)]} of the trace's sites, split as (inlined, out of line)."""
    inl, out = {}, {}
    for p, s, _ in walk(fn["sites"]):
        (inl if s["verdict"] == "inline" else out).setdefault(s["raw"], []).append((p, s))
    return inl, out


def retail_compare(fn, retail_calls, ours_calls=None):
    """Per callee of the trace: [(callee, retail n or None, ours n, candidates, note)].
    `retail_calls`: {callee raw: retail's out-of-line calls of it (summed over its copies)}, None when it has no
    address. `ours_calls`: the same counted in our compiled object (tools/callset.py), to flag a stale build."""
    inl, out = out_of_line(fn)
    rows = []
    for raw in sorted(set(inl) | set(out), key=lambda r: min(p for p, _ in inl.get(r, []) + out.get(r, []))):
        sites_in, sites_out = inl.get(raw, []), out.get(raw, [])
        name = (sites_in or sites_out)[0][1]["callee"]
        ours = len(sites_out)
        r = retail_calls.get(raw)
        note, cands = None, []
        if r is None:
            if ours:
                note = "no address for this callee: compare by hand"
        elif r > ours:
            # retail calls it more: some of ours' expansions were out of line in retail; the tightest first
            cands = sorted(sites_in, key=lambda ps: (ps[1]["budget"] - ps[1]["cost"]
                                                     if charged(ps[1]) else 10 ** 6, -len(ps[0]), ps[0]))
            note = "retail has %d more out of line: of ours' %d expanded site%s, these could be them" % (
                r - ours, len(sites_in), "" if len(sites_in) == 1 else "s")
        elif r < ours:
            cands = sorted(sites_out, key=lambda ps: (ps[1]["cost"] - ps[1]["budget"]
                                                      if ps[1]["verdict"] == "reject" and ps[1].get("why")
                                                      and all(w.startswith("cost ") for w in ps[1]["why"])
                                                      else 10 ** 6, ps[0]))
            note = "retail expands %d more: of ours' %d out-of-line site%s, these could be them" % (
                ours - r, len(sites_out), "" if len(sites_out) == 1 else "s")
        if ours_calls is not None and raw in ours_calls and ours_calls[raw] != ours:
            note = (note + "; " if note else "") + "our object has %d calls (build/obj is stale, or calls outside " \
                                                   "the trace)" % ours_calls[raw]
        rows.append((name, raw, r, ours, len(sites_in), cands, note))
    return rows


def report_retail(fn, retail_calls, ours_calls, say, unnamed=()):
    """`unnamed`: [(address, n)] of retail's calls from this function to code nothing names yet."""
    rows = retail_compare(fn, retail_calls, ours_calls)
    say("")
    say("retail against the trace, %s (out-of-line calls per callee; retail's from tools/callset.py):" % fn["name"])
    say("%6s %6s %8s  %s" % ("retail", "ours", "inlined", "callee"))
    differ = 0
    for name, raw, r, ours, n_in, cands, note in rows:
        mark = "" if r is None or r == ours else "  <- differs"
        differ += bool(mark)
        say("%6s %6d %8d  %s%s" % ("?" if r is None else r, ours, n_in, name, mark))
        if note and (mark or r is None or "stale" in note):
            say("         %s" % note)
        for p, s in cands[:8]:
            v = s["verdict"]
            if v == "inline":
                extra = ("margin %d" % (s["budget"] - s["cost"])) if charged(s) else "free (cost <= 40)"
            else:
                extra = ("needs %d more" % (s["cost"] - s["budget"])) if s.get("why") and all(
                    w.startswith("cost ") for w in s["why"]) else "; ".join(s.get("why") or ["not a budget reject"])
            say("           [%s] cost %d budget %d %s: %s" % (path_text(p), s["cost"], s["budget"], verdict_word(v),
                                                             extra))
        if len(cands) > 8:
            say("           ... %d more" % (len(cands) - 8))
    say("%d callee%s differ" % (differ, "" if differ == 1 else "s"))
    loose = [(name, ours) for name, raw, r, ours, n_in, cands, note in rows if r is None and ours]
    if unnamed:
        say("retail also calls code without an identity: %s" % ", ".join("%s x%d" % t for t in unnamed))
        if loose:
            say("  ours calls out of line, with no address to compare: %s; a pair with the same count is a lead "
                "for that address's identity, not evidence" % ", ".join("%s x%d" % t for t in loose))
    return differ


# --- front-end statements --------------------------------------------------------------------------------------------

ENDS = {0x4B, 0x41, 0x3A, 0x38}     # discard, return, jump, branch-false
SEQ, DISCARD = 0x4C, 0x4B


def statements(ops):
    """The tuples cut where a statement ends: after a discard, a return, a jump or a conditional branch, and after a
    sequence point not followed by a discard. A heuristic: the front end's IL has no statement markers."""
    out, cur = [], []
    for i, o in enumerate(ops):
        cur.append(o)
        nxt = ops[i + 1] if i + 1 < len(ops) else None
        if o in ENDS or (o == SEQ and nxt != DISCARD):
            out.append(cur)
            cur = []
    if cur:
        out.append(cur)
    return out


def worth(ops, n):
    """([(index, statement)] worth exactly n tuples, [(index, first, second)] adjacent pairs summing to n)."""
    st = statements(ops)
    single = [(i + 1, s) for i, s in enumerate(st) if len(s) == n]
    pairs = [(i + 1, st[i], st[i + 1]) for i in range(len(st) - 1) if len(st[i]) + len(st[i + 1]) == n]
    return single, pairs
