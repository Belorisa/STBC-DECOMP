"""Tests of c2il.py's verdicts on IL captures: line markers left out, symbol-only differences, code differences.

    python -m unittest discover -s tools/tests
"""

import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import c2il  # noqa: E402


class Verdict(unittest.TestCase):
    def test_line_markers_are_left_out(self):
        self.assertEqual(c2il.strip_lines(b"\x01\x4f\x01\x80\x10\x00\x02"), b"\x01\x02")

    def test_identical_when_only_lines_move(self):
        a = {"ex": b"\x01\x4f\x01\x80\x10\x00\x02", "gl": b"g", "sy": b"s", "in": b"i"}
        b = {"ex": b"\x01\x4f\x01\x80\x22\x00\x02", "gl": b"g", "sy": b"s", "in": b"i"}
        self.assertEqual(c2il.verdict(a, b)[0], "identical")

    def test_same_code_when_symbols_differ(self):
        a = {"ex": b"\x01", "gl": b"g", "sy": b"_x", "in": b"i"}
        b = {"ex": b"\x01", "gl": b"g", "sy": b"_y", "in": b"i"}
        v, detail = c2il.verdict(a, b)
        self.assertEqual(v, "same code")
        self.assertIn("sy", detail)

    def test_different_names_the_first_byte(self):
        a = {"ex": b"\x01\x02\x03"}
        b = {"ex": b"\x01\x05\x03\x04"}
        v, detail = c2il.verdict(a, b)
        self.assertEqual(v, "different")
        self.assertIn("byte 1", detail)

    def test_batch_copies_every_suffix(self):
        for s in ("ex", "gl", "sy", "in", "db"):
            self.assertIn(s, c2il.BATCH)
        self.assertIn("MSC_CMD_FLAGS", c2il.BATCH)


if __name__ == "__main__":
    unittest.main()
