"""Tests of decomp.py's Resolver for a decorated name annotated at several retail addresses: retail keeps more than one
body of an inline function when the linker didn't fold the units' copies (TGStream::ReadBool at 0x6D2030 in
TGStream's code and at 0x6D3330 in TGBufferedFileStream's), and each unit's references resolve to its own copy.

    python -m unittest discover -s tools/tests
"""

import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import coff  # noqa: E402
import decomp  # noqa: E402

TEXT = 0x60000020                 # code, executable
COMDAT = TEXT | coff.SCN_LNK_COMDAT
NAME = "?ReadBool@TGStream@@UAE_NXZ"


class Image:
    """A retail image with nothing readable: the tests below never compare bytes."""

    def read(self, addr, size):
        return None

    def dword(self, addr):
        return None


def obj(defined=True, flags=COMDAT):
    """An object with one section holding NAME (or referring to it, when not defined)."""
    o = object.__new__(coff.Coff)
    o.sections = [coff.Section(1, ".text", b"\x90" * 16, 16, flags, [])]
    sym = coff.Symbol(0, NAME, 0, 1 if defined else 0, 0x20, coff.SC_EXTERNAL)
    o.symbols = {0: sym}
    return o, sym


def unit(name, addrs, defined=True, flags=COMDAT):
    """A unit whose object defines NAME, annotated at each of addrs."""
    o, sym = obj(defined, flags)
    u = decomp.Unit(name, o, [])
    for a in addrs:
        e = decomp.Entry("FUNCTION", a, name, None, 1, "TGStream::ReadBool", None, [], 0, None)
        u.entries.append(e)
        u.symbols[a] = sym
    return u, sym


class Copies(unittest.TestCase):
    def test_each_unit_resolves_to_its_own_copy(self):
        a, sa = unit("game/TGStream", [0x6D2030])
        b, sb = unit("game/TGBufferedFileStream", [0x6D3330])
        res = decomp.Resolver(Image(), {a.name: a, b.name: b}, {})
        self.assertEqual(res.errors, [])
        self.assertEqual(res.resolve(a, sa, 0x6D2030, 0), (0x6D2030, None))
        self.assertEqual(res.resolve(b, sb, 0x6D3330, 0), (0x6D3330, None))
        self.assertNotIn(NAME, res.globals)

    def test_one_address_stays_global(self):
        a, _ = unit("game/TGStream", [0x6D2030])
        b, sb = unit("game/TGBufferedFileStream", [0x6D2030])
        res = decomp.Resolver(Image(), {a.name: a, b.name: b}, {})
        self.assertEqual(res.errors, [])
        self.assertEqual(res.globals[NAME], 0x6D2030)

    def test_one_unit_annotating_two_addresses_is_an_error(self):
        a, _ = unit("game/TGStream", [0x6D2030, 0x6D3330])
        res = decomp.Resolver(Image(), {a.name: a}, {})
        self.assertEqual(len(res.errors), 1)
        self.assertIn("annotated twice in game/TGStream", res.errors[0])

    def test_a_non_comdat_definition_has_one_address(self):
        a, _ = unit("game/A", [0x401000], flags=TEXT)
        b, _ = unit("game/B", [0x402000], flags=TEXT)
        res = decomp.Resolver(Image(), {a.name: a, b.name: b}, {})
        self.assertEqual(len(res.errors), 1)
        self.assertIn("only an inline or template (COMDAT) copy", res.errors[0])

    def test_a_claim_must_name_one_of_the_copies(self):
        a, _ = unit("game/TGStream", [0x6D2030])
        b, _ = unit("game/TGBufferedFileStream", [0x6D3330])
        self.assertEqual(decomp.Resolver(Image(), {a.name: a, b.name: b}, {NAME: 0x6D3330}).errors, [])
        a, _ = unit("game/TGStream", [0x6D2030])
        b, _ = unit("game/TGBufferedFileStream", [0x6D3330])
        res = decomp.Resolver(Image(), {a.name: a, b.name: b}, {NAME: 0x700000})
        self.assertEqual(len(res.errors), 1)
        self.assertIn("listed at 0x700000", res.errors[0])

    def test_a_single_annotation_against_a_claim_still_conflicts(self):
        a, _ = unit("game/TGStream", [0x6D2030])
        res = decomp.Resolver(Image(), {a.name: a}, {NAME: 0x6D3330})
        self.assertEqual(res.errors, ["%s annotated at 0x6D2030 and listed at 0x6D3330" % NAME])

    def test_a_referring_unit_takes_one_copy_at_every_use(self):
        a, _ = unit("game/TGStream", [0x6D2030])
        b, _ = unit("game/TGBufferedFileStream", [0x6D3330])
        c, sc = unit("game/Caller", [], defined=False)
        res = decomp.Resolver(Image(), {a.name: a, b.name: b, c.name: c}, {})
        self.assertEqual(res.resolve(c, sc, 0x6D3330, 0), (0x6D3330, None))
        self.assertEqual(res.resolve(c, sc, 0x6D3330, 0), (0x6D3330, None))
        addr, problem = res.resolve(c, sc, 0x6D2030, 0)
        self.assertIsNone(addr)
        self.assertIn("copies at 0x6D3330 and 0x6D2030 in one unit", problem)

    def test_a_referring_unit_cannot_name_an_unannotated_address(self):
        a, _ = unit("game/TGStream", [0x6D2030])
        b, _ = unit("game/TGBufferedFileStream", [0x6D3330])
        c, sc = unit("game/Caller", [], defined=False)
        res = decomp.Resolver(Image(), {a.name: a, b.name: b, c.name: c}, {})
        addr, problem = res.resolve(c, sc, 0x700000, 0)
        self.assertIsNone(addr)
        self.assertIn("unknown referent", problem)


if __name__ == "__main__":
    unittest.main()
