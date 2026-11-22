"""Tests of c2cost.py: opcode summaries, pairing C1XX's functions with the back end's, names on fake memory.

    python -m unittest discover -s tools/tests
"""

import struct
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import c2cost  # noqa: E402
import c2dbg  # noqa: E402


class Cost(unittest.TestCase):
    def test_summary(self):
        self.assertEqual(c2cost.summary([0x26, 0x30, 0x26, 0xEE]), "symbol x2, load, opEE")

    def test_same_function(self):
        self.assertTrue(c2cost.same_function("__ctor", "??0PhaserBeam@@QAE@XZ", "PhaserBeam::PhaserBeam"))
        self.assertFalse(c2cost.same_function("__ctor", "??1PhaserBeam@@QAE@XZ", "PhaserBeam::~PhaserBeam"))
        self.assertTrue(c2cost.same_function("Get", "?Get@S@@QBEHXZ", "S::Get"))
        self.assertFalse(c2cost.same_function(None, "?Get@S@@QBEHXZ", "S::Get"))

    def test_pair(self):
        records = [("Get", [1] * 20), ("__ctor", [1] * 30), ("Set", [1] * 30), ("Set", [1] * 26)]
        traced = [("?Set@S@@QAEXH@Z", "S::Set", 30), ("?Get@S@@QBEHXZ", "S::Get", 20), ("?X@@YAXXZ", "X", 99)]
        got = [(n, r[0] if r else None) for _, n, _, r in c2cost.pair(records, traced)]
        self.assertEqual(got, [("S::Set", "Set"), ("S::Get", "Get"), ("X", None)])

    def test_function_name(self):
        mem = c2dbg.FakeMemory({0x100: struct.pack("<III", 0, 0, 0x200), 0x200: struct.pack("<II", 0, 0x300),
                                0x300: b"e00\0"})
        self.assertEqual(c2cost.function_name(mem, 0x100), "e00")
        self.assertIsNone(c2cost.function_name(mem, 0x900))


if __name__ == "__main__":
    unittest.main()
