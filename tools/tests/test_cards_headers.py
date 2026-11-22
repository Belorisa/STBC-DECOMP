"""Tests of cards.header_tries and openwork.header_lines: header leads across the cards.

    python -m unittest discover -s tools/tests
"""

import json
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import cards  # noqa: E402
import openwork  # noqa: E402

ENTRIES = {
    "0x41F350": [
        {"date": "2026-11-16", "kind": "try", "lane": "ni-list-leads", "by": "opus",
         "how": "NiTList.h's NiTListBlock constructor A/B over the six list units: compiler state"},
        {"date": "2026-11-18", "kind": "try", "lane": "near-miss-2", "by": "opus",
         "how": "the constructor's base/index order; NiTList.h is off-limits to this lane"},
    ],
    "0x481CC0": [
        {"date": "2026-11-18", "kind": "try", "lane": "near-miss-2", "by": "opus",
         "how": "same order; NiTList.h is off-limits to this lane"},
        {"date": "2026-11-19", "kind": "try", "lane": "x", "by": "opus", "group": "hdr-nitlistblock-ctor",
         "how": "the block constructor written as Gamebryo's"},
        {"date": "2026-11-19", "kind": "score", "how": "NiTArray.h mentioned in a score entry"},
    ],
}


class HeaderTries(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        root = Path(self.tmp.name)
        (root / "cards").mkdir()
        for addr, entries in ENTRIES.items():
            (root / "cards" / ("%s.jsonl" % addr)).write_text(
                "".join(json.dumps(e) + "\n" for e in entries), encoding="utf-8")
        self.table = cards.header_tries(root)

    def tearDown(self):
        self.tmp.cleanup()

    def test_worked_and_noted(self):
        t = self.table["NiTList.h"]
        self.assertEqual([a for a, _ in t["worked"]], ["0x41F350"])
        self.assertEqual(sorted(a for a, _ in t["noted"]), ["0x41F350", "0x481CC0"])

    def test_group_slug_counts_as_header(self):
        self.assertEqual([a for a, _ in self.table["hdr-nitlistblock-ctor"]["worked"]], ["0x481CC0"])

    def test_only_tries(self):
        self.assertNotIn("NiTArray.h", self.table)

    def test_packet_lines_show_the_settled_try(self):
        lines = openwork.header_lines("0x481CC0", self.table)
        nitlist = [l for l in lines if l.startswith("NiTList.h")]
        self.assertTrue(nitlist and "1 other cards with a try that changed" in nitlist[0])
        self.assertTrue(any(l.strip().startswith("settled: 2026-11-16 ni-list-leads 0x41F350") for l in lines))


if __name__ == "__main__":
    unittest.main()
