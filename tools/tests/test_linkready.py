import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
import coff  # noqa: E402
import linkready as L  # noqa: E402


class FakeSection:
    def __init__(self, index, name, size, flags):
        self.index, self.name, self.size, self.flags = index, name, size, flags

    is_code = property(lambda self: bool(self.flags & coff.SCN_MEM_EXECUTE))
    is_bss = property(lambda self: bool(self.flags & coff.SCN_CNT_UNINITIALIZED))
    alignment = property(lambda self: 16)


class FakeSymbol:
    def __init__(self, name, section):
        self.name, self.section, self.storage = name, section, coff.SC_EXTERNAL

    defined = property(lambda self: self.section > 0)


class FakeObj:
    def __init__(self, sections, symbols):
        self.sections = sections
        self.symbols = dict(enumerate(symbols))


CODE = coff.SCN_MEM_EXECUTE | coff.SCN_LNK_COMDAT


class TextOffsets(unittest.TestCase):
    def test_sections_are_laid_end_to_end_at_their_alignment(self):
        obj = FakeObj([FakeSection(1, ".text", 0x13, CODE), FakeSection(2, ".text", 0x20, CODE),
                       FakeSection(3, ".text$x", 0x10, CODE), FakeSection(4, ".text", 4, CODE)],
                      [FakeSymbol("a", 1), FakeSymbol("b", 2), FakeSymbol("c", 4)])
        self.assertEqual(L.text_offsets(obj), {1: 0, 2: 0x20, 4: 0x40})

    def test_a_copy_retail_kept_from_another_unit_is_dropped(self):
        obj = FakeObj([FakeSection(1, ".text", 0x10, CODE), FakeSection(2, ".text", 0x10, CODE),
                       FakeSection(3, ".text", 0x10, CODE)],
                      [FakeSymbol("own", 1), FakeSymbol("inline", 2), FakeSymbol("next", 3)])
        self.assertEqual(L.text_offsets(obj, frozenset({"inline"})), {1: 0, 3: 0x10})


class Kinds(unittest.TestCase):
    def test_section_kinds(self):
        self.assertEqual(L.section_kind(FakeSection(1, ".text", 1, CODE)), "text")
        self.assertEqual(L.section_kind(FakeSection(1, ".bss", 1, coff.SCN_CNT_UNINITIALIZED)), "bss")
        self.assertEqual(L.section_kind(FakeSection(1, ".rdata", 1, 0)), "rdata")
        self.assertEqual(L.section_kind(FakeSection(1, ".data", 1, 0)), "data")
        self.assertIsNone(L.section_kind(FakeSection(1, ".drectve", 1, 0)))

    def test_unresolved_kinds(self):
        self.assertEqual(L.unresolved_kind("??_EFoo@@UAEPAXI@Z"), "deleting destructor")
        self.assertEqual(L.unresolved_kind("??_7Foo@@6B@"), "vtable")
        self.assertEqual(L.unresolved_kind("?SetAt@?$NiTMap@HH@@QAEXHH@Z"), "template instance")
        self.assertEqual(L.unresolved_kind("?cubeEdgeFlags@@3QBIB"), "function or data")


if __name__ == "__main__":
    unittest.main()
