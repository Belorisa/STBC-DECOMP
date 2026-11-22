"""Tests of the BUG-note forms the rule checker (rules.bug_reason, rules.bug_noted) and the merge's listing of new
notes (merge_lane.note_marked) accept: a `//` comment, and a C comment as the CRT writes them
(src/crt/fpexcept.c: `/* BUG: the precision is set under the rounding mask: ... */`).

    python -m unittest discover -s tools/tests
"""

import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import merge_lane  # noqa: E402
import rules  # noqa: E402


class CommentNoteReasons(unittest.TestCase):
    def test_c_comment_with_a_reason(self):
        self.assertTrue(rules.bug_reason("/* BUG: the precision is set under the rounding mask */"))

    def test_empty_c_comment(self):
        self.assertFalse(rules.bug_reason("x = y; /* BUG: */"))
        self.assertFalse(rules.bug_reason("/* BUG: redundant: */"))


class BugNoted(unittest.TestCase):
    def test_line_comment_above(self):
        lines = ["// BUG: the count is never reset", "x = y;"]
        self.assertTrue(rules.bug_noted(lines, 1))

    def test_one_line_c_comment_above(self):
        lines = ["/* BUG: the precision is set under the rounding mask */", "switch (prec) {"]
        self.assertTrue(rules.bug_noted(lines, 1))

    def test_c_comment_block_above(self):
        lines = ["/*", " * BUG: the handler's rounding mode", " * is lost", " */", "*pcw = *pcw & ~IMCW_RC;"]
        self.assertTrue(rules.bug_noted(lines, 4))

    def test_empty_c_comment_excuses_nothing(self):
        self.assertFalse(rules.bug_noted(["/* BUG: */", "x = y;"], 1))

    def test_note_on_an_earlier_statement_does_not_reach(self):
        # a statement with a trailing comment ends the comment block above the line
        lines = ["/* BUG: the first store is lost */", "a = 1; /* the first store */", "b = 2;"]
        self.assertFalse(rules.bug_noted(lines, 2))

    def test_same_line_c_comment(self):
        self.assertTrue(rules.bug_noted(["x = y; /* BUG: never released */"], 0))


class MergeListing(unittest.TestCase):
    def test_forms(self):
        self.assertTrue(merge_lane.note_marked("\tx = y;\t// BUG: never released", "BUG:"))
        self.assertTrue(merge_lane.note_marked("        /* BUG: the precision is lost */", "BUG:"))
        self.assertTrue(merge_lane.note_marked(" * BUG: continued in a block", "BUG:"))
        self.assertTrue(merge_lane.note_marked("/* RULE-EXCEPTION: R8, the CRT's layout */", "RULE-EXCEPTION:"))
        self.assertTrue(merge_lane.note_marked("/* MATCH-PENDING: R17, a stand-in */", "MATCH-PENDING:"))

    def test_not_a_note(self):
        self.assertFalse(merge_lane.note_marked("\tint BUG = 0;", "BUG:"))
        self.assertFalse(merge_lane.note_marked("\t*p = q;\t/* BUGS: none */", "BUG:"))
        self.assertFalse(merge_lane.note_marked('\tprintf("BUG: %d");', "BUG:"))


if __name__ == "__main__":
    unittest.main()
