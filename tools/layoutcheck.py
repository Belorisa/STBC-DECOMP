"""Compare our class layouts with what retail's code does with them: wrong member types, class sizes and vtable sizes.

    python tools/layoutcheck.py                      # every finding, strongest first
    python tools/layoutcheck.py --class ShipClass    # one class (and the members it inherits)
    python tools/layoutcheck.py --check member --top 50
    python tools/layoutcheck.py --json build/layoutcheck.json
    python tools/layoutcheck.py --weak               # also the weak findings (a movzx of a signed member, ...)

Our layouts are the compiler's own: every compiled unit is built once more with /Z7 into build/types/
(tools/ghidra/export_types.py's build; a unit is compiled again only when its build/obj/ object is newer) and the
CodeView type records give each class's members (offset, type), bases, vtable pointer and virtual slots. The merged
types are cached in build/layoutcheck_types.pkl by the objects' stamps.

Three checks, read from the retail image (build/report.json gives the annotated functions and their classes):

- **member** (member access types). Every annotated function whose decorated name this tool reads is disassembled
  from retail, and a forward must-analysis over its basic blocks follows which registers hold a pointer of known
  class: `this` (ECX at entry of a `__thiscall` member: tier 0), and, as tier 1, a pointer loaded from a member of
  pointer type (NiPointer<T> included), a class-pointer or reference parameter of the decorated signature (read
  from its stack slot before the first call, and dropped when the function writes that slot: VC6 reuses dead
  parameter slots), a global object or global pointer of class type (from the matched functions' relocations) and
  the result of a callee whose decorated name returns a class pointer. Copies, `lea` and `add`/`sub` adjustments
  carry the pointer. Each `[reg+disp]` operand on such a register is resolved to the member at that offset, down to
  its leaf (bases, nested structs and arrays are entered), and judged against the leaf's type: its width
  (byte/word/dword/qword), signedness (movsx/movzx, and a signed jcc right after a `cmp`) and float against integer
  (fld/fst/fcom against fild/fist, mov, add, inc). A direct call to a member function with ECX on a subobject is
  judged too: the callee's class must be the subobject's class or one of its bases (a member of another class, a
  derived class's method, another instance of a template). Operands on untracked registers and stack slots are
  left out and counted. Grades: strong (a float op on an integer, a movsx of an unsigned member, a store narrower
  than the member, a wider access to a lone member, an access at an offset no member covers, an unrelated class's
  method), medium (a narrower load, a load inside a member, a signed compare of an unsigned member, a derived class's
  method called more often than the member's own class's, another template instance) and weak (only with --weak: a
  movzx of a signed member, an access past sizeof through a typed pointer, a cast-like derived call). A
  contradiction inside a matched function counts one grade lower: its bytes are our compiler's output for our
  source, so a cast there explains it. Never findings: a bit operation or shift on the loaded part of an integer
  (VC6 narrows `member & mask`), a narrow load stored, pushed or computed at its own width (a conversion to a
  narrower field, argument or local), an __int64 in dword halves, a wider move over whole neighbouring members (a
  struct copy), bitfields, empty classes and classes with virtual bases.
- **size** (class sizes). Each call to an allocator (operator new, TGPoolManager::GetPool / AllocateObject) whose
  size is a pushed immediate, whose result reaches a constructor call in ECX (followed through register copies; a
  member's constructor on an offset into the object doesn't count), gives the constructed class's retail size. When
  the constructor is a base's, the derived class's inline constructor stores its own vtable at offset 0 afterwards
  and the last such store names the class (an unknown vtable leaves the site out). A multiple of our size (an array)
  or the size of a class our types derive from it (an inline constructor with no vtable to store) agrees.
- **vtable** (vtable sizes). Each class whose retail vtable address is known (a relocation to '??_7Class@@6B@' in a
  matched function, or config/vtables.txt) has its retail length counted: consecutive relocated dwords pointing into
  .text, up to the next address any code references. Set against our slot count, with the functions retail holds in
  the slots that differ: a slot past ours holding a method of the class is a virtual our source declares
  non-virtual (strong). A virtual call through a tracked vtable pointer to a slot past our count is reported too.

Each finding gives its counts in matched and open functions and two example sites; --json writes every site. The
tool can't see the source's casts and accessor return types, so a finding is a lead to read, not a verdict. A
report only: it changes no source.
"""
import argparse
import bisect
import json
import pickle
import re
import sys
from collections import Counter, defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
sys.path.insert(0, str(Path(__file__).resolve().parent / "ghidra"))
import decomp  # noqa: E402
import export_types  # noqa: E402
import openwork  # noqa: E402

TYPES_CACHE = decomp.BUILD / "layoutcheck_types.pkl"
CACHE_VERSION = 2

# ---------------------------------------------------------------------------------------------------------------
# our types


class BitTypeTable(export_types.TypeTable):
    """export_types' reader with bitfields kept as such (a bitfield's access width says nothing of its type)."""

    def ref(self, index, depth=0):
        rec = self.get(index) if index >= 0x1000 else None
        if rec is not None and len(rec) >= 8 and rec[0] | rec[1] << 8 == 0x1205:
            return {"k": "bits", "of": export_types.TypeTable.ref(self, int.from_bytes(rec[2:6], "little"), depth + 1),
                    "width": rec[6], "pos": rec[7]}
        return export_types.TypeTable.ref(self, index, depth)

    def composite(self, leaf, size, field):
        d = export_types.TypeTable.composite(self, leaf, size, field)
        # a class with virtual bases has a vbase table pointer and the bases' members after its own: neither is
        # laid out here, so its holes are not evidence
        d["vbases"] = any(kind == "vbase" for kind, _, _, _, _ in self.fields(field))
        return d


def load_types(fresh=False):
    """{name: definition} of every class, struct, union and enum our compiled units define (export_types' shape)."""
    if fresh:
        objs = export_types.build_objects()
    else:
        objs = export_types.build_objects(stale_only=True)
    stamp = sorted((u, o.stat().st_mtime_ns, o.stat().st_size) for u, o in objs if o is not None)
    try:
        with open(TYPES_CACHE, "rb") as f:
            cached = pickle.load(f)
        if cached.get("version") == CACHE_VERSION and cached.get("stamp") == stamp:
            return cached["types"], cached["failed"]
    except (OSError, pickle.PickleError, EOFError, AttributeError, KeyError):
        pass
    types, _ = export_types.read_types(objs, BitTypeTable)
    failed = [u for u, o in objs if o is None]
    TYPES_CACHE.parent.mkdir(parents=True, exist_ok=True)
    with open(TYPES_CACHE, "wb") as f:
        pickle.dump({"version": CACHE_VERSION, "stamp": stamp, "types": types, "failed": failed}, f,
                    pickle.HIGHEST_PROTOCOL)
    return types, failed


PRIM_SIZE = {"void": 0, "schar": 1, "uchar": 1, "char": 1, "wchar": 2, "short": 2, "ushort": 2, "long": 4,
             "ulong": 4, "int": 4, "uint": 4, "longlong": 8, "ulonglong": 8, "float": 4, "double": 8,
             "longdouble": 8, "bool": 1}
SIGNED = {"schar", "char", "short", "long", "int", "longlong"}
UNSIGNED = {"uchar", "wchar", "ushort", "ulong", "uint", "ulonglong", "bool"}
FLOATS = {"float", "double", "longdouble"}


def type_text(t):
    k = t["k"]
    if k == "prim":
        return {"uchar": "unsigned char", "ushort": "unsigned short", "uint": "unsigned int", "ulong": "unsigned long",
                "schar": "signed char", "longlong": "__int64", "ulonglong": "unsigned __int64"}.get(t["n"], t["n"])
    if k == "ptr":
        return type_text(t["to"]) + " *"
    if k == "arr":
        return "%s[%d bytes]" % (type_text(t["of"]), t["bytes"])
    if k == "named":
        return t["n"]
    if k == "bits":
        return "%s : %d" % (type_text(t["of"]), t["width"])
    if k in ("empty", "vptr"):
        return t.get("n") or "vtable pointer"
    return "function"


