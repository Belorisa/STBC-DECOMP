"""Tests of vtslots.py: VC6's number encoding and vcall-thunk names, the member-pointer reading of the probe's
template, the slot map (overload groups, overrides, destructors), the slot verdicts, and (with tools/vc6) a
synthetic class compiled with /Z7 and probed, its CodeView slots set against the thunks.

    python -m unittest discover -s tools/tests
"""

import json
import shutil
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "ghidra"))
import vtslots as V  # noqa: E402


class Names(unittest.TestCase):
    def test_number_encoding(self):
        for n, text in ((0, "A@"), (1, "0"), (4, "3"), (10, "9"), (12, "M@"), (16, "BA@"), (0x24, "CE@"),
                        (0x158, "BFI@")):
            self.assertEqual(V.encode_number(n), text)

    def test_thunk_offset(self):
        # read from VC6's output for TGObject (GetObjType is slot 1, GetClassName slot 9)
        self.assertEqual(V.thunk_offset("??_9@$B3AE"), 4)
        self.assertEqual(V.thunk_offset("??_9@$B7AE"), 8)
        self.assertEqual(V.thunk_offset("??_9@$BM@AE"), 12)
        self.assertEqual(V.thunk_offset("??_9@$BCE@AE"), 0x24)
        self.assertEqual(V.thunk_offset("??_9@$BBFI@AE"), 0x158)
        self.assertEqual(V.thunk_offset("??_9@$BA@AE"), 0)
        self.assertIsNone(V.thunk_offset("?GetObjType@TGObject@@UAEHXZ"))
        for n in (0, 4, 0x24, 0x158, 0x400):
            self.assertEqual(V.thunk_offset("??_9@$B%sAE" % V.encode_number(n)), n)

    def test_take_pmf_gives_the_method_name(self):
        owner, mangled, rest = V.take_pmf("?vtslots_take@@YAXP8TGObject@@AE_NPAVTGStream@@@Z@Z")
        self.assertEqual(owner, "TGObject")
        self.assertEqual(mangled, "TGObject@@")
        self.assertIn("?WriteToStream@TGObject@@UAE_NPAVTGStream@@@Z", V.method_symbols("WriteToStream", mangled, rest))
        owner, mangled, rest = V.take_pmf("?vtslots_take@@YAXP8Inner@Outer@@AEXH@Z@Z")
        self.assertEqual(owner, "Outer::Inner")
        self.assertIn("?Go@Inner@Outer@@UAEXH@Z", V.method_symbols("Go", mangled, rest))
        self.assertIsNone(V.take_pmf("?other@@YAXP8TGObject@@AEHXZ@Z"))

    def test_vtable_names(self):
        self.assertEqual(V.vtable_name("??_7TGObject@@6B@"), ("TGObject", None))
        self.assertEqual(V.vtable_name("??_7Torpedo@@6BPhysicsObjectClass@@@"), ("Torpedo", "PhysicsObjectClass"))
        self.assertIsNone(V.vtable_name("?GetObjType@TGObject@@UAEHXZ"))

    def test_symbol_method(self):
        self.assertEqual(V.symbol_method("?GetObjType@TGObject@@UAEHXZ"), ("TGObject", "GetObjType"))
        self.assertEqual(V.symbol_method("??_GTGObject@@UAEPAXI@Z"), ("TGObject", "~"))
        self.assertEqual(V.symbol_method("??8NiPoint3@@QBEHABV0@@Z"), ("NiPoint3", "operator=="))

    def test_bool_variants(self):
        self.assertEqual(V.bool_variants("void (A::*)(int)"), ["void (A::*)(int)"])
        got = V.bool_variants("void (A::*)(unsigned char, unsigned char)")
        self.assertEqual(len(got), 4)
        self.assertIn("void (A::*)(bool, unsigned char)", got)

    def test_class_spellings(self):
        self.assertEqual(V.norm_class("NiTMap<const_char*,X *>"), V.norm_class("NiTMap<char const *,X *>"))
        self.assertTrue(V.same_name("GetClassName", "GetClassNameA"))
        self.assertFalse(V.same_name("GetClassName", "GetClass"))


def method(name, mode, voff=None, args="[]", pmf=None):
    return (name, mode, voff, args, 3, pmf)


def cdef(bases=(), methods=(), vfptr=False):
    return {"bases": list(bases), "vbases": False, "vfptr": vfptr, "methods": list(methods)}


class SlotMap(unittest.TestCase):
    def setUp(self):
        V._MEMO.clear()
        self.defs = {
            "Base": cdef(vfptr=True, methods=[
                method("~Base", 4, 0), method("__vecDelDtor", 4, 0), method("A", 4, 4),
                # VC6's CodeView: both members of the overload group introduce at the group's last offset,
                # the non-virtual one included
                method("B", 4, 12, '["int"]', "void (Base::*)(int)"),
                method("B", 4, 12, '["char"]', "void (Base::*)(char *)"), method("C", 5, None),
                method("C", 6, 16)]),
            "Derived": cdef(bases=[("Base", 0)], methods=[
                method("~Derived", 1), method("__vecDelDtor", 1), method("A", 1),
                method("D", 4, 20)]),
        }

    def test_unresolved_group_takes_the_slots_in_method_list_order(self):
        m = V.slot_map(self.defs, "Base")
        self.assertEqual(m[8]["args"], '["int"]')
        self.assertEqual(m[12]["args"], '["char"]')
        self.assertTrue(m[8]["uncertain"] and m[12]["uncertain"])
        self.assertEqual(m[0]["name"], "~")
        self.assertTrue(m[16]["pure"])

    def test_resolved_group(self):
        resolved = {("Base", "B", "void (Base::*)(int)"): None, ("Base", "B", "void (Base::*)(char *)"): 12}
        m = V.slot_map(self.defs, "Base", resolved)
        self.assertNotIn(8, m)
        self.assertEqual(m[12]["args"], '["char"]')
        self.assertFalse(m[12]["uncertain"])

    def test_overrides_and_destructor(self):
        m = V.slot_map(self.defs, "Derived")
        self.assertEqual(m[0]["owner"], "Derived")
        self.assertEqual(m[4]["owner"], "Derived")
        self.assertEqual(m[4]["intro"], "Base")
        self.assertEqual(m[16]["owner"], "Base")
        self.assertEqual(m[20]["name"], "D")
        self.assertEqual(V.chain(self.defs, "Derived"), ["Derived", "Base"])


