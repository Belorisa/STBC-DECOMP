"""Trace the decisions of VC6 SP5's back end (C2.DLL build 8966) while it compiles one function of a unit.

    python tools/c2trace.py <unit|0xADDRESS> [function] [--inline] [--regs] [--frame] [--all] [--raw]
    python tools/c2trace.py 0x661E40 --whatif site=30,-1 --whatif own=+31 --retail
    python tools/c2trace.py 0x662460 --cost --worth 7

The unit is compiled with its own flags by the real CL.EXE, run under a small debugger (tools/c2dbg.py) that stops
C2.DLL at the routines whose rules docs/c2-backend.md describes and prints what they decided:

- inline (default): for each function the back end compiles, its own cost and starting budget, then every call site
  the inline driver visits (depth first, in IL order): the callee, its cost (the front end's IL size, read from the
  symbol), the budget left, the decision and why (depth, budget, the 35,000 total, a parameter mismatch, a late
  rejection), and for an expansion the sub-budget its own sites get and what they consumed;
- regs: the register allocator's candidates for the function (see docs/c2-backend.md, "Registers");
- frame: the locals the frame layout places, with their sort keys and offsets;
- operands: the global optimiser's commutative operand sort, each comparison with the two operands' weights
  (tools/c2operands.py; docs/c2-backend.md, "Operand order of commutative operations").
- sched: the instruction scheduler, per region: input order, priorities, each pick and what decided it
  (tools/c2sched.py; docs/c2-backend.md, "Scheduling").

--header-override HEADER=FILE compiles the unit with a header replaced by a scratch copy (src/ untouched). In Git
Bash, write a scratch file as C:/... (a /c/... path is converted) and set MSYS_NO_PATHCONV=1 for --flags.

Under each budget verdict the inline trace prints what flips it (tools/c2inline.py, from the share formula), and it
records the trace in build/c2trace/<unit>--<name>.trace.json: --whatif replays the driver's arithmetic on that record
with a site added or removed or another own cost, --retail sets its out-of-line sites against retail's calls per
callee (tools/callset.py), and --cost --worth N lists the statements worth N tuples.

A function is picked by an address (its FUNCTION annotation names the unit and the name) or by a substring of its
undecorated name; --all traces every function of the unit. Nothing in tools/vc6 is modified: breakpoints are int3
bytes written into CL.EXE's memory after C2.DLL loads.

Method credit: byte-tactics (VC5 SP3, tools/c2prio.py, MIT) read C2's register priorities from a debugger; this tool
uses the same idea against VC6 SP5's C2.DLL with its own addresses and rules.
"""
import argparse
import ctypes
import json
import os
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import c2dbg  # noqa: E402
import c2inline  # noqa: E402
import vc6  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
SRC = ROOT / "src"
OUT = ROOT / "build" / "c2trace"
C2_BASE = 0x10700000    # C2.DLL's preferred base; breakpoints are relocated by the actual base

# --- C2.DLL 8966 addresses (docs/c2-backend.md) ---------------------------------------------------------------------
INL_TOP = 0x107180FD        # inline expansion of one function: ecx = function context, ctx[0] = function symbol
INL_DRIVER = 0x107181A8     # driver(ctx, depth, budget, forceflag): fastcall, returns the budget consumed
INL_SITES = 0x107181E3      # after the site list is built: [esp+0x24] = number of sites
INL_SITE = 0x107186EB       # one site: edi = callee symbol, ebx = site node, esi = actual argument count
INL_GATE_OK = 0x10718765    # passed depth, budget, total and parameter checks
INL_COPY = 0x107187AE       # the callee's IL is copied: the expansion is committed unless a late check fails
INL_RECURSE = 0x1071888B    # eax = the sub-budget for the copy's own sites
INL_RETURNED = 0x10718890   # eax = what the copy's sites consumed
INL_LATE_SIZE = 0x10794032  # /Os-style size check (sub_10794A84) rejected the expanded copy
INL_LATE_FREE = (0x10718418, 0x10793F40)   # copy freed before splicing (returned UDT, EH)
INL_TOTAL = 0x1079F234      # running total of the function's own cost plus everything expanded into it
OPT_BIT23 = 0x107AC0B4      # bit 23 of the function's option word (sub_1071BC73); 0 turns the size limit on
SIZE_LIMIT = 0x10799280     # 0x10 << vol (128 by default): the candidate size limit, applied only when OPT_BIT23 is 0

