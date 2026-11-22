"""Tests of sibling.py: the pairing of two builds' functions and the call-set comparison.

    python -m unittest discover -s tools/tests
"""

import io
import sys
import unittest
from collections import Counter
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import sibling  # noqa: E402


def fn(h, size=32, refs=(), calls=None):
    refs = [list(r) for r in refs]
    if calls is None:
        calls = Counter(t for k, t in refs if k == "c")
    return {"size": size, "hash": h, "refs": refs, "calls": dict(calls)}


class Immediate(unittest.TestCase):
    def test_pushed_pointer_is_immediate_table_is_not(self):
        self.assertTrue(sibling.immediate("0x401230", 0x401230))
        self.assertTrue(sibling.immediate("dword ptr [eax + 4], 0x401230", 0x401230))
        self.assertFalse(sibling.immediate("dword ptr [eax*4 + 0x401230]", 0x401230))
        self.assertFalse(sibling.immediate("0x4012301", 0x401230))


class FakeImage:
    """.text at 0x401000 holding `code`, no relocations."""

    def __init__(self, code):
        self.code = bytes(code)
        self.text = (".text", 0x401000, len(self.code), 0, len(self.code))
        self.relocs = set()

    def read(self, va, n):
        off = va - 0x401000
        return self.code[off:off + n] if 0 <= off and off + n <= len(self.code) else None

    def dword(self, va):
        b = self.read(va, 4)
        return None if b is None else int.from_bytes(b, "little")


class Scan(unittest.TestCase):
    def setUp(self):
        self.saved = sibling.entry_point
        sibling.entry_point = lambda img: 0x401000

    def tearDown(self):
        sibling.entry_point = self.saved

    def test_fill_tail_jump_and_packed_code_split(self):
        code = bytearray(b"\x90" * 0x40)
        code[0:4] = b"\x85\xc0\x74\x01"   # test eax, eax; je +1 (reaches the ret)
        code[4:6] = b"\x40\xc3"           # inc eax; ret
        code[0x10:0x15] = b"\xe9\x0b\x00\x00\x00"   # jmp 0x401020: a stub's tail jump past its fill
        code[0x20:0x22] = b"\x31\xc0"     # xor eax, eax
        code[0x22] = 0xC3                 # ret, then packed code with no fill
        code[0x23:0x25] = b"\x33\xc0"     # xor eax, eax
        code[0x25] = 0xC3
        c = sibling.Code(FakeImage(code))
        self.assertEqual(c.starts, [0x401000, 0x401010, 0x401020, 0x401023])
        self.assertEqual(c.funcs[0x401000]["size"], 6)
        self.assertEqual(c.funcs[0x401010]["refs"], [["j", "0x401020"]])


