"""Check every virtual our class headers declare against retail's vtables, slot by slot.

    python tools/vtslots.py                        # every class whose retail vtable is known: findings first
    python tools/vtslots.py --class ShipClass      # one class, every slot
    python tools/vtslots.py --all                  # every class, every slot (agreeing slots too)
    python tools/vtslots.py --json build/vtslots.json
    python tools/vtslots.py --no-probe             # CodeView only (no probe compiles)

layoutcheck compares vtable LENGTHS only, so a virtual declared in the wrong slot (or two swapped) is invisible to
the byte gate whenever no matched function calls it. This tool reads which slot our compiler gives each virtual, and
compares the slot's retail target with the address our annotations give the method that should sit there.

Our slots, two readings of the same compiler:

- **CodeView** (every class): the /Z7 objects layoutcheck builds into build/types/ (tools/ghidra/export_types.py's
  build) record, per class, every method with its access and virtual mode, and each introducing virtual with its
  vtable offset. A class's slots are its primary base's (the base at offset 0, recursively) with its own introducing
  virtuals added at their offsets and its overrides (same name and parameter list; a destructor overrides the
  base's destructor) replacing the base's entry. Each class is read in the unit chosen to probe it (below); a class
  whose slots differ between two units that define it is reported as `inconsistent` (one class, two definitions).
- **vcall thunks** (the probe; `--no-probe` leaves it out): for each class, one probe source per owning unit
  `#include`s the unit's own .cpp (so the class is seen with that unit's flags, defines and include path), with
  `private` and `protected` defined as `public`, and takes `&Class::Method` of every virtual of the class's
  slot map whose name is not overloaded in the chain, passed to an undefined `template<class T> void
  vtslots_take(T)`. VC6 emits the address of a virtual as the vcall thunk `??_9@$B<offset>AE` (thiscall: `mov eax,
  [ecx]; jmp [eax+offset]`), the offset in VC6's number encoding (`3` is 4: a digit d is d+1; `CE@` is 0x24: hex
  digits A-P, `@`-terminated; `A@` is 0), and the instantiated `vtslots_take`'s decorated name spells the member
  pointer's type `P8<owner>@@<this cv>E<signature>`: the class the name was found in, which is the final
  overrider's, and the exact decorated name of the method there (`?<name>@<owner>@@{E,M,U}<this cv>E<signature>`).
  Each probe's slot is set against the CodeView slot map (a disagreement is a tool error, printed), and its
  decorated name finds the method's annotation without guessing between overloads. A probe that doesn't compile
  (a name declared private before any access label, a hidden or ambiguous name) is dropped by its error line and
  the class falls back to CodeView for that slot. Results are cached in build/vtslots/ by the probe text and the
  unit's /Z7 object.

Retail: each class's vtable address comes from layoutcheck's discovery (a relocation to `??_7Class@@6B@` in a
matched function, config/vtables.txt) and the `??_7` claims of config/symbols.txt; its length is layoutcheck's
(relocated dwords into .text up to the next referenced address). Slot N's target is read from the image and named by
the annotations (build/report.json) and claims (config/symbols.txt) at that address.

Verdicts per slot (strongest first):

- `moved`: our method is annotated at an address retail holds in another slot of the same vtable (wrong slot, or
  two swapped).
- `wrong`: our method is annotated at an address retail doesn't hold anywhere in this vtable (a wrong override, or
  a wrong slot whose real occupant has no annotation).
- `name`: our method has no annotation; retail's target is a method of a class in this one's chain (or of the class
  itself) under another name.
- `override`: ours declares an override that retail's class doesn't have, or lacks one retail's has: read against the
  primary base's own retail vtable (retail's slot differs from the base's exactly when retail's class overrides),
  or retail's target is the same method name of another class of the chain. Identical bodies are flagged: another
  kept copy of a base's inline looks like an override.
- `pure`: ours is pure virtual and retail's slot holds a function, or retail's holds `_purecall` and ours isn't pure.
- `retail-only` / `ours-only`: slots one side has past the other's length (layoutcheck's vtable check reports the
  length itself).
- `agree`: retail's target is our method's annotated address (or `_purecall` for a pure virtual, or the class's own
  deleting destructor for the destructor slot); `agree-name`: our method has no annotation of its exact decorated
  name, and retail's target is a method of the same class and name (an overload or signature to check, not a slot).
- `unknown`: our method has no annotation and retail's target has no identity, or belongs to an unrelated class (a
  COMDAT-folded copy of an identical body, most often).

Also: `inconsistent`, a polymorphic class whose slots differ between two units that define it (one class, two
definitions; a <windows.h> A/W macro renaming a virtual in some units is the usual cause), and `--leads`, the
retail slot targets without an identity with the method our slot names there (candidates for annotation).

Out of scope in this version: secondary vtables (a class with several bases with vtables: only the vtable at offset
0 is checked, under its `??_7Class@@6B<base>@@@` name), and classes with virtual bases. A report only: it changes no
source.
"""
import argparse
import concurrent.futures
import hashlib
import json
import os
import re
import sys
from collections import Counter, defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
sys.path.insert(0, str(Path(__file__).resolve().parent / "ghidra"))
import coff  # noqa: E402
import decomp  # noqa: E402
import export_types  # noqa: E402
import layoutcheck  # noqa: E402
import openwork  # noqa: E402
import vc6  # noqa: E402

OUTDIR = decomp.BUILD / "vtslots"
PROBE_VERSION = 1
TAKE = "vtslots_take"

# ---------------------------------------------------------------------------------------------------------------
# VC6's names


def encode_number(n):
    """VC6's decorated-name number: 1-10 as one digit (d+1), anything else as hex digits A-P ending in '@'."""
    if n < 0:
        return "?" + encode_number(-n)
    if 1 <= n <= 10:
        return str(n - 1)
    if n == 0:
        return "A@"
    out = ""
    while n:
        out = chr(ord("A") + (n & 15)) + out
        n >>= 4
    return out + "@"