class Layouts:
    """Member lookup over our types: flattened members (bases inlined at their offsets), leaves, sizes, slots."""

    def __init__(self, types):
        self.types = types
        self._flat, self._leaves = {}, {}

    def size_of(self, t):
        k = t["k"]
        if k == "prim":
            return PRIM_SIZE.get(t["n"], 4)
        if k == "ptr":
            return 4
        if k == "arr":
            return t["bytes"]
        if k == "named":
            d = self.types.get(t["n"])
            return d["size"] if d else None
        if k == "bits":
            return self.size_of(t["of"])
        return None

    def sizeof(self, name):
        d = self.types.get(name)
        return d["size"] if d and d["kind"] != "enum" else None

    def members(self, name, seen=()):
        """[(offset, owner class, member name, type)] of a class with its bases inlined. The vtable pointer at a
        base's offset is a member 'vftable' of that base's primary chain."""
        if name in self._flat and not seen:
            return self._flat[name]
        d = self.types.get(name)
        out = []
        if d is not None and d["kind"] != "enum" and name not in seen:
            for b in d["bases"]:
                out += [(off + b["offset"], owner, n, t) for off, owner, n, t in self.members(b["name"], seen + (name,))]
            if d["vfptr"] and not any(off == 0 and n == "vftable" for off, _, n, _ in out):
                out.append((0, name, "vftable", {"k": "vptr", "cls": name}))
            out += [(f["offset"], name, f["name"], f["type"]) for f in d["fields"]]
            out.sort(key=lambda m: m[0])
        if not seen:
            self._flat[name] = out
        return out

    def leaves(self, name, off, depth=0):
        """[(owner, path, leaf type, leaf offset in `name`, leaf size, top-level member offset, top-level size)] of
        every leaf that covers byte `off` of class `name` (several in a union); [] when no member covers it."""
        key = (name, off)
        if depth == 0 and key in self._leaves:
            return self._leaves[key]
        out = []
        for moff, owner, mname, t in self.members(name):
            size = self.size_of(t) if t["k"] != "vptr" else 4
            if size is None or not (moff <= off < moff + max(size, 1)):
                continue
            for path, lt, loff, lsize in self._descend(t, off - moff, mname, depth):
                out.append((owner, path, lt, moff + loff, lsize, moff, size))
        if depth == 0:
            self._leaves[key] = out
        return out

    def _descend(self, t, off, path, depth):
        if depth > 8:
            return []
        k = t["k"]
        if k == "arr":
            esize = self.size_of(t["of"]) or 1
            i = off // esize
            return [(p, lt, i * esize + lo, ls) for p, lt, lo, ls in
                    self._descend(t["of"], off - i * esize, "%s[%d]" % (path, i), depth + 1)]
        if k == "named":
            d = self.types.get(t["n"])
            if d is None:
                return [(path, t, 0, None)]
            if d["kind"] == "enum":
                return [(path, t, 0, 4)]
            if not d["fields"] and not d["bases"] and not d["vfptr"]:
                return [(path, {"k": "empty", "n": t["n"]}, 0, d["size"])]   # an empty class (std::allocator)
            out = []
            for owner, p, lt, loff, lsize, _, _ in self.leaves(t["n"], off, depth + 1):
                out.append(("%s.%s" % (path, p), lt, loff, lsize))
            return out or [(path + ".?", None, off, None)]   # padding inside the struct
        return [(path, t, 0, self.size_of(t) if k != "vptr" else 4)]

    def has_vbases(self, name, depth=0):
        d = self.types.get(name)
        return d is not None and depth < 12 and (d.get("vbases") or any(
            self.has_vbases(b["name"], depth + 1) for b in d.get("bases", ())))

    def subobjects(self, name, off, depth=0):
        """[(class, owner, member path, member offset)] of every class subobject that starts at byte `off` of
        `name`: the object itself at 0 (owner None), its bases, and members of class type (arrays entered). The
        owner and path name the outermost member that holds the subobject, at its offset in `name`."""
        out = [(name, None, None, None)] if off == 0 else []
        d = self.types.get(name)
        if d is None or d["kind"] == "enum" or depth > 8:
            return out
        for b in d["bases"]:
            size = self.sizeof(b["name"]) or 0
            if b["offset"] <= off < b["offset"] + max(size, 1):
                for c, owner, path, moff in self.subobjects(b["name"], off - b["offset"], depth + 1):
                    out.append((c, owner, path, None if moff is None else moff + b["offset"]))
        for f in d["fields"]:
            t, fo, path = f["type"], f["offset"], f["name"]
            while t["k"] == "arr":
                esize = self.size_of(t["of"]) or 1
                if not f["offset"] <= off < f["offset"] + t["bytes"]:
                    break
                i = (off - fo) // esize
                fo, path, t = fo + i * esize, "%s[%d]" % (path, i), t["of"]
            if t["k"] != "named" or self.types.get(t["n"], {}).get("kind") in (None, "enum"):
                continue
            size = self.sizeof(t["n"]) or 0
            if fo <= off < fo + max(size, 1):
                for c, owner, p, moff in self.subobjects(t["n"], off - fo, depth + 1):
                    out.append((c, name, path if p is None else "%s.%s" % (path, p), f["offset"]))
        return out

    def slots(self, name):
        """Our vtable length of a class: its primary chain's virtual slots."""
        s = export_types.vtable(name, self.types)
        return max(s) // 4 + 1 if s else 0


def category(t, types):
    """'float', 'sint', 'uint', 'ptr', 'vptr', 'bits', 'struct' or None of a leaf type."""
    if t is None:
        return None
    k = t["k"]
    if k == "prim":
        n = t["n"]
        return "float" if n in FLOATS else "sint" if n in SIGNED else "uint" if n in UNSIGNED else None
    if k == "ptr":
        return "ptr"
    if k == "vptr":
        return "vptr"
    if k == "bits":
        return "bits"
    if k == "empty":
        return "empty"
    if k == "named":
        d = types.get(t["n"])
        if d is not None and d["kind"] == "enum":
            return "sint"   # VC6 enums are ints
        return "struct"
    return None


# ---------------------------------------------------------------------------------------------------------------
# decorated names

THISCALL_ACCESS = set("ABEFIJMNQRUV")   # non-static members: private, protected, public, plain and virtual
PRIM_CODES = {"C": ("signed char", 1), "D": ("char", 1), "E": ("unsigned char", 1), "F": ("short", 2),
              "G": ("unsigned short", 2), "H": ("int", 4), "I": ("unsigned int", 4), "J": ("long", 4),
              "K": ("unsigned long", 4), "M": ("float", 4), "N": ("double", 8), "O": ("long double", 8),
              "X": ("void", 0)}
PRIM_EXT = {"N": ("bool", 1), "J": ("__int64", 8), "K": ("unsigned __int64", 8), "W": ("wchar_t", 2)}


class Undecorated(Exception):
    pass


class Demangler:
    """A reader of the MSVC decorated names this project's functions and globals carry: qualified names (templates
    included, spelled as the CodeView records spell them: 'NiTArray<NiObject *>', 'NiTMap<char const *,X *>'),
    and types as (CodeView spelling, size, class a pointer or reference points to). Raises Undecorated on what it
    can't read (function pointers, member pointers, the rarer codes)."""

    def __init__(self, s, lay=None):
        self.s, self.lay = s, lay

    # -- names ------------------------------------------------------------------------------------------------
    def fragment(self, i, names):
        s = self.s
        if s[i].isdigit():
            k = int(s[i])
            if k >= len(names):
                raise Undecorated(s)
            return names[k], i + 1
        if s.startswith("?$", i):
            j = s.find("@", i + 2)
            if j < 0:
                raise Undecorated(s)
            base = s[i + 2:j]
            inner = [base]   # a template's arguments have a back-reference table of their own
            args, i = self.targs(j + 1, inner)
            text = "%s<%s%s>" % (base, ",".join(args), " " if args and args[-1].endswith(">") else "")
            if len(names) < 10:
                names.append(text)
            return text, i
        if s[i] == "?":
            raise Undecorated(s)
        j = s.find("@", i)
        if j < 0:
            raise Undecorated(s)
        text = s[i:j]
        if text not in names and len(names) < 10:
            names.append(text)
        return text, j + 1

    def qualified(self, i, names):
        """([fragments, innermost first], next index) of a qualified name ending in '@'."""
        parts = []
        while i < len(self.s) and self.s[i] != "@":
            text, i = self.fragment(i, names)
            parts.append(text)
        if not parts:
            raise Undecorated(self.s)
        return parts, i + 1

    def targs(self, i, names):
        args, types = [], []
        while i < len(self.s) and self.s[i] != "@":
            if self.s.startswith("$0", i):
                v, i = self.number(i + 2)
                args.append(str(v))
                continue
            t, i = self.type(i, names, types)
            args.append(t[0])
        return args, i + 1

    def number(self, i):
        s = self.s
        neg = s[i] == "?"
        if neg:
            i += 1
        if s[i].isdigit():
            v, i = int(s[i]) + 1, i + 1
        else:
            j = s.find("@", i)
            if j < 0:
                raise Undecorated(s)
            v = 0
            for ch in s[i:j]:
                v = v * 16 + ord(ch) - ord("A")
            i = j + 1
        return (-v if neg else v), i

    # -- types ------------------------------------------------------------------------------------------------
    def type(self, i, names, types=None):
        """((spelling, size, class pointed to, is a class), next index) of the type at s[i]; `types` is the argument list's
        back-reference table (a digit names an earlier multi-character argument type)."""
        s = self.s
        if i >= len(s):
            raise Undecorated(s)
        c = s[i]
        if types is not None and c.isdigit():
            k = int(c)
            if k >= len(types):
                raise Undecorated(s)
            return types[k], i + 1
        start = i
        if c in PRIM_CODES:
            n, size = PRIM_CODES[c]
            return (n, size, None, False), i + 1
        if c == "_" and s[i + 1:i + 2] in PRIM_EXT:
            n, size = PRIM_EXT[s[i + 1]]
            t, i = (n, size, None, False), i + 2
        elif c in "PQAR" and s[i + 1:i + 2] in ("A", "B", "C", "D"):
            if s[i + 2:i + 3] == "6":
                raise Undecorated(s)   # a function pointer
            inner, i = self.type(i + 2, names)
            const = " const" if s[start + 1] in "BD" else ""
            t = (inner[0] + const + (" &" if c in "AR" else " *"), 4, inner[0] if inner[3] else None, False)
        elif c in ("V", "U", "T"):
            parts, i = self.qualified(i + 1, names)
            name = "::".join(reversed(parts))
            size = self.lay.sizeof(name) if self.lay else None
            t = (name, size, None, True)
        elif c == "W" and s[i + 1:i + 2] in "01234567":
            parts, i = self.qualified(i + 2, names)
            t = ("::".join(reversed(parts)), 4, None, False)
        else:
            raise Undecorated(s)
        if types is not None and i - start > 1 and len(types) < 10:
            types.append(t)
        return t, i

    def params(self, i, names):
        """[(spelling, size, class)] of a parameter list at s[i] ('X' for none; ends at '@' or 'Z')."""
        if self.s[i:i + 1] == "X":
            return []
        out, types = [], []
        while i < len(self.s) and self.s[i] not in "@Z":
            if self.s.startswith("ZZ", i):
                break   # varargs
            t, i = self.type(i, names, types)
            out.append(t)
        return out


