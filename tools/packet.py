"""Everything a worker needs to start on one function, in one compact file: the work packet.

    python tools/packet.py 0x6C4D90                 # to stdout
    python tools/packet.py 0x6C4D90 0x72A080 -o build/packets   # one file per function, <addr>.md
    python tools/packet.py 0x6C4D90 --no-ghidra     # without Ghidra's decompilation (Ghidra not running)

For an annotated function: its source, PARKED note, score history and card (tools/cards.py), the diff cut to the rows that differ with
what kind of difference comes first (tools/fncontext.py's diagnosis), for a register wall up to
0x600 bytes the frame map (tools/framemap.py; `--no-framemap` leaves it out), the closest documented
levers from docs/levers/, a matched function of the same unit to copy, the class declaration, the callees by name,
and Ghidra's decompilation. For a function claimed in config/symbols.txt with no source yet: its declaration, the
class, the retail assembly with names, Ghidra's decompilation, the callees, and a matched function of the class.
For an address with neither (a gap): its extent (Ghidra's, else a scan to the first return before the next known
address), the retail assembly with names, its callers and stored pointers, its callees, the annotated neighbours and
their units, and Ghidra's decompilation. Lever entries ending in an `{asm: ...}` key whose pattern occurs in the diff
are quoted before the ones closest by text. All three also list the matched functions whose retail code looks most like this one, with the source of the closest two
(tools/similar.py).

A worker reads the packet instead of exploring: the file, the header, the diff, the levers and Ghidra are each one
command, and the packet runs them all. Run `decomp.py build` first (the report drives scores and names). An
annotated function's unit is recompiled for its diff; nothing in src/ is changed. Several addresses in one call
share one load of the units: a lane's packets are written in one go, at about a second each.
"""

import argparse
import bisect
import json
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
sys.path.insert(0, str(Path(__file__).resolve().parent / "ghidra"))
import decomp  # noqa: E402
import funcsource as L  # noqa: E402
import fncontext as F  # noqa: E402
import openwork  # noqa: E402
import ghidra_http  # noqa: E402
import similar  # noqa: E402
import cards  # noqa: E402
import framemap  # noqa: E402

FRAMEMAP_BYTES = 0x600   # a register wall's frame map is included up to this size (the `large` bound)
DECL_CHARS = 5000    # a class declaration longer than this is cut (the members near the top are the base's)
GHIDRA_CHARS = 6000


def names_by_address(rows, claims):
    names = {a: n for n, a, _ in claims}
    names.update({int(r["addr"], 16): r["name"] for r in rows if r["name"]})
    names.update(openwork.INLINE_COPIES)
    return names


def callee_lines(img, addr, rows, claims, names):
    """The function's direct callees, each with its name and state: what it calls is what it must call."""
    known = sorted({int(r["addr"], 16) for r in rows} | {a for _, a, _ in claims})
    i = bisect.bisect_right(known, addr)
    stop = known[i] if i < len(known) else addr + openwork.SCAN_CAP
    targets, size = openwork.callees(img, addr, stop)
    state = {int(r["addr"], 16): openwork.status_of(r) for r in rows}
    out = []
    for t in sorted(targets):
        if t in names:
            out.append("- 0x%X %s (%s)" % (t, names[t], state.get(t, "claimed, no source")))
        elif img.section_of(t) == img.section_of(addr):
            out.append("- 0x%X UNIDENTIFIED: no annotation or symbols.txt identity" % t)
        else:
            out.append("- 0x%X (outside .text: an import)" % t)
    return out, size


def history(addr):
    """The baseline's scores: a historical best above the current source's best is a lost match to recover."""
    try:
        b = json.loads((decomp.CONFIG / "baseline.json").read_text(encoding="utf-8")).get("0x%X" % addr)
    except (OSError, ValueError):
        return None
    if not b:
        return None
    line = "baseline: score %.1f, best for this source %.1f, historical best %.1f" % (b["score"], b["best"], b["hist"])
    if b["hist"] > b["best"]:
        line += ("  -- LOST MATCH: an earlier source scored higher; recover it from git history "
                 "(git log -L on the function) before inventing anything")
    return line