THUNK = re.compile(r"\?\?_9@\$B(\??(?:[0-9]|[A-P]+@))AE$")


def thunk_offset(symbol):
    """The vtable offset a thiscall vcall thunk `??_9@$B<n>AE` jumps through, or None."""
    m = THUNK.match(symbol)
    if not m:
        return None
    return layoutcheck.Demangler(symbol).number(m.start(1))[0]


def take_pmf(symbol):
    """(owner class, mangled owner, rest) of an instantiated `vtslots_take<R (Owner::*)(...)>`: rest is the member
    pointer's `<this cv>E<return><parameters>` part, which follows `?<name>@<owner>@@<access>` in the method's own
    decorated name. None if the symbol isn't one."""
    prefix = "?%s@@YAXP8" % TAKE
    if not symbol.startswith(prefix) or not symbol.endswith("@Z"):
        return None
    d = layoutcheck.Demangler(symbol)
    names = [TAKE]
    try:
        parts, i = d.qualified(len(prefix), names)
    except (layoutcheck.Undecorated, IndexError):
        return None
    return "::".join(reversed(parts)), symbol[len(prefix):i], symbol[i:-2]


def method_symbols(name, mangled_owner, rest):
    """The decorated names a virtual `name` of the owner can have: private, protected or public."""
    return ["?%s@%s%s%s" % (name, mangled_owner, acc, rest) for acc in "EMU"]


OPERATORS = {"4": "operator=", "8": "operator==", "9": "operator!=", "A": "operator[]", "R": "operator()",
             "H": "operator+", "G": "operator-", "D": "operator*", "K": "operator/", "M": "operator<",
             "O": "operator>", "Y": "operator+=", "Z": "operator-="}


def symbol_method(symbol):
    """(class, method name) of a member function's decorated name, with '~' for a destructor or deleting
    destructor ('??1', '??_G', '??_E'); None for anything else."""
    r = layoutcheck.decode(symbol)
    if r is None:
        return None
    d, names, parts, i, special = r
    if special is None:
        if len(parts) < 2:
            return None
        return "::".join(reversed(parts[1:])), parts[0]
    if special in ("1", "_G", "_E"):
        return ("::".join(reversed(parts)), "~") if parts else None
    if special in OPERATORS and parts:
        return "::".join(reversed(parts)), OPERATORS[special]
    return None


def vtable_name(symbol):
    """(class, base or None) of a `??_7Class@@6B@` (None: the class's only vtable) or `??_7Class@@6BBase@@@`."""
    r = layoutcheck.decode(symbol)
    if r is None or r[4] != "_7" or not r[2]:
        return None
    d, names, parts, i, special = r
    cls = "::".join(reversed(parts))
    tail = symbol[i:]
    if tail == "6B@":
        return cls, None
    if not tail.startswith("6B"):
        return None
    try:
        bparts, j = d.qualified(i + 2, names)
    except (layoutcheck.Undecorated, IndexError):
        return None
    return cls, "::".join(reversed(bparts))


# ---------------------------------------------------------------------------------------------------------------
# our slots, from CodeView


CPP_PRIMITIVES = {
    0x03: "void", 0x10: "signed char", 0x20: "unsigned char", 0x70: "char", 0x71: "wchar_t", 0x11: "short",
    0x21: "unsigned short", 0x72: "short", 0x73: "unsigned short", 0x12: "long", 0x22: "unsigned long",
    0x74: "int", 0x75: "unsigned int", 0x13: "__int64", 0x23: "unsigned __int64", 0x40: "float", 0x41: "double",
    0x42: "long double", 0x30: "bool", 0x68: "signed char", 0x69: "unsigned char",
}


class Unspellable(Exception):
    pass


def cpp_type(tt, index, depth=0):
    """The C++ spelling of a CodeView type (what a member pointer cast names), or Unspellable."""
    if depth > 12:
        raise Unspellable(index)
    if index < 0x1000:
        base = CPP_PRIMITIVES.get(index & 0xFF)
        if base is None:
            raise Unspellable(index)
        return base + (" *" if (index >> 8) & 0xF else "")
    rec = tt.get(index)
    if rec is None:
        raise Unspellable(index)
    leaf = rec[0] | rec[1] << 8
    if leaf == 0x1001:   # LF_MODIFIER
        inner, attr = int.from_bytes(rec[2:6], "little"), rec[6] | rec[7] << 8
        t = cpp_type(tt, inner, depth + 1)
        quals = " ".join(q for q, bit in (("const", 1), ("volatile", 2)) if attr & bit)
        if not quals:
            return t
        irec = tt.get(inner) if inner >= 0x1000 else None
        if (irec is not None and irec[:2] == b"") or (inner < 0x1000 and (inner >> 8) & 0xF):
            return t + " " + quals
        return quals + " " + t
    if leaf == 0x1002:   # LF_POINTER
        utype, attr = int.from_bytes(rec[2:6], "little"), int.from_bytes(rec[6:10], "little")
        mode = (attr >> 5) & 7
        if mode not in (0, 1):
            raise Unspellable(index)
        t = cpp_type(tt, utype, depth + 1) + (" &" if mode == 1 else " *")
        if attr & 0x400:
            t += " const"
        if attr & 0x200:
            t += " volatile"
        return t
    if leaf in (0x1004, 0x1005, 0x1006, 0x1007):
        name = tt.name_of(rec)
        if not name or name.startswith("<unnamed"):
            raise Unspellable(index)
        return name
    raise Unspellable(index)