SYM_NAME, SYM_FLAGS36, SYM_PARAMS, SYM_COST, SYM_FLAGS73 = 0x18, 0x36, 0x6B, 0x6D, 0x73
FORCEINLINE = 0x2000


def undecorate(name):
    if not name or not name.startswith("?"):
        return name
    try:
        dbghelp = ctypes.WinDLL("dbghelp")
        buf = ctypes.create_string_buffer(1024)
        # UNDNAME_NAME_ONLY (0x1000)
        if dbghelp.UnDecorateSymbolName(name.encode("latin-1"), buf, 1024, 0x1000):
            return buf.value.decode("latin-1")
    except OSError:
        pass
    return name


# --- unit lookup ------------------------------------------------------------------------------------------------------
ANNOT = re.compile(r"//\s*FUNCTION:\s*STBC\s+0x([0-9A-Fa-f]+)")


def find_address(addr):
    """(unit, function name) for an annotated address."""
    sys.path.insert(0, str(ROOT / "tools"))
    for path in SRC.rglob("*"):
        if path.suffix.lower() not in (".c", ".cpp", ".h", ".hpp", ".inl"):
            continue
        text = path.read_text(encoding="latin-1")
        if ("0x%X" % addr).lower() not in text.lower() and ("0x%08X" % addr).lower() not in text.lower():
            continue
        lines = text.splitlines()
        for i, l in enumerate(lines):
            m = ANNOT.search(l)
            if m and int(m.group(1), 16) == addr:
                k = i + 1
                while k < len(lines) and lines[k].strip().startswith("//"):
                    k += 1
                head = " ".join(lines[k:k + 3])
                nm = re.search(r"([A-Za-z_~][\w:~]*(?:operator\s*\S+)?)\s*\(", head)
                name = nm.group(1) if nm else None
                if path.suffix.lower() in (".c", ".cpp"):
                    unit = path.relative_to(SRC).with_suffix("").as_posix()
                else:
                    bind = re.search(r"@(\S+)", l)
                    unit = bind.group(1) if bind else None
                return unit, name
    return None, None


def unit_source(unit):
    for ext in (".cpp", ".c"):
        p = SRC / (unit + ext)
        if p.exists():
            return p
    raise SystemExit("no source for unit %s" % unit)


