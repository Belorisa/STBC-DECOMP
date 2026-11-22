"""Tests of merge_lane.py's command line: --accept keeps every address it is given.

    python -m unittest discover -s tools/tests
"""

import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import merge_lane  # noqa: E402


def parse(*argv):
    return merge_lane.build_parser().parse_args(["merge", "slot", "-m", "merge: lane/x", *argv])


class AcceptArguments(unittest.TestCase):
    def test_none(self):
        self.assertEqual(parse().accept, [])

    def test_one_list(self):
        self.assertEqual(parse("--accept", "0x1", "0x2").accept, ["0x1", "0x2"])

    def test_repeated_flag_keeps_both(self):
        self.assertEqual(parse("--accept", "0x1", "--accept", "0x2", "0x3").accept, ["0x1", "0x2", "0x3"])


if __name__ == "__main__":
    unittest.main()
