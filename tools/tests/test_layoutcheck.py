"""Tests of layoutcheck.py: the decorated-name reader, member lookup, the access rules, the data flow on assembled
code, the allocation-site scan and the vtable length.

    python -m unittest discover -s tools/tests
"""

import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import layoutcheck as L  # noqa: E402


def prim(n):
    return {"k": "prim", "n": n}


def ptr(to):
    return {"k": "ptr", "to": {"k": "named", "n": to}}


def cls(size, fields, bases=(), vfptr=False, virtuals=()):
    return {"kind": "class", "size": size, "bases": [{"name": b, "offset": o} for b, o in bases],
            "fields": [{"name": n, "offset": o, "type": t} for n, o, t in fields], "vfptr": vfptr,
            "virtuals": [{"name": n, "offset": o, "sig": None} for n, o in virtuals]}


TYPES = {
    "Base": cls(0x8, [("count", 4, prim("int"))], vfptr=True, virtuals=[("~Base", 0)]),
    "Point": cls(0xC, [("x", 0, prim("float")), ("y", 4, prim("float")), ("z", 8, prim("float"))]),
    "Pair": cls(0x4, [("lo", 0, prim("ushort")), ("hi", 2, prim("ushort"))]),
    "Foo": cls(0x30, [("flags", 8, prim("uint")), ("speed", 0xC, prim("float")), ("marker", 0x10, prim("ushort")),
                      ("pos", 0x14, {"k": "named", "n": "Point"}), ("pair", 0x20, {"k": "named", "n": "Pair"}),
                      ("other", 0x24, ptr("Point")), ("big", 0x28, prim("longlong"))],
               bases=[("Base", 0)], virtuals=[("Run", 4)]),
    "Empty": cls(1, []),
}


class DecoratedNames(unittest.TestCase):
    def test_method_class(self):
        self.assertEqual(L.method_class("?Update@ShipClass@@UAEXM@Z"), "ShipClass")
        self.assertEqual(L.method_class("??0Foo@@QAE@ABV0@@Z"), "Foo")
        self.assertEqual(L.method_class("??_GFoo@@UAEPAXI@Z"), "Foo")
        self.assertEqual(L.method_class("??4?$NiPointer@VNiNode@@@@QAEAAV0@PAVNiNode@@@Z"), "NiPointer<NiNode>")
        self.assertEqual(L.method_class("?IsKeysEqual@?$NiTMap@PBDV?$NiPointer@VNiImage@@@@@@MAE_NPBD0@Z"),
                         "NiTMap<char const *,NiPointer<NiImage> >")
        self.assertIsNone(L.method_class("?Cast@ShipClass@@SAPAV1@PAX@Z"))   # static
        self.assertIsNone(L.method_class("?F@@YAHH@Z"))                       # free function
        self.assertIsNone(L.method_class("?Print@Foo@@QAAXHZZ"))              # __cdecl varargs member

    def test_ctor_class(self):
        self.assertEqual(L.ctor_class("??0?$NiTList@PAD@@QAE@I@Z"), "NiTList<char *>")
        self.assertIsNone(L.ctor_class("??1Foo@@QAE@XZ"))

    def test_signature(self):
        kind, ret, ps = L.signature_of("?SetTarget@X@@QAEXPAVShipClass@@ABVNiPoint3@@0@Z")
        self.assertEqual(kind, "thiscall")
        self.assertEqual([p[2] for p in ps], ["ShipClass", "NiPoint3", "ShipClass"])   # 0: a type back-reference
        self.assertEqual(L.param_slots((kind, ret, ps)), {4: "ShipClass", 8: "NiPoint3", 12: "ShipClass"})
        kind, ret, ps = L.signature_of("?F@@YAHPAUS@@NPAU1@@Z")
        self.assertEqual(L.param_slots((kind, ret, ps)), {4: "S", 16: "S"})   # the double takes 8 bytes
        self.assertEqual(L.return_class("?GetShip@Foo@@QAEPAVShipClass@@XZ"), "ShipClass")
        self.assertEqual(L.signature_of("??2@YAPAXI@Z")[0], "stack")

    def test_data_type(self):
        self.assertEqual(L.data_type("?g_kPoolManager@@3VTGPoolManager@@A"), ("obj", "TGPoolManager"))
        self.assertEqual(L.data_type("?pools@TGPoolManager@@2PAVTGPool@@A"), ("ptr", "TGPool"))
        self.assertIsNone(L.data_type("?count@@3HA"))


