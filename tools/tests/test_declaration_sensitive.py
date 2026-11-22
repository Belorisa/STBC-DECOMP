"""Tests of the declaration-sensitive list: config/declaration_sensitive.txt, decomp.flip_kind and sensitive_flip, and
merge_lane.py's reading of regress's accepted flips.

    python -m unittest discover -s tools/tests
"""

import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import decomp  # noqa: E402
import merge_lane  # noqa: E402

RETAIL = [("push", "ebx"), ("mov", "eax, dword ptr [esp + 0x10]"), ("lea", "ecx, [esp + 0x18]"),
          ("call", "0x6fa000"), ("mov", "dword ptr [0x98d804], eax"), ("ret", "")]


class FlipKind(unittest.TestCase):
    def test_frame_slots_and_registers_only(self):
        ours = [("push", "esi"), ("mov", "edx, dword ptr [esp + 0x14]"), ("lea", "ecx, [esp + 0x10]"),
                ("call", "0x6fa000"), ("mov", "dword ptr [0x98d804], eax"), ("ret", "")]
        self.assertEqual(decomp.flip_kind(ours, RETAIL), "frame/register")

    def test_same_instructions_in_another_order(self):
        ours = [RETAIL[0], RETAIL[2], RETAIL[1]] + RETAIL[3:]
        self.assertEqual(decomp.flip_kind(ours, RETAIL), "schedule")

    def test_an_immediate_or_a_global_is_not_a_frame_slot(self):
        ours = list(RETAIL)
        ours[1] = ("mov", "eax, 0x10")
        self.assertIsNone(decomp.flip_kind(ours, RETAIL))
        ours = list(RETAIL)
        ours[4] = ("mov", "dword ptr [0x98d808], ecx")   # another global: masked like any address
        self.assertEqual(decomp.flip_kind(ours, RETAIL), "frame/register")

    def test_a_member_offset_is_not_a_frame_slot(self):
        ours = list(RETAIL)
        ours[1] = ("mov", "eax, dword ptr [ecx + 0x14]")
        theirs = list(RETAIL)
        theirs[1] = ("mov", "eax, dword ptr [ecx + 0x10]")
        self.assertIsNone(decomp.flip_kind(ours, theirs))
        ours[1] = ("mov", "eax, dword ptr [esi + 0x10]")   # another register, the same member
        self.assertEqual(decomp.flip_kind(ours, theirs), "frame/register")

    def test_ebp_is_a_frame_pointer_only_after_mov_ebp_esp(self):
        prologue = [("push", "ebp"), ("mov", "ebp, esp")]
        ours = [("mov", "eax, dword ptr [ebp - 8]"), ("ret", "")]
        theirs = [("mov", "eax, dword ptr [ebp - 0xc]"), ("ret", "")]
        self.assertEqual(decomp.flip_kind(prologue + ours, prologue + theirs), "frame/register")
        self.assertIsNone(decomp.flip_kind(ours, theirs))   # ebp as a general register: [ebp - 8] is no slot

    def test_indexed_frame_slot(self):
        ours = [("mov", "eax, dword ptr [esp + ecx*4 + 0x20]")]
        theirs = [("mov", "eax, dword ptr [esp + edx*4 + 0x24]")]
        self.assertEqual(decomp.flip_kind(ours, theirs), "frame/register")
        self.assertEqual(decomp.flip_kind([("mov", "eax, dword ptr [esp]")], [("mov", "eax, dword ptr [esp + 4]")]),
                         "frame/register")

    def test_an_extra_instruction_is_not_a_flip(self):
        self.assertIsNone(decomp.flip_kind(RETAIL + [("nop", "")], RETAIL + [("int3", "")]))


class ListAndAcceptance(unittest.TestCase):
    def test_the_list_parses(self):
        with tempfile.TemporaryDirectory() as d:
            p = Path(d) / "declaration_sensitive.txt"
            p.write_text("# comment\n0x554040 GraphicsMenu::ChangeDisplayMode frame/register\n"
                         "0x585F40 WeaponSystem_GetTargetList frame/register,schedule  # note\n\n")
            got = decomp.declaration_sensitive(p)
        self.assertEqual(got, {0x554040: ("GraphicsMenu::ChangeDisplayMode", ("frame/register",)),
                               0x585F40: ("WeaponSystem_GetTargetList", ("frame/register", "schedule"))})

    def test_the_repository_list_names_the_ruled_functions(self):
        got = decomp.declaration_sensitive()
        # CreateClone (0x8022A0): user's ruling, 2026-11-18; DisableGlossMaps (0x45CD20): 2026-11-19
        self.assertEqual(sorted(got), [0x45CD20, 0x554040, 0x585F40, 0x59D5B0, 0x6FA560, 0x7A8DD0, 0x8022A0])
        self.assertIn("schedule", got[0x585F40][1])
        self.assertNotIn("schedule", got[0x554040][1])

    def test_sensitive_flip(self):
        listed = {0x554040: ("GraphicsMenu::ChangeDisplayMode", ("frame/register",))}
        row = {"addr": "0x554040", "status": "DIFF", "wall": "register", "flip": "frame/register"}
        self.assertTrue(decomp.sensitive_flip(row, False, listed))
        self.assertFalse(decomp.sensitive_flip(row, True, listed))                       # source edited
        self.assertFalse(decomp.sensitive_flip(dict(row, flip="schedule"), False, listed))   # not allowed here
        self.assertFalse(decomp.sensitive_flip(dict(row, flip=None), False, listed))
        self.assertFalse(decomp.sensitive_flip(dict(row, wall="control-flow"), False, listed))
        self.assertFalse(decomp.sensitive_flip(dict(row, addr="0x554050"), False, listed))   # not listed


class MergeReadsTheFlips(unittest.TestCase):
    def test_regress_lines_are_read(self):
        out = ("declaration-sensitive 0x554040 GraphicsMenu::ChangeDisplayMode lost MATCH (DIFF 97.3, frame/register),"
               " source unchanged: accepted, listed in config/declaration_sensitive.txt\n"
               "REGRESSION 0x401000 X::Y lost MATCH (DIFF, 90.0)\nregressions: 1\n")
        flips = merge_lane.sensitive_flips(out)
        self.assertEqual(flips, [("0x554040", "GraphicsMenu::ChangeDisplayMode", "DIFF", 97.3, "frame/register")])
        e = merge_lane.flip_entry(flips[0], "tools-merge", "2026-11-16")
        self.assertEqual((e["kind"], e["by"], e["to"], e["wall"]), ("try", "merge_lane", 97.3, "register"))
        self.assertIn("frame/register", e["how"])


if __name__ == "__main__":
    unittest.main()
