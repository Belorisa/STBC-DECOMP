"""Tests of tools/statics.py's pure rules: the stub pairing of `renumber` and the ownership rule of `place-all`.

    python -m unittest discover -s tools/tests
"""

import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import statics  # noqa: E402


class PairRenames(unittest.TestCase):
    """renumber: a `jmp` stub is named after the body it enters, once the bodies carry their new numbers."""

    def test_stub_left_at_old_number_follows_its_body(self):
        # CharacterClass: the body 0x666C10 went _$E306 -> _$E352; its stub 0x666C00 stayed _$E307, which still
        # compiles to a `jmp`, so only the pairing tells it should be _$E353
        annotated = {0x666C00: 307, 0x666C10: 352}
        retail = {0x666C00: 0x666C10}
        stubs = {307: 306, 353: 352}
        self.assertEqual(statics.pair_renames(annotated, retail, stubs), [(0x666C00, 307, 353)])

    def test_stub_already_right_is_left(self):
        self.assertEqual(statics.pair_renames({0x10: 353, 0x20: 352}, {0x10: 0x20}, {353: 352}), [])

    def test_body_not_annotated_or_ambiguous_is_left(self):
        self.assertEqual(statics.pair_renames({0x10: 5}, {0x10: 0x20}, {7: 6}), [])
        self.assertEqual(statics.pair_renames({0x10: 5, 0x20: 6}, {0x10: 0x20}, {7: 6, 9: 6}), [])


class Blocks(unittest.TestCase):
    """place-all: a unit boundary needs every earlier store below every later one."""

    def test_interleaved_stores_are_one_block(self):
        # stubs in code order; the first unit's stores interleave (name order), the second's lie above them
        stores = [0x120, 0x100, 0x110, 0x200, 0x230, 0x210]
        self.assertEqual(statics.blocks(stores), [(0, 3), (3, 4), (4, 6)])

    def test_a_late_store_binds_everything_before_it(self):
        # OptimizedAvoidObstacles: a stub inside its code stores among its tail run's stores
        stores = [0x25C, 0x1F0, 0x2DC, 0x23C, 0x320, 0x434]
        self.assertEqual(statics.blocks(stores), [(0, 4), (4, 5), (5, 6)])

    def test_empty(self):
        self.assertEqual(statics.blocks([]), [])


class BlockOwner(unittest.TestCase):
    def test_data_decides(self):
        self.assertEqual(statics.block_owner({"game/A"}, {"game/B"}), ("game/B", "bss"))

    def test_data_first(self):
        owners = [("game/A", "code"), ("game/B", "code"), ("game/A", "bss"), (None, None)]
        self.assertEqual(statics.data_first(owners),
                         [(None, None), ("game/B", "code"), ("game/A", "bss"), (None, None)])

    def test_data_of_several_units_needs_code_to_pick(self):
        self.assertEqual(statics.block_owner({"game/B"}, {"game/A", "game/B"}), ("game/B", "bss"))
        self.assertEqual(statics.block_owner(set(), {"game/A", "game/B"}), (None, None))

    def test_code_decides_without_data(self):
        self.assertEqual(statics.block_owner({"game/A"}, set()), ("game/A", "code"))
        self.assertEqual(statics.block_owner({"game/A", "game/B"}, set()), (None, None))


class FillBetween(unittest.TestCase):
    def test_gap_between_one_units_blocks(self):
        owners = [("A", "bss"), (None, None), ("A", "code"), (None, None), ("B", "bss")]
        self.assertEqual(statics.fill_between(owners),
                         [("A", "bss"), ("A", "between"), ("A", "code"), (None, None), ("B", "bss")])


class DefaultOrder(unittest.TestCase):
    def test_right_after_code_trails_it(self):
        self.assertEqual(statics.default_order(0x1100, "A", 0x1080, "B"), [("A", "trailing"), ("B", "leading")])

    def test_after_unannotated_code_leads_the_next_unit(self):
        self.assertEqual(statics.default_order(0x3000, "A", 0x1080, "B"), [("B", "leading"), ("A", "trailing")])


class Trimmed(unittest.TestCase):
    def test_far_point_dropped(self):
        self.assertEqual(statics.trimmed([0x9821F0, 0x982200, 0x982210, 0x99F000]), [0x9821F0, 0x982200, 0x982210])


