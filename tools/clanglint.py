"""Clang as a linter over the compiled units: the type and declaration mistakes a byte match can't see.

    python tools/clanglint.py                         # every compiled unit, a summary by warning
    python tools/clanglint.py game/ShipClass          # one or more units, every finding
    python tools/clanglint.py --module src/netimmerse --list
    python tools/clanglint.py --json build/clanglint.json

A report only: it never edits src/ and its findings decide nothing (the byte gate does). VC6 stays the only compiler
of the bytes; clang-cl (LLVM, `C:/Program Files/LLVM/bin`, or --clang) only parses each unit with the unit's own
include paths, defines and forced includes (decomp.py's cl_args), `_MSC_VER` 1200 and VC6's headers, and reports:

    -Wreturn-type               a function declared to return a value that falls off its end (often a `void`
                                that should return what its last call leaves in EAX, or the reverse)
    -Wint-to-pointer-cast       an integer member or local used as a pointer: a type modelled too narrow or wrong
    -Wpointer-to-int-cast
    -Wreorder-ctor              a constructor's initialiser list out of member order
    -Wdelete-non-virtual-dtor   (reported as -Wdelete-non-abstract-non-virtual-dtor) delete through a base whose destructor isn't virtual although the class has virtuals
    -Woverloaded-virtual        a member that hides a base virtual of the same name: often an override whose
                                signature (const, a parameter type) differs from the base's, so it isn't one
    -Wsign-compare              with --sign only: signed against unsigned, a member's signedness in question

Only findings in src/ are reported, and an error there (a construct clang rejects) is listed too: most are VC6's
dialect (a temporary bound to a non-const reference, a string literal assigned to `char *`), a few a wrong type.
VC6's `wchar_t` is a typedef of `unsigned short` (`/Zc:wchar_t-`). VC6's for-scope (a `for`-init variable used after
its loop) is not an option clang has: such an "undeclared identifier" is left out when the same file declares the
name in a `for`-init above the line. By default the units of modules with a pristine upstream text (Python, the CRT,
the C++ runtime, GameSpy: `"upstream"` in config/project.json) are left out; `--all` includes them.

VC6's C++ library headers predate the standard, and clang rejects a few of their lines. The tool writes patched copies
of those headers under build/clanglint/include/ (searched before VC98\\INCLUDE) and never touches the originals:

    - a template's default arguments repeated after <iosfwd> declared them: dropped from the repeat;
    - an explicit specialisation written without `template<>`: the prefix added;
    - `flags() & unitbuf` and `flags() & skipws` inside basic_ostream/basic_istream, which two-phase lookup binds to
      the manipulator functions: qualified as ios_base members;
    - vector<bool>::const_iterator's converting constructor from an iterator declared after it: removed.
"""

import argparse
import concurrent.futures
import json
import os
import re
import subprocess
import sys
from collections import Counter, defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import decomp  # noqa: E402

ROOT = decomp.ROOT
VC98_INCLUDE = ROOT / "tools" / "vc6" / "VC98" / "INCLUDE"
SHIM = ROOT / "build" / "clanglint" / "include"
CLANG = Path("C:/Program Files/LLVM/bin/clang-cl.exe")
WARNINGS = ["-Wreturn-type", "-Wint-to-pointer-cast", "-Wpointer-to-int-cast", "-Wreorder-ctor",
            "-Wdelete-non-virtual-dtor", "-Woverloaded-virtual"]
DIAG = re.compile(r"^(?P<file>.+?)\((?P<line>\d+),(?P<col>\d+)\)\s*:\s*(?P<kind>error|warning):\s*(?P<msg>.*?)"
                  r"(?:\s*\[(?P<flag>-W[\w-]+)\])?$")

# --- the patched VC6 headers ------------------------------------------------------------------------------------

TEMPLATE = re.compile(r"template<(?P<params>(?:[^<>]|<[^<>]*>)*)>\s*(?:class|struct)\s+(?:_CRTIMP\s+)?(?P<name>\w+)")
DEFAULT = re.compile(r"\s*=\s*(?:[^,<>]|<[^<>]*>)+")
SPECIALISATION = re.compile(r"^(?P<lead>[ \t]*)(?P<decl>class\s+(?:_CRTIMP\s+)?\w+<[^<>;]*(?:<[^<>]*>[^<>;]*)*>\s*[:{])",
                            re.M)