# --- the tracer -------------------------------------------------------------------------------------------------------
class Tracer:
    def __init__(self, match, want, raw=False, out=None, margin=10):
        self.match, self.want, self.raw, self.out = match, want, raw, out
        self.margin = margin
        self.records = []    # the recorded inline trace of each traced function (tools/c2inline.py's format)
        self.record = None
        self.base = None
        self.active = False
        self.fn = None
        self.stack = []      # inline driver levels: dict(depth, budget, sites, visited)
        self.site = None
        self.functions = []
        self.costs = []      # (decorated, undecorated, front-end cost) of each traced function, for --cost
        self.default_limit = 8   # #pragma inline_depth default (the site node carries the pragma state of the function holding the site)

    def rel(self, a):
        return a - C2_BASE + self.base

    def say(self, s):
        (self.out or sys.stdout).write(s + "\n")

    def sym(self, ctx, s):
        name = ctx.cstr(ctx.u32(s + SYM_NAME))
        return name if self.raw else undecorate(name)

    def sym_raw(self, ctx, s):
        return ctx.cstr(ctx.u32(s + SYM_NAME))

    def install(self, dbg, base):
        self.base = base
        dbg.add(self.rel(INL_TOP), self.on_top)
        if "inline" in self.want:
            for a, h in ((INL_DRIVER, self.on_driver), (INL_SITES, self.on_sites), (INL_SITE, self.on_site),
                         (INL_GATE_OK, self.on_gate_ok), (INL_COPY, self.on_copy),
                         (INL_RECURSE, self.on_recurse), (INL_RETURNED, self.on_returned),
                         (INL_LATE_SIZE, self.on_late_size)):
                dbg.add(self.rel(a), h)
            for a in INL_LATE_FREE:
                dbg.add(self.rel(a), self.on_late_free)

    # inline ---------------------------------------------------------------------------------------------------------
    def on_top(self, dbg, ctx):
        fsym = ctx.u32(ctx.ecx)
        raw = ctx.cstr(ctx.u32(fsym + SYM_NAME))
        name = undecorate(raw)
        self.functions.append(name)
        self.fn_raw = raw
        self.active = self.match(raw, name)
        if self.active:
            self.costs.append((raw, name, ctx.s16(fsym + SYM_COST)))
        self.fn = name
        self.stack = []
        self.record = None
        if self.active and "inline" in self.want:
            cost = ctx.s16(fsym + SYM_COST)
            budget = max(1000, 2 * cost)
            self.record = {"name": name, "raw": raw, "own": cost, "budget": budget, "sites": []}
            self.records.append(self.record)
            self.say("")
            self.say("function %s  (own cost %d; budget max(1000, 2*%d) = %d; total cap 35000)"
                     % (raw if self.raw else name, cost, cost, budget))
            if 2 * cost > 35000:
                self.say("  no inline expansion: 2 * own cost > 35000")
            if ctx.u32(self.rel(OPT_BIT23)) == 0:
                self.say("  size limit on (option bit 23 clear): callees of cost >= %d are not candidates"
                         % ctx.s32(self.rel(SIZE_LIMIT)))

    def on_driver(self, dbg, ctx):
        if not self.active:
            return
        lvl = {"depth": ctx.edx, "budget": ctx.arg(0), "sites": 0, "visited": 0, "rec": [], "parent": None}
        if self.stack and self.stack[-1]["rec"]:
            up = self.stack[-1]
            ps = up["rec"][-1]
            ps["nested"] = lvl["rec"]
            ps["share"] = _signed(lvl["budget"])
            L = ps["budget"] - (ps["cost"] if c2inline.charged(ps) else 0)
            R = up["sites"] - up["visited"] + 1
            if R > 0 and c2inline.cdiv(L, R) == ps["share"]:
                lvl["parent"] = {"L": L, "R": R, "pos": "[%d/%d]" % (up["visited"], up["sites"]),
                                 "share": ps["share"]}
        elif not self.stack and self.record is not None:
            self.record["sites"] = lvl["rec"]
        self.stack.append(lvl)

    def ind(self):
        return "  " * len(self.stack)

    def on_sites(self, dbg, ctx):
        if not self.active or not self.stack:
            return
        lvl = self.stack[-1]
        lvl["sites"] = ctx.u32(ctx.esp + 0x24) if ctx.eax else 0
        if lvl["sites"] and len(self.stack) > 1:
            self.say("%s(%d nested sites, budget %d = left / sites not yet visited)"
                     % ("  " * (len(self.stack) - 1), lvl["sites"], lvl["budget"]))

    def on_site(self, dbg, ctx):
        if not self.active or not self.stack:
            return
        lvl = self.stack[-1]
        lvl["visited"] += 1
        callee = ctx.edi
        cost = ctx.s16(callee + SYM_COST)
        params = ctx.s16(callee + SYM_PARAMS)
        flags = ctx.u32(callee + SYM_FLAGS73)
        limit = ctx.u32(ctx.ebx + 8) & 0xFF
        budget = ctx.s32(ctx.esp + 0x48)
        total = ctx.s32(self.rel(INL_TOTAL))
        depth = ctx.u32(ctx.esp + 0x34)
        force = bool(flags & FORCEINLINE)
        why = []
        if depth > limit:
            why.append("depth %d > inline_depth %d" % (depth, limit))
        if not force:
            if budget < cost and cost > 40:
                why.append("cost %d > budget %d" % (cost, budget))
            if total > 35000:
                why.append("total %d > 35000" % total)
        self.site = {"callee": self.sym(ctx, callee), "cost": cost, "budget": budget, "why": why,
                     "force": force, "limit": limit, "depth": depth, "ok": False, "copied": False, "params": params, "args": ctx.esi}
        self.site_level = len(self.stack)
        rec = {"callee": self.site["callee"], "raw": self.sym_raw(ctx, callee), "cost": cost, "budget": budget,
               "force": force, "limit": limit, "depth": depth, "params": params, "args": ctx.esi,
               "verdict": "reject", "why": list(why), "share": None, "nested": None}
        lvl["rec"].append(rec)
        self.site["rec"] = rec

    def flush_site(self, verdict):
        s = self.site
        if s is None:
            return
        lvl = self.stack[-1] if self.stack else {"visited": 0, "sites": 0}
        lim = "" if s["limit"] == self.default_limit else "  (inline_depth %d)" % s["limit"]
        self.say("%s[%d/%d] %-48s cost %4d  budget %5d  %s%s"
                 % (self.ind(), lvl["visited"], lvl["sites"], s["callee"], s["cost"], s["budget"], verdict, lim))
        rec = s.get("rec")
        if rec is not None:
            if verdict.startswith("inline"):
                rec["verdict"] = "inline"
            elif "(late:" in verdict:
                rec["verdict"] = "late"     # charged: the copy was made (a rejection after the gate is not charged)
            note = c2inline.margin_text(rec, parent=lvl.get("parent"),
                                        fn=self.record if len(self.stack) == 1 else None, margin=self.margin)
            if note:
                self.say("%s      -> %s" % (self.ind(), note))
        self.site = None

    def on_gate_ok(self, dbg, ctx):
        if self.active and self.site:
            self.site["ok"] = True

    def on_copy(self, dbg, ctx):
        if not self.active or not self.site:
            return
        s = self.site
        free = s["force"] or s["cost"] <= 40
        left = s["budget"] if free else s["budget"] - s["cost"]
        tag = "inline (forceinline)" if s["force"] else ("inline (cost <= 40: free)" if s["cost"] <= 40
                                                          else "inline -> budget %d" % left)
        self.flush_site(tag)

    def on_recurse(self, dbg, ctx):
        pass

    def on_returned(self, dbg, ctx):
        if not self.active or not self.stack:
            return
        lvl = self.stack.pop()
        used = _signed(ctx.eax)
        if lvl["sites"] or used:
            self.say("%s  (nested sites consumed %d)" % (self.ind(), used))

    def last_rec(self):
        return self.stack[-1]["rec"][-1] if self.stack and self.stack[-1]["rec"] else None

    def on_late_size(self, dbg, ctx):
        if self.active:
            self.say("%s  -> expansion undone: size check (-vol) rejected the expanded copy" % self.ind())
            if self.last_rec():
                self.last_rec()["verdict"] = "late"

    def on_late_free(self, dbg, ctx):
        if not self.active:
            return
        if self.site:
            self.flush_site("not inlined (late: returns a UDT / EH state)")
        else:
            self.say("%s  -> expansion undone (late check)" % self.ind())
            if self.last_rec():
                self.last_rec()["verdict"] = "late"

    def finish_level_sites(self):
        pass


