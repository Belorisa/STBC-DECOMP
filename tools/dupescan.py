"""Find unwritten or unmatched retail functions whose bytes equal an already-matched function's: free candidates.

    python tools/dupescan.py                       # every candidate, exact twins first
    python tools/dupescan.py --kind diff           # only annotated functions that don't match yet
    python tools/dupescan.py --kind gap --min-size 32
    python tools/dupescan.py --tier exact --json build/dupescan.json

A candidate is a retail function that no matched source covers yet: an annotated function whose status isn't MATCH
(`diff`), a function claimed in config/symbols.txt without source (`claimed`), or a function found in an unwritten gap
between annotated ones (`gap`, split at each return as `openwork.py gaps` scans them). Each candidate's retail body,
read from the image with its relocations masked, is set against every matched function's retail body:

- relocations are masked from the image's own base relocations (absolute addresses: globals, vtables, jump tables,
  `push offset`) and from the disassembly (the rel32 of a `call`, `jmp` or `jcc` that leaves the function); an
  address inside the function itself (a jump table, a local label) counts as its offset from the start, so two
  copies of a function at different addresses compare equal;
- `exact`: the masked bytes are equal and every relocation names the same target (the same callee, the same global):
  the candidate is the twin's code at another address (another unit's copy of an inline, a function the linker
  didn't fold), and the twin's source written for it should match as it stands;
- `shape`: the masked bytes are equal but some targets differ (a template instance calling its own type's helpers,
  a wrapper of another global): the twin's source is a template for it, with those referents changed.

Retail bodies are scanned as `openwork.py ready` scans them (to the first return followed by alignment padding,
bounded by the next annotated address), on both sides, so the extents agree for equal code. Bodies under --min-size
bytes (default 8: a `ret` or a one-load accessor matches hundreds of unrelated functions) are left out. Retail's
signatures are cached in build/dupescan_cache.pkl (retail never changes; delete it at will). Run `build` first: the
matched set is build/report.json's.

Method after bw1-decomp's tools/asm-dupe-scan.py (openblack's Black & White decompilation, CC0), which indexes a
split target's objects by their relocated bytes; this one reads the retail image directly. A report only: it
writes no source and no annotation.
"""
import argparse
import bisect
import json
import pickle
import re
import sys
from collections import defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import decomp  # noqa: E402
import openwork  # noqa: E402

CACHE = decomp.BUILD / "dupescan_cache.pkl"
PAD = (0x90, 0xCC)
KINDS = ("diff", "claimed", "gap")


def signature(code, addr, relocs, disasm):
    """(masked bytes, relocation targets) of a function body at addr.

    `relocs` is the image's base-relocation sites (absolute dwords), sorted; `disasm(code, addr)` yields (address,
    mnemonic, operands, size). A target inside the body becomes ("self", offset); an absolute one ("abs", address); a
    branch or call that leaves the body ("rel", address)."""
    end = addr + len(code)
    masked = bytearray(code)
    fields = []
    lo, hi = bisect.bisect_left(relocs, addr), bisect.bisect_right(relocs, end - 4)
    for site in relocs[lo:hi]:
        off = site - addr
        target = int.from_bytes(code[off:off + 4], "little")
        fields.append((off, ("self", target - addr) if addr <= target < end else ("abs", target)))
        masked[off:off + 4] = b"\0\0\0\0"
    for a, mnem, ops, size in disasm(code, addr):
        if not (mnem == "call" or mnem.startswith("j")) or not re.fullmatch(r"0x[0-9a-f]+", ops):
            continue
        target = int(ops, 16)
        if addr <= target < end:
            continue
        width = 4 if size >= 5 else 1
        off = a + size - width - addr
        if any(o <= off < o + 4 for o, _ in fields):
            continue
        fields.append((off, ("rel", target)))
        masked[off:off + width] = b"\0" * width
    fields.sort()
    return bytes(masked), tuple(t for _, t in fields)


def shape_of(targets):
    """The targets with every external address reduced to its kind: what `shape` twins share."""
    return tuple(t if t[0] == "self" else (t[0],) for t in targets)


def disasm_lite(code, addr):
    """(address, mnemonic, operands, size) rows, from capstone's light disassembler (no instruction objects)."""
    decomp.disasm(b"", addr)   # creates the disassembler
    return [(a, m, o, n) for a, n, m, o in decomp._cs.disasm_lite(bytes(code), addr)]


class Retail:
    """Retail bodies and signatures, cached by (address, stop) between runs."""

    def __init__(self, img, cache_path=CACHE):
        self.img, self.path = img, cache_path
        self.relocs = sorted(img.relocs)
        try:
            with open(cache_path, "rb") as f:
                self.cache = pickle.load(f)
        except (OSError, pickle.PickleError, EOFError, AttributeError):
            self.cache = {}
        self.dirty = False

    def body(self, addr, stop):
        """(masked bytes, targets) of the function at addr, scanned up to stop; None when it has no bytes."""
        key = (addr, stop)
        if key in self.cache:
            return self.cache[key]
        _, size = openwork.callees(self.img, addr, stop)
        sig = None
        if size:
            code = self.img.read(addr, size)
            if code:
                sig = signature(code, addr, self.relocs, disasm_lite)
        self.cache[key] = sig
        self.dirty = True
        return sig

    def save(self):
        if self.dirty:
            self.path.parent.mkdir(parents=True, exist_ok=True)
            with open(self.path, "wb") as f:
                pickle.dump(self.cache, f, pickle.HIGHEST_PROTOCOL)


