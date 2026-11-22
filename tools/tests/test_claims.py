"""Tests of the identity checks that sit outside the byte compare: import claims (decomp.import_name,
decomp.import_claim_errors) and the reason a '// BUG:' note must give (rules.bug_reason).

    python -m unittest discover -s tools/tests
"""

import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import decomp  # noqa: E402
import rules  # noqa: E402


class ImportNames(unittest.TestCase):
    def test_stdcall_drops_underscore_and_argument_bytes(self):
        self.assertEqual(decomp.import_name("__imp__GetTickCount@0"), "GetTickCount")

    def test_cdecl_drops_underscore(self):
        self.assertEqual(decomp.import_name("__imp__strlen"), "strlen")

    def test_cpp_name_is_kept_whole(self):
        self.assertEqual(decomp.import_name("__imp_?f@@YAXXZ"), "?f@@YAXXZ")

    def test_other_symbols_are_not_imports(self):
        self.assertIsNone(decomp.import_name("_malloc"))


class FakeImage:
    def __init__(self, table):
        self.table = table

    def imports(self):
        return self.table


class ImportClaims(unittest.TestCase):
    image = FakeImage({0x888000: ("WINMM.dll", "timeGetTime"), 0x888004: ("KERNEL32.dll", "GetTickCount"),
                       0x888008: ("mss32.dll", "_AIL_WAV_info@8")})

    def test_right_claim_holds(self):
        self.assertEqual(decomp.import_claim_errors(self.image, {"__imp__timeGetTime@0": 0x888000}), [])

    def test_decorated_export_holds(self):
        self.assertEqual(decomp.import_claim_errors(self.image, {"__imp__AIL_WAV_info@8": 0x888008}), [])

    def test_swapped_claims_fail(self):
        errors = decomp.import_claim_errors(self.image, {"__imp__timeGetTime@0": 0x888004,
                                                         "__imp__GetTickCount@0": 0x888000})
        self.assertEqual(len(errors), 2)

    def test_address_outside_the_iat_fails(self):
        self.assertEqual(len(decomp.import_claim_errors(self.image, {"__imp__Sleep@4": 0x400000})), 1)

    def test_non_import_claims_are_ignored(self):
        self.assertEqual(decomp.import_claim_errors(self.image, {"_malloc": 0x718D60}), [])


class BugReasons(unittest.TestCase):
    def test_note_with_a_reason(self):
        self.assertTrue(rules.bug_reason("x = y;\t// BUG: never released"))

    def test_redundant_with_a_reason(self):
        self.assertTrue(rules.bug_reason("// BUG: redundant: the total is never used"))

    def test_reason_on_the_next_comment_line(self):
        self.assertTrue(rules.bug_reason("// BUG: // the key list is never released."))

    def test_empty_note(self):
        self.assertFalse(rules.bug_reason("x = y;\t// BUG:"))

    def test_empty_redundant_note(self):
        self.assertFalse(rules.bug_reason("// BUG: redundant:"))


if __name__ == "__main__":
    unittest.main()
