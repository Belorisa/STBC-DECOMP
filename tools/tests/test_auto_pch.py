"""Tests of the /YX switch ("auto_pch" in config/project.json): the arguments it adds, the fingerprint, the .pch's
lifetime, the trace tools' own .pch paths, trial's route, and one real compile of a small unit.

    python -m unittest discover -s tools/tests
"""

import copy
import shutil
import sys
import tempfile
import unittest
from contextlib import contextmanager
from pathlib import Path
from types import SimpleNamespace
from unittest import mock

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import decomp  # noqa: E402
import trial  # noqa: E402

UNIT = "game/TGKeyboardEvent"   # a small game unit: two headers, a few functions


@contextmanager
def switch(on):
    """config/project.json as loaded, with src/game's auto_pch set to `on` (src/netimmerse untouched)."""
    project = copy.deepcopy(decomp.PROJECT)
    for u in project["units"]:
        if u["path"] == "src/game":
            u["auto_pch"] = on
    with mock.patch.object(decomp, "PROJECT", project):
        yield


def unit_path():
    return decomp.source_files()[UNIT]


class Arguments(unittest.TestCase):
    def test_off_adds_nothing(self):
        with switch(False):
            args = decomp.cl_args(UNIT, unit_path())
        self.assertNotIn("/YX", args)
        self.assertFalse(any(a.startswith("/Fp") for a in args))

    def test_on_adds_yx_and_the_default_pch_before_the_source(self):
        with switch(True):
            args = decomp.cl_args(UNIT, unit_path())
        self.assertEqual(args[-2:], ["/Tp", str(unit_path())])
        i = args.index("/YX")
        self.assertEqual(args[i + 1], "/Fp" + str(decomp.PCH_DIR / (UNIT + ".pch")))
        self.assertLess(i, len(args) - 2)

    def test_a_callers_pch_and_none_for_preprocessing(self):
        with switch(True):
            own = decomp.cl_args(UNIT, unit_path(), pch=Path("C:/scratch/x.pch"))
            pre = decomp.cl_args(UNIT, unit_path(), pch=False)
        self.assertIn("/Fp" + str(Path("C:/scratch/x.pch")), own)
        self.assertNotIn("/YX", pre)
        self.assertFalse(any(a.startswith("/Fp") for a in pre))

    def test_the_switch_is_per_module(self):
        with switch(True):
            py = decomp.source_files()["python/Objects/listobject"]
            self.assertFalse(decomp.auto_pch(py))
            self.assertTrue(decomp.auto_pch(unit_path()))
            self.assertNotIn("/YX", decomp.cl_args("python/Objects/listobject", py))

    def test_a_unit_listed_in_auto_pch_units_alone_gets_yx(self):
        project = copy.deepcopy(decomp.PROJECT)
        for u in project["units"]:
            if u["path"] == "src/game":
                u["auto_pch"] = False
                u["auto_pch_units"] = [UNIT]
        with mock.patch.object(decomp, "PROJECT", project):
            self.assertIn("/YX", decomp.cl_args(UNIT, unit_path()))
            self.assertTrue(decomp.auto_pch(unit_path()))
            other = decomp.source_files()["game/TGMouseEvent"]
            self.assertNotIn("/YX", decomp.cl_args("game/TGMouseEvent", other))
            self.assertFalse(decomp.auto_pch(other))

    def test_the_fingerprint_covers_the_switch(self):
        with switch(False):
            off = decomp.unit_fingerprint(UNIT, unit_path())
        with switch(True):
            on = decomp.unit_fingerprint(UNIT, unit_path())
        self.assertNotEqual(off, on)

    def test_trial_strips_yx_from_the_preprocessed_flags(self):
        with switch(True):
            flags = trial.i_flags(UNIT, unit_path())
        self.assertNotIn("/YX", flags)


