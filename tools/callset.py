"""Which out-of-line calls differ between retail and ours in the open functions, by callee: the inline-budget map.

    python tools/callset.py [--min-score N] [--top N]   # both tables: the callees ranked by open functions
    python tools/callset.py --callee NAME|ADDR           # one callee's open functions, with scores, on both sides
    python tools/callset.py fn 0x5AD450 [--all]          # one function: the call targets that differ (--all: every one)

For every open function (status DIFF: drafts and parked ones included) it compares the multiset of direct call
targets in retail's code with the one in our compiled code, read the way `tools/walls.py` reads them (both sides cut
at the function's end; a call of ours whose target has no address yet is paired with retail's leftover calls when
the counts agree). Then, per callee X:

    retail calls X out of line where ours doesn't   ours expands X inline (or lacks the call): check X's inline
                                                    body in its header against lineage, or the budget it spends
    ours calls X out of line where retail doesn't   retail expands X (or ours has an extra call): X may be an
                                                    inline in the original whose body ours keeps out of line

Each table is ranked by the number of open functions, then by their mean score. A callee that is wrong at many
sites is one header body to fix for all of them. Callees are named from the annotations (with the file that defines
them) and config/symbols.txt; "(no identity)" is retail code nothing names yet, and "(no address)" is a symbol of
ours that has none. Direct calls and import-slot calls (`call [__imp_X]`) count; virtual calls through a register
don't.

Reads build/report.json and build/obj (run `decomp.py build` first); the result is cached in build/callset.json
until report.json changes. A first run loads every unit (about 15 seconds).
"""

import argparse
import json
import re
import sys
from collections import Counter, defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import decomp  # noqa: E402
import walls  # noqa: E402

CACHE = decomp.BUILD / "callset.json"
FORMAT = 5   # the cache's layout: a cache of another format is rebuilt
IMPORT_SLOT = re.compile(r"dword ptr \[(0x[0-9a-f]{5,8})\]")


def call_targets(ins):
    """Counter of a function's direct call targets: "0x..." for an address (a code address or an import slot),
    "?<symbol>" for a call of ours whose target has no address. Calls through a register are left out."""
    out = Counter()
    for m, o in ins:
        if m != "call":
            continue
        if o.startswith("?"):
            out[o] += 1
            continue
        hit = walls.ADDRESS.fullmatch(o) or IMPORT_SLOT.fullmatch(o)
        if hit:
            out["0x%X" % int(hit.group(hit.lastindex or 0), 16)] += 1
    return out


def placed(calls_o, calls_r, addresses):
    """Ours' calls without an address, given one where the symbol is annotated (or claimed) elsewhere: an inline or
    template annotated at several copies resolves only in the units that annotate it, so a call of another unit's
    takes the copy retail calls here, else the first. `addresses`: {decorated name: [addresses]}."""
    out = Counter()
    for t, n in calls_o.items():
        where = addresses.get(t[1:]) if t.startswith("?") else None
        if where:
            keys = ["0x%X" % a for a in where]
            t = next((k for k in keys if calls_r.get(k)), keys[0])
        out[t] += n
    return out


def compare(ours, retail, addresses=None):
    """(retail's calls, ours' calls, only retail, only ours) as Counters: ours' calls without an address placed
    where they can be, and then paired with retail's leftover calls as tools/walls.py pairs them."""
    calls_o, calls_r = call_targets(ours), call_targets(retail)
    calls_o = placed(calls_o, calls_r, addresses or {})
    paired = walls.pair_unresolved(calls_o, calls_r)
    return calls_r, calls_o, calls_r - paired, paired - calls_r


