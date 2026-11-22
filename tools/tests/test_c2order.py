"""Tests of tools/c2order.py (the deciding site of a schedule or operand-order difference): the first-divergence
classifier, retail's permutation of a window, the mapping of instructions to scheduler nodes, the probe's target
order and sequence override, and the verdict helpers.

    python -m unittest discover -s tools/tests
"""

import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import c2order as O  # noqa: E402


def ins(*rows):
    return [(0x1000 + 4 * i, m, o, 4) for i, (m, o) in enumerate(rows)]


class Divergence(unittest.TestCase):
    def test_none(self):
        a = ins(("mov", "eax, 1"), ("ret", ""))
        self.assertEqual(O.first_divergence(a, a)["kind"], "none")

    def test_reorder(self):
        # SetInvincible (0x5AE090): the flag's store retail keeps before the warp engines' load
        ours = ins(("mov", "eax, dword ptr [ecx + 0x2d0]"), ("mov", "dl, byte ptr [esp + 4]"), ("test", "eax, eax"),
                   ("mov", "byte ptr [ecx + 0x2e9], dl"), ("je", "0x5ae0c6"))
        retail = ins(("mov", "dl, byte ptr [esp + 4]"), ("mov", "byte ptr [ecx + 0x2e9], dl"),
                     ("mov", "eax, dword ptr [ecx + 0x2d0]"), ("test", "eax, eax"), ("je", "0x5ae0c6"))
        d = O.first_divergence(ours, retail)
        self.assertEqual((d["kind"], d["k"], d["w"]), ("reorder", 0, 4))
        self.assertEqual(O.retail_permutation(ours, retail, 0, 4), [1, 3, 0, 2])

    def test_branch_target_masked(self):
        # a size difference before a forward branch's target is not a difference of the branch
        ours = ins(("je", "0x41a7b0"), ("mov", "ecx, 1"))
        retail = ins(("je", "0x41a7b3"), ("mov", "ecx, 2"))
        self.assertEqual(O.first_divergence(ours, retail)["k"], 1)

    def test_x87_swap(self):
        ours = ins(("fld", "dword ptr [esi + 4]"), ("fmul", "dword ptr [edi + 8]"))
        retail = ins(("fld", "dword ptr [edi + 8]"), ("fmul", "dword ptr [esi + 4]"))
        self.assertEqual(O.first_divergence(ours, retail)["kind"], "x87-swap")
        retail = ins(("fld", "dword ptr [edi + 8]"), ("fsubr", "dword ptr [esi + 4]"))
        ours = ins(("fld", "dword ptr [esi + 4]"), ("fsub", "dword ptr [edi + 8]"))
        self.assertEqual(O.first_divergence(ours, retail)["kind"], "x87-swap")

    def test_base_index(self):
        ours = ins(("mov", "dword ptr [eax + ecx - 8], 0"))
        retail = ins(("mov", "dword ptr [ecx + eax - 8], 0"))
        self.assertEqual(O.first_divergence(ours, retail)["kind"], "base-index")

    def test_reorder_with_rotated_registers(self):
        ours = ins(("mov", "eax, dword ptr [esi + 4]"), ("mov", "ecx, dword ptr [esi + 8]"), ("ret", ""))
        retail = ins(("mov", "eax, dword ptr [esi + 8]"), ("mov", "ecx, dword ptr [esi + 4]"), ("ret", ""))
        d = O.first_divergence(ours, retail)
        self.assertEqual(d["kind"], "reorder-regs")
        self.assertEqual(O.retail_permutation(ours, retail, 0, 2), [1, 0])

    def test_float_copy_and_frame(self):
        ours = ins(("mov", "eax, dword ptr [esi + 0x94]"))
        retail = ins(("fld", "dword ptr [esi + 0x94]"))
        self.assertEqual(O.first_divergence(ours, retail)["kind"], "float-copy")
        ours = ins(("lea", "ecx, [esp + 0x2c]"))
        retail = ins(("lea", "ecx, [esp + 0x1c]"))
        self.assertEqual(O.first_divergence(ours, retail)["kind"], "frame")
        self.assertEqual(O.first_divergence(ins(("sub", "esp, 0x90")), ins(("sub", "esp, 0x84")))["kind"], "frame")

    def test_register(self):
        ours = ins(("mov", "eax, 1"), ("mov", "ebx, 2"), ("push", "eax"), ("push", "ebx"))
        retail = ins(("mov", "ecx, 1"), ("mov", "edx, 2"), ("push", "ecx"), ("push", "edx"))
        self.assertEqual(O.first_divergence(ours, retail)["kind"], "register")