def member_pointer_type(tt, mrec, owner):
    """`R (Owner::*)(A, B) const` of an LF_MFUNCTION record, or None when a type can't be spelled."""
    import struct
    try:
        rv, cls, this, call, _, count, arglist = struct.unpack_from("<IIIBBHI", mrec, 2)
        al = tt.get(arglist)
        n = int.from_bytes(al[2:6], "little")
        args = []
        for t in struct.unpack_from("<%dI" % n, al, 6):
            if t == 0:
                args.append("...")
            elif t != 0x0003:
                args.append(cpp_type(tt, t))
        cv = ""
        trec = tt.get(this) if this >= 0x1000 else None
        if trec is not None and trec[:2] == b"":
            pointee = tt.get(int.from_bytes(trec[2:6], "little"))
            if pointee is not None and pointee[:2] == b"":
                attr = pointee[6] | pointee[7] << 8
                cv = " const" * bool(attr & 1) + " volatile" * bool(attr & 2)
        conv = {11: "", 0: "__cdecl ", 7: "__stdcall "}.get(call)
        if conv is None:
            return None
        return "%s (%s%s::*)(%s)%s" % (cpp_type(tt, rv), conv, owner, ", ".join(args) or "void", cv)
    except (Unspellable, TypeError, struct.error):
        return None


def class_defs(tt):
    """{class: {"bases": [(name, offset)], "vbases": bool, "vfptr": bool, "methods": [(name, mode, vboff, args,
    access)]}} of one object's type table; mode is CodeView's (1 virtual, 4 introducing, 5 pure, 6 pure
    introducing), args the parameter list as export_types spells types."""
    out = {}
    for rec in tt.records:
        if len(rec) < 2:
            continue
        leaf = rec[0] | rec[1] << 8
        if leaf not in (0x1004, 0x1005):
            continue
        count, prop, field = rec[2] | rec[3] << 8, rec[4] | rec[5] << 8, int.from_bytes(rec[6:10], "little")
        if prop & 0x80:
            continue
        _, off = export_types.numeric(rec, 18)
        name = export_types.pstring(rec, off)[0]
        if not name or name.startswith("<unnamed") or name in out:
            continue
        d = {"bases": [], "vbases": False, "vfptr": False, "methods": []}
        for kind, t, voff, mname, attr in tt.fields(field):
            if kind == "base":
                brec = tt.get(t)
                if brec is not None:
                    d["bases"].append((tt.name_of(brec), voff))
            elif kind == "vbase":
                d["vbases"] = True
            elif kind == "vfptr":
                d["vfptr"] = True
            elif kind == "method":
                mode = (attr >> 2) & 7
                if mode not in (1, 4, 5, 6):
                    d["methods"].append((mname, mode, None, None, attr & 3, None))
                    continue
                mrec = tt.get(t)
                sig = tt.signature(mrec) if mrec is not None else None
                args = json.dumps(sig["args"][1 if sig["this"] else 0:], sort_keys=True) if sig else None
                pmf = member_pointer_type(tt, mrec, name) if mrec is not None else None
                d["methods"].append((mname, mode, voff, args, attr & 3, pmf))
        out[name] = d
    return out


_MEMO = {}


def primary_base(defs, name):
    d = defs.get(name)
    if d is None:
        return None
    for b, off in d["bases"]:
        if off == 0 and ((defs.get(b) or {}).get("vfptr") or slot_map(defs, b)):
            return b
    return None


def is_dtor(mname):
    return mname.startswith("~") or mname in ("__vecDelDtor", "__delDtor")


def slot_map(defs, name, resolved=None, seen=()):
    """{offset: entry} of a class's primary vtable; an entry is {"owner", "name", "args", "pure", "access",
    "intro", "pmf", "uncertain"} (intro: the class that introduced the slot; pmf: the member pointer type's C++
    spelling). Destructors are named '~'.

    VC6's CodeView is wrong about overloads: every member of an overload group that has a virtual introducing
    member is recorded as an introducing virtual, all at the same offset (the group's last slot), non-virtual
    overloads included. `resolved` ({(class, name, pmf): offset, or None for a non-virtual}, from the vcall-thunk
    probe) places them; without it a group's members take the consecutive slots ending at that offset in the
    order of the method list (the reverse of the declaration order, which is VC6's order when all of them are
    virtual) and are marked uncertain."""
    key = (id(defs), name, id(resolved))
    if key in _MEMO:
        return {o: dict(e) for o, e in _MEMO[key].items()}
    d = defs.get(name)
    if d is None or name in seen:
        return {}
    m = {}
    pb = primary_base(defs, name) if d["bases"] else None
    if pb is not None:
        m = slot_map(defs, pb, resolved, seen + (name,))
    intros = [(mn, voff) for mn, mode, voff, _, _, _ in d["methods"] if mode in (4, 6) and not is_dtor(mn)]
    group, placed = Counter(intros), Counter()
    singles = {voff for mn, voff in intros if group[(mn, voff)] == 1}
    for mname, mode, voff, args, access, pmf in d["methods"]:
        if mode not in (1, 4, 5, 6):
            continue
        dtor = is_dtor(mname)
        e = {"owner": name, "name": "~" if dtor else mname, "args": None if dtor else args, "pure": mode in (5, 6),
             "access": access, "intro": name, "pmf": pmf, "uncertain": False}
        if mode in (4, 6) and voff is not None:
            n = 1 if dtor else group[(mname, voff)]
            if n > 1:
                r = (resolved or {}).get((name, mname, pmf), "?")
                if r is None:
                    continue   # a non-virtual overload
                if r == "?":
                    # unresolved: the slots ending at the offset, unless that runs into another virtual of the
                    # class (then some overloads aren't virtual, and only the last is kept, at the offset)
                    e["uncertain"] = True
                    at = voff - 4 * (n - 1 - placed[(mname, voff)])
                    if not any(voff - 4 * j in singles for j in range(1, n)):
                        m[at] = e
                    elif at == voff:
                        m[at] = e
                else:
                    m[r] = e
                placed[(mname, voff)] += 1
            else:
                m[voff] = e
            continue
        for o, x in m.items():
            if x["name"] == e["name"] and (dtor or x["args"] == args):
                m[o] = dict(e, intro=x["intro"], uncertain=x["uncertain"])
    _MEMO[key] = m
    return {o: dict(e) for o, e in m.items()}


