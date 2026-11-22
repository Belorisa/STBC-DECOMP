"""c2float: show why VC6 SP5 copies a float through the FPU (fld/fstp) or through integer moves, for one function.

    python tools/c2float.py game/EngPowerCtrl HandlePowerChange          # fld/fstp pairs at the FP peephole
    python tools/c2float.py build/c2f/a.cpp Two --il                      # statement tuples after inlining and after
                                                                         # the optimiser, then at the FP peephole
    python tools/c2float.py x.cpp Two --il --stages 0x10718EAF,0x107281CD  # chosen pipeline stages (C2 addresses)

The rule it shows (docs/c2-backend.md, "Float copies"): every float copy is lowered to `fld src; fstp dst`; the FP
peephole pass (0x1072FEF2, cold half 0x107773AF) turns an fstp (op 0x63) whose *previous* tuple is the fld (op 0x60)
into one integer move (op 1) when neither side is a register, both have the same type and an indirect source or
destination has no index register (operand +0x2C). A copy whose value went through a temporary that the optimiser
forward-substituted (0x10711AFA) carries an op 0x162 tuple (a float value held in a register) between the fld and
the fstp, so the pair isn't adjacent and stays on the FPU.

Output, `--il`: one line per statement tuple, `Txxxx opNNN type  dst <- src`. Operands: `R<n>` a register (32 is
st(0); negative numbers are the back end's virtual registers), `k2` a variable (named when the symbol has a name),
`k6` an indirect memory operand (`+2c` its index register, 0 for none), `T` a tuple. Opcodes seen: 0x15a load into
a register, 0x15b assignment, 0x162 float value held in a register (forward substitution), 0x60 fld, 0x63 fstp,
0x1 move. Lines `---- kindNN` are markers (kind 20: EH state, kind 14: sequence point).

The walk over the statement list reads the function context's block list (+8): first block +0x1C, last block +0x20;
during the optimiser's own stages the list isn't reachable that way and those stages print nothing.
"""
import argparse
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import c2dbg  # noqa: E402
import vc6  # noqa: E402

LOOP = (0x10757FC2, 0x10757FC2 + 2020)   # the per-function loop (main.c)
FUNCTION_CONTEXT = 0x1071BA73              # fastcall(function symbol) -> function context, called by the loop
FP_PEEPHOLE = 0x1072FEF2                   # fastcall(function context), called from the register allocator
AFTER_INLINE = 0x10718EAF                  # first stage after inline expansion (0x107180FD) that sees the copies
AFTER_OPTIMISER = 0x107281CD               # first stage after the global optimiser (0x107130CB)
REGISTERS = 0x107AC6DC                     # register records, 0x54 bytes each
FLD, FSTP, FPVALUE = 0x60, 0x63, 0x162


def symbol_name(ctx, sym):
    try:
        s = ctx.cstr(ctx.u32(sym + 0x18), 60)
    except Exception:
        return None
    return s if s and s.isprintable() and s[0] in "_?$" else None


