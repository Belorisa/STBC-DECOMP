"""The deciding site of a schedule or operand-order difference (VC6 SP5 C2.DLL 8966): docs/c2-backend.md,
"Scheduling" and "Operand order of commutative operations".

    python tools/c2order.py 0x72C8A0                 # one function: the first divergence and what decided it
    python tools/c2order.py 0x72C8A0 --verbose       # with the probe's picks
    python tools/c2order.py triage 0x561330 0x72C8A0 # the first divergence's kind for each (no compile of C2)
    python tools/c2order.py triage --group schedule-only [--open]

What it does for one function:

1. compares ours (the unit's object in build/obj, relocated as `decomp.py diff` does) with retail instruction by
   instruction and names the first divergence: `reorder` (a window holding the same instructions in another order),
   `x87-swap` (a `fld A ... fop B` that retail writes `fld B ... fop A`: the commutative sort's operand order),
   `base-index` (`[a+b]` against `[b+a]`: the sort's order on the address sum, `c2trace --addr`), `register` (the
   same instructions with other registers) or `other`;
2. for a reorder, traces the scheduler on our source (`c2trace --sched`'s hooks), maps each of our instructions to
   its scheduler node, and finds the region and the nodes retail placed in another order;
3. reruns the real scheduler with the region's sequence numbers rewritten to retail's order (the probe: the
   scheduler's only use of the sequence number is to break ties between equal priorities, docs/c2-backend.md), and
   says which of two things retail's order needs:
   - **input order**: the probe reproduces retail's order, so the same dependence graph with another input order
     gives retail's bytes. It lists the ties that decide it: the pairs whose input order must flip, each with its
     instruction and source line. The input order is the code generator's, i.e. the order of the statements and of
     the evaluation within a statement;
   - **not the input order**: the probe still differs. It names the first pick where it does and why: the
     instruction retail needs there wasn't ready (the chain that held it, with every no-code node on the chain: an
     op 0x162 float-in-register or another tuple that emits nothing takes an issue slot of its own) or had the lower
     priority (the heights to make up).

For an x87 swap it prints the commutative sort's comparisons that involve a variable of the swapped pair
(`c2trace --operands`), whose weights decide the order: two same-shaped leaves order by symbol number, i.e. by which
was created first.
"""
import argparse
import difflib
import io
import json
import re
import sys
from collections import Counter
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

ROOT = Path(__file__).resolve().parent.parent

COMMUTATIVE_X87 = ("fadd", "fmul")
REVERSIBLE_X87 = {"fsub": "fsubr", "fsubr": "fsub", "fdiv": "fdivr", "fdivr": "fdiv"}
REG = re.compile(r"\b(?:e?[abcd]x|[abcd][lh]|e?[sd]i|e?[sb]p)\b")
NO_CODE = None   # a scheduler node that emits no instruction


# --- the first divergence (pure) -------------------------------------------------------------------------------------
def text(i):
    """An instruction's text with a branch's target masked: a size difference before the target moves it, which is
    not a difference of the branch."""
    ops = "T" if (i[1].startswith("j") or i[1] == "call") and re.fullmatch(r"0x[0-9a-f]+", i[2]) else i[2]
    return ("%s %s" % (i[1], ops)).strip()


