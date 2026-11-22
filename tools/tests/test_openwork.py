"""Tests of openwork.py's ORIGINAL-ASM, IMPORT-THUNK and LIBRARY-DATA ledger: the CRT's assembly routines and the import libraries'
stubs that config/exceptions.md lists apart are neither claimed work nor unwritten bytes.

    python -m unittest discover -s tools/tests
"""

import contextlib
import io
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import openwork  # noqa: E402

LEDGER = """
| address | function | rule | deviation | status |
|---|---|---|---|---|
| 0x6F9B60 | `_wrap_Breakpoint` (`src/game/PyEmbed.cpp`) | R18 | the original's inline `__asm int 3` | ORIGINAL-ASM |
| 0x85B536 | `_realloc` | R17 | unused local; 4 bytes of frame | MATCH-PENDING |
| 0x85FFD0 | `memcpy` | - | VC98/CRT/SRC/PLATFORM/MEMCPY.ASM: LIBCMT.LIB `memcpy.obj`; 821 bytes | ORIGINAL-ASM |
| 0x860440 | `strlen` | - | VC98/CRT/SRC/PLATFORM/STRLEN.ASM; 123 bytes | ORIGINAL-ASM |
| 0x860305 | `_RtlUnwind@16` | - | `jmp [0x888170]`, KERNEL32.dll!RtlUnwind | IMPORT-THUNK |
"""

# the same, with library data filling the gap before strlen
LIBRARY_LEDGER = LEDGER + """| 0x860320 | `c_dfDIJoystick` object table | - | dinput.lib's DIOBJECTDATAFORMAT array; 288 bytes | LIBRARY-DATA |
"""


def row(addr, name, size, status="MATCH"):
    return {"addr": "0x%X" % addr, "name": name, "size": size, "status": status, "score": 100.0, "rules": []}


class Ledger(unittest.TestCase):
    def test_reads_address_name_and_bytes(self):
        asm = openwork.original_asm(LEDGER)
        self.assertEqual(asm, {0x6F9B60: ("_wrap_Breakpoint (src/game/PyEmbed.cpp)", 0),
                               0x85FFD0: ("memcpy", 821), 0x860440: ("strlen", 123)})

    def test_annotated_rows_are_left_to_the_report(self):
        self.assertNotIn(0x6F9B60, openwork.original_asm(LEDGER, annotated={0x6F9B60}))

    def test_other_statuses_are_not_listed(self):
        self.assertNotIn(0x85B536, openwork.original_asm(LEDGER))
        self.assertNotIn(0x860305, openwork.original_asm(LEDGER))

    def test_import_thunks_are_six_bytes(self):
        self.assertEqual(openwork.import_thunks(LEDGER), {0x860305: ("_RtlUnwind@16", 6)})

    def test_library_data_reads_its_bytes(self):
        self.assertEqual(openwork.library_data(LIBRARY_LEDGER), {0x860320: ("c_dfDIJoystick object table", 288)})
        self.assertEqual(openwork.library_data(LEDGER), {})


def patch_ledger(test, ledger):
    """Point openwork's three ledger readers at `ledger` for the length of a test."""
    saved = openwork.original_asm, openwork.import_thunks, openwork.library_data
    openwork.original_asm = lambda text=None, annotated=(): {
        a: v for a, v in saved[0](ledger).items() if a not in annotated}
    openwork.import_thunks = lambda text=None, annotated=(): {
        a: v for a, v in saved[1](ledger).items() if a not in annotated}
    openwork.library_data = lambda text=None, annotated=(): {
        a: v for a, v in saved[2](ledger).items() if a not in annotated}

    def restore():
        openwork.original_asm, openwork.import_thunks, openwork.library_data = saved
    test.addCleanup(restore)