def gap_functions(img, start, end):
    """The functions of a gap: from start, skipping padding, one body at a time to its return (openwork.callees)."""
    out, cursor = [], start
    while cursor < end:
        b = img.read(cursor, 1)
        if b is None:
            break
        if b[0] in PAD or b[0] == 0:
            cursor += 1
            continue
        _, size = openwork.callees(img, cursor, end)
        if size <= 0:
            break
        out.append((cursor, size))
        cursor += size
    return out


def collect(rows, claims, img):
    """(matched [(addr, stop, row)], candidates [(addr, stop, kind, name)]) over the report."""
    asm = openwork.apart(annotated={int(r["addr"], 16) for r in rows})
    starts = sorted({int(r["addr"], 16) for r in rows} | set(asm))
    text_end = img.text[1] + img.text[2]

    def stop_of(a):
        i = bisect.bisect_right(starts, a)
        return min(starts[i] if i < len(starts) else a + openwork.SCAN_CAP, a + openwork.SCAN_CAP)

    matched, cands = [], []
    for r in rows:
        a = int(r["addr"], 16)
        if r["status"] == "MATCH":
            if r.get("size"):
                matched.append((a, min(a + r["size"], stop_of(a)), r))
        elif r["status"] != "PATCHED":
            cands.append((a, stop_of(a), "diff", r.get("name") or r["addr"]))
    annotated = set(starts)
    claimed = {a: n for n, a, _ in claims if img.text[1] <= a < text_end and a not in annotated}
    seen = set()
    gaps, tails, _, _ = openwork.unwritten_gaps(rows)
    for lo, hi, _, _ in gaps + tails:
        for a, size in gap_functions(img, lo, hi):
            seen.add(a)
            cands.append((a, hi, "claimed" if a in claimed else "gap", claimed.get(a, "")))
    for a, n in claimed.items():
        if a not in seen:
            cands.append((a, stop_of(a), "claimed", n))
    return matched, cands


def scan(rows, claims, img, retail, min_size=8):
    """[(candidate (addr, kind, name, size), tier, [twin rows])], exact candidates first, then by size, largest
    first."""
    matched, cands = collect(rows, claims, img)
    exact, shape = defaultdict(list), defaultdict(list)
    sizes = set()
    cand_sigs = []
    for a, stop, kind, name in cands:
        sig = retail.body(a, stop)
        if sig is None or len(sig[0]) < min_size:
            continue
        cand_sigs.append(((a, kind, name, len(sig[0])), sig))
        sizes.add(len(sig[0]))
    for a, stop, r in matched:
        if stop - a < min_size or stop - a < min(sizes, default=0):
            continue
        sig = retail.body(a, stop)
        if sig is None or len(sig[0]) not in sizes:
            continue
        exact[sig].append(r)
        shape[(sig[0], shape_of(sig[1]))].append(r)
    out = []
    for cand, sig in cand_sigs:
        twins = [r for r in exact.get(sig, []) if int(r["addr"], 16) != cand[0]]
        if twins:
            out.append((cand, "exact", twins))
            continue
        twins = [r for r in shape.get((sig[0], shape_of(sig[1])), []) if int(r["addr"], 16) != cand[0]]
        if twins:
            out.append((cand, "shape", twins))
    out.sort(key=lambda x: (x[1] != "exact", KINDS.index(x[0][1]), -x[0][3], x[0][0]))
    return out


def twin_text(r):
    return "0x%s %s (%s)" % (r["addr"][2:].upper(), r.get("name") or "?", r.get("unit", "?"))


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("--kind", choices=KINDS, action="append", help="only these candidates (repeatable)")
    ap.add_argument("--tier", choices=("exact", "shape"), help="only this tier")
    ap.add_argument("--min-size", type=int, default=8, help="leave out bodies under N bytes (default 8)")
    ap.add_argument("--twins", type=int, default=3, help="twins listed per candidate (default 3)")
    ap.add_argument("--json", help="also write the candidates to this file")
    a = ap.parse_args(argv)
    rows, claims = openwork.load()
    img = decomp.pe.Image(decomp.target_image())
    retail = Retail(img)
    try:
        found = scan(rows, claims, img, retail, a.min_size)
    finally:
        retail.save()
    found = [f for f in found if (not a.kind or f[0][1] in a.kind) and (not a.tier or f[1] == a.tier)]
    counts = defaultdict(int)
    for cand, tier, _ in found:
        counts[(tier, cand[1])] += 1
    print("%d candidates (%s)" % (len(found), ", ".join("%s %s %d" % (t, k, n) for (t, k), n in sorted(counts.items()))))
    for (addr, kind, name, size), tier, twins in found:
        more = " (+%d more)" % (len(twins) - a.twins) if len(twins) > a.twins else ""
        print("%-5s %-7s 0x%X %5d  %s" % (tier, kind, addr, size, name))
        print("        twin %s%s" % ("; ".join(twin_text(r) for r in twins[:a.twins]), more))
    if a.json:
        Path(a.json).write_text(json.dumps([
            {"addr": "0x%X" % c[0], "kind": c[1], "name": c[2], "size": c[3], "tier": t,
             "twins": [{"addr": r["addr"], "name": r.get("name"), "unit": r.get("unit")} for r in tw]}
            for c, t, tw in found], indent=1), encoding="utf-8")
    return 0


if __name__ == "__main__":
    sys.exit(main())
