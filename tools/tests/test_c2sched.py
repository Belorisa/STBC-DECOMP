"""Tests of tools/c2sched.py (the scheduler view of tools/c2trace.py --sched) and of c2trace's path helpers: the
priority arithmetic, the pick explanation, the moved list, Git Bash paths and the header-override shadow.

    python -m unittest discover -s tools/tests
"""

import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import c2sched as S  # noqa: E402
import c2trace as T  # noqa: E402

G5 = [-1, 13, -5, -1, -2, 16, 16, 0]     # 0x107A0D98, model 2 (/G5, the default)


def node(seq, height, reads=0, extra=0, succs=1):
    prio = (height << 13) + (reads << 16) + extra
    return {"seq": seq, "height": height, "reads": reads, "succs": succs, "prio": prio, "dyn": prio}


class Priority(unittest.TestCase):
    def test_terms(self):
        t = S.priority_terms(G5, 5, 2, 1, 1, 0)
        self.assertEqual(t["height"], 5 << 13)
        self.assertEqual(t["reads"], 1 << 16)
        self.assertEqual(t["critical"], 0)     # shifted right by 1: a bit never counts under /G5
        self.assertEqual(t["succs"], 0)        # successors >> 5
        self.assertEqual(S.formula(G5), "height<<13 + succs>>5 + read<<16 + float-store<<16")

    def test_residual(self):
        self.assertEqual(S.residual(node(1, 45, extra=1 << 16), G5), 1 << 16)
        self.assertEqual(S.residual(node(1, 45, reads=1), G5), 0)


class Explain(unittest.TestCase):
    def test_tie(self):
        text = S.explain(node(6, 5), node(9, 5), G5)
        self.assertIn("tie on priority", text)
        self.assertIn("#9 ahead of #6", text)

    def test_height(self):
        text = S.explain(node(2, 10), node(6, 5), G5)
        self.assertIn("height 10 against 5", text)
        self.assertIn("#6 needs 5 more height", text)

    def test_float_store(self):
        text = S.explain(node(34, 45, extra=1 << 16), node(35, 49), G5)
        self.assertIn("float store", text)
        self.assertIn("needs 4 more height", text)

    def test_moved(self):
        self.assertEqual(S.moved([1, 2, 3]), [])
        self.assertEqual(S.moved([8, 12, 1, 2]), [(8, 1), (12, 1)])


class Paths(unittest.TestCase):
    def test_msys(self):
        self.assertEqual(T.msys_path("/c/Recomp/x.cpp"), "C:/Recomp/x.cpp")
        self.assertEqual(T.msys_path("C:/Recomp/x.cpp"), "C:/Recomp/x.cpp")
        self.assertEqual(T.msys_path("game/LODModel"), "game/LODModel")
        self.assertEqual(T.msys_path("0x44F800"), "0x44F800")

    def test_shadow(self):
        with tempfile.TemporaryDirectory() as d:
            repl = Path(d) / "NiTList.h"
            repl.write_text("// replacement\n")
            header = (T.ROOT / "src/netimmerse/NiTList.h").resolve()
            inc = str(T.ROOT / "src/netimmerse")
            src = str(T.ROOT / "src/game/LODModel.cpp")
            args = ["/c", "/I", str(T.ROOT / "src/game"), "/I", inc, "/Tp", src]
            out = T.shadow_includes(args, [(header, repl.resolve())], src)
            shadow = T.OUT / "override" / "src" / "netimmerse"
            self.assertEqual((shadow / "NiTList.h").read_text(), "// replacement\n")
            self.assertIn(str(shadow), out)
            self.assertEqual(out.index(str(shadow)), 4)   # the shadow takes the real directory's place in the search
            # the real directory follows it, for includes into its subdirectories ("renderer/NiImage.h")
            self.assertEqual(out.index(inc), out.index(str(shadow)) + 2)
            self.assertIn(src, out)          # the unit's own directory is not shadowed: compiled in place


if __name__ == "__main__":
    unittest.main()
