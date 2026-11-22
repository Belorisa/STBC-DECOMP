"""Instruction-scheduler decoders for tools/c2trace.py (VC6 SP5 C2.DLL 8966): see docs/c2-backend.md, "Scheduling".

The list scheduler (schedmd.c; driver sub_107374AA, run after the frame layout on the machine instructions) cuts each
basic block into regions (sub_10737A43: at most 81 instructions, ending at a call, a label or a branch), builds a
dependence graph per region (sub_107396F6), gives every node a priority (sub_1073A684) and schedules the region cycle
by cycle (sub_1073AF90 -> sub_1073B176). The ready list is kept sorted by priority, largest first, and among equal
priorities by the node's sequence number, the instruction's position in the region's input order (sub_1073B147):
**a tie goes to the instruction that came first**. Under /G5 (the default blend: model 2, two instructions a cycle)
the priority is `height << 13` plus `1 << 16` for an instruction that reads memory; `height` is the longest path to
the region's end (1 + the largest successor height plus edge latency).

This prints, for the traced function, per region:

- the input order: each instruction with its sequence number, height, memory-read bit and priority;
- each pick, cycle by cycle (U and V slot), with the ready list as the picker saw it: the instructions ahead of the
  pick that it passed over and why (not ready until a later cycle, doesn't pair in the V slot), and what decided the
  pick against the next ready instruction behind it: a tie broken by input order, or the priority difference
  (height, memory read) that a change would have to overcome;
- the output order, with the instructions that moved against the input order.
"""
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

C2_BASE = 0x10700000
SCHED_DRIVER = 0x107374AA     # fastcall(fn, fp): fp = 0 for the integer pass
SCHEDULE_REGION = 0x1073AF90  # fastcall(dag): dag[0] = entry node, dag[1] = exit node (priorities computed)
TAKE_NODE = 0x1073B53E        # fastcall(node): the picked node leaves the ready list (still linked at entry)
FIX_DISPLACEMENT = 0x10751EA0 # fastcall(edge): a soft edge broken; the successor's address is adjusted (push/esp)
CPU = 0x107AC0B0              # /G3../G6: 3..6 (default /G5 here: the blend)
MODEL = 0x10799218            # s16: CPU - 3, at least 0
ISSUE = 0x10799250            # s16: instructions per cycle (model 0..3: 2, 1, 2, 3)
SHIFTS = 0x10799204           # -> s16[8] per model (0x107A0D98 integer, 0x107A6E88 FP)
FP_PASS = 0x1079F270          # nonzero during the FP-stack pass
CYCLE = 0x1079F238
READY = 0x1079F278            # ready list head, linked at +0x10
MNEMONICS = 0x107A5A90        # char *[]: the machine opcode's name (index = tuple opcode)
REGS = 0x107AC730             # register records, 0x54 bytes each

REG_NAMES = {1: "eax", 2: "ecx", 3: "edx", 4: "ebx", 5: "esp", 6: "ebp", 7: "esi", 8: "edi",
             9: "ax", 10: "cx", 11: "dx", 12: "bx", 13: "sp", 14: "bp", 15: "si", 16: "di",
             17: "al", 18: "cl", 19: "dl", 20: "bl", 21: "ah", 22: "ch", 23: "dh", 24: "bh"}
FLAGS_TYPE = 0x8000

# node fields (0x3C bytes, sub_10739A2D)
N_PRED, N_SUCC, N_TUPLE, N_PREDS_LEFT, N_SUCCS, N_SOFT = 0x08, 0x0C, 0x1C, 0x20, 0x22, 0x24
N_PRIO, N_DYN, N_EARLIEST, N_HEIGHT, N_SEQ, N_LAT, N_B39, N_B3A = 0x28, 0x2C, 0x30, 0x34, 0x36, 0x38, 0x39, 0x3A


