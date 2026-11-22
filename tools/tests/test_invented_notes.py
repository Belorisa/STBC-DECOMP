"""Tests of merge_lane.py's listing of added lines marked invented, beside the BUG and RULE-EXCEPTION notes.

    python -m unittest discover -s tools/tests
"""

import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import merge_lane  # noqa: E402


class InventedMarked(unittest.TestCase):
    def test_forms(self):
        for line in ("\tinline int Count() { return n; }   // (invented) the inline the loop shows",
                     "// FUNCTION: STBC 0x401000",
                     "/* the name is invented: no string names it */",
                     " * Invented helper, nothing in retail names it",
                     "class StaticsUnit4B3510;   // an invented, marked name"):
            self.assertEqual(merge_lane.invented_marked(line), "invented" in line.lower(), line)

    def test_code_is_not_a_comment(self):
        self.assertFalse(merge_lane.invented_marked("\tint invented = 0;"))
        self.assertFalse(merge_lane.invented_marked('\tTrace("invented");'))


class NotesLines(unittest.TestCase):
    def test_grouped_and_capped(self):
        notes = [("src/game/A.cpp", 10, "BUG", "x = x;   // BUG: redundant: repeats the store above")]
        notes += [("src/game/B.cpp", 20 + k, "invented", "// (invented) helper %d" % k) for k in range(3)]
        notes += [("src/game/C.cpp", 5, "invented", "// (invented) text")]
        out = merge_lane.notes_lines(notes, shown=2)
        self.assertIn("BUG / RULE-EXCEPTION notes added, MATCH-PENDING notes removed: 1", out[0])
        self.assertIn("invented helpers, names and text added: 4 in 2 files", out[2])
        self.assertEqual(out[3:], ["    src/game/B.cpp (3)", "      :20  // (invented) helper 0",
                                   "      :21  // (invented) helper 1", "      ... and 1 more",
                                   "    src/game/C.cpp (1)", "      ... and 1 more"])

    def test_none(self):
        self.assertEqual(merge_lane.notes_lines([]),
                         ["  BUG / RULE-EXCEPTION notes added, MATCH-PENDING notes removed: none",
                          "  invented helpers, names and text added: none"])


if __name__ == "__main__":
    unittest.main()
