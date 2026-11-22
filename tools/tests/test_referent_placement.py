"""Tests of decomp.py's referent placement: a unit-local referent (a `__real` constant) takes the address retail uses
at the first site that places it, and only a site whose instruction is retail's own may place it. A function
misaligned against retail reads a garbage "retail address" at ours' offset; when the garbage happens to hold the same
bytes (0.0f is four zeros), it used to fix the constant there and fail a matching function of the same unit
(NiParticleSystemController::Update, 0x7B1B90, after its unit's open 0x7B0330 was respelled).

    python -m unittest discover -s tools/tests
"""

import struct
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import coff  # noqa: E402
import decomp  # noqa: E402

TEXT = 0x60000020
RDATA = 0x40000040 | coff.SCN_LNK_COMDAT
REAL = "__real@4@00000000000000000000"
F1, F2, F3 = 0x401000, 0x401010, 0x401020
GARBAGE, CONST, OTHER = 0x502000, 0x503000, 0x504000


class Image:
    def __init__(self, regions):
        self.regions = regions        # {address: bytes}

    def read(self, addr, size):
        for base, data in self.regions.items():
            if base <= addr and addr + size <= base + len(data):
                return data[addr - base:addr - base + size]
        return None

    def dword(self, addr):
        b = self.read(addr, 4)
        return None if b is None else struct.unpack("<I", b)[0]


def pad(code):
    return code + b"\xCC" * (16 - len(code))


def scenario(f3=None):
    """One unit: F1 (ours misaligned against retail), F2 (identical to retail), optionally F3; each loads REAL."""
    ours = pad(b"\x55\xD9\x05\0\0\0\0\x5D\xC3") + pad(b"\xD9\x05\0\0\0\0\xC3")
    relocs = [(3, 3, coff.IMAGE_REL_I386_DIR32), (0x12, 3, coff.IMAGE_REL_I386_DIR32)]
    retail = pad(b"\x55\x8B\xEC" + struct.pack("<I", GARBAGE) + b"\x5D\xC3") + \
        pad(b"\xD9\x05" + struct.pack("<I", CONST) + b"\xC3")
    if f3 is not None:
        ours += pad(b"\xD9\x05\0\0\0\0\x90\xC3")
        relocs.append((0x22, 3, coff.IMAGE_REL_I386_DIR32))
        retail += pad(b"\xD9\x05" + struct.pack("<I", f3) + b"\xC3")
    o = object.__new__(coff.Coff)
    o.sections = [coff.Section(1, ".text", ours, len(ours), TEXT, relocs),
                  coff.Section(2, ".rdata", b"\0\0\0\0", 4, RDATA, [])]
    o.symbols = {0: coff.Symbol(0, "?f1@@YAXXZ", 0, 1, 0x20, coff.SC_EXTERNAL),
                 1: coff.Symbol(1, "?f2@@YAXXZ", 0x10, 1, 0x20, coff.SC_EXTERNAL),
                 2: coff.Symbol(2, "?f3@@YAXXZ", 0x20, 1, 0x20, coff.SC_EXTERNAL),
                 3: coff.Symbol(3, REAL, 0, 2, 0, coff.SC_EXTERNAL)}
    u = decomp.Unit("game/Particles", o, [])
    for i, a in enumerate((F1, F2, F3)[:3 if f3 is not None else 2]):
        u.entries.append(decomp.Entry("FUNCTION", a, u.name, None, 1, "f%d" % (i + 1), None, ["void f() {}"], 0,
                                      None))
        u.symbols[a] = o.symbols[i]
    img = Image({F1: retail, GARBAGE: b"\0" * 4, CONST: b"\0" * 4, OTHER: b"\0" * 4})
    return decomp.Resolver(img, {u.name: u}, {}), u


def status(res, u, addr):
    return decomp.compare_bytes(res, u, next(e for e in u.entries if e.addr == addr))[0]


class Placement(unittest.TestCase):
    def test_a_misaligned_site_does_not_place(self):
        res, u = scenario()
        r1 = status(res, u, F1)
        self.assertEqual(r1["status"], "DIFF")
        self.assertNotIn(2, u.bases)                 # F1's garbage placement was undone
        r2 = status(res, u, F2)
        self.assertEqual(r2["status"], "MATCH")
        self.assertEqual(u.bases[2], CONST)

    def test_the_old_order_dependence(self):
        # what relocating F1 without the trust check did: the garbage address holds zeros, so it verified
        res, u = scenario()
        sec = u.obj.section(1)
        res.relocate(u, sec, 0, 0x10, F1)
        self.assertEqual(u.bases[2], GARBAGE)
        data, problems = res.relocate(u, sec, 0x10, 0x20, F2)
        self.assertEqual(struct.unpack_from("<I", data, 2)[0], GARBAGE)

    def test_an_agreeing_instruction_places_in_a_non_matching_function(self):
        # F3 differs from retail after its load (nop against ret), but the load itself is retail's instruction
        res, u = scenario(f3=CONST)
        self.assertEqual(status(res, u, F3)["status"], "DIFF")
        self.assertEqual(u.bases[2], CONST)
        self.assertEqual(status(res, u, F2)["status"], "MATCH")

    def test_one_datum_one_address(self):
        # a second trusted site naming another retail copy of the constant is still a mismatch (two spellings)
        res, u = scenario(f3=OTHER)
        self.assertEqual(status(res, u, F2)["status"], "MATCH")
        r3 = status(res, u, F3)
        self.assertEqual(r3["status"], "DIFF")
        self.assertEqual(u.bases[2], CONST)

    def test_agreeing_sites(self):
        code = b"\x55\xD9\x05\0\0\0\0\x5D\xC3"
        relocs = [(3, 0, coff.IMAGE_REL_I386_DIR32)]
        same = b"\x55\xD9\x05\x11\x22\x33\x44\x5D\xC3"
        self.assertEqual(decomp.agreeing_sites(code, relocs, 0, same), {3})
        shifted = b"\x55\x8B\xEC\x11\x22\x33\x44\x5D\xC3"
        self.assertEqual(decomp.agreeing_sites(code, relocs, 0, shifted), set())
        self.assertEqual(decomp.agreeing_sites(code, relocs, 0, None), set())
        # an earlier difference of the same length (another register) leaves the site's instruction aligned
        code2 = b"\x8B\xC1\xD9\x05\0\0\0\0\xC3"
        retail2 = b"\x8B\xC2\xD9\x05\x11\x22\x33\x44\xC3"
        self.assertEqual(decomp.agreeing_sites(code2, [(4, 0, coff.IMAGE_REL_I386_DIR32)], 0, retail2), {4})


if __name__ == "__main__":
    unittest.main()
