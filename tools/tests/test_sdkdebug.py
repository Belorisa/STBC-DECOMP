"""Tests of sdkdebug.py: function starts, asserts and file runs of a debug image, the pairing helpers and the names.

    python -m unittest discover -s tools/tests
"""

import struct
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import pe  # noqa: E402
import sdkdebug  # noqa: E402

BASE = 0x400000
TEXT = BASE + 0x1000
DATA = BASE + 0x2000


def image(text, data):
    """A minimal PE32 image: .text at 0x401000 (the entry point), .data at 0x402000."""
    pe_off = 0x40
    opt = pe_off + 24
    hdr = bytearray(0x200)
    struct.pack_into("<I", hdr, 0x3C, pe_off)
    hdr[pe_off:pe_off + 4] = b"PE\0\0"
    struct.pack_into("<HH", hdr, pe_off + 4, 0x14C, 2)
    struct.pack_into("<H", hdr, pe_off + 20, 0xE0)
    struct.pack_into("<I", hdr, opt + 16, TEXT - BASE)
    struct.pack_into("<I", hdr, opt + 28, BASE)
    sec = opt + 0xE0
    raw = 0x200
    body = bytearray()
    for name, va, blob in ((b".text", TEXT, text), (b".data", DATA, data)):
        size = (len(blob) + 0x1FF) & ~0x1FF
        hdr[sec:sec + 8] = name.ljust(8, b"\0")
        struct.pack_into("<IIII", hdr, sec + 8, len(blob), va - BASE, size, raw + len(body))
        body += bytes(blob).ljust(size, b"\0")
        sec += 40
    return pe.Image(bytes(hdr) + bytes(body))


def call(at, target):
    return b"\xE8" + struct.pack("<i", target - (at + 5))


class Builder:
    """Lays out functions in .text and strings in .data."""

    def __init__(self):
        self.text = bytearray()
        self.data = bytearray()

    def string(self, s):
        va = DATA + len(self.data)
        self.data += s.encode() + b"\0"
        return va

    def function(self, body_fn):
        va = TEXT + len(self.text)
        self.text += b"\x55\x8B\xEC" + body_fn(va + 3) + b"\x5D\xC3"
        return va


def assert_body(expr, path, line, assert_fn):
    def body(at):
        code = b"\x6A" + bytes([line]) + b"\x68" + struct.pack("<I", path) + b"\x68" + struct.pack("<I", expr)
        return code + call(at + len(code), assert_fn) + b"\x83\xC4\x0C"
    return body


def build():
    b = Builder()
    b.text += b"\xCC" * 4
    path_a = b.string("C:\\TGSharedExternal\\NDL3\\Src\\NiMain\\NiNode.cpp")
    path_h = b.string("C:\\TGSharedExternal\\NDL3\\Src\\NiMain\\NiTArray.inl")
    path_b = b.string("C:\\TGSharedExternal\\NDL3\\Src\\NiMain\\NiCamera.cpp")
    e1, e2, e3 = b.string("pChild"), b.string("m_pBase"), b.string("m_pkCamera != 0")
    assert_fn = b.function(lambda at: b"\x90")
    # placeholders for the callers: each body is the same length, so lay the assert function out first
    f1 = b.function(assert_body(e1, path_a, 27, assert_fn))
    f2 = b.function(lambda at: b"\x90\x90")
    f3 = b.function(assert_body(e2, path_h, 86, assert_fn))
    f4 = b.function(assert_body(e1, path_a, 40, assert_fn))
    f5 = b.function(assert_body(e3, path_b, 12, assert_fn))
    return image(b.text, b.data), assert_fn, (f1, f2, f3, f4, f5)


class DebugImage(unittest.TestCase):
    def setUp(self):
        img, self.assert_fn, self.fns = build()
        self.dbg = sdkdebug.Debug(img)

    def test_starts_are_the_prologues(self):
        for f in self.fns:
            self.assertIn(f, self.dbg.start_set)
        self.assertEqual(self.dbg.function_of(self.fns[1] + 2), self.fns[1])

    def test_asserts_read_expression_file_and_line(self):
        self.assertEqual(self.dbg.assert_fn, self.assert_fn)
        got = [(a["function"], a["expr"], sdkdebug.stem(a["file"]), a["line"]) for a in self.dbg.asserts]
        self.assertEqual(got, [(self.fns[0], "pChild", "NiNode", 27), (self.fns[2], "m_pBase", "NiTArray", 86),
                               (self.fns[3], "pChild", "NiNode", 40), (self.fns[4], "m_pkCamera != 0", "NiCamera", 12)])

    def test_file_runs_skip_header_asserts_and_hold_what_lies_between(self):
        runs = [(sdkdebug.stem(p), fns) for p, _, _, fns in self.dbg.files()]
        self.assertEqual(runs, [("NiNode", list(self.fns[:4])), ("NiCamera", [self.fns[4]])])


