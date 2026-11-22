"""Tests of tools/packet.py's gap packet helpers and lever keys (`{asm: ...}` at the end of a lever entry).

    python -m unittest discover -s tools/tests
"""

import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import packet  # noqa: E402


def side_by_side(rows):
    """A diff in decomp.py diff's layout from [(ours, mark, retail)]."""
    w = 60
    out = ["%-*s | %s" % (w, "compiled ?F@@YAXXZ", "retail 0x401000")]
    for l, mark, r in rows:
        out.append("%-*s %s %s" % (w, l[:w], mark, r))
    out.append("status DIFF  score 90.0")
    return "\n".join(out)


DIFF = side_by_side([
    ("00401000  test    eax, eax", " ", "00401000  test    eax, eax"),
    ("00401002  je      0x401010", "|", "00401002  jne     0x401008"),
    ("00401004  jmp     0x401000", "|", "00401004  mov     ecx, 1"),
    ("", ">", "00401009  ret     "),
])


class LeverKeys(unittest.TestCase):
    def test_key_parses(self):
        self.assertEqual(packet.asm_key("A loop's test at the top. {asm: test eax; je; jmp top}"),
                         [("test", ["eax"]), ("je", []), ("jmp", [])])
        self.assertIsNone(packet.asm_key("No key here."))
        self.assertIsNone(packet.asm_key("A {asm: x} not at the end of the entry, then more text."))

    def test_diff_sides(self):
        ours, retail = packet.diff_sides(DIFF)
        self.assertEqual([m for m, _, _ in ours], ["test", "je", "jmp"])
        self.assertEqual([m for m, _, _ in retail], ["test", "jne", "mov", "ret"])
        self.assertEqual([d for _, _, d in retail], [False, True, True, True])
        self.assertEqual(retail[2][1], "ecx, 1")

    def test_keyed_entries_come_first(self):
        entries = [("Locals", "Declare the counter first. {asm: mov ecx, 2}"),
                   ("Control flow", "A loop's test at the top. {asm: test; je; jmp top}"),
                   ("Control flow", "An inverted condition. {asm: jne; mov ecx}"),
                   ("Control flow", "Unchanged rows only. {asm: test eax}")]
        self.assertEqual(packet.keyed_levers(DIFF, entries), [entries[1][1], entries[2][1]])
        quoted = "- Some text-matched entry.\n- " + entries[2][1]
        got = packet.with_keyed_levers(DIFF, quoted, 1800, entries).splitlines()
        self.assertEqual(got, ["- (asm key in this diff) " + entries[1][1], "- (asm key in this diff) " + entries[2][1],
                               "- Some text-matched entry."])

    def test_x87_registers_and_signs_parse(self):
        self.assertEqual(packet.asm_key("x. {asm: fstp st(1); fxch st1}"),
                         [("fstp", ["st(1)"]), ("fxch", ["st(1)"])])
        self.assertEqual(packet.asm_key("x. {asm: cmp eax, -1; mov ecx, dword ptr [esp - 0x10]}"),
                         [("cmp", ["eax", "-1"]), ("mov", ["ecx", "dword", "ptr", "esp", "-0x10"])])

    def test_x87_register_matches(self):
        self.assertTrue(packet.word_in("st(1)", "st(1)"))
        self.assertTrue(packet.word_in("st(1)", "st(0), st(1)"))
        self.assertFalse(packet.word_in("st(1)", "st(3)"))

    def test_signed_numbers(self):
        self.assertTrue(packet.word_in("-1", "eax, -1"))
        self.assertTrue(packet.word_in("-1", "ecx, 0xffffffff"))
        self.assertFalse(packet.word_in("1", "eax, -1"))
        self.assertTrue(packet.word_in("1", "eax, 1"))
        self.assertTrue(packet.word_in("-0x10", "dword ptr [esp - 0x10]"))
        self.assertFalse(packet.word_in("0x10", "dword ptr [esp - 0x10]"))
        self.assertTrue(packet.word_in("0x18", "dword ptr [esp + 0x18]"))
        self.assertTrue(packet.word_in("24", "dword ptr [esp + 0x18]"))

    def test_signed_key_in_a_diff(self):
        diff = side_by_side([
            ("00401000  cmp     eax, 1", "|", "00401000  cmp     eax, -1"),
            ("00401003  setne   al", " ", "00401003  setne   al"),
        ])
        entries = [("x", "Signed. {asm: cmp eax, -1; setne al}"), ("x", "Unsigned. {asm: cmp eax, 2; setne al}")]
        self.assertEqual(packet.keyed_levers(diff, entries), [entries[0][1]])

    def test_pattern_both_sides_have_as_often_is_left_out(self):
        diff = side_by_side([
            ("00401000  fld     dword ptr [esp + 4]", "|", "00401000  fld     dword ptr [esp + 8]"),
            ("00401004  fstp    dword ptr [esp + 8]", "|", "00401004  fstp    dword ptr [esp + 4]"),
        ])
        self.assertEqual(packet.keyed_levers(diff, [("x", "Copy. {asm: fld dword; fstp dword}")]), [])
        diff = side_by_side([
            ("00401000  fld     dword ptr [esp + 4]", "|", "00401000  mov     eax, dword ptr [esp + 4]"),
            ("00401004  fstp    dword ptr [esp + 8]", "|", "00401004  mov     dword ptr [esp + 8], eax"),
        ])
        self.assertEqual(len(packet.keyed_levers(diff, [("x", "Copy. {asm: fld dword; fstp dword}")])), 1)

    def test_no_key_keeps_the_quotes(self):
        self.assertEqual(packet.with_keyed_levers(DIFF, "- a\n- b", 1800, []), "- a\n- b")
        self.assertIsNone(packet.with_keyed_levers(DIFF, None, 1800, []))


class GapHelpers(unittest.TestCase):
    def test_neighbours(self):
        rows = [{"addr": "0x401000"}, {"addr": "0x401100"}, {"addr": "0x401200"}]
        before, after = packet.neighbours(0x401150, rows)
        self.assertEqual((before["addr"], after["addr"]), ("0x401100", "0x401200"))
        self.assertEqual(packet.neighbours(0x400000, rows)[0], None)
        self.assertEqual(packet.neighbours(0x401300, rows)[1], None)

    def test_pointer_refs(self):
        class Img:
            sections = [(".text", 0x401000, 0x10, 0, 0), (".rdata", 0x500000, 12, 0x10, 0)]
            data = bytes(0x10) + bytes(4) + (0x401230).to_bytes(4, "little") + bytes(4)
        self.assertEqual(packet.pointer_refs(Img(), 0x401230), ["0x500004 (.rdata)"])
        self.assertEqual(packet.pointer_refs(Img(), 0x401234), [])


if __name__ == "__main__":
    unittest.main()