def first_divergence(ours, retail, max_window=48):
    """The first place two instruction lists [(addr, mnem, ops, ...)] differ, as a dict:
    kind reorder (k, w: ours[k:k+w] and retail[k:k+w] hold the same instructions), x87-swap (k: the fld, j: the
    operation), base-index (k), register (k), other (k) or none."""
    n = min(len(ours), len(retail))
    k = next((i for i in range(n) if text(ours[i]) != text(retail[i])), None)
    if k is None:
        if len(ours) == len(retail):
            return {"kind": "none"}
        return {"kind": "other", "k": n, "note": "one listing is longer (%d against %d)" % (len(ours), len(retail))}
    o, r = ours[k], retail[k]
    # an x87 operand swap: fld A ... fop B against fld B ... fop A
    if o[1] == r[1] and o[1].startswith("fld") and o[2] != r[2]:
        for j in range(k + 1, min(k + 4, n)):
            oj, rj = ours[j], retail[j]
            if not (oj[1].startswith("f") and rj[1].startswith("f")):
                continue
            same_op = oj[1] == rj[1] and oj[1].rstrip("p") in COMMUTATIVE_X87
            reversed_op = REVERSIBLE_X87.get(oj[1]) == rj[1]
            if (same_op or reversed_op) and oj[2] == r[2] and rj[2] == o[2]:
                return {"kind": "x87-swap", "k": k, "j": j, "a": o[2], "b": r[2], "op": (oj[1], rj[1])}
    # base and index of a two-register address swapped
    m1, m2 = re.search(r"\[(\w+) \+ (\w+)", o[2]), re.search(r"\[(\w+) \+ (\w+)", r[2])
    if o[1] == r[1] and m1 and m2 and (m1.group(1), m1.group(2)) == (m2.group(2), m2.group(1)) and \
            o[2].replace(m1.group(0), "") == r[2].replace(m2.group(0), ""):
        return {"kind": "base-index", "k": k}
    # a window holding the same instructions in another order
    for w in range(2, max_window + 1):
        if k + w > n:
            break
        if Counter(text(x) for x in ours[k:k + w]) == Counter(text(x) for x in retail[k:k + w]):
            return {"kind": "reorder", "k": k, "w": w}
    if o[1] == r[1] and REG.sub("R", o[2]) == REG.sub("R", r[2]):
        # the same instruction with other registers here: a register choice, whatever follows
        return {"kind": "register", "k": k}
    # the same instructions in another order once registers are masked: the temporaries rotate with the order
    masked = lambda x: REG.sub("R", text(x))
    for w in range(2, max_window + 1):
        if k + w > n:
            break
        if Counter(masked(x) for x in ours[k:k + w]) == Counter(masked(x) for x in retail[k:k + w]):
            return {"kind": "reorder-regs", "k": k, "w": w}
    # a float copied with integer moves on one side and through the FPU on the other (tools/c2float.py)
    mem = lambda x: re.search(r"\[[^\]]*\]", x[2])
    if {o[1], r[1]} & {"fld"} and {o[1], r[1]} & {"mov", "push"} and mem(o) and mem(r) and             mem(o).group(0) == mem(r).group(0):
        return {"kind": "float-copy", "k": k}
    # another frame: the same instruction with another stack displacement, or another frame size (framemap.py)
    esp = re.compile(r"\[esp \+ 0x[0-9a-f]+\]|\[esp\]")
    if o[1] == r[1] and (esp.search(o[2]) or esp.search(r[2])) and esp.sub("S", o[2]) == esp.sub("S", r[2]):
        return {"kind": "frame", "k": k}
    if o[1] == r[1] == "sub" and o[2].startswith("esp,") and r[2].startswith("esp,"):
        return {"kind": "frame", "k": k}
    return {"kind": "other", "k": k}


def retail_permutation(ours, retail, k, w):
    """For a reorder window: the positions (0..w-1, into ours[k:k+w]) in retail's order."""
    for key in (text, lambda x: REG.sub("R", text(x))):
        used = set()
        perm = []
        for r in retail[k:k + w]:
            i = next((i for i in range(w) if i not in used and key(ours[k + i]) == key(r)), None)
            if i is None:
                break
            used.add(i)
            perm.append(i)
        if len(perm) == w:
            return perm
    return None


# --- our instructions against the scheduler's nodes (pure) ----------------------------------------------------------
def node_key(node_text):
    """The emitted mnemonic a scheduler node stands for, or NO_CODE."""
    m = node_text.split(" ", 1)[0]
    if m in ("_epush", "_rpush"):
        return "push"
    if m in ("_epop", "_rpop"):
        return "pop"
    if m == "_jcc":
        return "j"
    if m.startswith("op") or m.startswith("("):
        return NO_CODE
    return m.lstrip("_")


def ins_key(mnem):
    if mnem.startswith("j") and mnem != "jmp":
        return "j"
    if mnem in ("ret", "retn"):
        return "ret"
    return mnem


def map_picks(ins, regions):
    """{index into ins: (region index, node addr)}: our instructions against the scheduled nodes in output order,
    aligned by mnemonic (a region of fewer than two real instructions isn't scheduled, and no-code nodes emit
    nothing, so either side has extra entries)."""
    seq = []
    for ri, reg in enumerate(regions):
        nodes = {x["addr"]: x for x in reg["nodes"]}
        for p in reg["picks"]:
            key = node_key(nodes[p["addr"]]["text"])
            if key is not NO_CODE:
                seq.append((key, ri, p["addr"]))
    a = [ins_key(i[1]) for i in ins]
    b = [s[0] for s in seq]
    sm = difflib.SequenceMatcher(None, a, b, autojunk=False)
    out = {}
    for blk in sm.get_matching_blocks():
        for t in range(blk.size):
            out[blk.a + t] = seq[blk.b + t][1:]
    return out


