"""Tests of decomp.py's tree-wide copy rule (AGENTS.md "Referents": one datum, one address at every use). A COMDAT
constant (`__real@4@...`) has one retail address, the one most functions' own instructions read it at; a site
reading another copy of an equal value is a wrong referent, not a match. A unit-private datum may not sit where an
annotated datum or another name's copy lives.

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
RDATA = 0x40000040
COMDAT = RDATA | coff.SCN_LNK_COMDAT
REAL = "__real@4@3ff583126f0000000000"
CONST, OTHER, THIRD = 0x503000, 0x504000, 0x505000
ONE = struct.pack("<f", 0.001)


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


def unit(name, base, reads, static=False, misaligned=()):
    """A unit of functions at base, base + 0x10, ...: function i loads the constant (`fld dword ptr [x]`) and retail's
    function i reads reads[i] there. static: the constant is a file static, not a COMDAT. misaligned: the indexes of
    functions whose retail code is shifted against ours (a push before the load), so their sites can't place."""
    ours, retail, relocs = b"", b"", []
    for i, addr in enumerate(reads):
        ours += pad(b"\xD9\x05\0\0\0\0\xC3")
        relocs.append((0x10 * i + 2, 9, coff.IMAGE_REL_I386_DIR32))
        if i in misaligned:
            retail += pad(b"\x55\xD9\x05" + struct.pack("<I", addr) + b"\xC3")
        else:
            retail += pad(b"\xD9\x05" + struct.pack("<I", addr) + b"\xC3")
    o = object.__new__(coff.Coff)
    o.sections = [coff.Section(1, ".text", ours, len(ours), TEXT, relocs),
                  coff.Section(2, ".rdata", ONE, 4, RDATA if static else COMDAT, [])]
    o.symbols = {i: coff.Symbol(i, "?f%d%s@@YAXXZ" % (i, name[-1]), 0x10 * i, 1, 0x20, coff.SC_EXTERNAL)
                 for i in range(len(reads))}
    o.symbols[9] = coff.Symbol(9, "_k" if static else REAL, 0, 2, 0,
                               coff.SC_STATIC if static else coff.SC_EXTERNAL)
    u = decomp.Unit(name, o, [])
    for i in range(len(reads)):
        a = base + 0x10 * i
        u.entries.append(decomp.Entry("FUNCTION", a, name, None, 1, "f%d" % i, None, ["void f() {}"], 0, None))
        u.symbols[a] = o.symbols[i]
    return u, retail


def tree(*specs, data=None):
    """Units from (name, base, reads, static, misaligned) specs; data: {address: name} of annotated data, given as a
    unit of DATA entries whose datum is the constant's value."""
    units, regions = {}, {CONST: ONE, OTHER: ONE, THIRD: ONE}
    for name, base, reads, static, misaligned in specs:
        u, retail = unit(name, base, reads, static, misaligned)
        units[name] = u
        regions[base] = retail
    if data:
        o = object.__new__(coff.Coff)
        o.sections = [coff.Section(1, ".rdata", ONE * len(data), 4 * len(data), RDATA, [])]
        o.symbols = {}
        u = decomp.Unit("game/Data", o, [])
        for i, (addr, name) in enumerate(sorted(data.items())):
            o.symbols[i] = coff.Symbol(i, name, 4 * i, 1, 0, coff.SC_EXTERNAL)
            u.entries.append(decomp.Entry("DATA", addr, u.name, None, 1, name, None, ["const float k;"], 0, None))
            u.symbols[addr] = o.symbols[i]
        units[u.name] = u
    res = decomp.Resolver(Image(regions), units, {})
    decomp.prepare_copies(res, {}, cache=False)
    return res, units


def status(res, units, name, i):
    u = units[name]
    base = u.entries[0].addr
    return decomp.compare_bytes(res, u, next(e for e in u.entries if e.addr == base + 0x10 * i))[0]