def _signed(v):
    return v - (1 << 32) if v & 0x80000000 else v


class InlineTracer(Tracer):
    """Adds the rejected-site output: a site that never reaches the copy is printed at the next site or level end."""

    def on_site(self, dbg, ctx):
        if self.site is not None:
            self.flush_site(self.reject_text())
        super().on_site(dbg, ctx)

    def reject_text(self):
        s = self.site
        if s["why"]:
            return "not inlined: " + "; ".join(s["why"])
        if s["args"] != s["params"]:
            return "not inlined: %d arguments for %d parameters" % (s["args"], s["params"])
        if s["ok"]:
            return "not inlined: rejected after the budget gate (address taken / UDT / EH / setjmp)"
        return "not inlined"

    def on_returned(self, dbg, ctx):
        if self.site is not None:
            self.flush_site(self.reject_text())
        super().on_returned(dbg, ctx)

    def on_top(self, dbg, ctx):
        if self.site is not None:
            self.flush_site(self.reject_text())
        super().on_top(dbg, ctx)

    def close(self):
        if self.site is not None:
            self.flush_site(self.reject_text())


def msys_path(p):
    """A Git Bash path (/c/Recomp/x.cpp) as Windows spells it (C:/Recomp/x.cpp); other paths unchanged."""
    m = re.fullmatch(r"/([A-Za-z])(/.*)?", p or "")
    return "%s:%s" % (m.group(1).upper(), m.group(2) or "/") if m else p