def bool_variants(pmf, cap=8):
    """The spellings of a member pointer type with each `unsigned char` read as `bool` or not: VC6's CodeView
    records `bool` as `unsigned char`, and a cast that selects an overload must name the exact type."""
    parts = pmf.split("unsigned char")
    n = len(parts) - 1
    out = []
    for bits in range(min(1 << n, cap)):
        t = parts[0]
        for i in range(n):
            t += ("bool" if bits >> (n - 1 - i) & 1 else "unsigned char") + parts[i + 1]
        out.append(t)
    return out[::-1] if n else [pmf]


def overload_groups(defs, name):
    """[(class, name, pmf)] of the introducing members of overload groups along a class's chain: the entries
    CodeView can't place (see slot_map)."""
    out = []
    for c in chain(defs, name):
        d = defs[c]
        intros = Counter((mn, voff) for mn, mode, voff, _, _, _ in d["methods"] if mode in (4, 6) and not is_dtor(mn))
        for mn, mode, voff, args, access, pmf in d["methods"]:
            if mode in (4, 6) and not is_dtor(mn) and intros[(mn, voff)] > 1 and pmf:
                out.append((c, mn, pmf))
    return out


def chain(defs, name):
    """The class and its primary bases, most derived first."""
    out = []
    while name is not None and name not in out:
        out.append(name)
        name = primary_base(defs, name)
    return out


def overloaded(defs, name):
    """The method names that have more than one declaration (an overload or a hiding name) along the chain, apart
    from a virtual's own overrides."""
    seen = defaultdict(set)
    for c in chain(defs, name):
        for mname, mode, voff, args, access, pmf in defs[c]["methods"]:
            if not is_dtor(mname):
                seen[mname].add(args if mode in (1, 4, 5, 6) else ("nonvirtual", c))
    return {n for n, s in seen.items() if len(s) > 1}


def object_defs(objs):
    """[(unit, {class: definition})] of the /Z7 objects."""
    out = []
    for unit, obj in objs:
        if obj is None:
            continue
        defs = {}
        for tt in export_types.debug_types(obj):
            for k, v in class_defs(tt).items():
                defs.setdefault(k, v)
        out.append((unit, defs))
    return out


def norm_class(name):
    """A class name with the template spellings evened out: 'NiTMap<const_char*,X>' (config/vtables.txt),
    'NiTMap<char const *,X>' (CodeView)."""
    t = name.replace("const_char", "char const").replace("const char", "char const")
    return re.sub(r"\s+", "", t)


def same_name(a, b):
    """Two spellings of one method: equal, or one of them renamed by <windows.h>'s A/W macros (GetClassName is
    GetClassNameA in a unit that includes it)."""
    return a == b or a in (b + "A", b + "W") or b in (a + "A", a + "W")


def spellings(name):
    """A method's name and, for a name <windows.h> renamed (GetClassNameA), the name the source wrote."""
    return [name, name[:-1]] if name[-1:] in ("A", "W") else [name]


def same_slots(m1, m2):
    if set(m1) != set(m2):
        return False
    return all(m1[o]["owner"] == m2[o]["owner"] and m1[o]["pure"] == m2[o]["pure"]
               and same_name(m1[o]["name"], m2[o]["name"]) for o in m1)


def choose_units(wanted, units_defs):
    """{class: unit} for the classes wanted: a unit named after the class, else the unit defining most of them."""
    defining = defaultdict(list)
    for unit, defs in units_defs:
        for c in wanted:
            d = defs.get(c)
            if d is not None and (d["methods"] or d["bases"]):
                defining[c].append(unit)
    cover = Counter(u for us in defining.values() for u in us)
    out = {}
    for c, us in defining.items():
        short = re.sub(r"<.*", "", c).split("::")[-1].lower()
        own = [u for u in us if u.rsplit("/", 1)[-1].lower() == short]
        out[c] = own[0] if own else max(us, key=lambda u: (cover[u], u))
    return out, defining


# ---------------------------------------------------------------------------------------------------------------
# the probe


def class_probes(defs, cls):
    """[(kind, class or pmf, class named in the probe, name, offset, cast)] of one class: kind 'a', every virtual
    name of the class's slots that isn't overloaded along its chain, taken from the class (the offset is
    CodeView's, to check); kind 'b', every member of an overload group, taken through each spelling of its own
    member pointer type from the class that declares it (the second field is the CodeView pmf it resolves)."""
    out = []
    m = slot_map(defs, cls)
    ov = overloaded(defs, cls)
    for o, e in sorted(m.items()):
        if e["name"] == "~" or e["name"] in ov or not re.fullmatch(r"[A-Za-z_]\w*|operator\W+", e["name"]):
            continue
        for n in spellings(e["name"]):   # GetClassNameA: the macro may be gone at the end of the unit
            out.append(("a", cls, cls, n, o, None))
    for c, n, pmf in overload_groups(defs, cls):
        for cast in bool_variants(pmf):
            out.append(("b", pmf, c, n, None, cast))
    return out


def resolve_groups(probes, res):
    """({(class, name, pmf): offset or None for a non-virtual}, {(class, name, pmf): (owner, mangled owner, rest)})
    of the overload-group probes' results: the first spelling that compiled decides."""
    resolved, decorated = {}, {}
    for k, (kind, orig, pc, name, o, cast) in enumerate(probes):
        r = res.get(k)
        if kind == "b" and r is not None and (pc, name, orig) not in resolved:
            resolved[(pc, name, orig)] = r[1] if r[0] == "thunk" else None
            if r[0] == "thunk" and r[2]:
                decorated[(pc, name, orig)] = r[2]
    return resolved, decorated


