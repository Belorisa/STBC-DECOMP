"""Tests of tools/framemap.py's frame reading: ESP tracking, slot collection, pairing, CodeView types and the
reading line. The compiler isn't needed: the instruction lists are written out.

    python -m unittest discover -s tools/tests
"""

import struct
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import framemap as F  # noqa: E402


def code(lines, base=0x401000):
    """[Ins] from "mnemonic operands" lines, one byte apart (addresses only order them; jumps name indices)."""
    out = []
    for k, text in enumerate(lines):
        m, _, o = text.partition(" ")
        o = o.strip()
        if o.startswith("@"):
            o = hex(base + int(o[1:]))
        out.append(F.Ins(base + k, m, o, 1))
    return out


# CloseRedbookCD's shape: a 0x38 frame, two saves at entry, two deferred, an early return, a stdcall import call
REDBOOK = [
    "sub esp, 0x38",                    # 0
    "push ebp",
    "push esi",
    "mov esi, ecx",
    "xor ebp, ebp",
    "mov eax, dword ptr [esi + 8]",     # 5
    "cmp eax, ebp",
    "jne @12",
    "pop esi",
    "pop ebp",
    "add esp, 0x38",                    # 10
    "ret",
    "push ebx",                         # 12
    "push edi",
    "lea ecx, [esp + 0x14]",
    "push ecx",                         # 15
    "push ebp",
    "push 0x808",
    "push eax",
    "call dword ptr [0x88833c]",
    "mov dword ptr [esp + 0x10], eax",  # 20
    "pop edi",
    "pop ebx",
    "pop esi",
    "pop ebp",
    "add esp, 0x38",
    "ret",
]


class FrameTests(unittest.TestCase):
    def test_roles(self):
        ins = code(REDBOOK)
        roles = F.push_roles(ins)
        self.assertEqual({k for k, r in roles.items() if r == "save"}, {1, 2, 12, 13})
        self.assertNotIn(15, roles)   # an argument

    def test_track_and_slots(self):
        ins = code(REDBOOK)
        frame = F.frame_of(ins, {})
        self.assertEqual(frame.alloc, 0x38)
        self.assertEqual(frame.saves, ["ebp", "esi", "ebx", "edi"])
        # the early return's path doesn't disturb the branch target's delta
        self.assertEqual(ins[12].delta, -0x40)
        # after the stdcall import pops its four arguments, esp is back at the saves
        self.assertEqual(ins[20].delta, -0x48)
        self.assertIn(-0x34, frame.slots)            # lea ecx, [esp + 0x14] with four pushes made
        self.assertEqual(frame.slots[-0x34].access["&arg"], 1)
        self.assertIn(-0x38, frame.slots)            # [esp + 0x10] after the call
        self.assertEqual(sorted(frame.locals()), [-0x38, -0x34])

    def test_cdecl_add_esp(self):
        ins = code(["push ecx", "push 1", "call @9", "add esp, 4", "mov dword ptr [esp], eax", "pop ecx", "ret"])
        frame = F.frame_of(ins, {})
        self.assertEqual(frame.alloc, 4)
        self.assertEqual(ins[4].delta, -4)
        self.assertIn(-4, frame.slots)

    def test_known_pop(self):
        # a nested call: the outer constant is pushed before the inner call, which pops only its own argument
        ins = code(["sub esp, 8", "push 5", "push 7", "call @9", "push eax", "call @9",
                    "mov dword ptr [esp], eax", "add esp, 8", "ret"])
        F.frame_of(ins, {3: 4, 5: 8})
        self.assertEqual(ins[4].delta, -0xC)
        self.assertEqual(ins[6].delta, -8)

    def test_eh_registration(self):
        ins = code(["push -1", "push 0x401000", "mov eax, dword ptr fs:[0]", "push eax",
                    "mov dword ptr fs:[0], esp", "sub esp, 8", "push esi",
                    "mov dword ptr [esp + 0x14], 0", "mov dword ptr [esp + 4], ecx", "pop esi", "add esp, 8", "ret"])
        roles = F.push_roles(ins)
        self.assertEqual([roles.get(k) for k in (0, 1, 3, 6)], ["eh", "eh", "eh", "save"])
        frame = F.frame_of(ins, {})
        self.assertEqual(frame.state, -4)
        self.assertEqual(frame.state_stores, 1)
        self.assertEqual(sorted(frame.locals()), [-0x14])


    def test_jump_table(self):
        # a switch: the cases are reached only through the table; data after the last return is not code
        ins = code(["sub esp, 4", "jmp dword ptr [eax*4 + 0x500000]", "mov dword ptr [esp], 1", "add esp, 4", "ret",
                    "mov dword ptr [esp], 2", "add esp, 4", "ret", "add byte ptr [ebp + ebx - 0x7a], al"])
        frame = F.frame_of(ins, {}, lambda i: [0x401002, 0x401005])
        self.assertTrue(ins[5].reached)
        self.assertFalse(ins[8].reached)
        self.assertEqual(frame.slots[-4].writes, 2)
        self.assertEqual(F.table_disp(ins[1]), 0x500000)
        self.assertEqual(F.table_disp(F.Ins(0, "jmp", "dword ptr [eax*4]", 7)), 0)


    def test_symbol_pop(self):
        self.assertEqual(F.symbol_pop("_sprintf"), 0)
        self.assertEqual(F.symbol_pop("_mciSendCommandA@16"), 16)
        self.assertEqual(F.symbol_pop("__imp__auxGetNumDevs@0"), 0)
        self.assertEqual(F.symbol_pop("?F@@YAXXZ"), 0)
        self.assertEqual(F.symbol_pop("?H@C@@SAXXZ"), 0)
        self.assertIsNone(F.symbol_pop("?G@C@@QAEXXZ"))   # __thiscall: the callee pops its arguments
        self.assertIsNone(F.symbol_pop("?I@@YGXH@Z"))


