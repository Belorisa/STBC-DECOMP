"""Write build/ghidra_types.json: the layout of every class, struct, union and enum the source defines, as the
compiler sees it, for tools/ghidra/ApplyStbcTypes.java.

Every compiled unit is built once more with /Z7 into build/types/ (never into build/obj/: /Z7 changes codegen, and
these objects only serve their type records), and the CodeView type records of each object's .debug$T section
(VC6 writes the 32-bit "ST" leaves: 32-bit type indices, length-prefixed names) are read back: members and their
offsets, base classes, the virtual functions each class introduces with their vtable offsets, and enumerators.
The compiler's layout includes inheritance, padding and the vtable pointer, so nothing is guessed from the headers.
Definitions are merged across units by name; a name defined with two different sizes is reported and the larger
kept.

    python tools/decomp.py build              (once, so the units compile)
    python tools/ghidra/export_types.py       then run ApplyStbcTypes.java in Ghidra on build/ghidra_types.json
"""
import concurrent.futures
import json
import os
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import coff  # noqa: E402
import decomp  # noqa: E402
import vc6  # noqa: E402

OUTDIR = ROOT / "build" / "types"

# CodeView primitive types: the low byte of a type index below 0x1000; bits 8-11 give a pointer mode.
PRIMITIVES = {
    0x03: "void", 0x10: "schar", 0x20: "uchar", 0x70: "char", 0x71: "wchar", 0x11: "short", 0x21: "ushort",
    0x72: "short", 0x73: "ushort", 0x12: "long", 0x22: "ulong", 0x74: "int", 0x75: "uint", 0x13: "longlong",
    0x23: "ulonglong", 0x76: "longlong", 0x77: "ulonglong", 0x40: "float", 0x41: "double", 0x42: "longdouble",
    0x30: "bool", 0x31: "short", 0x32: "int", 0x68: "schar", 0x69: "uchar",
}
NUMERIC = {0x8000: ("<b", 1), 0x8001: ("<h", 2), 0x8002: ("<H", 2), 0x8003: ("<i", 4), 0x8004: ("<I", 4),
           0x8009: ("<q", 8), 0x800A: ("<Q", 8)}


def numeric(buf, off):
    (v,) = struct.unpack_from("<H", buf, off)
    if v < 0x8000:
        return v, off + 2
    fmt, n = NUMERIC.get(v, ("<I", 4))
    return struct.unpack_from(fmt, buf, off + 2)[0], off + 2 + n


def pstring(buf, off):
    n = buf[off]
    return buf[off + 1:off + 1 + n].decode("latin-1"), off + 1 + n