def decode(symbol):
    """(names, fragments innermost first, index after the qualified name, special code) of a decorated name, or
    None: `special` is the '??X' code of a constructor ('0'), destructor ('1'), operator or deleting destructor
    ('_G'), and None for an ordinary name."""
    if not symbol or not symbol.startswith("?"):
        return None
    d = Demangler(symbol)
    names = []
    try:
        if symbol.startswith("??"):
            if symbol[2] == "_":
                special, i = symbol[2:4], 4
            elif symbol[2] == "$" or symbol[2] == "?":
                return None
            else:
                special, i = symbol[2], 3
            if symbol[i] == "@":
                return d, names, [], i + 1, special   # a global operator (??2@YAPAXI@Z)
            parts, i = d.qualified(i, names)
            return d, names, parts, i, special
        parts, i = d.qualified(1, names)
    except (Undecorated, IndexError):
        return None
    return d, names, parts, i, None


def method_class(symbol):
    """The class of a __thiscall member function's decorated name, or None (a static member, a free function, a
    __cdecl member, a name this reader can't take)."""
    r = decode(symbol)
    if r is None:
        return None
    d, names, parts, i, special = r
    scope = parts if special else parts[1:]
    tail = symbol[i:]
    if special and special not in ("0", "1", "_G", "_E", "_D") and not special.isalnum():
        return None
    if not scope or len(tail) < 3 or tail[0] not in THISCALL_ACCESS or tail[2] != "E":
        return None
    return "::".join(reversed(scope))


def ctor_class(symbol):
    r = decode(symbol)
    if r is None or r[4] != "0" or not r[2]:
        return None
    return "::".join(reversed(r[2]))


def signature_of(symbol, lay=None):
    """(kind, return (spelling, size, class) or None, [(spelling, size, class)] parameters) of a decorated
    function name; kind is 'thiscall' (`this` in ECX, parameters on the stack) or 'stack' (__cdecl or __stdcall:
    all on the stack). None for anything else."""
    r = decode(symbol)
    if r is None:
        return None
    d, names, parts, i, special = r
    d.lay = lay
    tail, s = symbol[i:], symbol
    try:
        if tail[:1] == "Y":
            kind, conv, i = "stack", s[i + 1], i + 2
        elif tail[:1] in THISCALL_ACCESS:
            kind, conv, i = "thiscall", s[i + 2], i + 3
        elif tail[:1] in "CKS":
            kind, conv, i = "stack", s[i + 1], i + 2
        else:
            return None
        if (kind == "thiscall" and conv != "E") or (kind == "stack" and conv not in ("A", "G")):
            return None
        if special in ("0", "1") or special in ("_G", "_E", "_D"):
            if special in ("0", "1"):
                if s[i] != "@":
                    return None
                ret, i = None, i + 1
            else:
                ret, i = d.type(i, names)
        else:
            if s.startswith("?A", i):
                i += 2   # a class returned by value
            ret, i = d.type(i, names)
        return kind, ret, d.params(i, names)
    except (Undecorated, IndexError):
        return None


def param_slots(sig):
    """{stack offset at entry: class} of a signature's class-pointer (and class-reference) parameters; offset 4
    is the first stack parameter (the return address is at 0)."""
    kind, ret, ps = sig
    out, off = {}, 4
    for spelling, size, cls, _ in ps:
        if size is None:
            return out   # a by-value class of unknown size: the later offsets are unknown
        if cls:
            out[off] = cls
        off += (size + 3) & ~3
    return out


def return_class(symbol):
    """The class a function's decorated name returns a pointer (or reference) to, or None."""
    sig = signature_of(symbol)
    return sig[1][2] if sig and sig[1] else None


def data_type(symbol):
    """('obj', class) or ('ptr', class) of a decorated global or static member of class or pointer type, else
    None."""
    r = decode(symbol)
    if r is None or r[4] is not None:
        return None
    d, names, parts, i, special = r
    s = symbol
    if i >= len(s) or s[i] not in "0123":
        return None
    try:
        t, _ = d.type(i + 1, names)
    except (Undecorated, IndexError):
        return None
    if t[2]:
        return ("ptr", t[2])
    if t[3]:
        return ("obj", t[0])
    return None


class Env:
    """What the data flow knows beyond `this`: global objects and pointers of known class, and callees that return
    a typed pointer. Globals take their retail address from the matched functions' relocations."""

    def __init__(self, lay, globals_=(), returns=None, names=None, sizes=None):
        self.objects, self.pointers = {}, {}
        self.names = names or {}
        self.methods = {a: c for a, sym in self.names.items() for c in [method_class(sym)] if c and c in lay.types}
        self.small = {a: n <= 16 for a, n in (sizes or {}).items()}
        for addr, symbol in globals_:
            t = data_type(symbol)
            if t is None or t[1] not in lay.types or lay.types[t[1]]["kind"] == "enum":
                continue
            if t[0] == "obj":
                size = lay.sizeof(t[1])
                if size:
                    self.objects[addr] = (addr, t[1], size)
            else:
                self.pointers[addr] = t[1]
        self._starts = sorted(self.objects)
        self.returns = {a: c for a, c in (returns or {}).items() if c in lay.types}

    def global_at(self, addr):
        """(start, class) of the global object holding byte `addr`, or None."""
        i = bisect.bisect_right(self._starts, addr) - 1
        if i >= 0:
            start, cls, size = self.objects[self._starts[i]]
            if addr < start + size:
                return start, cls
        return None


# ---------------------------------------------------------------------------------------------------------------
# retail instructions

REG32 = ("eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi")
SUBREG = {"al": "eax", "ah": "eax", "ax": "eax", "cl": "ecx", "ch": "ecx", "cx": "ecx", "dl": "edx", "dh": "edx",
          "dx": "edx", "bl": "ebx", "bh": "ebx", "bx": "ebx", "sp": "esp", "bp": "ebp", "si": "esi", "di": "edi"}
WIDTH = {"byte": 1, "word": 2, "dword": 4, "qword": 8, "tbyte": 10, "xmmword": 16}
MEM = re.compile(r"(?:(byte|word|dword|qword|tbyte|xmmword) ptr )?(?:\w+:)?\[([^\]]+)\]")
NO_WRITE = {"cmp", "test", "push", "jmp", "ret", "nop", "fld", "fild", "fcom", "fcomp", "fcompp", "ficom", "ficomp",
            "fadd", "fsub", "fmul", "fdiv", "fsubr", "fdivr", "fiadd", "fisub", "fimul", "fidiv", "fisubr", "fidivr",
            "fst", "fstp", "fist", "fistp", "fldcw", "fnstcw", "fxch", "fucom", "fucomp", "fucompp", "fchs", "fabs",
            "fldz", "fld1", "fsqrt", "fsin", "fcos", "fpatan", "fptan", "frndint", "fscale", "f2xm1", "fyl2x",
            "faddp", "fsubp", "fmulp", "fdivp", "fsubrp", "fdivrp", "fstsw", "bt", "int3", "wait", "fwait", "fninit",
            "fldpi", "fldl2e", "fldln2", "fldlg2", "fldl2t", "ftst", "fxam", "fprem", "fprem1", "fdecstp",
            "fincstp", "ffree", "fclex", "fnclex", "sahf", "cld", "std", "clc", "stc", "leave"}
IMPLICIT = {"call": ("eax", "ecx", "edx"), "cdq": ("edx",), "cwde": ("eax",), "cbw": ("eax",), "lahf": ("eax",),
            "fnstsw": ("eax",), "rdtsc": ("eax", "edx"), "cpuid": ("eax", "ebx", "ecx", "edx"), "loop": ("ecx",),
            "pushal": REG32, "popal": REG32, "leave": ("esp", "ebp")}
STRING_OP = re.compile(r"(?:rep\w* )?(?:movs|stos|lods|scas|cmps)[bwd]")   # not movsx
MEM_READERS = {"cmp", "test", "push", "fld", "fild", "fcom", "fcomp", "ficom", "ficomp", "fadd", "fsub", "fmul",
               "fdiv", "fsubr", "fdivr", "fiadd", "fisub", "fimul", "fidiv", "fisubr", "fidivr", "fldcw", "bt",
               "call", "jmp", "lea"}
