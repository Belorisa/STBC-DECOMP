"""Tests of nocall.py: the referenced set, how a definition links, the referent trace and the candidate filter.

    python -m unittest discover -s tools/tests
"""

import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import nocall  # noqa: E402


def call(at, target, opcode=b"\xE8"):
    return opcode + ((target - (at + 5)) & 0xFFFFFFFF).to_bytes(4, "little")


class Img:
    """A .text at 0x401000 holding `code`, and base relocations at `relocs` reading `words`."""

    def __init__(self, code, words=None):
        self.data = bytes(code)
        self.text = (".text", 0x401000, len(code), 0, len(code))
        self.words = words or {}
        self.relocs = set(self.words)

    def read(self, va, n):
        w = self.words.get(va)
        return w.to_bytes(4, "little") if w is not None else None


def row(addr, unit, status="MATCH", name=None, score=100.0, **kw):
    r = {"addr": "0x%X" % addr, "name": name or "F%X" % addr, "unit": unit, "status": status, "score": score}
    r.update(kw)
    return r


class Referenced(unittest.TestCase):
    def test_calls_jumps_and_relocations(self):
        code = call(0x401000, 0x402000) + call(0x401005, 0x403000, b"\xE9") + b"\x0F\x84" + \
            ((0x404000 - 0x401010) & 0xFFFFFFFF).to_bytes(4, "little")
        refs = nocall.referenced(Img(code, {0x500000: 0x405000}))
        for target in (0x402000, 0x403000, 0x404000, 0x405000):
            self.assertIn(target, refs)
        self.assertNotIn(0x406000, refs)

    def test_uses_are_counted(self):
        code = call(0x401000, 0x402000) + call(0x401005, 0x402000)
        self.assertEqual(nocall.referenced(Img(code))[0x402000], 2)


class DefinitionKind(unittest.TestCase):
    def test_kinds(self):
        self.assertEqual(nocall.definition_kind("src/x/A.h", "int A::f() {"), "header")
        self.assertEqual(nocall.definition_kind("src/x/A.cpp", "// FUNCTION: STBC 0x1\nstatic int f( int a ) {"),
                         "static")
        self.assertEqual(nocall.definition_kind("src/x/A.cpp", "inline int\nf() {"), "inline")
        self.assertEqual(nocall.definition_kind("src/x/A.cpp", "int A::f( bool isStatic ) {"), "extern")

    def test_auto_inlines(self):
        self.assertTrue(nocall.auto_inlines("netimmerse/NiCamera"))
        self.assertFalse(nocall.auto_inlines("netimmerse/NiListener"))   # its /Ob1 override
        self.assertFalse(nocall.auto_inlines("game/ShipClass"))


class Trace(unittest.TestCase):
    def test_names_it(self):
        self.assertTrue(nocall.names_it("x = GetSurfaceDesc( s, &d );", "GetSurfaceDesc"))
        self.assertFalse(nocall.names_it("x = MyGetSurfaceDesc( s );", "GetSurfaceDesc"))
        self.assertFalse(nocall.names_it("GetSurfaceDesc", "GetSurfaceDesc"))
        ctor = nocall.short_name("NiD3DDirectDraw::NiD3DDirectDraw")
        self.assertTrue(nocall.names_it("object = new NiD3DDirectDraw;", ctor))
        self.assertFalse(nocall.names_it("object->Release();", ctor))

    def test_holds_and_distinctive(self):
        own = frozenset({("rel", 0x402000), ("abs", 0x900000)})
        self.assertTrue(nocall.holds(own, own | {("rel", 0x403000)}))
        self.assertFalse(nocall.holds(own, {("rel", 0x402000)}))
        self.assertIsNone(nocall.holds(frozenset(), own))
        uses = {0x402000: 3, 0x900000: nocall.COMMON + 1}
        self.assertEqual(nocall.distinctive(own, uses), {("rel", 0x402000)})


class Features(unittest.TestCase):
    def test_member_displacements_without_the_stack(self):
        rows = [(0, "mov", "eax, dword ptr [esi + 0x168]", 6), (6, "call", "dword ptr [edx + 0x94]", 6),
                (12, "mov", "eax, dword ptr [esp + 0x18]", 4), (16, "mov", "ecx, dword ptr [esi + eax*4 + 0x4dc]", 7)]
        found = nocall.features(b"", 0, lambda code, addr: rows)
        self.assertEqual(found, {("disp", 0x168), ("disp", 0x94), ("disp", 0x4DC)})

    def test_a_sibling_as_large_shares_nothing(self):
        class Refs:
            def size(self, a):
                return {1: 100, 2: 150, 3: 400}[a]
        self.assertFalse(nocall.smaller(Refs(), 1, 2))
        self.assertTrue(nocall.smaller(Refs(), 1, 3))


class Candidates(unittest.TestCase):
    UNIT = "netimmerse/NiThing"

    def run_case(self, caller_text, refs=None, helper_name="NiThing::Helper", **kw):
        helper, caller = 0x401000, 0x401100
        rows = [row(helper, self.UNIT, name=helper_name), row(caller, self.UNIT, "DIFF", "NiThing::Caller", 80.0)]
        sources = {helper: ("src/netimmerse/NiThing.cpp", "bool NiThing::Helper() {"),
                   caller: ("src/netimmerse/NiThing.cpp", caller_text)}
        trace = frozenset({("rel", 0x700000)})
        refs_of = {helper: trace, caller: trace | {("rel", 0x700100)}}.get
        return nocall.candidates(rows, sources, refs if refs is not None else {0x700000: 2}, refs_of=refs_of, **kw)

    def test_an_open_caller_holding_the_trace_is_a_lead(self):
        cands = self.run_case("void NiThing::Caller() { open coded }")
        self.assertEqual([c["addr"] for c in cands], ["0x401000"])
        self.assertEqual(cands[0]["kind"], "ob2")
        self.assertTrue(cands[0]["callers"][0]["holds"])

    def test_a_caller_that_names_it_is_no_lead(self):
        self.assertEqual(self.run_case("void NiThing::Caller() { Helper(); }"), [])

    def test_a_called_helper_is_no_candidate(self):
        self.assertEqual(self.run_case("void NiThing::Caller() {}", refs={0x401000: 1, 0x700000: 2}), [])

    def test_a_common_referent_traces_nothing(self):
        self.assertEqual(self.run_case("void NiThing::Caller() {}", refs={0x700000: nocall.COMMON + 1}), [])

    def test_deleting_destructors_are_left_out(self):
        self.assertEqual(self.run_case("void NiThing::Caller() {}",
                                       helper_name="NiThing::`vector deleting destructor'"), [])
        shown = self.run_case("void NiThing::Caller() {}", show_all=True,
                              helper_name="NiThing::`vector deleting destructor'")
        self.assertIn("0x401000", [c["addr"] for c in shown])


class Neighbours(unittest.TestCase):
    def test_a_header_helper_takes_the_units_around_it(self):
        ordered = [(0x1000, "a/A"), (0x2000, "(headers)"), (0x3000, "b/B")]
        self.assertEqual(nocall.neighbour_units(0x2000, ordered), ["a/A", "b/B"])


if __name__ == "__main__":
    unittest.main()
