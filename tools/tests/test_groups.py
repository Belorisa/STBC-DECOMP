"""Tests of the shared-wall groups (cards.py add --group, cards.py tag, openwork.py groups), openwork.py untried, the
compiler-state wall listed apart, and the gaps measured from retail's extent of a function that doesn't match.

    python -m unittest discover -s tools/tests
"""

import contextlib
import io
import json
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import cards  # noqa: E402
import openwork  # noqa: E402


def row(addr, name, score, status="DIFF", **kw):
    r = {"addr": "0x%X" % addr, "name": name, "score": score, "status": status, "size": 0x20, "rules": []}
    r.update(kw)
    return r


def entry(date, by, kind="try", **kw):
    e = {"date": date, "kind": kind, "lane": "x", "by": by, "how": "%s's try" % by}
    e.update(kw)
    return e


def run(fn, *args):
    out = io.StringIO()
    with contextlib.redirect_stdout(out):
        fn(*args)
    return out.getvalue()


class FakeCards:
    """cards.read over a dict {address: [entries]}, for the length of a test."""

    def __init__(self, test, table):
        saved = cards.read
        cards.read = lambda addr, root=None: table.get(cards.key(addr), [])
        test.addCleanup(lambda: setattr(cards, "read", saved))


class Groups(unittest.TestCase):
    def test_tag_and_add_store_the_group(self):
        with tempfile.TemporaryDirectory() as root:
            (Path(root) / "build").mkdir()
            (Path(root) / "build" / "report.json").write_text(json.dumps(
                {"functions": [row(0x41F350, "A", 99.7)]}), encoding="utf-8")
            cards.tag(["0x41f350", "0x481CC0"], "nitlist-order", "one template", "opus", root)
            cards.add("0x41F350", "tried X", "opus", "register", root, group="nitlist-order")
            g = cards.groups(root)
            self.assertEqual(sorted(g), ["nitlist-order"])
            self.assertEqual(sorted(g["nitlist-order"]), ["0x41F350", "0x481CC0"])
            self.assertEqual([e["kind"] for e in g["nitlist-order"]["0x41F350"]], ["group", "try"])
            text = "\n".join(cards.lines("0x41F350", root))
            self.assertIn("group  nitlist-order  one template", text)
            self.assertIn("[wall: register]  [group: nitlist-order]", text)
            # a tag is not a try: it doesn't count for merge_lane's missing-try check
            self.assertEqual(sum(1 for e in cards.read("0x481CC0", root) if e["kind"] == "try"), 0)

    def test_group_ids_are_slugs(self):
        with self.assertRaises(SystemExit):
            cards.group_id("Has Spaces")
        self.assertEqual(cards.group_id("ni-point3.ctor"), "ni-point3.ctor")

    def test_compiler_state_is_a_wall(self):
        self.assertIn("compiler-state", cards.WALLS)

    def test_listing(self):
        table = {"g1": {"0x500000": [entry("2026-11-10", "opus", "group")],
                        "0x500100": [entry("2026-11-11", "opus", group="g1")]}}
        FakeCards(self, {"0x500000": [entry("2026-11-12", "sonnet", how="the last one", wall="register")],
                         "0x500100": [entry("2026-11-11", "opus", group="g1")]})
        rows = [row(0x500000, "A::F", 99.0), row(0x500100, "A::G", 80.0, wall="inline")]
        text = run(openwork.cmd_groups, None, rows, table)
        self.assertIn("g1", text)
        self.assertIn("99.0 80.0", text)
        self.assertIn("2026-11-12 sonnet 0x500000: the last one", text)
        one = run(openwork.cmd_groups, "g1", rows, table)
        self.assertIn("group g1: 2 functions", one)
        self.assertIn("wall register", one)
        self.assertIn("no group g2", run(openwork.cmd_groups, "g2", rows, table))