def parse_overrides(specs):
    """--header-override HEADER=FILE ... -> [(repository header Path, replacement Path)]."""
    out = []
    for spec in specs or ():
        if "=" not in spec:
            raise SystemExit("--header-override takes HEADER=FILE, got %r" % spec)
        h, f = (msys_path(x.strip()) for x in spec.split("=", 1))
        hp = Path(h) if os.path.isabs(h) else ROOT / h
        fp = Path(f).resolve()
        if not hp.is_file():
            raise SystemExit("--header-override: no header %s" % h)
        if not fp.is_file():
            raise SystemExit("--header-override: no file %s" % f)
        out.append((hp.resolve(), fp))
    return out


def shadow_includes(args, overrides, source):
    """Compile with headers replaced, leaving src/ untouched: each overridden header's directory is shadowed in
    build/c2trace/override/ (its headers copied, the replacement over the one named) and searched first, the original
    directory right after it for includes into its subdirectories; when the
    unit's own source sits in a shadowed directory it is compiled from there, so its quoted includes see the copies.
    Returns the new argument list."""
    import shutil
    root = OUT / "override"
    base = ROOT.resolve()
    shadows = {}
    for header, repl in overrides:
        d = header.parent
        if d not in shadows:
            try:
                sd = root / d.relative_to(base)
            except ValueError:
                sd = root / d.name
            if sd.exists():
                shutil.rmtree(sd)
            sd.mkdir(parents=True)
            for f in d.iterdir():
                if f.is_file() and f.suffix.lower() in (".h", ".hpp", ".inl"):
                    shutil.copy2(f, sd / f.name)
            shadows[d] = sd
        shutil.copy2(repl, shadows[d] / header.name)
    out, i = [], 0
    while i < len(args):
        a = args[i]
        if a == "/I" and i + 1 < len(args) and Path(args[i + 1]).resolve() in shadows:
            # the shadow first, then the original directory: the shadow holds only the top-level headers, so an
            # include into a subdirectory ("renderer/NiImage.h") still resolves from the original
            out += ["/I", str(shadows[Path(args[i + 1]).resolve()]), "/I", args[i + 1]]
            i += 2
            continue
        out.append(a)
        i += 1
    extra = [x for d, sd in shadows.items() if str(sd) not in out for x in ("/I", str(sd), "/I", str(d))]
    first = out.index("/I") if "/I" in out else 0
    out[first:first] = extra
    src = Path(source).resolve()
    if src.parent in shadows:
        copy = shadows[src.parent] / src.name
        shutil.copy2(src, copy)
        out = [str(copy) if x == str(source) else x for x in out]
    return out