def forward_declared(iosfwd):
    """The templates <iosfwd> declares with default arguments."""
    return {m.group("name") for m in TEMPLATE.finditer(iosfwd) if "=" in m.group("params")}


def patch_header(name, text, fwd):
    """The header's text with the lines clang rejects rewritten, and whether anything changed."""
    out = text
    if name.lower() != "iosfwd":
        def strip(m):
            if m.group("name") not in fwd or "=" not in m.group("params"):
                return m.group(0)
            return m.group(0).replace(m.group("params"), DEFAULT.sub("", m.group("params")))
        out = TEMPLATE.sub(strip, out)

    def specialise(m):
        """Prefix `template<>` unless the declaration's previous non-blank text already ends with a template head."""
        if re.search(r"template\s*<[^<>]*>\s*$", m.string[:m.start()]):
            return m.group(0)
        return m.group("lead") + "template<> " + m.group("decl")

    out = SPECIALISATION.sub(specialise, out)
    out = re.sub(r"flags\(\) & unitbuf\b", "flags() & ios_base::unitbuf", out)
    out = re.sub(r"flags\(\) & skipws\b", "flags() & ios_base::skipws", out)
    out = re.sub(r"\n[ \t]*const_iterator\(const iterator& _X\)\n[ \t]*: _Off\(_X\._Off\), _Ptr\(_X\._Ptr\) \{\}", "",
                 out)
    return out, out != text


def write_shim():
    """Patched copies of the VC6 headers clang rejects, under build/clanglint/include/; their names."""
    SHIM.mkdir(parents=True, exist_ok=True)
    iosfwd = (VC98_INCLUDE / "IOSFWD").read_text(encoding="latin-1")
    fwd = forward_declared(iosfwd)
    written = []
    for p in sorted(VC98_INCLUDE.iterdir()):
        if not p.is_file():
            continue
        text = p.read_text(encoding="latin-1")
        new, changed = patch_header(p.name, text, fwd)
        if changed:
            (SHIM / p.name.lower()).write_text(new, encoding="latin-1", newline="")
            written.append(p.name.lower())
    return written


# --- one unit ----------------------------------------------------------------------------------------------------

def clang_args(unit, path, sign=False):
    """clang-cl arguments for a unit: cl_args' include paths, defines, forced includes, packing and language."""
    keep = []
    for a in decomp.cl_args(unit, path, pch=False):
        if a.startswith(("/D", "/FI", "/Zp", "/Tp", "/Tc")) or a in ("/I", "/MT", "/GR") or keep[-1:] == ["/I"] \
                or a == str(path):
            keep.append(a)
        elif a == "/GX":
            keep.append("/EHsc")
    return ["--target=i386-pc-windows-msvc", "-fms-compatibility-version=12.00", "/Zs", "/X", "/Zc:wchar_t-",
            "/imsvc", str(SHIM), "/imsvc", str(VC98_INCLUDE), "-ferror-limit=0", "-fno-color-diagnostics",
            "-Wno-everything", *WARNINGS, *(["-Wsign-compare"] if sign else []), *keep]


UNDECLARED = re.compile(r"use of undeclared identifier '(\w+)'")


def for_scope(rel, line, msg, texts):
    """Whether an "undeclared identifier" is VC6's for-scope: the file declares the name in a for-init above."""
    m = UNDECLARED.match(msg)
    if not m:
        return False
    if rel not in texts:
        texts[rel] = (ROOT / rel).read_text(encoding="latin-1").splitlines()
    pat = re.compile(r"\bfor\s*\(\s*[\w:<>,\s\*&]*?\b%s\s*[=;]" % re.escape(m.group(1)))
    return any(pat.search(l) for l in texts[rel][:line])