def target_order(reg, moved):
    """The region's output order (node addrs) with the nodes of `moved` (a list of node addrs in retail's order)
    put in that order at the places our output holds them."""
    picks = [p["addr"] for p in reg["picks"]]
    slots = [i for i, a in enumerate(picks) if a in set(moved)]
    out = list(picks)
    for s, a in zip(slots, moved):
        out[s] = a
    return out


def seq_override(reg, order):
    """{original sequence number: new one} giving the nodes `order`'s positions."""
    by = {x["addr"]: x for x in reg["nodes"]}
    return {by[a]["orig"]: i + 1 for i, a in enumerate(order)}


def deciding_ties(probe, order, orig_seq):
    """The ties the probe broke the other way from our input order, in retail's favour: [(picked, passed)] node
    addrs, where both were ready at the pick with the same priority and `passed` came first in our input."""
    pos = {a: i for i, a in enumerate(order)}
    out = []
    for p in probe["picks"]:
        a = p["addr"]
        for r in p["ready"]:
            if r == a or p["dyn"].get(r) != p["dyn"].get(a) or p["earliest"].get(r, 0) > p["cycle"]:
                continue
            if pos.get(r, -1) > pos.get(a, -1) and orig_seq[r] < orig_seq[a]:
                out.append((a, r))
    return out


def reaches(reg, a, b):
    """Whether node b depends on node a (a path of predecessor edges from b back to a)."""
    by = {x["addr"]: x for x in reg["nodes"]}
    seen, todo = set(), [b]
    while todo:
        n = todo.pop()
        for p, _ in by[n].get("preds", []):
            if p == a:
                return True
            if p not in seen and p in by:
                seen.add(p)
                todo.append(p)
    return False


def memory_hints(reg, order):
    """[(x, y)]: memory operations retail puts in the order x, y where ours has y first and our graph has no
    dependence between them, one of them a store."""
    by = {x["addr"]: x for x in reg["nodes"]}
    ours = [p["addr"] for p in reg["picks"]]
    pos_o = {a: i for i, a in enumerate(ours)}
    pos_r = {a: i for i, a in enumerate(order)}
    out = []
    for x in order:
        for y in order:
            if pos_r[x] < pos_r[y] and pos_o[x] > pos_o[y]:
                bx, byy = by[x], by[y]
                mem = (bx.get("writes") and (byy.get("reads") or byy.get("writes"))) or                       (byy.get("writes") and bx.get("reads"))
                if mem and not reaches(reg, x, y) and not reaches(reg, y, x):
                    out.append((x, y))
    return out


def head_loads(reg, moved):
    """Whether every node of `moved` is a `mov reg, [memory]` with no predecessor inside the region."""
    by = {x["addr"]: x for x in reg["nodes"]}
    for a in moved:
        x = by[a]
        if not re.match(r"mov \w+, \[", x["text"]):
            return False
        if any(by.get(p, {}).get("real") for p, _ in x.get("preds", [])):
            return False
    return True


def first_miss(probe, order):
    """The first pick of the probe that isn't `order`'s next node: (pick index, wanted addr, picked addr)."""
    got = [p["addr"] for p in probe["picks"]]
    for i, (g, w) in enumerate(zip(got, order)):
        if g != w:
            return i, w, g
    return None


def chain(reg, addr, issued, depth=8):
    """The predecessor chain that bounds a node's earliest cycle: [(node, its issue cycle, edge latency)], from the
    node back, following the predecessor whose cycle + latency is largest."""
    by = {x["addr"]: x for x in reg["nodes"]}
    out = []
    cur = addr
    for _ in range(depth):
        preds = [(p, lat) for p, lat in by[cur].get("preds", []) if p in issued]
        if not preds:
            break
        p, lat = max(preds, key=lambda t: issued[t[0]] + t[1])
        out.append((p, issued[p], lat))
        cur = p
    return out