def priority_terms(shifts, height, succs, crit, reads, fold, writes=0, fp_type=False):
    """The priority sub_1073A684 gives a node: each term shifted by the model's table (negative: a right shift)."""
    def sh(v, s):
        return v >> -s if s < 0 else v << s
    terms = {"fold": sh(fold, shifts[0]), "height": sh(height, shifts[1]), "succs": sh(succs, shifts[2]),
             "critical": sh(crit, shifts[3]), "reads": sh(reads, shifts[5])}
    if fp_type:
        terms["writes"] = sh(writes, shifts[6])
    return terms


def explain(pick, other, shifts):
    """Why `pick` was taken before `other` (both dicts with seq, prio, height, reads...): one line."""
    if pick["dyn"] == other["dyn"]:
        return ("tie on priority %#x with #%d: input order decides (#%d first); #%d ahead of #%d in the input flips it"
                % (pick["dyn"], other["seq"], pick["seq"], other["seq"], pick["seq"]))
    parts = []
    if pick["height"] != other["height"]:
        parts.append("height %d against %d" % (pick["height"], other["height"]))
    if pick["reads"] != other["reads"]:
        parts.append("memory read %d against %d" % (pick["reads"], other["reads"]))
    rp, ro = residual(pick, shifts), residual(other, shifts)
    if rp != ro:
        parts.append("float store or fold bonus %#x against %#x" % (rp, ro))
    unit = 1 << shifts[1] if shifts[1] >= 0 else 1
    gap = pick["dyn"] - other["dyn"]
    need = -(-gap // unit)
    if other["seq"] > pick["seq"]:
        hint = "#%d needs %d more height (or a memory read) to tie, and then the input order still keeps #%d first" % (
            other["seq"], need, pick["seq"])
    else:
        hint = "#%d needs %d more height to tie (it is first in the input, so a tie would flip it)" % (other["seq"], need)
    return "priority %#x over #%d's %#x (%s): %s" % (pick["dyn"], other["seq"], other["dyn"],
                                                     ", ".join(parts) or "other terms", hint)


def residual(n, shifts):
    """What a node's static priority holds beyond its height and memory-read terms: the float-store term
    (shift 6, an instruction writing memory whose destination is a float) or the fold term (shift 0)."""
    def sh(v, k):
        return v >> -k if k < 0 else v << k
    return n["prio"] - sh(n["height"], shifts[1]) - sh(n["reads"], shifts[5]) - sh(n["succs"], shifts[2])


def moved(order):
    """Sequence numbers in output order -> those that left their input order (placed before a lower number)."""
    out = []
    for i, s in enumerate(order):
        later_lower = [t for t in order[i + 1:] if t < s]
        if later_lower:
            out.append((s, min(later_lower)))
    return out


def extend(tracer, want):
    base_install = tracer.install

    def rel(a):
        return a - C2_BASE + tracer.base

    st = {"region": 0, "nodes": {}, "picks": [], "cycle": None, "slot": 0, "fp": 0, "shifts": None, "fn": None}

    def mnemonic(ctx, op):
        try:
            if 0 <= op < 0x144:
                s = ctx.cstr(ctx.u32(rel(MNEMONICS) + 4 * op), 24)
                if s:
                    return s
        except OSError:
            pass
        return "op%x" % op

    def reg(ctx, rec):
        r = (rec - rel(REGS)) // 0x54 if rec >= rel(REGS) else -1
        return REG_NAMES.get(r, "r%d" % r)

    def sym_name(ctx, s):
        try:
            n = ctx.cstr(ctx.u32(s + 0x18), 60)
        except OSError:
            return None
        if n and n.isprintable():
            return n[1:] if n[0] in "_$" and len(n) > 1 and not tracer.raw else n
        return None

    def operand(ctx, p):
        kind, typ = ctx.u8(p + 8), ctx.u16(p + 10)
        if kind == 1:
            return reg(ctx, ctx.u32(p + 0x18))
        if kind == 2:
            # the variable's operand record (+4: 3 local, 4 temporary, 5 parameter; +0x1C its id)
            v = ctx.u32(p + 0x14)
            k = ctx.u8(v + 4) if v else 0
            return "[%s#%d]" % ({3: "local", 4: "temp", 5: "param"}.get(k, "var"), ctx.u32(v + 0x1C) if v else 0)
        if kind == 6:
            parts = []
            base, index = ctx.u32(p + 0x28), ctx.u32(p + 0x2C)
            symp = ctx.u32(p + 0x20)
            if symp:
                n = sym_name(ctx, symp)
                parts.append(n or "sym")
            if base:
                parts.append(reg(ctx, ctx.u32(base + 0x18)))
            if index:
                scale = 1 << (ctx.u8(p + 0x10) & 0xF)
                parts.append(reg(ctx, ctx.u32(index + 0x18)) + ("*%d" % scale if scale > 1 else ""))
            disp = ctx.s32(p + 0x24)
            text = "+".join(parts)
            if disp or not text:
                text += ("%+#x" % disp) if text else "%#x" % disp
            return "[%s]" % text
        if kind == 3:
            # an address: a global's (its symbol at +0x14) or a frame slot's
            v = ctx.u32(p + 0x14)
            n = sym_name(ctx, v) if v else None
            return "&%s" % n if n else "&addr"
        if kind == 7:
            return "%#x" % ctx.u32(p + 0x18)
        return "k%d" % kind

    def operands(ctx, p, skip_esp):
        out = []
        n = 0
        while p and n < 8:
            n += 1
            if ctx.u16(p + 10) != FLAGS_TYPE:
                t = operand(ctx, p)
                if not (skip_esp and t == "esp"):
                    out.append(t)
            p = ctx.u32(p)
        return out

    def text(ctx, t):
        if not t:
            return "(region %s)" % "entry/exit"
        op = ctx.u32(t + 4)
        m = mnemonic(ctx, op)
        skip = m in ("push", "pop", "_epush", "_epop", "call", "_rpush")
        dst = operands(ctx, ctx.u32(t + 0x1C), skip)
        src = [s for s in operands(ctx, ctx.u32(t + 0x18), skip) if s not in dst or m in ("xor", "sub", "add", "and",
                                                                                         "or", "cmp", "test")]
        # a register that only forms a memory destination's address is not an operand of its own
        src = [s for s in src if not any(d.startswith("[") and s in d[1:-1].replace("*", "+").split("+")
                                         for d in dst)]
        if m in ("call", "_jcc", "jmp"):
            src = [s for s in src if s.startswith("[") or s.startswith("0x")]
            dst = []
        args = dst + src
        if m in ("xor", "sub", "and", "or", "add") and len(args) == 3 and args[0] == args[1]:
            args = args[1:]
        return ("%s %s" % (m, ", ".join(args))).strip()

    def node(ctx, n):
        b39, b3a = ctx.u8(n + N_B39), ctx.u8(n + N_B3A)
        t = ctx.u32(n + N_TUPLE)
        return {"addr": n, "seq": ctx.u16(n + N_SEQ), "height": ctx.u16(n + N_HEIGHT), "succs": ctx.u16(n + N_SUCCS),
                "reads": b3a & 1, "writes": (b3a >> 1) & 1, "crit": (b39 >> 6) & 1, "fold": b39 >> 7,
                "lat": ctx.u8(n + N_LAT), "pipe": b39 & 7, "prio": ctx.u32(n + N_PRIO), "dyn": ctx.u32(n + N_DYN),
                "earliest": ctx.u32(n + N_EARLIEST), "text": text(ctx, t), "real": bool(t)}

    def flush():
        if not st["nodes"]:
            return
        order = [p["seq"] for p in st["picks"]]
        mv = moved(order)
        tracer.say("    output: " + " ".join("#%d" % s for s in order))
        if mv:
            tracer.say("    moved against the input: " + ", ".join("#%d before #%d" % m for m in mv))
        else:
            tracer.say("    output = input order")
        st["nodes"] = {}
        st["picks"] = []
        st["issued"] = {}

    def on_driver(dbg, ctx):
        if tracer.active:
            flush()
            st["fp"] = ctx.edx

    def on_region(dbg, ctx):
        if not tracer.active:
            return
        flush()
        st["region"] += 1
        st["cycle"] = None
        model = ctx.s16(rel(MODEL))
        shifts = [ctx.s16(ctx.u32(rel(SHIFTS)) + 2 * i) for i in range(8)]
        st["shifts"] = shifts
        fp = ctx.u32(rel(FP_PASS))
        if st["fn"] != tracer.fn:
            st["fn"] = tracer.fn
            st["region"] = 1
            tracer.say("")
            tracer.say("schedule of %s  (/G%d: model %d, %d per cycle; shifts %s: priority = %s)"
                       % (tracer.fn, ctx.s32(rel(CPU)), model, ctx.s16(rel(ISSUE)), shifts, formula(shifts)))
        dag = ctx.ecx
        n, last = ctx.u32(dag), ctx.u32(dag + 4)
        nodes = []
        guard = 0
        while n and guard < 400:
            guard += 1
            nodes.append(node(ctx, n))
            if n == last:
                break
            n = ctx.u32(n)
        real = [x for x in nodes if x["real"]]
        # --seq override (tools/c2order.py): new sequence numbers for this region's nodes, written before the
        # scheduler reads them, so the real scheduler breaks its ties in another input order
        override = getattr(tracer, "sched_seq", {}).get((tracer.fn, st["region"]))
        if override:
            for x in nodes:
                if x["seq"] in override:
                    ctx_write(ctx, x["addr"] + N_SEQ, struct.pack("<H", override[x["seq"]]))
        for x in nodes:
            x["preds"] = preds_of(ctx, x["addr"])
            x["orig"] = x["seq"]
            if override and x["seq"] in override:
                x["seq"] = override[x["seq"]]
        st["nodes"] = {x["addr"]: x for x in nodes}
        log = getattr(tracer, "sched_log", None)
        if log is not None:
            st["rec"] = {"fn": tracer.fn, "region": st["region"], "fp": bool(fp), "shifts": shifts,
                         "nodes": nodes, "picks": []}
            log.append(st["rec"])
        else:
            st["rec"] = None
        tracer.say("  region %d%s: %d instructions (input order)" % (st["region"], " (FP pass)" if fp else "",
                                                                      len(real)))
        for x in real:
            tracer.say("    #%-3d %-40s h %3d%s  prio %#7x" % (x["seq"], x["text"][:40], x["height"],
                                                            "  read" if x["reads"] else "      ", x["prio"]))

    def on_take(dbg, ctx):
        if not tracer.active or not st["nodes"]:
            return
        cyc = ctx.u32(rel(CYCLE))
        if cyc != st["cycle"]:
            st["cycle"], st["slot"] = cyc, 0
        else:
            st["slot"] += 1
        pick = node(ctx, ctx.ecx)
        ready = []
        x = ctx.u32(rel(READY))
        guard = 0
        while x and guard < 200:
            guard += 1
            ready.append(node(ctx, x))
            x = ctx.u32(x + 0x10)
        ahead, behind, seen = [], [], False
        for r in ready:
            if r["addr"] == pick["addr"]:
                seen = True
            elif seen:
                behind.append(r)
            else:
                ahead.append(r)
        st["picks"].append(pick)
        st.setdefault("issued", {})[pick["addr"]] = cyc
        if st.get("rec") is not None:
            st["rec"]["picks"].append({"addr": pick["addr"], "cycle": cyc, "slot": st["slot"],
                                       "ready": [r["addr"] for r in ready],
                                       "earliest": {r["addr"]: r["earliest"] for r in ready},
                                       "dyn": {r["addr"]: r["dyn"] for r in ready}})
        slot = "UV"[st["slot"]] if st["slot"] < 2 else str(st["slot"])
        tracer.say("    c%-3d %s #%-3d %-40s prio %#x" % (cyc, slot, pick["seq"], pick["text"][:40], pick["dyn"]))
        for a in ahead:
            if a["earliest"] > cyc:
                why = "not ready before cycle %d%s" % (a["earliest"], bound_by(ctx, a))
            elif st["slot"] >= 1:
                why = "doesn't pair in the V slot"
            else:
                why = "held back (address-generation or FP rule)"
            tracer.say("           passed over #%d %s (prio %#x): %s" % (a["seq"], a["text"][:32], a["dyn"], why))
        rival = next((b for b in behind if b["earliest"] <= cyc), None)
        if rival:
            tracer.say("           over #%d %s: %s" % (rival["seq"], rival["text"][:32],
                                                     explain(pick, rival, st["shifts"])))

    def bound_by(ctx, a):
        """The predecessors whose issue cycle plus edge latency reach the node's earliest cycle."""
        out = []
        e = ctx.u32(a["addr"] + N_PRED)
        guard = 0
        while e and guard < 64:
            guard += 1
            p = ctx.u32(e + 8)
            c = st.get("issued", {}).get(p)
            if c is not None:
                lat = ctx.u16(e + 0x14)
                q = st["nodes"].get(p)
                out.append((c + lat, "#%d at c%d %+d" % (q["seq"] if q else -1, c, lat)))
            e = ctx.u32(e + 4)
        if not out:
            return ""
        top = max(v for v, _ in out)
        return " (after %s%s)" % (", ".join(t for v, t in out if v == top),
                                 "" if top == a["earliest"] else "; + %d unit busy" % (a["earliest"] - top))

    def ctx_write(ctx, addr, data):
        tracer.dbg.write(addr, data)

    def preds_of(ctx, n):
        """[(predecessor node, edge latency)] of a node."""
        out = []
        e = ctx.u32(n + N_PRED)
        guard = 0
        while e and guard < 64:
            guard += 1
            out.append((ctx.u32(e + 8), ctx.u16(e + 0x14)))
            e = ctx.u32(e + 4)
        return out

    def on_fix(dbg, ctx):
        if not tracer.active or not st["nodes"]:
            return
        e = ctx.ecx
        a, b = st["nodes"].get(ctx.u32(e + 8)), st["nodes"].get(ctx.u32(e + 0xC))
        if a and b:
            tracer.say("           (#%d moves past #%d: its stack displacement is adjusted)" % (b["seq"], a["seq"]))

    def install(dbg, base):
        base_install(dbg, base)
        tracer.dbg = dbg
        if "sched" in want:
            dbg.add(rel(SCHED_DRIVER), on_driver)
            dbg.add(rel(SCHEDULE_REGION), on_region)
            dbg.add(rel(TAKE_NODE), on_take)
            dbg.add(rel(FIX_DISPLACEMENT), on_fix)

    base_close = getattr(tracer, "close", None)

    def close():
        if base_close:
            base_close()
        flush()

    tracer.install = install
    tracer.close = close
    return tracer


def formula(shifts):
    names = ["fold", "height", "succs", "critical", None, "read", "float-store"]
    out = []
    for i, n in enumerate(names):
        if n is None:
            continue
        s = shifts[i]
        if s >= 0:
            out.append("%s<<%d" % (n, s))
        elif n in ("height", "succs", "float-store"):
            out.append("%s>>%d" % (n, -s))
    return " + ".join(out)


def _selftest():
    sh = [-1, 13, -5, -1, -2, 16, 16, 0]
    t = priority_terms(sh, 5, 2, 1, 1, 0)
    assert t["height"] == 5 << 13 and t["reads"] == 1 << 16 and t["critical"] == 0 and t["succs"] == 0
    assert moved([6, 10, 1, 2]) == [(6, 1), (10, 1)]
    n = {"prio": (45 << 13) + (1 << 16), "height": 45, "reads": 0, "succs": 1}
    assert residual(n, sh) == 1 << 16
    return True


if __name__ == "__main__":
    print(_selftest())