class Counts(unittest.TestCase):
    def setUp(self):
        patch_ledger(self, LEDGER)

    def run_cmd(self, fn, *args):
        out = io.StringIO()
        with contextlib.redirect_stdout(out):
            fn(*args)
        return out.getvalue()

    rows = [row(0x6F9B60, "_wrap_Breakpoint", 16), row(0x85FF00, "free", 0xC0)]
    claims = [("_memcpy", 0x85FFD0, ""), ("_strlen", 0x860440, ""), ("_malloc", 0x861000, ""),
              ("_RtlUnwind@16", 0x860305, "")]

    def test_phases_counts_them_apart(self):
        text = self.run_cmd(openwork.cmd_phases, self.rows, self.claims,
                            {"text_bytes": 10000, "source_bytes": 5000}, {})
        # 0x6F9B70-0x85FF00 after _wrap_Breakpoint, and 0x860320-0x860440 before strlen (memcpy and the thunk
        # bound the rest): 1467280 + 288
        self.assertIn("1467568 bytes of .text unwritten", text)
        self.assertIn("in 2 gaps", text)
        self.assertIn("1 functions claimed without source", text)         # _malloc only
        self.assertIn("ORIGINAL-ASM listed apart: 2 functions, 944 bytes", text)
        self.assertIn("IMPORT-THUNK listed apart (from the import libraries): 1 stubs, 6 bytes", text)

    def test_range_shows_them_as_asm(self):
        text = self.run_cmd(openwork.cmd_range, 0x85FF00, 0x860320, self.rows, self.claims)
        self.assertIn("asm", text.splitlines()[2])
        self.assertIn("0x85FFD0 memcpy (821 bytes)", text)
        self.assertNotIn("gap", text)    # free, memcpy and the thunk cover the range up to 0x86030B
        self.assertIn("0 claimed without source, 1 ORIGINAL-ASM, 1 IMPORT-THUNK", text)

    def test_range_shows_a_thunk(self):
        text = self.run_cmd(openwork.cmd_range, 0x85FF00, 0x860320, self.rows, self.claims)
        line = next(l for l in text.splitlines() if "0x860305" in l)
        self.assertTrue(line.strip().startswith("thunk"), line)
        self.assertIn("_RtlUnwind@16 (6 bytes)", line)

    def test_classes_do_not_count_a_thunk_claimed(self):
        text = self.run_cmd(openwork.cmd_classes, self.rows, [("?Close@TGSocket@@QAEXXZ", 0x860305, "")])
        self.assertNotIn("TGSocket", text)


class ThunkCheck(unittest.TestCase):
    """`openwork.py thunks`: each row's bytes are `FF 25 <slot>` and the slot is the import the row names."""

    class Img:
        def __init__(self, code, relocs, imports):
            self.code, self.relocs, self._imports = code, relocs, imports

        def read(self, va, n):
            return self.code.get(va, b"")[:n] or None

        def imports(self):
            return self._imports

    def img(self, code=b"\xFF\x25\x70\x81\x88\x00", relocs=(0x86F762,)):
        return self.Img({0x86F760: code}, set(relocs), {0x888170: ("KERNEL32.dll", "RtlUnwind"),
                                                        0x888364: ("WSOCK32.dll", "#10")})

    def check(self, name, deviation="", **kw):
        import decomp
        return decomp.check_import_thunk(self.img(**kw), 0x86F760, name, deviation)

    def test_a_thunk_by_name(self):
        self.assertIsNone(self.check("_RtlUnwind@16"))

    def test_the_wrong_import(self):
        self.assertIn("not _HeapFree@12", self.check("_HeapFree@12"))

    def test_not_a_jump(self):
        self.assertIn("not a `jmp [slot]`", self.check("_RtlUnwind@16", code=b"\x55\x8B\xEC\x83\xEC\x08"))

    def test_no_relocation(self):
        self.assertIn("no base relocation", self.check("_RtlUnwind@16", relocs=()))

    def test_an_import_by_ordinal(self):
        code = b"\xFF\x25\x64\x83\x88\x00"
        self.assertIsNone(self.check("_inet_addr@4", "WSOCK32.dll ordinal 10 (inet_addr)", code=code))
        self.assertIn("ordinal 10", self.check("_inet_addr@4", "WSOCK32.dll ordinal 11", code=code))

    def test_the_command_counts_problems(self):
        out = io.StringIO()
        with contextlib.redirect_stdout(out):
            n = openwork.cmd_thunks(LEDGER.replace("0x860305", "0x86F760"), self.img(), [])
        self.assertEqual(n, 0)
        self.assertIn("1 IMPORT-THUNK rows, 0 problems", out.getvalue())