class TypeTable:
    """The type records of one object, indexed from 0x1000."""

    def __init__(self, data):
        self.records = []
        off = 4   # signature
        while off + 4 <= len(data):
            (length,) = struct.unpack_from("<H", data, off)
            self.records.append(data[off + 2:off + 2 + length])
            off += 2 + length

    def get(self, index):
        i = index - 0x1000
        return self.records[i] if 0 <= i < len(self.records) else None

    # -- references, as a small JSON-able description --------------------------------------------------------------
    def ref(self, index, depth=0):
        if index < 0x1000:
            mode, base = (index >> 8) & 0xF, index & 0xFF
            prim = {"k": "prim", "n": PRIMITIVES.get(base, "undefined%d" % 4)}
            return {"k": "ptr", "to": prim} if mode else prim
        rec = self.get(index)
        if rec is None or depth > 12:
            return {"k": "prim", "n": "undefined4"}
        (leaf,) = struct.unpack_from("<H", rec, 0)
        if leaf == 0x1001:   # LF_MODIFIER: const/volatile
            return self.ref(struct.unpack_from("<I", rec, 2)[0], depth + 1)
        if leaf == 0x1002:   # LF_POINTER
            utype, attr = struct.unpack_from("<II", rec, 2)
            return {"k": "ptr", "to": self.ref(utype, depth + 1)}
        if leaf == 0x1003:   # LF_ARRAY
            elem, _ = struct.unpack_from("<II", rec, 2)
            size, _ = numeric(rec, 10)
            return {"k": "arr", "of": self.ref(elem, depth + 1), "bytes": size}
        if leaf in (0x1004, 0x1005, 0x1006, 0x1007):
            return {"k": "named", "n": self.name_of(rec)}
        if leaf in (0x1008, 0x1009):   # a function: used through pointers
            return {"k": "func", "sig": self.signature(rec, depth + 1)}
        if leaf == 0x1205:   # LF_BITFIELD: the underlying type
            return self.ref(struct.unpack_from("<I", rec, 2)[0], depth + 1)
        return {"k": "prim", "n": "undefined4"}

    def name_of(self, rec):
        (leaf,) = struct.unpack_from("<H", rec, 0)
        if leaf in (0x1004, 0x1005):
            _, off = numeric(rec, 18)
            return pstring(rec, off)[0]
        if leaf == 0x1006:
            _, off = numeric(rec, 10)
            return pstring(rec, off)[0]
        if leaf == 0x1007:
            return pstring(rec, 14)[0]
        return None

    def signature(self, rec, depth=0):
        (leaf,) = struct.unpack_from("<H", rec, 0)
        if leaf == 0x1009:   # LF_MFUNCTION
            rv, cls, this, call, _, count, arglist = struct.unpack_from("<IIIBBHI", rec, 2)
        else:                # LF_PROCEDURE
            rv, call, _, count, arglist = struct.unpack_from("<IBBHI", rec, 2)
            this = 0
        args = []
        al = self.get(arglist)
        if al is not None:
            (n,) = struct.unpack_from("<I", al, 2)
            args = [self.ref(t, depth + 1) for t in struct.unpack_from("<%dI" % n, al, 6)
                    if t != 0x0003]
        # The 'this' pointer is the first argument Ghidra sees: a __thiscall definition passes its first parameter
        # in ECX, so without it every argument would shift by one.
        if this:
            args.insert(0, self.ref(this, depth + 1))
        return {"ret": self.ref(rv, depth + 1), "args": args, "this": this != 0,
                "call": {0: "__cdecl", 7: "__stdcall", 11: "__thiscall", 4: "__fastcall"}.get(call, "__cdecl")}

    # -- definitions ---------------------------------------------------------------------------------------------
    def fields(self, index):
        """The subrecords of a field list, following LF_INDEX continuations."""
        out = []
        while index:
            rec = self.get(index)
            index = 0
            if rec is None:
                break
            off = 2
            while off < len(rec):
                if rec[off] >= 0xF0:   # LF_PAD
                    off += rec[off] & 0x0F
                    continue
                (leaf,) = struct.unpack_from("<H", rec, off)
                p = off + 2
                if leaf == 0x1400:     # LF_BCLASS
                    attr, t = struct.unpack_from("<HI", rec, p)
                    boff, p = numeric(rec, p + 6)
                    out.append(("base", t, boff, None, attr))
                elif leaf in (0x1401, 0x1402):   # virtual bases: recorded, not laid out
                    attr, t, _ = struct.unpack_from("<HII", rec, p)
                    _, p = numeric(rec, p + 10)
                    _, p = numeric(rec, p)
                    out.append(("vbase", t, 0, None, attr))
                elif leaf == 0x0403:   # LF_ENUMERATE
                    attr = struct.unpack_from("<H", rec, p)[0]
                    value, p = numeric(rec, p + 2)
                    name, p = pstring(rec, p)
                    out.append(("enum", 0, value, name, attr))
                elif leaf == 0x1404:   # LF_INDEX: the list continues in another record
                    index = struct.unpack_from("<I", rec, p + 2)[0]
                    p += 6
                elif leaf == 0x1405:   # LF_MEMBER
                    attr, t = struct.unpack_from("<HI", rec, p)
                    moff, p = numeric(rec, p + 6)
                    name, p = pstring(rec, p)
                    out.append(("member", t, moff, name, attr))
                elif leaf == 0x1406:   # LF_STMEMBER
                    attr, t = struct.unpack_from("<HI", rec, p)
                    name, p = pstring(rec, p + 6)
                    out.append(("static", t, 0, name, attr))
                elif leaf == 0x1407:   # LF_METHOD: an overload set
                    count, mlist = struct.unpack_from("<HI", rec, p)
                    name, p = pstring(rec, p + 6)
                    for attr, t, vboff in self.method_list(mlist, count):
                        out.append(("method", t, vboff, name, attr))
                elif leaf == 0x1408:   # LF_NESTTYPE
                    _, t = struct.unpack_from("<HI", rec, p)
                    _, p = pstring(rec, p + 6)
                elif leaf == 0x1409:   # LF_VFUNCTAB
                    t = struct.unpack_from("<I", rec, p + 2)[0]
                    out.append(("vfptr", t, 0, None, 0))
                    p += 6
                elif leaf == 0x140B:   # LF_ONEMETHOD
                    attr, t = struct.unpack_from("<HI", rec, p)
                    p += 6
                    vboff = None
                    if (attr >> 2) & 7 in (4, 6):
                        vboff = struct.unpack_from("<I", rec, p)[0]
                        p += 4
                    name, p = pstring(rec, p)
                    out.append(("method", t, vboff, name, attr))
                elif leaf == 0x1403 or leaf == 0x140A:   # friends
                    p += 6
                    if leaf == 0x1403:
                        _, p = pstring(rec, p)
                elif leaf == 0x140C:   # LF_VFUNCOFF
                    p += 10
                else:
                    break   # an unknown leaf: the rest of this list can't be read
                off = p
        return out

    def method_list(self, index, count):
        rec = self.get(index)
        out, p = [], 2
        while rec is not None and p < len(rec) and len(out) < count:
            attr, _, t = struct.unpack_from("<HHI", rec, p)
            p += 8
            vboff = None
            if (attr >> 2) & 7 in (4, 6):
                vboff = struct.unpack_from("<I", rec, p)[0]
                p += 4
            out.append((attr, t, vboff))
        return out

    def definitions(self):
        """Every complete class/struct/union/enum of the object, by name."""
        out = {}
        for i, rec in enumerate(self.records):
            if len(rec) < 2:
                continue
            (leaf,) = struct.unpack_from("<H", rec, 0)
            if leaf in (0x1004, 0x1005, 0x1006):
                count, prop = struct.unpack_from("<HH", rec, 2)
                if prop & 0x80:   # forward reference
                    continue
                if leaf == 0x1006:
                    field = struct.unpack_from("<I", rec, 6)[0]
                    size, off = numeric(rec, 10)
                else:
                    field = struct.unpack_from("<I", rec, 6)[0]
                    size, off = numeric(rec, 18)
                name = pstring(rec, off)[0]
                if not name or name.startswith("<unnamed") or name == "__unnamed":
                    continue
                out[name] = self.composite(leaf, size, field)
            elif leaf == 0x1007:
                count, prop, utype, field = struct.unpack_from("<HHII", rec, 2)
                if prop & 0x80:
                    continue
                name = pstring(rec, 14)[0]
                if not name or name.startswith("<unnamed"):
                    continue
                values = [{"name": n, "value": v} for kind, _, v, n, _ in self.fields(field) if kind == "enum"]
                out[name] = {"kind": "enum", "size": 4, "values": values}
        return out

    def composite(self, leaf, size, field):
        d = {"kind": {0x1004: "class", 0x1005: "struct", 0x1006: "union"}[leaf], "size": size,
             "bases": [], "fields": [], "virtuals": [], "vfptr": False}
        for kind, t, off, name, attr in self.fields(field):
            if kind == "base":
                rec = self.get(t)
                if rec is not None:
                    d["bases"].append({"name": self.name_of(rec), "offset": off})
            elif kind == "member":
                d["fields"].append({"name": name, "offset": off, "type": self.ref(t)})
            elif kind == "vfptr":
                d["vfptr"] = True
            elif kind == "method" and off is not None:   # a virtual this class introduces, at its vtable offset
                rec = self.get(t)
                sig = self.signature(rec) if rec is not None else None
                d["virtuals"].append({"name": name, "offset": off, "sig": sig})
        return d