class Helpers(unittest.TestCase):
    def test_plain_name_drops_the_hungarian(self):
        self.assertEqual(sdkdebug.plain_name("m_iRefCount"), "refCount")
        self.assertEqual(sdkdebug.plain_name("m_pkChild"), "child")
        self.assertEqual(sdkdebug.plain_name("ms_pLoaders"), "loaders")
        self.assertEqual(sdkdebug.plain_name("m_uiMaxPlanes"), "maxPlanes")

    def test_short_and_stem(self):
        path = "C:\\TGSharedExternal\\NDL3\\Src\\NiAnimation\\NiPSysController.cpp"
        self.assertEqual(sdkdebug.short(path), "NiAnimation\\NiPSysController.cpp")
        self.assertEqual(sdkdebug.stem(path), "NiPSysController")

    def test_field_at_enters_bases(self):
        types = {"NiObject": {"fields": [{"name": "refCount", "offset": 4}], "bases": []},
                 "NiNode": {"fields": [{"name": "children", "offset": 200}], "bases": [{"name": "NiObject",
                                                                                         "offset": 0}]}}
        self.assertEqual(sdkdebug.field_at(types, "NiNode", 4), ("NiObject", "refCount"))
        self.assertEqual(sdkdebug.field_at(types, "NiNode", 200), ("NiNode", "children"))
        self.assertIsNone(sdkdebug.field_at(types, "NiNode", 8))

    def test_gaps_pair_lone_callees_between_anchors(self):
        pairs = {1: 101, 5: 105}
        # debug: 1, 2 (unpaired), 5; retail: 101, 102 (unpaired), 105
        self.assertEqual(sdkdebug.gaps([1, 2, 5], [101, 102, 105], pairs), [([2], [102])])
        # a debug-only inline before the anchor makes no retail gap
        self.assertEqual(sdkdebug.gaps([7, 1, 5], [101, 105], pairs), [])

    def test_undecorate_type(self):
        self.assertEqual(sdkdebug.undecorate_type(".?AVNiNode@@"), "NiNode")
        self.assertEqual(sdkdebug.undecorate_type(".?AV?$NiTArray@G@@"), ".?AV?$NiTArray@G@@")


class FindBody(unittest.TestCase):
    def test_masked_relocations_match_any_address(self):
        class Dbg:
            tva = TEXT
            code = b"\xCC\x55\x8B\xEC\xB8\x11\x22\x33\x44\x5D\xC3"
            start_set = {TEXT + 1}
        body = b"\x55\x8B\xEC\xB8\0\0\0\0\x5D\xC3"
        self.assertEqual(sdkdebug.find_body(Dbg, body, {4}), [TEXT + 1])
        self.assertEqual(sdkdebug.find_body(Dbg, body, set()), [])


