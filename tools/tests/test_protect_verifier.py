import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent / "hooks"))
import protect_verifier as H  # noqa: E402

SLOT = "C:/Recomp/stbc-wt/slot2"
MAIN = "C:/Recomp/stbc-decomp"


def call(tool, cwd=SLOT, **args):
    return {"tool_name": tool, "cwd": cwd, "tool_input": args}


class FileTools(unittest.TestCase):
    def test_protected_files_in_a_slot_are_refused(self):
        for rel in ("config/baseline.json", "tools/decomp.py", "tools/rules.py", "tools/merge_lane.py",
                    "orig/stbc.exe", "build/gate/game/Foo.obj"):
            self.assertTrue(H.verdict(call("Edit", file_path=SLOT + "/" + rel)), rel)
            self.assertTrue(H.verdict(call("Write", file_path=(SLOT + "/" + rel).replace("/", "\\"))), rel)

    def test_other_files_in_a_slot_are_allowed(self):
        for rel in ("src/game/ShipClass.cpp", "tools/packet.py", "cards/0x401000.jsonl", "build/obj/game/Foo.obj",
                    "config/symbols.txt", "docs/levers/x87.md"):
            self.assertIsNone(H.verdict(call("Edit", file_path=SLOT + "/" + rel)), rel)

    def test_the_main_checkout_is_not_covered(self):
        self.assertFalse(H.verdict(call("Edit", cwd=MAIN, file_path=MAIN + "/tools/decomp.py")))
        self.assertFalse(H.verdict(call("Write", cwd=MAIN, file_path=MAIN + "/config/baseline.json")))


class ShellCommands(unittest.TestCase):
    def test_running_the_verifier_is_allowed(self):
        for cmd in ("python tools/decomp.py match game/ShipClass 2>&1 | tail -5",
                    "python tools/decomp.py diff 0x5A7550 > build/d.txt",
                    "python tools/merge_lane.py check",
                    "cd C:/Recomp/stbc-wt/slot2 && python tools/decomp.py build -s",
                    "git diff tools/decomp.py",
                    "cat config/baseline.json | head"):
            self.assertIsNone(H.verdict(call("Bash", command=cmd)), cmd)

    def test_writing_a_protected_path_is_refused(self):
        for cmd in ("echo {} > config/baseline.json",
                    "cd C:/Recomp/stbc-wt/slot2 && sed -i 's/a/b/' tools/rules.py",
                    "cp /tmp/x.py tools/decomp.py",
                    "git checkout HEAD~1 -- tools/merge_lane.py",
                    "rm -rf build/gate",
                    "python tools/decomp.py report --update-baseline"):
            self.assertTrue(H.verdict(call("Bash", command=cmd)), cmd)

    def test_powershell_writes_are_refused(self):
        cmd = "Set-Content -Path C:\\Recomp\\stbc-wt\\slot2\\config\\baseline.json -Value x"
        self.assertTrue(H.verdict(call("PowerShell", cwd=MAIN, command=cmd)))

    def test_commands_outside_a_slot_are_allowed(self):
        self.assertIsNone(H.verdict(call("Bash", cwd=MAIN, command="python tools/decomp.py report --update-baseline")))
        self.assertIsNone(H.verdict(call("Bash", cwd=MAIN, command="sed -i 's/a/b/' tools/rules.py")))


if __name__ == "__main__":
    unittest.main()
