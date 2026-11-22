"""Operand-order decoders for tools/c2trace.py (VC6 SP5 C2.DLL 8966): see docs/c2-backend.md, "Operand order of
commutative operations".

The global optimiser gives every operand of every expression node a weight (sub_1070DA9A, ecx = node, the weight
returned in eax and kept at node +0x0C) and sorts a commutative node's operand list by it, descending, with a stable
merge sort whose comparator is 0x1070F6AE (ecx, edx = the two operands; -1 when the first goes first). The weight is
`height << 24 | size << 16 | hash16`, and a variable leaf's hash16 is its symbol number << 5: the number is the dword
at +0x1C of the leaf's descriptor (node +0x14), whose +0x00 is the front end's symbol record (name at +0x18). The
numbers follow first reference in the function's IL, so two same-shaped operands are ordered by which was named
first. Mechanism read by bw1-decomp (openblack's Black & White decompilation, CC0: docs/msvc6_operand_order.md
there) on the same C2.DLL build, and confirmed on ours.

`c2trace --operands` prints, for the traced function, each distinct comparison the sort made: the two operands
(a variable by its source name and symbol number, anything else by its IL opcode), their weights split into height,
size and hash, and which one goes first (`=` for a tie, which keeps the list order). The optimiser walks the trees
more than once; a comparison is printed the first time it is made.
"""
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import c2regs  # noqa: E402

C2_BASE = 0x10700000
COMPARE = 0x1070F6AE        # fastcall(a, b): the commutative sort's comparator, returns -1/0/1
N_OP, N_KIND, N_WEIGHT, N_DESC = 0x04, 0x08, 0x0C, 0x14
D_NUMBER = 0x1C
LEAF = 2                    # node kind of a variable leaf (the low byte of +0x08)
# IL opcodes seen in the sort (scratch compiles): a constant's hash is its value and its size bit is clear
OPS = {0x146: "constant", 0x14A: "variable", 0x14C: "memory", 0x162: "float in register", 0x16D: "add",
       0x16F: "multiply"}


def split(weight):
    """(height, size, hash16) of a weight."""
    return weight >> 24, (weight >> 16) & 0xFF, weight & 0xFFFF


def describe(mem, node, raw=False):
    """A short text for an operand node: `name #number` for a variable leaf, else `op 0xNNN`."""
    op = mem.u32(node + N_OP)
    kind = mem.u8(node + N_KIND)
    if kind == LEAF:
        desc = mem.u32(node + N_DESC)
        if desc:
            num = mem.u32(desc + D_NUMBER)
            name = c2regs.source_name(mem, desc, raw)
            return "%s #%d" % (name or "temp", num)
    return "%s (op 0x%X)" % (OPS[op], op) if op in OPS else "op 0x%X" % op


def comparison(mem, a, b, result, raw=False):
    """One line for a comparator call: the operand that goes first on the left."""
    wa, wb = mem.u32(a + N_WEIGHT), mem.u32(b + N_WEIGHT)
    da, db = describe(mem, a, raw), describe(mem, b, raw)
    text = lambda d, w: "%s [h%d s%d #%04X]" % ((d,) + split(w))
    if result > 0:
        return "%s  before  %s" % (text(db, wb), text(da, wa))
    sep = "  before  " if result < 0 else "  =  "
    return text(da, wa) + sep + text(db, wb)


def extend(tracer, want):
    base_install = tracer.install
    seen = {"fn": None, "lines": set()}

    def rel(a):
        return a - C2_BASE + tracer.base

    def on_compare(dbg, ctx):
        if not tracer.active:
            return
        a, b = ctx.ecx, ctx.edx

        def done(dbg, rctx):
            result = rctx.eax - (1 << 32) if rctx.eax & 0x80000000 else rctx.eax
            line = comparison(rctx, a, b, result, tracer.raw)
            if seen["fn"] != tracer.fn:
                seen["fn"], seen["lines"] = tracer.fn, set()
                tracer.say("")
                tracer.say("operand order in %s  (weight = height, size, hash; heavier first)" % tracer.fn)
            if line not in seen["lines"]:
                seen["lines"].add(line)
                tracer.say("  " + line)
        ctx.on_return(done)

    def install(dbg, base):
        base_install(dbg, base)
        if "operands" in want:
            dbg.add(rel(COMPARE), on_compare)

    tracer.install = install
    return tracer