# --- running the scheduler -------------------------------------------------------------------------------------------
def trace(addr, seq=None, unit=None, fname=None):
    """Run c2trace's scheduler hooks on the function: (its scheduler log [regions], the trace text)."""
    import c2dbg
    import c2sched
    import c2trace
    import vc6
    if unit is None:
        unit, fname = c2trace.find_address(addr)
        if unit is None:
            raise SystemExit("no FUNCTION annotation for 0x%X" % addr)
    buf = io.StringIO()
    tracer = c2trace.InlineTracer(lambda raw, name: fname in name or fname in (raw or ""), {"sched"}, out=buf)
    tracer = c2sched.extend(tracer, {"sched"})
    tracer.sched_log = []
    tracer.sched_seq = seq or {}
    cmd = c2trace.build_command(unit)
    dbg = c2dbg.Debugger(cmd, env=vc6.env(), cwd=str(c2trace.OUT))
    dbg.on_module("c2.dll", lambda base: tracer.install(dbg, base))
    c2trace.OUT.mkdir(parents=True, exist_ok=True)
    dbg.run()
    tracer.close()
    regions = [r for r in tracer.sched_log if not r["fp"]]
    return regions, buf.getvalue(), unit, fname


# --- the two listings ------------------------------------------------------------------------------------------------
class Listings:
    """Ours (relocated against retail, as `decomp.py diff` prints it) and retail's instructions, from one load."""

    def __init__(self, targets):
        import decomp
        import funcsource
        self.decomp = decomp
        with funcsource.CheckoutLock():
            self.session = decomp.DiffSession(targets)

    def get(self, target):
        d = self.decomp
        s = self.session
        hit = next(((u, e) for u in s.units.values() for e in u.entries if s.wanted(target, e)), None)
        if hit is None:
            raise SystemExit("no compiled function %s" % target)
        u, entry = hit
        sym = u.symbols.get(entry.addr)
        if sym is None:
            raise SystemExit(str(u.errors.get(entry.addr)))
        sec = u.obj.section(sym.section)
        start, end = u.obj.function_extent(sym)
        relocs = [r for r in sec.relocs if start <= r[0] < end]
        agree = d.agreeing_sites(sec.data[start:end], relocs, start, s.img.read(entry.addr, end - start))
        data, _ = s.res.relocate(u, sec, start, end, entry.addr, unresolved={}, trusted=agree.__contains__)
        retail = s.img.read(entry.addr, len(data)) or b""
        ours = d.disasm_sized(data, entry.addr)
        theirs = d.disasm_sized(retail, entry.addr)
        return entry, ours, theirs


def source_lines(target):
    """(line of each of our instruction offsets, the source lines), from framemap's /Z7 copy; ({}, []) on failure."""
    try:
        import framemap
        o = framemap.load_ours(target)
    except Exception:   # noqa: BLE001 - the lines are a convenience
        return None, []
    return o.lines, o.source_lines


def line_at(lines, offset):
    if not lines:
        return None
    best = None
    for off, ln in lines:
        if off <= offset:
            best = ln
    return best


