"""Tests of c2regs.py's readers on fake memory: a register candidate's source name and C2's bit sets.

    python -m unittest discover -s tools/tests
"""

import struct
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import c2dbg  # noqa: E402
import c2regs  # noqa: E402


def dwords(*values):
    return struct.pack("<%dI" % len(values), *values)


class SourceNames(unittest.TestCase):
    def mem(self, name):
        # operand at 0x1000 (+0: the symbol record), record at 0x2000 (+0x18: its name), name at 0x3000
        return c2dbg.FakeMemory({0x1000: dwords(0x2000, 0x20804, 0x1000),
                                 0x2000: dwords(0, 1, 0x1000, 0xFFFFFFB8, 4, 0x900, 0x3000),
                                 0x3000: name + b"\0"})

    def test_named_local(self):
        self.assertEqual(c2regs.source_name(self.mem(b"_total"), 0x1000), "total")
        self.assertEqual(c2regs.source_name(self.mem(b"_total"), 0x1000, raw=True), "_total")
        self.assertEqual(c2regs.source_name(self.mem(b"_"), 0x1000), "_")

    def test_back_end_temporary(self):
        m = c2dbg.FakeMemory({0x1000: dwords(0, 0x2000003, 0x1000)})
        self.assertIsNone(c2regs.source_name(m, 0x1000))

    def test_unreadable(self):
        m = c2dbg.FakeMemory({0x1000: dwords(0x9000)})
        self.assertIsNone(c2regs.source_name(m, 0x1000))


class Sets(unittest.TestCase):
    def test_set_members(self):
        # set header at 0x100 -> chunk 0x200 {base 0, next 0x300, bits 0b10010} -> chunk 0x300 {base 32, next 0, bit 0}
        m = c2dbg.FakeMemory({0x100: dwords(0x200), 0x200: dwords(0, 0x300, 0b10010), 0x300: dwords(32, 0, 1)})
        self.assertEqual(c2regs.set_members(m, 0x100), [1, 4, 32])
        self.assertEqual(c2regs.set_members(m, 0), [])


if __name__ == "__main__":
    unittest.main()
