"""Tests of trial.py's pure parts: the definition's span, its place in the preprocessed text, the substitution, the
bounds (the gate, the cap) and the ranking.

    python -m unittest discover -s tools/tests
"""

import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import trial  # noqa: E402


class FakeEntry:
    def __init__(self, line, end):
        self.line, self.end = line, end


class Spans(unittest.TestCase):
    def test_definition_span_skips_the_annotation_stack(self):
        lines = ["/* banner */", "// FUNCTION: STBC 0x00401000", "// PARKED: a wall", "int F( void ) {",
                 "\treturn 1;", "}"]
        # the annotation is line 2 (1-based); the definition ends on line 6 (0-based 5)
        self.assertEqual(trial.definition_span(lines, FakeEntry(2, 5)), (4, 6))

    def test_preprocessed_span_follows_line_directives(self):
        path = Path("C:/x/unit.cpp")
        text = "\n".join(['#line 1 "C:\\\\x\\\\unit.cpp"', "a", '#line 1 "C:\\\\x\\\\head.h"', "h1", "h2",
                          '#line 3 "C:\\\\x\\\\unit.cpp"', "int F( void ) {", "return 1;", "}", "after"])
        span = trial.preprocessed_span(text, path, 3, 5)
        self.assertEqual([text.split("\n")[i] for i in span], ["int F( void ) {", "return 1;", "}"])
        self.assertEqual(trial.preprocessed_span(text, path, 40, 41), [])

    def test_substitute_keeps_the_line_count(self):
        text = "a\nint F() {\nreturn 1;\n}\nb"
        out = trial.substitute(text, [1, 2, 3], "int F() { return 2; }")
        self.assertEqual(out.split("\n"), ["a", "int F() { return 2; }", "", "", "b"])

    def test_unknown_names(self):
        known = {"int", "F", "return", "x"}
        self.assertEqual(trial.unknown_names("int F() { return NULL; }", known), {"NULL"})
        self.assertEqual(trial.unknown_names("int F() { return x; }", known), set())


class Bounds(unittest.TestCase):
    def test_one_variant_always_runs(self):
        self.assertIsNone(trial.check_gate(None, "abc", 1))

    def test_a_set_needs_a_variant_that_moved_this_source(self):
        self.assertIn("run one variant first", trial.check_gate(None, "abc", 2))
        self.assertIn("run one variant first", trial.check_gate({"src": "abc", "moved": []}, "abc", 2))
        self.assertIn("run one variant first", trial.check_gate({"src": "old", "moved": ["a"]}, "abc", 2))
        self.assertIsNone(trial.check_gate({"src": "abc", "moved": ["a"]}, "abc", 5))

    def test_the_cap(self):
        state = {"src": "abc", "moved": ["a"]}
        self.assertIsNone(trial.check_gate(state, "abc", trial.MAX_VARIANTS))
        self.assertIn("at most", trial.check_gate(state, "abc", trial.MAX_VARIANTS + 1))


class Scoring(unittest.TestCase):
    def test_rank(self):
        rs = [{"name": "b", "score": 90.0}, {"name": "a", "score": None}, {"name": "c", "score": 100.0}]
        self.assertEqual([r["name"] for r in trial.rank(rs)], ["c", "b", "a"])

    def test_masked(self):
        ours, theirs = trial.masked(b"\xe8\0\0\0\0\xc3", [(1, 0, 0x14)], 0, b"\xe8\x10\x20\x30\x40\xc3")
        self.assertEqual(ours, theirs)

    def test_normalised_reads_relocation_fields_alike(self):
        ours = trial.normalised([(0, "mov", "ecx, 0"), (5, "call", "0x5"), (10, "cmp", "dword ptr [edi + 4], 0")])
        retail = trial.normalised([(0, "mov", "ecx, 0x88bf28"), (5, "call", "0x7dc950"),
                                   (10, "cmp", "dword ptr [edi + 4], 0x99ae00")])
        self.assertEqual(ours, retail)


class HeaderDefined(unittest.TestCase):
    def test_braced_end_of_a_member_defined_in_its_class(self):
        lines = ["class A {", "public:", "	A( int x ) : y( x ) { f( \"}\" ); }  // }", "	int y;", "};"]
        self.assertEqual(trial.braced_end(lines, 3), 3)
        self.assertEqual(trial.braced_end(["int F( void ) {", "	if ( a ) {", "	}", "	return '{';", "}", "}"], 1), 5)
        self.assertIsNone(trial.braced_end(["int F( void );"], 1))

    def test_make_shadow_links_the_headers_and_writes_the_variant(self):
        import tempfile
        with tempfile.TemporaryDirectory() as t:
            root = Path(t) / "inc"
            (root / "sub").mkdir(parents=True)
            (root / "a.h").write_text("a", encoding="latin-1")
            (root / "sub" / "b.h").write_text("b", encoding="latin-1")
            (root / "sub" / "c.h").write_text("c", encoding="latin-1")
            (root / "x.cpp").write_text("x", encoding="latin-1")
            sd = trial.make_shadow(root, root / "sub" / "b.h", "variant", Path(t) / "sd")
            self.assertEqual((sd / "sub" / "b.h").read_text(), "variant")
            self.assertEqual((root / "sub" / "b.h").read_text(), "b")
            self.assertEqual((sd / "a.h").read_text(), "a")
            self.assertEqual((sd / "sub" / "c.h").read_text(), "c")
            self.assertFalse((sd / "x.cpp").exists())

    def test_include_root_is_the_deepest_include_directory(self):
        here = Path(__file__).resolve()
        args = ["/nologo", "/I", str(here.parents[2]), "/I", str(here.parents[1]), "x.cpp"]
        self.assertEqual(trial.include_root(args, here), here.parents[1])


@unittest.skipUnless((trial.decomp.BUILD / "report.json").exists() and bool(trial.decomp.target_image()),
                     "needs the toolchain, build/report.json and the retail image")
class HeaderFunctionsEndToEnd(unittest.TestCase):
    """A variant of a function defined in a header compiles from the header's own text: an unchanged spelling gives
    the base's score and bytes."""

    def run_trial(self, addr, variant_text):
        import tempfile
        lines = []
        with tempfile.TemporaryDirectory() as t:
            v = Path(t) / "v.cpp"
            v.write_text(variant_text, encoding="latin-1")
            trial.main([addr, str(v)], out=lines.append)
        return "\n".join(lines)

    def definition(self, addr):
        unit, path, e = trial.find_entry(addr)
        lines = Path(e.path).read_text(encoding="latin-1").split("\n")
        first, last = trial.definition_span(lines, e)
        last = trial.braced_end(lines, first) or last
        return unit, "\n".join(lines[first - 1:last])

    def test_header_function_of_a_yx_unit(self):
        unit, text = self.definition(0x850EC0)      # NiBezierTextureSets::Realloc, NiBezierPatch.h
        self.assertTrue(trial.decomp.auto_pch(trial.decomp.source_files()[unit]))
        out = self.run_trial("0x850EC0", text)
        self.assertIn("base 100.0", out)
        self.assertIn("100.0  no", out)

    def test_header_function_of_a_plain_unit_with_a_macro(self):
        unit, text = self.definition(0x439290)      # NiAlphaAccumulator::RegisterObject, NiAlphaAccumulator.h
        self.assertFalse(trial.decomp.auto_pch(trial.decomp.source_files()[unit]))
        out = self.run_trial("0x439290", text.replace("return false;", "return NULL;", 1))
        self.assertIn("(from the source)", out)
        self.assertNotIn("compile failed", out)


if __name__ == "__main__":
    unittest.main()