def probe_text(unit_path, probes, access_public=True):
    lines = []
    if access_public:
        lines += ["#define private public", "#define protected public"]
    lines += ['#include "%s"' % Path(unit_path).as_posix(), "template<class T> void %s( T );" % TAKE]
    first = len(lines) + 1
    for k, (cls, name, cast) in enumerate(probes):
        # an overloaded name is taken through a cast to the overload's own member pointer type
        expr = "(%s)&%s::%s" % (cast, cls, name) if cast else "&%s::%s" % (cls, name)
        lines.append("void vtslots_probe_%d() { %s( %s ); }" % (k, TAKE, expr))
    return "\n".join(lines) + "\n", first


ERROR_LINE = re.compile(r"^(.*?)\((\d+)\) : (?:fatal )?error ", re.M)


def read_probe(obj_path, count):
    """{probe index: ('thunk', offset, (owner, mangled owner, rest)) or ('direct', symbol)} of a probe object."""
    c = coff.Coff(obj_path.read_bytes())
    out = {}
    for sym in c.symbols.values():
        m = re.fullmatch(r"\?vtslots_probe_(\d+)@@YAXXZ", sym.name)
        if not m or not sym.defined:
            continue
        k = int(m.group(1))
        sec = c.section(sym.section)
        start, end = c.function_extent(sym)
        off, pmf, direct = None, None, None
        for roff, idx, typ in sec.relocs:
            if not start <= roff < end:
                continue
            t = c.symbols.get(idx)
            if t is None:
                continue
            if thunk_offset(t.name) is not None:
                off = thunk_offset(t.name)
            elif take_pmf(t.name):
                pmf = take_pmf(t.name)
            elif t.name.startswith("?") and not t.name.startswith("?" + TAKE):
                direct = t.name
        if off is not None:
            out[k] = ("thunk", off, pmf)
        elif direct is not None:
            out[k] = ("direct", direct)
    return out


def run_probe(unit, path, probes, stamp):
    """{probe index: result} for one unit's probes, compiled with the unit's flags; failing probes are dropped by
    their error lines (up to five rounds), and the access defines are dropped if the unit's own text fails."""
    OUTDIR.mkdir(parents=True, exist_ok=True)
    safe = unit.replace("/", "--")
    text, _ = probe_text(path, probes)
    key = hashlib.sha1(("%d|%s|%s" % (PROBE_VERSION, stamp, text)).encode()).hexdigest()
    cache = OUTDIR / (safe + ".json")
    try:
        c = json.loads(cache.read_text(encoding="utf-8"))
        if c.get("key") == key:
            return {int(k): tuple(v) if v[0] == "direct" else (v[0], v[1], tuple(v[2]) if v[2] else None)
                    for k, v in c["results"].items()}, c["dropped"], c["log"]
    except (OSError, ValueError, KeyError):
        pass
    results, dropped, log = compile_probes(path, probes, decomp.cl_args(unit, path, pch=False)[:-2],
                                           decomp.compiler_of(path), OUTDIR, safe)
    cache.write_text(json.dumps({"key": key, "results": {str(k): v for k, v in results.items()},
                                 "dropped": {str(k): v for k, v in dropped.items()}, "log": log}),
                     encoding="utf-8")
    return results, {str(k): v for k, v in dropped.items()}, log


def compile_probes(path, probes, base_args, compiler, workdir, stem):
    """(results, {probe index: error}, log of a compile that failed outside the probes) of one probe source that
    includes `path`, compiled with `base_args` (the unit's flags, without the source)."""
    src, obj = Path(workdir) / (stem + ".cpp"), Path(workdir) / (stem + ".obj")
    live = list(range(len(probes)))
    access_public, dropped, log, results = True, {}, "", {}
    for _ in range(6):
        t, first = probe_text(path, [probes[k] for k in live], access_public)
        src.write_text(t, encoding="latin-1", newline="")
        if obj.exists():
            obj.unlink()
        args = list(base_args) + ["/c", "/Tp", str(src), "/Fo" + str(obj)]
        rc, out = vc6.run("cl", args, cwd=str(workdir), compiler=compiler)
        if rc == 0 and obj.exists():
            got = read_probe(obj, len(live))
            results = {live[k]: v for k, v in got.items()}
            break
        bad, foreign = set(), False
        for fname, line in ERROR_LINE.findall(out):
            n = int(line) - first
            if Path(fname).name == src.name and 0 <= n < len(live):
                bad.add(n)
            else:
                foreign = True
        if foreign and access_public:
            access_public = False   # the unit's own code doesn't take `private` as `public`
            continue
        if not bad:
            log = out[-3000:]
            break
        for n in sorted(bad):
            k = live[n]
            msg = [ln for ln in out.splitlines() if re.match(r".*\(%d\) : (?:fatal )?error" % (first + n), ln)]
            dropped[k] = msg[0].split(" : ", 1)[1][:160] if msg else "error"
        live = [k for n, k in enumerate(live) if n not in bad]
        if not live:
            break
    for f in (src, obj):
        try:
            f.unlink()
        except OSError:
            pass
    return results, dropped, log


# ---------------------------------------------------------------------------------------------------------------
# retail


def retail_vtables(img, rows, claims):
    """{class: retail address} of the classes' primary vtables (the one at offset 0)."""
    found = {}   # class -> {base or None: address}
    names = layoutcheck.relocated_names(img, rows)
    for a, n in names.items():
        if n.startswith("??_7"):
            v = vtable_name(n)
            if v:
                found.setdefault(v[0], {}).setdefault(v[1], a)
    for n, a, _ in claims:
        if n.startswith("??_7"):
            v = vtable_name(n)
            if v:
                found.setdefault(v[0], {}).setdefault(v[1], a)
    for cls, (a, _) in layoutcheck.listed_vtables().items():
        found.setdefault(cls, {}).setdefault(None, a)
    return found


