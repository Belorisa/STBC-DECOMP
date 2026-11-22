"""Run the portable MSVC 6.0 SP5 toolchain in tools/vc6 (same setup as tools/vc6env.bat)."""
import os
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
VC6 = ROOT / "tools" / "vc6"


def env():
    e = dict(os.environ)
    vc = VC6 / "VC98"
    e["PATH"] = os.pathsep.join([str(vc / "BIN"), str(VC6 / "COMMON" / "MSDEV98" / "BIN"), e.get("PATH", "")])
    e["INCLUDE"] = os.pathsep.join(str(vc / p) for p in ("INCLUDE", "MFC/INCLUDE", "ATL/INCLUDE"))
    e["LIB"] = os.pathsep.join(str(vc / p) for p in ("LIB", "MFC/LIB"))
    return e


COMPILERS = {
    None: VC6 / "VC98" / "BIN",          # SP5: cl 12.00.8804, c1/c1xx/c2 build 8966
    "rtm": VC6 / "_rtm_backup" / "BIN",  # RTM: 12.00.8168
    "sp4": VC6 / "_sp4" / "BIN",         # SP4: cl 12.00.8804, c1/c1xx 8867, c2 (Enterprise msvcep.dll) 8799
    "pp": VC6 / "_pp" / "BIN",           # SP5 with the Processor Pack back end: c2 13.00.9044 (vcpp.dll from SP5's cabs)
}


def run(tool, args, cwd=None, compiler=None):
    """Run a VC6 tool (from the given compiler's BIN); return (exit code, combined output)."""
    exe = COMPILERS[compiler] / (tool.upper() + ".EXE")
    p = subprocess.run([str(exe), *args], cwd=cwd, env=env(), capture_output=True, text=True, errors="replace")
    return p.returncode, p.stdout + p.stderr
