"""Tests of the switch-table extents of openwork's gap scan and of `cards.py add` on gap addresses.

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

START = 0x2000


def build_image():
    """A function at 0x2000: mov dl,[ecx+T2]; jmp [edx*4+T1]; ret; then the jump table, the index table and
    int3 fill up to the next function at 0x2080."""
    t1, t2 = START + 0x20, START + 0x30
    body = (b"\x8A\x91" + t2.to_bytes(4, "little")        # mov dl, byte ptr [ecx + T2]
            + b"\xFF\x24\x95" + t1.to_bytes(4, "little")  # jmp dword ptr [edx*4 + T1]
            + b"\xC3")
    body += b"\x90" * (0x20 - len(body))
    return body + b"\x01\x02\x03\x04" * 4 + b"\x00\x01" * 8 + b"\xCC" * 0x80


class Img:
    def __init__(self, code):
        self.code = code

    def read(self, va, n):
        off = va - START
        return self.code[off:off + n] if 0 <= off < len(self.code) else None


def row(addr, name, status="MATCH", size=0x20):
    return {"addr": "0x%X" % addr, "name": name, "size": size, "status": status, "score": 50.0, "rules": []}


class SwitchTables(unittest.TestCase):
    rows = [row(START, "Switch", "DIFF"), row(START + 0x80, "Next", "MATCH")]

    def test_the_tables_belong_to_the_function(self):
        img = Img(build_image())
        self.assertEqual(openwork.callees(img, START, START + 0x80)[1], 0x0E)
        ext = openwork.retail_extents(self.rows, img)
        self.assertEqual(ext, {START: 0x80})
        self.assertEqual(openwork.gaps_of(self.rows, ext), [])

    def test_without_a_table_read_the_bytes_stay_a_gap(self):
        code = bytearray(build_image())
        code[:0x0E] = b"\x90" * 0x0D + b"\xC3"   # no table reads
        ext = openwork.retail_extents(self.rows, Img(bytes(code)))
        self.assertEqual(ext, {START: 0x0E})
        rows = [row(START, "Switch", "DIFF"), row(START + 0x100, "Next", "MATCH")]
        self.assertEqual([(lo, hi) for lo, hi, _, _ in openwork.gaps_of(rows, ext)], [(START + 0x20, START + 0x100)])


class GapCards(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name)
        for d in ("build", "config", "cards"):
            (self.root / d).mkdir()
        (self.root / "build" / "report.json").write_text(json.dumps({"functions": [
            {"addr": "0x1000", "name": "F", "score": 80.0}]}), encoding="utf-8")
        (self.root / "config" / "baseline.json").write_text("{}", encoding="utf-8")
        self.saved = cards.git, cards.in_gap
        cards.git = lambda *a, **k: "lane/x\n"
        cards.in_gap = lambda addr: int(str(addr), 16) in range(0x3000, 0x3100)
        self.addCleanup(lambda: (setattr(cards, "git", self.saved[0]), setattr(cards, "in_gap", self.saved[1])))

    def tearDown(self):
        self.tmp.cleanup()

    def test_refuses_an_address_nothing_covers(self):
        with self.assertRaises(SystemExit):
            cards.add("0x4000", "tried", "sonnet", root=self.root)

    def test_accepts_a_gap_address(self):
        e = cards.add("0x3010", "the table is a jump table", "sonnet", root=self.root)
        self.assertIsNone(e["to"])
        self.assertEqual(len(cards.read("0x3010", self.root)), 1)
        self.assertIn("1 tries", cards.summary("0x3010", self.root))

    def test_gap_flag(self):
        cards.add("0x4000", "tried", "sonnet", root=self.root, gap=True)
        self.assertEqual(len(cards.read("0x4000", self.root)), 1)

    def test_an_annotated_function_still_works(self):
        self.assertEqual(cards.add("0x1000", "x", "sonnet", root=self.root)["to"], 80.0)


if __name__ == "__main__":
    unittest.main()