def card_part(addr):
    """The function's card (cards/<addr>.jsonl): every merge that moved its score and every recorded try."""
    head = cards.summary(addr)
    if not head:
        return []
    out = ["", "## Card: what earlier tries did (do not repeat a try that failed)", head, "```"] + \
        cards.lines(addr) + ["```"]
    import openwork
    hdr = openwork.header_lines(cards.key(addr))
    if hdr:
        out += ["", "## Headers this card names: what other cards record (`openwork.py headers NAME` for the entries)",
                "```"] + hdr + ["```"]
    return out


def frame_part(addr, row, args):
    """The frame map (tools/framemap.py) of a register wall: retail's and our frame slots and callee-saved registers,
    ours named. Behind a size limit, since it compiles the unit a second time with /Z7."""
    if not getattr(args, "framemap", True) or row.get("wall") != "register" or row.get("size", 0) > FRAMEMAP_BYTES:
        return []
    try:
        with L.CheckoutLock():
            text = framemap.report("0x%X" % addr)
    except (framemap.FramemapError, decomp.DiffError) as e:
        text = "framemap failed: %s" % str(e)[:300]
    return ["## Frame map (tools/framemap.py: retail's slots and registers beside ours, ours named by a /Z7 copy)",
            "```", text, "```", ""]


def class_part(cls):
    path, decl = F.class_declaration(cls) if cls else (None, None)
    if not decl:
        return []
    if len(decl) > DECL_CHARS:
        decl = decl[:DECL_CHARS] + "\n\t// ... (cut; read %s for the rest)" % path.relative_to(L.ROOT).as_posix()
    return ["## Class declaration (%s)" % path.relative_to(L.ROOT).as_posix(), "```cpp", decl, "```"]


def ghidra_part(addr, use):
    if not use:
        return []
    code = ghidra_http.decompile(addr)
    if not code:
        return ["## Ghidra", "(Ghidra's server is not running, or has no function here)"]
    if len(code) > GHIDRA_CHARS:
        code = code[:GHIDRA_CHARS] + "\n/* ... cut */"
    return ["## Ghidra's decompilation (evidence, never source: its locals, casts and control flow are Ghidra's)",
            "```c", code, "```"]


def annotated_packet(addr, row, rows, claims, names, img, args):
    path, ann = L.find_annotation(addr)
    rel = path.relative_to(L.ROOT).as_posix()
    lines = path.read_text(encoding="utf-8", errors="replace").splitlines(keepends=True)
    start, end = L.function_span(lines, ann)
    source = "".join(lines[start:end]).replace("\r\n", "\n")
    unit = row["unit"]
    cls = (row["name"] or "").split("::")[0] if "::" in (row["name"] or "") else None
    note = row.get("parked")
    try:
        raw = args.session.text("0x%X" % addr)
    except decomp.DiffError as e:
        raw = str(e)
    diff, diagnosis, _ = F.best_view(addr, 120, raw=raw)
    if row.get("wall_note"):
        diagnosis = "%sWall class by the diff (wall-identifier's routing order: inline, control-flow, referent, " \
                    "register): **%s**, %s." % (diagnosis + "\n" if diagnosis else "", row["wall"], row["wall_note"])
    out = ["# 0x%X %s" % (addr, row["name"]),
           "%s:%d, unit %s; %s %.1f" % (rel, ann + 1, unit, row["status"], row["score"]),
           "compiler: %s" % F.compiler_description(unit, path)]
    h = history(addr)
    if h:
        out.append(h)
    if note:
        out.append("PARKED: %s" % note)
    if row.get("draft"):
        out.append("DRAFT: %s" % row["draft"])
    out += card_part(addr)
    out += ["", "## Source", "```cpp", source.rstrip(), "```", ""]
    if diagnosis:
        out += ["## What the diff shows", diagnosis, ""]
    out += ["## Diff (ours left, retail right; '~' a branch that differs only by its target)", "```", diff, "```", ""]
    out += frame_part(addr, row, args)
    levers = with_keyed_levers(raw, F.documented_levers(note, diff, args.lever_chars), args.lever_chars)
    if levers:
        out += ["## Closest documented levers (docs/levers/)", levers, ""]
    calls, _ = callee_lines(img, addr, rows, claims, names)
    if calls:
        out += ["## Callees (retail)"] + calls + [""]
    name, example = F.pick_example(rows, unit, addr, source)
    if example:
        out += ["## A matched function of the same unit (%s): copy its habits" % name, "```cpp", example.rstrip(), "```", ""]
    out += similar.section(addr, img=img, rows=rows, claims=claims)
    out += class_part(cls)
    out += [""] + ghidra_part(addr, args.ghidra)
    return "\n".join(out)