X87_INT = {"fild", "fist", "fistp", "fiadd", "fisub", "fimul", "fidiv", "fisubr", "fidivr", "ficom", "ficomp"}
X87_NOTYPE = {"fldcw", "fnstcw", "fstcw", "fldenv", "fnstenv", "fstenv", "fnsave", "frstor", "fnstsw", "fstsw",
              "fbld", "fbstp"}
ARITH = {"add", "sub", "adc", "sbb", "inc", "dec", "neg", "not", "imul", "mul", "div", "idiv", "shl", "shr", "sar",
         "sal", "rol", "ror", "xadd"}
BITOP = {"test", "and", "or", "xor", "bt", "bts", "btr", "btc"}
SIGNED_JCC = {"jl", "jle", "jg", "jge", "jnge", "jnl", "jng", "jnle"}
UNSIGNED_JCC = {"jb", "jbe", "ja", "jae", "jnae", "jnb", "jna", "jnbe", "jc", "jnc"}


def disasm_lite(code, addr):
    """[(address, size, mnemonic, operands)] from capstone's light disassembler."""
    decomp.disasm(b"", addr)   # creates the disassembler
    return list(decomp._cs.disasm_lite(bytes(code), addr))


def split_ops(ops):
    return [o.strip() for o in ops.split(",")] if ops else []


def parse_mem(op):
    """(width or None, base register or None, index or None, displacement) of a memory operand, else None."""
    m = MEM.fullmatch(op)
    if not m:
        return None
    width = WIDTH.get(m.group(1)) if m.group(1) else None
    base = index = None
    disp = 0
    for sign, term in re.findall(r"([+-]?)\s*([^\s+-]+)", m.group(2)):
        if term in REG32:
            if base is None:
                base = term
            else:
                index = term
        elif "*" in term:
            index = term
        else:
            try:
                v = int(term, 0)
            except ValueError:
                return None
            disp += -v if sign == "-" else v
    return width, base, index, disp


def reg32(op):
    return op if op in REG32 else SUBREG.get(op)


def is_imm(op):
    return re.fullmatch(r"-?(0x[0-9a-f]+|\d+)", op) is not None


def written(mnem, ops):
    """The 32-bit registers an instruction writes."""
    out = set(IMPLICIT.get(mnem, ()))
    if STRING_OP.fullmatch(mnem):
        out.update(("esi", "edi", "ecx", "eax"))
    if mnem in ("mul", "div", "idiv") or (mnem == "imul" and len(ops) == 1):
        out.update(("eax", "edx"))
    if mnem == "xchg":
        out.update(r for r in map(reg32, ops) if r)
    if ops and mnem not in NO_WRITE and not mnem.startswith("j"):
        r = reg32(ops[0])
        if r:
            out.add(r)
    if mnem == "pop" and ops:
        r = reg32(ops[0])
        if r:
            out.add(r)
    return out


# ---------------------------------------------------------------------------------------------------------------
# the data flow: which registers hold `this` or a typed pointer


def esp_change(m, ops):
    """How an instruction moves ESP (0 for most), or None when it moves it by an amount this reader doesn't
    follow (a call: the callee may pop its arguments)."""
    if m == "push":
        return -4
    if m == "pop":
        return 4
    if m in ("call", "leave", "pushal", "popal", "pushfd", "popfd", "ret", "retn"):
        return None if m not in ("pushfd", "popfd") else (-4 if m == "pushfd" else 4)
    if ops and ops[0] == "esp":
        if m in ("add", "sub") and len(ops) == 2 and is_imm(ops[1]):
            return int(ops[1], 0) * (1 if m == "add" else -1)
        return None
    return 0


def blocks(insns, start, end):
    """(leaders in order, {leader: [successor leaders]}) of a function's instructions. An indirect jump (a switch)
    has no known successors: the blocks it reaches start with nothing known."""
    addrs = [a for a, _, _, _ in insns]
    leaders = {start}
    for i, (a, n, m, o) in enumerate(insns):
        if m.startswith("j") or m in ("ret", "retn"):
            if i + 1 < len(insns):
                leaders.add(insns[i + 1][0])
            if is_imm(o) and start <= int(o, 0) < end:
                leaders.add(int(o, 0))
    leaders = sorted(l for l in leaders if l in set(addrs))
    succ = {}
    index = {a: i for i, a in enumerate(addrs)}
    for li, l in enumerate(leaders):
        stop = leaders[li + 1] if li + 1 < len(leaders) else None
        i = index[l]
        while i + 1 < len(insns) and insns[i + 1][0] != stop:
            i += 1
        a, n, m, o = insns[i]
        out = []
        target = int(o, 0) if is_imm(o) and start <= int(o, 0) < end else None
        if m == "jmp":
            if target is not None:
                out.append(target)
        elif m in ("ret", "retn"):
            pass
        elif m.startswith("j"):
            if target is not None:
                out.append(target)
            if stop is not None:
                out.append(stop)
        elif stop is not None:
            out.append(stop)
        succ[l] = [s for s in out if s in index]
    return leaders, succ, index


class Flow:
    """Forward must-analysis of the registers holding a known pointer: ('obj', class, offset, tier) for a pointer
    to byte `offset` of a class object (tier 0: `this`, tier 1: loaded from a member of known pointer type) and
    ('vptr', class, tier) for a loaded vtable pointer."""

    def __init__(self, layouts, insns, start, end, cls, env=None, params=None):
        self.lay, self.insns, self.start, self.end, self.cls = layouts, insns, start, end, cls
        self.env = env
        self.params = params or {}   # {stack offset at entry: class} of the class-pointer parameters
        self.decoded = [None] * len(insns)

    def decode(self, i):
        """(instruction, operands, memory operands, registers written, ESP change) of instruction i, decoded once
        for all the passes over it."""
        d = self.decoded[i]
        if d is None:
            ins = self.insns[i]
            m, ops = ins[2], split_ops(ins[3])
            mems = [(k, p) for k, p in ((k, parse_mem(x)) for k, x in enumerate(ops)) if p is not None]
            d = self.decoded[i] = (ins, ops, mems, written(m, ops), esp_change(m, ops))
        return d

    def step(self, state, i, visit=None):
        ins, ops, mems, out, espd = self.decode(i)
        a, n, m, o = ins
        if visit is not None:
            for i, p in mems:
                visit(ins, ops, i, p, state)
            if m == "call" and is_imm(o):
                visit.call(ins, state)
        if self.params and mems and mems[0][0] == 0 and m not in MEM_READERS:
            w, base, index, disp = mems[0][1]
            frame = state.get("_esp") if base == "esp" else state.get("_ebp") if base == "ebp" else None
            if frame is not None and index is None:
                self.clobbered.add(frame + disp)   # VC6 reuses a dead parameter's slot for a local
        new = None
        env = self.env
        if m == "mov" and len(ops) == 2 and ops[0] == "ebp" and ops[1] == "esp" and "_esp" in state:
            state = dict(state)
            state["_ebp"] = state["_esp"]   # a frame pointer: [ebp+X] reads the parameters too
        elif m == "mov" and len(ops) == 2 and ops[0] in REG32:
            src = ops[1]
            if src in REG32:
                new = state.get(src)
            elif env is not None and is_imm(src) and int(src, 0) in env.objects:
                new = ("obj", env.objects[int(src, 0)][1], 0, 1, ("global", int(src, 0)))   # a global object
            elif mems and mems[0][0] == 1:
                new = self.loaded(state, mems[0][1])
        elif m == "lea" and len(ops) == 2 and ops[0] in REG32 and mems:
            w, base, index, disp = mems[0][1]
            v = state.get(base) if base else None
            if v and v[0] == "obj" and index is None:
                new = ("obj", v[1], v[2] + disp, v[3], v[4])
        elif m in ("add", "sub") and len(ops) == 2 and ops[0] in REG32 and is_imm(ops[1]):
            v = state.get(ops[0])
            if v and v[0] == "obj":
                d = int(ops[1], 0)
                new = ("obj", v[1], v[2] + (d if m == "add" else -d), v[3], v[4])
        if out:
            state = {r: v for r, v in state.items() if r not in out}
        if "_esp" in state:
            d = espd
            if d is None:
                state = {r: v for r, v in state.items() if r != "_esp"}
            elif d:
                state = dict(state)
                state["_esp"] += d
        if "ebp" in out and "_ebp" in state and not (m == "mov" and ops == ["ebp", "esp"]):
            state = {r: v for r, v in state.items() if r != "_ebp"}
        if new is not None:
            state[ops[0]] = new
        if m == "call" and env is not None and is_imm(o) and int(o, 0) in env.returns:
            # a callee returning a typed pointer
            state["eax"] = ("obj", env.returns[int(o, 0)], 0, 1, ("return", int(o, 0)))
        return state

    def loaded(self, state, mem):
        """What a dword load from [base+disp] puts in its register: a vtable pointer or a typed pointer."""
        w, base, index, _ = mem
        if self.params and index is None and w in (None, 4):
            frame = state.get("_esp") if base == "esp" else state.get("_ebp") if base == "ebp" else None
            if frame is not None and frame + mem[3] in self.params:
                # a parameter our signature types
                return ("obj", self.params[frame + mem[3]], 0, 1, ("param", frame + mem[3]))
        if base is None and index is None and self.env is not None and mem[3] in self.env.pointers:
            return ("obj", self.env.pointers[mem[3]], 0, 1, ("global", mem[3]))   # a global pointer
        v, disp = base_of(state, mem, self.env)
        if not v or v[0] != "obj" or index is not None or w not in (None, 4):
            return None
        off = v[2] + disp
        hits = self.lay.leaves(v[1], off)
        if len(hits) != 1:
            return None
        owner, path, lt, loff, lsize, _, _ = hits[0]
        if lt is None or loff != off:
            return None
        if lt["k"] == "vptr":
            return ("vptr", v[1] if v[2] == 0 and off == 0 else lt["cls"], v[3])
        if lt["k"] == "ptr" and lt["to"]["k"] == "named" and self.lay.types.get(lt["to"]["n"], {}).get("kind") in (
                "class", "struct", "union"):
            key = ("member", owner, path, loff - lay_offset(self.lay, owner, v[1], loff), type_text(lt))
            return ("obj", lt["to"]["n"], 0, 1, key)
        return None

    def run(self, visit):
        """The fixed point, then one pass of `visit` over it. A parameter whose slot the function writes is
        dropped and the analysis run again: the slot may hold a local by then."""
        self.clobbered = set()
        fixed = self.fixpoint()
        if self.clobbered & set(self.params):
            self.params = {o: c for o, c in self.params.items() if o not in self.clobbered}
            fixed = self.fixpoint()
        leaders, IN, bounds = fixed
        for l in leaders:
            if l not in IN:
                continue
            st = dict(IN[l])
            for i in range(*bounds[l]):
                visit.after = self.insns[i + 1:i + 9]
                st = self.step(st, i, visit)

    def fixpoint(self):
        """(block leaders, {leader: state at its entry}, {leader: instruction index range})."""
        leaders, succ, index = blocks(self.insns, self.start, self.end)
        preds = defaultdict(list)
        for l, ss in succ.items():
            for s in ss:
                preds[s].append(l)
        entry_state = {"_esp": 0}
        if self.cls:
            entry_state["ecx"] = ("obj", self.cls, 0, 0, None)
        IN, OUT = {}, {}
        work = list(leaders)
        order = {l: i for i, l in enumerate(leaders)}
        bounds = {}
        for li, l in enumerate(leaders):
            bounds[l] = (index[l], index[leaders[li + 1]] if li + 1 < len(leaders) else len(self.insns))
        rounds = 0
        while work and rounds < 50 * len(leaders) + 100:
            rounds += 1
            work.sort(key=order.get)
            l = work.pop(0)
            if l == self.start:
                st = dict(entry_state)
            else:
                ins_ = [OUT[p] for p in preds[l] if p in OUT]
                if not preds[l]:
                    st = {}
                elif not ins_:
                    continue
                else:
                    st = dict(ins_[0])
                    for o in ins_[1:]:
                        st = {r: v for r, v in st.items() if o.get(r) == v}
                    if len(ins_) < len(preds[l]):
                        # a predecessor not reached yet: optimistic until it is (it will requeue this block)
                        pass
            IN[l] = st
            for i in range(*bounds[l]):
                st = self.step(st, i)
            if OUT.get(l) != st:
                OUT[l] = st
                for s in succ.get(l, ()):
                    if s not in work:
                        work.append(s)
        return leaders, IN, bounds