class Lookup(unittest.TestCase):
    def setUp(self):
        self.lay = L.Layouts(TYPES)

    def test_leaves(self):
        (owner, path, lt, loff, lsize, moff, msize), = self.lay.leaves("Foo", 0x18)
        self.assertEqual((owner, path, loff, lsize), ("Foo", "pos.y", 0x18, 4))
        self.assertEqual(self.lay.leaves("Foo", 4)[0][:2], ("Base", "count"))   # inherited
        self.assertEqual(self.lay.leaves("Foo", 0)[0][2]["k"], "vptr")
        self.assertEqual(self.lay.leaves("Foo", 0x12), [])                      # padding

    def test_subobjects_and_slots(self):
        subs = {s[0] for s in self.lay.subobjects("Foo", 0x14)}
        self.assertEqual(subs, {"Point"})
        self.assertIn("Base", {s[0] for s in self.lay.subobjects("Foo", 0)})
        self.assertEqual(self.lay.slots("Foo"), 2)


def judge(lay, ins_text, width, after=()):
    m, _, o = ins_text.partition(" ")
    ops = L.split_ops(o)
    opi = next(i for i, x in enumerate(ops) if L.parse_mem(x))
    mem = L.parse_mem(ops[opi])
    state = {mem[1]: ("obj", "Foo", 0, 0, None)}
    return L.judge(lay, (0x1000, 3, m, o), ops, opi, mem, state, after)


class Rules(unittest.TestCase):
    def setUp(self):
        self.lay = L.Layouts(TYPES)

    def verdict(self, text, after=()):
        key, r = judge(self.lay, text, None, after)
        if key is None:
            return r
        return r[2] if r[0] == "skip" else r[:2]

    def test_parse_mem(self):
        self.assertEqual(L.parse_mem("dword ptr [esi + 0x14]"), (4, "esi", None, 0x14))
        self.assertEqual(L.parse_mem("byte ptr [eax + ecx*4 - 8]"), (1, "eax", "ecx*4", -8))
        self.assertEqual(L.parse_mem("word ptr [0x9a1234]"), (2, None, None, 0x9A1234))
        self.assertIsNone(L.parse_mem("eax"))

    def test_float_against_integer(self):
        self.assertEqual(self.verdict("fld dword ptr [ecx + 0xc]"), ("agree", None))
        self.assertEqual(self.verdict("mov eax, dword ptr [ecx + 0xc]"), ("agree", None))   # a float copied
        self.assertEqual(self.verdict("fld dword ptr [ecx + 8]"), ("contra", "strong"))
        self.assertEqual(self.verdict("fild dword ptr [ecx + 0xc]"), ("contra", "strong"))
        self.assertEqual(self.verdict("inc dword ptr [ecx + 0xc]"), ("contra", "strong"))

    def test_width_and_sign(self):
        self.assertEqual(self.verdict("movzx eax, word ptr [ecx + 0x10]"), ("agree", None))
        self.assertEqual(self.verdict("movsx eax, word ptr [ecx + 0x10]"), ("contra", "strong"))
        self.assertEqual(self.verdict("mov eax, dword ptr [ecx + 0x10]"), ("contra", "strong"))  # lone + padding
        self.assertEqual(self.verdict("mov byte ptr [ecx + 8], al"), ("contra", "strong"))       # narrow store
        self.assertEqual(self.verdict("test byte ptr [ecx + 9], 0x80"), ("agree", None))         # narrowing
        self.assertEqual(self.verdict("mov eax, dword ptr [ecx + 0x20]"), "struct copy")         # lo and hi
        self.assertEqual(self.verdict("mov eax, dword ptr [ecx + 0x2c]"), ("agree", None))       # __int64 half

    def test_narrowed_loads(self):
        mask = [(0x1003, 2, "and", "al, 1")]
        dest = [(0x1003, 4, "mov", "word ptr [edx + 8], ax")]
        self.assertEqual(self.verdict("mov al, byte ptr [ecx + 8]", mask), ("agree", None))
        self.assertEqual(self.verdict("mov ax, word ptr [ecx + 8]", dest), "narrower destination")
        self.assertEqual(self.verdict("mov al, byte ptr [ecx + 8]"), ("contra", "medium"))

    def test_holes(self):
        key, r = judge(self.lay, "mov eax, dword ptr [ecx + 0x30]", None)
        self.assertEqual((key[0], r[1]), ("past", "strong"))
        key, r = judge(self.lay, "mov ax, word ptr [ecx + 0x12]", None)
        self.assertEqual((key[0], r[1]), ("hole", "strong"))


def assemble(*chunks):
    return bytes(b for c in chunks for b in c)