class Pairs(unittest.TestCase):
    def test_unique_bytes_pair_and_calls_propagate(self):
        r = {0x1000: fn("a", refs=[("c", "0x2000")]), 0x2000: fn("x", 40)}
        d = {0x1100: fn("a", refs=[("c", "0x2200")]), 0x2200: fn("y", 44)}
        p = sibling.Pairing(r, d).run()
        self.assertEqual(p.fmap, {0x1000: 0x1100, 0x2000: 0x2200})
        self.assertEqual(p.how[0x2000], "calls")

    def test_inserted_helper_stays_unpaired(self):
        # retail expands the helper the demo calls between the same two callees
        r = {0x1000: fn("a", refs=[("c", "0x2000"), ("c", "0x3000")]), 0x2000: fn("p"), 0x3000: fn("q")}
        d = {0x1100: fn("b", refs=[("c", "0x2200"), ("c", "0x2800"), ("c", "0x3300")]),
             0x2200: fn("p"), 0x2800: fn("h", 12), 0x3300: fn("q")}
        p = sibling.Pairing(r, d).run()
        self.assertNotIn(0x2800, p.finv)
        self.assertEqual(p.fmap[0x1000], 0x1100)

    def test_stub_runs_align_on_their_callees(self):
        # identical stubs; the demo's run has one more at its head. Aligned on bytes alone they would shift by one
        r = {0x1000: fn("u"), 0x1010: fn("stub", 5, [("c", "0x5000")]), 0x1020: fn("stub", 5, [("c", "0x6000")]),
             0x1030: fn("v"), 0x5000: fn("e1"), 0x6000: fn("e2")}
        d = {0x1000: fn("u"), 0x1010: fn("stub", 5, [("c", "0x4100")]), 0x1020: fn("stub", 5, [("c", "0x5100")]),
             0x1028: fn("stub", 5, [("c", "0x6100")]), 0x1030: fn("v"), 0x4100: fn("e0"), 0x5100: fn("e1"),
             0x6100: fn("e2")}
        p = sibling.Pairing(r, d).run()
        self.assertEqual(p.fmap[0x1010], 0x1020)
        self.assertEqual(p.fmap[0x1020], 0x1028)
        self.assertNotIn(0x1010, p.finv)

    def test_votes_pair_a_revised_callee_of_stubs(self):
        r = {0x1000: fn("u"), 0x1010: fn("stub", 5, [("c", "0x5000")]), 0x1020: fn("stub2", 5, [("c", "0x5000")]),
             0x1030: fn("v"), 0x5000: fn("reg", 333)}
        d = {0x1000: fn("u"), 0x1010: fn("stub", 5, [("c", "0x5100")]), 0x1020: fn("stub2", 5, [("c", "0x5100")]),
             0x1030: fn("v"), 0x5100: fn("reg-revised", 474)}
        p = sibling.Pairing(r, d).run()
        self.assertEqual(p.fmap.get(0x5000), 0x5100)

    def test_a_paired_reference_contradicts_same_bytes(self):
        r = {0x1010: fn("f", refs=[("c", "0x5000")]), 0x5000: fn("e1")}
        d = {0x1010: fn("f", refs=[("c", "0x5200")]), 0x5100: fn("e1"), 0x5200: fn("e2")}
        p = sibling.Pairing(r, d)
        self.assertTrue(p.compatible(0x1010, 0x1010))
        p.pair(0x5000, 0x5100, "bytes")
        self.assertFalse(p.compatible(0x1010, 0x1010))
        p.pair(0x1010, 0x1010, "order")
        p.prune()
        self.assertNotIn(0x1010, p.fmap)


class SyntheticRelocs(unittest.TestCase):
    def test_operands_jump_table_and_data(self):
        code = bytearray(b"\xcc" * 0x40)
        code[0:5] = b"\xa1\x00\x20\x40\x00"               # mov eax, [0x402000]: a pointer
        code[5:10] = b"\xb8\x45\x23\x01\x00"              # mov eax, 0x12345: a constant
        code[10:17] = b"\xff\x24\x85\x20\x10\x40\x00"     # jmp [eax*4 + 0x401020]
        code[0x20:0x28] = b"\x00\x10\x40\x00\x05\x10\x40\x00"   # its table: two labels
        code[0x28:0x2d] = b"\x68\x00\x10\x40\x00"         # push 0x401000, after the table
        img = FakeImage(code)
        img.base = 0x400000
        data = b"\x00\x10\x40\x00" + b"\x01\x00\x00\x00"
        img.sections = [img.text, (".data", 0x402000, len(data), 0, len(data))]
        img.data_bytes = data
        read = img.read
        img.read = lambda va, n: data[va - 0x402000:va - 0x402000 + n] if va >= 0x402000 else read(va, n)
        got = sibling.synthetic_relocs(img, (0x401000, 0x403000))
        self.assertEqual(got, {0x401001, 0x40100D, 0x401020, 0x401024, 0x401029, 0x402000})


class FakeCode:
    """A Code stand-in for the pairing's similarity check: shapes given, no relocations or strings."""

    def __init__(self, shapes):
        self.shapes = shapes
        self.relocs = set()

    def shape(self, a):
        return self.shapes[a]

    def string_at(self, a):
        return None


