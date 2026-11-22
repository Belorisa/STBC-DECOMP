"""Tests of dupescan.py: the relocation-masked signature, the exact and shape tiers, and the gap split.

    python -m unittest discover -s tools/tests
"""

import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import dupescan  # noqa: E402


def body(addr, callee, glob):
    """`mov eax, [glob]; call callee; jmp short +0; ret` assembled at addr."""
    code = bytearray(b"\xa1" + glob.to_bytes(4, "little"))
    rel = (callee - (addr + 10)) & 0xFFFFFFFF
    code += b"\xe8" + rel.to_bytes(4, "little")
    code += b"\xeb\x00\xc3"
    return bytes(code)


class FakeImage:
    def __init__(self, blocks):
        self.blocks = blocks

    def read(self, addr, n):
        for start, data in self.blocks.items():
            if start <= addr and addr + n <= start + len(data):
                return data[addr - start:addr - start + n]
        return None


class Signature(unittest.TestCase):
    def sig(self, addr, callee, glob):
        return dupescan.signature(body(addr, callee, glob), addr, [addr + 1], dupescan.disasm_lite)

    def test_copies_at_two_addresses_are_exact_twins(self):
        a = self.sig(0x401000, 0x500000, 0x900000)
        b = self.sig(0x402000, 0x500000, 0x900000)
        self.assertEqual(a, b)
        self.assertEqual(a[1], (("abs", 0x900000), ("rel", 0x500000)))

    def test_another_callee_is_a_shape_twin(self):
        a = self.sig(0x401000, 0x500000, 0x900000)
        b = self.sig(0x402000, 0x500100, 0x900000)
        self.assertNotEqual(a, b)
        self.assertEqual(a[0], b[0])
        self.assertEqual(dupescan.shape_of(a[1]), dupescan.shape_of(b[1]))

    def test_an_address_inside_the_body_is_its_offset(self):
        code = b"\xb8" + (0x401003).to_bytes(4, "little") + b"\xc3"
        masked, targets = dupescan.signature(code, 0x401000, [0x401001], dupescan.disasm_lite)
        self.assertEqual(targets, (("self", 3),))
        self.assertEqual(masked, b"\xb8\0\0\0\0\xc3")

    def test_gap_functions_skip_padding(self):
        code = b"\xc3" + b"\xcc" * 15 + b"\x33\xc0\xc3" + b"\x90" * 13
        img = FakeImage({0x401000: code + b"\xcc" * 64})
        self.assertEqual(dupescan.gap_functions(img, 0x401000, 0x401020), [(0x401000, 1), (0x401010, 3)])


if __name__ == "__main__":
    unittest.main()