def patched_row(addr, name, size):
    r = row(addr, name, size, status="PATCHED")
    r.update(score=12.5, patched="test; nop; nop; return 0")
    return r


class Patched(unittest.TestCase):
    """A function under a `// PATCHED:` note: binary-patched in retail, written as the original, neither matched
    nor open."""

    rows = [row(0x460700, "Before", 0x100), patched_row(0x460880, "FindDiscWithContents", 0x106),
            row(0x460990, "After", 0x9B, status="DIFF")]

    def run_cmd(self, fn, *args):
        out = io.StringIO()
        with contextlib.redirect_stdout(out):
            fn(*args)
        return out.getvalue()

    def test_status(self):
        self.assertEqual(openwork.status_of(self.rows[1]), "patched")

    def test_phases_counts_it_apart(self):
        patch_ledger(self, "")
        text = self.run_cmd(openwork.cmd_phases, self.rows, [], {"text_bytes": 10000, "source_bytes": 5000}, {})
        self.assertIn(" 128 bytes of .text unwritten", text)     # 0x460800-0x460880, between Before and the PATCHED one
        self.assertIn("written but not exact: 1 functions, 155 bytes", text)   # After (0x9B), not the PATCHED one
        self.assertIn("PATCHED listed apart (binary-patched in retail, written as the original): 1 functions, "
                      "262 bytes", text)
        self.assertIn("0 drafts, 1 open without a note", text)   # After only

    def test_range_shows_it_as_patched(self):
        patch_ledger(self, "")
        text = self.run_cmd(openwork.cmd_range, 0x460700, 0x460A30, self.rows, [])
        self.assertIn("3 annotated (1 PATCHED)", text)
        line = next(l for l in text.splitlines() if "0x460880 FindDiscWithContents" in l)
        self.assertTrue(line.strip().startswith("patched"), line)
        self.assertIn("test; nop; nop; return 0", line)

    def test_queues_leave_it_out(self):
        text = self.run_cmd(openwork.cmd_drafts, 0.0, self.rows)
        self.assertNotIn("FindDiscWithContents", text)
        text = self.run_cmd(openwork.cmd_parked, 0.0, self.rows)
        self.assertNotIn("FindDiscWithContents", text)


class PatchedInTheBuild(unittest.TestCase):
    """decomp.py reads the note, and its summary counts the function apart."""

    def test_scan_reads_the_note(self):
        import decomp
        text = ("// FUNCTION: %s 0x460880\n// PATCHED: test; nop; nop; return 0\nint f( int *p ) {\n\treturn 1;\n}\n"
                % decomp.PROJECT["module"])
        (e,) = decomp.scan_text("game/X", Path("X.cpp"), text)
        self.assertEqual(e.patched, "test; nop; nop; return 0")
        self.assertEqual(e.name, "f")

    def test_summary_counts_it_apart(self):
        import decomp

        class Img:
            text = (0, 0, 1000)
        rows = [row(0x401000, "a", 16), patched_row(0x460880, "FindDiscWithContents", 0x106),
                row(0x402000, "b", 32, status="DIFF")]
        s = decomp.summarize(rows, Img(), ledger="")
        self.assertEqual((s["functions"], s["matched_rules"], s["diff"], s["patched"], s["patched_bytes"]),
                         (2, 1, 1, 1, 0x106))
        self.assertEqual(s["source_bytes"], 16)

    def test_summary_counts_the_ledger_apart(self):
        import decomp

        class Img:
            text = (0, 0, 1000)
        # memcpy, strlen and the thunk; _wrap_Breakpoint is annotated, so the report's row counts it
        s = decomp.summarize([row(0x6F9B60, "_wrap_Breakpoint", 16)], Img(), ledger=LEDGER)
        self.assertEqual((s["original_asm"], s["original_asm_bytes"], s["import_thunks"], s["import_thunk_bytes"]),
                         (2, 944, 1, 6))
        out = io.StringIO()
        with contextlib.redirect_stdout(out):
            decomp.print_summary(s)
        self.assertIn("import thunks (IMPORT-THUNK, from the import libraries, counted apart): 1 stubs, 6 bytes",
                      out.getvalue())
        self.assertIn("(ORIGINAL-ASM, counted apart): 2 routines, 944 bytes", out.getvalue())

    def test_cards_do_not_count_it_open(self):
        import cards
        self.assertFalse(cards.is_open(patched_row(0x460880, "FindDiscWithContents", 0x106)))