# ---------------------------------------------------------------------------
# the layout Ghidra gets: bases flattened, one vtable structure per class

def vtable(name, types, seen=()):
    """{offset: (method, signature)} of a class's vtable: its primary base's (the base at offset 0) with the
    virtuals the class introduces added at their offsets."""
    d = types.get(name)
    if d is None or d.get("kind") == "enum" or name in seen:
        return {}
    slots = {}
    for b in d["bases"]:
        if b["offset"] == 0:
            slots.update(vtable(b["name"], types, seen + (name,)))
    for v in d["virtuals"]:
        slots.setdefault(v["offset"], (v["name"], v["sig"]))
    return slots


def flatten(name, types, seen=()):
    """[(offset, name, type)] of a class with its bases' members inlined at the base's offset. A class's own vtable
    pointer, and the one its primary base chain holds at offset 0, is the field 'vftable'."""
    d = types.get(name)
    if d is None or d.get("kind") == "enum" or name in seen:
        return []
    out = []
    for b in d["bases"]:
        for off, n, t in flatten(b["name"], types, seen + (name,)):
            if b["offset"] and n == "vftable":
                n = "vftable_" + b["name"]   # a secondary base's vtable pointer
            out.append((off + b["offset"], n, t))
    if d["vfptr"] and not any(off == 0 and n == "vftable" for off, n, _ in out):
        out.append((0, "vftable", {"k": "prim", "n": "undefined4"}))
    out += [(f["offset"], f["name"], f["type"]) for f in d["fields"]]
    return out