def build_command(unit, extra=(), overrides=()):
    if os.path.isfile(unit):      # a scratch file: compiled with --flags only
        OUT.mkdir(parents=True, exist_ok=True)
        src = os.path.abspath(unit)
        obj = OUT / (Path(src).stem + ".obj")
        exe = vc6.COMPILERS[None] / "CL.EXE"
        tp = "/Tp" if src.lower().endswith(".cpp") else "/Tc"
        args = ["/nologo", "/c", *extra, tp, src, "/Fo" + str(obj)]
        if overrides:
            args = shadow_includes(args, overrides, src)
        return [str(exe), *args]
    sys.path.insert(0, str(ROOT / "tools"))
    import decomp
    path = unit_source(unit)
    comp = decomp.compiler_of(path)
    if comp is not None:
        raise SystemExit("unit %s is built with compiler %r; c2trace knows SP5's C2.DLL only" % (unit, comp))
    OUT.mkdir(parents=True, exist_ok=True)
    obj = OUT / (unit.replace("/", "_") + ".obj")
    args = ["/c", *decomp.cl_args(unit, path, pch=obj.with_suffix(".pch")), "/Fo" + str(obj), *extra]
    decomp.fresh_pch(args)   # a /YX unit creates its .pch beside the object, afresh
    if overrides:
        args = shadow_includes(args, overrides, str(path))
    exe = vc6.COMPILERS[None] / "CL.EXE"
    return [str(exe), *args]