class Copies(unittest.TestCase):
    def test_the_majority_copy_matches_and_the_other_is_wrong(self):
        res, units = tree(("game/A", 0x401000, [CONST, CONST], False, ()),
                          ("game/B", 0x402000, [OTHER], False, ()))
        self.assertEqual(res.canon[REAL], CONST)
        self.assertEqual(status(res, units, "game/A", 0)["status"], "MATCH")
        self.assertEqual(status(res, units, "game/A", 1)["status"], "MATCH")
        r = status(res, units, "game/B", 0)
        self.assertEqual(r["status"], "WRONG-REFERENT")
        self.assertIn("its copy is at 0x503000", r["problems"][0])

    def test_without_the_rule_each_unit_placed_its_own_copy(self):
        # what the resolver did before: unit B's copy verified by content at the other address
        res, units = tree(("game/A", 0x401000, [CONST, CONST], False, ()),
                          ("game/B", 0x402000, [OTHER], False, ()))
        res.canon, res.misread, res.foreign = {}, {}, {}
        self.assertEqual(status(res, units, "game/B", 0)["status"], "MATCH")

    def test_order_does_not_decide(self):
        # the minority reader compared first, in the same unit, doesn't fix the copy for the others
        res, units = tree(("game/A", 0x401000, [OTHER, CONST, CONST], False, ()))
        self.assertEqual(status(res, units, "game/A", 0)["status"], "WRONG-REFERENT")
        self.assertEqual(status(res, units, "game/A", 1)["status"], "MATCH")
        self.assertEqual(status(res, units, "game/A", 2)["status"], "MATCH")

    def test_a_misaligned_site_neither_votes_nor_fails(self):
        # the misaligned function's "retail address" is noise: it takes the copy and doesn't vote for OTHER
        res, units = tree(("game/A", 0x401000, [CONST], False, ()),
                          ("game/B", 0x402000, [OTHER, OTHER], False, (0, 1)))
        self.assertEqual(res.canon[REAL], CONST)
        r = status(res, units, "game/B", 0)
        self.assertEqual(r["status"], "DIFF")
        self.assertFalse([p for p in r.get("problems", []) if "copy" in p])
        self.assertEqual(status(res, units, "game/A", 0)["status"], "MATCH")

    def test_a_tie_names_no_copy(self):
        res, units = tree(("game/A", 0x401000, [CONST], False, ()),
                          ("game/B", 0x402000, [OTHER], False, ()))
        self.assertNotIn(REAL, res.canon)
        self.assertEqual(status(res, units, "game/A", 0)["status"], "WRONG-REFERENT")
        self.assertEqual(status(res, units, "game/B", 0)["status"], "WRONG-REFERENT")

    def test_a_static_where_a_name_has_its_copy(self):
        # unit B's file static verifies at the shared literal's address by content: it isn't B's datum
        res, units = tree(("game/A", 0x401000, [CONST, CONST], False, ()),
                          ("game/B", 0x402000, [CONST], True, ()))
        r = status(res, units, "game/B", 0)
        self.assertEqual(r["status"], "WRONG-REFERENT")
        self.assertIn("holds " + REAL, r["problems"][0])
        self.assertEqual(status(res, units, "game/A", 0)["status"], "MATCH")

    def test_a_static_at_its_own_address(self):
        res, units = tree(("game/A", 0x401000, [CONST], False, ()),
                          ("game/B", 0x402000, [THIRD, THIRD], True, ()))
        self.assertEqual(status(res, units, "game/B", 1)["status"], "MATCH")

    def test_a_name_read_where_an_annotated_datum_lives(self):
        res, units = tree(("game/A", 0x401000, [CONST], False, ()),
                          data={CONST: "?k@Ship@@2MB"})
        r = status(res, units, "game/A", 0)
        self.assertEqual(r["status"], "WRONG-REFERENT")
        self.assertIn("which holds", r["problems"][0])

    def test_reads_are_retail_own_sites_only(self):
        res, units = tree(("game/A", 0x401000, [CONST], False, ()),
                          ("game/B", 0x402000, [OTHER], False, (0,)))
        reads = decomp.collect_reads(res, {}, cache=False)
        self.assertEqual(reads, {("game/A", 0x401000): [(REAL, True, 2, 0, CONST)]})


if __name__ == "__main__":
    unittest.main()