# ---------------------------------------------------------------------------
# lever keys: a lever entry may end in `{asm: test; je; jmp top}`, an instruction pattern its wall shows

ASM_KEY = re.compile(r"\{asm:\s*([^}]*)\}\s*$")
X86_OPERAND_WORD = re.compile(r"^(?:e?[abcd]x|[abcd][lh]|e?[sd]i|e?[sb]p|st\(\d\)|st|-?0x[0-9a-f]+|-?\d+|byte|word|"
                              r"dword|qword|tbyte|ptr)$")
NUMBER = re.compile(r"^-?(?:0x[0-9a-f]+|\d+)$")
OPERAND_NUMBER = re.compile(r"(?<![\w.])-?(?:0x[0-9a-f]+|\d+)(?![\w(])")


def operand_text(text):
    """Operands as the key reads them: lower case, a sign joined to its number (`[esp - 4]` -> `[esp -4]`), x87
    registers spelled `st(N)` (`st1` -> `st(1)`)."""
    text = re.sub(r"-\s+(?=\w)", "-", text.lower())
    return re.sub(r"\bst(\d)\b", r"st(\1)", text)


def number_value(word):
    """A number's 32-bit value: `-1` and `0xffffffff` are one immediate, as the disassembler prints either."""
    sign, digits = (-1, word[1:]) if word.startswith("-") else (1, word)
    return sign * int(digits, 16 if digits.startswith("0x") else 10) & 0xFFFFFFFF


def asm_key(entry):
    """The entry's instruction pattern: [(mnemonic, [operand words])], or None. Elements are separated by `;`; the
    first word of an element is the mnemonic, and of its other words only registers (x87's `st(N)` too), numbers
    (signed) and size words are compared (a label like `top` stands for any operand)."""
    m = ASM_KEY.search(entry)
    if not m:
        return None
    out = []
    for element in m.group(1).split(";"):
        words = re.findall(r"-?[\w()]+", operand_text(element))
        if words:
            out.append((words[0], [w for w in words[1:] if X86_OPERAND_WORD.match(w)]))
    return out or None


def word_in(word, ops):
    """True when an operand word of a key occurs in an instruction's operands: a number by its 32-bit value (so `1`
    is not `-1`, and `-1` is `0xffffffff`), anything else as a whole word."""
    ops = operand_text(ops)
    if NUMBER.match(word):
        value = number_value(word)
        return any(number_value(n) == value for n in OPERAND_NUMBER.findall(ops))
    return re.search(r"(?<!\w)%s(?![\w(])" % re.escape(word), ops) is not None