def operand(ctx, p):
    if not p:
        return "-"
    kind, typ = ctx.u8(p + 8), ctx.u16(p + 10)
    w = ctx.dwords(p, 12)
    if kind == 12:
        return "T%04x" % (p & 0xFFFF)
    if kind == 1:
        return "R%d:%04x" % ((w[6] - c2dbg.c2_va(ctx.dbg, REGISTERS)) // 0x54, typ)
    name = None
    for f in (7, 5, 0):
        if w[f] and not name:
            name = symbol_name(ctx, w[f])
            if not name and w[f] > 0x10000:
                try:
                    name = symbol_name(ctx, ctx.u32(w[f]))
                except Exception:
                    name = None
    text = "k%d:%04x%s" % (kind, typ, (" " + name) if name else "")
    if kind == 6:
        text += " +2c=%x" % w[11]
    return text


def walk(ctx, fctx, stop_at_end=True):
    blocks = ctx.u32(fctx + 8)
    if not blocks:
        return
    t = ctx.u32(ctx.u32(ctx.u32(blocks) + 0x1C))
    end = ctx.u32(ctx.u32(blocks + 4) + 0x20)
    n = 0
    while t and n < 20000 and not (stop_at_end and t == end):
        yield t
        t = ctx.u32(t)
        n += 1


def dump_il(ctx, fctx, title):
    print("== %s" % title)
    for t in walk(ctx, fctx):
        w = ctx.dwords(t, 8)
        if ctx.u8(t + 8) == 12:
            print("  T%04x op%-3x %04x  %s <- %s" % (t & 0xFFFF, w[1], ctx.u16(t + 10), operand(ctx, w[7]),
                                                  operand(ctx, w[6])))
        else:
            print("  ---- kind%d op%x" % (ctx.u8(t + 8), w[1]))


def pairs(ctx, fctx):
    """Each fstp with what the peephole sees: its previous tuple and the merge conditions."""
    print("== FP peephole (0x1072FEF2): fstp tuples")
    for t in walk(ctx, fctx):
        if ctx.u8(t + 8) != 12 or ctx.u32(t + 4) != FSTP:
            continue
        prev = ctx.u32(t + 0xC)
        dst = ctx.u32(t + 0x1C)
        pop = ctx.u32(prev + 4) if prev and ctx.u8(prev + 8) == 12 else None
        if pop == FLD:
            src = ctx.u32(prev + 0x18)
            why = []
            if ctx.u8(src + 8) == 1 or ctx.u8(dst + 8) == 1:
                why.append("a register operand")
            if ctx.u16(src + 10) != ctx.u16(dst + 10):
                why.append("types differ")
            for side, o in (("source", src), ("destination", dst)):
                if ctx.u8(o + 8) == 6 and ctx.u32(o + 0x2C):
                    why.append("indexed %s" % side)
            verdict = "integer move" if not why else "stays fld/fstp: " + ", ".join(why)
            print("  fld %s ; fstp %s  -> %s" % (operand(ctx, src), operand(ctx, dst), verdict))
        else:
            print("  fstp %s after op%s  -> stays fld/fstp (previous tuple isn't the fld)" % (
                operand(ctx, dst), "%x" % pop if pop is not None else "?"))


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("target", help="a unit (game/Foo) or a scratch .c/.cpp file")
    ap.add_argument("function", help="substring of the decorated name")
    ap.add_argument("--il", action="store_true", help="also dump the statement tuples after inlining and optimising")
    ap.add_argument("--stages", default="", help="comma-separated C2 addresses of pipeline stages to dump at")
    ap.add_argument("--flags", default="/O2 /GX", help="flags for a scratch file")
    a = ap.parse_args(argv)
    if a.target.endswith((".c", ".cpp")):
        cmd, _ = c2dbg.compile_command(source=a.target, flags=a.flags.split())
    else:
        cmd, _ = c2dbg.compile_command(unit=a.target)
    dbg = c2dbg.Debugger(cmd, env=vc6.env(), cwd=str(c2dbg.OUT))
    state = {"ctx": None}

    def on_context(dbg, ctx):
        ret = c2dbg.ghidra_address(dbg, ctx.u32(ctx.esp))
        if not (LOOP[0] <= ret < LOOP[1]):
            return
        fn = c2dbg.current_function(ctx)
        if fn and a.function in fn:
            print("#", fn)
            ctx.on_return(lambda d, r: state.__setitem__("ctx", r.eax))
        else:
            state["ctx"] = None

    def stage(addr):
        def h(dbg, ctx):
            if state["ctx"]:
                dump_il(ctx, state["ctx"], "stage 0x%X" % addr)
        return h

    def peephole(dbg, ctx):
        if state["ctx"]:
            pairs(ctx, ctx.ecx)

    stages = [int(x, 16) for x in a.stages.split(",") if x] or ([AFTER_INLINE, AFTER_OPTIMISER] if a.il else [])
    probes = [c2dbg.Probe(FUNCTION_CONTEXT, on_context), c2dbg.Probe(FP_PEEPHOLE, peephole)]
    probes += [c2dbg.Probe(s, stage(s)) for s in stages]
    c2dbg.install(dbg, probes)
    return dbg.run()


if __name__ == "__main__":
    sys.exit(main())