# ---------------------------------------------------------------------------------------------------------------
# judging an access


def base_of(state, mem, env=None):
    """(what the operand's base holds, displacement from it) of a memory operand: a tracked register, or a global
    object of known class addressed absolutely."""
    width, base, index, disp = mem
    if base:
        return state.get(base), disp
    if env is not None and index is None:
        g = env.global_at(disp)
        if g is not None:
            return ("obj", g[1], 0, 1, ("global", g[0])), disp - g[0]
    return None, disp


def judge(lay, ins, ops, opi, mem, state, after=(), env=None):
    """(member key, access record) of one memory operand on a tracked register, or (None, reason) when it is left
    out. A record is (verdict, strength, reason): verdict 'agree', 'contra' or 'skip'."""
    a, n, m, o = ins
    width, base, index, _ = mem
    v, disp = base_of(state, mem, env)
    if not v:
        return None, "stack" if base == "esp" else "absolute" if base is None else "untracked"
    if v[0] == "vptr":
        if m in ("call", "jmp", "mov", "push"):
            return ("vcall", v[1], disp, v[2]), None
        return None, "untracked"
    if index is not None:
        return None, "indexed"
    if m == "lea":
        return None, "address"
    cls, off, tier = v[1], v[2] + disp, v[3]
    if width is None:
        return None, "nowidth"
    if m in X87_NOTYPE or m.startswith("rep") or m in ("nop",):
        return None, "notype"
    hits = lay.leaves(cls, off)
    size = lay.sizeof(cls)
    if not hits and lay.has_vbases(cls):
        return None, "virtual base"
    if not hits:
        if size is not None and (off >= size or off < 0):
            # one finding per class: through a typed pointer this is mostly an array walk or a cast to a derived
            # class (weak); through `this` the class is too small (strong)
            key = ("past", cls, size)
            st = "strong" if tier == 0 and off >= 0 else "weak"
            return key, ("contra", st, "%s past sizeof (%s)" % (m, "this" if tier == 0 else "typed pointer"), tier)
        key = ("hole", cls, off)
        return key, ("contra", "strong" if tier == 0 else "medium",
                     "%s at +0x%X: no member of %s covers it" % (m, off, cls), tier)
    best = None
    if any(lsize and width > lsize and off == loff for _, _, _, loff, lsize, _, _ in hits) and \
            whole_leaves(lay, cls, off, width):
        return None, "struct copy"
    for owner, path, lt, loff, lsize, moff, msize in hits:
        r = judge_leaf(lay, m, ops, opi, width, off - loff, lt, lsize, loff, moff, msize, after)
        key = ("member", owner, path, loff - lay_offset(lay, owner, cls, loff), type_text(lt) if lt else "?")
        rank = {"agree": 0, "skip": 1, "contra": 2}[r[0]]
        if best is None or rank < best[2]:
            best = (key, r, rank)
    key, r, _ = best
    return key + ((cls, loff),), r + (tier,)


def whole_leaves(lay, cls, off, width):
    """True when bytes [off, off + width) of `cls` are whole leaves back to back (padding allowed): a wider move
    over them is a copy of several members (a struct assignment), not a wrongly sized one."""
    pos, end, count = off, off + width, 0
    while pos < end:
        hits = lay.leaves(cls, pos)
        if not hits:
            pos += 1   # padding
            continue
        lo = min(h[3] for h in hits)
        size = max((h[4] or 0) for h in hits)
        if lo != pos or not size or pos + size > end:
            return False
        pos += size
        count += 1
    return count >= 2   # one member and its padding is a wider access, not a copy


def lay_offset(lay, owner, cls, loff):
    """The offset of base `owner` inside `cls` (0 when it is the class itself or not found)."""
    if owner == cls:
        return 0
    d = lay.types.get(cls)
    if d is None:
        return 0
    for b in d["bases"]:
        if b["name"] == owner:
            return b["offset"]
        sub = lay_offset(lay, owner, b["name"], loff)
        if sub or base_chain(lay, b["name"], owner):
            return b["offset"] + sub
    return 0


def base_chain(lay, cls, owner, depth=0):
    if cls == owner:
        return True
    d = lay.types.get(cls)
    return depth < 12 and d is not None and any(base_chain(lay, b["name"], owner, depth + 1) for b in d["bases"])


def narrowed(ops, after):
    """Why a load narrower than its member is not a type error, judged from the next instructions, or None:
    'mask' when a bit operation or shift with an immediate follows on the register (VC6's narrowing of
    `member & mask` to the bytes the mask covers), 'dest' when the narrow register is stored, pushed or computed
    with at its own width (a conversion to a narrower destination: a short field, argument or local)."""
    r = ops[0] if ops else None
    if r is None or not (r in REG32 or r in SUBREG):
        return None
    whole = reg32(r)
    for a, n, m, o in after:
        xs = split_ops(o)
        if xs and reg32(xs[0]) == whole and m in ("shr", "sar", "shl", "rol", "ror"):
            return "mask"
        if m in BITOP and any(reg32(x) == whole for x in xs):
            return "mask"   # the mask may be in another register (`mov bl, 8; test bl, cl`)
        if r in SUBREG and (m == "mov" and len(xs) == 2 and xs[1] == r and parse_mem(xs[0]) or
                            m == "push" and xs == [whole] or
                            m in ("inc", "dec", "add", "sub", "neg", "imul") and xs and xs[0] == r):
            return "dest"
        if whole in written(m, xs):
            return None
    return None


