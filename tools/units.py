"""Where a source unit's boundaries may disagree with retail's, from retail addresses and the annotations.

    python tools/units.py [boundaries|scattered|order] [UNIT]

`boundaries`: each `.cpp` whose own annotated code is interrupted by a static-initialiser run that carries its own
`<iostream>` quads (a stub calling `ios_base::Init::Init` or `_Winit::_Winit`) or the locale-id pair (the
test-and-set of `std::ctype<unsigned short>::id`'s guard byte). Such a run with the unit's code on both sides is a
candidate for two retail units (docs/levers/statics.md): both conditions must hold, and it stays a candidate until
the `.bss` stores and the second run's own quads confirm it. A run with only registrations or handler quads is no
boundary and is not listed.

`scattered`: each unit whose own code other units' own code splits into several runs (a function in the wrong
`.cpp`, or a `.cpp` that is several retail units).

`order`: each unit whose functions are defined in an order different from retail's address order. Function order
changes bytes (docs/levers/walls.md, the EH-state entry: an inline's out-of-line copy follows the first function
calling it), so a function out of place is worth checking before its wall is called bounded; moving a definition
only to move codegen is still a fakematch. Listed: the functions out of place (those outside the longest run already
in retail's order), ranked by how many of them are open; with a unit named, all of them.

Only a unit's own definitions count (its `.cpp`, not the headers or included files it binds copies of), and deleting
destructors are left out (they sit with the unit's COMDAT copies). Reads
build/report.json (run `decomp.py build` first) and the source annotations.
"""

import argparse
import bisect
import sys
from collections import defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import decomp  # noqa: E402

IOS_INIT = 0x855F23     # std::ios_base::Init::Init (src/cpprt/iostream.cpp)
WINIT = 0x856FD1        # std::_Winit::_Winit (src/cpprt/wiostrea.cpp)
LOCALE_GUARD = bytes.fromhex("8A0D343E9B00")   # mov cl, [0x9B3E34]: the locale-id pair's test-and-set


def stub_kind(img, addr, size):
    """"iostream", "locale" or None: what a static initialiser at addr does, read from retail's bytes."""
    code = img.read(addr, max(size, 16)) or b""
    if LOCALE_GUARD in code:
        return "locale"
    for a, m, o in decomp.disasm(code, addr):
        if m == "call" and o.startswith("0x") and int(o, 16) in (IOS_INIT, WINIT):
            return "iostream"
    return None


def own_functions(rows):
    """{unit: [(address, row)]} of the functions each unit defines in its own file (not a header's or an included
    file's copies, nor `_$E` stubs), in source order."""
    by_addr = {int(r["addr"], 16): r for r in rows}
    out = {}
    for unit, path in decomp.source_files().items():
        mine = [e for e in decomp.scan(unit, path) if e.kind == "FUNCTION" and not getattr(e, "bind", None)]
        sel = []
        for e in sorted(mine, key=lambda e: e.line):
            r = by_addr.get(e.addr)
            if r is not None and r.get("unit") == unit and not r.get("static"):
                sel.append((e.addr, r))
        if sel:
            out[unit] = sel
    return out


GENERATED = ("??_G", "??_E")   # deleting destructors: placed with the unit's COMDAT copies, not its code


def boundaries(rows, own, img):
    """[(unit, before, after, [(address, kind, owner unit)])]: the static runs with `<iostream>` quads or a locale
    pair that lie between two of a unit's own functions with no other unit's own function between them (a unit split
    by other units' code is `scattered`'s, not this)."""
    statics = sorted((int(r["addr"], 16), r) for r in rows if r.get("static"))
    keys = [a for a, _ in statics]
    owner = sorted((a, u) for u, funcs in own.items() for a, r in funcs if not generated(r))
    owner_keys = [a for a, _ in owner]
    kinds = {}
    out = []
    for unit, funcs in sorted(own.items()):
        addrs = sorted((f for f in funcs if not generated(f[1])), key=lambda f: f[0])
        for (a, ra), (b, rb) in zip(addrs, addrs[1:]):
            between = owner[bisect.bisect_right(owner_keys, a):bisect.bisect_left(owner_keys, b)]
            if any(u != unit for _, u in between):
                continue
            found = []
            for s, r in statics[bisect.bisect_right(keys, a):bisect.bisect_left(keys, b)]:
                if s not in kinds:
                    kinds[s] = stub_kind(img, s, r.get("size", 0))
                if kinds[s]:
                    found.append((s, kinds[s], r.get("unit")))
            if found:
                out.append((unit, ra, rb, found))
    return out


def generated(r):
    return (r.get("symbol") or "").startswith(GENERATED)