class FrameMask(unittest.TestCase):
    def test_frame_slots_mask_in_either_direction_and_base(self):
        n = sdkdebug.normalize
        self.assertEqual(n("mov eax, dword ptr [ebp - 0x10]"), "mov eax, dword ptr [ebp X]")
        self.assertEqual(n("mov eax, dword ptr [ebp + 8]"), "mov eax, dword ptr [ebp X]")
        self.assertEqual(n("lea ecx, [ebp - 0x1c]"), "lea ecx, [ebp X]")
        self.assertEqual(n("mov eax, dword ptr [ebp - 0x10]", mask_frame=False), "mov eax, dword ptr [ebp - 0x10]")

    def test_other_operands_are_kept(self):
        n = sdkdebug.normalize
        self.assertEqual(n("mov eax, dword ptr [ecx + 0x10]"), "mov eax, dword ptr [ecx + 0x10]")
        self.assertEqual(n("mov dword ptr [ebp], eax"), "mov dword ptr [ebp], eax")
        self.assertEqual(n("sub esp, 0x40"), "sub esp, 0x40")
        self.assertEqual(n("call ?Foo@@YAXXZ"), "call")

    def test_compare_reports_both_ratios(self):
        ours = ["push ebp", "mov ebp, esp", "mov eax, dword ptr [ebp - 4]", "pop ebp", "ret"]
        theirs = ["push ebp", "mov ebp, esp", "mov eax, dword ptr [ebp - 8]", "pop ebp", "ret"]
        masked, unmasked, _, _ = sdkdebug.compare(ours, theirs)
        self.assertEqual(masked, 1.0)
        self.assertAlmostEqual(unmasked, 0.8)


    def test_every_ebp_based_operand_masks_its_displacement(self):
        n = sdkdebug.normalize
        self.assertEqual(n("fstp dword ptr [ebp + eax*4 - 0x98]"), "fstp dword ptr [ebp + eax*4 X]")
        self.assertEqual(n("mov dword ptr [ebp + ecx*8 + 0x10], 0"), "mov dword ptr [ebp + ecx*8 X], 0")
        self.assertEqual(n("lea eax, [ebp + ecx - 0x10]"), "lea eax, [ebp + ecx X]")
        # no displacement, or ebp as the index: not a frame slot's displacement
        self.assertEqual(n("mov eax, dword ptr [ebp + eax*4]"), "mov eax, dword ptr [ebp + eax*4]")
        self.assertEqual(n("add byte ptr [edx + ebp*8 + 0x46], bh"), "add byte ptr [edx + ebp*8 + 0x46], bh")
        self.assertEqual(n("fstp dword ptr [ebp + eax*4 - 0x98]", mask_frame=False),
                         "fstp dword ptr [ebp + eax*4 - 0x98]")

    def test_indexed_slots_compare_alike_masked(self):
        ours = ["push ebp", "fstp dword ptr [ebp + eax*4 - 0x98]", "mov dword ptr [ebp + ecx*4 - 0x88], 0"]
        theirs = ["push ebp", "fstp dword ptr [ebp + eax*4 - 0x88]", "mov dword ptr [ebp + ecx*4 - 0x98], 0"]
        masked, unmasked, _, _ = sdkdebug.compare(ours, theirs)
        self.assertEqual(masked, 1.0)
        self.assertAlmostEqual(unmasked, 1 / 3)


    def test_pct_reads_100_only_for_identical(self):
        self.assertEqual(sdkdebug.pct(1.0), 100.0)
        self.assertEqual(sdkdebug.pct(0.9997), 99.9)
        self.assertEqual(sdkdebug.pct(0.95), 95.0)
        self.assertEqual(sdkdebug.pct(0.957, 0), 95)


class FrameAlignment(unittest.TestCase):
    """The frame order is read along the statements' alignment, not by a matcher of its own."""

    ours = ["push ebp", "mov ebp, esp", "mov dword ptr [ebp - 4], ecx", "mov eax, dword ptr [ebp - 8]",
            "mov dword ptr [ebp - 8], eax", "mov eax, dword ptr [ebp - 4]", "ret"]
    # the two locals in each other's slots: line for line the same statements
    theirs = [t.replace("ebp - 8", "ebp - Y").replace("ebp - 4", "ebp - 8").replace("ebp - Y", "ebp - 4")
              for t in ours]

    def test_slots_compare_line_for_line(self):
        import difflib
        plain = difflib.SequenceMatcher(None, self.ours, self.theirs, autojunk=False)
        # a matcher of the unmasked listings pairs the slot lines across statements: a swap that isn't there
        self.assertTrue(any(tag in ("insert", "delete") for tag, *_ in plain.get_opcodes()))
        masked, unmasked, _, framed = sdkdebug.compare(self.ours, self.theirs)
        self.assertEqual(masked, 1.0)
        for tag, a0, a1, b0, b1 in framed.get_opcodes():
            self.assertIn(tag, ("equal", "replace"))
            self.assertEqual((a0, a1), (b0, b1))
        self.assertEqual([(t, a0, a1) for t, a0, a1, _, _ in framed.get_opcodes()],
                         [("equal", 0, 2), ("replace", 2, 6), ("equal", 6, 7)])
        self.assertAlmostEqual(unmasked, 2 * 3 / 14)

    def test_statement_differences_stay_outside_the_frame_runs(self):
        theirs = self.theirs[:3] + ["inc eax"] + self.theirs[3:]
        masked, unmasked, sm, framed = sdkdebug.compare(self.ours, theirs)
        self.assertLess(masked, 1.0)
        self.assertIn(("insert", 3, 3, 3, 4), framed.get_opcodes())
        self.assertAlmostEqual(unmasked, 2 * 3 / 15)