def layouts(types):
    out = {}
    for name, d in types.items():
        if d["kind"] == "enum":
            out[name] = d
            continue
        fields = flatten(name, types)
        slots = vtable(name, types)
        if slots:
            out[name + "_vftable"] = {"kind": "struct", "size": max(slots) + 4,
                                      "fields": [{"offset": off, "name": n, "type": {"k": "ptr", "to": {"k": "func", "sig": s}}}
                                                 for off, (n, s) in sorted(slots.items())]}
            fields = [(off, n, {"k": "ptr", "to": {"k": "named", "n": name + "_vftable"}})
                      if off == 0 and n == "vftable" else (off, n, t) for off, n, t in fields]
        out[name] = {"kind": "union" if d["kind"] == "union" else "struct", "size": d["size"],
                     "fields": [{"offset": off, "name": n, "type": t} for off, n, t in sorted(fields, key=lambda f: f[0])]}
    return out


def compile_unit(unit, path):
    obj = OUTDIR / (unit + ".obj")
    obj.parent.mkdir(parents=True, exist_ok=True)
    if obj.exists():
        obj.unlink()
    args = ["/c", *decomp.cl_args(unit, path, pch=obj.with_suffix(".pch")), "/Z7", "/Fo" + str(obj)]
    pch = decomp.fresh_pch(args)
    rc, out = vc6.run("cl", args, cwd=str(obj.parent),
                      compiler=decomp.compiler_of(path))
    decomp.drop_pch(pch)
    return unit, obj if rc == 0 and obj.exists() else None


def build_objects(units=None, stale_only=False):
    """[(unit, /Z7 object or None)] of every compiled unit (or of `units`), compiled into build/types/. With
    `stale_only`, a unit whose /Z7 object is newer than its build/obj/ object (the gated compile) is not
    compiled again."""
    units = units or decomp.units_to_compile(decomp.source_files())

    def one(u):
        obj = OUTDIR / (u + ".obj")
        if stale_only and obj.exists():
            gated = decomp.BUILD / "obj" / (u + ".obj")
            if gated.exists() and obj.stat().st_mtime >= gated.stat().st_mtime:
                return u, obj
        return compile_unit(u, units[u])
    with concurrent.futures.ThreadPoolExecutor(max_workers=os.cpu_count()) as ex:
        return list(ex.map(one, sorted(units)))


def debug_types(obj, table=TypeTable):
    """The TypeTable (or `table` subclass) of each CodeView type section of a /Z7 object."""
    c = coff.Coff(obj.read_bytes())
    for s in c.sections:
        if s.name == ".debug$T" and len(s.data) >= 4 and struct.unpack_from("<I", s.data, 0)[0] == 2:
            yield table(s.data)


def read_types(objs, table=TypeTable):
    """({name: definition}, {name: sizes}) merged over the objects by name: a name defined with two different
    sizes is a conflict, and the larger definition is kept."""
    types, conflicts = {}, {}
    for unit, obj in objs:
        if obj is None:
            continue
        for tt in debug_types(obj, table):
            for name, d in tt.definitions().items():
                old = types.get(name)
                if old is None:
                    types[name] = d
                elif old["size"] != d["size"]:
                    conflicts.setdefault(name, set()).update((old["size"], d["size"]))
                    if d["size"] > old["size"]:
                        types[name] = d
    return types, conflicts


def main():
    objs = build_objects()
    failed = [u for u, o in objs if o is None]
    types, conflicts = read_types(objs)
    out = ROOT / "build" / "ghidra_types.json"
    out.write_text(json.dumps({"types": layouts(types)}, indent=0, sort_keys=True), encoding="utf-8")
    kinds = {}
    for d in types.values():
        kinds[d["kind"]] = kinds.get(d["kind"], 0) + 1
    print("%d types written to %s (%s)" % (len(types), out, ", ".join("%d %s" % (n, k) for k, n in sorted(kinds.items()))))
    if failed:
        print("%d units did not compile with /Z7: %s" % (len(failed), ", ".join(failed[:10])))
    for name, sizes in sorted(conflicts.items()):
        print("size conflict: %s %s" % (name, sorted(sizes)))


if __name__ == "__main__":
    main()
