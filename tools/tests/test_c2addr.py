"""Tests of tools/c2addr.py: the hash arithmetic of sub_1070DB59, the flip search and the shift table on fake memory,
and (when the toolchain is present) a live run checking docs/c2-backend.md's rule that the operand the commutative sort
puts first becomes the base of a two-register address, and that the predicted shift flips it.

    python -m unittest discover -s tools/tests
"""

import io
import struct
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import c2addr as A  # noqa: E402
import c2dbg  # noqa: E402


def node(op, kind, weight=0, desc=0, disp=0, base=0, index=0):
    d = bytearray(0x30)
    struct.pack_into("<I", d, 0x04, op)
    struct.pack_into("<I", d, 0x08, kind)
    struct.pack_into("<I", d, 0x0C, weight)
    struct.pack_into("<I", d, 0x14, desc)
    struct.pack_into("<i", d, 0x24, disp)
    struct.pack_into("<I", d, 0x28, base)
    struct.pack_into("<I", d, 0x2C, index)
    return bytes(d)


def descriptor(number, var_kind=3, expr=0, record=0):
    d = bytearray(0x20)
    struct.pack_into("<I", d, 0x00, record)
    d[0x04] = var_kind
    struct.pack_into("<I", d, 0x14, expr)
    struct.pack_into("<I", d, 0x1C, number)
    return bytes(d)


class Hash(unittest.TestCase):
    def test_fold(self):
        self.assertEqual(A.fold(0x00012345), 0x2344)
        self.assertEqual(A.fold(-8), 0xFFFF ^ 0xFFF8)

    def test_leaf(self):
        # a back-end temporary or local (descriptor +4 == 3): number << 6; a parameter: number << 5 (the doc's _a #2)
        self.assertEqual(A.leaf_hash(165, 3, 2), 0x2940)
        self.assertEqual(A.leaf_hash(1225, 3, 2), 0x3240)
        self.assertEqual(A.leaf_hash(2, 5, 2), 0x40)

    def terms(self):
        mem = A.Term("memory", "memory", number=0, const=7,
                     parts=[A.Term("leaf", "temp", number=166, var_kind=3, node_kind=2)])
        tmp = A.Term("leaf", "temp", number=165, var_kind=3, node_kind=2)
        return mem, tmp

    def test_memory_takes_the_low_byte_of_its_base(self):
        mem, tmp = self.terms()
        self.assertEqual(mem.hash(), 0x8007)           # 7 + ((166 << 6) << 8), 16 bits
        self.assertEqual(tmp.hash(), 0x2940)
        self.assertEqual(mem.show(), "[temp #166]")

    def test_flips(self):
        mem, tmp = self.terms()
        d = A.flips(mem, tmp, 0x18007, 0x12940, 165)
        self.assertIn(2, d)
        self.assertIn(-2, d)
        self.assertNotIn(1, d)
        self.assertEqual(A.nearest(d), "+2 or -2")
        self.assertEqual(A.flip_report(mem, tmp, 0x18007, 0x12940)[0],
                         "flip: symbols numbered before both: +2 or -2")

    def test_shift_table(self):
        mem, tmp = self.terms()
        far = A.Term("leaf", "temp", number=944, var_kind=3, node_kind=2)
        near = A.Term("leaf", "temp", number=702, var_kind=3, node_kind=2)
        sites = [("S1", mem, tmp, 0x18007, 0x12940), ("S2", far, near, 0x1EC00, 0x1AF80)]
        rows = A.shift_table(sites)
        self.assertIn((2, 165, 166, ["S1"]), rows)
        self.assertFalse([r for r in rows if "S2" in r[3]])     # 944 needs +80 to wrap past 1024
        self.assertIn("+2 at P <= 166: S1", A.table_lines(rows, sites))
        self.assertEqual(A.flips(far, near, 0x1EC00, 0x1AF80, 944)[:1], [80])


