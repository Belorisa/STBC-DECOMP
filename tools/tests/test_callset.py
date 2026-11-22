"""Tests of callset.py: the call targets read from two disassemblies, their pairing, the names and the reports.

    python -m unittest discover -s tools/tests
"""

import io
import sys
import unittest
from collections import Counter
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import callset  # noqa: E402


class Targets(unittest.TestCase):
    def test_direct_import_and_unresolved_calls_count_registers_dont(self):
        ins = [("call", "0x414540"), ("call", "0x414540"), ("call", "dword ptr [0x888170]"),
               ("call", "?_foo"), ("call", "dword ptr [eax + 0x10]"), ("call", "eax"), ("mov", "eax, 0x414540")]
        self.assertEqual(callset.call_targets(ins),
                         Counter({"0x414540": 2, "0x888170": 1, "?_foo": 1}))

    def test_addresses_are_upper_case(self):
        self.assertEqual(callset.call_targets([("call", "0x40cfe0")]), Counter({"0x40CFE0": 1}))


class Compare(unittest.TestCase):
    def test_retail_calls_what_ours_expands(self):
        ours = [("call", "0x500000")]
        retail = [("call", "0x500000"), ("call", "0x414540"), ("call", "0x414540")]
        _, _, only_r, only_o = callset.compare(ours, retail)
        self.assertEqual(only_r, Counter({"0x414540": 2}))
        self.assertEqual(only_o, Counter())

    def test_ours_calls_what_retail_expands(self):
        _, _, only_r, only_o = callset.compare([("call", "0x414560")], [])
        self.assertEqual((only_r, only_o), (Counter(), Counter({"0x414560": 1})))

    def test_unresolved_calls_are_placed_at_the_copy_retail_calls(self):
        addresses = {"??1X@@QAE@XZ": [0x401EA0, 0x416F80]}
        ours = [("call", "???1X@@QAE@XZ"), ("call", "0x500000")]
        retail = [("call", "0x416f80")]
        _, calls_o, only_r, only_o = callset.compare(ours, retail, addresses)
        self.assertEqual(calls_o, Counter({"0x416F80": 1, "0x500000": 1}))
        self.assertEqual((only_r, only_o), (Counter(), Counter({"0x500000": 1})))

    def test_unplaced_unresolved_calls_pair_like_walls(self):
        # one call of ours without an address, one retail call nothing else accounts for: the sets agree
        _, _, only_r, only_o = callset.compare([("call", "?_bar")], [("call", "0x600000")])
        self.assertEqual((only_r, only_o), (Counter(), Counter()))


class Pretty(unittest.TestCase):
    def test_names(self):
        cases = {"??0NiPoint3@@QAE@MMM@Z": "NiPoint3::NiPoint3",
                 "??4NiPoint3@@QAEAAV0@ABV0@@Z": "NiPoint3::operator=",
                 "??1?$NiPointer@VNiAVObject@@@@QAE@XZ": "NiPointer<NiAVObject>::~NiPointer",
                 "??0?$NiPointer@VNiProperty@@@@QAE@PAVNiProperty@@@Z": "NiPointer<NiProperty>::NiPointer",
                 "??2@YAPAXI@Z": "operator new",
                 "?SetField@NiProperty@@QAEXGGG@Z": "NiProperty::SetField",
                 "?GetAt@?$NiTArray@V?$NiPointer@VNiAVObject@@@@@@QBEABV?$NiPointer@VNiAVObject@@@@I@Z":
                     "NiTArray<NiPointer<NiAVObject>>::GetAt",
                 "_malloc": "malloc"}
        for decorated, want in cases.items():
            self.assertEqual(callset.pretty(decorated), want, decorated)

    def test_unreadable_is_none(self):
        self.assertIsNone(callset.pretty("??$Foo@PAH@@YAXXZ"))


def data():
    def f(name, score, only_r=None, only_o=None, retail=None, ours=None):
        return {"name": name, "score": score, "unit": "game/X", "parked": None, "draft": "x", "retail": retail or {},
                "ours": ours or {}, "only_retail": only_r or {}, "only_ours": only_o or {}}
    return {"functions": {
        "0x500000": f("A::F", 90.0, {"0x414540": 2}, retail={"0x414540": 2, "0x600000": 1}, ours={"0x600000": 1}),
        "0x500100": f("A::G", 60.0, {"0x414540": 1}),
        "0x500200": f("B::H", 99.0, {"0x40CFE0": 1}, {"0x414560": 1}),
        "0x500300": f("B::I", 80.0)},
        "names": {"0x414540": ["NiPoint3::NiPoint3", "src/netimmerse/NiPoint3.h, MATCH"],
                  "0x414560": ["NiPoint3::operator=", "src/netimmerse/NiPoint3.h, MATCH"]}}


class Reports(unittest.TestCase):
    def text(self, fn, *args, **kw):
        out = io.StringIO()
        fn(data(), *args, out=out, **kw)
        return out.getvalue()

    def test_ranked_by_functions_then_mean_score(self):
        text = self.text(callset.report)
        self.assertIn("4 open functions, 3 whose call sets differ", text)
        lines = [l for l in text.splitlines() if "0x" in l]
        self.assertIn("     2      3   75.0  0x414540 NiPoint3::NiPoint3", lines[0])
        self.assertIn("0x40CFE0 (no identity)", lines[1])
        self.assertIn("0x414560 NiPoint3::operator=", lines[2])

    def test_min_score(self):
        text = self.text(callset.report, 85.0)
        self.assertIn("     1      2   90.0  0x414540", text)

    def test_callee_by_name_or_address(self):
        by_name = self.text(callset.report_callee, "niPOINT3::niPoint3")
        self.assertIn("2 open functions", by_name)
        self.assertIn("90.0 0x500000  draft  A::F  x2", by_name)
        self.assertEqual(by_name, self.text(callset.report_callee, "414540"))
        self.assertIn("no open function", self.text(callset.report_callee, "Nothing"))

    def test_function(self):
        text = self.text(callset.report_function, "0x500000")
        self.assertIn("     2      0  0x414540 NiPoint3::NiPoint3  <- differs", text)
        self.assertIn("1 other callees agree", text)
        self.assertIn("is not an open", self.text(callset.report_function, "0x123456"))


if __name__ == "__main__":
    unittest.main()