class PairingTests(unittest.TestCase):
    def test_pair_by_alignment(self):
        ours = code(["sub esp, 8", "mov dword ptr [esp + 4], eax", "mov dword ptr [esp], ecx",
                     "mov eax, dword ptr [esp + 4]", "add esp, 8", "ret"])
        retail = code(["sub esp, 8", "mov dword ptr [esp], eax", "mov dword ptr [esp + 4], ecx",
                       "mov eax, dword ptr [esp]", "add esp, 8", "ret"])
        of, rf = F.frame_of(ours, {}), F.frame_of(retail, {})
        pairs = F.align(ours, retail)
        slotmap, shared = F.pair_slots(of, rf, pairs)
        self.assertEqual(slotmap, {-4: -8, -8: -4})
        self.assertEqual(shared, {})

    def test_shared_retail_slot(self):
        ours = code(["sub esp, 8", "mov dword ptr [esp + 4], eax", "lea ecx, [esp]", "push ecx", "call @9",
                     "add esp, 8", "ret"])
        retail = code(["push ecx", "mov dword ptr [esp], eax", "lea ecx, [esp]", "push ecx", "call @9",
                       "pop ecx", "ret"])
        of, rf = F.frame_of(ours, {4: 4}), F.frame_of(retail, {4: 4})
        slotmap, shared = F.pair_slots(of, rf, F.align(ours, retail))
        self.assertEqual(len(slotmap), 1)
        self.assertEqual(list(shared.values()), [-4])


