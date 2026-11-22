"""Runtime leads from the running game: what class `this` is in a function, who calls it, what its arguments are.

Frida attaches to stbc.exe (the retail binary, loaded at its fixed base, so retail addresses are live addresses)
and hooks each target function's entry. Per call it records ecx and the vtable ecx points at, the return address
and the vtables of the first stack arguments. Each target detaches after a few distinct (vtable, caller) pairs, so
the game stays playable with thousands of hooks.

    python tools/trace.py targets                  # unidentified functions worth tracing -> build/runtime/targets.txt
    python tools/trace.py run [--spawn] [--cap 12] # hook them (attach to a running stbc.exe, or start it); Ctrl-C
                                                   # stops; samples -> build/runtime/trace-<time>.jsonl
    python tools/trace.py report [FILES...]        # per target: classes of `this` and callers (--args: the stack
                                                   # slots' classes, stale when the function takes fewer)

A vtable is named from config/vtables.txt, else from the annotated or claimed function in its slot 0 (the class's
deleting destructor or its first virtual). Code that never runs gives no sample: play the parts of the game that
reach it. The game is config/project.json's "game", or --game.
"""
import argparse
import bisect
import json
import re
import struct
import sys
import time
from collections import Counter, defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import decomp  # noqa: E402

OUT = decomp.ROOT / "build" / "runtime"
TARGETS = OUT / "targets.txt"
DEFAULT_GAME = "C:/Recomp/Star Trek Bridge Commander/stbc.exe"

AGENT = r"""
const targets = %TARGETS%;
const cap = %CAP%;
const vtLo = %VTLO%, vtHi = %VTHI%;
const maxHits = 4000;

function vtableOf(p) {
    try {
        if (p < 0x10000) return 0;
        const v = ptr(p).readU32();
        return (v >= vtLo && v < vtHi) ? v : 0;
    } catch (e) { return 0; }
}

function readable(p) {
    try { if (p < 0x10000) return false; ptr(p).readU32(); return true; } catch (e) { return false; }
}

let installed = 0, failed = 0;
for (const a of targets) {
    const seen = new Set();
    let hits = 0, listener = null, done = false;
    try {
        listener = Interceptor.attach(ptr(a), {
            onEnter(args) {
                if (done) return;
                hits++;
                const ctx = this.context;
                const ecx = ctx.ecx.toUInt32();
                const vt = vtableOf(ecx);
                const ret = this.returnAddress.toUInt32();
                const key = vt + ':' + ret;
                if (!seen.has(key)) {
                    seen.add(key);
                    const sp = ctx.esp;
                    const argv = [];
                    for (let i = 1; i <= 4; i++) {
                        try { const v = sp.add(4 * i).readU32(); argv.push([v, vtableOf(v)]); } catch (e) { break; }
                    }
                    send({ t: a, ecx: ecx, vt: vt, ptr: vt ? 1 : (readable(ecx) ? 1 : 0), ret: ret, args: argv, hits: hits });
                }
                if (seen.size >= cap || hits >= maxHits) {
                    done = true;
                    send({ t: a, detach: true, hits: hits });
                    setTimeout(() => listener.detach(), 0);
                }
            }
        });
        installed++;
    } catch (e) {
        failed++;
        send({ t: a, error: String(e) });
    }
}
send({ installed: installed, failed: failed });
"""


def load_rows():
    import openwork
    rows, claims = openwork.load()
    return openwork, rows, claims


def cmd_targets(args):
    """Unidentified functions the open work depends on: callees of open and claimed functions, own vtable slots."""
    openwork, rows, claims = load_rows()
    img = decomp.pe.Image(decomp.target_image())
    annotated = {int(r["addr"], 16): r for r in rows}
    known = set(annotated) | {a for _, a, _ in claims} | set(openwork.INLINE_COPIES)
    starts = sorted(known)
    reasons = defaultdict(set)

    def scan(addr, why):
        i = bisect.bisect_right(starts, addr)
        stop = starts[i] if i < len(starts) else addr + openwork.SCAN_CAP
        try:
            callees, _ = openwork.callees(img, addr, stop)
        except Exception:
            return
        for t in callees:
            if t not in known and img.section_of(t) == img.section_of(addr):
                reasons[t].add(why)

    for addr, r in annotated.items():
        if r["status"] != "MATCH" and not (r["unit"] or "").startswith("python/"):
            scan(addr, "callee of open %s" % (r["name"] or hex(addr)))
    for c, addr, name, size, unknown in openwork.readiness(rows, claims):
        for t in unknown:
            reasons[t].add("callee of claimed %s" % name)
    cart = decomp.ROOT / "build" / "cartography.json"
    if cart.exists():
        for e in json.loads(cart.read_text(encoding="utf-8")):
            for slot in e.get("unknown_slots", []):
                addr = int(slot.split(":")[1], 16)
                if addr not in known and img.section_of(addr) and img.section_of(addr)[0] == ".text":
                    reasons[addr].add("%s vtable slot %s" % (e["class"], slot.split(":")[0]))
    OUT.mkdir(parents=True, exist_ok=True)
    with TARGETS.open("w", encoding="utf-8") as f:
        for a in sorted(reasons):
            f.write("0x%06X  # %s\n" % (a, "; ".join(sorted(reasons[a]))[:300]))
    print("%d targets -> %s" % (len(reasons), TARGETS.relative_to(decomp.ROOT)))