def diff_sides(raw):
    """The side-by-side diff of `decomp.py diff` as two instruction lists [(mnemonic, operands, differs)]: ours, retail."""
    ours, retail = [], []
    for line in (raw or "").splitlines()[1:]:
        if len(line) < 62 or line[61] not in " |<>":
            continue
        differs = line[61] != " "
        for side, text in ((ours, line[:60]), (retail, line[63:])):
            m = re.match(r"\s*[0-9A-F]{8}\s+(\S+)\s*(.*)", text)
            if m:
                side.append((m.group(1).lower(), m.group(2).lower().strip(), differs))
    return ours, retail


def pattern_count(pattern, ins):
    """(occurrences, occurrences on a differing row): how often the pattern's elements match consecutive
    instructions, and how often one of the matched instructions is on a row that differs."""
    n = len(pattern)
    total = differing = 0
    for k in range(len(ins) - n + 1):
        window = ins[k:k + n]
        if all(m == mnem and all(word_in(w, ops) for w in words)
               for (m, words), (mnem, ops, _) in zip(pattern, window)):
            total += 1
            differing += any(d for _, _, d in window)
    return total, differing


def pattern_in(pattern, ins):
    """True when the pattern's elements match consecutive instructions, one of them on a row that differs."""
    return pattern_count(pattern, ins)[1] > 0


def keyed_levers(raw, entries=None):
    """The lever entries whose `{asm: ...}` pattern occurs in the function's diff on a differing row, in file order,
    and occurs a different number of times on the two sides: a pattern both sides have as often (an `fld; fstp`
    copy shifted by the residue) doesn't tell them apart."""
    ours, retail = diff_sides(raw)
    out = []
    for _, text in (entries if entries is not None else F.lever_entries()):
        pattern = asm_key(text)
        if not pattern:
            continue
        (n_ours, d_ours), (n_retail, d_retail) = pattern_count(pattern, ours), pattern_count(pattern, retail)
        if (d_ours or d_retail) and n_ours != n_retail:
            out.append(text)
    return out


def with_keyed_levers(raw, quoted, limit_chars, entries=None):
    """The levers section: the entries whose asm key occurs in the diff first, then the text-matched quotes not
    already among them, within limit_chars."""
    keyed = keyed_levers(raw, entries)
    out, used = [], 0
    for text in keyed:
        if used + len(text) > limit_chars and out:
            break
        out.append("- (asm key in this diff) " + text)
        used += len(text)
    for line in (quoted or "").splitlines():
        if line.startswith("- ") and line[2:] in keyed:
            continue
        if used + len(line) > limit_chars and out:
            break
        out.append(line)
        used += len(line)
    return "\n".join(out) or None


def inline_definition(decl, method):
    """The first line of the method's inline body in a class declaration, or None."""
    m = re.search(r"^[^\n;{}()=]*\b%s\s*\([^;{\n]*\)[^;{\n]*\{[^\n]*" % re.escape(method), decl, re.M)
    return m.group(0).strip() if m else None


def units_defining(decorated, limit=6):
    """Compiled units whose object holds the decorated name (as a COMDAT copy of a header inline), from build/obj."""
    needle = decorated.encode()
    out = []
    for obj in sorted((L.ROOT / "build" / "obj").rglob("*.obj")):
        try:
            if needle in obj.read_bytes():
                out.append(obj.relative_to(L.ROOT / "build" / "obj").with_suffix("").as_posix())
        except OSError:
            continue
        if len(out) >= limit:
            out.append("...")
            break
    return out


