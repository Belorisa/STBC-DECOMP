"""Tests of tools/ctxscan.py's helpers: the k windows it prints, where the probes go, and how a state is classed.

    python -m unittest discover -s tools/tests
"""

import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import ctxscan  # noqa: E402


class Entry:
    def __init__(self, line):
        self.line = line


class Ranges(unittest.TestCase):
    def test_runs(self):
        self.assertEqual(ctxscan.ranges([0, 1, 2, 3, 8, 60, 61, 62, 63]), "0-3,8,60-63")
        self.assertEqual(ctxscan.ranges([]), "none")
        self.assertEqual(ctxscan.ranges([5]), "5")


class InsertionLine(unittest.TestCase):
    LINES = ["#include \"a.h\"", "", "// Does a thing.", "// FUNCTION: STBC 0x00401000 @game/A",
             "// FUNCTION: STBC 0x00401100 @game/B", "void A( void ) {", "}"]

    def test_above_the_annotation_stack(self):
        # the entry's annotation is line 5 (1-based); the probes go above the stack's first line, under the comment
        self.assertEqual(ctxscan.insertion_line(self.LINES, Entry(5), False), 3)
        self.assertEqual(ctxscan.insertion_line(self.LINES, Entry(4), False), 3)

    def test_top(self):
        self.assertEqual(ctxscan.insertion_line(self.LINES, Entry(5), True), 0)


class KindOf(unittest.TestCase):
    def test_exact_frame_and_other(self):
        push_ebx = bytes([0x53])
        mov_esp10 = bytes([0x8B, 0x44, 0x24, 0x10])   # mov eax, [esp + 0x10]
        mov_esp14 = bytes([0x8B, 0x44, 0x24, 0x14])   # mov eax, [esp + 0x14]
        mov_ecx10 = bytes([0x8B, 0x41, 0x10])         # mov eax, [ecx + 0x10]
        mov_ecx14 = bytes([0x8B, 0x41, 0x14])         # mov eax, [ecx + 0x14]
        ret = bytes([0xC3])
        self.assertEqual(ctxscan.kind_of(push_ebx + mov_esp10 + ret, push_ebx + mov_esp10 + ret, 0x401000), "exact")
        self.assertEqual(ctxscan.kind_of(push_ebx + mov_esp10 + ret, push_ebx + mov_esp14 + ret, 0x401000),
                         "frame/register")
        self.assertEqual(ctxscan.kind_of(mov_ecx10 + ret, mov_ecx14 + ret, 0x401000), "other")


if __name__ == "__main__":
    unittest.main()