def collect():
    """{address: {"name", "score", "unit", "parked", "draft", "retail": {target: n}, "ours": {target: n},
    "only_retail": {...}, "only_ours": {...}}} for every open function, plus the names of the targets seen."""
    rep = decomp.load_report()
    rows = {int(r["addr"], 16): r for r in rep["functions"] if r["status"] == "DIFF"}
    all_units = decomp.source_files()
    objdir = decomp.BUILD / "obj"
    objs = {u: objdir / (u + ".obj") for u in all_units if (objdir / (u + ".obj")).exists()}
    units = decomp.load_units(objs, all_units)
    img = decomp.pe.Image(decomp.target_image())
    res = decomp.Resolver(img, units, decomp.extra_symbols())
    addresses = defaultdict(set)
    for decorated, a in res.globals.items():
        addresses[decorated].add(a)
    for decorated, per in res.copies.items():
        addresses[decorated].update(per.values())
    addresses = {k: sorted(v) for k, v in addresses.items()}
    funcs = {}
    for u in units.values():
        for e in u.entries:
            r = rows.get(e.addr)
            sym = u.symbols.get(e.addr)
            if e.kind != "FUNCTION" or r is None or sym is None or e.addr in funcs:
                continue
            sec = u.obj.section(sym.section)
            start, end = u.obj.function_extent(sym)
            unresolved = {}
            data, _ = res.relocate(u, sec, start, end, e.addr, unresolved=unresolved)
            # retail is read up to the next known address, as the build's wall reading does (decomp.wall_of)
            stop = res.next_start(e.addr)
            size = max(len(data), min(stop - e.addr if stop else 0, 4 * len(data) + 256))
            retail = img.read(e.addr, size) or img.read(e.addr, len(data)) or b""
            ours_ins = walls.instructions(decomp.disasm_sized, bytes(data), e.addr, unresolved)
            retail_ins = walls.instructions(decomp.disasm_sized, retail, e.addr)
            calls_r, calls_o, only_r, only_o = compare(ours_ins, retail_ins, addresses)
            funcs["0x%X" % e.addr] = {
                "name": r["name"], "score": r["score"], "unit": r["unit"], "parked": r.get("parked"),
                "draft": r.get("draft"), "retail": dict(calls_r), "ours": dict(calls_o),
                "only_retail": dict(only_r), "only_ours": dict(only_o)}
    return {"functions": funcs, "names": names(units, res, rep["functions"])}


# The special names of MSVC's decoration (`??<code>`): constructors, destructors and operators.
OPERATORS = {"0": None, "1": None, "2": "operator new", "3": "operator delete", "4": "operator=", "7": "operator!",
             "8": "operator==", "9": "operator!=", "A": "operator[]", "B": "operator cast", "C": "operator->",
             "D": "operator*", "E": "operator++", "F": "operator--", "G": "operator-", "H": "operator+",
             "K": "operator/", "M": "operator<", "N": "operator<=", "O": "operator>", "P": "operator>=",
             "X": "operator*=", "Y": "operator+=", "Z": "operator-=", "_0": "operator/=", "_U": "operator new[]",
             "_V": "operator delete[]", "_G": "`scalar deleting destructor'", "_E": "`vector deleting destructor'"}
PRIMITIVES = {"C": "signed char", "D": "char", "E": "unsigned char", "F": "short", "G": "unsigned short", "H": "int",
              "I": "unsigned int", "J": "long", "K": "unsigned long", "M": "float", "N": "double", "_N": "bool"}


def _scope(s, i):
    """The names of a decorated scope from s[i] up to its closing '@' (innermost first), and the index after it."""
    parts = []
    while s[i] != "@":
        if s.startswith("?$", i):
            name, i = _template(s, i + 2)
        else:
            j = s.index("@", i)
            name, i = s[i:j], j + 1
        parts.append(name)
    return parts, i + 1


def _template(s, i):
    j = s.index("@", i)
    name, i, args = s[i:j], j + 1, []
    while s[i] != "@":
        if s[i] in "VU":
            inner, i = _scope(s, i + 1)
            args.append("::".join(reversed(inner)))
        elif s[i:i + 2] in PRIMITIVES:
            args.append(PRIMITIVES[s[i:i + 2]])
            i += 2
        elif s[i] in PRIMITIVES:
            args.append(PRIMITIVES[s[i]])
            i += 1
        else:
            raise ValueError(s)
    return "%s<%s>" % (name, ",".join(args)), i + 1