class Similarity(unittest.TestCase):
    def run_pairing(self, callee_shape):
        r = {0x1000: fn("a", refs=[("c", "0x2000")]), 0x2000: fn("x", 40)}
        d = {0x1100: fn("a", refs=[("c", "0x2200")]), 0x2200: fn("y", 44)}
        body = ["push ebp", "mov ebp, esp", "call A", "pop ebp", "ret"]
        rc = FakeCode({0x1000: ["call A", "ret"], 0x2000: body})
        dc = FakeCode({0x1100: ["call A", "ret"], 0x2200: callee_shape})
        return sibling.Pairing(r, d, rc, dc).run()

    def test_alike_callee_pairs_and_records_its_ratio(self):
        p = self.run_pairing(["push ebp", "mov ebp, esp", "call A", "xor eax, eax", "pop ebp", "ret"])
        self.assertEqual(p.fmap.get(0x2000), 0x2200)
        self.assertAlmostEqual(p.ratio(0x2000, 0x2200), 10 / 11)

    def test_unlike_callee_stays_unpaired(self):
        p = self.run_pairing(["fld st(0)", "fmul st(1)", "fstp dword ptr [A]", "int3", "int3"])
        self.assertEqual(p.fmap, {0x1000: 0x1100})


class Seeds(unittest.TestCase):
    def test_symbols_and_named_vtables_seed_the_pairing(self):
        r = {0x1000: fn("a"), 0x2000: fn("b"), 0x3000: fn("c")}
        d = {0x1100: fn("a2"), 0x2200: fn("b2"), 0x3300: fn("c2")}
        p = sibling.Pairing(r, d)
        p.seed_pairs = [(0x1000, 0x1100, "symbol")]
        p.seed_tables = [([0x2000, 0x3000], [0x2200, 0x3300])]
        p.run()
        self.assertEqual(p.how, {0x1000: "symbol", 0x2000: "rtti", 0x3000: "rtti"})

    def test_unique_names_leave_out_stubs_and_shared_names(self):
        got = sibling.unique_names({0x10: ["?F@@YAXXZ", "_$E12"], 0x20: ["?G@@YAXXZ"], 0x30: ["?G@@YAXXZ"],
                                    0x40: ["??_7A@@6B@"]})
        self.assertEqual(got, {"?F@@YAXXZ": 0x10})


class Names(unittest.TestCase):
    def test_qualified_names(self):
        q = sibling.qualified
        self.assertEqual(q("?Add@InsideData@BinaryVoxelizer@@QAEXGG@Z"), ("BinaryVoxelizer", "InsideData", "Add"))
        self.assertEqual(q("??0OBBNode@@QAE@XZ"), ("OBBNode", "{ctor}"))
        self.assertEqual(q("??_GOBBNode@@UAEPAXI@Z"), ("OBBNode", "{scalar deleting}"))
        self.assertEqual(q("??8InsideData@BinaryVoxelizer@@QAE_NABV01@@Z"), ("BinaryVoxelizer", "InsideData", "{op 8}"))
        self.assertIsNone(q("?GetAt@?$NiTArray@PAX@@QBEPAXI@Z"))   # a template scope
        self.assertIsNone(q("_main"))

    def named(self, r, d, rnames, dnames, ratios=None):
        p = sibling.Pairing(r, d)
        p.rnames = {a: sibling.qualified(n) for a, n in rnames.items()}
        p.dnames = {a: [sibling.qualified(n) for n in ns] for a, ns in dnames.items()}
        p.renames = {"BinaryVoxel::Slice": "VoxelSlice"}
        p.ratios.update(ratios or {})
        return p

    def test_a_renamed_class_pairs_by_name_and_overloads_by_likeness(self):
        r = {0x1000: fn("a"), 0x2000: fn("b"), 0x2100: fn("c")}
        d = {0x1100: fn("a2"), 0x2200: fn("b2"), 0x2300: fn("c2")}
        p = self.named(r, d, {0x1000: "?Refresh@VoxelSlice@@QAEXXZ", 0x2000: "?Get@VoxelSlice@@QAEXH@Z",
                              0x2100: "?Get@VoxelSlice@@QAEXHH@Z"},
                       {0x1100: ["?Refresh@Slice@BinaryVoxel@@QAEXXZ"], 0x2200: ["?Get@Slice@BinaryVoxel@@QAEXHH@Z"],
                        0x2300: ["?Get@Slice@BinaryVoxel@@QAEXH@Z"]},
                       {(0x1000, 0x1100): 0.15, (0x2000, 0x2200): 0.3, (0x2000, 0x2300): 0.7,
                        (0x2100, 0x2200): 0.6, (0x2100, 0x2300): 0.35})
        p.by_names()
        # one of a name on each side pairs at 0.15; the overloads pair the most alike first
        self.assertEqual(p.fmap, {0x1000: 0x1100, 0x2000: 0x2300, 0x2100: 0x2200})
        self.assertEqual(set(p.how.values()), {"name"})

    def test_unlike_overloads_stay_unpaired(self):
        r = {0x2000: fn("b"), 0x2100: fn("c")}
        d = {0x2200: fn("b2"), 0x2300: fn("c2")}
        p = self.named(r, d, {0x2000: "?Get@A@@QAEXH@Z", 0x2100: "?Get@A@@QAEXHH@Z"},
                       {0x2200: ["?Get@A@@QAEXH@Z"], 0x2300: ["?Get@A@@QAEXHH@Z"]},
                       {(0x2000, 0x2200): 0.15, (0x2000, 0x2300): 0.1, (0x2100, 0x2200): 0.1,
                        (0x2100, 0x2300): 0.15})
        p.by_names()
        self.assertEqual(p.fmap, {})

    def test_class_votes_rename_and_contradictions_block_weak_pairs(self):
        r = {0x1000: fn("a"), 0x1010: fn("b"), 0x2000: fn("c")}
        d = {0x1100: fn("a"), 0x1110: fn("b"), 0x2200: fn("c2")}
        p = self.named(r, d, {0x1000: "?F@Ours@@QAEXXZ", 0x1010: "?G@Ours@@QAEXXZ", 0x2000: "?H@Other@@QAEXXZ"},
                       {0x1100: ["?F@Theirs@@QAEXXZ"], 0x1110: ["?G2@Theirs@@QAEXXZ"],
                        0x2200: ["?H@Theirs@@QAEXXZ"]})
        p.by_bytes()
        p.class_votes()
        self.assertEqual(p.renames["Theirs"], "Ours")
        self.assertTrue(p.contradicts(0x2000, 0x2200))
        self.assertFalse(p.pair(0x2000, 0x2200, "order"))
        self.assertTrue(p.pair(0x2000, 0x2200, "bytes"))   # strong evidence still pairs: syms shows the clash