class DropLines(unittest.TestCase):
    def test_block_heading_goes_with_its_last_line(self):
        text = "\n".join(['#include "A.h"', "", statics.REGISTRATION_BLOCK, "// FUNCTION: STBC 0x00401000 _$E5",
                          "// FUNCTION: STBC 0x00401010 _$E4", "", "int x;"])
        self.assertEqual(statics.drop_lines(text, {0x401000, 0x401010}), '#include "A.h"\n\nint x;')

    def test_other_lines_stay(self):
        text = "\r\n".join([statics.REGISTRATION_BLOCK, "// FUNCTION: STBC 0x00401000 _$E5",
                            "// FUNCTION: STBC 0x00401010 _$E4"])
        self.assertEqual(statics.drop_lines(text, {0x401000}),
                         "\r\n".join([statics.REGISTRATION_BLOCK, "// FUNCTION: STBC 0x00401010 _$E4"]))


class RenumberRefresh(unittest.TestCase):
    """renumber compares the units it would rewrite afresh before reading their rows and objects: a slot's build/
    can predate its source (AppWrap's 0x64C570/0x64C580 were once renumbered from a previous lane's report)."""

    def _root(self, rows):
        import json
        import tempfile
        root = Path(tempfile.mkdtemp())
        (root / "build").mkdir()
        (root / "build" / "report.json").write_text(json.dumps({"functions": rows}))
        return root

    def test_only_units_with_a_failing_initialiser(self):
        rows = [{"addr": "0x64C570", "name": "_$E964", "unit": "game/AppWrap", "static": True, "status": "DIFF"},
                {"addr": "0x401000", "name": "Foo", "unit": "game/Other", "status": "DIFF"},
                {"addr": "0x402000", "name": "_$E5", "unit": "game/Fine", "static": True, "status": "MATCH"}]
        self.assertEqual(statics.failing_static_units(rows), ["game/AppWrap"])

    def test_stale_rows_are_refreshed_before_use(self):
        import json
        rows = [{"addr": "0x64C570", "name": "_$E964", "unit": "game/AppWrap", "static": True, "status": "DIFF"}]
        root = self._root(rows)
        asked = []

        def refresh(r, units):
            # the fresh compare finds the initialiser matching: the stale row described an older source
            asked.append(units)
            fresh = [dict(rows[0], status="MATCH")]
            (Path(r) / "build" / "report.json").write_text(json.dumps({"functions": fresh}))
        changes, problems = statics.renumber(root, refresh=refresh)
        self.assertEqual(asked, [["game/AppWrap"]])
        self.assertEqual((changes, problems), ([], []))

    def test_nothing_to_refresh_when_every_initialiser_matches(self):
        rows = [{"addr": "0x402000", "name": "_$E5", "unit": "game/Fine", "static": True, "status": "MATCH"}]
        asked = []
        statics.renumber(self._root(rows), refresh=lambda r, u: asked.append(u))
        self.assertEqual(asked, [])


if __name__ == "__main__":
    unittest.main()


class Scope(unittest.TestCase):
    """place-all --units / --changed: only the named units change."""

    def test_units_of_keeps_sources_only(self):
        self.assertEqual(statics.units_of(["src/game/A.cpp", "src/game/A.h", "tools/x.py", "src/python/B.c\n"]),
                         {"game/A", "python/B"})

    def test_arguments(self):
        self.assertEqual(statics.place_all_args(["place-all"]), (False, False, None))
        self.assertEqual(statics.place_all_args(["place-all", "--write", "--units", "src/game/A.cpp", "game/B"]),
                         (True, False, {"game/A", "game/B"}))
        self.assertEqual(statics.place_all_args(["place-all", "--units", "game/A", "--evict"]),
                         (False, True, {"game/A"}))

    def test_a_unit_with_failing_registrations_is_left(self):
        # TGTimerManager after a +6 shift: its 140 registration rows fail; placing around them took 34 more stubs
        rows = [{"unit": "game/T", "addr": "0x10", "static": True, "status": "DIFF"},
                {"unit": "game/U", "addr": "0x20", "static": True, "status": "MATCH"},
                {"unit": "game/V", "addr": "0x30", "static": True, "status": "DIFF"},   # not a registration
                {"unit": "game/W", "addr": "0x40", "status": "DIFF"}]                 # a function, not a static
        self.assertEqual(statics.stale_units(rows, {0x10, 0x20, 0x40}, {"game/T", "game/U", "game/V", "game/W"}),
                         ["game/T"])


