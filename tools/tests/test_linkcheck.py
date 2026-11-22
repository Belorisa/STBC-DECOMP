"""Tests of tools/linkcheck.py's link dry-run rules.

    python -m unittest discover -s tools/tests
"""

import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import coff  # noqa: E402
import linkcheck  # noqa: E402


class Analyse(unittest.TestCase):
    def test_duplicates_unresolved_and_variants(self):
        objects = {
            "game/A": ({"?F@@YAXXZ", "_g_count"}, {"??_7Base@@6B@"}, {"?G@@YAXXZ", "_memcpy", "?H@@YAXXZ"}),
            "game/B": ({"?F@@YAXXZ"}, {"??_7Base@@6B@"}, {"?H@@YAXXZ"}),
            "crt/crt0": ({"___app_type"}, set(), set()),
            "crt/wincrt0": ({"___app_type", "_WinMainCRTStartup"}, set(), {"?G@@YAXXZ"}),
        }
        dup, unres, variants = linkcheck.analyse(objects, {"_memcpy"}, {"crt/crt0": {"crt/wincrt0"}})
        self.assertEqual(dup, {"?F@@YAXXZ": ["game/A", "game/B"]})            # a COMDAT twice is no duplicate
        self.assertEqual(variants, {"___app_type": ["crt/crt0", "crt/wincrt0"]})
        self.assertEqual(unres, {"?G@@YAXXZ": ["crt/wincrt0", "game/A"], "?H@@YAXXZ": ["game/A", "game/B"]})
        self.assertEqual(linkcheck.by_unit(unres)["game/A"], 2)

    def test_report_counts(self):
        lines = linkcheck.report({}, {"?G@@YAXXZ": ["game/A"]}, top=5, variants={})
        self.assertTrue(lines[0].startswith("duplicate definitions (non-COMDAT, in more than one object): 0"))
        self.assertIn("1 names, 1 references", lines[2])


class ObjectSymbols(unittest.TestCase):
    def test_kinds(self):
        class Sec:
            def __init__(self, flags):
                self.flags = flags

        class Obj:
            sections = [Sec(0), Sec(coff.SCN_LNK_COMDAT)]
            symbols = {0: coff.Symbol(0, "_plain", 0, 1, 0x20, coff.SC_EXTERNAL),
                       1: coff.Symbol(1, "_inline", 0, 2, 0x20, coff.SC_EXTERNAL),
                       2: coff.Symbol(2, "_common", 4, 0, 0, coff.SC_EXTERNAL),
                       3: coff.Symbol(3, "_ref", 0, 0, 0x20, coff.SC_EXTERNAL),
                       4: coff.Symbol(4, "$SG100", 0, 1, 0, coff.SC_STATIC)}

            def section(self, n):
                return self.sections[n - 1]
        self.assertEqual(linkcheck.object_symbols(Obj()), ({"_plain"}, {"_inline", "_common"}, {"_ref": None}))

    def test_weak_external_names_its_fallback(self):
        class Obj:
            weak = coff.Symbol(0, "??_EFoo@@UAEPAXI@Z", 0, 0, 0x20, coff.SC_WEAK_EXTERNAL)
            weak.weak_default = 1
            symbols = {0: weak, 1: coff.Symbol(1, "??_GFoo@@UAEPAXI@Z", 0, 0, 0x20, coff.SC_EXTERNAL)}
        self.assertEqual(linkcheck.object_symbols(Obj())[2],
                         {"??_EFoo@@UAEPAXI@Z": "??_GFoo@@UAEPAXI@Z", "??_GFoo@@UAEPAXI@Z": None})


class Weak(unittest.TestCase):
    def test_weak_reference_resolves_through_its_fallback(self):
        # the vftable's unit references a weak ??_E falling back to ??_G; the defining unit emits only ??_G
        objects = {
            "game/A": (set(), {"??_7Foo@@6B@", "??_GFoo@@UAEPAXI@Z"}, {"??_EFoo@@UAEPAXI@Z": "??_GFoo@@UAEPAXI@Z"}),
            "game/B": (set(), set(), {"??_EBar@@UAEPAXI@Z": "??_GBar@@UAEPAXI@Z"}),
        }
        _, unres, _ = linkcheck.analyse(objects, set())
        self.assertEqual(unres, {"??_EBar@@UAEPAXI@Z": ["game/B"]})       # neither it nor its fallback is defined
        self.assertEqual(linkcheck.weak_fallbacks(objects, unres), {"??_EBar@@UAEPAXI@Z": ["??_GBar@@UAEPAXI@Z"]})
        _, unres, _ = linkcheck.analyse(objects, {"??_GBar@@UAEPAXI@Z"})   # a claimed fallback resolves it
        self.assertEqual(unres, {})

    def test_split_variants_and_imports(self):
        unres = {"_main": ["crt/crt0"], "__imp__X@4": ["game/A"], "?G@@YAXXZ": ["crt/crt0", "game/A"]}
        rest, variant, imports = linkcheck.split(unres, {"crt/crt0": {"crt/wincrt0"}})
        self.assertEqual(rest, {"?G@@YAXXZ": ["crt/crt0", "game/A"]})
        self.assertEqual(variant, {"_main": ["crt/crt0"]})
        self.assertEqual(imports, {"__imp__X@4": ["game/A"]})


if __name__ == "__main__":
    unittest.main()
