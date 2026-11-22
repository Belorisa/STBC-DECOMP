import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
import eaxcheck as E  # noqa: E402


def after(*lines):
    """The instructions after a call, as (address, mnemonic, operands) rows with the call first."""
    rows = [(0x1000, "call", "0x2000")]
    for i, line in enumerate(lines):
        mnem, _, ops = line.partition(" ")
        rows.append((0x1005 + i, mnem, ops.strip()))
    return E.scan_after_call(rows, 0)


class AfterCall(unittest.TestCase):
    def test_reads(self):
        for line in ("test eax, eax", "mov ecx, eax", "push eax", "cmp al, 1", "mov dword ptr [esi + 4], eax",
                     "movzx ecx, al", "add eax, 4"):
            self.assertIsNotNone(after(line), line)

    def test_writes_clear_it(self):
        for line in ("mov eax, dword ptr [esp + 8]", "xor eax, eax", "or eax, 0xffffffff", "sbb eax, eax",
                     "fnstsw ax", "pop eax", "lea eax, [esi + 8]", "sete al", "mov al, 1"):
            self.assertIsNone(after(line, "test eax, eax"), line)

    def test_unrelated_instructions_are_skipped(self):
        self.assertIsNotNone(after("mov ecx, esi", "add esp, 8", "test eax, eax"))

    def test_the_block_ends_the_check(self):
        for end in ("ret", "jmp 0x3000", "je 0x3000", "call 0x4000"):
            self.assertIsNone(after(end, "test eax, eax"), end)

    def test_void_callees_come_from_the_decorated_name(self):
        rows = [{"addr": "0x401000", "symbol": "?Update@TGObject@@UAEXM@Z", "name": "TGObject::Update"},
                {"addr": "0x401100", "symbol": "?GetCount@TGObject@@QBEHXZ", "name": "TGObject::GetCount"},
                {"addr": "0x401200", "symbol": "??0TGObject@@QAE@XZ", "name": "TGObject::TGObject"},
                {"addr": "0x401300", "symbol": "_free", "name": "free"}]
        self.assertEqual(set(E.void_callees(rows, [])), {0x401000})


if __name__ == "__main__":
    unittest.main()