def region(nodes, picks):
    return {"fn": "F", "region": 1, "fp": False,
            "nodes": [{"addr": a, "seq": s, "orig": s, "text": t, "preds": p} for a, s, t, p in nodes],
            "picks": [{"addr": a, "cycle": c, "slot": 0, "ready": r, "earliest": e, "dyn": d}
                      for a, c, r, e, d in picks]}


class Nodes(unittest.TestCase):
    def test_keys(self):
        self.assertEqual(O.node_key("_epush ebx"), "push")
        self.assertEqual(O.node_key("_epop edi"), "pop")
        self.assertEqual(O.node_key("_jcc"), "j")
        self.assertIs(O.node_key("op162 r31"), O.NO_CODE)
        self.assertEqual(O.node_key("fld r31, [var#36]"), "fld")
        self.assertEqual(O.ins_key("jne"), "j")
        self.assertEqual(O.ins_key("jmp"), "jmp")

    def test_map_skips_no_code(self):
        r = region([(10, 1, "fld r31, [var#1]", []), (11, 2, "op162 r31", [(10, 1)]),
                    (12, 3, "fmul r31, [edi+0x28]", [(11, 0)])],
                   [(10, 0, [], {}, {}), (11, 1, [], {}, {}), (12, 2, [], {}, {})])
        listing = ins(("fld", "dword ptr [0x99d244]"), ("fmul", "dword ptr [edi + 0x28]"))
        self.assertEqual(O.map_picks(listing, [r]), {0: (0, 10), 1: (0, 12)})

    def test_target_and_override(self):
        r = region([(10, 1, "a", []), (11, 2, "b", []), (12, 3, "c", [])],
                   [(10, 0, [], {}, {}), (11, 0, [], {}, {}), (12, 1, [], {}, {})])
        order = O.target_order(r, [12, 10])
        self.assertEqual(order, [12, 11, 10])
        self.assertEqual(O.seq_override(r, order), {3: 1, 2: 2, 1: 3})

    def test_ties_and_miss(self):
        # the probe picked 12 over 10 on a tie; 10 came first in our input
        probe = region([(10, 1, "a", []), (12, 2, "c", [])],
                       [(12, 0, [10, 12], {10: 0, 12: 0}, {10: 5, 12: 5}), (10, 1, [10], {10: 0}, {10: 5})])
        self.assertEqual(O.deciding_ties(probe, [12, 10], {10: 1, 12: 2}), [(12, 10)])
        self.assertIsNone(O.first_miss(probe, [12, 10]))
        self.assertEqual(O.first_miss(probe, [10, 12]), (0, 10, 12))

    def test_head_loads(self):
        r = region([(9, 0, "(region entry/exit)", []), (10, 1, "mov esi, [param#10]", [(9, 0)]),
                    (11, 2, "mov ebx, [param#5]", [(9, 0)]), (12, 3, "mov ecx, [esi+0x4], esi", [(10, 1)])], [])
        for x in r["nodes"]:
            x["real"] = x["addr"] != 9
        self.assertTrue(O.head_loads(r, [10, 11]))
        self.assertFalse(O.head_loads(r, [10, 12]))

    def test_chain(self):
        r = region([(10, 1, "fld", []), (11, 2, "op162 r31", [(10, 1)]), (12, 3, "fmul", [(11, 0)]),
                    (13, 4, "fdiv", [(12, 3)])], [])
        issued = {10: 0, 11: 1, 12: 2}
        self.assertEqual(O.chain(r, 13, issued), [(12, 2, 3), (11, 1, 0), (10, 0, 1)])


if __name__ == "__main__":
    unittest.main()