def pretty(decorated):
    """A readable name for a decorated name: `NiPoint3::operator=`, `NiPointer<NiAVObject>::~NiPointer`; None when
    it can't be read simply (a template argument that isn't a class or a primitive)."""
    try:
        if decorated.startswith("??"):
            code = decorated[2:4] if decorated[2] == "_" else decorated[2]
            if code not in OPERATORS:
                return None
            scope, _ = _scope(decorated, 2 + len(code))
            if code in ("0", "1"):
                base = scope[0].split("<")[0]
                return "::".join(reversed(scope)) + "::" + ("~" if code == "1" else "") + base
            return "::".join(list(reversed(scope)) + [OPERATORS[code]])
        if decorated.startswith("?"):
            scope, _ = _scope(decorated, 1)
            return "::".join(reversed(scope))
    except (ValueError, IndexError):
        return None
    return decomp.qualified(decorated)


def names(units, res, rows):
    """{"0x...": (name, where, decorated)} for every address the annotations or symbols.txt name: where is the
    annotated file and its status, or "symbols.txt" for a claim without source; decorated is the symbol's decorated
    name (tools/c2trace.py --retail pairs the back end's callees with addresses by it)."""
    status = {r["addr"]: r["status"] for r in rows}
    out = {}
    for decorated, addr in res.globals.items():
        q = pretty(decorated)
        if decorated.startswith("__imp_"):
            q = "[import] " + (decomp.import_name(decorated) or decorated)
        out["0x%X" % addr] = [q or decorated, "symbols.txt", decorated]
    for u in units.values():
        for e in u.entries:
            if e.kind != "FUNCTION":
                continue
            k = "0x%X" % e.addr
            sym = u.symbols.get(e.addr)
            decorated = sym.name if sym is not None else e.decorated
            name = (pretty(decorated) if decorated else None) or decomp.display_name(e.name, decorated)
            path = Path(e.path)
            try:
                path = path.relative_to(decomp.ROOT)
            except ValueError:
                pass
            out[k] = [name, "%s, %s" % (path.as_posix(), status.get(k, "?")), decorated]
    return out


def load(refresh=False):
    """The collected call sets, from the cache when build/report.json hasn't changed since."""
    report = decomp.BUILD / "report.json"
    if not report.exists():
        sys.exit("no build/report.json: run build first")
    stamp = "%d:%d:%d" % (FORMAT, report.stat().st_mtime_ns, report.stat().st_size)
    if not refresh and CACHE.exists():
        try:
            data = json.loads(CACHE.read_text(encoding="utf-8"))
            if data.get("stamp") == stamp:
                return data
        except ValueError:
            pass
    data = collect()
    data["stamp"] = stamp
    CACHE.write_text(json.dumps(data), encoding="utf-8")
    return data


def label(target, names):
    """A call target as the reports print it."""
    if target.startswith("?"):
        sym = target[1:]
        return "%s (no address)" % (pretty(sym) or sym)
    name = names.get(target)
    if name is None:
        return "%s (no identity)" % target
    return "%s %s" % (target, name[0])


def by_callee(funcs, side, min_score=0.0):
    """{target: [(address, n)]}: the open functions where `side` ("only_retail" or "only_ours") has the target."""
    out = defaultdict(list)
    for addr, f in funcs.items():
        if f["score"] < min_score:
            continue
        for t, n in f[side].items():
            out[t].append((addr, n))
    return out


def ranked(table, funcs):
    """The callees by number of open functions, then by their mean score."""
    def mean(t):
        return sum(funcs[a]["score"] for a, _ in table[t]) / len(table[t])
    return sorted(table, key=lambda t: (-len(table[t]), -mean(t), t)), mean


SIDES = (("only_retail", "retail calls X out of line where ours doesn't (ours expands it, or lacks the call)"),
         ("only_ours", "ours calls X out of line where retail doesn't (retail expands it, or ours has an extra call)"))