class Identities:
    """The names retail's addresses carry (annotations and claims), and the addresses of our names."""

    def __init__(self, rows, claims):
        self.at = defaultdict(set)       # address -> decorated names
        self.addr = defaultdict(set)     # decorated name -> addresses
        for r in rows:
            if r.get("symbol"):
                a = int(r["addr"], 16)
                self.at[a].add(r["symbol"])
                self.addr[r["symbol"]].add(a)
        for n, a, _ in claims:
            self.at[a].add(n)
            self.addr[n].add(a)
        self.by_method = defaultdict(set)   # (class, name) -> decorated names
        for n in self.addr:
            m = symbol_method(n)
            if m:
                self.by_method[m].add(n)
        self.purecall = set()
        for n in ("__purecall", "_purecall"):
            self.purecall |= self.addr.get(n, set())

    def methods_at(self, a):
        """[(class, name, symbol)] of the member functions retail's address carries."""
        out = []
        for s in sorted(self.at.get(a, ())):
            m = symbol_method(s)
            if m:
                out.append((m[0], m[1], s))
        return out

    def label(self, a):
        if a in self.purecall:
            return "_purecall"
        ms = self.methods_at(a)
        if ms:
            return " / ".join("%s::%s" % (c, n) for c, n, _ in ms[:2])
        names = sorted(self.at.get(a, ()))
        return names[0] if names else "?"


# ---------------------------------------------------------------------------------------------------------------
# the comparison

RANK = {"moved": 0, "wrong": 1, "name": 2, "pure": 3, "override": 4, "inconsistent": 5, "retail-only": 6,
        "ours-only": 7, "probe": 8, "unknown": 9, "agree-name": 10, "agree": 11}


def same_bytes(img, a, b, n=64):
    """Whether two short functions are byte-identical up to their first `ret` (a copy of an identical body)."""
    def body(x):
        out = []
        for i in range(n):
            w = img.dword(x + i)
            if w is None:
                return None
            out.append(w & 0xFF)
            if out[-1] in (0xC3, 0xC2):
                return out
        return None
    x = body(a)
    return x is not None and x == body(b)


def judge_class(cls, defs, m, probe, va, img, length, ids, base_slot=None):
    """[slot rows] of one class: {"slot", "offset", "verdict", "ours", "retail", "target", "why"}. `base_slot(k)`
    gives retail's target of slot k in the primary base's own vtable (None when unknown)."""
    rows = []
    targets = [img.dword(va + 4 * k) for k in range(length)]
    where = defaultdict(list)
    for k, t in enumerate(targets):
        where[t].append(k)
    ch = set(chain(defs, cls))
    nslots = max(m) // 4 + 1 if m else 0
    for k in range(max(nslots, length)):
        e = m.get(4 * k)
        t = targets[k] if k < length else None
        row = {"slot": k, "offset": 4 * k, "target": t, "retail": ids.label(t) if t is not None else None,
               "ours": None, "why": ""}
        rows.append(row)
        if e is None:
            row["verdict"] = "retail-only" if t is not None else "ours-only"
            if t is not None and k < nslots:
                row["why"] = "no virtual of ours at this offset"
            continue
        row["uncertain"] = e["uncertain"]
        row["ours"] = "%s::%s%s" % (e["owner"], e["name"] if e["name"] != "~" else "~" + e["owner"].split("::")[-1].split("<")[0],
                                    " = 0" if e["pure"] else "")
        if t is None:
            row["verdict"] = "ours-only"
            continue
        # our method's addresses
        if e["name"] == "~":
            syms = [s for s in ids.by_method.get((cls, "~"), ()) if s.startswith(("??_G", "??_E"))]
        else:
            p = probe.get(4 * k)
            if p is not None:
                syms = [s for n in spellings(e["name"]) for s in method_symbols(n, p[1], p[2]) if s in ids.addr]
            else:
                syms = sorted(s for n in spellings(e["name"]) for s in ids.by_method.get((e["owner"], n), ()))
        ours = set()
        for s in syms:
            ours |= ids.addr[s]
        if e["pure"]:
            ours |= ids.purecall
        if t in ours:
            row["verdict"] = "agree"
            continue
        if e["pure"]:
            row["verdict"] = "pure"
            row["why"] = "ours is pure virtual; retail's slot holds a function"
            continue
        if t in ids.purecall:
            row["verdict"] = "pure"
            row["why"] = "retail's slot is _purecall; ours isn't pure"
            continue
        if ours:
            other = sorted(j for a in ours for j in where.get(a, ()) if j != k)
            if other:
                row["verdict"] = "moved"
                row["why"] = "our method is retail's slot %s" % ", ".join(map(str, other))
            else:
                row["verdict"] = "wrong"
                row["why"] = "our method is annotated at %s, not in this vtable" % ", ".join(
                    "0x%X" % a for a in sorted(ours))
            continue
        held = ids.methods_at(t)
        same = [x for x in held if same_name(x[1], e["name"]) and x[0] == e["owner"]]
        if same:
            row["verdict"] = "agree-name"
            row["why"] = "retail's target is %s (not our method's decorated name)" % same[0][2]
            continue
        bt = base_slot(k) if base_slot and e["name"] != "~" else None
        if bt is not None and (bt != t) == (e["owner"] == cls):
            pass   # retail's class overrides the slot exactly when ours does
        elif bt is not None and e["owner"] != cls:
            row["verdict"] = "override"
            row["why"] = "retail's %s has its own function here (the base's is 0x%X); ours inherits %s::%s%s" % (
                cls, bt, e["owner"], e["name"], "; the two bodies are identical: an override with "
                "the same body, or another kept copy of the inline (AGENTS.md, Annotations: TGStream::ReadBool)"
                if same_bytes(img, t, bt) else "")
            continue
        elif bt is not None:
            row["verdict"] = "override"
            row["why"] = "ours overrides %s here; retail holds the base's function (or a folded identical body)" % (
                e["name"])
            continue
        if e["name"] == "~":
            row["verdict"] = "wrong" if any(n == "~" and c in ch for c, n, _ in held) else "unknown"
            row["why"] = "our destructor slot: retail holds another class's deleting destructor" if row[
                "verdict"] == "wrong" else ""
            continue
        rel = [x for x in held if x[0] in ch or x[0] == cls]
        if any(same_name(x[1], e["name"]) for x in rel):
            x = next(x for x in rel if same_name(x[1], e["name"]))
            row["verdict"] = "override"
            row["why"] = "retail holds %s::%s, ours %s::%s" % (x[0], x[1], e["owner"], e["name"])
        elif rel:
            row["verdict"] = "name"
            row["why"] = "retail holds %s::%s here" % (rel[0][0], rel[0][1])
        else:
            row["verdict"] = "unknown"
            if held:
                row["why"] = "retail's target belongs to %s (a folded copy?)" % held[0][0]
    return rows


