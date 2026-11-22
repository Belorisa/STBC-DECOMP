"""Tests of the batch path of `decomp.py diff`: the target list, the per-function header, the one-line summary, and
the caches the load of a session leans on (the annotation scan, the object-cache sweep).

    python -m unittest discover -s tools/tests
"""

import contextlib
import io
import os
import sys
import tempfile
import time
import types
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import decomp  # noqa: E402


def args(function=(), list_file=None, summary=False, size=None):
    return types.SimpleNamespace(function=list(function), list=list_file, summary=summary, size=size)


class FakeSession:
    """Stands in for DiffSession: two known functions, everything else is an error."""
    compile_failed = False
    asked = []

    def __init__(self, targets):
        FakeSession.asked.append(list(targets))

    def text(self, target, size=None):
        if target == "missing":
            raise decomp.DiffError("no annotated function missing")
        return "diff of %s" % target

    def summary_line(self, target, size=None):
        if target == "missing":
            raise decomp.DiffError("no annotated function missing")
        return "%s  Name  DIFF  97.2  | pop esi  vs  -  | wall: register" % target


class Targets(unittest.TestCase):
    def test_list_file_skips_comments_and_blanks(self):
        with tempfile.TemporaryDirectory() as d:
            f = Path(d) / "l.txt"
            f.write_text("# header\n0x514F00\n\n  Foo::Bar  # trailing\n0x493DD0\n", encoding="utf-8")
            self.assertEqual(decomp.diff_targets(args(["0x1"], str(f))), ["0x1", "0x514F00", "Foo::Bar", "0x493DD0"])

    def test_repeats_are_dropped_in_order(self):
        self.assertEqual(decomp.diff_targets(args(["a", "b", "a"])), ["a", "b"])


class Command(unittest.TestCase):
    def run_diff(self, a):
        real = decomp.DiffSession
        decomp.DiffSession = FakeSession
        FakeSession.asked = []
        out = io.StringIO()
        try:
            with contextlib.redirect_stdout(out):
                code = decomp.cmd_diff(a)
        finally:
            decomp.DiffSession = real
        return code, out.getvalue()

    def test_one_function_prints_the_bare_diff(self):
        code, out = self.run_diff(args(["0x1"]))
        self.assertEqual((code, out), (0, "diff of 0x1\n"))

    def test_several_functions_share_one_session_and_get_headers(self):
        code, out = self.run_diff(args(["0x1", "0x2"]))
        self.assertEqual(code, 0)
        self.assertEqual(out, "=== 0x1 ===\ndiff of 0x1\n\n=== 0x2 ===\ndiff of 0x2\n\n")
        self.assertEqual(FakeSession.asked, [["0x1", "0x2"]])

    def test_summary_is_one_line_per_function_and_an_unknown_one_does_not_stop_it(self):
        code, out = self.run_diff(args(["0x1", "missing", "0x2"], summary=True))
        lines = out.splitlines()
        self.assertEqual(code, 1)
        self.assertEqual(len(lines), 3)
        self.assertTrue(lines[0].startswith("0x1  Name  DIFF  97.2"))
        self.assertEqual(lines[1], "missing  ERROR  no annotated function missing")

    def test_no_function_is_an_error(self):
        with self.assertRaises(SystemExit):
            self.run_diff(args())


class Caches(unittest.TestCase):
    def test_scan_cache_round_trip(self):
        src = "// FUNCTION: STBC 0x00401000\nint Foo( void ) {\n\treturn 1;\n}\n"
        with tempfile.TemporaryDirectory() as d:
            path = Path(d) / "a.cpp"
            path.write_text(src, encoding="latin-1")
            saved = (decomp._scan_disk, decomp._scan_dirty, decomp.SCAN_CACHE, decomp.BUILD)
            try:
                decomp.BUILD = Path(d)
                decomp.SCAN_CACHE = Path(d) / "scan.pkl"
                decomp._scan_disk, decomp._scan_dirty = None, False
                fresh = decomp.scan_cached("a", path, src)
                self.assertTrue(decomp._scan_dirty)
                decomp.save_scan_cache()
                decomp._scan_disk = None
                cached = decomp.scan_cached("a", path, src)
                self.assertFalse(decomp._scan_dirty)
                f = decomp.ENTRY_FIELDS
                self.assertEqual([[getattr(e, k) for k in f] for e in cached], [[getattr(e, k) for k in f] for e in fresh])
                self.assertEqual(cached[0].text, fresh[0].text)
                # a changed file is scanned again
                path.write_text(src + "// FUNCTION: STBC 0x00401010\nint Bar( void ) {\n}\n", encoding="latin-1")
                again = decomp.scan_cached("a", path, decomp.source_text(path))
                self.assertEqual([e.addr for e in again], [0x401000, 0x401010])
            finally:
                decomp._scan_disk, decomp._scan_dirty, decomp.SCAN_CACHE, decomp.BUILD = saved

    def test_obj_cache_sweep_runs_once_a_day(self):
        with tempfile.TemporaryDirectory() as d:
            saved = decomp.OBJ_CACHE
            try:
                decomp.OBJ_CACHE = Path(d)
                old = Path(d) / "old.obj"
                old.write_bytes(b"x")
                t = time.time() - 30 * 86400
                os.utime(old, (t, t))
                decomp.prune_obj_cache()
                self.assertFalse(old.exists())
                old.write_bytes(b"x")
                os.utime(old, (t, t))
                decomp.prune_obj_cache()   # stamped just now: no sweep
                self.assertTrue(old.exists())
            finally:
                decomp.OBJ_CACHE = saved


if __name__ == "__main__":
    unittest.main()