class Neighbours(unittest.TestCase):
    def test_callees_pair_out_of_index_by_likeness(self):
        # the revision reordered the calls: indexes don't line up, likeness does
        r = {0x1000: fn("a", refs=[("c", "0x2000"), ("c", "0x3000")]), 0x2000: fn("p", 40), 0x3000: fn("q", 300)}
        d = {0x1100: fn("a2", refs=[("c", "0x3300"), ("c", "0x2200"), ("c", "0x4400")]), 0x2200: fn("p2", 44),
             0x3300: fn("q2", 900), 0x4400: fn("z", 20)}
        p = sibling.Pairing(r, d)
        p.ratios.update({(0x1000, 0x1100): 0.8, (0x2000, 0x2200): 0.6, (0x2000, 0x3300): 0.1,
                         (0x2000, 0x4400): 0.3, (0x3000, 0x2200): 0.1, (0x3000, 0x3300): 0.45,
                         (0x3000, 0x4400): 0.2})
        p.pair(0x1000, 0x1100, "symbol")
        p.neighbours()
        self.assertEqual(p.fmap, {0x1000: 0x1100, 0x2000: 0x2200, 0x3000: 0x3300})
        self.assertEqual(p.how[0x3000], "neighbour")

    def test_a_tie_stays_unpaired(self):
        r = {0x1000: fn("a", refs=[("c", "0x2000")]), 0x2000: fn("p", 40)}
        d = {0x1100: fn("a2", refs=[("c", "0x2200"), ("c", "0x2300")]), 0x2200: fn("p2", 44), 0x2300: fn("p3", 44)}
        p = sibling.Pairing(r, d)
        p.ratios.update({(0x2000, 0x2200): 0.5, (0x2000, 0x2300): 0.5})
        p.pair(0x1000, 0x1100, "symbol")
        p.neighbours()
        self.assertEqual(p.fmap, {0x1000: 0x1100})

    def test_agreeing_callees_lower_the_floor(self):
        r = {0x1000: fn("a", refs=[("c", "0x2000")]), 0x2000: fn("p", 40, [("c", "0x5000"), ("c", "0x6000")]),
             0x5000: fn("e1"), 0x6000: fn("e2")}
        d = {0x1100: fn("a2", refs=[("c", "0x2200")]), 0x2200: fn("p2", 90, [("c", "0x5100"), ("c", "0x6100")]),
             0x5100: fn("e1"), 0x6100: fn("e2")}
        p = sibling.Pairing(r, d)
        p.ratios[(0x2000, 0x2200)] = 0.3
        p.pair(0x1000, 0x1100, "symbol")
        p.neighbours()
        self.assertNotIn(0x2000, p.fmap)   # 0.3 alike, its callees unpaired
        p.by_bytes()
        p.neighbours()
        self.assertEqual(p.fmap.get(0x2000), 0x2200)

    def test_stubs_pair_by_their_paired_callees_away_from_the_link_order(self):
        r = {0x1000: fn("stub", 5, [("c", "0x5000"), ("d", "0x9000")]), 0x5000: fn("reg")}
        d = {0x8000: fn("stub", 5, [("c", "0x5100"), ("d", "0x9100")]), 0x5100: fn("reg")}
        p = sibling.Pairing(r, d)
        p.pair(0x5000, 0x5100, "symbol")
        p.by_keys()
        self.assertEqual(p.fmap.get(0x1000), 0x8000)


