"""Front-end cost decoder for tools/c2trace.py: the inline cost C1XX.DLL (VC6 SP5) gives each function.

The cost the back end reads from a function's symbol record (+0x6D, docs/c2-backend.md, "Cost") is the number of IL
tuples the front end emitted for the function's body, prologue and epilogue included. In C1XX.DLL (image base
0x10400000; Ghidra export in C:\\Recomp\\c2-ghidra\\export-c1xx):

- sub_10415BCE( opcode, node ), cdecl, emits one tuple and adds 1 to the counter 0x104E2E8C;
- sub_10416705 starts a function's IL and sets the counter to 0;
- sub_10418D02( function symbol ) ends it: the counter goes into the function's table entry (+0x14), and the symbol
  record writer (sub_10401DAB, case 0x19) writes it as the 16-bit cost (0xFFFF from 65536 tuples up). Under /Ob2 a
  function with fewer than 175 tuples (0x104D7D34) is also marked as an automatic inline candidate (flag 0x40).

This prints, for each function c2trace traces, its cost and the opcodes of the tuples that make it up. The opcode names are
read from scratch compiles (which tuples a spelling adds), not from the DLL, which has no table of them.
"""
C1_BASE = 0x10400000
EMIT = 0x10415BCE
START = 0x10416705
END = 0x10418D02
AUTO_INLINE_LIMIT = 175      # 0x104D7D34: /Ob2 marks functions below this cost as automatic inline candidates

# observed in scratch compiles (tools/c2trace.py --cost): which source construct adds the tuple
OPCODES = {
    0x02: "add", 0x04: "mul", 0x0F: "add-assign", 0x22: "less", 0x24: "greater", 0x26: "symbol", 0x27: "member",
    0x29: "label", 0x2A: "this-in", 0x2B: "this-out", 0x2C: "convert", 0x2D: "param", 0x30: "load",
    0x32: "assign", 0x33: "constant", 0x35: "post-inc", 0x36: "post-dec", 0x38: "branch-false", 0x3A: "jump",
    0x3E: "call", 0x41: "return", 0x46: "entry", 0x47: "exit", 0x4B: "discard", 0x4C: "seq", 0x53: "scope",
    0x54: "end-scope", 0x5A: "this", 0x99: "callee", 0x9A: "vcall",
}


def opname(op):
    return OPCODES.get(op, "op%02X" % op)


def summary(ops):
    """'symbol x3, load x2, ...' in order of first appearance."""
    counts = {}
    for o in ops:
        counts[o] = counts.get(o, 0) + 1
    return ", ".join("%s x%d" % (opname(o), n) if n > 1 else opname(o) for o, n in counts.items())


def function_name(mem, fsym):
    """The function's own identifier (unqualified) from C1XX's symbol: +8 points at a record whose +4 is the name."""
    try:
        return mem.cstr(mem.u32(mem.u32(fsym + 8) + 4), 256)
    except OSError:
        return None


def same_function(c1_name, raw, name):
    """Whether C1XX's unqualified identifier can be the function the back end names raw (decorated) / name."""
    if not c1_name:
        return False
    special = {"__ctor": "??0", "__dtor": "??1", "__delDtor": "??_G", "__vecDelDtor": "??_E"}
    if c1_name in special:
        return (raw or "").startswith(special[c1_name])
    return c1_name in (name or "") or c1_name in (raw or "")


def pair(records, traced):
    """Match the back end's traced functions [(raw, name, cost)] with C1XX's records [(c1 name, ops)], in order: the
    first unused record with the same cost and a compatible name, else the first with the same cost."""
    used, out = set(), []
    for raw, name, cost in traced:
        pick = None
        for i, (c1, ops) in enumerate(records):
            if i in used or len(ops) != cost:
                continue
            if same_function(c1, raw, name):
                pick = i
                break
            if pick is None:
                pick = i
        if pick is not None:
            used.add(pick)
        out.append((raw, name, cost, records[pick] if pick is not None else None))
    return out


class CostTracer:
    def __init__(self, say=print):
        self.say = say
        self.ops = []
        self.records = []    # (C1XX's identifier, opcodes) of every function, in the order C1XX ends them
        self.base = None

    def rel(self, a):
        return a - C1_BASE + self.base

    def install(self, dbg, base):
        self.base = base
        dbg.add(self.rel(START), lambda dbg, ctx: self.ops.clear())
        dbg.add(self.rel(EMIT), lambda dbg, ctx: self.ops.append(ctx.arg(0)))
        dbg.add(self.rel(END), self.on_end)

    def on_end(self, dbg, ctx):
        fsym = ctx.arg(0)
        if fsym:
            self.records.append((function_name(ctx, fsym), self.ops))
        self.ops = []

    def report_traced(self, traced, raw=False):
        for draw, name, cost, rec in pair(self.records, traced):
            if rec is None:
                self.say("")
                self.say("front-end cost of %s: %d (no C1XX function with that many tuples)" % (draw if raw else name,
                                                                                             cost))
            else:
                self.report(draw if raw else name, rec[1])

    def report(self, name, ops):
        self.say("")
        self.say("front-end cost of %s: %d tuples%s" % (name, len(ops), "" if len(ops) < AUTO_INLINE_LIMIT else
                                                        "  (above the /Ob2 automatic inline limit, 175)"))
        self.say("  " + summary(ops))
        line = []
        for o in ops:
            line.append(opname(o))
            if o in (0x4B, 0x41, 0x4C, 0x3A, 0x38):   # a statement's last tuple
                self.say("    " + " ".join(line))
                line = []
        if line:
            self.say("    " + " ".join(line))


def attach(dbg, say=print):
    t = CostTracer(say)
    dbg.on_module("c1xx.dll", lambda base: t.install(dbg, base))
    return t