class Constants(unittest.TestCase):
    def test_debug_image_operands_show_their_values(self):
        consts = {0x402010: struct.pack("<f", 90.0), 0x402020: struct.pack("<d", 0.1),
                  0x402030: (1 << 63).to_bytes(8, "little") + (16383 + 1).to_bytes(2, "little"),
                  0x402040: struct.pack("<i", 12)}
        code = b"".join(op + struct.pack("<I", va) for op, va in (
            (b"\xD8\x35", 0x402010), (b"\xDC\x0D", 0x402020), (b"\xDB\x2D", 0x402030), (b"\xDB\x05", 0x402040),
            (b"\xD8\x35", 0x402050)))

        def data(va, n):
            raw = consts.get(va)
            return raw[:n] if raw is not None else None
        got = sdkdebug.listing(code, TEXT, data=data)
        self.assertEqual(got, ["fdiv dword ptr [=90.0]", "fmul qword ptr [=0.1]", "fld xword ptr [=2.0]",
                               "fild dword ptr [=12]", "fdiv dword ptr [<addr>]"])
        self.assertEqual([sdkdebug.normalize(t) for t in got[:2]],
                         ["fdiv dword ptr [<addr>]", "fmul qword ptr [<addr>]"])

    def test_object_operands_show_their_values(self):
        code = b"\xD8\x35\0\0\0\0\xD8\x0D\x04\0\0\0"     # fdiv [__real@...], fmul [_g + 4]
        relocs = {2: "__real@4@4005b400000000000000", 8: "_g"}
        data = {2: struct.pack("<f", 90.0), 8: None}
        got = sdkdebug.listing(code, 0, relocs=relocs, data=lambda off, n: data[off])
        self.assertEqual(got, ["fdiv dword ptr [=90.0]", "fmul dword ptr [<addr>]"])

    def test_const_text(self):
        c = sdkdebug.const_text
        self.assertEqual(c("fld", 4, struct.pack("<f", 0.3)), "0.3")
        self.assertEqual(c("fcomp", 4, struct.pack("<f", 0.0)), "0.0")
        self.assertEqual(c("fld", 4, struct.pack("<f", 1e-6)), "1e-06")
        self.assertEqual(c("fld", 4, struct.pack("<I", 0x7F7FFFFF)), "3.4028235e+38")     # FLT_MAX
        self.assertEqual(c("fldcw", 2, b"\x7f\x02"), "639")
        self.assertEqual(c("mov", 4, struct.pack("<i", -1)), "-1")
        self.assertEqual(c("mov", 4, struct.pack("<I", 0x80000000)), "0x80000000")
        self.assertIsNone(c("fld", 4, None))

    def test_constants_differ_on_alike_lines(self):
        ours = ["push ebp", "fdiv dword ptr [=90.0]", "fmul dword ptr [=2.0]", "ret"]
        theirs = ["push ebp", "fdiv dword ptr [=180.0]", "fmul dword ptr [=2.0]", "ret"]
        masked, _, sm, _ = sdkdebug.compare(ours, theirs)
        self.assertEqual(masked, 1.0)
        self.assertEqual(sdkdebug.constants_differ(ours, theirs, sm), [(ours[1], theirs[1])])