class Read(unittest.TestCase):
    def test_term_of_memory_over_a_temporary(self):
        mem = c2dbg.FakeMemory({
            0x1000: node(0x14C, 6, 0x18007, disp=0, base=0x1100),
            0x1100: node(0x149, 2, 0, desc=0x2000),
            0x2000: descriptor(166),
            0x1200: node(0x14A, 2, 0x12940, desc=0x2100),
            0x2100: descriptor(165),
            0x1300: node(0x14A, 2, 0, desc=0x2200),            # a common subexpression: hashes as its expression
            0x2200: descriptor(9, var_kind=4, expr=0x1200)})
        t = A.term(mem, 0x1000)
        self.assertEqual((t.kind, t.hash(), t.show()), ("memory", 0x8007, "[temp #166]"))
        self.assertEqual(A.term(mem, 0x1200).hash(), 0x2940)
        c = A.term(mem, 0x1300)
        self.assertEqual((c.kind, c.hash(), c.show()), ("cse", 0x2940, "temp #9{temp #165}"))

    def test_operand_text(self):
        self.assertEqual(A.operand_text("eax", "ecx", -8), "[eax+ecx-8]")
        self.assertEqual(A.operand_text("ecx", "eax", 12), "[ecx+eax+12]")
        self.assertEqual(A.operand_text("edx", "eax", 0), "[edx+eax]")

    def test_explain_names_the_first_sorted_operand_the_base(self):
        mem = A.Term("memory", "memory", number=0, const=7,
                     parts=[A.Term("leaf", "temp", number=166, var_kind=3, node_kind=2)])
        tmp = A.Term("leaf", "temp", number=165, var_kind=3, node_kind=2)
        rec = {"a": 1, "b": 2, "wa": 0x12940, "wb": 0x18007, "ta": tmp, "tb": mem, "res": 1, "seq": 1}
        lines = A.explain({"base": 9, "index": 1, "rec": rec}, "eax", "ebx")
        self.assertTrue(lines[0].startswith("base eax  <- [temp #166]"))
        self.assertTrue(lines[1].startswith("index ebx <- temp #165"))
        self.assertNotIn("note:", " ".join(lines))


@unittest.skipUnless((Path(A.__file__).parent / "vc6" / "VC98" / "BIN" / "C2.DLL").exists() and sys.platform == "win32",
                     "needs tools/vc6")
class Live(unittest.TestCase):
    # NiTListBlock's constructor (NiTList.h) on its own: the last item's store adds the size*12 common subexpression
    # and the items load; two more symbols numbered before them (pad[5] = 1 creates them) flip the order, as the
    # shift arithmetic predicts for the first build
    SRC = """
struct Item { Item * next; Item * prev; int element; };
struct Block { Block * next; Item * items; Block( unsigned int size ); };
inline Block::Block( unsigned int size ) {
	next = 0;
	items = new Item[size];
	for ( unsigned int i = 0; i < size - 1; i++ ) {
		items[i].next = &items[i + 1];
	}
	items[size - 1].next = 0;
}
struct Owner { int pad[10]; unsigned int blockSize; Block * blocks; void Grow(); };
void Owner::Grow() {
%s	Block * b = new Block( blockSize );
	b->next = blocks;
	blocks = b;
}
"""

    def run_trace(self, before):
        import c2trace as T
        with tempfile.TemporaryDirectory() as d:
            src = Path(d) / "addr.cpp"
            src.write_text(self.SRC % before)
            out = io.StringIO()
            old = sys.stdout
            sys.stdout = out
            try:
                T.main([str(src), "Grow", "--addr"])
            finally:
                sys.stdout = old
        text = out.getvalue()
        site = text[text.index("-12]"):]
        return site.splitlines()

    def test_base_follows_the_sort_and_the_predicted_shift_flips_it(self):
        first = self.run_trace("")
        self.assertIn("<- [temp #", first[1])                 # the items load sorts first: the base
        self.assertRegex(first[1], r"^\s+base ")
        self.assertIn("+2", first[3])
        moved = self.run_trace("\tpad[5] = 1;\n")
        self.assertRegex(moved[1], r"^\s+base \w+ +<- temp #\d+")   # two more symbols: size*12 sorts first
        self.assertIn("<- [temp #", moved[2])


if __name__ == "__main__":
    unittest.main()
