"""Tests of decomp.name_unresolved: the diff names an operand whose relocation didn't resolve instead of printing the
retail dword Resolver.relocate leaves there.

    python -m unittest discover -s tools/tests
"""

import struct
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import decomp  # noqa: E402


class NameUnresolved(unittest.TestCase):
    def rows(self, code, unresolved, addr=0x401000):
        return decomp.name_unresolved(decomp.disasm_sized(code, addr), code, addr, unresolved)

    def test_immediate_store_names_the_symbol(self):
        # mov dword ptr [esi], 0x12c85: retail's bytes at ours' operand, part of another instruction there
        code = b"\xc7\x06" + struct.pack("<I", 0x12C85)
        self.assertEqual(self.rows(code, {0x401002: "??_7PhaserGlow@@6B@"}),
                         [(0x401000, "mov", "dword ptr [esi], ???_7PhaserGlow@@6B@")])

    def test_direct_call_names_the_symbol(self):
        code = b"\xe8" + struct.pack("<I", 0xC7574F54)
        self.assertEqual(self.rows(code, {0x401001: "?IncRefCount@NiObject@@QAEXXZ"}),
                         [(0x401000, "call", "??IncRefCount@NiObject@@QAEXXZ")])

    def test_resolved_operands_are_left_alone(self):
        code = b"\xc7\x06" + struct.pack("<I", 0x88D4F0) + b"\xe8" + struct.pack("<I", 0x10)
        self.assertEqual(self.rows(code, {}), [(0x401000, "mov", "dword ptr [esi], 0x88d4f0"),
                                               (0x401006, "call", "0x40101b")])

    def test_displacement_keeps_the_rest_of_the_operand(self):
        # mov eax, dword ptr [ecx*4 + 0x991068]
        code = b"\x8b\x04\x8d" + struct.pack("<I", 0x24448C8C)
        self.assertEqual(self.rows(code, {0x401003: "_swig_types"}),
                         [(0x401000, "mov", "eax, dword ptr [ecx*4 + ?_swig_types]")])


if __name__ == "__main__":
    unittest.main()