def run(only=None, probe=True, fresh=False, jobs=None):
    objs = export_types.build_objects(stale_only=not fresh)
    failed = [u for u, o in objs if o is None]
    units_defs = object_defs(objs)
    rows, claims = openwork.load()
    img = decomp.pe.Image(decomp.target_image())
    ids = Identities(rows, claims)
    vt = retail_vtables(img, rows, claims)
    merged = {}
    for _, defs in units_defs:
        for k, v in defs.items():
            merged.setdefault(k, v)
    spelled = {norm_class(k): k for k in merged}
    vt = {spelled.get(norm_class(k), k): v for k, v in vt.items()}
    # each class's primary vtable: its only one, or the one named after its primary base chain
    wanted = {}
    for cls, by_base in vt.items():
        if cls not in merged or (only and cls not in only):
            continue
        if merged[cls]["vbases"]:
            continue
        if None in by_base:
            wanted[cls] = by_base[None]
        else:
            ch = chain(merged, cls)[1:]
            for b in ch:
                if b in by_base:
                    wanted[cls] = by_base[b]
                    break
    unit_of, defining = choose_units(wanted, units_defs)
    defs_of = dict(units_defs)
    srcs = decomp.source_files()
    # inconsistency between units: every class with virtuals that two units define, retail vtable known or not
    inconsistent = {}
    if not only:
        polymorphic = {c for c, d in merged.items() if d["vfptr"] or any(m[1] in (1, 4, 5, 6) for m in d["methods"])}
        others, odefining = choose_units(polymorphic - set(wanted), units_defs)
        all_units, all_defining = dict(others, **unit_of), dict(odefining, **defining)
    else:
        all_units, all_defining = unit_of, defining
    for cls, us in all_defining.items():
        if len(us) < 2:
            continue
        ref = slot_map(defs_of[all_units[cls]], cls)
        if not ref:
            continue
        for u in us:
            if u != all_units[cls] and not same_slots(slot_map(defs_of[u], cls), ref):
                inconsistent.setdefault(cls, []).append(u)
    # the probes: (a) every virtual name of a class that isn't overloaded along its chain, taken from the class;
    # (b) every member of an overload group, taken through its own member pointer type from the class that
    # declares it (CodeView can't place those)
    probes_of = defaultdict(list)   # unit -> [(kind, class, class named in the probe, name, offset, cast)]
    for cls, u in unit_of.items():
        for item in class_probes(defs_of[u], cls):
            if item[0] == "a" or item not in probes_of[u]:
                probes_of[u].append(item)
    probe_rows = defaultdict(dict)   # class -> {offset: (owner, mangled owner, rest)}
    resolved = defaultdict(dict)     # unit -> {(class, name, pmf): offset or None}
    decorated = {}                   # (unit, class, name, pmf) -> (owner, mangled owner, rest)
    tool_errors, dropped_n, probed = [], 0, 0
    results = {}
    if probe:
        objs_by = dict(objs)

        def one(u):
            ps = probes_of[u]
            st = objs_by.get(u)
            stamp = "%s:%s" % (st.stat().st_mtime_ns, st.stat().st_size) if st is not None and st.exists() else ""
            return u, run_probe(u, srcs[u], [(pc, n, cast) for _, _, pc, n, _, cast in ps], stamp)
        with concurrent.futures.ThreadPoolExecutor(max_workers=jobs or os.cpu_count()) as ex:
            for u, (res, dropped, log) in ex.map(one, sorted(probes_of)):
                if log:
                    tool_errors.append("%s: the probe did not compile: %s" % (u, log.strip().splitlines()[-1][:200]))
                results[u] = res
                resolved[u], dec = resolve_groups(probes_of[u], res)
                decorated.update(((u,) + k, v) for k, v in dec.items())
    maps = {}
    for cls, u in unit_of.items():
        maps[cls] = slot_map(defs_of[u], cls, resolved[u] if probe else None)
        for o, e in maps[cls].items():
            pmf = decorated.get((u, e["owner"], e["name"], e["pmf"]))
            if pmf:
                probe_rows[cls][o] = pmf
    for u, res in results.items():
        groups, done = set(), set()
        for k, (kind, cls, pc, name, o, cast) in enumerate(probes_of[u]):
            g = (kind, cls, o) if kind == "a" else (kind, pc, name, cls)
            groups.add(g)
            r = res.get(k)
            if r is None or g in done:
                continue
            done.add(g)
            if kind != "a":
                continue
            if r[0] == "direct":
                tool_errors.append("%s::%s (offset 0x%X): &%s::%s is not a vcall thunk: %s" % (
                    cls, name, o, cls, name, r[1]))
                continue
            probed += 1
            _, off, pmf = r
            e = maps[cls].get(off)
            if e is None or not same_name(e["name"], name):
                tool_errors.append("%s::%s: CodeView offset 0x%X, the vcall thunk's 0x%X" % (cls, name, o, off))
                continue
            if pmf and pmf[0] != e["owner"]:
                tool_errors.append("%s::%s: CodeView owner %s, the member pointer's %s" % (cls, name, e["owner"], pmf[0]))
            if pmf:
                probe_rows[cls][off] = pmf
        dropped_n += len(groups - done)
    probed += sum(1 for v in resolved.values() for x in v.values() if x is not None)
    relocs = img.relocs
    labels = layoutcheck.code_referenced(img) | set(wanted.values())
    report, lengths = [], {}
    for cls in sorted(wanted):
        if cls not in unit_of:
            continue
        va = wanted[cls]
        length = layoutcheck.vtable_length(img, va, labels, relocs)
        pb = primary_base(defs_of[unit_of[cls]], cls)

        def base_slot(k, pb=pb):
            if pb not in wanted:
                return None
            bva = wanted[pb]
            return img.dword(bva + 4 * k) if k < lengths.setdefault(
                pb, layoutcheck.vtable_length(img, bva, labels, relocs)) else None
        slots = judge_class(cls, defs_of[unit_of[cls]], maps[cls], probe_rows.get(cls, {}), va, img, length, ids,
                            base_slot)
        report.append({"class": cls, "address": va, "unit": unit_of[cls], "retail_slots": length,
                       "our_slots": max(maps[cls]) // 4 + 1 if maps[cls] else 0, "slots": slots,
                       "inconsistent": inconsistent.get(cls, []),
                       "probed": sorted(o // 4 for o in probe_rows.get(cls, {}))})
    unchecked = {c: (all_units[c], us) for c, us in inconsistent.items() if c not in wanted}
    return {"classes": report, "inconsistent_unchecked": unchecked, "tool_errors": tool_errors, "probed": probed, "dropped": dropped_n,
            "failed_units": failed, "vtables_known": len(vt),
            "skipped": sorted(c for c in set(vt) - set(unit_of) if not only or c in only)}


def class_score(c):
    v = Counter(s["verdict"] for s in c["slots"])
    return tuple(-v.get(k, 0) for k in ("moved", "wrong", "name", "pure", "override")) + (-len(c["inconsistent"]),)


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("--class", dest="cls", action="append", help="only this class (repeatable): every slot")
    ap.add_argument("--all", action="store_true", help="every slot of every class, agreeing slots too")
    ap.add_argument("--no-probe", action="store_true", help="CodeView only: no vcall-thunk probe compiles")
    ap.add_argument("--fresh", action="store_true", help="compile every unit's /Z7 copy again")
    ap.add_argument("--top", type=int, default=0, help="print only the first N classes with findings")
    ap.add_argument("--leads", action="store_true",
                    help="list the retail slot targets without an identity, with the method our slot names there")
    ap.add_argument("--json", help="also write the full report to this file")
    a = ap.parse_args(argv)
    r = run(set(a.cls) if a.cls else None, not a.no_probe, a.fresh)
    classes = r["classes"]
    total = Counter(s["verdict"] for c in classes for s in c["slots"])
    findings = ("moved", "wrong", "name", "pure", "override")
    flagged = [c for c in classes if any(s["verdict"] in findings for s in c["slots"]) or c["inconsistent"]]
    print("%d classes checked (%d slots), %d vtables known, %d classes out of scope (no definition, virtual bases or "
          "a secondary vtable only); probe: %d slots by vcall thunk, %d probes dropped, %d tool errors%s" % (
              len(classes), sum(total.values()), r["vtables_known"], len(r["skipped"]), r["probed"], r["dropped"],
              len(r["tool_errors"]), "; %d units did not compile with /Z7" % len(r["failed_units"])
              if r["failed_units"] else ""))
    print("verdicts: " + ", ".join("%s %d" % (k, total[k]) for k in sorted(total, key=RANK.get)))
    print("secondary vtables (multiple inheritance) are not checked; (?) marks an overload CodeView can't place "
          "and the probe didn't resolve")
    for e in r["tool_errors"][:20]:
        print("tool: " + e)
    leads = defaultdict(set)   # retail target without an identity -> our methods there
    for c in classes:
        for sl in c["slots"]:
            if sl["verdict"] == "unknown" and sl["retail"] == "?" and sl["ours"]:
                leads[sl["target"]].add(sl["ours"])
    print("%d retail slot targets have no identity; our slots name a method for each%s" % (
        len(leads), "" if a.leads else " (--leads lists them)"))
    if a.leads:
        for t in sorted(leads):
            names = sorted(leads[t])
            print("  0x%X  %s%s" % (t, " | ".join(names[:3]), "  (%d names: conflicting)" % len(names)
                                      if len({n.split("::")[-1] for n in names}) > 1 else ""))
    other = r.get("inconsistent_unchecked") or {}
    if other and not a.cls:
        print("\n%d classes without a known retail vtable have slots that differ between units:" % len(other))
        for cls, (u, us) in sorted(other.items()):
            print("  %s: %s against %s" % (cls, u, ", ".join(us[:5])))
    show = classes if (a.all or a.cls) else sorted(flagged, key=class_score)
    if a.top:
        show = show[:a.top]
    for c in show:
        v = Counter(s["verdict"] for s in c["slots"])
        print("\n%s  vtable 0x%X  retail %d slots, ours %d  (%s; %s)" % (
            c["class"], c["address"], c["retail_slots"], c["our_slots"], c["unit"],
            ", ".join("%s %d" % (k, v[k]) for k in sorted(v, key=RANK.get))))
        if c["inconsistent"]:
            print("  inconsistent: the slots differ in %s" % ", ".join(c["inconsistent"][:5]))
        for s in c["slots"]:
            if not (a.all or a.cls) and s["verdict"] not in findings:
                continue
            print("  [%2d] +0x%03X %-11s ours %-45s retail %s%s%s" % (
                s["slot"], s["offset"], s["verdict"], (s["ours"] or "-") + (" (?)" if s.get("uncertain") else ""),
                "0x%X " % s["target"] if s["target"] is not None else "", s["retail"] or "-",
                "  (%s)" % s["why"] if s["why"] else ""))
    if a.json:
        Path(a.json).write_text(json.dumps(r, indent=1), encoding="utf-8")
    return 0


if __name__ == "__main__":
    sys.exit(main())