# --- the report ------------------------------------------------------------------------------------------------------
def describe_function(target, listings, verbose=False, out=print):
    entry, ours, retail = listings.get(target)
    addr = entry.addr
    div = first_divergence(ours, retail)
    out("%s 0x%X: first divergence %s" % (entry.name, addr, div["kind"]))
    result = {"addr": "0x%X" % addr, "name": entry.name, "kind": div["kind"]}
    if div["kind"] == "none":
        return result
    k = div["k"]
    o = ours[k] if k < len(ours) else None
    r = retail[k] if k < len(retail) else None
    out("  at %08X: ours %s | retail %s" % (o[0] if o else 0, text(o) if o else "-", text(r) if r else "-"))
    if div["kind"] == "x87-swap":
        out("  the commutative sort ordered %s and %s the other way (fld/%s): read `c2trace --operands`; two "
            "same-shaped leaves order by symbol number (creation order), docs/c2-backend.md" % (div["a"], div["b"],
                                                                                              div["op"][0]))
        return result
    if div["kind"] == "base-index":
        out("  base and index swapped: the operand sort on the address sum, `c2trace --addr`")
        return result
    if div["kind"] not in ("reorder", "reorder-regs"):
        return result
    w = div["w"]
    perm = retail_permutation(ours, retail, k, w)
    out("  reorder window of %d instructions; retail's order: %s" % (w, " ".join(str(p) for p in perm)))
    regions, _, unit, fname = trace(addr)
    mapping = map_picks(ours, regions)
    window = [mapping.get(k + i) for i in range(w)]
    if any(x is None for x in window):
        out("  (an instruction of the window has no scheduler node: a region of one instruction, or the alignment "
            "missed; read `c2trace --sched`)")
        result["verdict"] = "unmapped"
        return result
    regs = {ri for ri, _ in window}
    if len(regs) > 1:
        out("  the window spans regions %s: an instruction crossed a region boundary, which the scheduler never "
            "does: the code generator's order or the region cut differs (every tuple counts toward the 81)"
            % sorted(r + 1 for r in regs))
        result["verdict"] = "cross-region"
        return result
    ri = regs.pop()
    reg = regions[ri]
    by = {x["addr"]: x for x in reg["nodes"]}
    moved = [window[p][1] for p in perm]
    order = target_order(reg, moved)
    lines, src = source_lines(target)

    def show(a):
        x = by[a]
        i = next((i for i, m in mapping.items() if m == (ri, a)), None)
        ln = line_at(lines, ours[i][0] - addr) if i is not None and lines else None
        where = (" (line %d: %s)" % (ln, src[ln - 1].strip()[:60])) if ln and ln <= len(src) else ""
        return "#%d %s%s" % (x["orig"], x["text"][:36], where)

    out("  region %d of %s; the nodes retail moves, in our input order:" % (reg["region"], fname))
    for a in sorted(moved, key=lambda a: by[a]["orig"]):
        x = by[a]
        out("    #%-3d %-40s h %3d%s prio %#x" % (x["orig"], x["text"][:40], x["height"],
                                                  " read" if x["reads"] else "     ", x["prio"]))
    seq = {(fname_of(reg), reg["region"]): seq_override(reg, order)}
    probe_regions, probe_text, _, _ = trace(addr, seq, unit, fname)
    probe = next((p for p in probe_regions if p["region"] == reg["region"] and p["fn"] == reg["fn"]), None)
    if verbose:
        out(probe_text)
    if probe is None:
        out("  (the probe lost the region)")
        result["verdict"] = "probe-failed"
        return result
    orig_seq = {x["addr"]: x["orig"] for x in reg["nodes"]}
    # the probe's node addresses are a new compile's: pair them with ours by original sequence number
    pmap = {x["orig"]: x["addr"] for x in probe["nodes"]}
    back = {v: k_ for k_, v in pmap.items()}
    porder = [pmap[by[a]["orig"]] for a in order]
    miss = first_miss(probe, porder)
    if miss is None:
        result["verdict"] = "input-order"
        pseq = {pmap[s]: s for s in pmap}
        ties = deciding_ties(probe, porder, pseq)
        out("  verdict: INPUT ORDER. The real scheduler gives retail's order from our dependence graph when the "
            "input order is retail's; the ties that decide it:")
        pby = {x["addr"]: x for x in probe["nodes"]}
        for a, b in ties:
            out("    %s ahead of %s (priority %#x each): retail's input had it first"
                % (show(by_orig(reg, back[a])), show(by_orig(reg, back[b])), pby[a]["dyn"]))
        if not ties:
            out("    (no tie at a pick: the new order moved readiness; read the probe with --verbose)")
        if head_loads(reg, moved):
            out("  the nodes are loads into registers with nothing before them in the region: values the register "
                "allocator loads at a block's head (a loop's entry, after a call), emitted in its order, not a "
                "statement's: a register wall (`c2trace --regs`), not a statement order")
            result["verdict"] = "input-order-head-loads"
        result["ties"] = [(back[a], back[b]) for a, b in ties]
        return result
    i, want, got = miss
    result["verdict"] = "not-input-order"
    hints = memory_hints(reg, order)
    for x, y in hints:
        out("  retail keeps %s ahead of %s, which have no dependence in ours: in retail they did. A store and "
            "any later access (load or store) stay in order unless both are addressed from the same pointer "
            "variable: in retail one of them went through another pointer (an inline member of an embedded "
            "object, whose `this` is &member; &member itself; a parameter), or the store was an inline setter's "
            "(`t->SetNext( 0 )`: a setter's `this` is not substituted, a getter's is), docs/c2-backend.md, \"Scheduling\"" % (show(x), show(y)))
    if hints:
        result["memory"] = len(hints)
    pick = probe["picks"][i]
    pby = {x["addr"]: x for x in probe["nodes"]}
    w_, g_ = pby[want], pby[got]
    out("  verdict: NOT THE INPUT ORDER. With retail's input order the scheduler still picks #%d %s at cycle %d, "
        "where retail has #%d %s:" % (g_["orig"], g_["text"][:32], pick["cycle"], w_["orig"], w_["text"][:32]))
    if want not in pick["ready"]:
        out("    #%d was not on the ready list: a predecessor wasn't scheduled yet" % w_["orig"])
        result["why"] = "not-ready"
    elif pick["earliest"].get(want, 0) > pick["cycle"]:
        issued = {p["addr"]: p["cycle"] for p in probe["picks"][:i]}
        ch = chain(probe, want, issued)
        out("    #%d not ready before cycle %d; the chain that holds it:" % (w_["orig"], pick["earliest"][want]))
        nocode = 0
        for p, c, lat in ch:
            x = pby[p]
            nc = node_key(x["text"]) is NO_CODE
            nocode += nc
            out("      #%-3d %-36s issued c%d, +%d%s" % (x["orig"], x["text"][:36], c, lat,
                                                      "   (emits nothing: an issue slot of its own)" if nc else ""))
        out("    retail needs it %d cycle(s) earlier%s" % (pick["earliest"][want] - pick["cycle"],
                                                         ": a no-code node on the chain is a lead" if nocode else ""))
        result["why"] = "latency"
        result["nocode"] = nocode
    else:
        need = pick["dyn"][got] - pick["dyn"][want]
        out("    #%d had priority %#x against #%d's %#x: %d more height (%#x a unit), or a memory read"
            % (w_["orig"], pick["dyn"][want], g_["orig"], pick["dyn"][got], -(-need // 8192), 8192))
        result["why"] = "priority"
    return result


def fname_of(reg):
    return reg["fn"]


def by_orig(reg, seqno):
    return next(x["addr"] for x in reg["nodes"] if x["orig"] == seqno)


def group_addresses(group, open_only):
    import openwork
    rows = openwork.group_rows(group) if hasattr(openwork, "group_rows") else None
    if rows is None:
        cards = ROOT / "cards"
        out = []
        for f in sorted(cards.glob("*.jsonl")):
            for line in f.read_text(encoding="utf-8").splitlines():
                try:
                    e = json.loads(line)
                except ValueError:
                    continue
                if e.get("group") == group:
                    out.append(f.stem)
                    break
        rows = out
    if open_only:
        import decomp
        rep = {int(r["addr"], 16): r for r in decomp.load_report()["functions"]}
        rows = [a for a in rows if rep.get(int(a, 16), {}).get("status", "").startswith("DIFF")]
    return ["0x%X" % int(a, 16) for a in rows]


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("targets", nargs="*", help="addresses; `triage` first for the cheap kind only")
    ap.add_argument("--group", help="triage: the functions whose cards carry this group")
    ap.add_argument("--open", action="store_true", help="triage: only those not exact")
    ap.add_argument("--verbose", action="store_true")
    ap.add_argument("--json", help="write the results to this file")
    a = ap.parse_args(argv)
    triage = bool(a.targets) and a.targets[0] == "triage"
    targets = a.targets[1:] if triage else a.targets
    if a.group:
        targets += group_addresses(a.group, a.open)
    if not targets:
        ap.error("name a function")
    listings = Listings(targets)
    results = []
    for t in targets:
        if triage:
            try:
                entry, ours, retail = listings.get(t)
            except SystemExit as e:
                print("%s  ERROR %s" % (t, e))
                continue
            div = first_divergence(ours, retail)
            k = div.get("k")
            note = ""
            if k is not None and k < min(len(ours), len(retail)):
                note = "%s | %s" % (text(ours[k]), text(retail[k]))
            print("%-10s %-11s %-4s %s  %s" % (t, div["kind"], div.get("w", ""), entry.name[:44], note[:70]))
            results.append({"addr": t, "name": entry.name, **{x: y for x, y in div.items() if x != "op"}})
        else:
            results.append(describe_function(t, listings, a.verbose))
    if a.json:
        Path(a.json).write_text(json.dumps(results, indent=1), encoding="utf-8")
    return 0


if __name__ == "__main__":
    sys.exit(main())