class RenumberGuards(unittest.TestCase):
    """renumber: never one number for two annotations, and never from rows older than the object."""

    def test_doubled(self):
        text = ("// FUNCTION: STBC 0x0058b010 _$E234\n// FUNCTION: STBC 0x0058b020 _$E233\n"
                "// FUNCTION: STBC 0x0058b030 _$E234\n")
        self.assertEqual(statics.doubled(text), [234])
        self.assertEqual(statics.doubled(text.replace("30 _$E234", "30 _$E235")), [])

    def test_stale_rows(self):
        import os
        import tempfile
        with tempfile.TemporaryDirectory() as d:
            report = Path(d) / "report.json"
            report.write_text("{}")
            (Path(d) / "obj" / "game").mkdir(parents=True)
            fresh, late = Path(d) / "obj" / "game" / "A.obj", Path(d) / "obj" / "game" / "B.obj"
            fresh.write_bytes(b"")
            late.write_bytes(b"")
            t = os.stat(report).st_mtime_ns
            os.utime(fresh, ns=(t - 10**9, t - 10**9))
            os.utime(late, ns=(t + 10**9, t + 10**9))
            rows = [{"unit": "game/A"}, {"unit": "game/B"}, {"unit": "game/C"}]
            self.assertEqual(statics.stale_rows(rows, report, Path(d) / "obj"), ["game/B"])


def fixture(rows):
    """A unit's text and its fit tables from [(address, annotated number, numbers its bytes fit, numbers its bytes and
    targets fit)]: the shape `renumber_unit` reads from the object and retail."""
    text = "".join("// FUNCTION: STBC 0x%08x _$E%d\n" % (a, n) for a, n, _, _ in rows)
    fit = {a: list(f) for a, _, f, _ in rows}
    fit_t = {a: list(t) for a, _, _, t in rows}
    return text, (lambda a: fit[a]), (lambda a: fit_t[a])