def stack_note(img, addr, size, decorated):
    """What retail's returns say about the signature: a __thiscall or __stdcall function pops its arguments (ret N),
    so N is the bytes they take: a point passed by value takes 12, by pointer or reference 4. The declaration in a
    header, and the decorated name of a claim, are guesses until N agrees with them; the return type shows in how
    callers use the result (a `test al, al; setnz` on it means a char or bool in al, not an int)."""
    pops = sorted({int(ops, 16) if ops else 0 for a, m, ops in decomp.disasm(img.read(addr, size), addr)
                   if m in ("ret", "retn") and (not ops or ops.startswith("0x") or ops.isdigit())})
    cdecl = "@@Y" in decorated and "A" == decorated.split("@@Y", 1)[1][:1]
    if not pops:
        return "Stack: no return found in the scanned body (a tail jump): read the arguments from the callers."
    what = ("pops %s bytes of arguments" % "/".join(str(p) for p in pops)) if any(pops) else "pops no arguments"
    return ("Stack: retail %s (`ret`%s). Check the declaration's parameters against it (by value, a NiPoint3 takes "
            "12 bytes, a pointer or reference 4)%s; a header declaration or a claim's decorated name can be wrong."
            % (what, "" if not any(pops) else " " + "/".join(hex(p) for p in pops),
               "; a __cdecl function pops nothing whatever its arguments" if cdecl else ""))


CALL = bytes([0xE8])   # call rel32


def retail_callers(img, addr, rows, limit=6):
    """Retail functions that call addr directly (E8 rel32), each with its annotated owner or UNIDENTIFIED."""
    _, va, size, off, _ = img.sections[0]
    text = img.data[off:off + size]
    spans = sorted(((int(r["addr"], 16), int(r["addr"], 16) + max(r.get("size", 0), 1), r) for r in rows),
                   key=lambda s: s[:2])
    starts = [s[0] for s in spans]
    found, i = [], text.find(CALL)
    while i != -1 and len(found) < limit:
        if i + 5 <= len(text) and va + i + 5 + int.from_bytes(text[i + 1:i + 5], "little", signed=True) == addr:
            site = va + i
            k = bisect.bisect_right(starts, site) - 1
            if k >= 0 and spans[k][0] <= site < spans[k][1]:
                r = spans[k][2]
                found.append("0x%X in %s (%s, unit %s)" % (site, r["name"], r["status"], r["unit"]))
            else:
                found.append("0x%X in UNIDENTIFIED code" % site)
        i = text.find(CALL, i + 1)
    return found


def claimed_packet(addr, decorated, evidence, rows, claims, names, img, args):
    cls, method = F.method_of_decorated(decorated)
    calls, size = callee_lines(img, addr, rows, claims, names)
    out = ["# 0x%X %s::%s (claimed in config/symbols.txt, no source yet)" % (addr, cls, method),
           "decorated: %s" % decorated, "evidence: %s" % (evidence or "-"),
           "size: %d bytes; home unit: %s" % (size, F.home_unit(cls, rows) or "none yet"), ""]
    header, decl = F.class_declaration(cls) if cls else (None, None)
    m = F.declaration(decl, method) if decl and method else None
    inline = inline_definition(decl, method) if decl and method else None
    if inline:
        units = units_defining(decorated)
        if not units:
            callers = retail_callers(img, addr, rows)
            out += ["No compiled unit emits this inline yet: a unit emits the out-of-line copy only when its source "
                    "calls it. Retail's callers: %s. Annotate it `@<unit>` of an identified caller's unit; if every "
                    "caller is unidentified, it is blocked on them." % ("; ".join(callers) or "none found (a vtable "
                    "or pointer use: look for its address in .rdata)"), ""]
        out += ["**Defined inline in %s**: `%s`. The body exists: annotate it there, on the line directly above its "
                "definition, with the decorated name (`// FUNCTION: STBC 0x%X %s`). Retail kept one unit's copy, and "
                "the match shows under that unit, not the home unit%s." % (
                    header.relative_to(L.ROOT).as_posix(), inline, addr, decorated,
                    ": units whose objects define it: " + ", ".join(units) if units else
                    " (`python tools/comdat.py <unit>` finds it)"), ""]
    elif m:
        out += ["declared as: `%s`" % m.group(0).strip(), ""]
    elif decl:
        out += ["(the class declares no such method yet: add it, with the signature the decorated name gives)", ""]
    out += [stack_note(img, addr, size, decorated), ""]
    callers = retail_callers(img, addr, rows)
    out += ["Retail callers: %s. A header or .inl inline gets its out-of-line copy only from a unit whose source "
            "calls it: annotate it `@<unit>` of an identified caller's unit; when every caller is UNIDENTIFIED, it "
            "waits for them." % ("; ".join(callers) or "none by direct call (a vtable or a function pointer: look "
                                  "for the address in .rdata)"), ""]
    out += ["When it matches, remove its claim from config/symbols.txt in the same commit (not before: callers' "
            "referents resolve through the claim until the annotation exists).", ""]
    if calls:
        out += ["## Callees (retail)"] + calls + [""]
    out += ["## Retail assembly", "```", F.retail_asm(img, addr, size, names), "```", ""]
    unit = F.home_unit(cls, rows)
    if unit:
        name, example = F.example_function(cls, unit, rows)
        if example:
            out += ["## A matched function of the class (%s): copy its habits" % name, "```cpp", example.rstrip(), "```", ""]
    out += similar.section(addr, img=img, rows=rows, claims=claims)
    out += class_part(cls)
    out += [""] + ghidra_part(addr, args.ghidra)
    return "\n".join(out)