def scattered(own):
    """{unit: number of runs} for the units whose own code other units' own code splits into several runs."""
    owner = sorted((a, u) for u, funcs in own.items() for a, r in funcs if not generated(r))
    runs = defaultdict(int)
    last = None
    for _, u in owner:
        if u != last:
            runs[u] += 1
        last = u
    return {u: n for u, n in runs.items() if n > 1}


def out_of_place(funcs):
    """The functions (address, row) outside the longest run of the source order that is already in address order."""
    n = len(funcs)
    if n < 2:
        return []
    # longest increasing subsequence of the addresses, by patience sorting with back links
    tails, links, prev = [], [], [None] * n
    for i, (a, _) in enumerate(funcs):
        k = bisect.bisect_left(tails, a)
        if k == len(tails):
            tails.append(a)
            links.append(i)
        else:
            tails[k], links[k] = a, i
        prev[i] = links[k - 1] if k else None
    keep, i = set(), links[-1]
    while i is not None:
        keep.add(i)
        i = prev[i]
    return [f for i, f in enumerate(funcs) if i not in keep]


def status(r):
    return "MATCH" if r["status"] == "MATCH" else "%s %.1f" % (r["status"], r.get("score", 0))


def cmd_boundaries(rows, own, img, unit=None, out=sys.stdout):
    found = boundaries(rows, {u: f for u, f in own.items() if unit in (None, u)}, img)
    print("%d units whose own code a static run with <iostream> quads or a locale pair interrupts (candidates for "
          "two retail units: confirm by the .bss stores and the run's own quads)" % len({f[0] for f in found}),
          file=out)
    for u, ra, rb, stubs in sorted(found, key=lambda f: (not strong(f[3]), f[0])):
        kinds = defaultdict(int)
        for _, k, _ in stubs:
            kinds[k] += 1
        owners = sorted({o or "?" for _, _, o in stubs})
        print("  %-6s %-36s between 0x%X %s and 0x%X %s: %s at 0x%X-0x%X (annotated to %s)" % (
            "quads" if strong(stubs) else "locale", u, int(ra["addr"], 16), ra["name"], int(rb["addr"], 16), rb["name"],
            ", ".join("%d %s" % (n, k) for k, n in sorted(kinds.items())), stubs[0][0], stubs[-1][0],
            ", ".join(owners)), file=out)


def strong(stubs):
    """Whether a run has `<iostream>` quads of its own, not only a locale guard."""
    return any(k == "iostream" for _, k, _ in stubs)


def cmd_scattered(own, unit=None, out=sys.stdout):
    sel = {u: n for u, n in scattered(own).items() if unit in (None, u)}
    print("%d units whose own code other units' code splits into several runs (a misplaced function, or a .cpp that "
          "is several retail units)" % len(sel), file=out)
    for u, n in sorted(sel.items(), key=lambda kv: (-kv[1], kv[0])):
        print("  %-44s %d runs" % (u, n), file=out)


def is_open(r):
    return r["status"] not in ("MATCH", "PATCHED")


def cmd_order(own, unit=None, out=sys.stdout):
    sel = []
    for u, funcs in sorted(own.items()):
        if unit not in (None, u):
            continue
        funcs = [f for f in funcs if not generated(f[1])]
        bad = out_of_place(funcs)
        if bad:
            sel.append((u, bad, funcs))
    print("%d units define functions in an order other than retail's: %d functions out of place, %d of them open "
          "(deleting destructors left out; by open functions out of place)" % (
              len(sel), sum(len(b) for _, b, _ in sel), sum(1 for _, b, _ in sel for _, r in b if is_open(r))),
          file=out)
    for u, bad, funcs in sorted(sel, key=lambda t: (-sum(1 for _, r in t[1] if is_open(r)), -len(t[1]), t[0])):
        open_ = [(a, r) for a, r in bad if is_open(r)]
        print("  %-44s %d of %d out of place, %d open" % (u, len(bad), len(funcs), len(open_)), file=out)
        shown = open_ if unit is None else bad
        for a, r in shown[:8]:
            print("      0x%X %-50s %s" % (a, (r["name"] or "?")[:50], status(r)), file=out)
        if len(shown) > 8:
            print("      ... %d more" % (len(shown) - 8), file=out)


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("what", nargs="?", choices=("boundaries", "scattered", "order"),
                    help="one report (default: all three)")
    ap.add_argument("unit", nargs="?", help="one unit, e.g. game/ShipClass")
    args = ap.parse_args(argv)
    rows = decomp.load_report()["functions"]
    own = own_functions(rows)
    if args.what in (None, "boundaries"):
        cmd_boundaries(rows, own, decomp.pe.Image(decomp.target_image()), args.unit)
    if args.what in (None, "scattered"):
        if args.what is None:
            print()
        cmd_scattered(own, args.unit)
    if args.what in (None, "order"):
        if args.what is None:
            print()
        cmd_order(own, args.unit)
    return 0


if __name__ == "__main__":
    sys.exit(main())
