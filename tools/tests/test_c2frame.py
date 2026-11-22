"""Tests of c2frame.py's readers on fake memory: the locals list, the slot sort key and the final offsets.

    python -m unittest discover -s tools/tests
"""

import struct
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import c2dbg  # noqa: E402
import c2frame  # noqa: E402


def local(rec, kind, size, nxt, refs, lid, offset=0):
    d = bytearray(0x40)
    struct.pack_into("<I", d, 0x00, rec)
    d[4] = kind
    struct.pack_into("<i", d, 0x20, size)
    struct.pack_into("<i", d, 0x28, offset)
    struct.pack_into("<I", d, 0x2C, nxt)
    struct.pack_into("<i", d, 0x34, refs)
    struct.pack_into("<I", d, 0x38, lid)
    return bytes(d)


def record(name_ptr, offset):
    d = bytearray(0x20)
    struct.pack_into("<i", d, 0x0C, offset)
    struct.pack_into("<I", d, 0x18, name_ptr)
    return bytes(d)


class Frame(unittest.TestCase):
    def setUp(self):
        # a temporary named "_buf" whose record holds offset -0x44, then a parameter (kind 5) at +8 with no name
        self.mem = c2dbg.FakeMemory({0x1000: local(0x2000, 4, 64, 0x1100, 3, 1),
                                     0x1100: local(0, 5, 4, 0, 1, 0, offset=8),
                                     0x2000: record(0x3000, -0x44),
                                     0x3000: b"_buf\0"})

    def test_walk(self):
        locs = c2frame.walk_locals(self.mem, 0x1000)
        self.assertEqual([a for a, _ in locs], [0x1000, 0x1100])
        self.assertEqual((locs[0][1]["size"], locs[0][1]["refs"], locs[0][1]["id"]), (64, 3, 1))
        self.assertEqual(c2frame.walk_locals(self.mem, 0), [])

    def test_names_and_offsets(self):
        self.assertEqual(c2frame.local_name(self.mem, 0x1000), "buf (temp #1)")
        self.assertEqual(c2frame.local_name(self.mem, 0x1100), "param #0")
        self.assertEqual(c2frame.final_offset(self.mem, 0x1000), -0x44)
        self.assertEqual(c2frame.final_offset(self.mem, 0x1100), 8)

    def test_sort_key(self):
        self.assertEqual(c2frame.sort_key({"refs": 3, "size": 16}), 187)
        self.assertEqual(c2frame.sort_key({"refs": 3, "size": 0}), 0)


if __name__ == "__main__":
    unittest.main()