class LibraryData(unittest.TestCase):
    """LIBRARY-DATA (a library's data linked into .text) is listed apart like an import thunk."""
    def setUp(self):
        patch_ledger(self, LIBRARY_LEDGER)

    run_cmd = Counts.run_cmd
    rows, claims = Counts.rows, Counts.claims

    def test_phases_counts_it_apart(self):
        text = self.run_cmd(openwork.cmd_phases, self.rows, self.claims,
                            {"text_bytes": 10000, "source_bytes": 5000}, {})
        self.assertIn("1467280 bytes of .text unwritten", text)      # the gap before strlen is the data's
        self.assertIn("in 1 gaps", text)
        self.assertIn("LIBRARY-DATA listed apart (library data in .text): 1 blocks, 288 bytes", text)

    def test_range_shows_it_as_data(self):
        text = self.run_cmd(openwork.cmd_range, 0x85FF00, 0x860500, self.rows, self.claims)
        line = next(l for l in text.splitlines() if "0x860320" in l)
        self.assertTrue(line.strip().startswith("data"), line)
        self.assertIn("(288 bytes)", line)
        self.assertIn("1 IMPORT-THUNK, 1 LIBRARY-DATA", text)
        self.assertNotIn("gap   0x860320", text)

    def test_summary_counts_it_apart(self):
        import decomp

        class Img:
            text = (0, 0, 1000)
        s = decomp.summarize([row(0x6F9B60, "_wrap_Breakpoint", 16)], Img(), ledger=LIBRARY_LEDGER)
        self.assertEqual((s["library_data"], s["library_data_bytes"]), (1, 288))
        out = io.StringIO()
        with contextlib.redirect_stdout(out):
            decomp.print_summary(s)
        self.assertIn("library data in .text (LIBRARY-DATA, from the libraries, counted apart): 1 blocks, 288 bytes",
                      out.getvalue())


class ThunkCards(unittest.TestCase):
    """cards.py (and so merge_lane.py check) never asks for a card entry on an import thunk."""

    def test_touched_open_skips_a_thunk(self):
        import json
        import tempfile
        import cards
        with tempfile.TemporaryDirectory() as root:
            (Path(root) / "config").mkdir()
            (Path(root) / "build").mkdir()
            (Path(root) / "config" / "baseline.json").write_text("{}", encoding="utf-8")
            (Path(root) / "config" / "exceptions.md").write_text(LEDGER, encoding="utf-8")
            rows = [row(0x860305, "_RtlUnwind@16", 6, status="DIFF"), row(0x402000, "b", 32, status="DIFF")]
            (Path(root) / "build" / "report.json").write_text(json.dumps({"functions": rows}), encoding="utf-8")
            self.assertEqual([k for k, _ in cards.touched_open(root)], ["0x402000"])


if __name__ == "__main__":
    unittest.main()
