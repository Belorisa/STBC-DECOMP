"""Register-allocation decoders for tools/c2trace.py (VC6 SP5 C2.DLL 8966): see docs/c2-backend.md, "Registers".

The global allocator (color.c, sub_1072FB58) pops candidates from a list sorted by priority (+0x0C), then by the
number of the last tuple writing the candidate (+0x40), both largest first. For each popped candidate this prints its
name, priority, spill cost, reference count and the outcome: memory (spill cost < 1), or the register the chooser
(sub_10732F7C) picked, with the per-register costs it compared.
"""
import struct

C2_BASE = 0x10700000
RA_DRIVER = 0x1072FB58       # GlobalRegAlloc(fn): ecx = function context
RA_POP = 0x1072FDFF          # esi = candidate just popped from the worklist
RA_MEMORY = 0x1072FDC7       # call sub_10732001: candidate left in memory (spill cost < 1)
RA_CHOSEN = 0x1072FE64       # after sub_10732F7C: candidate +0x10 = &regs[r] (0x107AC730 + r*0x54)
RA_COSTS = 0x1079D868        # int[9]: per-register cost the chooser compared
RA_OPTFLAG = 0x107AC0B4      # bit 23 of the optimisation word: loop weights and some size checks
REGS = 0x107AC730
REG_NAMES = {1: "eax", 2: "ecx", 3: "edx", 4: "ebx", 5: "esp", 6: "ebp", 7: "esi", 8: "edi"}
ORDER = (1, 2, 3, 7, 8, 4, 6)   # 0x107A09E8: eax ecx edx esi edi ebx ebp

KIND = {3: "var", 4: "temp", 5: "param", 0xD: "const"}   # operand kind at +4


def set_members(ctx, setp, limit=64):
    """Members of a C2 bit set: a list of 32-bit chunks {base, next, bits}."""
    out = []
    if not setp:
        return out
    chunk = ctx.u32(setp)
    while chunk and len(out) < limit:
        base, nxt, bits = struct.unpack("<III", ctx.read(chunk, 12))
        for i in range(32):
            if bits & (1 << i):
                out.append(base + i)
        chunk = nxt
    return out


def source_name(ctx, op, raw=False):
    """The source name of the local or parameter a register candidate's operand stands for, else None.

    The operand's +0 points at the symbol record the front end gave the variable (null for the back end's own
    temporaries: induction variables, common subexpressions). That record's +0x18 is its name as the back end spells
    it (sub_10742388: '_' before a C name of an automatic, '$' or '@' for other kinds); its +0x0C is the frame offset
    and +0x10 the size. The names are there without /Zi or /Z7."""
    try:
        rec = ctx.u32(op)
        name = ctx.cstr(ctx.u32(rec + 0x18)) if rec else None
    except OSError:
        return None
    if name and not raw and name[0] in "_$@" and len(name) > 1:
        name = name[1:]
    return name or None


def extend(tracer, want):
    base_install = tracer.install

    def rel(a):
        return a - C2_BASE + tracer.base

    def cand_name(ctx, c):
        op = ctx.u32(c)
        cid = ctx.u32(c + 0x1C)
        if not op:
            return "#%d" % cid
        kind = ctx.u8(op + 4)
        label = "%s #%d" % (KIND.get(kind, "kind%d" % kind), cid)
        if kind == 0xD:
            return label
        name = source_name(ctx, op, raw=tracer.raw)
        return "%s (%s)" % (name, label) if name else label

    state = {"pending": None}

    def on_driver(dbg, ctx):
        if tracer.active:
            tracer.say("")
            tracer.say("registers of %s  (global allocator; loop weights %s)"
                       % (tracer.fn, "on" if ctx.u32(rel(RA_OPTFLAG)) else "off: every block weighs 1"))

    def on_pop(dbg, ctx):
        if not tracer.active:
            return
        c = ctx.esi
        allowed = [REG_NAMES.get(r, str(r)) for r in set_members(ctx, ctx.u32(c + 0x20)) if r in REG_NAMES]
        state["pending"] = c
        if tracer.raw:
            sym = ctx.u32(c)
            tracer.say("    cand " + ctx.read(c, 0x44).hex())
            tracer.say("    sym  " + (ctx.read(sym, 0x40).hex() if sym else "-"))
        tracer.say("  %-28s prio %6d  spill %5d  refs %3d  last-def %5d  allowed %s"
                   % (cand_name(ctx, c), ctx.s32(c + 0xC), ctx.s32(c + 0x3C), ctx.s32(c + 0x24),
                      ctx.u32(c + 0x40), ",".join(allowed) or "-"))

    def on_memory(dbg, ctx):
        if tracer.active:
            tracer.say("  %28s -> memory" % "")

    def on_chosen(dbg, ctx):
        if not tracer.active:
            return
        c = ctx.esi
        r = (ctx.u32(c + 0x10) - rel(REGS)) // 0x54
        costs = struct.unpack("<9i", ctx.read(rel(RA_COSTS), 36))
        shown = " ".join("%s:%d" % (REG_NAMES[x], costs[x]) for x in ORDER if costs[x])
        tracer.say("  %28s -> %s%s" % ("", REG_NAMES.get(r, "r%d" % r),
                                        ("   (costs " + shown + ")") if shown else ""))

    def install(dbg, base):
        base_install(dbg, base)
        if "regs" in want:
            dbg.add(rel(RA_DRIVER), on_driver)
            dbg.add(rel(RA_POP), on_pop)
            dbg.add(rel(RA_MEMORY), on_memory)
            dbg.add(rel(RA_CHOSEN), on_chosen)

    tracer.install = install
    return tracer