def lint_unit(clang, unit, path, sign=False):
    """(unit, findings): each a dict of file (relative to the root), line, kind, flag and message, in src/ only."""
    proc = subprocess.run([str(clang), *clang_args(unit, path, sign)], capture_output=True, text=True,
                          encoding="latin-1", cwd=ROOT)
    found, seen, texts = [], set(), {}
    for line in (proc.stdout + proc.stderr).splitlines():
        m = DIAG.match(line.strip())
        if not m:
            continue
        f = Path(m.group("file"))
        f = f if f.is_absolute() else ROOT / f
        try:
            rel = f.resolve().relative_to(ROOT).as_posix()
        except ValueError:
            continue
        if not rel.startswith("src/"):
            continue
        key = (rel, m.group("line"), m.group("msg"))
        if key in seen or (m.group("kind") == "error" and for_scope(rel, int(m.group("line")), m.group("msg"), texts)):
            continue
        seen.add(key)
        found.append({"file": rel, "line": int(m.group("line")), "kind": m.group("kind"),
                      "flag": m.group("flag") or ("error" if m.group("kind") == "error" else "other"),
                      "message": m.group("msg")})
    return unit, found


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("units", nargs="*", help="units (game/ShipClass); default every compiled unit")
    ap.add_argument("--module", help="only the units under this source directory (src/netimmerse)")
    ap.add_argument("--list", action="store_true", help="every finding, not only the summary")
    ap.add_argument("--flag", help="only findings of this warning (-Woverloaded-virtual) or 'error'")
    ap.add_argument("--sign", action="store_true", help="add -Wsign-compare")
    ap.add_argument("--all", action="store_true", help="include the modules with a pristine upstream text")
    ap.add_argument("--json", help="write every finding to this file")
    ap.add_argument("--clang", default=str(CLANG), help="clang-cl.exe")
    ap.add_argument("--jobs", type=int, default=os.cpu_count())
    args = ap.parse_args()
    clang = Path(args.clang)
    if not clang.exists():
        sys.exit("no clang-cl at %s (install LLVM, or pass --clang)" % clang)
    if not VC98_INCLUDE.is_dir():
        sys.exit("no VC6 headers at %s" % VC98_INCLUDE)
    write_shim()

    units = decomp.units_to_compile(decomp.source_files())
    if args.units:
        missing = [u for u in args.units if u not in units]
        if missing:
            sys.exit("not a compiled unit: %s" % ", ".join(missing))
        units = {u: units[u] for u in args.units}
    if not args.units and not args.all and not args.module:
        units = {u: p for u, p in units.items()
                 if not decomp.unit_config(p.relative_to(ROOT).as_posix()).get("upstream")}
    if args.module:
        prefix = args.module.rstrip("/") + "/"
        units = {u: p for u, p in units.items() if p.relative_to(ROOT).as_posix().startswith(prefix)}

    results = {}
    with concurrent.futures.ThreadPoolExecutor(args.jobs) as pool:
        for unit, found in pool.map(lambda up: lint_unit(clang, up[0], up[1], args.sign), units.items()):
            results[unit] = found

    # A header's finding appears once per unit that includes it: keep it once, under the first unit.
    unique, owner = {}, {}
    for unit in sorted(results):
        for f in results[unit]:
            key = (f["file"], f["line"], f["message"])
            if key not in unique:
                unique[key] = f
                owner[key] = unit
    findings = [dict(unique[k], unit=owner[k]) for k in sorted(unique)]
    if args.flag:
        findings = [f for f in findings if f["flag"] == args.flag]

    if args.json:
        Path(args.json).write_text(json.dumps(findings, indent=1), encoding="utf-8")
    if args.list or args.units:
        for f in findings:
            print("%s:%d: %s [%s] %s" % (f["file"], f["line"], f["kind"], f["flag"], f["message"]))
    by_flag = Counter(f["flag"] for f in findings)
    by_module = defaultdict(Counter)
    for f in findings:
        by_module[f["flag"]]["/".join(f["file"].split("/")[:2])] += 1
    print("clanglint: %d units, %d findings in src/ (each header finding once)" % (len(units), len(findings)))
    for flag, n in by_flag.most_common():
        print("  %-28s %5d   %s" % (flag, n, ", ".join("%s %d" % kv for kv in by_module[flag].most_common())))


if __name__ == "__main__":
    main()