def main(argv=None):
    ap = argparse.ArgumentParser(
        description=__doc__.split("\n\n")[0],
        epilog="In Git Bash: write a scratch file as C:/... (a /c/... path is converted) and set MSYS_NO_PATHCONV=1, "
               "or --flags \"/O2\" is rewritten into a path and the file is compiled without optimisation.")
    ap.add_argument("target", help="a unit (game/Foo), an annotated address (0x59A2C0) or a scratch .c/.cpp file "
                                   "(C:/... in Git Bash)")
    ap.add_argument("function", nargs="?", help="substring of the undecorated name (default: the address's)")
    ap.add_argument("--inline", action="store_true", help="trace inline expansion (the default)")
    ap.add_argument("--regs", action="store_true", help="trace register allocation")
    ap.add_argument("--frame", action="store_true", help="trace frame layout")
    ap.add_argument("--sched", action="store_true",
                    help="trace the instruction scheduler: per region the input order, each pick with its priority "
                         "and the ready list, and what decided it (tools/c2sched.py)")
    ap.add_argument("--operands", action="store_true",
                    help="trace the commutative operand sort: each comparison's operands, weights and winner "
                         "(tools/c2operands.py)")
    ap.add_argument("--addr", action="store_true",
                    help="trace two-register addresses ([base+index+disp]): which operand the sort made the base, "
                         "the hashes behind it and the symbol-number shifts that flip it (tools/c2addr.py)")
    ap.add_argument("--cost", action="store_true", help="the front end's cost of the function: its IL tuples (C1XX)")
    ap.add_argument("--all", action="store_true", help="trace every function of the unit")
    ap.add_argument("--raw", action="store_true", help="print decorated names")
    ap.add_argument("--header-override", action="append", metavar="HEADER=FILE",
                    help="compile with a repository header replaced by a scratch copy, leaving src/ untouched "
                         "(src/netimmerse/NiAVObject.h=C:/scratch/NiAVObject.h); repeatable")
    ap.add_argument("--flags", default="/O2 /GX", help="compiler flags for a scratch .c/.cpp file (default /O2 /GX)")
    ap.add_argument("--list", action="store_true", help="only list the functions the back end compiles, in order")
    ap.add_argument("--margin", type=int, default=10,
                    help="print what flips an expanded site whose budget margin is below this (default 10)")
    ap.add_argument("--whatif", action="append", metavar="EDIT",
                    help="replay the recorded trace's arithmetic with an edit: site=K[.J],+1|-1[,cost=N] (one more "
                         "site at position K, cost 24 unless given, or K removed) or own=+N|-N; repeatable")
    ap.add_argument("--recorded", metavar="JSON", help="--whatif/--retail on this recorded trace, without compiling")
    ap.add_argument("--retail", action="store_true",
                    help="compare the trace's out-of-line sites with retail's calls per callee (tools/callset.py)")
    ap.add_argument("--worth", type=int, metavar="N",
                    help="with --cost: the statements worth exactly N tuples, and adjacent pairs summing to N")
    a = ap.parse_args(argv)
    if a.worth is not None:
        a.cost = True
    if a.recorded:
        return replay_recorded(a)

    want = set()
    if a.inline:
        want.add("inline")
    if a.regs:
        want.add("regs")
    if a.frame:
        want.add("frame")
    if a.cost:
        want.add("cost")
    if a.sched:
        want.add("sched")
    if a.operands:
        want.add("operands")
    if a.addr:
        want.add("addr")
    if not want or a.whatif or a.retail:
        want.add("inline")

    a.target = msys_path(a.target)
    unit, fname = a.target, a.function
    if re.fullmatch(r"0x[0-9A-Fa-f]+", a.target):
        unit, name = find_address(int(a.target, 16))
        if unit is None:
            raise SystemExit("no FUNCTION annotation for %s (or it sits in a header without @unit)" % a.target)
        fname = fname or name
    if a.list:
        a.all, want = True, set()
    if not fname and not a.all:
        raise SystemExit("name a function (or --all)")

    def match(raw, name):
        if a.all:
            return True
        return fname in name or fname in (raw or "")

    tracer = InlineTracer(match, want, raw=a.raw, margin=a.margin)
    if "regs" in want:
        import c2regs
        tracer = c2regs.extend(tracer, want)
    if "frame" in want:
        import c2frame
        tracer = c2frame.extend(tracer, want)
    if "sched" in want:
        import c2sched
        tracer = c2sched.extend(tracer, want)
    if "operands" in want:
        import c2operands
        tracer = c2operands.extend(tracer, want)
    if "addr" in want:
        import c2addr
        tracer = c2addr.extend(tracer, want)
    cmd = build_command(unit, a.flags.split() if os.path.isfile(unit) else (),
                        overrides=parse_overrides(a.header_override))
    dbg = c2dbg.Debugger(cmd, env=vc6.env(), cwd=str(OUT))
    dbg.on_module("c2.dll", lambda base: tracer.install(dbg, base))
    costs = None
    if "cost" in want:
        import c2cost
        costs = c2cost.attach(dbg)
    print("unit %s: %s" % (unit, " ".join(cmd[1:3])) if False else "unit %s" % unit)
    rc = dbg.run()
    tracer.close()
    if costs is not None:
        costs.report_traced(tracer.costs, raw=a.raw)
        if a.worth is not None:
            report_worth(costs, tracer.costs, a.worth, raw=a.raw)
    if tracer.records:
        path = save_records(unit, fname if not a.all else None, tracer.records)
        if a.whatif or a.retail:
            print("")
            print("recorded: %s" % path.relative_to(ROOT).as_posix())
        post_trace(a, tracer.records)
    if a.list:
        for n in tracer.functions:
            print("  " + n)
    elif not any(match("", n) or a.all for n in tracer.functions):
        print("no function matched %r; the back end compiled:" % fname)
        for n in tracer.functions:
            print("  " + n)
    if rc:
        print("cl exited with %d" % rc)
    return 0


def save_records(unit, fname, records):
    """Write the recorded inline trace to build/c2trace/<unit>[--<function>].trace.json and return its path."""
    stem = re.sub(r"[^\w.-]+", "_", Path(unit).as_posix() if not os.path.isfile(unit) else Path(unit).stem)
    if fname:
        stem += "--" + re.sub(r"[^\w.-]+", "_", fname)
    path = OUT / (stem + ".trace.json")
    OUT.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps({"unit": unit, "function": fname, "functions": records}), encoding="utf-8")
    return path