class JumpTables(unittest.TestCase):
    """/Od puts a switch's jump tables after the function's last instruction."""

    def build(self):
        a = TEXT + 4
        # a: push ebp; mov ebp, esp; jmp [eax*4 + table]; L0: pop ebp; ret; L1: pop ebp; ret; table: L0, L1; then b
        # right after the table (no int3 or ret before it, nothing calls it); then c after int3 fill
        code = bytearray(b"\xCC" * 4)
        code += b"\x55\x8B\xEC\xFF\x24\x85" + b"\0\0\0\0" + b"\x5D\xC3" + b"\x5D\xC3"
        table = TEXT + len(code)
        struct.pack_into("<I", code, a - TEXT + 6, table)
        code += struct.pack("<II", a + 10, a + 12)
        b = TEXT + len(code)
        code += b"\x55\x8B\xEC\x90\x5D\xC3" + b"\xCC" * 2
        c = TEXT + len(code)
        code += b"\x55\x8B\xEC\x5D\xC3"
        img = image(code, b"\0" * 16)
        img.relocs = {a + 6, table, table + 4}
        return sdkdebug.Debug(img), a, table, b, c, bytes(code[a - TEXT:b - TEXT])

    def test_end_of_stops_at_the_table_and_the_next_function_starts_after_it(self):
        dbg, a, table, b, c, _ = self.build()
        self.assertEqual(dbg.tables, {a: table})
        self.assertEqual(dbg.end_of(a), table)
        self.assertEqual(dbg.next_start(a), b)
        self.assertIn(b, dbg.start_set)
        self.assertEqual(dbg.end_of(b), c)
        self.assertEqual(dbg.table_run(a, table), table + 8)
        listing = sdkdebug.DebugListings(dbg, names={}).raw(a)
        self.assertEqual(listing, ["push ebp", "mov ebp, esp", "jmp <label>", "pop ebp", "ret ", "pop ebp", "ret "])

    def test_same_body_compares_the_tables(self):
        dbg, a, table, b, c, body = self.build()
        # our object: the jmp's field and the table's entries relocated (they read section offsets)
        ours = bytearray(body)
        struct.pack_into("<I", ours, 6, 0)
        struct.pack_into("<II", ours, 14, 10, 12)
        masked = {6, 14, 18}
        self.assertTrue(sdkdebug.same_body(dbg, a, bytes(ours), masked, table=14))
        self.assertFalse(sdkdebug.same_body(dbg, a, bytes(ours[:18]), {6, 14}, table=14))   # one entry short
        self.assertFalse(sdkdebug.same_body(dbg, a, bytes(ours), masked))                     # the table as code

    def test_object_table_is_the_lowest_forward_reference(self):
        from types import SimpleNamespace as NS
        import coff
        data = bytearray(32)
        struct.pack_into("<I", data, 6, 0)          # jmp [eax*4 + $L20]: the label carries the offset
        struct.pack_into("<I", data, 12, 26)        # mov dl, [eax + section + 26]: the inline addend does
        dir32 = coff.IMAGE_REL_I386_DIR32
        sec = NS(data=bytes(data), relocs=[(6, 1, dir32), (12, 2, dir32), (20, 3, dir32), (16, 4, dir32)])
        symbols = {1: NS(section=1, value=20), 2: NS(section=1, value=0), 3: NS(section=1, value=2),
                   4: NS(section=2, value=24)}
        o = NS(section=lambda n: sec, symbols=symbols)
        fn = NS(section=1)
        self.assertEqual(sdkdebug.coff_table(o, fn, 0, 32), 20)
        self.assertIsNone(sdkdebug.coff_table(o, fn, 0, 20))