def read_targets(path):
    out = []
    for line in Path(path).read_text(encoding="utf-8").splitlines():
        body = line.split("#")[0].strip()
        if body:
            out.append(int(body, 16))
    return out


def vtable_entries():
    """(class, retail address) of config/vtables.txt; a template class's name may hold spaces."""
    for line in (decomp.CONFIG / "vtables.txt").read_text(encoding="utf-8").splitlines():
        m = re.match(r"\s*(.+?)\s+(0x[0-9A-Fa-f]+)\b", line.split("#")[0])
        if m:
            yield m.group(1), int(m.group(2), 16)


def vtable_range(img):
    """The section that holds the vtables (the one config/vtables.txt's entries are in)."""
    for _, addr in vtable_entries():
        s = img.section_of(addr)
        if s:
            return s[1], s[1] + s[2]
    raise SystemExit("no vtable in config/vtables.txt lies in a section of the image")


def cmd_run(args):
    import frida
    img = decomp.pe.Image(decomp.target_image())
    targets = read_targets(args.targets)
    lo, hi = vtable_range(img)
    game = Path(args.game or decomp.PROJECT.get("game", DEFAULT_GAME))
    data = game.read_bytes()
    if decomp.hashlib.sha1(data).hexdigest() != decomp.PROJECT["sha1"]:
        sys.exit("%s is not the retail binary (SHA-1 differs): its addresses would not be retail's" % game)
    device = frida.get_local_device()
    if args.spawn:
        pid = device.spawn([str(game)], cwd=str(game.parent))
        session = device.attach(pid)
    else:
        session = device.attach(game.name)
        pid = None
    src = (AGENT.replace("%TARGETS%", json.dumps(targets)).replace("%CAP%", str(args.cap))
           .replace("%VTLO%", str(lo)).replace("%VTHI%", str(hi)))
    OUT.mkdir(parents=True, exist_ok=True)
    out = OUT / time.strftime("trace-%Y%m%d-%H%M%S.jsonl")
    f = out.open("w", encoding="utf-8")
    stats = Counter()

    def on_message(msg, _data):
        if msg["type"] != "send":
            print("agent:", msg.get("description") or msg)
            return
        p = msg["payload"]
        if "installed" in p:
            print("hooks installed: %d, failed: %d" % (p["installed"], p["failed"]))
            return
        stats["detached" if p.get("detach") else "error" if p.get("error") else "sample"] += 1
        f.write(json.dumps(p) + "\n")
        f.flush()

    script = session.create_script(src)
    script.on("message", on_message)
    detached = []
    session.on("detached", lambda reason, *_: detached.append(reason))
    script.load()
    if pid is not None:
        device.resume(pid)
    print("tracing %d targets -> %s (Ctrl-C to stop)" % (len(targets), out.relative_to(decomp.ROOT)))
    try:
        while not detached:
            time.sleep(5)
            print("  samples %d, targets done %d, errors %d" % (stats["sample"], stats["detached"], stats["error"]))
    except KeyboardInterrupt:
        pass
    finally:
        try:
            session.detach()
        except Exception:
            pass
        f.close()
    print("stopped (%s); %d samples" % (detached[0] if detached else "Ctrl-C", stats["sample"]))


def names():
    """Retail address -> name, from annotations and claims."""
    openwork, rows, claims = load_rows()
    out = {}
    for name, addr, _ in claims:
        out.setdefault(addr, name)
    for r in rows:
        out[int(r["addr"], 16)] = r["name"] or r.get("symbol") or "?"
    return openwork, out


def class_of_name(openwork, name):
    m = re.match(r"(\w+)::", name or "")
    if m:
        return m.group(1)
    return openwork.class_of_decorated(name or "")


def compiled_vtables(img):
    """Retail vtable address -> class, from the matched functions: a relocation to '??_7Class@@6B@' in a function
    whose bytes match retail's holds, at the same offset in retail, that class's retail vtable."""
    import coff
    rep = decomp.load_report()
    by_unit = defaultdict(list)
    for r in rep["functions"]:
        if r["status"] == "MATCH" and r.get("symbol"):
            by_unit[r["unit"]].append(r)
    out = {}
    for unit, rows in by_unit.items():
        path = decomp.ROOT / "build" / "obj" / (unit + ".obj")
        if not path.exists():
            continue
        obj = coff.Coff(path.read_bytes())
        by_name = {}
        for s in obj.symbols.values():
            if s.defined:
                by_name.setdefault(s.name, s)
        for r in rows:
            sym = by_name.get(r["symbol"])
            if sym is None:
                continue
            sec = obj.section(sym.section)
            start, end = obj.function_extent(sym)
            for off, idx, typ in sec.relocs:
                target = obj.symbols.get(idx)
                if not (start <= off < end) or target is None or not target.name.startswith("??_7") or typ != 6:
                    continue   # 6: IMAGE_REL_I386_DIR32
                m = re.match(r"\?\?_7(.+?)@@6B", target.name)
                if not m:
                    continue
                addend = struct.unpack_from("<I", sec.data, off)[0]
                vt = img.dword(int(r["addr"], 16) + off - start) - addend
                out.setdefault(vt, m.group(1))
    return out


