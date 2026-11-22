"""Tests for tools/levercheck.py: entry splitting and the two suspect classes."""
import os
import sys
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import levercheck  # noqa: E402

TEXT = """# Title

### Section

- **First lever:** closed Foo::Bar
  (0x401000), continued.
  - a sub-bullet
- **Second lever:** Baz::Qux *(open)*.

Plain paragraph, not an entry.
"""


def fn(addr, name, status, score=100.0):
    return {'addr': addr, 'name': name, 'status': status, 'score': score}


class LeverCheckTest(unittest.TestCase):
    def test_split(self):
        e = levercheck.split_entries(TEXT)
        self.assertEqual([x[0] for x in e], [5, 8])
        self.assertIn('sub-bullet', e[0][2])
        self.assertEqual(e[0][1], 'Section')
        self.assertNotIn('Plain paragraph', e[1][2])

    def test_classify(self):
        report = {'functions': [fn('0x401000', 'Foo::Bar', 'DIFF 90', 90.0), fn('0x402000', 'Baz::Qux', 'MATCH')]}
        by_addr, by_name = levercheck.load_functions(report)
        e = levercheck.split_entries(TEXT)
        c0 = levercheck.cited(e[0][2], by_addr, by_name)
        c1 = levercheck.cited(e[1][2], by_addr, by_name)
        self.assertEqual(len(c0), 1)
        self.assertEqual(levercheck.classify(e[0][2], c0), 'all-open-unmarked')
        self.assertEqual(levercheck.classify(e[1][2], c1), 'open-mark-all-matched')
        self.assertIsNone(levercheck.classify('- nothing cited', []))


if __name__ == '__main__':
    unittest.main()
