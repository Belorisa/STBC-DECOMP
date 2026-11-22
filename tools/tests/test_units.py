"""Tests of units.py: the static runs that interrupt a unit's code, units split by others' code, and function order.

    python -m unittest discover -s tools/tests
"""

import io
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import units  # noqa: E402


def call(at, target):
    return b"\xE8" + ((target - (at + 5)) & 0xFFFFFFFF).to_bytes(4, "little")


class Img:
    """Stubs at fixed addresses: an <iostream> quad body at 0x2000, a locale guard at 0x2100, a registration at 0x2200."""

    def __init__(self):
        self.code = {0x2000: b"\xB9\x00\x10\x9B\x00" + call(0x2005, units.IOS_INIT) + b"\xC3",
                     0x2100: b"\x8A\x0D\x34\x3E\x9B\x00\xC3",
                     0x2200: call(0x2200, 0x7D0000) + b"\xC3"}

    def read(self, va, n):
        return self.code.get(va, b"")[:n] or None


def row(addr, unit, status="MATCH", static=False, symbol=None, score=100.0):
    return {"addr": "0x%X" % addr, "name": "F%X" % addr, "unit": unit, "status": status, "static": static,
            "symbol": symbol or "?F%X@@YAXXZ" % addr, "size": 16, "score": score}


class Kinds(unittest.TestCase):
    def test_stub_kinds(self):
        img = Img()
        self.assertEqual(units.stub_kind(img, 0x2000, 16), "iostream")
        self.assertEqual(units.stub_kind(img, 0x2100, 16), "locale")
        self.assertIsNone(units.stub_kind(img, 0x2200, 16))


class Boundaries(unittest.TestCase):
    def run_case(self, statics, between=()):
        a1, a2 = row(0x1000, "game/A"), row(0x3000, "game/A")
        own = {"game/A": [(0x1000, a1), (0x3000, a2)]}
        for addr in between:
            own.setdefault("game/B", []).append((addr, row(addr, "game/B")))
        rows = [a1, a2] + [row(a, "game/A", static=True) for a in statics]
        return units.boundaries(rows, own, Img())

    def test_a_run_with_quads_inside_the_code(self):
        (found,) = self.run_case([0x2000, 0x2100, 0x2200])
        self.assertEqual(found[0], "game/A")
        self.assertEqual([k for _, k, _ in found[3]], ["iostream", "locale"])
        self.assertTrue(units.strong(found[3]))

    def test_registrations_alone_are_no_boundary(self):
        self.assertEqual(self.run_case([0x2200]), [])

    def test_another_units_code_between_is_not_this(self):
        self.assertEqual(self.run_case([0x2000], between=[0x2400]), [])

    def test_scattered(self):
        own = {"game/A": [(0x1000, row(0x1000, "game/A")), (0x3000, row(0x3000, "game/A"))],
               "game/B": [(0x2000, row(0x2000, "game/B"))]}
        self.assertEqual(units.scattered(own), {"game/A": 2})


class Order(unittest.TestCase):
    def test_out_of_place_is_outside_the_longest_ordered_run(self):
        funcs = [(a, row(a, "game/A")) for a in (0x100, 0x500, 0x200, 0x300, 0x400)]
        self.assertEqual([a for a, _ in units.out_of_place(funcs)], [0x500])
        self.assertEqual(units.out_of_place(funcs[:1]), [])

    def test_report_ranks_by_open_functions(self):
        own = {"game/A": [(a, row(a, "game/A")) for a in (0x200, 0x100)],
               "game/B": [(0x400, row(0x400, "game/B", status="DIFF", score=50.0)), (0x300, row(0x300, "game/B"))],
               "game/C": [(0x500, row(0x500, "game/C")), (0x600, row(0x600, "game/C"))]}
        out = io.StringIO()
        units.cmd_order(own, out=out)
        text = out.getvalue()
        self.assertIn("2 units", text)
        self.assertLess(text.index("game/B"), text.index("game/A"))
        self.assertNotIn("game/C", text)


if __name__ == "__main__":
    unittest.main()