class Sides(unittest.TestCase):
    def test_sides_and_verdicts(self):
        retail, ours = Counter({"0xA": 1}), Counter({"0xB": 2})
        self.assertEqual(sibling.sides(retail, ours, Counter({"0xA": 1}), ["0xA", "0xB"]),
                         {"0xA": "retail", "0xB": "retail"})
        self.assertEqual(sibling.sides(retail, ours, Counter({"0xB": 2}), ["0xA", "0xB"]),
                         {"0xA": "ours", "0xB": "ours"})
        # a side resting on the sibling not calling 0xB, while it makes calls nothing maps, is unknown
        self.assertEqual(sibling.sides(retail, ours, Counter({"0xA": 1}), ["0xA", "0xB"], unexplained=1),
                         {"0xA": "retail", "0xB": "unknown"})
        self.assertEqual(sibling.verdict({"0xA": "retail", "0xB": "unknown"}), "mixed")
        self.assertEqual(sibling.verdict({}), "-")


@unittest.skipUnless(sibling.undecorate("?F@@YAXXZ"), "needs dbghelp's UnDecorateSymbolName")
class Signatures(unittest.TestCase):
    def test_parameter_name_and_const(self):
        ours = "?GetStatistics@NiFrameRate@@QAEXAAN000PAI@Z"
        theirs = "?GetStats@NiFrameRate@@QAEXAAN000PAH@Z"
        self.assertEqual(sibling.compare_signatures(ours, theirs),
                         ["name", "parameter 5 unsigned int * / int *"])
        self.assertEqual(sibling.compare_signatures("?Get@A@@QAEHXZ", "?Get@A@@QBEHXZ"), ["const no / yes"])
        self.assertEqual(sibling.compare_signatures(ours, ours), [])

    def test_best_name_of_folded_bodies(self):
        names = ["??_EA@@UAEPAXI@Z", "?Other@B@@QAEXXZ", "?Get@A@@QBEHXZ"]
        self.assertEqual(sibling.best_name("?Get@A@@QAEHXZ", names), "?Get@A@@QBEHXZ")


class Flips(unittest.TestCase):
    def test_classes(self):
        base = [("push", "esi"), ("mov", "dword ptr [esp + 0x18], edi"), ("call", "dword ptr [eax + 0x68]")]
        slots = [("push", "esi"), ("mov", "dword ptr [esp + 0x1c], ebp"), ("call", "dword ptr [eax + 0x68]")]
        moved = [base[1], base[0], base[2]]
        vtable = [("push", "esi"), ("mov", "dword ptr [esp + 0x18], edi"), ("call", "dword ptr [eax + 0x64]")]
        self.assertEqual(sibling.flip_class(base, slots), "frame/register")
        self.assertEqual(sibling.flip_class(base, moved), "schedule")
        self.assertEqual(sibling.flip_class(base, vtable), "layout")
        self.assertEqual(sibling.flip_class(base, base[:2]), "code")


