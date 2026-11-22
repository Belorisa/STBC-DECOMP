"""Tests of tools/c2inline.py: the margin lines, the replay of a recorded trace, --whatif, the retail comparison and
the --worth statement cut. The recorded trace in data/ is c2trace's record of BridgeObjectClass::SetupLighting.

    python -m unittest discover -s tools/tests
"""

import copy
import json
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import c2inline as I  # noqa: E402

DATA = Path(__file__).resolve().parent / "data" / "c2trace_SetupLighting.json"


def site(**kw):
    s = {"callee": "f", "raw": "?f@@YAXXZ", "cost": 66, "budget": 42, "force": False, "limit": 8, "depth": 2,
         "params": 1, "args": 1, "verdict": "reject", "why": ["cost 66 > budget 42"], "share": None, "nested": None}
    s.update(kw)
    return s


def recorded():
    return json.loads(DATA.read_text(encoding="utf-8"))["functions"]


def top(fn, k):
    return fn["sites"][k - 1]


class Margins(unittest.TestCase):
    def test_nested_reject(self):
        # SetupLighting [2/40] operator= -> DecRefCount: share 1651 / 39 = 42, cost 66
        t = I.margin_text(site(), parent={"L": 1651, "R": 39, "pos": "[2/40]", "share": 42})
        self.assertIn("needs >= 24 more units here", t)
        self.assertIn("share 42 -> 66: >= 923 more left at [2/40] above", t)
        self.assertIn("14 fewer sites from there on", t)
        self.assertEqual(1651 // (39 - 14), 66)
        self.assertLess(1651 // (39 - 13), 66)

    def test_nested_accept_within_margin(self):
        s = site(verdict="inline", why=[], cost=41, budget=42)
        t = I.margin_text(s, parent={"L": 1651, "R": 39, "pos": "[2/40]", "share": 42})
        self.assertIn("flips with >= 2 fewer units here", t)
        self.assertIn(">= 53 fewer left at [2/40] above", t)
        self.assertIn("2 more sites after it there", t)
        self.assertLess((1651 - 53) // 39, 41)
        self.assertGreaterEqual((1651 - 52) // 39, 41)
        self.assertLess(1651 // 41, 41)
        self.assertGreaterEqual(1651 // 40, 41)

    def test_wide_margin_and_free_sites_are_quiet(self):
        self.assertIsNone(I.margin_text(site(verdict="inline", why=[], cost=41, budget=90), fn={"own": 10,
                                                                                                "budget": 1000}))
        self.assertIsNone(I.margin_text(site(cost=24, budget=10, why=[])))
        self.assertIsNone(I.margin_text(site(why=["depth 3 > inline_depth 2"])))

    def test_top_level(self):
        t = I.margin_text(site(cost=149, budget=88, why=["cost 149 > budget 88"]), fn={"own": 871, "budget": 1742})
        self.assertEqual(t, "rejected: needs >= 61 more units here (own cost +31, or 61 fewer units spent before it)")
        t = I.margin_text(site(verdict="inline", why=[], cost=60, budget=63), fn={"own": 400, "budget": 1000})
        self.assertIn("budget is at the 1000 floor", t)
        t = I.margin_text(site(verdict="inline", why=[], cost=60, budget=63), fn={"own": 871, "budget": 1742})
        self.assertIn("own cost -2", t)


class Replay(unittest.TestCase):
    def test_replay_agrees_with_the_compiler(self):
        for fn in recorded():
            self.assertEqual(I.check_replay(fn), [])

    def test_own_cost_flip_matches_the_margin(self):
        fn = recorded()[0]
        get_next = top(fn, 35)
        self.assertEqual((get_next["callee"].split("::")[-1], get_next["verdict"]), ("GetNext", "reject"))
        self.assertIn("own cost +31", I.margin_text(get_next, fn=fn))
        _, _, rows = I.whatif(fn, [I.parse_edit("own=+30")])
        self.assertNotIn((35,), [p for p, _, _, old, new in rows if old and old["verdict"] != new["verdict"]])
        _, f, rows = I.whatif(fn, [I.parse_edit("own=+31")])
        flips = {p: (old["verdict"], new["verdict"]) for p, _, _, old, new in rows if old and
                 old["verdict"] != new["verdict"]}
        self.assertEqual(flips[(35,)], ("reject", "inline"))
        self.assertEqual(flips[(38,)], ("inline", "reject"))   # NiColor pays for it

    def test_removing_a_site_raises_the_shares_before_it(self):
        fn = recorded()[0]
        base, f, rows = I.whatif(fn, [I.parse_edit("site=40,-1")])
        old_share = top(base, 2)["sim"]["share"]
        new_share = top(f, 2)["sim"]["share"]
        self.assertEqual((old_share, new_share), (1651 // 39, 1651 // 38))

    def test_added_site_is_charged(self):
        fn = recorded()[0]
        _, f, rows = I.whatif(fn, [I.parse_edit("site=34,+1,cost=60")])
        added = top(f, 34)
        self.assertEqual(added["sim"]["verdict"], "inline")
        self.assertEqual(top(f, 35)["sim"]["budget"], 169 - 60)

    def test_borrowed_copy(self):
        # a site rejected in the record borrows its copy's sites from another expansion of the same callee
        fn = recorded()[0]
        tmpl = I.templates([fn])
        self.assertIn(top(fn, 35)["raw"], tmpl)

    def test_parse_edit(self):
        self.assertEqual(I.parse_edit("site=6.1,-1"), {"path": (6, 1), "delta": -1, "cost": 24})
        self.assertEqual(I.parse_edit("own=-12"), {"own": -12})
        with self.assertRaises(ValueError):
            I.parse_edit("site=6")


class Retail(unittest.TestCase):
    def test_count_mismatch_names_candidates(self):
        fn = recorded()[0]
        raw = top(fn, 35)["raw"]          # GetNext: 3 expanded, 1 out of line
        rows = {r[1]: r for r in I.retail_compare(fn, {raw: 2})}
        name, _, r, ours, n_in, cands, note = rows[raw]
        self.assertEqual((r, ours, n_in), (2, 1, 3))
        self.assertIn("retail has 1 more out of line", note)
        self.assertEqual(cands[0][0], (19,))       # the tightest expansion: budget 684 - 149
        rows = {r[1]: r for r in I.retail_compare(fn, {raw: 0})}
        self.assertEqual(rows[raw][5][0][0], (35,))

    def test_stale_object(self):
        fn = recorded()[0]
        raw = top(fn, 35)["raw"]
        rows = {r[1]: r for r in I.retail_compare(fn, {raw: 1}, {raw: 2})}
        self.assertIn("stale", rows[raw][6])


class Worth(unittest.TestCase):
    def test_statements_and_pairs(self):
        # symbol constant assign discard | symbol load discard | symbol load seq discard | return
        ops = [0x26, 0x33, 0x32, 0x4B, 0x26, 0x30, 0x4B, 0x26, 0x30, 0x4C, 0x4B, 0x41]
        st = I.statements(ops)
        self.assertEqual([len(s) for s in st], [4, 3, 4, 1])
        single, pairs = I.worth(ops, 4)
        self.assertEqual([i for i, _ in single], [1, 3])
        self.assertEqual(pairs, [])
        self.assertEqual([i for i, _, _ in I.worth(ops, 5)[1]], [3])
        single, pairs = I.worth(ops, 7)
        self.assertEqual([i for i, _, _ in pairs], [1, 2])


if __name__ == "__main__":
    unittest.main()