def report(data, min_score=0.0, top=25, out=sys.stdout):
    funcs, names_ = data["functions"], data["names"]
    sel = [a for a, f in funcs.items() if f["score"] >= min_score]
    differ = [a for a in sel if funcs[a]["only_retail"] or funcs[a]["only_ours"]]
    print("%d open functions%s, %d whose call sets differ" % (
        len(sel), " scoring >= %g" % min_score if min_score else "", len(differ)), file=out)
    for side, title in SIDES:
        table = by_callee(funcs, side, min_score)
        order, mean = ranked(table, funcs)
        print("\n%s: %d callees" % (title, len(order)), file=out)
        print("%6s %6s %6s  %s" % ("funcs", "sites", "mean", "callee [where]"), file=out)
        for t in order[:top]:
            where = names_.get(t, [None, None])[1]
            print("%6d %6d %6.1f  %s%s" % (len(table[t]), sum(n for _, n in table[t]), mean(t), label(t, names_),
                                           "  [%s]" % where if where else ""), file=out)
        if len(order) > top:
            print("  ... %d more (--top N)" % (len(order) - top), file=out)


def matches(target, query, names_):
    q = query.lower()
    if re.fullmatch(r"(0x)?[0-9a-f]{5,8}", q):
        return target.startswith("0x") and int(target, 16) == int(q, 16)
    return q in label(target, names_).lower()


def report_callee(data, query, min_score=0.0, out=sys.stdout):
    funcs, names_ = data["functions"], data["names"]
    found = False
    for side, title in SIDES:
        table = by_callee(funcs, side, min_score)
        for t in sorted(t for t in table if matches(t, query, names_)):
            found = True
            where = names_.get(t, [None, None])[1]
            print("%s%s\n  %s: %d open functions" % (label(t, names_), "  [%s]" % where if where else "",
                                                     title.replace("X", "it"), len(table[t])), file=out)
            for a, n in sorted(table[t], key=lambda x: -funcs[x[0]]["score"]):
                f = funcs[a]
                state = "parked" if f.get("parked") else "draft" if f.get("draft") else "open"
                print("    %5.1f %-9s %-6s %s%s" % (f["score"], a, state, f["name"], "  x%d" % n if n > 1 else ""),
                      file=out)
    if not found:
        print("no open function's call set differs on %s" % query, file=out)


def report_function(data, addr, show_all=False, out=sys.stdout):
    funcs, names_ = data["functions"], data["names"]
    k = "0x%X" % int(addr, 16)
    f = funcs.get(k)
    if f is None:
        print("%s is not an open (DIFF) function in build/report.json" % k, file=out)
        return 1
    print("%s %s  %.1f  (%s)" % (k, f["name"], f["score"], f["unit"]), file=out)
    retail, ours = Counter(f["retail"]), Counter(f["ours"])
    targets = sorted(set(retail) | set(ours), key=lambda t: label(t, names_))
    differ = [t for t in targets if f["only_retail"].get(t) or f["only_ours"].get(t)]
    if not differ:
        print("the call sets agree%s" % (" (ours' calls without an address stand for retail's leftover ones)"
                                         if any(t.startswith("?") for t in ours) else ""), file=out)
    print("%6s %6s  %s" % ("retail", "ours", "callee"), file=out)
    for t in (targets if show_all else differ):
        print("%6d %6d  %s%s" % (retail[t], ours[t], label(t, names_), "  <- differs" if t in differ else ""),
              file=out)
    if not show_all and len(targets) > len(differ):
        print("%d other callees agree (--all lists them)" % (len(targets) - len(differ)), file=out)
    return 0


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("cmd", nargs="*", help="`fn <addr>` for one function; nothing for the tables")
    ap.add_argument("--min-score", type=float, default=0.0, help="only open functions scoring at least this")
    ap.add_argument("--callee", help="one callee, by address or by part of its name")
    ap.add_argument("--top", type=int, default=25, help="callees per table (default 25)")
    ap.add_argument("--all", action="store_true", help="fn: list the callees that agree too")
    ap.add_argument("--refresh", action="store_true", help="ignore the cache")
    args = ap.parse_args(argv)
    if args.cmd and (args.cmd[0] != "fn" or len(args.cmd) != 2):
        ap.error("the only subcommand is `fn <addr>`")
    data = load(args.refresh)
    if args.cmd:
        return report_function(data, args.cmd[1], args.all)
    if args.callee:
        report_callee(data, args.callee, args.min_score)
    else:
        report(data, args.min_score, args.top)
    return 0


if __name__ == "__main__":
    sys.exit(main())
