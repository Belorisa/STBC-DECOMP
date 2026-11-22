"""Tests of merge_lane.py's invented-marker scan: a banner's plain text lines count as comment.

    python -m unittest discover -s tools/tests
"""

import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import merge_lane  # noqa: E402

SOURCE = """/*
================
CutEdge

(invented) The cut point.
================
*/
inline void CutEdge() {
	const char *s = "/* not a comment";
	int invented = 0;	// a plain local
}
int x; /* one line */ int y;
"""


class BlockComments(unittest.TestCase):
    def setUp(self):
        self.blocks = merge_lane.block_comment_lines(SOURCE)

    def test_banner_lines_inside(self):
        self.assertTrue({1, 2, 3, 4, 5, 6, 7} <= self.blocks)

    def test_code_outside(self):
        self.assertFalse({8, 9, 10, 11} & self.blocks)

    def test_one_line_comment(self):
        self.assertIn(12, self.blocks)

    def test_banner_marker_found(self):
        line = SOURCE.splitlines()[4]
        self.assertTrue(merge_lane.invented_marked(line, 5 in self.blocks))

    def test_identifier_not_a_marker(self):
        line = SOURCE.splitlines()[9]
        self.assertFalse(merge_lane.invented_marked(line, 10 in self.blocks))
        self.assertFalse(merge_lane.invented_marked("\tint invented = 0;", False))


if __name__ == "__main__":
    unittest.main()