class FakeImage:
    def __init__(self, words):
        self.words = words

    def dword(self, a):
        return self.words.get(a)


class Verdicts(unittest.TestCase):
    def test_swapped_and_agreeing_slots(self):
        V._MEMO.clear()
        defs = {"K": cdef(vfptr=True, methods=[method("~K", 4, 0), method("F", 4, 4), method("G", 4, 8),
                                                 method("H", 6, 12), method("J", 4, 16)])}
        m = V.slot_map(defs, "K")
        rows = [{"addr": "0x%X" % a, "symbol": s} for a, s in (
            (0x1000, "??_GK@@UAEPAXI@Z"), (0x1010, "?F@K@@UAEXXZ"), (0x1020, "?G@K@@UAEXXZ"),
            (0x1040, "?Other@K@@UAEXXZ"))]
        ids = V.Identities(rows, [("__purecall", 0x2000, "")])
        # retail: the destructor, then G and F swapped, a pure slot, and another method of K where ours has J
        va = 0x5000
        img = FakeImage({va: 0x1000, va + 4: 0x1020, va + 8: 0x1010, va + 12: 0x2000, va + 16: 0x1040})
        got = {r["slot"]: r["verdict"] for r in V.judge_class("K", defs, m, {}, va, img, 5, ids)}
        self.assertEqual(got, {0: "agree", 1: "moved", 2: "moved", 3: "agree", 4: "name"})
        img.words[va + 12] = 0x1040
        img.words[va + 20] = 0x1010
        got = {r["slot"]: r["verdict"] for r in V.judge_class("K", defs, m, {}, va, img, 6, ids)}
        self.assertEqual(got[3], "pure")
        self.assertEqual(got[5], "retail-only")


CL = V.decomp.TOOLS / "vc6" / "VC98" / "BIN" / "CL.EXE"

SYNTHETIC = r"""
class Stream;
class Base {
public:
	virtual ~Base();
	virtual int		GetType( void );
	virtual void	Set( int value );
	virtual void	Set( char *name );
	void			Set( float value );
	virtual bool	Test( bool flag ) = 0;
	virtual void	Write( Stream *stream, bool flush );
private:
	virtual void	Hidden( void );
};
class Derived : public Base {
public:
	virtual ~Derived();
	virtual void	Set( char *name );
	virtual bool	Test( bool flag );
	virtual void	Added( void );
};
Derived::~Derived() {}
void Derived::Added( void ) {}
"""


@unittest.skipUnless(CL.exists(), "needs tools/vc6")
class Compiled(unittest.TestCase):
    """The CodeView slot map of a synthetic class, set against VC6's own vcall thunks."""

    def test_codeview_and_thunks_agree(self):
        work = Path(tempfile.mkdtemp(prefix="vtslots"))
        try:
            src = work / "synthetic.cpp"
            src.write_text(SYNTHETIC, encoding="latin-1", newline="")
            obj = work / "synthetic.obj"
            rc, out = V.vc6.run("cl", ["/nologo", "/O2", "/GX", "/c", "/Z7", "/Tp", str(src), "/Fo" + str(obj)],
                                cwd=str(work))
            self.assertEqual(rc, 0, out)
            defs = {}
            for tt in V.export_types.debug_types(obj):
                for k, v in V.class_defs(tt).items():
                    defs.setdefault(k, v)
            V._MEMO.clear()
            probes = V.class_probes(defs, "Derived")
            res, dropped, log = V.compile_probes(src, [(pc, n, c) for _, _, pc, n, _, c in probes],
                                                 ["/nologo", "/O2", "/GX"], None, work, "probe")
            self.assertEqual(log, "")
            resolved, decorated = V.resolve_groups(probes, res)
            # Set(float) is the group's non-virtual member; Set(int) and Set(char *) are VC6's reverse order
            self.assertIn(None, resolved.values())
            m = V.slot_map(defs, "Derived", resolved)
            names = {o: (e["owner"], e["name"], e["args"] and json.loads(e["args"])) for o, e in m.items()}
            self.assertEqual(names[0][:2], ("Derived", "~"))
            self.assertEqual(names[4][:2], ("Base", "GetType"))
            self.assertEqual({names[8][:2], names[12][:2]}, {("Base", "Set"), ("Derived", "Set")})
            self.assertEqual(names[16][:2], ("Derived", "Test"))
            self.assertEqual(names[20][:2], ("Base", "Write"))
            self.assertEqual(names[24][:2], ("Base", "Hidden"))
            self.assertEqual(names[28][:2], ("Derived", "Added"))
            self.assertEqual(len(m), 8)
            # every non-overloaded name's thunk lands where the slot map puts it
            for k, (kind, cls, pc, name, o, cast) in enumerate(probes):
                if kind == "a" and k in res:
                    self.assertEqual(res[k][0], "thunk")
                    self.assertEqual(res[k][1], o, name)
                    self.assertEqual(res[k][2][0], m[o]["owner"], name)
        finally:
            shutil.rmtree(work, ignore_errors=True)


if __name__ == "__main__":
    unittest.main()