class RenumberShift(unittest.TestCase):
    """renumber_unit: one shift, or one per block, applies to rows that were failing before it too (CameraMode's
    0x41DFB0, TGObject's per-block +2); only rows that really disagree are refused."""

    def run_unit(self, rows, failing, was_failing=()):
        text, fit, fit_t = fixture(rows)
        new, changes, problems = statics.renumber_unit(
            "game/U", text, ["0x%X" % a for a in failing], fit, fit_t,
            was_failing=lambda a: int(a, 16) in was_failing)
        return dict((int(a, 16), n) for a, n in statics.annotations(new)), changes, problems

    def test_camera_mode_failing_row_moves_with_its_neighbours(self):
        # Exit destructors are byte-identical (each fits every exit number); the bodies fit only their own. The
        # header gained two statics: every number is two short. 0x41DFB0 failed before and fits no number now.
        exits = [378, 383, 388]
        rows = [(0x41DF80, 379, [381], []), (0x41DF90, 376, exits, []), (0x41DFA0, 378, [380], []),
                (0x41DFB0, 377, [], []),
                (0x41DFD0, 381, exits, []), (0x41DFE0, 380, [382], [])]
        got, changes, problems = self.run_unit(rows, [a for a, *_ in rows], was_failing={0x41DFB0})
        self.assertEqual(got, {0x41DF80: 381, 0x41DF90: 378, 0x41DFA0: 380, 0x41DFB0: 379, 0x41DFD0: 383,
                               0x41DFE0: 382})
        self.assertEqual(len(changes), 6)
        self.assertEqual([p for p in problems if "left failing: 0x41DFB0" in p], problems)
        self.assertEqual(len(problems), 1)

    def test_failing_row_alone_is_left_and_reported(self):
        rows = [(0x41DF80, 381, [381], []), (0x41DFB0, 379, [374, 384], [])]
        got, changes, problems = self.run_unit(rows, [0x41DFB0], was_failing={0x41DFB0})
        self.assertEqual(got[0x41DFB0], 379)
        self.assertEqual(changes, [])
        self.assertTrue(problems and "left failing: 0x41DFB0" in problems[0] and "no single shift" not in problems[0])

    def test_tgobject_quads_keep_theirs_the_rest_moves_by_block(self):
        # The <iostream> quads moved by one (a static before them), the registrations after them by two.
        rows = [(0x6EFDE0, 31, [32], []), (0x6EFDF0, 28, [29, 35], []), (0x6EFE00, 30, [31], []),
                (0x6EFE10, 29, [30], []),
                (0x6EFE60, 73, [75], []), (0x6EFE70, 72, [74, 29], []), (0x6EFE80, 77, [79], []),
                (0x6EFE90, 76, [78], [])]
        got, changes, problems = self.run_unit(rows, [a for a, *_ in rows])
        self.assertEqual([got[a] for a, *_ in rows], [32, 29, 31, 30, 75, 74, 79, 78])
        self.assertEqual(problems, [])

    def test_tgobject_stub_settled_alone_wrongly_joins_the_shift(self):
        # A jmp stub fits one initialiser alone by its targets, read from the stale annotations: a byte-identical twin
        # of its body (444). The number clashes with the body shifted onto it, so the stub joins the +2 shift.
        stubs = [439, 444, 449]
        rows = [(0x6F08A0, 440, [432, 437, 442, 447], []), (0x6F08B0, 437, stubs, [444]),
                (0x6F09A0, 442, [444, 439], [444]), (0x6F09B0, 445, [447], [])]
        got, changes, problems = self.run_unit(rows, [a for a, *_ in rows])
        self.assertEqual(got, {0x6F08A0: 442, 0x6F08B0: 439, 0x6F09A0: 444, 0x6F09B0: 447})
        self.assertEqual(problems, [])

    def test_rows_that_really_disagree_are_refused(self):
        rows = [(0x10, 10, [11], []), (0x20, 20, [23], []), (0x30, 30, [31], []), (0x40, 40, [43], [])]
        got, changes, problems = self.run_unit(rows, [a for a, *_ in rows])
        self.assertEqual(got, {0x10: 10, 0x20: 20, 0x30: 30, 0x40: 40})
        self.assertEqual(changes, [])
        self.assertTrue(problems[0].startswith("game/U: no single shift fits"))

    def test_reverse_numbered_twins_move_by_one_offset(self):
        # VisibleDamageBuilder: the exit destructors of two function-local static bit arrays, byte-identical and
        # numbered in reverse address order (0x4B5620 _$E435, 0x4B5630 _$E434), both one short. Each fits both
        # numbers, bytes and targets alike: 0x4B5620's own name still fits, 0x4B5630 allows +1 or +2; together only
        # +1 keeps the run's order. A body settled alone by its targets moved by +1 beside them.
        rows = [(0x4B5620, 435, [436, 435], [436, 435]), (0x4B5630, 434, [436, 435], [436, 435]),
                (0x4BB990, 453, [454], [454])]
        got, changes, problems = self.run_unit(rows, [a for a, *_ in rows])
        self.assertEqual(got, {0x4B5620: 436, 0x4B5630: 435, 0x4BB990: 454})
        self.assertEqual(problems, [])
        self.assertEqual(len(changes), 3)

    def test_twin_failing_for_its_own_reason_is_left(self):
        # A row whose own name still fits its bytes (and a twin's) doesn't move when the shifters settle without it.
        rows = [(0x10, 10, [11], []), (0x20, 20, [21], []), (0x30, 30, [30, 35], [])]
        got, changes, problems = self.run_unit(rows, [a for a, *_ in rows])
        self.assertEqual(got, {0x10: 11, 0x20: 21, 0x30: 30})
        self.assertEqual(problems, [])
        # Alone, it is no shift at all.
        got, changes, problems = self.run_unit(rows[2:], [0x30])
        self.assertEqual((got, changes, problems), ({0x30: 30}, [], []))

    def test_shift_plan_blocks(self):
        order = ["a", "b", "c", "d", "e"]
        self.assertEqual(statics.shift_plan({"a": {1, 5}, "b": {1}, "d": {2}, "e": {2, 7}}, {"c"}, order),
                         {"a": 1, "b": 1, "c": 1, "d": 2, "e": 2})
        self.assertIsNone(statics.shift_plan({"a": {1}, "b": {2}, "c": {1}}, set(), ["a", "b", "c"]))