class PchFile(unittest.TestCase):
    def setUp(self):
        self.dir = Path(tempfile.mkdtemp())

    def tearDown(self):
        shutil.rmtree(self.dir, ignore_errors=True)

    def test_fresh_pch_removes_a_stale_file_and_makes_the_folder(self):
        pch = self.dir / "sub" / "u.pch"
        pch.parent.mkdir()
        pch.write_bytes(b"stale")
        self.assertEqual(decomp.fresh_pch(["/c", "/YX", "/Fp" + str(pch)]), pch)
        self.assertFalse(pch.exists())
        other = self.dir / "new" / "v.pch"
        decomp.fresh_pch(["/Fp" + str(other)])
        self.assertTrue(other.parent.is_dir())

    def test_no_pch_named(self):
        self.assertIsNone(decomp.fresh_pch(["/c", "/O2"]))
        decomp.drop_pch(None)   # nothing to do, no error

    def test_compile_unit_creates_and_drops_its_pch(self):
        seen = {}

        def fake_run(tool, args, cwd=None, compiler=None):
            pch = Path(next(a[3:] for a in args if a.startswith("/Fp")))
            seen["fresh"] = not pch.exists()
            pch.write_bytes(b"pch")
            obj = Path(next(a[3:] for a in args if a.startswith("/Fo")))
            obj.write_bytes(b"obj")
            seen["pch"] = pch
            return 0, ""

        stale = self.dir / (UNIT + ".pch")
        stale.parent.mkdir(parents=True)
        stale.write_bytes(b"stale")
        with switch(True), mock.patch.object(decomp.vc6, "run", fake_run):
            _, obj, _ = decomp.compile_unit(UNIT, unit_path(), self.dir, cache=False)
        self.assertEqual(seen["pch"], stale)        # beside the object, one per unit and output folder
        self.assertTrue(seen["fresh"])              # a stale .pch is removed before the compile
        self.assertFalse(stale.exists())            # and the new one after it
        self.assertTrue(obj.exists())


class TrialRoute(unittest.TestCase):
    def test_a_yx_unit_compiles_every_variant_from_the_source_copy(self):
        calls = []

        def fake_compile(unit, path, entry, text, d, preprocessed):
            calls.append((preprocessed, text))
            return None, "stop"

        lines = ["#include \"a.h\"", "int F() {", "return 1;", "}", "int G;"]
        entry = SimpleNamespace(path=str(unit_path()), addr=0)   # the function's definition is in the unit itself
        with switch(True), mock.patch.object(trial, "compile_text", fake_compile):
            trial.run_variant(None, UNIT, unit_path(), entry, "pre", set(), [], lines, 2, 4, "(base)", None,
                              Path("d"))
            trial.run_variant(None, UNIT, unit_path(), entry, "pre", set(), [], lines, 2, 4, "v", "int F() {}",
                              Path("d"))
        self.assertEqual([p for p, _ in calls], [False, False])
        self.assertEqual(calls[0][1], "\n".join(lines))                  # the base: the source unchanged
        self.assertIn("int F() {}", calls[1][1].split("\n"))

    def test_off_keeps_the_preprocessed_route(self):
        calls = []

        def fake_compile(unit, path, entry, text, d, preprocessed):
            calls.append(preprocessed)
            return None, "stop"

        with switch(False), mock.patch.object(trial, "compile_text", fake_compile):
            trial.run_variant(None, UNIT, unit_path(), None, "pre", set(), [], ["x"], 1, 1, "(base)", None,
                              Path("d"))
        self.assertEqual(calls, [True])


@unittest.skipUnless((decomp.TOOLS / "vc6" / "VC98" / "BIN" / "CL.EXE").exists(), "needs tools/vc6")
class RealCompile(unittest.TestCase):
    def test_yx_compiles_and_leaves_no_pch(self):
        d = Path(tempfile.mkdtemp())
        try:
            with switch(True):
                _, obj, out = decomp.compile_unit(UNIT, unit_path(), d, cache=False)
            self.assertIsNotNone(obj, out)
            self.assertTrue(obj.exists())
            self.assertEqual(list(d.rglob("*.pch")), [])
        finally:
            shutil.rmtree(d, ignore_errors=True)


if __name__ == "__main__":
    unittest.main()