class DataFlow(unittest.TestCase):
    def run_flow(self, code, cls="Foo", params=None):
        lay = L.Layouts(TYPES)
        col = L.Collector(lay, L.Env(lay))
        insns = L.disasm_lite(code, 0x1000)
        col.function({"addr": "0x1000", "name": "F"}, cls, insns, 0x1000, 0x1000 + len(code), False, params)
        return col

    def test_this_copies_and_typed_pointers(self):
        code = assemble(b"\x8b\xf1",              # mov esi, ecx
                        b"\x0f\xbf\x46\x10",      # movsx eax, word ptr [esi+0x10]   (unsigned short: strong)
                        b"\x8b\x46\x24",          # mov eax, [esi+0x24]              (Point *)
                        b"\xdb\x40\x04",          # fild dword ptr [eax+4]           (Point::y is a float)
                        b"\xc3")
        col = self.run_flow(code)
        reasons = {k[2]: [c["reason"] for c in e["contra"]] for k, e in col.members.items() if e["contra"]}
        self.assertIn("marker", reasons)
        self.assertIn("y", reasons)
        self.assertEqual(col.members[("member", "Foo", "other", 0x24, "Point *")]["agree"], 1)

    def test_a_call_ends_ecx(self):
        code = assemble(b"\xe8\x00\x00\x00\x00",  # call next
                        b"\x8b\x41\x08",          # mov eax, [ecx+8]: ECX is no longer `this`
                        b"\xc3")
        col = self.run_flow(code)
        self.assertEqual(col.counted, 0)
        self.assertEqual(col.left_out["untracked"], 1)

    def test_join_keeps_only_agreement(self):
        code = assemble(b"\x85\xc0",              # test eax, eax
                        b"\x74\x02",              # je +2
                        b"\x8b\xf1",              # mov esi, ecx   (one path only)
                        b"\x8b\x46\x08",          # mov eax, [esi+8]: esi is `this` on one path only
                        b"\xc3")
        col = self.run_flow(code)
        self.assertEqual(col.counted, 0)

    def test_parameters_and_reused_slots(self):
        code = assemble(b"\x8b\x44\x24\x04",      # mov eax, [esp+4]: the Point * parameter
                        b"\xdb\x40\x08",          # fild dword ptr [eax+8]
                        b"\xc3")
        col = self.run_flow(code, cls=None, params={4: "Point"})
        self.assertTrue(any(e["contra"] for e in col.members.values()))
        code = assemble(b"\x89\x4c\x24\x04",      # mov [esp+4], ecx: the slot reused
                        b"\x8b\x44\x24\x04",      # mov eax, [esp+4]
                        b"\xdb\x40\x08",          # fild dword ptr [eax+8]
                        b"\xc3")
        col = self.run_flow(code, cls=None, params={4: "Point"})
        self.assertFalse(any(e["contra"] for e in col.members.values()))


class Allocations(unittest.TestCase):
    def test_constructor_and_inline_derived(self):
        new, ctor, vt = 0x2000, 0x3000, 0x900000
        code = assemble(b"\x6a\x30",                                   # push 0x30
                        b"\xe8" + (new - 0x1007).to_bytes(4, "little", signed=True),
                        b"\x8b\xf0",                                   # mov esi, eax
                        b"\x8b\xce",                                   # mov ecx, esi
                        b"\xe8" + (ctor - 0x1010).to_bytes(4, "little", signed=True),
                        b"\xc7\x06" + vt.to_bytes(4, "little"),        # mov dword ptr [esi], vt
                        b"\xc3")
        insns = L.disasm_lite(code, 0x1000)
        sites = L.allocation_sites(insns, {new}, {ctor: "Base"}, {vt: "Foo"}, is_code=lambda a: a < 0x800000)
        self.assertEqual(sites, [(0x1002, 0x30, "Foo", "inline constructor after Base's")])
        sites = L.allocation_sites(insns, {new}, {ctor: "Base"}, {}, is_code=lambda a: a < 0x800000)
        self.assertEqual(sites, [])   # an unknown vtable: an unknown derived class

    def test_size_findings(self):
        lay = L.Layouts(TYPES)
        ok = L.size_findings(lay, {"Point": [(1, 0xC, "constructor", "F"), (2, 0x30, "constructor", "F")]})
        self.assertEqual(ok, [])   # an array of four
        bad = L.size_findings(lay, {"Foo": [(1, 0x2C, "constructor", "F")]})
        self.assertEqual(bad[0]["strength"], "strong")


class FakeImage:
    def __init__(self, base, words, text):
        self.base, self.words, self.text = base, words, text

    def dword(self, va):
        i = (va - self.base) // 4
        return self.words[i] if 0 <= i < len(self.words) else None


class Vtables(unittest.TestCase):
    def test_length_stops_at_a_label(self):
        img = FakeImage(0x900000, [0x401000, 0x401010, 0x401020, 0x401030, 0], (".text", 0x401000, 0x1000))
        relocs = {0x900000, 0x900004, 0x900008, 0x90000C}
        self.assertEqual(L.vtable_length(img, 0x900000, {0x900000}, relocs), 4)
        self.assertEqual(L.vtable_length(img, 0x900000, {0x900008}, relocs), 2)


if __name__ == "__main__":
    unittest.main()
