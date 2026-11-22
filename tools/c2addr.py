"""Address-mode decoders for tools/c2trace.py (VC6 SP5 C2.DLL 8966): see docs/c2-backend.md, "Base and index of a
two-register address".

A memory operand that adds two registers ([base+index+disp], IL op 0x14D) is built by the address matcher
(sub_1072AC94, which works backward from the register holding an address through its defining tuples) with one of
two constructors: sub_1070148F (an indirect operand, kind 6: `mov [eax+ecx-8], 0`) and sub_10726CD5 (an address,
kind 5: `lea edx, [eax+ecx+0xC]`). Both take (ecx = op, dx = type; stack: symbol, displacement, base, index, ...)
and store the base at +0x28 and the index at +0x2C; the listing printer (0x107753A9) prints +0x28 first, and the
encoder makes it the SIB base.

Which register lands in which slot is the global optimiser's commutative operand sort (c2operands.py) on the sum
that forms the address: the sum's add tuple adds its first (heavier) operand to a register loaded with its second,
and the matcher makes the added operand the base and the register it was added to the index. So the operand the
sort put first is the base. The two operands are compared by weight (`height << 24 | size << 16 | hash16`); for two
leaves the hash16 decides, and it comes from symbol numbers (sub_1070DB59):

- a variable or temporary leaf: `number << 6` when its descriptor's +4 is 3 (a local or a back-end temporary),
  `fold(number)` mixed `<< 5` otherwise; a temporary that stands for a common subexpression (descriptor +0x14)
  hashes as that expression;
- a memory reference: `fold(displacement) + op - 0x145 + (hash(its base) << 8)`, so only the low byte of the base's
  hash counts: for a `number << 6` base, `number & 3`;
- an expression: the sum of its operands' hashes `<< i` (i = 0..7 by position), plus `op - 0x145`;
- everything is truncated to 16 bits.

`c2trace --addr` prints, for each two-register address the matcher builds in the traced function, the instruction's
final registers, the two operands the sort compared (each hash taken apart down to its symbol numbers), which went
first, and the arithmetic of a flip: by how many symbol numbers the operands would have to move (every number at or
above the lower of the two operands' own numbers moved together, as a symbol first referenced earlier in the
function does) for the other operand to sort first.
"""
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import c2regs  # noqa: E402

C2_BASE = 0x10700000
COMPARE = 0x1070F6AE        # the commutative sort's comparator (c2operands.py)
MAKE_IND = 0x1070148F       # fastcall(op, type; symbol, disp, base, index, x): an indirect operand (kind 6)
MAKE_ADDR = 0x10726CD5      # fastcall(op, type; symbol, disp, base, index): an address (kind 5, lea)
FRAME = 0x10733B7B          # the frame layout: registers are final by now
# an existing one-register operand rewritten in place to op 0x14D (esi = the operand, at the `mov [esi+4], 0x14D`):
# no sort decides these, the rewriting code puts the register it adds in a fixed slot
REWRITES = {0x1071F93A: "rewritten to [base+index] by the global optimiser (C2 0x1071F93A)",
            0x10754B05: "a register added as index to an existing operand (C2 0x10754B05)",
            0x1077A8A4: "a register added as base to an operand that has an index (C2 0x1077A8A4)",
            0x1077AA8D: "a register added as index to an operand without one (C2 0x1077AA8D)"}
OP_BASE_INDEX = 0x14D       # [base + index + disp]
REGS = 0x107AC730           # &regs[r] = REGS + r * 0x54
N_OP, N_KIND, N_WEIGHT, N_DESC, N_DISP, N_BASE, N_INDEX = 0x04, 0x08, 0x0C, 0x14, 0x24, 0x28, 0x2C
D_KIND, D_EXPR, D_NUMBER = 0x04, 0x14, 0x1C
FLIP_RANGE = 128            # the shifts tried for a flip, each way


def s32(v):
    v &= 0xFFFFFFFF
    return v - (1 << 32) if v & 0x80000000 else v


def fold(v):
    """The hash's 32-to-16-bit fold: high half xor low half."""
    v &= 0xFFFFFFFF
    return ((v >> 16) ^ (v & 0xFFFF)) & 0xFFFF


def leaf_hash(number, var_kind, node_kind):
    """sub_1070DB59 for a leaf whose descriptor holds `number` (var_kind: the descriptor's +4)."""
    if node_kind in (1, 2) and var_kind == 3:
        return (number << 6) & 0xFFFF
    v = fold(number)
    if node_kind == 3:
        return v
    return (((v & 0x7FF) << 5) ^ (s32(v << 5) >> 16)) & 0xFFFF


