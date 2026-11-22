"""Functions declared `void` whose callers read EAX after the call in retail: a return type modelled wrong.

    python tools/eaxcheck.py                  # every finding, grouped by callee
    python tools/eaxcheck.py --callee 0x5A7550
    python tools/eaxcheck.py --json build/eaxcheck.json

A report only (after `decomp.py build`). The declared return type comes from the decorated name: the compiled symbol
of every annotated function (the report's), and the names config/symbols.txt claims. C names carry no type and are
not checked. The callers are every annotated function's retail body (its compiled size when exact, else retail's
scanned extent): after each direct `call` to a callee declared void, the instructions that follow in the same block
are read up to the first branch, return or call. EAX is caller-saved, so VC6 never reads it after a call without
writing it first: a read there (`test eax, eax`, `mov ecx, eax`, `push eax`, `cmp al, 1`) means the callee returns a
value in EAX, or the caller calls another function than the one claimed. A write (`mov eax, ...`, `xor eax, eax`,
`pop eax`, `or eax, -1`, `sbb eax, eax`, `fnstsw ax`, or any write to AL/AX, which ends the check) clears it.

Not seen: virtual calls (no callee address), tail calls, and callers with no annotation. The reverse mistake (a
value-returning declaration nobody reads) is not a defect and is not reported.
"""

import argparse
import json
import re
import sys
from collections import defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import decomp  # noqa: E402
import layoutcheck  # noqa: E402
import openwork  # noqa: E402

EAX = ("eax", "ax", "al", "ah")
REG = re.compile(r"\b(eax|ax|al|ah)\b")
PURE_WRITES = ("mov", "movzx", "movsx", "lea", "pop", "setz", "sete", "setnz", "setne", "setl", "setle", "setg",
               "setge", "setb", "setbe", "seta", "setae", "sets", "setns", "setp", "setnp", "seto", "setno")
LOOKAHEAD = 12


def void_callees(rows, claims):
    """{address: (decorated name, display name)} of the functions whose decorated name declares a void return."""
    out = {}
    named = [(int(r["addr"], 16), r.get("symbol"), r.get("name")) for r in rows if r.get("symbol")]
    named += [(a, n, n) for n, a, _ in claims]
    for addr, sym, name in named:
        if not sym or not sym.startswith("?") or sym.startswith("??0") or sym.startswith("??1"):
            continue
        sig = layoutcheck.signature_of(sym)
        if sig and sig[1] and sig[1][0] == "void":
            out.setdefault(addr, (sym, name or sym))
    return out


def eax_use(insn):
    """'read', 'write' or None: what one instruction (mnemonic, operands) does to EAX, reading before writing."""
    mnem, ops = insn
    parts = [p.strip() for p in ops.split(",")] if ops else []
    if not any(REG.search(p) for p in parts):
        return None
    dest = parts[0] if parts else ""
    dest_is_eax = dest in EAX
    srcs = parts[1:]
    if mnem in ("xor", "sub") and len(parts) == 2 and parts[0] == parts[1] and dest_is_eax:
        return "write"   # xor eax, eax: a zero idiom, no read
    if mnem in PURE_WRITES and dest_is_eax and not any(REG.search(s) for s in srcs):
        return "write"
    if mnem in ("fnstsw", "fstsw", "lahf") or (mnem == "pop" and dest_is_eax):
        return "write"   # the FPU status word into AX, the flags into AH
    if mnem == "sbb" and len(parts) == 2 and parts[0] == parts[1] and dest_is_eax:
        return "write"   # sbb eax, eax: 0 or -1 from the carry
    if mnem == "or" and dest_is_eax and srcs and srcs[0] in ("0xffffffff", "-1", "0xffff", "0xff"):
        return "write"   # or eax, -1: the short form of mov eax, -1
    if mnem == "and" and dest_is_eax and srcs and srcs[0] == "0":
        return "write"
    return "read"


def scan_after_call(insns, i):
    """The first instruction after the call at insns[i] that reads EAX before any write, or None."""
    for j in range(i + 1, min(len(insns), i + 1 + LOOKAHEAD)):
        addr, mnem, ops = insns[j]
        if mnem in ("ret", "retn", "call", "jmp") or mnem.startswith("j") or mnem == "int3":
            return None
        use = eax_use((mnem, ops))
        if use == "write":
            return None
        if use == "read":
            return insns[j]
    return None


def findings(rows, claims, img):
    voids = void_callees(rows, claims)
    extents = openwork.retail_extents(rows, img)
    out = defaultdict(list)
    for r in rows:
        if r["status"] in ("NO-SYMBOL", "NO-OBJ"):
            continue
        a = int(r["addr"], 16)
        size = r.get("size") if r["status"] in openwork.EXACT else extents.get(a)
        if not size:
            continue
        code = img.read(a, size)
        if not code:
            continue
        insns = decomp.disasm(code, a)
        for i, (addr, mnem, ops) in enumerate(insns):
            if mnem != "call" or not re.fullmatch(r"0x[0-9a-f]+", ops):
                continue
            target = int(ops, 16)
            if target not in voids:
                continue
            hit = scan_after_call(insns, i)
            if hit:
                out[target].append({"caller": r["addr"], "caller_name": r.get("name"), "caller_status": r["status"],
                                    "call": "0x%X" % addr, "read": "0x%X  %s %s" % hit})
    return voids, out


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--callee", help="only this callee's address")
    ap.add_argument("--json", help="write the findings to this file")
    args = ap.parse_args()
    rows, claims = openwork.load()
    img = decomp.pe.Image(decomp.target_image())
    voids, found = findings(rows, claims, img)
    if args.callee:
        found = {a: v for a, v in found.items() if a == int(args.callee, 16)}
    if args.json:
        Path(args.json).write_text(json.dumps({"0x%X" % a: {"callee": voids[a][1], "symbol": voids[a][0],
                                                             "sites": v} for a, v in found.items()}, indent=1),
                                   encoding="utf-8")
    for a in sorted(found, key=lambda a: (-len(found[a]), a)):
        sym, name = voids[a]
        print("0x%X  %s  (%s): %d call site(s) read EAX" % (a, name, sym, len(found[a])))
        for s in found[a][:6]:
            print("    %s in %s %s [%s]: %s" % (s["call"], s["caller"], s["caller_name"], s["caller_status"], s["read"]))
    print("eaxcheck: %d callees declared void, %d with a caller reading EAX after the call, %d sites"
          % (len(voids), len(found), sum(len(v) for v in found.values())))


if __name__ == "__main__":
    main()
