"""Tests of c2operands.py's readers on fake memory: weight split, operand names and the comparison line.

    python -m unittest discover -s tools/tests
"""

import struct
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import c2dbg  # noqa: E402
import c2operands  # noqa: E402


def node(op, kind, weight, desc=0):
    d = bytearray(0x20)
    struct.pack_into("<I", d, 0x04, op)
    struct.pack_into("<I", d, 0x08, 0x40040000 | kind)
    struct.pack_into("<I", d, 0x0C, weight)
    struct.pack_into("<I", d, 0x14, desc)
    return bytes(d)


def descriptor(record, number):
    d = bytearray(0x20)
    struct.pack_into("<I", d, 0x00, record)
    struct.pack_into("<I", d, 0x1C, number)
    return bytes(d)


def record(name_ptr):
    d = bytearray(0x20)
    struct.pack_into("<I", d, 0x18, name_ptr)
    return bytes(d)


class Operands(unittest.TestCase):
    def setUp(self):
        # h's leaves as the compiler built them: _a number 2 (weight 0x10040), _b number 1 (0x10020), the product
        self.mem = c2dbg.FakeMemory({
            0x1000: node(0x14A, 2, 0x10040, 0x2000), 0x2000: descriptor(0x3000, 2), 0x3000: record(0x4000),
            0x4000: b"_a\0",
            0x1100: node(0x14A, 2, 0x10020, 0x2100), 0x2100: descriptor(0x3100, 1), 0x3100: record(0x4100),
            0x4100: b"_b\0",
            0x1200: node(0x16F, 0x0C, 0x10300AA),
            0x1300: node(0x184, 0x0E, 0x8030147)})

    def test_split(self):
        self.assertEqual(c2operands.split(0x10300AA), (1, 3, 0xAA))

    def test_describe(self):
        self.assertEqual(c2operands.describe(self.mem, 0x1000), "a #2")
        self.assertEqual(c2operands.describe(self.mem, 0x1000, raw=True), "_a #2")
        self.assertEqual(c2operands.describe(self.mem, 0x1200), "multiply (op 0x16F)")
        self.assertEqual(c2operands.describe(self.mem, 0x1300), "op 0x184")

    def test_comparison(self):
        self.assertEqual(c2operands.comparison(self.mem, 0x1000, 0x1100, -1),
                         "a #2 [h0 s1 #0040]  before  b #1 [h0 s1 #0020]")
        # a positive result puts the second operand first
        self.assertEqual(c2operands.comparison(self.mem, 0x1100, 0x1000, 1),
                         "a #2 [h0 s1 #0040]  before  b #1 [h0 s1 #0020]")
        self.assertIn("  =  ", c2operands.comparison(self.mem, 0x1000, 0x1000, 0))


if __name__ == "__main__":
    unittest.main()
