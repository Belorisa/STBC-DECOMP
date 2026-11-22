"""Frame-layout decoders for tools/c2trace.py (VC6 SP5 C2.DLL 8966): see docs/c2-backend.md, "Frame layout".

The frame packer (stack.c, sub_1074B617, fastcall: ecx = function context, edx = frame pointer flag) walks the list
of locals left in memory (0x1079F220, chained at +0x2C) and gives each a slot: a fixed one (kind 5, parameters and
other already placed symbols), a slot it shares (sub_1074BAE6: the newest earlier slot at least half its size that
it doesn't interfere with) or a new one. This prints, for the traced function:

- the list in the order the packer visits it: each local's name, kind, id, size and reference count;
- every slot as the packer leaves it (after its refs*1000/size sort when the frame passes 0x80 bytes): size,
  reference count, sort key, fixed offset, members;
- each local's final frame offset.
"""
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import c2regs  # noqa: E402

C2_BASE = 0x10700000
LAYOUT = 0x10733BF8          # the frame layout of one function (fastcall: ecx = function context); it calls
                             # PackFrame only when some local isn't fixed (count 0x1079F20C != fixed 0x1079F21C)
PACK_FRAME = 0x1074B617      # PackFrame(fn, fp, ...): fastcall
PLACE_LOCAL = 0x1074BAE6     # PlaceLocal(local, total): fastcall, returns the new total
SORT_SLOTS = 0x10761BF0      # SortSlots(lo, hi): run when the total passes 0x80
LOCALS = 0x1079F220          # first local (an operand record: +4 kind, +0x20 size, +0x2C next, +0x34 refs, +0x38 id)
SLOT_COUNT = 0x1079F224
FIXED_COUNT = 0x1079F21C
SLOTS = 0x1079F228           # 20-byte slots: +0 members (set), +4 interference (set), +8 size, +0xC refs, +0x10 offset
BY_ID = 0x1079F218           # id -> operand record
TOTAL = 0x1079F1FC           # the frame total the packer computed

LOCAL = [("kind", 0x04, "u8"), ("size", 0x20, "s32"), ("next", 0x2C, "u32"), ("refs", 0x34, "s32"),
         ("id", 0x38, "u32")]
SLOT = [("members", 0x00, "u32"), ("interference", 0x04, "u32"), ("size", 0x08, "s32"), ("refs", 0x0C, "s32"),
        ("offset", 0x10, "s32")]


def sort_key(slot):
    """The key sub_10761BF0 sorts the non-fixed slots by, descending."""
    return slot["refs"] * 1000 // slot["size"] if slot["size"] else 0


def walk_locals(mem, first, limit=4096):
    """The locals list from its first record: [(address, fields)]."""
    out, a = [], first
    while a and len(out) < limit:
        f = mem.struct(a, LOCAL)
        out.append((a, f))
        a = f["next"]
    return out


def local_name(mem, op, raw=False):
    kind = mem.u8(op + 4)
    name = c2regs.source_name(mem, op, raw)
    label = "%s #%d" % (c2regs.KIND.get(kind, "kind%d" % kind), mem.u32(op + 0x38))
    return "%s (%s)" % (name, label) if name else label


def final_offset(mem, op):
    """The offset the packer wrote: a temporary's into its symbol record (+0x0C), any other local's at +0x28."""
    if mem.u8(op + 4) == 4:
        rec = mem.u32(op)
        return mem.s32(rec + 0xC) if rec else None
    return mem.s32(op + 0x28)


def extend(tracer, want):
    base_install = tracer.install

    def rel(a):
        return a - C2_BASE + tracer.base

    state = {"packed": False}

    def on_layout(dbg, ctx):
        if not tracer.active:
            return
        state["packed"] = False

        def done(dbg, rctx):
            if not state["packed"]:
                tracer.say("")
                tracer.say("frame of %s: nothing to pack (every local is fixed or in a register)" % tracer.fn)
        ctx.on_return(done)

    def on_pack(dbg, ctx):
        if not tracer.active:
            return
        state["packed"] = True
        first = ctx.u32(rel(LOCALS))
        locs = walk_locals(ctx, first)
        tracer.say("")
        tracer.say("frame of %s  (%s; %d locals in packing order)"
                   % (tracer.fn, "frame pointer" if ctx.edx else "no frame pointer", len(locs)))
        for a, f in locs:
            tracer.say("  %-32s size %4d  refs %4d%s" % (local_name(ctx, a, tracer.raw), f["size"], f["refs"],
                                                         "  fixed" if f["kind"] == 5 else ""))
        ctx.on_return(lambda dbg, rctx: on_packed(rctx, [a for a, _ in locs]))

    def on_packed(ctx, locs):
        n, fixed = ctx.s32(rel(SLOT_COUNT)), ctx.s32(rel(FIXED_COUNT))
        table, by_id = ctx.u32(rel(SLOTS)), ctx.u32(rel(BY_ID))
        total = ctx.s32(rel(TOTAL))
        tracer.say("  slots: %d (%d fixed), total %d bytes%s" % (n, fixed, total,
                                                               ", sorted by refs*1000/size" if total > 0x80 else ""))
        for i in range(max(0, min(n, 512))):
            s = ctx.struct(table + 20 * i, SLOT)
            members = []
            for mid in c2regs.set_members(ctx, s["members"]):
                op = ctx.u32(by_id + 4 * mid)
                members.append(local_name(ctx, op, tracer.raw) if op else "#%d" % mid)
            tracer.say("  [%2d] size %4d  refs %4d  key %6d%s  %s"
                       % (i, s["size"], s["refs"], sort_key(s),
                          ("  fixed at %d" % s["offset"]) if i < fixed else "", ", ".join(members)))
        tracer.say("  offsets:")
        for op in locs:
            off = final_offset(ctx, op)
            tracer.say("    %-32s %s" % (local_name(ctx, op, tracer.raw), "?" if off is None else "%d" % off))

    def install(dbg, base):
        base_install(dbg, base)
        if "frame" in want:
            dbg.add(rel(LAYOUT), on_layout)
            dbg.add(rel(PACK_FRAME), on_pack)

    tracer.install = install
    return tracer
