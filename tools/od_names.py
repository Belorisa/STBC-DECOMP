"""The /Od frame order of a scope's named locals, as VC6's front end lays them out (docs/c2-backend.md, "Frame layout
at /Od"), and a solver for names that give a target order (used against the SDK's debug NetImmerse, AnimOnly.exe).

hb.py name ...            : each name's /Od frame bucket (lower bucket = closer to ebp; ties: later declaration first)
hb.py --order n1 n2 ...      : predicted slot order (closest to ebp first) of one scope, names given in declaration order
hb.py --solve SPEC           : SPEC = "var:decl=cand1,cand2,... var2:decl=..." listed in the TARGET slot order (closest
                               first); decl = declaration index; the first candidate is the current name. Prints the
                               assignment with the fewest renames (then earliest candidates) that gives the target order."""
import sys
def h(s):
    v = 0
    for c in s:
        v = (((v << 2) & 0xffffffff) + (v >> 4) + ord(c)) & 0xffffffff
    return (v ^ (v >> 16)) % 16
def order(names):
    return sorted(names, key=lambda n: (h(n), -names.index(n)))
def ok(a, da, b, db):
    return h(a) < h(b) or (h(a) == h(b) and da > db)
def solve(vars_):
    # vars_: [(var, decl, [cands])] in target order
    best = {}
    for k, c in enumerate(vars_[0][2]):
        best[c] = ((c != vars_[0][2][0], k), [c])
    for i in range(1, len(vars_)):
        _, d, cands = vars_[i]
        pd = vars_[i - 1][1]
        nb = {}
        for k, c in enumerate(cands):
            cost = (c != cands[0], k)
            opts = [(sc[0] + cost[0], sc[1] + cost[1]), path + [c]] if False else None
            cand = None
            for pc, (sc, path) in best.items():
                if ok(pc, pd, c, d):
                    tot = (sc[0] + cost[0], sc[1] + cost[1])
                    if cand is None or tot < cand[0]:
                        cand = (tot, path + [c])
            if cand: nb[c] = cand
        best = nb
        if not best: return None
    return min(best.values())
if __name__ == "__main__" and sys.argv[1:2] != ["--table"]:
    a = sys.argv[1:]
    if a and a[0] == "--order":
        print(" ".join("%s(%d)" % (n, h(n)) for n in order(a[1:])))
    elif a and a[0] == "--solve":
        vs = []
        for t in " ".join(a[1:]).split():
            v, _, rest = t.partition(":"); d, _, cs = rest.partition("=")
            cl = [v] + [x for x in cs.split(",") if x and x != v]
            vs.append((v, int(d), cl))
        r = solve(vs)
        if r is None: print("no solution"); sys.exit(1)
        print("renames", r[0][0])
        for (v, d, _), n in zip(vs, r[1]):
            print("  %-14s -> %-16s (%d)%s" % (v, n, h(n), "" if v == n else "  *"))
    else:
        for n in a: print("%2d %s" % (h(n), n))

def table(spec):
    for t in spec.split():
        v, _, rest = t.partition(":"); d, _, cs = rest.partition("=")
        cl = [v] + [x for x in cs.split(",") if x and x != v]
        by = {}
        for c in cl: by.setdefault(h(c), []).append(c)
        print("%-12s" % v, "  ".join("%d:%s" % (k, ",".join(by[k])) for k in sorted(by)))
if __name__ == "__main__" and sys.argv[1:2] == ["--table"]:
    table(" ".join(sys.argv[2:]))