def view():
    data = {"retail": {"0x1000": fn("a", refs=[("c", "0x2000"), ("c", "0x3000")]), "0x2000": fn("p"),
                       "0x3000": fn("q")},
            "sibling": {"0x1100": fn("b", refs=[("c", "0x2200"), ("c", "0x2800")]), "0x2200": fn("p"),
                     "0x2800": fn("h", 12, refs=[("c", "0x3300")]), "0x3300": fn("q")},
            "pairs": {"0x1000": ["0x1100", "bytes"], "0x2000": ["0x2200", "calls"], "0x3000": ["0x3300", "calls"]},
            "data": {}, "imports": {}}
    rows = [{"addr": "0x1000", "name": "Caller::Run", "status": "DIFF", "score": 80.0},
            {"addr": "0x2000", "name": "P", "status": "MATCH", "score": 100.0},
            {"addr": "0x3000", "name": "Q::Get", "status": "MATCH", "score": 100.0}]
    return sibling.View(data, rows, extra={})


class CallSets(unittest.TestCase):
    def test_demo_helper_and_expanded_callee(self):
        v = view()
        rc, dc = v.calls(0x1000)
        self.assertEqual(rc - dc, Counter({"0x3000": 1}))
        self.assertEqual(dc - rc, Counter({"demo:0x2800": 1}))
        self.assertEqual(set(sibling.differences(v, only_open=True)), {0x1000})

    def test_reports_name_the_callees(self):
        v = view()
        out = io.StringIO()
        sibling.report_callsets(v, out=out)
        text = out.getvalue()
        self.assertIn("0x1000 Caller::Run", text)
        self.assertIn("0x3000 Q::Get", text)
        self.assertIn("demo:0x2800", text)
        out = io.StringIO()
        sibling.report_new(v, out=out)
        self.assertIn("00002800    12      1  0x1000 Caller::Run", out.getvalue())


class OpenFunctions(unittest.TestCase):
    def test_a_sibling_sides_with_retail(self):
        v = view()
        # retail calls P where ours expands it: the demo calls P too
        cs = {"functions": {"0x1000": {"name": "Caller::Run", "score": 80.0, "unit": "game/Caller",
                                       "retail": {"0x2000": 1, "0x3000": 1}, "ours": {"0x3000": 1},
                                       "only_retail": {"0x2000": 1}, "only_ours": {}}},
              "names": {"0x2000": "P"}}
        out = io.StringIO()
        entries = sibling.report_open({"demo": v}, cs, out=out)
        self.assertEqual(entries[0]["for_retail"], ["demo"])
        text = out.getvalue()
        self.assertIn("80.0 0x1000 Caller::Run (game/Caller)  retail side: demo", text)
        self.assertIn("retail only: 0x2000 P", text)
        self.assertIn("also calls demo:0x2800", text)

    def test_a_side_resting_on_a_missing_call_is_unknown_beside_an_unmapped_call(self):
        # retail calls Q::Get, ours doesn't, and the demo calls a helper of its own that calls Q::Get
        v = view()
        cs = {"functions": {"0x1000": {"name": "Caller::Run", "score": 80.0, "unit": "game/Caller",
                                       "retail": {"0x2000": 1, "0x3000": 1}, "ours": {"0x2000": 1},
                                       "only_retail": {"0x3000": 1}, "only_ours": {}}}, "names": {}}
        entries = sibling.open_functions({"demo": v}, cs)
        self.assertEqual(entries[0]["builds"]["demo"]["verdict"], "unknown")

    def test_identical_copy_maps_its_calls_by_position(self):
        data = {"retail": {"0x1000": fn("same", refs=[("c", "0x2000")]), "0x2000": fn("p")},
                "sibling": {"0x1100": fn("same", refs=[("c", "0x2900")]), "0x2200": fn("p"), "0x2900": fn("p2")},
                "pairs": {"0x1000": ["0x1100", "bytes"], "0x2000": ["0x2200", "calls"]},
                "data": {}, "imports": {}, "build": "mpe"}
        v = sibling.View(data, [{"addr": "0x1000", "name": "F", "status": "DIFF", "score": 50.0}], extra={})
        self.assertEqual(v.sibling_calls(0x1000), Counter({"0x2000": 1}))
        self.assertEqual(v.map_sibling(0x2900), "mpe:0x2900")


if __name__ == "__main__":
    unittest.main()