def post_trace(a, records, out=print):
    """--whatif and --retail on the recorded functions."""
    if not (a.whatif or a.retail):
        return
    edits = [c2inline.parse_edit(e) for e in (a.whatif or [])]
    for fn in records:
        if edits:
            bad = c2inline.check_replay(fn)
            if bad:
                out("")
                out("warning: replaying %s's record disagrees with the compiler at %d sites (first: [%s] %s); "
                    "the what-if below is approximate" % (fn["name"], len(bad), c2inline.path_text(bad[0][0]),
                                                          bad[0][1]["callee"]))
            c2inline.report_whatif(fn, edits, records, out)
        if a.retail:
            retail_report(a, fn, out)


def retail_counts(addr, fn):
    """(retail's out-of-line calls per callee raw name, ours' per callee from build/obj, or a reason string)."""
    import callset
    try:
        data = callset.load()
    except SystemExit as e:
        return None, None, str(e)
    f = data["functions"].get("0x%X" % addr)
    if f is None:
        return None, None, ("0x%X is not an open (DIFF) function in build/report.json: if it matches, retail's "
                            "calls are ours" % addr)
    by_raw = {}
    for target, v in data["names"].items():
        if len(v) > 2 and v[2]:
            by_raw.setdefault(v[2], []).append(target)
    retail, ours = {}, {}
    unnamed = sorted((t, n) for t, n in f["retail"].items() if t not in data["names"])
    for _, s, _ in c2inline.walk(fn["sites"]):
        raw = s["raw"]
        if raw in retail:
            continue
        targets = by_raw.get(raw)
        retail[raw] = sum(f["retail"].get(t, 0) for t in targets) if targets else None
        n = sum(f["ours"].get(t, 0) for t in targets or ()) + f["ours"].get("?" + raw, 0)
        if targets or ("?" + raw) in f["ours"]:
            ours[raw] = n
    return retail, ours, unnamed


def retail_report(a, fn, out=print):
    addr = int(a.target, 16) if re.fullmatch(r"0x[0-9A-Fa-f]+", a.target) else None
    if addr is None:
        out("--retail needs the function's address as the target")
        return
    retail, ours, extra = retail_counts(addr, fn)
    if isinstance(extra, str):
        out("")
        out("--retail: " + extra)
        return
    c2inline.report_retail(fn, retail, ours, out, unnamed=extra)


def replay_recorded(a):
    data = json.loads(Path(a.recorded).read_text(encoding="utf-8"))
    records = data["functions"]
    if a.function:
        records = [f for f in records if a.function in f["name"] or a.function in f["raw"]]
    if not records:
        raise SystemExit("no recorded function matches")
    post_trace(a, records)
    return 0


def report_worth(costs, traced, n, raw=False, out=print):
    """--cost --worth N: the statements (and adjacent pairs) of each traced function worth exactly N tuples."""
    import c2cost
    for draw, name, cost, rec in c2cost.pair(costs.records, traced):
        if rec is None:
            continue
        single, pairs = c2inline.worth(rec[1], n)
        out("")
        out("statements of %s worth %d tuples (statements cut at discard/return/jump/branch/sequence point; "
            "numbered from 1):" % (draw if raw else name, n))
        for i, st in single:
            out("  #%d  %s" % (i, " ".join(c2cost.opname(o) for o in st)))
        for i, s1, s2 in pairs:
            out("  #%d+#%d  %d+%d  %s | %s" % (i, i + 1, len(s1), len(s2), " ".join(c2cost.opname(o) for o in s1),
                                               " ".join(c2cost.opname(o) for o in s2)))
        if not single and not pairs:
            out("  none")


if __name__ == "__main__":
    sys.exit(main())