class Term:
    """An operand's hash as a tree of the parts sub_1070DB59 combines, so it can be recomputed with other numbers."""

    def __init__(self, kind, text, number=None, var_kind=None, node_kind=None, parts=(), const=0, op=0):
        self.kind, self.text, self.number = kind, text, number
        self.var_kind, self.node_kind, self.parts, self.const, self.op = var_kind, node_kind, list(parts), const, op

    def hash(self, shift=lambda n: n):
        if self.kind == "leaf":
            return leaf_hash(shift(self.number), self.var_kind, self.node_kind)
        if self.kind == "cse":
            return self.parts[0].hash(shift)
        if self.kind == "memory":
            h = self.const
            if self.parts:
                h += self.parts[0].hash(shift) << 8
            return h & 0xFFFF
        if self.kind == "expr":
            h = sum(p.hash(shift) << (i & 7) for i, p in enumerate(self.parts))
            return (h + self.const) & 0xFFFF
        return self.const

    def numbers(self):
        out = [self.number] if self.kind == "leaf" else []
        for p in self.parts:
            out += p.numbers()
        return out

    def show(self, shift=lambda n: n):
        if self.kind == "leaf":
            return "%s #%d" % (self.text, shift(self.number))
        if self.kind == "cse":
            return "%s{%s}" % (self.text, self.parts[0].show(shift))
        if self.kind == "memory":
            inner = self.parts[0].show(shift) if self.parts else ""
            return "[%s%s]" % (inner, " %+d" % self.number if self.number else "")
        if self.kind == "expr":
            return "op%X(%s)" % (self.op, ", ".join(p.show(shift) for p in self.parts))
        return self.text


def term(mem, n, raw=False, depth=0):
    """The Term of operand node n, read the way sub_1070DB59 walks it."""
    if not n or depth > 8:
        return Term("other", "?")
    k = mem.u8(n + N_KIND)
    op = mem.u32(n + N_OP)
    if k in (1, 2, 3):
        desc = mem.u32(n + N_DESC)
        if not desc:
            return Term("other", "leaf", const=mem.u32(n + N_WEIGHT) & 0xFFFF)
        name = c2regs.source_name(mem, desc, raw) or "temp"
        if k in (1, 2) and mem.u32(desc + D_EXPR):
            return Term("cse", "%s #%d" % (name, mem.u32(desc + D_NUMBER)),
                        parts=[term(mem, mem.u32(desc + D_EXPR), raw, depth + 1)])
        return Term("leaf", name, number=mem.u32(desc + D_NUMBER), var_kind=mem.u8(desc + D_KIND), node_kind=k)
    if k in (5, 6):
        disp = s32(mem.u32(n + N_DISP))
        base = mem.u32(n + N_BASE)
        parts = [term(mem, base, raw, depth + 1)] if base else []
        return Term("memory", "memory", number=disp, parts=parts, const=(fold(disp) + op - 0x145) & 0xFFFF)
    if k == 7:
        return Term("other", "constant %d" % s32(mem.u32(n + 0x18)), const=mem.u32(n + N_WEIGHT) & 0xFFFF)
    if k >= 0x0C:
        parts, it = [], mem.u32(n + 0x18)
        while it and len(parts) < 16:
            parts.append(term(mem, it, raw, depth + 1))
            it = mem.u32(it)
        return Term("expr", "op%X" % op, parts=parts, const=(op - 0x145) & 0xFFFF, op=op)
    return Term("other", "kind %d op 0x%X" % (k, op), const=mem.u32(n + N_WEIGHT) & 0xFFFF)


def first_wins(wa, wb):
    """The sort's verdict for a pair in list order (a before b): True when a stays first (descending, stable)."""
    return wa >= wb


def flips(a, b, wa, wb, lo):
    """The shifts d in -FLIP_RANGE..FLIP_RANGE (d != 0) of every symbol number >= lo that reverse the order of a and b
    (a before b in list order, weights wa and wb)."""
    now = first_wins(wa, wb)
    out = []
    for d in range(-FLIP_RANGE, FLIP_RANGE + 1):
        if d == 0:
            continue
        sh = lambda n, d=d: n + d if n >= lo else n
        if any(sh(n) < 0 for n in a.numbers() + b.numbers()):
            continue
        na = (wa & 0xFFFF0000) | a.hash(sh)
        nb = (wb & 0xFFFF0000) | b.hash(sh)
        if first_wins(na, nb) != now:
            out.append(d)
    return out