def vtable_names(img, openwork, fn_names):
    known = compiled_vtables(img)
    known.update({addr: cls for cls, addr in vtable_entries()})

    def name(vt):
        if not vt:
            return None
        if vt in known:
            return known[vt]
        # the classes of the slots' known functions: overrides name the class, inherited slots its bases
        text = img.text
        classes = Counter()
        for i in range(160):
            try:
                slot = img.dword(vt + 4 * i)
            except Exception:
                break
            if not (text[1] <= slot < text[1] + text[2]) or (i and vt + 4 * i in known):
                break
            c = class_of_name(openwork, fn_names.get(slot))
            if c:
                classes[c] += 1
        if not classes:
            return "vtable 0x%X" % vt
        return "vtable 0x%X {%s}" % (vt, " ".join("%s:%d" % kv for kv in classes.most_common(3)))
    return name


def cmd_report(args):
    img = decomp.pe.Image(decomp.target_image())
    openwork, fn_names = names()
    vname = vtable_names(img, openwork, fn_names)
    starts = sorted(fn_names)

    sizes = {int(r["addr"], 16): r.get("size") for r in decomp.load_report()["functions"] if r.get("size")}

    def where(ret):
        """The function holding the return address. Past the end of the nearest annotated function it lies in
        unannotated code (often the traced function itself, recursing), so it is shown as a gap after it."""
        i = bisect.bisect_right(starts, ret) - 1
        if i < 0:
            return "0x%X" % ret
        start = starts[i]
        if start in sizes and ret - start >= sizes[start]:
            return "gap at 0x%X (after %s)" % (ret, fn_names[start][:40])
        return "%s+0x%X" % (fn_names[start][:50], ret - start)

    files = [Path(p) for p in args.files] or sorted(OUT.glob("trace-*.jsonl"))
    reasons = {}
    if TARGETS.exists():
        for line in TARGETS.read_text(encoding="utf-8").splitlines():
            if "#" in line:
                a, why = line.split("#", 1)
                reasons[int(a.strip(), 16)] = why.strip()
    per = defaultdict(lambda: {"this": Counter(), "callers": Counter(), "args": defaultdict(Counter), "hits": 0})
    for p in files:
        for line in p.read_text(encoding="utf-8").splitlines():
            s = json.loads(line)
            if "vt" not in s:
                continue
            e = per[s["t"]]
            e["hits"] = max(e["hits"], s.get("hits", 0))
            if s["vt"]:
                e["this"][vname(s["vt"])] += 1
            elif s.get("ptr"):
                e["this"]["(ecx points at memory with no vtable: an object without one, or not thiscall)"] += 1
            elif "ptr" in s:
                e["this"]["(ecx not a pointer: not a member call)"] += 1
            else:
                e["this"]["(no vtable in ecx)"] += 1
            e["callers"][where(s["ret"])] += 1
            for i, (_, vt) in enumerate(s.get("args", [])):
                if vt:
                    e["args"][i][vname(vt)] += 1
    print("%d functions seen, from %s" % (len(per), ", ".join(p.name for p in files)))
    for t in sorted(per):
        e = per[t]
        print("\n0x%X %s  (%d+ calls)" % (t, fn_names.get(t, ""), e["hits"]))
        if t in reasons:
            print("  wanted as: %s" % reasons[t][:150])
        print("  this: " + ", ".join("%s x%d" % kv for kv in e["this"].most_common(4)))
        print("  callers: " + ", ".join("%s x%d" % kv for kv in e["callers"].most_common(4)))
        if args.args:
            # stack slots past the real arguments hold stale values: a class seen once is noise
            for i in sorted(e["args"]):
                kept = [kv for kv in e["args"][i].most_common(3) if kv[1] >= 2]
                if kept:
                    print("  stack slot %d (if the function takes it): %s"
                          % (i + 1, ", ".join("%s x%d" % kv for kv in kept)))


def main(argv):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    sub.add_parser("targets")
    r = sub.add_parser("run")
    r.add_argument("--targets", default=str(TARGETS))
    r.add_argument("--spawn", action="store_true", help="start the game instead of attaching to it")
    r.add_argument("--cap", type=int, default=12, help="distinct (vtable, caller) pairs per target before detaching")
    r.add_argument("--game")
    rep = sub.add_parser("report")
    rep.add_argument("files", nargs="*")
    rep.add_argument("--args", action="store_true",
                     help="also print the classes seen in the first stack slots (unreliable: stale for shorter "
                     "argument lists)")
    args = ap.parse_args(argv)
    {"targets": cmd_targets, "run": cmd_run, "report": cmd_report}[args.cmd](args)


if __name__ == "__main__":
    main(sys.argv[1:])