def judge_leaf(lay, m, ops, opi, width, delta, lt, lsize, loff, moff, msize, after=()):
    """(verdict, strength, reason) of an access of `width` bytes at `delta` into a leaf."""
    cat = category(lt, lay.types)
    if cat is None:
        if lt is None:
            return ("contra", "medium", "%s %s into padding inside a struct member" % (m, wname(width)))
        return ("skip", None, "unknown leaf type")
    if cat == "bits":
        return ("skip", None, "bitfield")
    if cat == "empty":
        return ("skip", None, "empty class")
    if cat == "struct":
        return ("skip", None, "incomplete struct")
    x87 = m.startswith("f")
    if x87 and m in X87_INT:
        if cat == "float":
            return ("contra", "strong", "integer x87 %s %s on a float member" % (m, wname(width)))
        if delta == 0 and width == lsize:
            return ("agree", None, None)
        return ("contra", "strong", "%s %s on a %d-byte integer" % (m, wname(width), lsize))
    if x87:
        if cat == "float":
            if delta == 0 and width == lsize:
                return ("agree", None, None)
            return ("contra", "strong", "%s %s on a %d-byte float" % (m, wname(width), lsize))
        return ("contra", "strong", "float %s %s on an integer member" % (m, wname(width)))
    # integer instructions
    narrow_ok = m in BITOP and any(is_imm(x) for x in ops)
    if m in ("cmp",) and any(is_imm(x) for x in ops) and width < (lsize or 0):
        narrow_ok = narrow_ok or False
    if cat == "float":
        if m in ("movsx", "movzx"):
            return ("contra", "strong", "%s %s of a float member" % (m, wname(width)))
        if m in ARITH:
            return ("contra", "strong", "integer %s %s on a float member" % (m, wname(width)))
        if width in (4, 8) and delta in (0, 4) and width + delta <= lsize:
            return ("agree", None, None)   # a float moved as a dword
        if narrow_ok:
            return ("skip", None, "sign test of a float")
        if m == "cmp":
            return ("skip", None, "integer compare of a float")
        return ("contra", "medium", "%s %s at byte %d of a float member" % (m, wname(width), delta))
    if cat == "vptr":
        if width == 4 and delta == 0:
            return ("agree", None, None)
        return ("contra", "strong", "%s %s on the vtable pointer" % (m, wname(width)))
    signed = cat == "sint"
    if lsize == 8 and width == 4 and delta in (0, 4) and cat in ("sint", "uint"):
        return ("agree", None, None)   # an __int64 moved, compared or added in dword halves
    if width == lsize and delta == 0:
        if m == "movsx" and not signed:
            return ("contra", "strong", "movsx %s of an unsigned member" % wname(width))
        if m == "movzx" and signed:
            return ("contra", "weak", "movzx %s of a signed member" % wname(width))
        if m == "cmp" and after:
            j = after[0][2]
            if j in SIGNED_JCC and (not signed or cat == "ptr"):
                return ("contra", "medium", "cmp then signed %s: compared as signed (a cast or an int accessor?)" % j)
        return ("agree", None, None)
    if width > (lsize or 0) and delta == 0:
        # a wider move across neighbouring leaves of one struct member: a struct copy
        if moff <= loff and loff + width <= moff + msize and loff != moff or (msize > lsize and loff + width <= moff + msize):
            return ("skip", None, "struct copy")
        if m in ("mov", "push") and width == 4:
            return ("contra", "strong", "%s %s of a %d-byte member" % (m, wname(width), lsize))
        return ("contra", "strong", "%s %s of a %d-byte member" % (m, wname(width), lsize))
    if delta + width > (lsize or 0):
        if msize > (lsize or 0) and loff + delta + width <= moff + msize:
            return ("skip", None, "struct copy")
        return ("contra", "medium", "%s %s at byte %d of a %d-byte member" % (m, wname(width), delta, lsize))
    # narrower than the member, inside it
    why = narrowed(ops, after) if m in ("mov", "movzx", "movsx") and opi == 1 else None
    if narrow_ok or why == "mask":
        return ("agree", None, None)
    if why == "dest":
        return ("skip", None, "narrower destination")
    if m == "mov" and opi == 0:
        return ("contra", "strong", "store %s at byte %d of a %d-byte member" % (wname(width), delta, lsize))
    if m in ("mov", "movsx", "movzx", "cmp", "push") or m in ARITH:
        st = "strong" if m in ARITH or (m == "movsx" and delta) else "medium"
        return ("contra", st, "%s %s at byte %d of a %d-byte member" % (m, wname(width), delta, lsize))
    return ("skip", None, "narrow %s" % m)


def wname(w):
    return {1: "byte", 2: "word", 4: "dword", 8: "qword", 10: "tbyte"}.get(w, "%d-byte" % w)


# ---------------------------------------------------------------------------------------------------------------
# collecting


class Collector:
    """Accesses of every member function, aggregated by member."""

    def __init__(self, lay, env=None):
        self.lay, self.env = lay, env
        self.members = defaultdict(lambda: {"agree": 0, "contra": [], "funcs": set(), "open": set(),
                                            "matched": set(), "call_agree": 0})
        self.vcalls = defaultdict(list)
        self.left_out = Counter()
        self.counted = 0
        self.functions = 0

    def function(self, row, cls, insns, start, end, matched, params=None):
        self.functions += 1
        flow = Flow(self.lay, insns, start, end, cls, self.env, params)
        fname = row.get("name") or row["addr"]

        def visit(ins, ops, opi, mem, state):
            key, r = judge(self.lay, ins, ops, opi, mem, state, visit.after, self.env)
            if key is None:
                self.left_out[r] += 1
                return
            if key[0] == "vcall":
                self.vcalls[(key[1], key[2])].append((ins[0], fname, matched, key[3]))
                return
            self.counted += 1
            if r[0] == "skip":
                self.left_out[r[2]] += 1
                return
            e = self.members[key[:5] if key[0] == "member" else key]
            if r[0] == "agree":
                e["agree"] += 1
                return
            e["contra"].append({"site": ins[0], "ins": "%s %s" % (ins[2], ins[3]), "func": fname, "fn": start,
                                "strength": r[1], "reason": r[2], "tier": r[3] if len(r) > 3 else 0,
                                "matched": matched, "via": key[5] if key[0] == "member" else (key[1], key[2])})
            e["funcs"].add(start)
            (e["matched"] if matched else e["open"]).add(start)
        visit.after = []
        visit.call = lambda ins, state: self.call(ins, state, fname, start, matched)
        flow.run(visit)

    def call(self, ins, state, fname, start, matched):
        """A direct call to a member function with ECX on a known subobject: the callee's class must be the
        subobject's class or one of its bases."""
        target = int(ins[3], 0)
        callee = self.env.methods.get(target) if self.env is not None else None
        v = state.get("ecx")
        if callee is None or not v or v[0] != "obj":
            return
        cls, off, tier, origin = v[1], v[2], v[3], v[4]
        if off == 0 and origin is None:
            return   # `this` itself: the method's own class (a call on a cast `this` is the source's business)
        lay = self.lay
        subs = lay.subobjects(cls, off)
        if off == 0:
            if origin[0] != "member":
                return   # a parameter, global or return value: reported under the member check only
            key = origin
            what = "a %s" % cls
        else:
            held = [s for s in subs if s[1] is not None]
            if not held:
                return
            c, owner, path, moff = held[0]
            key = ("member", owner, path, moff - lay_offset(lay, owner, cls, moff), "/".join(sorted({s[0] for s in held})))
            what = "+0x%X of %s" % (off, cls)
        classes = [s[0] for s in subs]
        e = self.members[key]
        self.counted += 1
        if callee in classes or any(base_chain(lay, c, callee) for c in classes):
            e["agree"] += 1
            e["call_agree"] += 1
            return
        name = self.env.names.get(target, "0x%X" % target)
        if any(base_chain(lay, callee, c) for c in classes):
            strength, reason = "medium", "calls derived %s on %s (a cast, or the member's type is a base)" % (
                callee, what)
        elif any(c.split("<")[0] == callee.split("<")[0] and "<" in callee for c in classes):
            strength, reason = "medium", "calls %s on %s: another instance of the template" % (callee, what)
        else:
            strength, reason = "strong", "calls %s on %s" % (callee, what)
        if self.env.small.get(target):
            strength = {"strong": "medium", "medium": "weak"}[strength]
            reason += " (a small body: identical bodies fold)"
        e["contra"].append({"site": ins[0], "ins": "call %s" % name, "func": fname, "fn": start,
                            "strength": strength, "reason": reason, "tier": tier, "matched": matched,
                            "via": (cls, off)})
        e["funcs"].add(start)
        (e["matched"] if matched else e["open"]).add(start)


STRENGTH = {"strong": 3, "medium": 2, "weak": 1}


def member_findings(col, weak=False):
    out = []
    for key, e in col.members.items():
        if not e["contra"]:
            continue
        # a contradiction in a matched function is our own compiler's output for our source: a cast there
        # explains it, so it counts one grade lower than the same access in an open function
        # calls of a derived class's methods through a member are casts in the source unless they outnumber the
        # calls its own type explains: only then is the member's type itself in question
        derived = sum(1 for c in e["contra"] if c["reason"].startswith("calls derived"))
        cast = derived and e["call_agree"] >= derived
        strongest = max(STRENGTH[c["strength"]] - (1 if c["matched"] else 0) -
                        (1 if cast and c["reason"].startswith("calls derived") else 0) for c in e["contra"])
        if strongest < 2 and not weak:
            continue
        strongest = max(strongest, 1)
        # one finding per member; its evidence grouped by reason
        reasons = Counter((c["reason"], c["strength"]) for c in e["contra"])
        score = (strongest, len(e["open"]), len(e["funcs"]), len(e["contra"]) / (len(e["contra"]) + e["agree"]))
        out.append({"check": "member", "key": key, "score": score, "agree": e["agree"], "contra": e["contra"],
                    "reasons": reasons, "funcs": sorted(e["funcs"]), "open": sorted(e["open"]),
                    "matched": sorted(e["matched"]), "strength": {3: "strong", 2: "medium", 1: "weak"}[strongest]})
    return out