def nearest(shifts):
    """The smallest flipping shift each way, as text."""
    up = [d for d in shifts if d > 0]
    down = [d for d in shifts if d < 0]
    parts = []
    if up:
        parts.append("+%d" % min(up))
    if down:
        parts.append("%d" % max(down))
    return " or ".join(parts) if parts else "none within +-%d" % FLIP_RANGE


def flip_report(a, b, wa, wb):
    """Two families of shift: symbols numbered before both operands (every number from the lower one up moves), and
    symbols numbered between them (only the later-numbered operand's numbers move: a temporary the optimiser
    numbers after the whole function's symbols, an induction variable or a reload, moves with every symbol of the
    function)."""
    na, nb = a.numbers(), b.numbers()
    if not na or not nb:
        return ["no symbol number behind one operand: nothing to shift"]
    lines = ["flip: symbols numbered before both: %s" % nearest(flips(a, b, wa, wb, min(na + nb)))]
    lo_set, hi_set = (na, nb) if max(na) < min(nb) else (nb, na) if max(nb) < min(na) else (None, None)
    if lo_set is not None:
        lines.append("      symbols numbered between them: %s" % nearest(flips(a, b, wa, wb, max(lo_set) + 1)))
    return lines


def reg_name(mem, node, regs):
    """The register a register node (kind 1) holds once allocated, else a placeholder."""
    if not node or mem.u8(node + N_KIND) != 1:
        return "?"
    r = mem.u32(node + 0x18) - regs
    if 0 <= r < 0x54 * 9 and r % 0x54 == 0:
        return c2regs.REG_NAMES.get(r // 0x54, "r%d" % (r // 0x54))
    return "?"


def operand_text(base, index, disp):
    return "[%s+%s%s]" % (base, index, ("%+d" % disp).replace("+-", "-") if disp else "")


def extend(tracer, want):
    base_install = tracer.install
    state = {"seq": 0, "last": {}, "made": [], "fn": None}

    def rel(a):
        return a - C2_BASE + tracer.base

    def reset():
        if state["fn"] != tracer.fn:
            state.update(fn=tracer.fn, seq=0, last={}, made=[])

    def on_compare(dbg, ctx):
        if not tracer.active:
            return
        reset()
        a, b = ctx.ecx, ctx.edx
        wa, wb = ctx.u32(a + N_WEIGHT), ctx.u32(b + N_WEIGHT)
        if (wa >> 16) != 1 or (wb >> 16) != 1:      # only leaves (height 0, size 1) are ever an address's two registers
            return
        ta, tb = term(ctx, a, tracer.raw), term(ctx, b, tracer.raw)

        def done(dbg, rctx):
            state["seq"] += 1
            rec = {"seq": state["seq"], "a": a, "b": b, "wa": wa, "wb": wb, "ta": ta, "tb": tb,
                   "res": s32(rctx.eax)}
            state["last"][a] = state["last"][b] = rec
        ctx.on_return(done)

    def on_make(kind):
        def handler(dbg, ctx):
            if not tracer.active or ctx.ecx != OP_BASE_INDEX:
                return
            reset()
            base, index, disp = ctx.arg(2), ctx.arg(3), s32(ctx.arg(1))
            recs = [r for r in (state["last"].get(base), state["last"].get(index)) if r]
            rec = max(recs, key=lambda r: r["seq"]) if recs else None

            def done(dbg, rctx):
                state["made"].append({"node": rctx.eax, "kind": kind, "disp": disp, "base": base, "index": index,
                                      "rec": rec})
            ctx.on_return(done)
        return handler

    def on_rewrite(where):
        def handler(dbg, ctx):
            if not tracer.active:
                return
            reset()
            state["made"].append({"node": ctx.esi, "kind": ctx.u8(ctx.esi + N_KIND), "disp": None, "base": 0,
                                  "index": 0, "rec": None, "how": REWRITES[where]})
        return handler

    def on_frame(dbg, ctx):
        if not tracer.active:
            return
        reset()
        if not state["made"]:
            return
        regs = rel(REGS)
        sites = []
        tracer.say("")
        tracer.say("two-register addresses in %s  [%s]  (base = the operand the commutative sort put first)"
                   % (tracer.fn, getattr(tracer, "fn_raw", "")))
        for m in state["made"]:
            n = m["node"]
            if ctx.u32(n + N_OP) != OP_BASE_INDEX:
                continue
            b, i = reg_name(ctx, ctx.u32(n + N_BASE), regs), reg_name(ctx, ctx.u32(n + N_INDEX), regs)
            if "?" in (b, i):
                continue
            label = "S%d" % (len(sites) + 1)
            disp = m["disp"] if m["disp"] is not None else s32(ctx.u32(n + N_DISP))
            tracer.say("  %-4s %s %s" % (label, "lea" if m["kind"] == 5 else "mem", operand_text(b, i, disp)))
            for line in ([m["how"]] if m.get("how") else explain(m, b, i)):
                tracer.say("         " + line)
            if m["rec"] is not None:
                sites.append((label,) + order_of(m["rec"]))
            else:
                sites.append((label, Term("other", "-"), Term("other", "-"), 0, 0))
        rows = shift_table([s for s in sites if s[1].kind != "other" or s[2].kind != "other"])
        if rows:
            tracer.say("  symbol-number shifts that flip sites (k symbols more (+) or fewer (-) numbered from P on):")
            for line in table_lines(rows, sites):
                tracer.say("    " + line)
        state["made"] = []

    def install(dbg, base):
        base_install(dbg, base)
        if "addr" in want:
            dbg.add(rel(COMPARE), on_compare)
            dbg.add(rel(MAKE_IND), on_make(6))
            dbg.add(rel(MAKE_ADDR), on_make(5))
            dbg.add(rel(FRAME), on_frame)
            for where in REWRITES:
                dbg.add(rel(where), on_rewrite(where))

    tracer.install = install
    return tracer


def order_of(rec):
    """(first term, second term, first weight, second weight) of a recorded comparison."""
    if rec["res"] > 0:
        return rec["tb"], rec["ta"], rec["wb"], rec["wa"]
    return rec["ta"], rec["tb"], rec["wa"], rec["wb"]


SHIFTS = (-4, -3, -2, -1, 1, 2, 3, 4)


def shift_table(sites):
    """Rows (k, low, high, labels): moving every symbol number >= P by k, for each P from low to high, flips exactly
    the sites in labels. A symbol first referenced at some point of the function (a local, a temporary, an inlined
    copy of a callee's parameter or local) moves every number from that point on; the back end's own temporaries
    (common subexpressions, induction variables, reloads) come after all of the function's symbols."""
    nums = sorted({n for _, a, b, _, _ in sites for n in a.numbers() + b.numbers()})
    rows = []
    for k in SHIFTS:
        run = None
        for p in nums:
            sh = lambda n, p=p, k=k: n + k if n >= p else n
            flipped = []
            for label, a, b, wa, wb in sites:
                if any(sh(n) < 0 for n in a.numbers() + b.numbers()):
                    continue
                na, nb = (wa & 0xFFFF0000) | a.hash(sh), (wb & 0xFFFF0000) | b.hash(sh)
                if first_wins(na, nb) != first_wins(wa, wb):
                    flipped.append(label)
            if run and run[3] == flipped:
                run[2] = p
            else:
                if run and run[3]:
                    rows.append(tuple(run))
                run = [k, p, p, flipped]
        if run and run[3]:
            rows.append(tuple(run))
    return rows


def table_lines(rows, sites):
    """The shift table as text: a range that starts at the lowest number of any site reads `P <= hi` (any point
    before it moves every site's numbers alike)."""
    nums = [n for _, a, b, _, _ in sites for n in a.numbers() + b.numbers()]
    low = min(nums) if nums else 0
    out = []
    for k, lo, hi, labels in rows:
        where = "P <= %d" % hi if lo == low else ("P %d" % lo if lo == hi else "P %d..%d" % (lo, hi))
        out.append("%+d at %s: %s" % (k, where, " ".join(labels)))
    return out


def explain(m, base_reg, index_reg):
    """The lines under one address: the sort's two operands, the winner and the flip arithmetic."""
    rec = m["rec"]
    if rec is None:
        return ["no leaf comparison recorded for its operands (built from expressions, or one operand only)"]
    first, second = ("b", "a") if rec["res"] > 0 else ("a", "b")    # the comparator's -1/0: a stays first
    tf, ts = rec["t" + first], rec["t" + second]
    wf, ws = rec["w" + first], rec["w" + second]
    nf = rec[first]
    as_base = nf == m["base"] or (rec[second] == m["index"])
    roles = ("base %s" % base_reg, "index %s" % index_reg) if as_base else ("index %s" % index_reg,
                                                                           "base %s" % base_reg)
    out = ["%-9s <- %s  hash %04X" % (roles[0], tf.show(), wf & 0xFFFF),
           "%-9s <- %s  hash %04X" % (roles[1], ts.show(), ws & 0xFFFF)]
    if (wf & 0xFFFF) == (ws & 0xFFFF):
        out.append("tie: the list order decides")
    out += flip_report(tf, ts, wf, ws)
    if not as_base:
        out.append("note: the first-sorted operand became the index here (not the rule above)")
    return out