class Untried(unittest.TestCase):
    rows = [row(0x500000, "Never", 90.0), row(0x500100, "SonnetOnly", 95.0), row(0x500200, "Opus", 99.0),
            row(0x500300, "Old", 80.0), row(0x500400, "Matched", 100.0, status="MATCH"),
            row(0x500500, "AutoOnly", 70.0)]

    def setUp(self):
        FakeCards(self, {"0x500100": [entry("2026-11-10", "sonnet")], "0x500200": [entry("2026-11-10", "opus")],
                         "0x500300": [entry("2026-11-01", "opus")], "0x500500": [entry("2026-11-12", "merge_lane")]})

    def names(self, **kw):
        return [r["name"] for r, _ in openwork.untried(self.rows, **kw)]

    def test_default(self):
        self.assertEqual(self.names(), ["SonnetOnly", "Never", "Old", "AutoOnly"])

    def test_bounds_and_score(self):
        self.assertEqual(self.names(lo=0x500000, hi=0x500200, min_score=91), ["SonnetOnly"])

    def test_authors_and_date(self):
        self.assertEqual(self.names(ignore=()), ["Never", "Old"])
        self.assertEqual(self.names(since="2026-10-01"), ["SonnetOnly", "Never", "AutoOnly"])

    def test_command_line(self):
        text = run(openwork.cmd_untried, ["0x500000", "0x500400", "85", "--ignore", ""], self.rows)
        self.assertIn("Never", text)
        self.assertNotIn("SonnetOnly", text)
        self.assertIn("1 functions", text)


class CompilerState(unittest.TestCase):
    rows = [row(0x500000, "Bounded", 99.0, parked="register: x"), row(0x500100, "Other", 98.0, parked="register: y"),
            row(0x500200, "Draft", 90.0, draft="x")]

    def setUp(self):
        FakeCards(self, {"0x500000": [entry("2026-11-12", "opus", wall="compiler-state")],
                         "0x500200": [entry("2026-11-12", "opus", wall="compiler-state")]})

    def test_bounded_needs_the_card(self):
        self.assertTrue(openwork.bounded(self.rows[0]))
        self.assertFalse(openwork.bounded(self.rows[1]))
        self.assertFalse(openwork.bounded(row(0x500300, "Read", 90.0, wall="compiler-state")))

    def test_drafts_list_it_apart(self):
        text = run(openwork.cmd_drafts, 0.0, self.rows)
        head, tail = text.split("compiler-state, proven bounded")
        self.assertNotIn("Draft", head)
        self.assertIn("Draft", tail)
        self.assertIn("1 of them proven bounded", tail)

    def test_parked_lists_it_apart(self):
        text = run(openwork.cmd_parked, 0.0, self.rows)
        self.assertTrue(text.index("Other") < text.index("compiler-state") < text.index("Bounded"))

    def test_walls_table(self):
        text = run(openwork.cmd_walls, None, 0.0, self.rows)
        self.assertIn("proven bounded", text)
        self.assertTrue(text.rstrip().splitlines()[-1].startswith("compiler-state"))

    def test_phases_counts_it(self):
        saved = openwork.original_asm, openwork.import_thunks
        openwork.original_asm = openwork.import_thunks = lambda text=None, annotated=(): {}
        self.addCleanup(lambda: (setattr(openwork, "original_asm", saved[0]),
                                 setattr(openwork, "import_thunks", saved[1])))
        text = run(openwork.cmd_phases, self.rows, [], {"text_bytes": 100, "source_bytes": 50})
        self.assertIn("still open, in the counts above): 2 functions", text)


class Img:
    """A retail image of one function at 0x1000: 0x50 bytes of `nop`, `ret`, then int3 fill to 0x1060."""

    def __init__(self):
        self.code = b"\x90" * 0x50 + b"\xC3" + b"\xCC" * 0x0F + b"\x90" * 0x100

    def read(self, va, n):
        off = va - 0x1000
        return self.code[off:off + n] if 0 <= off < len(self.code) else None


class Gaps(unittest.TestCase):
    rows = [row(0x1000, "Short", 70.0), row(0x10A0, "Next", 100.0, status="MATCH")]

    def test_extent_of_a_function_that_doesnt_match(self):
        self.assertEqual(openwork.retail_extents(self.rows, Img()), {0x1000: 0x51})

    def test_the_gap_starts_at_retails_end(self):
        ext = openwork.retail_extents(self.rows, Img())
        self.assertEqual([(lo, hi, t) for lo, hi, _, t in openwork.gaps_of(self.rows, ext)], [(0x1060, 0x10A0, None)])
        # read from the compiled size alone, the gap would start inside the function
        self.assertEqual(openwork.gaps_of(self.rows)[0][0], 0x1020)

    def test_unknown_extent_marks_a_tail(self):
        rows = [row(0x1000, "Short", 70.0), row(0x1040, "Next", 100.0, status="MATCH"),
                row(0x1100, "Far", 100.0, status="MATCH")]
        ext = {0x1000: None}
        self.assertEqual(openwork.end_of(rows[0], ext), (0x1020, "Short"))


if __name__ == "__main__":
    unittest.main()