class SourceTests(unittest.TestCase):
    def test_declarations(self):
        body = {10: "bool F::G( int count, char *name ) {",
                11: "\tMCI_GENERIC_PARMS stop;",
                12: "\tif ( count ) {",
                13: "\t\tDWORD both = count * 2;",
                14: "\t\tfor ( int i = 0; i < count; i++ ) {",
                15: "\t\t\tAUXCAPS caps;",
                16: "\t\t}",
                17: "\t}",
                18: "\treturn false;",
                19: "}"}
        d = F.declarations(body)
        self.assertEqual(d["count"][2], 0)
        self.assertEqual(d["stop"], ("MCI_GENERIC_PARMS", 11, 1))
        self.assertEqual(d["both"][2], 2)
        self.assertEqual(d["caps"][2], 3)
        self.assertNotIn("return", d)

    def test_types(self):
        def rec(leaf, body):
            body += b"\xf1" * ((-(len(body) + 2)) % 4)
            return struct.pack("<HH", len(body) + 2, leaf) + body
        struct_rec = struct.pack("<HHIII", 1, 0, 0, 0, 0) + struct.pack("<H", 0x30) + b"\x07AUXCAPS"
        fwd_rec = struct.pack("<HHIII", 0, 0x80, 0, 0, 0) + struct.pack("<H", 0) + b"\x07AUXCAPS"
        ptr_rec = struct.pack("<II", 0x1001, 0x40A)
        data = struct.pack("<I", 2) + rec(0x1005, struct_rec) + rec(0x1005, fwd_rec) + rec(0x1002, ptr_rec)
        t = F.Types(data)
        self.assertEqual(t.describe(0x1000), ("AUXCAPS", 0x30))
        self.assertEqual(t.describe(0x1001), ("AUXCAPS", 0x30))   # a forward reference takes the full size
        self.assertEqual(t.describe(0x1002), ("AUXCAPS *", 4))
        self.assertEqual(t.describe(0x74), ("int", 4))
        self.assertEqual(t.describe(0x403), ("void *", 4))

    def test_label(self):
        vs = [F.Var("desc", -0x88, "DDSURFACEDESC2", 0x7C, None), F.Var("i", -0xB0, "int", 4, None)]
        self.assertEqual(F.label(vs, -0x64)[0], "desc+0x24")
        self.assertEqual(F.label(vs, -0xB0)[0], "i")
        self.assertEqual(F.label(vs, -0x8)[0], "")


class ReadingTests(unittest.TestCase):
    def analysis(self, ralloc, oalloc, **kw):
        a = F.Analysis()
        a.rf, a.of = F.Frame([], ralloc, False, {}), F.Frame([], oalloc, False, {})
        a.shared_ours, a.shared_retail, a.copies, a.only_r, a.only_o = [], [], [], [], []
        a.inversions, a.rows, a.reg_swaps, a.write_kinds, a.split, a.merges = [], [], [], [], {}, []
        for k, v in kw.items():
            setattr(a, k, v)
        return a

    def test_readings(self):
        self.assertIn("ours shares one slot between numDevices and stop",
                      F.read_pattern(self.analysis(0x38, 0x34, shared_ours=[["numDevices", "stop"]])))
        self.assertIn("retail copies desc+0x24", F.read_pattern(self.analysis(0xD8, 0xD4, copies=[(-0xB8,
                                                                                                    "desc+0x24")])))
        self.assertIn("retail keeps player and count in one slot",
                      F.read_pattern(self.analysis(4, 8, shared_retail=[("count", "player", -4)])))
        self.assertIn("declaration order of a and b", F.read_pattern(self.analysis(8, 8, inversions=[("a", "b")])))
        self.assertIn("register roles swapped",
                      F.read_pattern(self.analysis(8, 8, reg_swaps=[("count", "esi", "edi")])))
        self.assertIn("through the FPU",
                      F.read_pattern(self.analysis(8, 8, write_kinds=[("power", "float", "int")])))
        self.assertIn("one variable reused",
                      F.read_pattern(self.analysis(8, 8, merges=[("subdirectoryHashes", "nameHashes", "-0x40C")])))
        self.assertIn("agree", F.read_pattern(self.analysis(8, 8)))
        self.assertIn("never used", F.read_pattern(self.analysis(0x18, 0x10)))


if __name__ == "__main__":
    unittest.main()
