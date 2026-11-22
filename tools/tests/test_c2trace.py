"""Tests of tools/c2trace.py: the verdict text of a rejected site, and (when the toolchain is present) a live run of
the real back end on a scratch unit, checking the inline rules docs/c2-backend.md states.

    python -m unittest discover -s tools/tests
"""

import io
import json
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import c2trace as T  # noqa: E402


class Verdicts(unittest.TestCase):
    def tracer(self):
        t = T.InlineTracer(lambda raw, name: True, {"inline"}, out=io.StringIO())
        t.active = True
        t.stack = [{"depth": 1, "budget": 100, "sites": 2, "visited": 1}]
        return t

    def site(self, **kw):
        s = {"callee": "f", "cost": 50, "budget": 10, "why": [], "force": False, "limit": 8, "depth": 1,
             "ok": False, "copied": False, "params": 1, "args": 1}
        s.update(kw)
        return s

    def test_budget_reason(self):
        t = self.tracer()
        t.site = self.site(why=["cost 50 > budget 10"])
        self.assertEqual(t.reject_text(), "not inlined: cost 50 > budget 10")

    def test_argument_mismatch(self):
        t = self.tracer()
        t.site = self.site(args=2)
        self.assertEqual(t.reject_text(), "not inlined: 2 arguments for 1 parameters")

    def test_late(self):
        t = self.tracer()
        t.site = self.site(ok=True)
        self.assertIn("after the budget gate", t.reject_text())

    def test_line_shows_non_default_depth(self):
        t = self.tracer()
        t.site = self.site(limit=1)
        t.flush_site("x")
        self.assertIn("(inline_depth 1)", t.out.getvalue())


@unittest.skipUnless((T.vc6.COMPILERS[None] / "C2.DLL").exists() and sys.platform == "win32", "needs tools/vc6")
class Live(unittest.TestCase):
    SRC = """
struct P { float x, y, z; P() { x = y = z = 0; } };
#pragma inline_depth(1)
struct B { P a; int f; B() { f = 1; } };
#pragma inline_depth()
struct C { P a; C() {} };
void use(void*);
void t() { B b; C c; use(&b); use(&c); }
"""

    def test_depth_belongs_to_the_holder(self):
        with tempfile.TemporaryDirectory() as d:
            src = Path(d) / "depth.cpp"
            src.write_text(self.SRC)
            out = io.StringIO()
            old = sys.stdout
            sys.stdout = out
            try:
                T.main([str(src), "t"])
            finally:
                sys.stdout = old
        text = out.getvalue()
        self.assertIn("budget max(1000, 2*", text)
        self.assertRegex(text, r"P::P .*not inlined: depth 2 > inline_depth 1")
        self.assertRegex(text, r"\[1/2\] B::B .*inline \(cost <= 40: free\)")
        self.assertRegex(text, r"budget 500 = left / sites not yet visited")

    BIG = """
struct A { int a[40]; int Big(int k); };
inline int A::Big(int k) {
    int s = 0;
    s += a[0] * k; s += a[1] * k; s += a[2] * k; s += a[3] * k; s += a[4] * k;
    s += a[5] * k; s += a[6] * k; s += a[7] * k; s += a[8] * k; s += a[9] * k;
    s += a[10] * k; s += a[11] * k; s += a[12] * k; s += a[13] * k; s += a[14] * k;
    return s;
}
int Caller(A *p) { return p->Big(3); }
"""

    def run_trace(self, text, *args):
        with tempfile.TemporaryDirectory() as d:
            src = Path(d) / "big.cpp"
            src.write_text(text)
            out = io.StringIO()
            old = sys.stdout
            sys.stdout = out
            try:
                T.main([str(src), *args])
            finally:
                sys.stdout = old
        return out.getvalue()

    def test_no_size_limit_under_our_flags(self):
        # docs/c2-backend.md, "Candidates": the 0x10 << vol limit applies only when 0x107AC0B4 is 0; it is 1 in our
        # builds, so a cost above 128 is a candidate and is expanded when the budget holds it
        text = self.run_trace(self.BIG, "Caller", "--flags", "/O2 /Ob1")
        self.assertRegex(text, r"A::Big +cost +2\d\d +budget +1000 +inline -> budget")
        self.assertNotIn("size limit on", text)

    def test_recorded_trace_and_whatif(self):
        text = self.run_trace(self.BIG, "Caller", "--flags", "/O2 /Ob1", "--whatif", "own=+0")
        self.assertIn("recorded: build/c2trace/big--Caller.trace.json", text)
        self.assertIn("0 verdicts flip", text)
        rec = json.loads((T.OUT / "big--Caller.trace.json").read_text(encoding="utf-8"))
        site = rec["functions"][0]["sites"][0]
        self.assertEqual((site["raw"], site["verdict"], site["nested"]), ("?Big@A@@QAEHH@Z", "inline", []))


if __name__ == "__main__":
    unittest.main()
