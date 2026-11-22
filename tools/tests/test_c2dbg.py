"""Tests of c2dbg.py's pure parts: addresses, memory readers, struct layouts, return matching, the command line.

    python -m unittest discover -s tools/tests
"""

import struct
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import c2dbg  # noqa: E402


def pe_image(image_base, stamp, size, name=b"C2.DLL"):
    """The first 0x200 bytes of a PE image with the given header fields and an export directory naming it."""
    d = bytearray(0x200)
    d[0:2] = b"MZ"
    struct.pack_into("<I", d, 0x3C, 0x80)
    d[0x80:0x84] = b"PE\0\0"
    struct.pack_into("<I", d, 0x80 + 8, stamp)
    struct.pack_into("<I", d, 0x80 + 24 + 28, image_base)
    struct.pack_into("<I", d, 0x80 + 24 + 56, size)
    struct.pack_into("<I", d, 0x80 + 24 + 96, 0x100)      # export directory
    struct.pack_into("<I", d, 0x100 + 12, 0x140)          # its Name
    d[0x140:0x140 + len(name) + 1] = name + b"\0"
    return bytes(d)


class Addresses(unittest.TestCase):
    def test_parse(self):
        self.assertEqual(c2dbg.parse_address("0x1075CC23"), ("va", 0x1075CC23))
        self.assertEqual(c2dbg.parse_address("1075cc23"), ("va", 0x1075CC23))
        self.assertEqual(c2dbg.parse_address("+0x5CC23"), ("rva", 0x5CC23))
        self.assertEqual(c2dbg.parse_address("rva:5cc23"), ("rva", 0x5CC23))

    def test_relocate(self):
        self.assertEqual(c2dbg.relocate("va", 0x1075CC23, 0x10700000, 0x10700000), 0x1075CC23)
        self.assertEqual(c2dbg.relocate("va", 0x1075CC23, 0x10700000, 0x00A00000), 0x00A5CC23)
        self.assertEqual(c2dbg.relocate("rva", 0x5CC23, 0x10700000, 0x00A00000), 0x00A5CC23)
        with self.assertRaises(ValueError):
            c2dbg.relocate("va", 0x5CC23, 0x10700000, 0x10700000)

    def test_pe_header(self):
        self.assertEqual(c2dbg.pe_header(pe_image(0x10700000, 0x39A4A91A, 0xBA000)), (0x10700000, 0x39A4A91A, 0xBA000))
        with self.assertRaises(ValueError):
            c2dbg.pe_header(bytes(0x200))

    def test_parse_bps(self):
        self.assertEqual(c2dbg.parse_bps(["0x10742BBE,+0x10", "0x1075CC23"]),
                         [("va", 0x10742BBE, "0x10742BBE"), ("rva", 0x10, "+0x10"), ("va", 0x1075CC23, "0x1075CC23")])
        self.assertEqual(c2dbg.parse_bps(None), [])

    def test_parse_regs(self):
        self.assertEqual(c2dbg.parse_regs("ECX, edx"), ["ecx", "edx"])
        self.assertEqual(c2dbg.parse_regs(""), [])
        with self.assertRaises(ValueError):
            c2dbg.parse_regs("ecx,r8")


class Readers(unittest.TestCase):
    def setUp(self):
        sym = bytearray(0x80)
        struct.pack_into("<I", sym, 0x18, 0x3000)        # name pointer
        struct.pack_into("<h", sym, 0x6D, -2)            # an unaligned signed word
        struct.pack_into("<I", sym, 0x73, 0x2020)
        struct.pack_into("<I", sym, 0x20, 0x2000)        # a pointer to a nested record (itself)
        self.mem = c2dbg.FakeMemory({0x2000: bytes(sym), 0x3000: b"?Get@Small@@QBEHXZ\0junk",
                                     0x4000: b"A" * 100})

    def test_numbers(self):
        m = self.mem
        self.assertEqual(m.u32(0x2018), 0x3000)
        self.assertEqual(m.s16(0x206D), -2)
        self.assertEqual(m.u16(0x206D), 0xFFFE)
        self.assertEqual(m.u8(0x2073), 0x20)
        self.assertEqual(m.dwords(0x2018, 3), [0x3000, 0, 0x2000])
        with self.assertRaises(OSError):
            m.u32(0x207E)                                 # crosses the block's end
        with self.assertRaises(OSError):
            m.u32(0x9000)

    def test_cstr(self):
        m = self.mem
        self.assertEqual(m.cstr(0x3000), "?Get@Small@@QBEHXZ")
        self.assertIsNone(m.cstr(0))
        self.assertEqual(m.cstr(0x4000, limit=10), "A" * 10)
        self.assertEqual(m.cstr(0x4000), "A" * 100)        # unterminated at the end of readable memory

    def test_struct(self):
        layout = [("name", 0x18, "cstr"), ("cost", 0x6D, "s16"), ("flags", 0x73, "u32"),
                  ("self", 0x20, [("flags", 0x73, "u32")]), ("raw", 0x18, "bytes:4")]
        got = self.mem.struct(0x2000, layout)
        self.assertEqual(got, {"name": "?Get@Small@@QBEHXZ", "cost": -2, "flags": 0x2020,
                               "self": {"flags": 0x2020}, "raw": b"\x00\x30\x00\x00"})
        self.assertEqual(self.mem.struct(0x2000, [("none", 0x0, [("x", 0, "u32")])]), {"none": None})