class Search(unittest.TestCase):
    def test_lcs_length_agrees_with_the_table(self):
        import random
        rnd = random.Random(7)

        def table(a, b):
            row = [0] * (len(b) + 1)
            for x in a:
                prev, row = row, [0]
                for j, y in enumerate(b):
                    row.append(prev[j] + 1 if x == y else max(prev[j + 1], row[j]))
            return row[-1]
        for _ in range(200):
            a = [rnd.choice("abcd") for _ in range(rnd.randint(0, 70))]
            b = [rnd.choice("abcde") for _ in range(rnd.randint(0, 70))]
            self.assertEqual(sdkdebug.lcs_length(sdkdebug.lcs_masks(a), len(a), b), table(a, b))

    def listings(self):
        body = ["push ebp", "mov ebp, esp", "sub esp, 8", "mov dword ptr [ebp - 4], ecx",
                "mov eax, dword ptr [ebp - 4]", "mov ecx, dword ptr [eax + 0x10]", "call ?Get@@QAEHXZ",
                "mov dword ptr [ebp - 8], eax", "mov eax, dword ptr [ebp - 8]", "mov esp, ebp", "pop ebp", "ret"]
        swapped = [t.replace("ebp - 8", "ebp - 0xc").replace("ebp - 4", "ebp - 8").replace("ebp - 0xc", "ebp - 4")
                   for t in body]
        texts = {
            0x1000: swapped,                                  # the copy, its two locals in the other slots
            0x2000: body[:6] + ["add eax, 1"] + body[6:],     # one statement more
            0x3000: body,                                     # the same body, also in the same slots: a twin
            0x4000: ["push ebp", "mov ebp, esp", "xor eax, eax", "pop ebp", "ret"],
            0x5000: [],
        }
        return body, texts, lambda a, mask=True: tuple(sdkdebug.normalize(t, mask) for t in texts[a])

    def test_rank_puts_the_frame_shifted_copy_level_with_the_exact_one(self):
        body, texts, key = self.listings()
        got = sdkdebug.rank(body, sorted(texts), key, top=3)
        self.assertEqual([a for _, _, a in got], [0x3000, 0x1000, 0x2000])
        self.assertEqual(got[0][:2], (1.0, 1.0))
        self.assertEqual(got[1][0], 1.0)
        self.assertLess(got[1][1], 1.0)
        self.assertLess(got[2][0], 1.0)

    def test_rank_without_the_mask_ranks_by_frame_slots(self):
        body, texts, key = self.listings()
        got = sdkdebug.rank(body, sorted(texts), key, top=2, mask_frame=False)
        self.assertEqual([a for _, _, a in got], [0x3000, 0x2000])

    def test_rank_equals_a_full_scan(self):
        import difflib
        import random
        rnd = random.Random(3)
        words = ["mov eax, dword ptr [ebp - %d]" % k for k in (4, 8, 12)] + ["push eax", "call x", "ret", "inc eax"]
        texts = {k: [rnd.choice(words) for _ in range(rnd.randint(1, 30))] for k in range(60)}
        ours = [rnd.choice(words) for _ in range(20)]
        key = lambda a, mask=True: tuple(sdkdebug.normalize(t, mask) for t in texts[a])
        mine = [sdkdebug.normalize(t) for t in ours]
        full = sorted(((difflib.SequenceMatcher(None, mine, list(key(a)), autojunk=False).ratio(), a)
                       for a in texts), key=lambda x: -x[0])
        got = sdkdebug.rank(ours, sorted(texts), key, top=4)
        self.assertEqual([round(r, 9) for r, _, _ in got], [round(r, 9) for r, _ in full[:4]])

    def test_bag_bound_is_quick_ratio(self):
        import difflib
        _, texts, key = self.listings()
        mine = list(key(0x2000))
        for a, b in zip(sorted(texts), sdkdebug.bag_bound(mine, sorted(texts), key)):
            sm = difflib.SequenceMatcher(None, mine, list(key(a)), autojunk=False)
            self.assertAlmostEqual(b, sm.quick_ratio() if mine or key(a) else 0.0)


class CandidatePairs(unittest.TestCase):
    def hit(self, debug, masked, paired=None):
        return {"debug": debug, "masked": masked, "unmasked": masked, "in_unit_file": True, "paired_with": paired}

    def test_clear_unclaimed_hits_and_unique_od_copies(self):
        results = [
            {"retail": "0x1", "name": "A", "hits": [self.hit("0x100", 1.0), self.hit("0x200", 0.5)]},    # clear
            {"retail": "0x2", "name": "B", "hits": [self.hit("0x300", 0.97), self.hit("0x400", 0.95)]},  # tied
            {"retail": "0x3", "name": "C", "hits": [self.hit("0x500", 0.99, "0x9 X")]},                  # held
            {"retail": "0x4", "name": "D", "hits": [self.hit("0x600", 0.90)]},                           # weak
            {"retail": "0x5", "name": "E", "hits": [self.hit("0x700", 1.0)]},                            # two
            {"retail": "0x6", "name": "F", "hits": [self.hit("0x700", 0.99)]},                           # claim it
        ]
        rows = [{"addr": "0x7", "name": "G", "symbol": "?G"}, {"addr": "0x8", "name": "H", "symbol": "?H"},
                {"addr": "0x9", "name": "I", "symbol": "?I"}]
        od_found = {"?G": ["0x800"], "?H": ["0x900", "0xA00"], "?I": ["0x100"]}
        lines = sdkdebug.candidate_pairs(results, rows, od_found, {})
        got = [tuple(line.split()[:2]) for line in lines]
        self.assertEqual(got, [("0x1", "0x100"), ("0x7", "0x800")])


class ConfirmedPairs(unittest.TestCase):
    def test_reads_retail_and_debug_with_comments(self):
        import tempfile
        with tempfile.TemporaryDirectory() as d:
            path = Path(d) / "pairs.txt"
            path.write_text("\n".join(["# retail debug", "0x7C3290 0x51128D  # SetFrameBuffer, cmp 72%", "",
                                       "0x7d0f90 0x5212a1"]))
            self.assertEqual(sdkdebug.confirmed_pairs(path), [(0x7C3290, 0x51128D), (0x7D0F90, 0x5212A1)])
            self.assertEqual(sdkdebug.confirmed_pairs(Path(d) / "missing.txt"), [])


if __name__ == "__main__":
    unittest.main()