# ---------------------------------------------------------------------------------------------------------------
# class sizes and vtables


def allocation_sites(insns, allocators, ctors, vtables, skip_calls=(), is_code=None):
    """[(site, size, class, how)] of the objects allocated by a pushed size and constructed in place: the allocator's
    result is followed through register copies to a constructor call with it in ECX (a member's constructor, called
    on an offset into the object, doesn't count). When that constructor is a base's, the derived class's inline
    constructor stores its own vtable at the object's offset 0 afterwards, and the last such store names the class.
    `how` is 'constructor', 'inline constructor after <base>'s' or 'base constructor only' (no vtable store seen)."""
    out = []
    last_push = None   # (index, value)
    for i, (a, n, m, o) in enumerate(insns):
        if m == "push" and is_imm(o):
            last_push = (i, int(o, 0))
            continue
        if m != "call" or not is_imm(o) or int(o, 0) not in allocators:
            continue
        if last_push is None or i - last_push[0] > 6 or any(insns[k][2] == "call" for k in range(last_push[0] + 1, i)):
            continue
        size = last_push[1] & 0xFFFFFFFF
        holders = {"eax"}
        cls = base = None
        for k in range(i + 1, min(i + 120, len(insns))):
            ka, kn, km, ko = insns[k]
            if km in ("ret", "retn") or (km == "jmp" and not is_imm(ko)):
                break
            ops = split_ops(ko)
            if km == "call":
                t = int(ko, 0) if is_imm(ko) else None
                if t in skip_calls and cls is None:   # TGPool::Allocate after GetPool: its result is the object
                    holders = {"eax"}
                    continue
                if cls is None:
                    if t not in ctors or "ecx" not in holders:
                        break
                    cls = base = ctors[t]
                    holders = (holders - {"ecx", "edx"}) | {"eax"}   # a constructor returns `this`
                    continue
                holders -= {"eax", "ecx", "edx"}
                continue
            if cls is not None and km == "mov" and len(ops) == 2 and is_imm(ops[1]):
                p = parse_mem(ops[0])
                vt = int(ops[1], 0)
                if p and p[0] == 4 and p[1] in holders and p[2] is None and p[3] == 0 and vt >= 0x400000 and \
                        not (is_code and is_code(vt)):
                    cls = vtables.get(vt, "?")   # a vtable this tool doesn't know: an unknown derived class
            new = ops[0] if km == "mov" and len(ops) == 2 and ops[0] in REG32 and ops[1] in holders else None
            holders -= written(km, ops)
            if new:
                holders.add(new)
            if not holders:
                break
        if cls is None or cls == "?":
            continue
        how = "constructor" if cls == base else "inline constructor after %s's" % base
        out.append((a, size, cls, how))
    return out


def relocated_names(img, rows):
    """{retail address: decorated name} of every absolute relocation target (DIR32) in the matched functions'
    compiled objects: a matched function holds, at the same offset in retail, the target's retail address (as
    tools/trace.py reads its vtables). Gives the vtables ('??_7Class@@6B@') and the globals the data flow types."""
    import coff
    import struct
    by_unit = defaultdict(list)
    for r in rows:
        if r["status"] == "MATCH" and r.get("symbol"):
            by_unit[r["unit"]].append(r)
    out = {}
    for unit, urows in by_unit.items():
        path = decomp.BUILD / "obj" / (unit + ".obj")
        if not path.exists():
            continue
        obj = coff.Coff(path.read_bytes())
        by_name = {}
        for sym in obj.symbols.values():
            if sym.defined:
                by_name.setdefault(sym.name, sym)
        for r in urows:
            sym = by_name.get(r["symbol"])
            if sym is None:
                continue
            sec = obj.section(sym.section)
            start, end = obj.function_extent(sym)
            for off, idx, typ in sec.relocs:
                target = obj.symbols.get(idx)
                if not (start <= off < end) or target is None or typ != 6 or not target.name.startswith("?"):
                    continue
                addend = struct.unpack_from("<I", sec.data, off)[0]
                at = img.dword(int(r["addr"], 16) + off - start)
                if at is not None:
                    out.setdefault(at - addend, target.name)
    return out


def compiled_vtables(names):
    """{retail address: class} of the primary vtables among relocated_names()."""
    out = {}
    for addr, name in names.items():
        m = re.fullmatch(r"\?\?_7(\w+)@@6B@", name)
        if m:
            out[addr] = m.group(1)
    return out


def listed_vtables():
    """{class: (address, slots)} of config/vtables.txt."""
    out = {}
    path = decomp.CONFIG / "vtables.txt"
    if path.exists():
        for line in path.read_text(encoding="utf-8").splitlines():
            m = re.match(r"\s*(.+?)\s+(0x[0-9A-Fa-f]+)\s+(\d+)", line.split("#")[0])
            if m:
                out[m.group(1)] = (int(m.group(2), 16), int(m.group(3)))
    return out


def code_referenced(img):
    """Every address outside .text that an absolute relocation inside .text names: the labels of the data."""
    lo, hi = img.text[1], img.text[1] + img.text[2]
    out = set()
    for site in img.relocs:
        if lo <= site < hi:
            t = img.dword(site)
            if t is not None and not lo <= t < hi:
                out.add(t)
    return out


def vtable_length(img, addr, labels, relocs):
    lo, hi = img.text[1], img.text[1] + img.text[2]
    n = 0
    while n < 400:
        p = addr + 4 * n
        if n and p in labels:
            break
        if p not in relocs:
            break
        t = img.dword(p)
        if t is None or not lo <= t < hi:
            break
        n += 1
    return n


# ---------------------------------------------------------------------------------------------------------------


def scan(rows, claims, img, lay):
    """(member collector, size observations, vtable rows)."""
    extents = openwork.retail_extents(rows, img)
    names = {}
    for n, a, _ in claims:
        names.setdefault(a, n)
    for r in rows:
        if r.get("symbol"):
            names[int(r["addr"], 16)] = r["symbol"]
    ctors = {a: c for a, s in names.items() for c in [ctor_class(s)] if c}
    allocators = {a for a, s in names.items() if s in (
        "??2@YAPAXI@Z", "?GetPool@TGPoolManager@@QAEPAVTGPool@@I@Z", "?AllocateObject@TGPoolManager@@QAEPAXIPBDH@Z")}
    skip_calls = {a for a, s in names.items() if s == "?Allocate@TGPool@@QAEPAXPBDH@Z"}
    relocated = relocated_names(img, rows)
    vtables = compiled_vtables(relocated)
    for cls, (a, _) in listed_vtables().items():
        vtables.setdefault(a, cls)
    returns = {a: c for a, sym in names.items() for c in [return_class(sym)] if c}
    body_sizes = {int(r["addr"], 16): r.get("size") or 0 for r in rows}
    env = Env(lay, relocated.items(), returns, names, body_sizes)

    col = Collector(lay, env)
    text_lo, text_hi = img.text[1], img.text[1] + img.text[2]

    def is_code(addr):
        return text_lo <= addr < text_hi

    sizes = defaultdict(list)
    for r in rows:
        if r["status"] in ("NO-SYMBOL", "NO-OBJ"):
            continue
        a = int(r["addr"], 16)
        size = extents.get(a, r.get("size")) if r["status"] not in openwork.EXACT else r.get("size")
        if not size:
            continue
        code = img.read(a, size)
        if not code:
            continue
        insns = disasm_lite(code, a)
        for site, n, cls, how in allocation_sites(insns, allocators, ctors, vtables, skip_calls, is_code):
            sizes[cls].append((site, n, how, r.get("name") or r["addr"]))
        cls = method_class(r.get("symbol"))
        if cls is not None and cls not in lay.types:
            cls = None
        sig = signature_of(r.get("symbol"), lay)
        params = {o: c for o, c in param_slots(sig).items() if c in lay.types} if sig else {}
        if cls is None and not params:
            continue
        col.function(r, cls, insns, a, a + size, r["status"] in openwork.EXACT, params)
    return col, sizes, vtables, env


def class_related(lay, cls, wanted):
    """True when `cls` is `wanted` or derives from it, or `wanted` derives from `cls` (it holds members `wanted`
    inherits)."""
    return base_chain(lay, cls, wanted) or base_chain(lay, wanted, cls)


def derived_sizes(lay, cls):
    """The sizes of the classes our types derive from `cls` at offset 0."""
    if not hasattr(lay, "_derived"):
        lay._derived = defaultdict(set)
        for name, d in lay.types.items():
            if d["kind"] == "enum":
                continue
            seen, todo = set(), [b["name"] for b in d["bases"] if b["offset"] == 0]
            while todo:
                b = todo.pop()
                if b in seen:
                    continue
                seen.add(b)
                lay._derived[b].add(d["size"])
                bd = lay.types.get(b)
                if bd:
                    todo += [x["name"] for x in bd["bases"] if x["offset"] == 0]
    return lay._derived.get(cls, set())