class Returns(unittest.TestCase):
    def test_pick_return(self):
        cb = object()
        pending = [(1, 0x1000, cb), (1, 0x0F00, cb), (2, 0x1000, cb)]
        self.assertEqual(c2dbg.pick_return(pending, 1, 0x1004), 0)      # cdecl: esp = entry + 4
        self.assertEqual(c2dbg.pick_return(pending, 1, 0x0F0C), 1)      # stdcall popped 8 bytes: the inner call
        self.assertEqual(c2dbg.pick_return(pending, 2, 0x1004), 2)
        self.assertIsNone(c2dbg.pick_return(pending, 1, 0x0F00))        # not returned yet
        self.assertIsNone(c2dbg.pick_return(pending, 3, 0x1004))


class CommandLine(unittest.TestCase):
    def test_env_block(self):
        self.assertEqual(c2dbg.env_block({"b": "2", "A": "1"}), "A=1\0b=2\0\0")

    def test_compile_command_source(self):
        out = Path(__file__).resolve().parents[2] / "build" / "c2dbg"
        argv, compiler = c2dbg.compile_command(source="x/y.cpp", flags=["/O2", "/W4"], out=out)
        self.assertIsNone(compiler)
        self.assertTrue(argv[0].upper().endswith("CL.EXE"))
        self.assertEqual(argv[1:5], ["/nologo", "/c", "/O2", "/W4"])
        self.assertEqual(argv[5], "/Tp")
        self.assertEqual(argv[-1], "/Fo" + str(out / "y.obj"))
        argv, _ = c2dbg.compile_command(source="z.c", out=out)
        self.assertIn("/Tc", argv)

    def test_format_hit(self):
        self.assertEqual(c2dbg.format_hit("0x10742BBE", 3, [4, 0x2C6], {"ecx": 0x10}, "f"),
                         "0x10742BBE #3 args=[0x4, 0x2C6] ecx=0x10 in f")
        self.assertEqual(c2dbg.format_hit("+0x10", 1, None, {}), "+0x10 #1")

    def test_parser(self):
        a = c2dbg.build_parser().parse_args(["game/X", "--bp", "0x1", "--bp", "+0x2", "--ret", "--args", "3"])
        self.assertEqual((a.unit, a.bp, a.ret, a.args), ("game/X", ["0x1", "+0x2"], True, 3))


class Probes(unittest.TestCase):
    def test_registry_and_install(self):
        saved = list(c2dbg.REGISTRY)
        try:
            @c2dbg.probe(0x1075CC23)
            def one(dbg, ctx):
                pass

            @c2dbg.probe("+0x10", module="C1XX.DLL", name="two")
            def two(dbg, ctx):
                pass

            added = []

            class Dbg:
                def add_module_bp(self, module, kind, value, handler):
                    added.append((module, kind, value, handler))

            c2dbg.install(Dbg(), c2dbg.REGISTRY[len(saved):])
            self.assertEqual(added, [("c2.dll", "va", 0x1075CC23, one), ("c1xx.dll", "rva", 0x10, two)])
            self.assertEqual(c2dbg.REGISTRY[-1].name, "two")
        finally:
            c2dbg.REGISTRY[:] = saved

    def test_module_load_relocates(self):
        """A module breakpoint is armed at the load base, from the image base in the module's own header."""
        dbg = c2dbg.Debugger(["cl.exe"])
        dbg.cur = c2dbg._Process(1, None)
        img = pe_image(0x10700000, 0x39A4A91A, 0xBA000)
        dbg.read = lambda addr, n, partial=False: img[addr - 0x00A00000:addr - 0x00A00000 + n]
        armed, seen = [], []
        dbg.add = lambda addr, handler: armed.append((addr, handler))
        dbg.add_module_bp("c2.dll", "va", 0x1075CC23, "h1")
        dbg.add_module_bp("c2.dll", "rva", 0x10, "h2")
        dbg.add_module_bp("c1xx.dll", "rva", 0x10, "h3")
        dbg.on_module("C2.DLL", seen.append)
        dbg._loaded(0x00A00000, "")          # no file path: the header is read from memory
        self.assertEqual(armed, [(0x00A5CC23, "h1"), (0x00A00010, "h2")])
        self.assertEqual(seen, [0x00A00000])
        self.assertEqual(c2dbg.ghidra_address(dbg, 0x00A5CC23), 0x1075CC23)
        self.assertEqual(c2dbg.ghidra_address(dbg, 0x5), 0x5)


if __name__ == "__main__":
    unittest.main()
