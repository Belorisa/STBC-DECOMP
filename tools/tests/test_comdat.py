"""Tests of comdat.py's order mode: the rising run, the out-of-order rows and the COMDAT selection reader.

    python -m unittest discover -s tools/tests
"""

import struct
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import comdat  # noqa: E402


def coff_with_sections(selections):
    """A COFF object with one .text section per selection (0: not a COMDAT) and a section symbol with its aux."""
    nsec = len(selections)
    header_size = 20 + 40 * nsec
    symbols = b""
    for i, sel in enumerate(selections):
        sym = b".text\0\0\0" + struct.pack("<IhHBB", 0, i + 1, 0, 3, 1)
        aux = struct.pack("<IHHIHB", 0x10, 0, 0, 0, 0, sel) + b"\0\0\0"
        symbols += sym + aux
    head = struct.pack("<HHIIIHH", 0x14C, nsec, 0, header_size, 2 * nsec, 0, 0)
    secs = b"".join(b".text\0\0\0" + struct.pack("<IIIIIIHHI", 0, 0, 0, 0, 0, 0, 0, 0, 0x60501020) for _ in selections)
    return head + secs + symbols + struct.pack("<I", 4)


class Order(unittest.TestCase):
    def test_rising_run(self):
        self.assertEqual(comdat.rising_run([10, 20, 5, 30, None, 40]), [0, 1, 3, 5])
        self.assertEqual(comdat.rising_run([None, None]), [])
        self.assertEqual(comdat.rising_run([3, 2, 1]), [2])

    def test_order_rows(self):
        secs = [("fn", "nodup", None, "?B@@YAXXZ", 0x200), ("fn", "nodup", None, "?A@@YAXXZ", 0x100),
                ("fn", "any", None, "?C@@YAXXZ", 0x300), ("fn", "any", None, "?D@@YAXXZ", None)]
        rows = comdat.order_rows(secs, comdat.rising_run([s[4] for s in secs]))
        marks = [r[0] for r in rows]
        # one of B and A is off the longest rising run (A, C), and is placed among the run's sections by its address
        self.assertEqual(marks, ["!", " ", " ", " "])
        self.assertEqual(rows[0][5], "retail: after A, before C")

    def test_selections(self):
        sel = comdat.section_selections(coff_with_sections([1, 2, 0, 6]))
        self.assertEqual(sel, {1: 1, 2: 2, 4: 6})
        self.assertEqual(comdat.SELECTIONS[sel[2]], "any")


if __name__ == "__main__":
    unittest.main()