def size_findings(lay, sizes, only_class=None, weak=False):
    out = []
    for cls, obs in sizes.items():
        if only_class and cls != only_class:
            continue
        ours = lay.sizeof(cls)
        if ours is None:
            continue
        # a multiple of our size through the class's own constructor: an array, constructed element by element;
        # the size of a class derived from it: that class's inline constructor (one without a vtable to store)
        derived = derived_sizes(lay, cls)
        bad = [o for o in obs if o[1] != ours and not (o[2] == "constructor" and (
            o[1] % ours == 0 or o[1] in derived))]
        if not bad:
            continue
        seen = Counter(o[1] for o in obs)
        # smaller than our sizeof: the constructor writes past the allocation (strong); larger through the class's
        # own constructor: an inline derived constructor whose vtable store wasn't seen is still possible (medium,
        # weak when the class has a vtable pointer to store and some sites agree)
        if any(o[1] < ours for o in bad):
            strength = "strong"
        elif any(o[2] != "constructor" for o in bad):
            strength = "strong"
        elif len(obs) > len(bad):
            strength = "weak"
        else:
            strength = "medium"
        if strength == "weak" and not weak:
            continue
        out.append({"check": "size", "class": cls, "ours": ours, "retail": dict(seen), "sites": bad[:2],
                    "count": len(bad), "agree": len(obs) - len(bad), "strength": strength,
                    "score": (STRENGTH[strength], len(bad), 0, 0)})
    return out


def vtable_findings(img, lay, vtables, col, only_class=None, weak=False, names=None):
    labels = code_referenced(img) | set(vtables)
    relocs = img.relocs
    listed = listed_vtables()
    out = []
    by_class = {}
    for a, cls in vtables.items():
        by_class.setdefault(cls, a)
    for cls, a in sorted(by_class.items()):
        if only_class and cls != only_class:
            continue
        if cls not in lay.types:
            continue
        ours = lay.slots(cls)
        retail = vtable_length(img, a, labels, relocs)
        lst = listed.get(cls, (None, None))[1]
        want = lst if lst is not None else retail
        if not ours or want == ours:
            continue
        # the slots one side has and the other hasn't, by the functions retail holds there: a slot past ours
        # holding a method of the class (or of a base) is a virtual our source declares non-virtual (strong)
        names = names or {}
        extra = []
        for k in range(min(ours, want), max(ours, want)):
            t = img.dword(a + 4 * k) if k < retail else None
            sym = names.get(t) if t is not None else None
            extra.append((k, t, sym))
        own = retail > ours and all(sym and (method_class(sym) == cls or base_chain(lay, cls, method_class(sym) or ""))
                                    for k, t, sym in extra)
        strength = "strong" if own or (lst is not None and lst == retail) else "medium"
        out.append({"check": "vtable", "class": cls, "address": a, "ours": ours, "retail": retail, "listed": lst,
                    "slots": [(k, t, sym) for k, t, sym in extra],
                    "strength": strength, "score": (STRENGTH[strength], abs(want - ours), 0, 0)})
    for (cls, disp), sites in col.vcalls.items():
        if only_class and cls != only_class:
            continue
        ours = lay.slots(cls)
        if cls not in lay.types or not ours or disp // 4 < ours:
            continue
        # through `this`, the slot is the method's own class's; through a typed member pointer, a cast in the
        # source to a derived class does the same (weak)
        strength = "strong" if any(s[3] == 0 for s in sites) else "weak"
        if strength == "weak" and not weak:
            continue
        out.append({"check": "vcall", "class": cls, "slot": disp // 4, "ours": ours, "sites": sites[:2],
                    "count": len(sites), "strength": strength,
                    "score": (STRENGTH[strength], len({s[1] for s in sites}), 0, 0)})
    return out


# ---------------------------------------------------------------------------------------------------------------
# output


def finding_line(f, names_of):
    if f["check"] == "member":
        _, owner, path, off, typ = f["key"][:5] if f["key"][0] == "member" else (None,) * 5
        if f["key"][0] != "member":
            kind, cls, off = f["key"]
            head = "%s (past sizeof 0x%X)" % (cls, off) if kind == "past" else "%s +0x%X (no member)" % (cls, off)
            typ = "-"
        else:
            head = "%s::%s +0x%X" % (owner, path, off)
        reasons = "; ".join("%s x%d" % (r, n) for (r, s), n in f["reasons"].most_common(3))
        ex = ", ".join("0x%X %s (%s%s)" % (c["site"], c["ins"], c["func"], "" if c["matched"] else ", open")
                       for c in f["contra"][:2])
        return "%-6s member %s  ours: %s | retail: %s | %d contra in %d fn (%d open), %d agree | e.g. %s" % (
            f["strength"], head, typ, reasons, len(f["contra"]), len(f["funcs"]), len(f["open"]), f["agree"], ex)
    if f["check"] == "size":
        ex = ", ".join("0x%X in %s (%s)" % (s[0], s[3], s[2]) for s in f["sites"])
        return "%-6s size   %s  ours: sizeof 0x%X | retail: %s | %d sites, %d agree | e.g. %s" % (
            f["strength"], f["class"], f["ours"], ", ".join("0x%X x%d" % kv for kv in sorted(f["retail"].items())),
            f["count"], f["agree"], ex)
    if f["check"] == "vtable":
        slots = ", ".join("slot %d %s" % (k, decomp.qualified(sym) or sym if sym else "0x%X" % t if t else "-")
                          for k, t, sym in f.get("slots", [])[:4])
        return "%-6s vtable %s  ours: %d slots | retail: %d at 0x%X%s | %s" % (
            f["strength"], f["class"], f["ours"], f["retail"], f["address"],
            "" if f["listed"] is None else " (config/vtables.txt: %d)" % f["listed"], slots)
    if f["check"] == "vcall":
        ex = ", ".join("0x%X (%s%s)" % (s[0], s[1], "" if s[2] else ", open") for s in f["sites"])
        return "%-6s vcall  %s  ours: %d slots | retail calls slot %d, %d sites | e.g. %s" % (
            f["strength"], f["class"], f["ours"], f["slot"], f["count"], ex)
    return str(f)


def to_json(f):
    g = {k: v for k, v in f.items() if k not in ("score", "reasons", "key", "contra", "funcs", "open", "matched")}
    if f["check"] == "member":
        k = f["key"]
        if k[0] == "member":
            g.update({"class": k[1], "member": k[2], "offset": k[3], "type": k[4]})
        else:
            g.update({"class": k[1], "member": None, "offset": k[2], "type": None, "kind": k[0]})
        g["reasons"] = [{"reason": r, "strength": s, "count": n} for (r, s), n in f["reasons"].most_common()]
        g["sites"] = [{k2: ("0x%X" % v if k2 in ("site", "fn") else v) for k2, v in c.items() if k2 != "via"}
                      for c in f["contra"]]
        g["functions"] = ["0x%X" % a for a in f["funcs"]]
        g["open"] = ["0x%X" % a for a in f["open"]]
    if "sites" in f and f["check"] != "member":
        g["sites"] = [list(s) for s in f["sites"]]
    return g


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("--class", dest="cls", help="only this class (and, for members, the classes it derives from)")
    ap.add_argument("--check", choices=("member", "size", "vtable"), action="append", help="only these checks")
    ap.add_argument("--weak", action="store_true", help="also the weak member findings")
    ap.add_argument("--top", type=int, default=0, help="print only the N strongest findings")
    ap.add_argument("--json", help="also write the findings to this file")
    ap.add_argument("--fresh", action="store_true", help="compile every unit's /Z7 copy again")
    a = ap.parse_args(argv)
    types, failed = load_types(a.fresh)
    lay = Layouts(types)
    rows, claims = openwork.load()
    img = decomp.pe.Image(decomp.target_image())
    if a.cls and a.cls not in types:
        print("no class %s in our types" % a.cls)
        return 1
    col, sizes, vtables, env = scan(rows, claims, img, lay)
    checks = a.check or ("member", "size", "vtable")
    found = []
    if "member" in checks:
        found += member_findings(col, a.weak)
    if "size" in checks:
        found += size_findings(lay, sizes, a.cls, a.weak)
    if "vtable" in checks:
        found += vtable_findings(img, lay, vtables, col, a.cls, a.weak, env.names)
    if a.cls:
        # a member finding belongs to the class that declares the member (or, for the others, the class named)
        found = [f for f in found if f["check"] != "member" or class_related(lay, f["key"][1], a.cls)]
    found.sort(key=lambda f: f["score"], reverse=True)
    per = Counter(f["check"] for f in found)
    left = sum(col.left_out.values())
    print("%d functions read: %d accesses and calls judged, %d operands left out (%s)" % (
        col.functions, col.counted, left, ", ".join("%s %d" % kv for kv in col.left_out.most_common(6))))
    print("%d allocation sites of %d classes, %d vtables known; findings: %s%s" % (
        sum(len(v) for v in sizes.values()), len(sizes), len(set(vtables.values())),
        ", ".join("%s %d" % kv for kv in sorted(per.items())) or "none",
        "" if not failed else "; %d units did not compile with /Z7" % len(failed)))
    for f in found[:a.top or None]:
        print(finding_line(f, None))
    if a.json:
        Path(a.json).write_text(json.dumps([to_json(f) for f in found], indent=1), encoding="utf-8")
    return 0


if __name__ == "__main__":
    sys.exit(main())