GAP_ASM_CAP = 0x1800   # bytes of a gap function's disassembly printed; the rest is cut with a note


def ghidra_extent(addr, use=True):
    """The end of Ghidra's function at addr (its instructions' last byte + 1), or None when Ghidra's server is down or
    has no function starting there."""
    if not use:
        return None
    import urllib.request
    try:
        with urllib.request.urlopen("%s/disassemble_function?address=0x%X" % (ghidra_http.SERVER, addr),
                                    timeout=30) as r:
            data = json.loads(r.read())
    except Exception:
        return None
    items = (data.get("data") or {}).get("items") or [] if data.get("success") else []
    ends = [int(i["address"], 16) + len(i.get("bytes") or "") // 2 for i in items if i.get("address")]
    if not ends or min(int(i["address"], 16) for i in items) != addr:
        return None
    return max(ends)


def neighbours(addr, rows):
    """(the annotated row before addr, the one after), by address; either may be None."""
    spans = sorted(rows, key=lambda r: int(r["addr"], 16))
    starts = [int(r["addr"], 16) for r in spans]
    k = bisect.bisect_right(starts, addr)
    return (spans[k - 1] if k else None), (spans[k] if k < len(spans) else None)


def gap_extent(img, addr, rows, claims, use_ghidra=True):
    """(end, how): Ghidra's extent when it has a function at addr, else the scan to the first return followed by
    padding, bounded by the next known (annotated or claimed) address."""
    known = sorted({int(r["addr"], 16) for r in rows} | {a for _, a, _ in claims})
    i = bisect.bisect_right(known, addr)
    stop = known[i] if i < len(known) else addr + openwork.SCAN_CAP
    end = ghidra_extent(addr, use_ghidra)
    if end and end <= stop:
        return end, "Ghidra's function"
    _, size = openwork.callees(img, addr, stop)
    return addr + max(size, 1), "scan to the first return before the next known address 0x%X" % stop


def pointer_refs(img, addr, limit=6):
    """Addresses outside .text that hold addr as a dword: vtable slots and function-pointer tables."""
    needle = addr.to_bytes(4, "little")
    out = []
    for name, va, size, off, _ in img.sections[1:]:
        data = img.data[off:off + size]
        i = data.find(needle)
        while i != -1 and len(out) < limit:
            out.append("0x%X (%s)" % (va + i, name))
            i = data.find(needle, i + 1)
    return out


def describe_neighbour(r, addr):
    if r is None:
        return "none"
    a = int(r["addr"], 16)
    return "0x%X %s (%s, unit %s; %s)" % (a, r["name"], r["status"], r["unit"],
                                          "0x%X bytes before" % (addr - a) if a < addr else "0x%X bytes after" % (a - addr))


def gap_packet(addr, rows, claims, names, img, args):
    """The packet of an address with no annotation and no claim: retail's disassembly with names, its callers and
    callees, the annotated functions around it and their units (where it likely belongs), and Ghidra's
    decompilation."""
    end, how = gap_extent(img, addr, rows, claims, args.ghidra)
    size = end - addr
    before, after = neighbours(addr, rows)
    inside = before is not None and int(before["addr"], 16) + before.get("size", 0) > addr
    out = ["# 0x%X (unwritten: no annotation, no claim in config/symbols.txt)" % addr,
           "extent: 0x%X-0x%X, %d bytes (%s)" % (addr, end, size, how)] + card_part(addr) + [""]
    if inside:
        out += ["**Inside the annotated function %s** (0x%s + 0x%X bytes): not a function start." % (
            before["name"], before["addr"][2:], before["size"]), ""]
    out += ["## Neighbours (the unit a function sits between is the likely home; the static layout can say otherwise)",
            "- before: " + describe_neighbour(before, addr), "- after: " + describe_neighbour(after, addr), ""]
    callers = retail_callers(img, addr, rows)
    refs = pointer_refs(img, addr)
    out += ["## Callers (retail)"] + ["- " + c for c in callers] + ["- pointer at " + r for r in refs]
    if not callers and not refs:
        out.append("- none by direct call or stored pointer")
    out.append("")
    calls, _ = callee_lines(img, addr, rows, claims, names)
    if calls:
        out += ["## Callees (retail)"] + calls + [""]
    out += [stack_note(img, addr, size, ""), ""]
    asm = F.retail_asm(img, addr, min(size, GAP_ASM_CAP), names)
    if size > GAP_ASM_CAP:
        asm += "\n; ... cut at 0x%X bytes of 0x%X" % (GAP_ASM_CAP, size)
    out += ["## Retail assembly", "```", asm, "```", ""]
    out += similar.section(addr, img=img, rows=rows, claims=claims)
    out += [""] + ghidra_part(addr, args.ghidra)
    return "\n".join(out)


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("functions", nargs="+", help="retail addresses, e.g. 0x6C4D90")
    ap.add_argument("-o", "--out", help="write <addr>.md files into this directory instead of printing")
    ap.add_argument("--no-ghidra", dest="ghidra", action="store_false")
    ap.add_argument("--lever-chars", type=int, default=1800)
    ap.add_argument("--no-framemap", dest="framemap", action="store_false",
                    help="leave out a register wall's frame map")
    args = ap.parse_args()
    try:
        sys.stdout.reconfigure(encoding="utf-8")
    except AttributeError:
        pass
    rows, claims = openwork.load()
    names = names_by_address(rows, claims)
    img = decomp.pe.Image(decomp.target_image())
    by_addr = {int(r["addr"], 16): r for r in rows}
    claimed = {a: (n, ev) for n, a, ev in claims}
    # one load of the units for every annotated function's diff (their units are recompiled first)
    annotated = ["0x%X" % int(t, 16) for t in args.functions if int(t, 16) in by_addr]
    if annotated:
        with L.CheckoutLock():
            args.session = decomp.DiffSession(annotated)
    for text in args.functions:
        addr = int(text, 16)
        if addr in by_addr:
            packet = annotated_packet(addr, by_addr[addr], rows, claims, names, img, args)
        elif addr in claimed:
            packet = claimed_packet(addr, claimed[addr][0], claimed[addr][1], rows, claims, names, img, args)
        else:
            packet = gap_packet(addr, rows, claims, names, img, args)
        if args.out:
            d = Path(args.out)
            d.mkdir(parents=True, exist_ok=True)
            (d / ("%08X.md" % addr)).write_text(packet + "\n", encoding="utf-8")
            print("%s" % (d / ("%08X.md" % addr)))
        else:
            print(packet + "\n")
    return 0


if __name__ == "__main__":
    sys.exit(main())
