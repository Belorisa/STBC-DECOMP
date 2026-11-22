"""stbc-decomp project tool.

    python tools/decomp.py build              compile every unit, compare every annotated function
    python tools/decomp.py match <unit>...    compile and compare the given units only
    python tools/decomp.py diff <function>... compiled vs retail disassembly (name or 0xADDRESS; several, --list, --summary)
    python tools/decomp.py build -s           the same, printing only the changes against config/baseline.json
    python tools/decomp.py report             summary from the last build; --update-baseline writes config/baseline.json
    python tools/decomp.py regress            last build vs config/baseline.json: lost matches (any function) and
                                              edited functions whose score fell; --accept ADDR for adjudicated losses
    python tools/decomp.py gate               clean rebuild, byte gate and rule check; prints the banked line
    python tools/decomp.py rules [<unit>...]  rule checker (tools/rules.py)
    python tools/decomp.py copies [--all]     data read at more than one retail address (one datum, one address)

A unit is a source path under src/ without its extension, e.g. python/Objects/listobject.
"""
import argparse
import atexit
import bisect
import concurrent.futures
import copy
import difflib
import hashlib
import json
import os
import pickle
import re
import shutil
import struct
import subprocess
import sys
import threading
import time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
if __name__ == "__main__":
    sys.modules["decomp"] = sys.modules["__main__"]   # tools that `import decomp` share this run's memos
import coff  # noqa: E402
import pe  # noqa: E402
import vc6  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
SRC = ROOT / "src"
CONFIG = ROOT / "config"
BUILD = ROOT / "build"
TOOLS = ROOT / "tools"

ANNOTATION = re.compile(r"^\s*//\s*(FUNCTION|DATA):\s*(\w+)\s+0x([0-9A-Fa-f]+)(?:\s+(?!@)(\S+))?(?:\s+@(\S+))?\s*$")
PARKED = re.compile(r"^\s*//\s*PARKED:\s*(.*)$")
STATIC_INIT = re.compile(r"^_\$E\d+$")   # a compiler-generated static initialiser, counted apart
DRAFT = re.compile(r"^\s*//\s*DRAFT:\s*(.*)$")
# A function binary-patched in retail: the source is the original, the note says what retail's bytes show instead.
# Listed apart, counted neither matched nor open, never spliced by the gate (AGENTS.md, Policies).
PATCHED = re.compile(r"^\s*//\s*PATCHED:\s*(.*)$")


def load_json(path):
    with open(path, encoding="utf-8") as f:
        return json.load(f)


PROJECT = load_json(CONFIG / "project.json")


# ---------------------------------------------------------------------------
# units and annotations

class Entry:
    """One annotated function or datum in the source."""

    def __init__(self, kind, addr, unit, path, line, name, decorated, lines, end, parked):
        self.kind = kind
        self.addr = addr
        self.unit = unit
        self.path = path
        self.line = line            # 1-based line of the annotation
        self.name = name            # source spelling, e.g. PyList_GetItem or ShipClass::GetAlertLevel
        self.decorated = decorated  # explicit decorated name from the annotation, or None
        self.lines = lines          # the file's lines, shared by its entries
        self.end = end              # 0-based last line of the definition
        self.parked = parked
        self._text = None

    @property
    def text(self):
        """Source text of the definition, annotation included (for hashing and the rule checker). Joined when first
        asked for: a one-unit match reads every file's annotations and few of their texts."""
        if self._text is None:
            self._text = "\n".join(self.lines[self.line - 1:self.end + 1])
        return self._text


def unit_config(rel):
    """Compile settings for a source path relative to the repository root: the longest matching prefix."""
    best = None
    for u in PROJECT["units"]:
        if rel.startswith(u["path"] + "/") and (best is None or len(u["path"]) > len(best["path"])):
            best = u
    return best


def source_files():
    out = {}
    for p in sorted(SRC.rglob("*")):
        if p.suffix.lower() in (".c", ".cpp"):
            rel = p.relative_to(ROOT).as_posix()
            if unit_config(rel) is not None:
                out[p.relative_to(SRC).with_suffix("").as_posix()] = p
    return out


_text_memo = {}   # path -> ((mtime, size), text)


def source_text(path):
    """A source file's text (latin-1), read once per run and again only when the file changed on disk: one build
    asks for each file several times (annotations, includers, the rule checker)."""
    st = os.stat(path)
    stamp = (st.st_mtime_ns, st.st_size)
    hit = _text_memo.get(str(path))
    if hit is None or hit[0] != stamp:
        hit = _text_memo[str(path)] = (stamp, Path(path).read_text(encoding="latin-1"))
    return hit[1]


def includers(all_units):
    """unit -> the units whose source #includes that unit's .c/.cpp file (by file name)."""
    by_name = {}
    for u, p in all_units.items():
        by_name.setdefault(p.name.lower(), []).append(u)
    out = {}
    pat = re.compile(r'^\s*#\s*include\s+[<"]([^>"]+\.(?:c|cpp))[>"]', re.M | re.I)
    for u, p in all_units.items():
        for inc in pat.findall(source_text(p)):
            for target in by_name.get(Path(inc).name.lower(), []):
                if target != u:
                    out.setdefault(target, set()).add(u)
    return out


def units_to_compile(all_units):
    """Units with annotations, plus the units that #include an annotated .c file."""
    annotated = {u for u, p in all_units.items() if scan(u, p)}
    out = set(annotated)
    for target, users in includers(all_units).items():
        if target in annotated:
            out |= users
    return {u: all_units[u] for u in sorted(out)}


def header_entries():
    """Annotations in headers. Each binds to the compiled unit whose object defines the symbol.

    The C++ library's standard headers (<xstring>, <fstream>, ...) have no extension."""
    out = []
    for p in sorted(SRC.rglob("*")):
        if p.suffix.lower() in (".h", ".hpp", ".inl") or (p.suffix == "" and p.is_file()):
            out += scan(None, p)
    return out


def definition_name(lines, i, kind):
    """The name defined by the declaration that starts at or after line i."""
    buf = ""
    for j in range(i, min(i + 12, len(lines))):
        s = lines[j].split("//")[0].strip()
        if not s or s.startswith("/*") or s.startswith("*"):
            if buf:
                continue
            continue
        if not buf and kind == "FUNCTION" and re.match(r"(class|struct)\s+\w+\s*(:[^;(*&]*)?\{?$", s):
            # a compiler-generated member (an implicit destructor) annotated above its class
            # definition: the annotation's decorated name identifies it
            return None, j
        buf += " " + s
        if kind == "FUNCTION" and "(" in buf:
            head = buf[:buf.index("(")].rstrip()
            m = re.search(r"((?:[A-Za-z_]\w*::)*(?:~?[A-Za-z_]\w*|operator\s*\S+))$", head)
            return (m.group(1).replace(" ", "") if m else None), j
        if kind == "DATA" and re.search(r"[=;\[]", buf):
            # "int x = 1;", "char s[4];", "DL_IMPORT(int) x;"; failing those, the direct-initialised
            # "const NiColor NiColor::BLACK( 0, 0, 0 );"
            name_at_end = r"((?:[A-Za-z_]\w*::)*[A-Za-z_]\w*)$"
            head = re.split(r"[=;\[]", buf)[0].rstrip()
            m = re.search(name_at_end, head)
            if not m:
                m = re.search(name_at_end, re.split(r"[=;\[(]", buf)[0].rstrip())
            return (m.group(1) if m else None), j
    return None, i


def definition_end(lines, j):
    """Last line of a function definition: the first column-0 closing brace at or after line j."""
    for k in range(j, len(lines)):
        if lines[k].startswith("}"):
            return k
    return len(lines) - 1


_scan_memo = {}   # (unit, path) -> (the text scanned, its entries)


def scan(unit, path):
    """The annotations of one file. The entries are fresh copies at every call: load_units rebinds their unit."""
    text = source_text(path)
    hit = _scan_memo.get((unit, str(path)))
    if hit is None or hit[0] is not text:
        hit = _scan_memo[(unit, str(path))] = (text, scan_cached(unit, path, text))
    return [copy.copy(e) for e in hit[1]]


_scan_disk = None     # build/scan.pkl: (unit, path) -> (stamp, [entry fields]) from earlier runs
_scan_dirty = False
SCAN_CACHE = BUILD / "scan.pkl"
ENTRY_FIELDS = ("kind", "addr", "unit", "line", "name", "decorated", "end", "parked", "bind", "draft", "patched")


def scan_version():
    """What a cached scan depends on besides the file: this module's own stamp (scan_text lives here) and the
    annotation module name."""
    return (file_stamp(__file__), file_stamp(ROOT / "config" / "project.json"), PROJECT["module"])


def scan_cached(unit, path, text):
    """scan_text, kept across runs in build/scan.pkl while the file's text (by hash, never by timestamp: a rebase
    or checkout can keep both) and this module are unchanged: reading every unit's annotations was a quarter of a
    one-function diff."""
    global _scan_disk, _scan_dirty
    if _scan_disk is None:
        _scan_disk = {}
        try:
            with open(SCAN_CACHE, "rb") as f:
                version, data = pickle.load(f)
            if version == scan_version():
                _scan_disk = data
        except Exception:
            pass
    key = (unit, str(path))
    stamp = hashlib.blake2b(text.encode("utf-8", "surrogateescape"), digest_size=16).digest()
    hit = _scan_disk.get(key)
    if hit is not None and hit[0] == stamp:
        lines = text.splitlines()
        out = []
        for fields in hit[1]:
            e = Entry(fields[0], fields[1], fields[2], path, fields[3], fields[4], fields[5], lines, fields[6],
                      fields[7])
            e.bind, e.draft, e.patched = fields[8:11]
            out.append(e)
        return out
    out = scan_text(unit, path, text)
    _scan_disk[key] = (stamp, [tuple(getattr(e, f) for f in ENTRY_FIELDS) for e in out])
    _scan_dirty = True
    return out


def save_scan_cache():
    """Write build/scan.pkl when this run scanned a file afresh (atomically: other tools may read it)."""
    global _scan_dirty
    if not _scan_dirty:
        return
    _scan_dirty = False
    try:
        BUILD.mkdir(exist_ok=True)
        tmp = BUILD / ("scan.pkl.%d" % os.getpid())
        with open(tmp, "wb") as f:
            pickle.dump((scan_version(), _scan_disk), f, 4)
        os.replace(tmp, SCAN_CACHE)
    except OSError:
        pass


def scan_text(unit, path, text):
    lines = text.splitlines()
    module = PROJECT["module"]
    entries = []
    for i, line in enumerate(lines):
        m = ANNOTATION.match(line) if module in line else None
        if not m or m.group(2) != module:
            continue
        kind, addr, decorated, bind = m.group(1), int(m.group(3), 16), m.group(4), m.group(5)
        k = i + 1
        parked = draft = patched = None
        while k < len(lines) and lines[k].strip().startswith("//"):
            pm = PARKED.match(lines[k]) if "PARKED:" in lines[k] else None
            if pm:
                parked = pm.group(1).strip()
            dm = DRAFT.match(lines[k]) if "DRAFT:" in lines[k] else None
            if dm:
                draft = dm.group(1).strip()
            xm = PATCHED.match(lines[k]) if "PATCHED:" in lines[k] else None
            if xm:
                patched = xm.group(1).strip() or "(no note)"
            k += 1
        if k < len(lines) and not lines[k].strip():
            name, j = None, k   # an annotation must sit directly above its definition
        else:
            name, j = definition_name(lines, k, kind)
        end = definition_end(lines, j) if kind == "FUNCTION" else j
        entries.append(Entry(kind, addr, unit, path, i + 1, name, decorated, lines, end, parked))
        entries[-1].bind = bind
        entries[-1].draft = draft
        entries[-1].patched = patched
    return entries


# ---------------------------------------------------------------------------
# compiling

PCH_DIR = BUILD / "pch"


def unit_auto_pch(unit, cfg):
    """True when a unit builds with VC6's automatic precompiled header (/YX): its module's "auto_pch" switch, or the
    unit listed in the module's "auto_pch_units" (a per-unit /YX, kept only where both the unit's matches and its
    retail section order show the original built it with a PCH: user's ruling, 2026-11-20)."""
    return bool(cfg.get("auto_pch", False)) or unit in cfg.get("auto_pch_units", [])


def auto_pch(path):
    """True when the source file builds with /YX (unit_auto_pch)."""
    rel = path.relative_to(ROOT).as_posix()
    unit = path.relative_to(SRC).with_suffix("").as_posix() if rel.startswith("src/") else rel
    return unit_auto_pch(unit, unit_config(rel))


def cl_args(unit, path, pch=None):
    """The compiler arguments for a unit, its source last (after /Tp or /Tc). Under the module's "auto_pch" switch
    they carry /YX and /Fp<pch>: pch is the .pch file the compile creates, by default build/pch/<unit>.pch (the
    path the fingerprint sees); a caller compiling elsewhere passes its own, so parallel compiles never share one,
    and calls fresh_pch before compiling. pch=False leaves both out (cl /E)."""
    cfg = unit_config(path.relative_to(ROOT).as_posix())
    args = ["/nologo", *cfg["flags"]]
    for inc in cfg.get("include", []):
        args += ["/I", str(ROOT / inc)]
    for d in cfg.get("defines", []):
        args.append("/D" + d)
    for o in cfg.get("overrides", {}).get(unit, []):
        if o.startswith("/I") and len(o) > 2:
            o = "/I" + str(ROOT / o[2:])   # an include override is relative to the repository root
        args.append(o)
    if pch is not False and unit_auto_pch(unit, cfg):
        args += ["/YX", "/Fp" + str(pch if pch is not None else PCH_DIR / (unit + ".pch"))]
    return args + ["/Tp" if path.suffix.lower() == ".cpp" else "/Tc", str(path)]


def fresh_pch(args):
    """Remove the .pch an argument list names (/Fp) and make its folder, so /YX creates it from this compile's own
    headers: a .pch left by another compile (other flags, older headers) is never used. Returns the path or None."""
    for a in args:
        if a.startswith("/Fp") and len(a) > 3:
            pch = Path(a[3:])
            pch.parent.mkdir(parents=True, exist_ok=True)
            try:
                pch.unlink()
            except FileNotFoundError:
                pass
            return pch
    return None


def drop_pch(pch):
    """Delete a .pch after its compile: it is a by-product (a few MB per unit), never read again."""
    if pch is not None:
        try:
            pch.unlink()
        except OSError:
            pass


def compiler_of(path):
    """The compiler a source file is built with: its unit's "unit_compiler" entry, else its module's "compiler",
    else None (SP5)."""
    cfg = unit_config(path.relative_to(ROOT).as_posix())
    unit = path.relative_to(SRC).with_suffix("").as_posix()
    return cfg.get("unit_compiler", {}).get(unit, cfg.get("compiler"))


LINE_DIRECTIVE = re.compile(r'#line\s+(\d+)\s+"(.*)"')


def preprocessed_lines(path, cache=True):
    """Line numbers of path that survive preprocessing with its unit's flags (None if cl /E fails). With cache, the
    answer for the same fingerprint (compiler, arguments, the file and every project header it reaches) is reused
    from build/objcache."""
    unit = path.relative_to(SRC).with_suffix("").as_posix()
    cached = OBJ_CACHE / (unit_fingerprint(unit, path) + ".lines") if cache else None
    if cached is not None and cached.exists():
        try:
            lines = set(json.loads(cached.read_text()))
            os.utime(cached)   # the cache prunes by last use
            return lines
        except ValueError:
            pass   # a truncated file: preprocess again
    rc, out = vc6.run("cl", ["/E", *cl_args(unit, path, pch=False)], compiler=compiler_of(path))
    if rc != 0:
        return None
    lines, current, n = set(), None, 0
    want = os.path.normcase(str(path))
    for l in out.splitlines():
        m = LINE_DIRECTIVE.match(l) if l.startswith("#") else None
        if m:
            n = int(m.group(1))
            current = os.path.normcase(m.group(2).replace("\\\\", "\\"))
            continue
        if current == want and l.strip():
            lines.add(n)
        n += 1
    if cached is not None:
        OBJ_CACHE.mkdir(parents=True, exist_ok=True)
        tmp = cached.with_suffix(".tmp%d" % os.getpid())
        tmp.write_text(json.dumps(sorted(lines)))
        os.replace(tmp, cached)
    return lines


INCLUDE_LINE = re.compile(r'^[ \t]*#[ \t]*include[ \t]*([<"])([^>"\r\n]+)[>"]', re.M)
OBJ_CACHE = BUILD / "objcache"
_include_memo = {}   # (file, include dirs) -> (content digest, resolved project includes)
_include_disk = None  # the same from earlier runs: (file, include dirs) -> (stamps, digest, includes)
_include_dirty = False
_include_lock = threading.Lock()
INCLUDE_CACHE = BUILD / "includes.pkl"


def file_stamp(path):
    st = os.stat(path)
    return st.st_mtime_ns, st.st_size


def include_disk():
    """build/includes.pkl: each file's digest and includes as an earlier run found them. An entry holds while the
    file keeps its timestamp and size and no file under src/ was added, removed or renamed (src_listing)."""
    global _include_disk
    with _include_lock:
        if _include_disk is None:
            try:
                with open(INCLUDE_CACHE, "rb") as f:
                    _include_disk = pickle.load(f)
            except Exception:
                _include_disk = {}
            atexit.register(save_include_disk)
    return _include_disk


def save_include_disk():
    if _include_dirty:
        BUILD.mkdir(exist_ok=True)
        tmp = INCLUDE_CACHE.with_suffix(".tmp%d" % os.getpid())
        with open(tmp, "wb") as f:
            pickle.dump(_include_disk, f, pickle.HIGHEST_PROTOCOL)
        os.replace(tmp, INCLUDE_CACHE)


def project_includes(path, inc_dirs):
    """(sha1 of path's bytes, the project files it #includes). Every #include counts, inside #if or not: a
    superset of the real dependencies, so a cached object is never reused after one of them changed. Files outside
    the repository and the VC6 toolchain's own headers never change and aren't followed."""
    global _include_dirty
    key = (str(path), inc_dirs)
    if key not in _include_memo:
        stamp = file_stamp(path)
        old = include_disk().get(key)
        if old is not None and old[0] == stamp and old[1] == src_listing():
            _include_memo[key] = old[2:]
            return _include_memo[key]
        data = Path(path).read_bytes()
        found = []
        for kind, name in INCLUDE_LINE.findall(data.decode("latin-1")):
            dirs = ((Path(path).parent,) if kind == '"' else ()) + tuple(Path(d) for d in inc_dirs)
            for d in dirs:
                f = d / name
                if f.is_file():
                    if ROOT in f.resolve().parents and TOOLS not in f.resolve().parents:
                        found.append(f.resolve())
                    break
        _include_memo[key] = (hashlib.sha1(data).hexdigest(), found)
        _include_disk[key] = (stamp, src_listing(), *_include_memo[key])
        _include_dirty = True
    return _include_memo[key]


_listing = None


def src_listing():
    """A digest of the names of every file under src/: an #include resolves to the first directory that holds the
    name, so a file added or removed anywhere can change what a remembered include resolved to."""
    global _listing
    if _listing is None:
        h = hashlib.sha1()
        for root, dirs, files in os.walk(SRC):
            dirs.sort()
            h.update(root.encode("utf-8", "replace"))
            for f in sorted(files):
                h.update(f.encode("utf-8", "replace"))
        _listing = h.hexdigest()
    return _listing


def unit_fingerprint(unit, path):
    """A digest of everything that decides a unit's object: the compiler, its arguments, the source and every
    project header it reaches (forced includes included). A /YX unit's arguments carry /YX and its default .pch path;
    the .pch itself needs no digest, because every compile creates it afresh from the headers digested here."""
    args = cl_args(unit, path)
    inc_dirs = tuple(args[i + 1] for i, a in enumerate(args) if a == "/I") + tuple(
        a[2:] for a in args if a.startswith("/I") and len(a) > 2)
    roots = [Path(path)]
    for forced in (a[3:] for a in args if a.startswith("/FI")):
        hit = next((d / forced for d in (Path(path).parent,) + tuple(Path(x) for x in inc_dirs)
                    if (d / forced).is_file()), None)
        if hit is not None:
            roots.append(hit)
    h = hashlib.sha1(repr((compiler_of(path), args)).encode())
    seen, todo = set(), [r.resolve() for r in roots]
    while todo:
        f = todo.pop()
        if f in seen:
            continue
        seen.add(f)
        digest, deps = project_includes(f, inc_dirs)
        h.update(("%s %s\n" % (f.relative_to(ROOT).as_posix(), digest)).encode())
        todo.extend(deps)
    return h.hexdigest()


def compile_unit(unit, path, outdir, cache=True):
    obj = outdir / (unit + ".obj")
    obj.parent.mkdir(parents=True, exist_ok=True)
    cached = OBJ_CACHE / (unit_fingerprint(unit, path) + ".obj") if cache else None
    if cached is not None and cached.exists():
        want = os.stat(cached)
        # copy2 keeps the timestamp: an object in place with the cached one's timestamp and size is that object
        if not obj.exists() or file_stamp(obj) != (want.st_mtime_ns, want.st_size):
            shutil.copy2(cached, obj)
        os.utime(cached, ns=(time.time_ns(), want.st_mtime_ns))   # the cache prunes by last use (access time)
        return unit, obj, ""
    if obj.exists():
        obj.unlink()
    # a /YX unit's .pch sits beside its object (build/obj, build/gate/obj): one file per unit and output folder
    args = ["/c", *cl_args(unit, path, pch=obj.with_suffix(".pch")), "/Fo" + str(obj)]
    pch = fresh_pch(args)
    rc, out = vc6.run("cl", args, cwd=str(obj.parent), compiler=compiler_of(path))
    drop_pch(pch)
    if rc != 0 or not obj.exists():
        return unit, None, out
    if cached is not None:
        OBJ_CACHE.mkdir(parents=True, exist_ok=True)
        tmp = cached.with_suffix(".tmp%d" % os.getpid())
        shutil.copy2(obj, tmp)
        os.replace(tmp, cached)
    return unit, obj, out


def compile_all(units, outdir, jobs=None, cache=True):
    """Compile units into outdir. With cache, an object whose fingerprint (compiler, arguments, source and project
    headers) was compiled before is reused from build/objcache; the gate compiles everything afresh."""
    results = {}
    with concurrent.futures.ThreadPoolExecutor(max_workers=jobs or os.cpu_count()) as ex:
        for unit, obj, out in ex.map(lambda u: compile_unit(u, units[u], outdir, cache), sorted(units)):
            results[unit] = (obj, out)
    if cache:
        prune_obj_cache()
    return results


def prune_obj_cache():
    """Drop cache objects unused for 14 days. The directory holds tens of thousands of files and stat-ing each one
    took seconds on every compile, so the sweep runs at most once a day (the stamp file's own time)."""
    stamp = OBJ_CACHE / ".pruned"
    now = time.time()
    try:
        if now - stamp.stat().st_mtime < 86400:
            return
    except OSError:
        pass
    if not OBJ_CACHE.exists():
        return
    old = now - 14 * 86400
    for f in OBJ_CACHE.iterdir():
        if f.suffix in (".obj", ".lines"):
            st = f.stat()
            if max(st.st_atime, st.st_mtime) < old:
                f.unlink()
    stamp.touch()


# ---------------------------------------------------------------------------
# decorated names

def qualified(decorated):
    """Source-level qualified name of an MSVC decorated name, or None when it can't be read simply."""
    n = decorated
    if not n.startswith("?"):
        if n[:1] in ("_", "@"):
            n = n[1:]
        return n.split("@")[0]
    if n.startswith("??0") or n.startswith("??1"):
        scope = n[3:].split("@@")[0].split("@")
        if any(s.startswith("?$") for s in scope):
            return None
        cls = scope[0]
        name = cls if n[2] == "0" else "~" + cls
        return "::".join(reversed(scope)) + "::" + name
    if n.startswith("??") or n.startswith("?$"):
        return None
    parts = n[1:].split("@@")[0].split("@")
    if any(p.startswith("?$") or not p for p in parts):
        return None
    return "::".join(reversed(parts))


DELETING = {"??_G": "`scalar deleting destructor'", "??_E": "`vector deleting destructor'"}


def display_name(name, decorated):
    """The name a report row shows. The source spelling, unless the annotation has none (a compiler-generated
    member annotated above its class, a definition the annotation names only by its decorated name) or the parser
    took a macro for it (`FUNC1(cmath_acos, ...)`, `NiImplementRegister( NiNode )`): then the compiled symbol's
    qualified name, or the decorated name when that can't be read simply (templates, operators)."""
    if not decorated:
        return name
    if STATIC_INIT.match(decorated):
        return decorated
    q = qualified(decorated)
    if q is None and decorated[:4] in DELETING and "?$" not in decorated:
        q = "::".join(reversed(decorated[4:].split("@@")[0].split("@"))) + "::" + DELETING[decorated[:4]]
    if name is None:
        return q or decorated
    if q is not None and q.split("::")[-1] != name.split("::")[-1] and not name.split("::")[-1].startswith("operator"):
        return q
    return name


def symbol_index(obj):
    """(decorated name -> symbols, qualified source name -> symbols) for an object's external and static symbols,
    in symbol-table order; built once per object."""
    index = getattr(obj, "_index", None)
    if index is None:
        by_name, by_qualified = {}, {}
        for s in obj.symbols.values():
            if s.storage not in (coff.SC_EXTERNAL, coff.SC_STATIC):
                continue
            by_name.setdefault(s.name, []).append(s)
            if not s.name.startswith(("$", ".")):
                by_qualified.setdefault(qualified(s.name), []).append(s)
        index = obj._index = (by_name, by_qualified)
    return index


def find_symbol(obj, entry):
    if entry.name is None and not entry.decorated:
        return None, "annotation is not directly above a definition"
    want_fn = entry.kind == "FUNCTION"
    by_name, by_qualified = symbol_index(obj)

    def eligible(s):
        common = s.section == 0 and s.value > 0 and s.storage == coff.SC_EXTERNAL
        return s.defined or (common and not want_fn)

    if entry.decorated:
        s = next((s for s in by_name.get(entry.decorated, ()) if eligible(s)), None)
        return (s, None) if s is not None else (None, "no compiled symbol for %s" % entry.decorated)
    cands = []
    for s in by_qualified.get(entry.name, ()):
        if not eligible(s):
            continue
        is_fn = s.is_function or (s.defined and obj.section(s.section).is_code and s.storage == coff.SC_EXTERNAL)
        if want_fn == is_fn:
            cands.append(s)
    if len(cands) == 1:
        return cands[0], None
    if not cands:
        return None, "no compiled symbol for %s" % (entry.decorated or entry.name)
    return None, "ambiguous %s: add the decorated name to the annotation (%s)" % (
        entry.name, ", ".join(c.name for c in cands))


# ---------------------------------------------------------------------------
# comparing

_coff_memo = {}   # object path -> (stamp, Coff)


def load_coff(path):
    """An object file, parsed once per run and again only when it changed on disk."""
    stamp = file_stamp(path)
    hit = _coff_memo.get(str(path))
    if hit is None or hit[0] != stamp:
        hit = _coff_memo[str(path)] = (stamp, coff.Coff(Path(path).read_bytes()))
    return hit[1]


class Unit:
    def __init__(self, name, obj, entries):
        self.name = name
        self.obj = obj
        self.entries = entries
        self.bases = {}       # section number -> retail address of its offset 0
        self.symbols = {}     # entry address -> Symbol
        self.bss = {}         # (section, offset) -> retail address of one uninitialised datum


class Resolver:
    """Resolves relocation targets to retail addresses and verifies referents."""

    def __init__(self, image, units, extra):
        self.img = image
        self.units = units
        self.globals = dict(extra)   # decorated external name -> address
        self.statics = {}            # (unit, decorated name) -> address
        self.errors = []
        self.data_errors = []
        self.comdat_verified = {}    # (decorated name, address) -> another unit's copy verified there
        self.comdat_index = {}       # unit name -> {decorated name: defined COMDAT symbol}
        self.comdat_owners = None    # decorated name -> the units that define it as a COMDAT, in unit order
        self.starts = None           # every annotated or listed address, sorted (next_start)
        self.placed_verified = {}    # (unit, section, name, addend) -> problem of a datum placed by its section
        self.verified = {}           # (unit, section, name, addend, base, depth) -> verify's result, failures included
        self.copies = {}             # decorated name -> {unit: address}, for a COMDAT annotated at several addresses
        self.copy_choice = {}        # (unit, decorated name) -> the copy an unannotated reference of the unit took
        self.journal = None          # while resolving sites that can't place referents: the placements to undo
        self.agree = {}              # (unit, function address) -> agreeing_sites, computed by collect_reads
        self.canon = {}              # COMDAT data name -> its one retail address (decide_copies)
        self.misread = {}            # COMDAT data name -> why no read of it is right (decide_copies)
        self.foreign = {}            # (unit, section, offset) -> why the unit-private datum's address is another's
        self.copy_conflicts = []
        annotated = {}               # decorated external name -> {unit: address}
        comdat = {}                  # decorated external name -> every annotated symbol is a COMDAT
        for u in units.values():
            for e in u.entries:
                s = u.symbols.get(e.addr)
                if s is None:
                    continue
                if s.storage == coff.SC_STATIC:
                    self.statics[(u.name, s.name)] = e.addr
                else:
                    per = annotated.setdefault(s.name, {})
                    if per.get(u.name, e.addr) != e.addr:
                        self.errors.append("%s annotated twice in %s, at 0x%X and 0x%X" % (
                            s.name, u.name, per[u.name], e.addr))
                    per.setdefault(u.name, e.addr)
                    comdat[s.name] = comdat.get(s.name, True) and s.defined and bool(
                        u.obj.section(s.section).flags & coff.SCN_LNK_COMDAT)
                if s.defined and u.obj.section(s.section).is_bss:
                    u.bss[(s.section, s.value)] = e.addr
                elif s.defined:
                    self.set_base(u, s.section, e.addr - s.value)
        for name, per in annotated.items():
            addrs = sorted(set(per.values()))
            listed = self.globals.get(name)
            if len(addrs) == 1:
                if listed is not None and listed != addrs[0]:
                    self.errors.append("%s annotated at 0x%X and listed at 0x%X" % (name, addrs[0], listed))
                self.globals[name] = addrs[0]
                continue
            # Retail keeps more than one body of an inline or template function when the linker didn't fold them:
            # each unit's annotation names its own copy, and that unit's references resolve to it.
            if not comdat[name]:
                self.errors.append("%s annotated at %s: only an inline or template (COMDAT) copy may have several "
                                   "addresses" % (name, ", ".join("0x%X" % a for a in addrs)))
                self.globals[name] = addrs[0]
                continue
            if listed is not None and listed not in addrs:
                self.errors.append("%s annotated at %s and listed at 0x%X" % (
                    name, ", ".join("0x%X" % a for a in addrs), listed))
            self.globals.pop(name, None)
            self.copies[name] = per

    def retail_copy(self, unit, name):
        """The one annotated copy of a COMDAT kept twice that retail's direct calls and jumps from unit's annotated
        functions reach, or None (no such call, or calls to more than one copy)."""
        if getattr(self, "_copy_callers", None) is None:
            self._copy_callers = self._scan_copy_callers()
        return self._copy_callers.get((unit, name))

    def _scan_copy_callers(self):
        targets = {a: name for name, per in self.copies.items() for a in per.values()}
        if not targets or getattr(self.img, "text", None) is None or getattr(self.img, "data", None) is None:
            return {}   # no retail image to scan (a test's stub image)
        _, va, vsize, rawptr, rawsize = self.img.text
        code = self.img.data[rawptr:rawptr + min(vsize, rawsize)]
        import numpy as np
        b = np.frombuffer(code, dtype=np.uint8)
        idx = np.nonzero((b[:-4] == 0xE8) | (b[:-4] == 0xE9))[0]
        rel = (b[idx + 1].astype(np.int64) | (b[idx + 2].astype(np.int64) << 8) | (b[idx + 3].astype(np.int64) << 16)
               | (b[idx + 4].astype(np.int64) << 24))
        rel = np.where(rel >= 1 << 31, rel - (1 << 32), rel)
        dest = va + idx + 5 + rel
        want = np.array(sorted(targets), dtype=np.int64)
        hit = np.isin(dest, want)
        starts = sorted((e.addr, u.name) for u in self.units.values() for e in u.entries)
        keys = [s[0] for s in starts]
        seen = {}
        for site, target in zip((va + idx[hit]).tolist(), dest[hit].tolist()):
            k = bisect.bisect_right(keys, site) - 1
            if k < 0:
                continue
            key = (starts[k][1], targets[target])
            seen.setdefault(key, set()).add(target)
        return {key: next(iter(found)) for key, found in seen.items() if len(found) == 1}

    def next_start(self, addr):
        """The next annotated or listed address above addr, or None: where the function at addr ends at the latest."""
        if self.starts is None:
            self.starts = sorted({e.addr for u in self.units.values() for e in u.entries} | set(self.globals.values())
                                 | {a for per in self.copies.values() for a in per.values()})
        k = bisect.bisect_right(self.starts, addr)
        return self.starts[k] if k < len(self.starts) else None

    def set_base(self, u, section, base):
        old = u.bases.get(section)
        if old is not None and old != base:
            return False
        self._put(u.bases, section, base)
        return True

    # Placements are decided only by sites whose code agrees with retail up to the site (relocate's `trusted`). A
    # site that doesn't (a function misaligned against retail reads a garbage "retail address" at ours' offset)
    # still resolves, so its own function compares as before, but every placement and verification it makes is
    # journaled and undone when the function's relocation ends: it can't fix where a referent lives for others.
    def _put(self, d, k, v):
        if self.journal is not None:
            self.journal.append((d, k, d.get(k, _MISSING)))
        d[k] = v

    def _drop(self, d, k):
        if self.journal is not None:
            self.journal.append((d, k, d[k]))
        del d[k]

    def _rollback(self):
        while self.journal:
            d, k, old = self.journal.pop()
            if old is _MISSING:
                d.pop(k, None)
            else:
                d[k] = old

    def known(self, u, sym):
        """Address of a symbol from annotations, the symbol list, or its section's known placement."""
        if sym.storage == coff.SC_STATIC and (u.name, sym.name) in self.statics:
            return self.statics[(u.name, sym.name)]
        if sym.storage in (coff.SC_EXTERNAL, coff.SC_WEAK_EXTERNAL) and sym.name in self.globals:
            return self.globals[sym.name]   # a weak external (a vtable's ??_E) listed by name resolves like any other
        if sym.storage in (coff.SC_EXTERNAL, coff.SC_WEAK_EXTERNAL) and u.name in self.copies.get(sym.name, ()):
            return self.copies[sym.name][u.name]   # the unit's own annotated copy of a COMDAT kept twice
        if sym.defined and sym.section in u.bases and not u.obj.section(sym.section).is_bss:
            return u.bases[sym.section] + sym.value
        return None

    def named(self, u, sym):
        """True when an annotation or a claim gives the external sym an address in unit u."""
        return sym.name in self.globals or u.name in self.copies.get(sym.name, ())

    def place_bss(self, u, sym, hint, addend):
        """Uninitialised data has no content to verify, and a unit's .bss also holds statics the source may not
        model yet, so each datum is placed on its own: by its DATA annotation, else at retail's address. The
        same datum must land on one address at every use, and two data never share one."""
        section_symbol = sym.name == u.obj.section(sym.section).name
        offset = sym.value + (addend if section_symbol else 0)
        shift = addend if section_symbol else 0
        key = (sym.section, offset)
        placed = u.bss.get(key)
        if placed is not None:
            if hint is not None and placed - shift != hint:
                return None, "referent %s placed at 0x%X, retail uses 0x%X here" % (sym.name, placed, hint + shift)
            return placed - shift, None
        if hint is None:
            return None, "no retail address for %s" % sym.name
        datum = hint + shift
        for other, addr in u.bss.items():
            if addr == datum and other != key:
                return None, "referent %s: retail 0x%X already holds another datum of this unit" % (sym.name, datum)
        self._put(u.bss, key, datum)
        return hint, None

    def resolve(self, u, sym, hint, addend, depth=0):
        """(address, problem) for relocation target sym; hint is the address retail uses there."""
        if sym.storage == coff.SC_WEAK_EXTERNAL and not self.named(u, sym) and sym.weak_default is not None:
            sym = u.obj.symbols[sym.weak_default]   # e.g. a vtable's ??_E falls back to the ??_G this object defines
            if not sym.defined:   # the default may be named by an undefined record; prefer this object's definition
                sym = next((s for s in u.obj.symbols.values() if s.name == sym.name and s.defined), sym)
        if sym.defined and u.obj.section(sym.section).is_bss and not (
                sym.storage == coff.SC_EXTERNAL and self.named(u, sym)):
            return self.place_bss(u, sym, hint, addend)
        if sym.defined and hint is not None and (self.canon or self.misread or self.foreign):
            hint, problem = self.copy_check(u, sym, hint, addend)
            if problem:
                return None, problem
        addr = self.known(u, sym)
        if addr is not None:
            if self.placed_by_section(u, sym):
                # Another datum fixed this section's placement; this one still has to prove its content.
                problem = self.verify_placed(u, sym, addend, depth)
                if problem:
                    return None, problem
            return addr, None
        if (sym.name in self.copies and u.name not in self.copies[sym.name]
                and hint not in self.copies[sym.name].values()):
            # A COMDAT kept twice, referenced from a unit without an annotated copy at a site whose retail dword is
            # not one of the copies (a site misaligned against retail reads part of another instruction): the unit
            # takes the copy retail names, the same one at every use, so take the one its other sites or retail's
            # own calls from the unit's functions name. Exactness is unaffected: retail's bytes at this site name
            # no copy, so the compare still differs here.
            choice = self.copy_choice.get((u.name, sym.name)) or self.retail_copy(u.name, sym.name)
            if choice is not None:
                return choice, None
        if not sym.defined and sym.name in self.copies and hint in self.copies[sym.name].values():
            # a reference to a COMDAT kept twice from a unit without an annotated copy: retail's operand names one
            # of the annotated copies, and every use in the unit must name the same one
            if (u.name, sym.name) not in self.copy_choice:
                self._put(self.copy_choice, (u.name, sym.name), hint)
            chosen = self.copy_choice[(u.name, sym.name)]
            if chosen != hint:
                return None, "referent %s: retail uses its copies at 0x%X and 0x%X in one unit" % (
                    sym.name, chosen, hint)
            return hint, None
        if not sym.defined:
            return None, "unknown referent %s (retail %s)" % (sym.name, "?" if hint is None else "0x%X" % hint)
        if hint is None:
            return None, "no retail address for %s" % sym.name
        # A unit-local or compiler-generated referent: take retail's address and prove the content.
        base = hint - sym.value
        if not self.set_base(u, sym.section, base):
            return None, "referent %s: retail 0x%X disagrees with its section's placement" % (sym.name, hint)
        problem = self.verify(u, sym, addend, depth)
        if problem:
            self._drop(u.bases, sym.section)
            # An inline or template COMDAT: the linker kept one unit's copy, which may come from a module with other
            # flags. Accept retail's address only when some unit's copy of the same name verifies there.
            if self.comdat_elsewhere(u, sym, hint, addend, depth):
                return hint, None
            return None, problem
        return hint, None

    def copy_check(self, u, sym, hint, addend):
        """(hint, problem) for a site reading initialised data, against decide_copies: a COMDAT name has one
        address, and a unit-private datum can't sit where another datum does. A site that can't place referents (the
        journal is open: its instruction isn't retail's, so its address is noise) takes the name's copy instead."""
        sec = u.obj.section(sym.section)
        if sec.is_code or sec.is_bss:
            return hint, None
        if sym.storage == coff.SC_EXTERNAL and sec.flags & coff.SCN_LNK_COMDAT:
            if self.named(u, sym):
                return hint, None
            canon = self.canon.get(sym.name)
            if canon is None:
                problem = self.misread.get(sym.name)
                return hint, (None if self.journal is not None else problem)
            if hint == canon:
                return hint, None
            if self.journal is not None:
                return canon, None
            return hint, "referent %s: retail reads 0x%X here, its copy is at 0x%X" % (sym.name, hint, canon)
        if self.journal is not None:
            return hint, None
        selector = addend if sym.name == sec.name else 0
        return hint, self.foreign.get((u.name, sym.section, sym.value + selector))

    def placed_by_section(self, u, sym):
        """True when sym's address comes only from its data section's placement, not from an annotation or a claim."""
        if sym.storage == coff.SC_STATIC and (u.name, sym.name) in self.statics:
            return False
        if sym.storage in (coff.SC_EXTERNAL, coff.SC_WEAK_EXTERNAL) and self.named(u, sym):
            return False
        sec = u.obj.section(sym.section)
        return sym.defined and not sec.is_bss and not sec.is_code

    def verify_placed(self, u, sym, addend, depth):
        key = (u.name, sym.section, sym.name, addend if sym.name == u.obj.section(sym.section).name else 0)
        if key not in self.placed_verified:
            self._put(self.placed_verified, key, None)    # guards recursion through data that refer to each other
            self._put(self.placed_verified, key, self.verify(u, sym, addend, depth))
        return self.placed_verified[key]

    def comdat_symbols(self, u):
        """Name -> the first defined COMDAT symbol of a unit's object, indexed once per unit."""
        index = self.comdat_index.get(u.name)
        if index is None:
            index = {}
            for s in u.obj.symbols.values():
                if s.defined and s.name not in index and u.obj.section(s.section).flags & coff.SCN_LNK_COMDAT:
                    index[s.name] = s
            self.comdat_index[u.name] = index
        return index

    def comdat_elsewhere(self, u, sym, hint, addend, depth):
        if sym.storage != coff.SC_EXTERNAL or not u.obj.section(sym.section).flags & coff.SCN_LNK_COMDAT:
            return False
        key = (sym.name, hint)
        if key in self.comdat_verified:
            return self.comdat_verified[key]
        self._put(self.comdat_verified, key, False)    # guards recursion through mutually referring copies
        if self.comdat_owners is None:
            self.comdat_owners = {}
            for o in self.units.values():
                if o.obj is not None:
                    for name in self.comdat_symbols(o):
                        self.comdat_owners.setdefault(name, []).append(o)
        for o in self.comdat_owners.get(sym.name, ()):
            if o is u:
                continue
            other = self.comdat_symbols(o)[sym.name]
            if not self.set_base(o, other.section, hint - other.value):
                continue
            if self.verify(o, other, addend, depth) is None:
                self._put(self.comdat_verified, key, True)
                return True
            self._drop(o.bases, other.section)
        return False

    def verify(self, u, sym, addend, depth):
        """None when sym's bytes, placed at its section's base, equal retail's; else the problem. Remembered per
        placement: a referent many functions share (AppWrap's swig_const_table strings) was re-verified thousands of
        times, most of a build's comparison time."""
        # The addend selects the datum only for a section symbol; any other symbol is checked over its own extent.
        selector = addend if sym.name == u.obj.section(sym.section).name else 0
        key = (u.name, sym.section, sym.name, selector, u.bases.get(sym.section), depth)
        if key in self.verified:
            return self.verified[key]
        problem = self._verify(u, sym, addend, depth)
        self._put(self.verified, key, problem)
        return problem

    def _verify(self, u, sym, addend, depth):
        obj = u.obj
        sec = obj.section(sym.section)
        if sec.is_bss:
            return None
        if depth > 8:
            return None
        if sym.name == sec.name:     # section symbol: the addend selects the datum
            start = addend
            end = sec.size
            for s in obj.symbols.values():
                if s.section == sym.section and start < s.value < end and s.name != sec.name:
                    end = s.value
        else:
            start, end = obj.extent(sym)
        base = u.bases[sym.section]
        data, problems = self.relocate(u, sec, start, end, base + start, depth + 1)
        if problems:
            return "referent %s: %s" % (sym.name, problems[0])
        retail = self.img.read(base + start, end - start)
        if retail == bytes(data):
            return None
        trimmed = bytes(data).rstrip(b"\0")
        if retail is not None and retail[:len(trimmed)] == trimmed and not any(retail[len(trimmed):len(trimmed) + 1]):
            return None
        return "referent %s at 0x%X differs from retail" % (sym.name, base + start)

    def relocate(self, u, sec, start, end, addr, depth=0, unresolved=None, trusted=None):
        """Section bytes [start, end) placed at addr with relocations resolved; (bytes, problems). `unresolved`, a
        dict, receives {site address: symbol name} for the relocations that didn't resolve: the bytes there are
        retail's, not a target. `trusted(offset)`, when given, says which sites may place a referent (see _put);
        the others are resolved after them, and what they placed is undone before this returns."""
        data = bytearray(sec.data[start:end])
        problems = []
        sites = [r for r in sec.relocs if start <= r[0] < end]
        if trusted is not None:
            sites.sort(key=lambda r: not trusted(r[0]))     # stable: the trusted sites first, in their order
        own_journal = False
        try:
            for off, symidx, typ in sites:
                if trusted is not None and self.journal is None and not trusted(off):
                    self.journal, own_journal = [], True
                self._relocate_site(u, off, symidx, typ, start, addr, depth, data, problems, unresolved)
        finally:
            if own_journal:
                self._rollback()
                self.journal = None
        return data, problems

    def _relocate_site(self, u, off, symidx, typ, start, addr, depth, data, problems, unresolved):
        """One relocation of relocate(): resolve its target and patch data in place."""
        site = addr + off - start
        i = off - start
        addend = struct.unpack_from("<i", data, i)[0]
        raw = self.img.dword(site)
        sym = u.obj.symbols[symidx]
        if typ == coff.IMAGE_REL_I386_DIR32:
            hint = None if raw is None else (raw - addend) & 0xFFFFFFFF
        elif typ == coff.IMAGE_REL_I386_REL32:
            hint = None if raw is None else (site + 4 + struct.unpack("<i", struct.pack("<I", raw))[0] - addend) & 0xFFFFFFFF
        else:
            problems.append("unsupported relocation type %d" % typ)
            return
        target, problem = self.resolve(u, sym, hint, addend, depth)
        if problem:
            problems.append(problem)
            if unresolved is not None:
                unresolved[site] = sym.name
            if raw is not None:
                struct.pack_into("<I", data, i, raw)   # keep retail's value so the rest still compares
            return
        if typ == coff.IMAGE_REL_I386_DIR32:
            value = (target + addend) & 0xFFFFFFFF
        else:
            value = (target + addend - (site + 4)) & 0xFFFFFFFF
        struct.pack_into("<I", data, i, value)


_MISSING = object()


def agreeing_sites(code, relocs, start, retail):
    """The relocation offsets (section offsets, as in relocs) whose instruction is retail's own: the same bytes at
    the same place, the relocation field aside. There, the address retail holds is the referent's; elsewhere (a
    function misaligned against retail) it can be anything (Resolver.relocate's `trusted`). A function whose code
    agrees with retail's everywhere but the fields trusts every site without disassembling."""
    if retail is None:
        return set()
    fields = set()
    for off, _, _ in relocs:
        fields.update(range(off - start, off - start + 4))
    n = min(len(code), len(retail))
    first = next((i for i in range(n) if code[i] != retail[i] and i not in fields), n)
    if first == n:
        return {off for off, _, _ in relocs}
    ours = {a: size for a, _, _, size in disasm_sized(code, 0)}
    theirs = {a: size for a, _, _, size in disasm_sized(retail, 0)}
    out = set()
    for off, _, _ in relocs:
        i = off - start
        if i < first:
            out.add(off)
            continue
        head = next((a for a in range(i, max(-1, i - 15), -1) if a in ours and a + ours[a] > i), None)
        if head is None or theirs.get(head) != ours[head] or head + ours[head] > n:
            continue
        if all(code[k] == retail[k] or k in fields for k in range(head, head + ours[head])):
            out.add(off)
    return out


# ---------------------------------------------------------------------------
# One datum, one address, across the tree (AGENTS.md "Referents").
#
# A compiler-generated constant (`__real@4@...`, a string literal's `??_C@`) is a COMDAT: the linker keeps one copy per
# name, so the name has one retail address. Within a unit the section placement holds it to one address, but each unit
# used to place its own copy by content, so a function reading another copy of an equal value (a C1 literal where
# retail's site reads C2's folded reciprocal, a file static, a `.data` global) verified and showed MATCH. Before the
# comparison, every site whose instruction is retail's own (agreeing_sites) votes the address it reads for the datum
# its relocation names; a name's copy is the address most functions read it at, and a unit-private datum may not sit
# where an annotated datum or another name's copy does. A site reading a datum elsewhere is a wrong referent.

def _site_hint(img, site, typ, addend):
    raw = img.dword(site)
    if raw is None:
        return None
    if typ == coff.IMAGE_REL_I386_DIR32:
        return (raw - addend) & 0xFFFFFFFF
    if typ == coff.IMAGE_REL_I386_REL32:
        return (site + 4 + struct.unpack("<i", struct.pack("<I", raw))[0] - addend) & 0xFFFFFFFF
    return None


def data_reads(img, u, e, agree):
    """The initialised-data reads of one annotated function at the sites retail's own instructions hold:
    [(symbol name, comdat, section, datum offset, retail address of the datum)]. A COMDAT external is identified by
    its name; any other datum by its unit, section and offset (its retail address includes the addend for a section
    symbol, which selects the datum)."""
    sym = u.symbols.get(e.addr)
    if sym is None or not sym.defined:
        return []
    sec = u.obj.section(sym.section)
    if not sec.is_code:
        return []
    start, end = u.obj.function_extent(sym)
    out = []
    for off, symidx, typ in sec.relocs:
        if not start <= off < end or off not in agree:
            continue
        s = u.obj.symbols[symidx]
        if not s.defined or s.section <= 0:
            continue
        tsec = u.obj.section(s.section)
        if tsec.is_code or tsec.is_bss:
            continue
        addend = struct.unpack_from("<i", sec.data, off)[0]
        hint = _site_hint(img, e.addr + off - start, typ, addend)
        if hint is None:
            continue
        comdat = s.storage == coff.SC_EXTERNAL and bool(tsec.flags & coff.SCN_LNK_COMDAT)
        selector = addend if s.name == tsec.name else 0
        out.append((s.name, comdat, s.section, s.value + selector, (hint + selector) & 0xFFFFFFFF))
    return out


READS_CACHE_VERSION = 1


def collect_reads(res, objs, cache=True):
    """{(unit, function address): data_reads(...)} for every annotated function of every loaded unit. Cached per unit
    in build/referent_reads.pkl by its object's stamp and its functions' annotations, so `match` reads the other units'
    votes without disassembling them; the gate passes cache=False. The agreeing sites computed here are kept in
    res.agree for compare_bytes."""
    path = BUILD / "referent_reads.pkl"
    store = {}
    if cache and path.exists():
        try:
            with open(path, "rb") as f:
                store = pickle.load(f)
            if store.get("version") != READS_CACHE_VERSION:
                store = {}
        except Exception:
            store = {}
    units_store = store.setdefault("units", {})
    store["version"] = READS_CACHE_VERSION
    out, dirty = {}, False
    for u in res.units.values():
        if u.obj is None:
            continue
        p = objs.get(u.name)
        try:
            st = os.stat(p) if p is not None else None
        except OSError:
            st = None
        fns = tuple(sorted((e.addr, u.symbols[e.addr].name) for e in u.entries
                           if e.kind == "FUNCTION" and e.addr in u.symbols))
        stamp = None if st is None else (str(p), st.st_mtime_ns, st.st_size, fns)
        hit = units_store.get(u.name) if cache else None
        if hit is not None and stamp is not None and hit[0] == stamp:
            for addr, reads in hit[1].items():
                out[(u.name, addr)] = reads
            continue
        mine = {}
        for e in u.entries:
            if e.kind != "FUNCTION" or e.addr not in u.symbols:
                continue
            sym = u.symbols[e.addr]
            if not sym.defined or not u.obj.section(sym.section).is_code:
                continue
            sec = u.obj.section(sym.section)
            start, end = u.obj.function_extent(sym)
            relocs = [r for r in sec.relocs if start <= r[0] < end]
            agree = agreeing_sites(sec.data[start:end], relocs, start, res.img.read(e.addr, end - start))
            res.agree[(u.name, e.addr)] = agree
            reads = data_reads(res.img, u, e, agree)
            if reads:
                mine[e.addr] = reads
                out[(u.name, e.addr)] = reads
        if stamp is not None:
            units_store[u.name] = (stamp, mine)
            dirty = True
    if cache and dirty:
        try:
            BUILD.mkdir(exist_ok=True)
            tmp = path.with_suffix(".tmp%d" % os.getpid())
            with open(tmp, "wb") as f:
                pickle.dump(store, f, protocol=pickle.HIGHEST_PROTOCOL)
            os.replace(tmp, path)
        except OSError:
            pass
    return out


def decide_copies(res, reads):
    """Fix, from the votes of collect_reads, which retail address each datum read by name has, and which reads are
    wrong. Sets res.canon {COMDAT name: address}, res.misread {COMDAT name: problem text} for a name whose every
    candidate address another datum owns, and res.foreign {(unit, section, offset): problem text} for a unit-private
    datum placed where another datum lives. Returns the conflicts, [(datum, {address: [function addresses]}, chosen
    address or None)], for the report."""
    owners = {}                            # retail address -> the annotated datum or claim there
    annotated = set()                      # (unit, section, offset) of each annotated datum, however it is reached
    for u in res.units.values():
        for e in u.entries:
            if e.kind == "DATA":
                owners.setdefault(e.addr, e.name)
                sym = u.symbols.get(e.addr)
                if sym is not None and sym.defined:
                    annotated.add((u.name, sym.section, sym.value))
    for name, addr in res.globals.items():
        owners.setdefault(addr, name)
    named = set(res.globals) | set(res.copies)
    by_name = {}                           # COMDAT name -> {address: {(unit, function)}}
    local = {}                             # (unit, section, offset) -> {address: {(unit, function)}}
    names = {}                             # (unit, section, offset) -> its symbol name
    for (uname, faddr), rs in reads.items():
        for name, comdat, section, offset, addr in rs:
            if comdat:
                if name in named:
                    continue
                by_name.setdefault(name, {}).setdefault(addr, set()).add((uname, faddr))
            else:
                if uname not in res.units:
                    continue
                key = (uname, section, offset)
                if (uname, name) in res.statics or name in named or key in annotated:
                    continue     # annotated: placed by its annotation, which verify_data checks
                local.setdefault(key, {}).setdefault(addr, set()).add((uname, faddr))
                names[key] = name

    def rank(item):
        addr, fns = item
        return (len({f for _, f in fns}), len({un for un, _ in fns}), -addr)

    res.canon, res.misread, res.foreign = {}, {}, {}
    conflicts = []
    taken = {}                             # retail address -> the COMDAT name whose copy it is
    for name in sorted(by_name):
        cands = by_name[name]
        free = [it for it in cands.items() if it[0] not in owners]
        if not free:
            addr = max(cands.items(), key=rank)[0]
            res.misread[name] = "referent %s: retail reads 0x%X here, which holds %s" % (name, addr, owners[addr])
            conflicts.append((name, cands, None))
            continue
        best = sorted(free, key=rank, reverse=True)
        if len(best) > 1 and rank(best[0])[:2] == rank(best[1])[:2]:
            # as many functions read it at two addresses: no majority says which is the name's copy
            res.misread[name] = "referent %s: retail reads it at %s in as many functions; no majority names its " \
                                "copy" % (name, " and ".join("0x%X" % a for a, _ in best[:2]))
            conflicts.append((name, cands, None))
            continue
        res.canon[name] = best[0][0]
        taken[best[0][0]] = name
        if len(cands) > 1:
            conflicts.append((name, cands, best[0][0]))
    at = {}                                # retail address -> the unit-private data read there
    for key, cands in local.items():
        for addr in cands:
            at.setdefault(addr, []).append(key)
    for key, cands in sorted(local.items()):
        for addr in cands:
            other = owners.get(addr) or taken.get(addr)
            if other is None:
                rivals = [k for k in at[addr] if k != key]
                if not rivals:
                    continue
                if any(k[0] == key[0] for k in rivals):
                    continue     # one unit's two placements of a section: its placement check reports it
                other = "%s of %s" % (names[rivals[0]], rivals[0][0])
            res.foreign[key] = "referent %s: retail 0x%X here holds %s" % (names[key], addr, other)
            conflicts.append(("%s (%s)" % (names[key], key[0]), cands, None))
    return conflicts


def prepare_copies(res, objs, cache=True):
    """collect_reads then decide_copies: the tree-wide copy of each datum, before any function is compared."""
    res.copy_conflicts = decide_copies(res, collect_reads(res, objs, cache))
    return res.copy_conflicts


def padding_ok(img, addr, size, align=16):
    """Retail bytes between the function's end and its section's next alignment boundary must be fill."""
    end = addr + size
    pad = (-end) % align
    tail = img.read(end, pad) if pad else b""
    return tail is not None and all(b in (0x90, 0xCC) for b in tail)


_cs = None


def disasm(code, addr):
    global _cs
    if _cs is None:
        import capstone
        _cs = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    return [(i.address, i.mnemonic, i.op_str) for i in _cs.disasm(bytes(code), addr)]


def disasm_sized(code, addr):
    disasm(b"", addr)   # creates the disassembler
    return [(i.address, i.mnemonic, i.op_str, i.size) for i in _cs.disasm(bytes(code), addr)]


def name_unresolved(ins, data, addr, unresolved):
    """`disasm_sized` rows as (address, mnemonic, operands) for the diff, with each operand whose relocation didn't
    resolve (`unresolved`: {site: symbol}) shown as "?<symbol>". Resolver.relocate leaves retail's dword at such a
    site, read at ours' offset: where the two layouts differ it is part of another instruction, not an address."""
    rows = []
    for a, m, o, size in ins:
        for site in sorted(s for s in unresolved if a <= s < a + size):
            name = "?" + unresolved[site]
            if m in ("call", "jmp") and site == a + size - 4:
                o = name
                continue
            value = "0x%x" % struct.unpack_from("<I", data, site - addr)[0]
            o = re.sub(r"(?<![0-9a-fx])%s(?![0-9a-f])" % value, name, o, count=1) if value in o else o + "  ; " + name
        rows.append((a, m, o))
    return rows


def wall_of(res, addr, data, problems, unresolved):
    """(class, note): which compiler decision differs (tools/walls.py). Retail is read up to the next known address,
    since its function may be longer than ours; walls.py cuts both sides at the function's end. `unresolved` names
    the call sites of ours whose target has no address (Resolver.relocate)."""
    import walls
    return walls.classify(*wall_instructions(res, addr, data, unresolved), sorted(set(problems)))


def wall_instructions(res, addr, data, unresolved):
    """(ours, retail) as walls.py reads them: [(mnemonic, operands)], each cut at the function's end."""
    import walls
    stop = res.next_start(addr)
    size = max(len(data), min(stop - addr if stop else 0, 4 * len(data) + 256))
    retail = res.img.read(addr, size) or res.img.read(addr, len(data)) or b""
    return (walls.instructions(disasm_sized, bytes(data), addr, unresolved),
            walls.instructions(disasm_sized, retail, addr))


# ---------------------------------------------------------------------------
# declaration-sensitive functions (config/declaration_sensitive.txt)

FLIP_KINDS = ("frame/register", "schedule")
MEMORY_OPERAND = re.compile(r"\[([^\]]*)\]")


def declaration_sensitive(path=None):
    """address -> (name, allowed kinds) from config/declaration_sensitive.txt: the functions ruled declaration-
    sensitive (AGENTS.md, "Policies"), whose lost match regress accepts when the diff is only an allowed kind."""
    path = Path(path) if path else CONFIG / "declaration_sensitive.txt"
    out = {}
    if not path.exists():
        return out
    for line in path.read_text(encoding="utf-8").splitlines():
        parts = line.split("#")[0].split()
        if len(parts) >= 3:
            kinds = tuple(k for k in parts[2].split(",") if k in FLIP_KINDS)
            out[int(parts[0], 16)] = (parts[1], kinds)
    return out


FRAME_BASE = re.compile(r"\[\s*(esp|ebp)\b")
DISPLACEMENT_AT_END = re.compile(r"\s*[+-]\s*(?:0x[0-9a-f]+|\d+)\s*$")


def frame_slot_free(o, frame_bases=("esp",)):
    """An operand string with the displacement of each frame-slot memory operand left out: those based on esp, or
    on ebp where it is the frame pointer. Other memory operands keep theirs (a member offset, a vtable slot)."""
    def mask(mm):
        inner = mm.group(1)
        base = FRAME_BASE.match("[" + inner)
        if base and base.group(1) in frame_bases:
            return "[%s + F]" % DISPLACEMENT_AT_END.sub("", inner)
        return mm.group(0)
    return MEMORY_OPERAND.sub(mask, o)


def frame_bases(ins):
    """The registers a function's frame slots are addressed through: esp, and ebp when the function sets it up as the
    frame pointer (`mov ebp, esp`); under /O2 ebp is otherwise a general register, and [ebp + N] a member."""
    return ("esp", "ebp") if ("mov", "ebp, esp") in ((m, o) for m, o in ins) else ("esp",)


def frame_free(ins):
    """Instructions with addresses, registers and frame-slot displacements left out: what stays equal when only the
    frame layout or the register picks differ. Displacements from other registers (member offsets, vtable slots)
    are kept, so a member that moved is not a flip."""
    import walls
    bases = frame_bases(ins)
    out = []
    for m, o in ins:
        o = "X" if o.startswith("?") else walls.ADDRESS.sub("X", o)
        o = walls.REGISTERS.sub("R", frame_slot_free(o, bases))
        out.append("%s %s" % (m, o))
    return out


def flip_kind(ours, retail):
    """"frame/register" when two instruction lists differ only in registers and frame offsets, "schedule" when they
    hold the same such instructions in another order, else None."""
    from collections import Counter
    a, b = frame_free(ours), frame_free(retail)
    if a == b:
        return "frame/register"
    if Counter(a) == Counter(b):
        return "schedule"
    return None


def score(code, retail, addr):
    if retail is None:
        return 0.0
    norm = re.compile(r"0x[0-9a-f]{5,8}")
    a = ["%s %s" % (m, norm.sub("X", o)) for _, m, o in disasm(code, addr)]
    b = ["%s %s" % (m, norm.sub("X", o)) for _, m, o in disasm(retail, addr)]
    return round(100.0 * difflib.SequenceMatcher(None, a, b, autojunk=False).ratio(), 1)


def compare_function(res, u, e):
    """One result row for an annotated function. A function under a PATCHED note (retail's bytes are a binary patch
    of the original the source reconstructs) gets status PATCHED, its navigation score kept, and is never spliced."""
    row, data = compare_bytes(res, u, e)
    patched = getattr(e, "patched", None)
    if patched is None or row["status"] in ("NO-OBJ", "NO-SYMBOL"):
        return row, data
    row["patched"] = patched
    if row["status"] == "MATCH":
        row.setdefault("problems", []).append("bytes match retail: the PATCHED note is wrong")
    row["status"] = "PATCHED"
    row.pop("wall", None)
    row.pop("wall_note", None)
    return row, None


def compare_bytes(res, u, e):
    row = {"addr": "0x%X" % e.addr, "name": e.name, "unit": u.name, "line": e.line,
           "src": hashlib.sha1(e.text.encode("latin-1")).hexdigest()[:12]}
    if e.parked:
        row["parked"] = e.parked
    if getattr(e, "draft", None):
        row["draft"] = e.draft
    if e.decorated and STATIC_INIT.match(e.decorated):
        row["static"] = True
    sym = u.symbols.get(e.addr)
    row["name"] = display_name(e.name, sym.name if sym is not None else e.decorated)
    if u.obj is None and e.addr not in u.errors:
        row.update(status="NO-OBJ", score=0.0)
        return row, None
    if sym is None:
        row.update(status="NO-SYMBOL", score=0.0, problems=[u.errors.get(e.addr, "symbol not found")])
        return row, None
    row["symbol"] = sym.name
    sec = u.obj.section(sym.section)
    start, end = u.obj.function_extent(sym)
    unresolved = {}
    relocs = [r for r in sec.relocs if start <= r[0] < end]
    agree = res.agree.get((u.name, e.addr))
    if agree is None:
        agree = agreeing_sites(sec.data[start:end], relocs, start, res.img.read(e.addr, end - start))
    data, problems = res.relocate(u, sec, start, end, e.addr, unresolved=unresolved, trusted=agree.__contains__)
    retail = res.img.read(e.addr, len(data))
    row["size"] = len(data)
    if retail == bytes(data) and padding_ok(res.img, e.addr, len(data), sec.alignment):
        if any(not pending_problem(p) for p in problems):
            row.update(status="WRONG-REFERENT", score=99.9, problems=sorted(set(problems)), wall="referent")
            return row, None
        if problems:
            row.update(status="REFERENT", score=100.0, problems=sorted(set(problems)), wall="referent")
            return row, None
        row.update(status="MATCH", score=100.0)
        return row, bytes(data)
    row.update(status="DIFF", score=min(99.9, score(data, retail, e.addr)))
    if problems:
        row["problems"] = sorted(set(problems))
    row["wall"], row["wall_note"] = wall_of(res, e.addr, data, problems, unresolved)
    if row["wall"] == "register" and e.addr in declaration_sensitive_listed():
        # what regress may accept for a listed function: only registers and frame slots, or the schedule
        row["flip"] = flip_kind(*wall_instructions(res, e.addr, data, unresolved))
    return row, None


_sensitive_memo = []


def declaration_sensitive_listed():
    """declaration_sensitive(), read once per process."""
    if not _sensitive_memo:
        _sensitive_memo.append(declaration_sensitive())
    return _sensitive_memo[0]


def pending_problem(p):
    """A referent whose identity isn't recorded yet: open work, not a wrong answer."""
    return p.split(": ")[-1].startswith("unknown referent")


def verify_data(res, u, e):
    """An annotated datum must hold retail's bytes at its address (uninitialised data can't be checked)."""
    if u.obj is None:
        return None
    sym = u.symbols.get(e.addr)
    if sym is None:
        return u.errors.get(e.addr, "symbol not found")
    if not sym.defined or u.obj.section(sym.section).is_bss:
        return None
    sec = u.obj.section(sym.section)
    start, end = u.obj.extent(sym)
    data, problems = res.relocate(u, sec, start, end, e.addr)
    retail = res.img.read(e.addr, len(data))
    if retail != bytes(data):
        trimmed = bytes(data).rstrip(b"\0")
        if retail is None or retail[:len(trimmed)] != trimmed:
            return "content differs from retail"
    real = [p for p in problems if not pending_problem(p)]
    return real[0] if real else None


def load_units(objs, all_units):
    units = {}
    including = includers(all_units)
    scanned = {}
    for name, path in all_units.items():
        entries = scanned[name] = scan(name, path)
        obj = objs.get(name)
        if not entries and obj is None:
            continue
        u = Unit(name, load_coff(obj) if obj else None, [])
        u.errors = {}
        units[name] = u
    for name, path in all_units.items():
        if name not in units:
            continue
        u = units[name]
        for e in scanned[name]:
            if e.bind and e.bind != name:
                # "@unit": the annotated definition is the one that unit (an includer) links, not this unit's own.
                o = units.get(e.bind)
                if e.bind not in including.get(name, ()) or o is None or o.obj is None:
                    u.entries.append(e)
                    u.errors[e.addr] = "@%s is not a compiled unit that includes this file" % e.bind
                    continue
                s, err = find_symbol(o.obj, e)
                e.unit = o.name
                o.entries.append(e)
                if s is not None:
                    o.symbols[e.addr] = s
                else:
                    o.errors[e.addr] = err
                continue
            s, err = find_symbol(u.obj, e) if u.obj is not None else (None, None)
            if s is None:
                # A .c file included by other units (crt0.c inside wincrt0.c): the includer may define it.
                for other in sorted(including.get(name, ())):
                    o = units.get(other)
                    if o is not None and o.obj is not None:
                        s, _ = find_symbol(o.obj, e)
                        if s is not None:
                            e.unit = other
                            o.entries.append(e)
                            o.symbols[e.addr] = s
                            break
                if s is not None:
                    continue
            u.entries.append(e)
            if s is not None:
                u.symbols[e.addr] = s
            elif err is not None:
                u.errors[e.addr] = err
    # which units' objects name a symbol at all (decorated or qualified), in unit order: find_symbol finds nothing
    # in the others
    by_decorated, by_qualified = {}, {}
    for name in sorted(units):
        if units[name].obj is not None:
            names, quals = symbol_index(units[name].obj)
            for k in names:
                by_decorated.setdefault(k, []).append(name)
            for k in quals:
                by_qualified.setdefault(k, []).append(name)
    for e in header_entries():
        owner = None
        # "@unit" picks the defining unit when several emit the same inline or template instance.
        having = by_decorated.get(e.decorated, ()) if e.decorated else by_qualified.get(e.name, ())
        for name in ([e.bind] if e.bind else having):
            if name not in units:
                continue
            u = units[name]
            if u.obj is not None:
                s, _ = find_symbol(u.obj, e)
                if s is not None:
                    owner = u
                    break
        if owner is None:
            orphan = units.setdefault("(headers)", Unit("(headers)", None, []))
            orphan.errors = getattr(orphan, "errors", {})
            orphan.errors[e.addr] = "no compiled unit defines %s" % (e.decorated or e.name)
            e.unit = orphan.name
            orphan.entries.append(e)
            continue
        e.unit = owner.name
        owner.entries.append(e)
        owner.symbols[e.addr] = s
    return units


def extra_symbols():
    out = {}
    path = CONFIG / "symbols.txt"
    if path.exists():
        for line in path.read_text(encoding="utf-8").splitlines():
            line = line.split("#")[0].strip()
            if line:
                name, addr = line.split()[:2]
                out[name] = int(addr, 16)
    return out


def target_image():
    data = (ROOT / PROJECT["target"]).read_bytes()
    digest = hashlib.sha1(data).hexdigest()
    if digest != PROJECT["sha1"]:
        sys.exit("target %s has SHA-1 %s, contract says %s" % (PROJECT["target"], digest, PROJECT["sha1"]))
    return data


def import_name(symbol):
    """The imported name an `__imp_` symbol stands for: `__imp__GetTickCount@0` -> GetTickCount, `__imp__strlen`
    -> strlen, `__imp_?f@@YAXXZ` -> ?f@@YAXXZ. None for any other symbol."""
    if not symbol.startswith("__imp_"):
        return None
    name = symbol[len("__imp_"):]
    if name.startswith("_"):
        name = re.sub(r"@\d+$", "", name[1:])
    return name


def import_claim_errors(img, symbols):
    """symbols.txt's `__imp_` claims checked against the image's import table: an IAT slot holds one import, and
    the claim must name it. A swapped pair (timeGetTime / GetTickCount) still matches byte for byte."""
    table = img.imports()
    out = []
    for symbol, addr in sorted(symbols.items()):
        want = import_name(symbol)
        if want is None:
            continue
        got = table.get(addr)
        if got is None:
            out.append("symbols.txt %s 0x%X: not an IAT slot" % (symbol, addr))
        elif got[1] != want and import_name("__imp_" + got[1]) != want:   # mss32 exports `_AIL_x@4`
            out.append("symbols.txt %s 0x%X: the IAT slot imports %s!%s" % (symbol, addr, got[0], got[1]))
    return out


def run_compare(objs, all_units, only=None, cache=True):
    img = pe.Image(target_image())
    units = load_units(objs, all_units)
    claims = extra_symbols()
    res = Resolver(img, units, claims)
    res.data_errors.extend(import_claim_errors(img, claims))
    prepare_copies(res, objs, cache)
    rows, matched = [], {}
    for name in sorted(units):
        if only and name not in only:
            continue
        u = units[name]
        for e in u.entries:
            if e.kind == "DATA":
                problem = verify_data(res, u, e)
                if problem:
                    res.data_errors.append("%s 0x%X %s: %s" % (u.name, e.addr, e.name, problem))
                continue
            row, data = compare_function(res, u, e)
            rows.append(row)
            if data is not None:
                matched[e.addr] = data
    return img, units, res, rows, matched


# ---------------------------------------------------------------------------
# rule checker hook

def rule_findings(units, rows, cache=True):
    import rules
    findings = rules.check_units(units, cache)
    for row in rows:
        f = findings.get(int(row["addr"], 16))
        if f:
            row["rules"] = f
    return findings


# Statuses of config/exceptions.md whose routines have no source to write: counted neither matched nor open.
# ORIGINAL-ASM: a CRT routine VC6 assembled from its .asm sources (a row with an annotation is C with an inline __asm
# block, and the report's). IMPORT-THUNK: a 6-byte `jmp [__imp_X]` stub an import library supplies. LIBRARY-DATA: a
# library object's data linked into .text (dinput.lib's DIOBJECTDATAFORMAT tables), its bytes the row's "<N> bytes".
LISTED_APART = ("ORIGINAL-ASM", "IMPORT-THUNK", "LIBRARY-DATA")
THUNK_BYTES = 6


def listed_apart(status, text=None, annotated=()):
    """{address: (name, bytes)} of the routines config/exceptions.md lists with `status` and no annotation. A row's
    byte count is the last "<N> bytes" of its deviation (0 when it gives none; an IMPORT-THUNK is 6 by default)."""
    if text is None:
        path = CONFIG / "exceptions.md"
        text = path.read_text(encoding="utf-8") if path.exists() else ""
    out = {}
    for line in text.splitlines():
        cells = [c.strip() for c in line.strip().strip("|").split("|")]
        if len(cells) < 5 or cells[-1] != status or not re.fullmatch(r"0x[0-9A-Fa-f]+", cells[0]):
            continue
        addr = int(cells[0], 16)
        if addr in annotated:
            continue
        sizes = re.findall(r"(\d+) bytes", cells[3])
        out[addr] = (cells[1].replace("`", ""), int(sizes[-1]) if sizes else
                     THUNK_BYTES if status == "IMPORT-THUNK" else 0)
    return out


def check_import_thunk(img, addr, name, deviation=""):
    """None when retail's bytes at addr are exactly `FF 25 <slot>` with a base relocation on the slot, and the slot is
    the import `name` (decorated: `_inet_addr@4`) names; else what is wrong. An import by ordinal is matched by the
    deviation's "ordinal N"."""
    code = img.read(addr, THUNK_BYTES)
    if code is None or code[:2] != b"\xFF\x25":
        return "0x%X: not a `jmp [slot]` (%s)" % (addr, code.hex(" ") if code else "no bytes")
    slot = int.from_bytes(code[2:], "little")
    if addr + 2 not in img.relocs:
        return "0x%X: no base relocation on the slot operand" % addr
    imp = img.imports().get(slot)
    if imp is None:
        return "0x%X: slot 0x%X is not in the import address table" % (addr, slot)
    dll, imported = imp
    plain = re.sub(r"@\d+$", "", name.lstrip("_")) if name.startswith("_") else name
    if imported.startswith("#"):
        if not re.search(r"\bordinal %s\b" % imported[1:], deviation):
            return "0x%X: slot 0x%X is %s ordinal %s, which the row doesn't name" % (addr, slot, dll, imported[1:])
    elif imported not in (plain, name):
        return "0x%X: slot 0x%X is %s!%s, not %s" % (addr, slot, dll, imported, name)
    return None


def summarize(rows, img, ledger=None):
    def pending(r):
        return any(x["tier"] == "match" for x in r.get("rules", []))
    annotated = {int(r["addr"], 16) for r in rows}
    asm = listed_apart("ORIGINAL-ASM", ledger, annotated)
    thunks = listed_apart("IMPORT-THUNK", ledger, annotated)
    library = listed_apart("LIBRARY-DATA", ledger, annotated)
    every_exact = [r for r in rows if r["status"] == "MATCH"]
    src_bytes = sum(r["size"] for r in every_exact)   # bytes count everything built from source, statics included
    statics = [r for r in rows if r.get("static")]
    patched = [r for r in rows if r["status"] == "PATCHED"]
    # the counts below are written functions; statics and binary-patched functions apart
    rows = [r for r in rows if not r.get("static") and r["status"] != "PATCHED"]
    exact = [r for r in rows if r["status"] == "MATCH"]
    clean = [r for r in exact if not pending(r)]
    text_bytes = img.text[2]
    listed = declaration_sensitive_listed()
    sensitive = [r for r in rows if int(r["addr"], 16) in listed]
    return {
        # the declaration-sensitive functions' state, on a line of its own: it explains the headline's jitter, and
        # the headline counts them as it counts every other function
        "sensitive": len(sensitive),
        "sensitive_matched": len([r for r in sensitive if r["status"] == "MATCH" and not pending(r)]),
        "sensitive_listed": len(listed),
        "statics": len(statics),
        "statics_matched": len([r for r in statics if r["status"] == "MATCH" and not pending(r)]),
        "statics_pending": [r["addr"] for r in statics if r["status"] == "MATCH" and pending(r)],
        "patched": len(patched),
        "patched_bytes": sum(r.get("size", 0) for r in patched),
        "original_asm": len(asm),
        "original_asm_bytes": sum(size for _, size in asm.values()),
        "import_thunks": len(thunks),
        "import_thunk_bytes": sum(size for _, size in thunks.values()),
        "library_data": len(library),
        "library_data_bytes": sum(size for _, size in library.values()),
        "functions": len(rows),
        "matched_rules": len(clean),
        "bytes_matched": len(exact),
        "match_pending": [r["addr"] for r in exact if pending(r)],
        "referent": len([r for r in rows if r["status"] == "REFERENT"]),
        "diff": len([r for r in rows if r["status"] in ("DIFF", "WRONG-REFERENT")]),
        "broken": len([r for r in rows if r["status"] in ("NO-OBJ", "NO-SYMBOL")]),
        "source_bytes": src_bytes,
        "text_bytes": text_bytes,
    }


def print_summary(s, prefix=""):
    print("%smatched under the rules: %d   bytes matched: %d   (MATCH-PENDING %d, referent-pending %d, "
          "diff %d, broken %d, of %d annotated)" % (
              prefix, s["matched_rules"], s["bytes_matched"], len(s["match_pending"]), s["referent"],
              s["diff"], s["broken"], s["functions"]))
    if s.get("statics"):
        print("%scompiler-generated static initialisers (_$E, counted apart): %d of %d matched%s" % (
            prefix, s["statics_matched"], s["statics"],
            ", %d MATCH-PENDING" % len(s["statics_pending"]) if s.get("statics_pending") else ""))
    if s.get("sensitive"):
        print("%sdeclaration-sensitive (listed): %d of %d matched (config/declaration_sensitive.txt; counted in the "
              "headline as usual)" % (prefix, s["sensitive_matched"], s["sensitive"]))
    if s.get("patched"):
        print("%sbinary-patched in retail (PATCHED, written as the original, counted apart): %d functions, %d bytes"
              % (prefix, s["patched"], s["patched_bytes"]))
    if s.get("original_asm"):
        print("%sassembled from VC6's .asm sources (ORIGINAL-ASM, counted apart): %d routines, %d bytes"
              % (prefix, s["original_asm"], s["original_asm_bytes"]))
    if s.get("import_thunks"):
        print("%simport thunks (IMPORT-THUNK, from the import libraries, counted apart): %d stubs, %d bytes"
              % (prefix, s["import_thunks"], s["import_thunk_bytes"]))
    if s.get("library_data"):
        print("%slibrary data in .text (LIBRARY-DATA, from the libraries, counted apart): %d blocks, %d bytes"
              % (prefix, s["library_data"], s["library_data_bytes"]))
    print("%s.text from source: %d of %d bytes (%.3f%%)" % (
        prefix, s["source_bytes"], s["text_bytes"], 100.0 * s["source_bytes"] / s["text_bytes"]))


def print_rows(rows, verbose):
    for r in rows:
        if r["status"] == "MATCH" and not r.get("rules") and not verbose:
            continue
        tag = r["status"]
        if r["status"] == "MATCH" and any(x["tier"] == "match" for x in r.get("rules", [])):
            tag = "MATCH-PENDING"
        print("  %-13s %5.1f  %-10s %s  (%s:%d)" % (tag, r["score"], r["addr"], r["name"], r["unit"], r["line"]))
        if r.get("patched"):
            print("        - retail patched: " + r["patched"][:150])
        for p in r.get("problems", [])[:6]:
            print("        - " + p)
        if r.get("wall_note"):
            print("        - wall: %s (%s)" % (r["wall"], r["wall_note"]))
        for f in r.get("rules", []):
            print("        - %s [%s] %s" % (f["rule"], f["tier"], f["text"]))


def compile_report(results):
    bad = 0
    for unit, (obj, out) in sorted(results.items()):
        if obj is None:
            bad += 1
            print("COMPILE FAILED: %s" % unit)
            print("\n".join(l for l in out.splitlines() if "error" in l.lower())[:4000])
    return bad


def cmd_build(args):
    all_units = source_files()
    units = units_to_compile(all_units)
    objdir = BUILD / "obj"
    results = compile_all(units, objdir, args.jobs)
    broken = compile_report(results)
    objs = {u: o for u, (o, _) in results.items() if o}
    img, uobjs, res, rows, _ = run_compare(objs, all_units)
    rule_findings(uobjs, rows)
    for e in res.errors:
        print("SYMBOL CONFLICT: " + e)
    for e in res.data_errors:
        print("DATA: " + e)
    s = summarize(rows, img)
    write_report(s, rows)
    if args.summary:
        print_changes(rows)
    else:
        print_rows(rows, args.verbose)
    print_summary(s)
    return 1 if broken or res.errors or res.data_errors else 0


def print_changes(rows):
    """What this build changed against config/baseline.json: new and lost matches, new and resolved problems,
    score moves. A worker reads this instead of 9,000 rows."""
    base_path = CONFIG / "baseline.json"
    base = load_json(base_path) if base_path.exists() else {}
    seen = set()
    for r in rows:
        seen.add(r["addr"])
        b = base.get(r["addr"])
        tag = "MATCH-PENDING" if r["status"] == "MATCH" and any(
            x["tier"] == "match" for x in r.get("rules", [])) else r["status"]
        if b is None:
            print("  new      %-13s %5.1f  %-10s %s  (%s:%d)" % (tag, r["score"], r["addr"], r["name"], r["unit"], r["line"]))
        elif r["status"] != b["status"] or abs(r["score"] - b["score"]) >= 0.05 or (
                tag == "MATCH-PENDING") != b.get("pending", False):
            was = "MATCH-PENDING" if b.get("pending") else b["status"]
            print("  changed  %-13s %5.1f  %-10s %s  (was %s %.1f)" % (tag, r["score"], r["addr"], r["name"], was, b["score"]))
        else:
            continue
        for p in r.get("problems", [])[:4]:
            print("        - " + p)
        if r.get("wall_note"):
            print("        - wall: %s (%s)" % (r["wall"], r["wall_note"]))
        for f in r.get("rules", []):
            if f["tier"] == "match":
                print("        - %s [match] %s" % (f["rule"], f["text"]))
    for addr, b in base.items():
        if addr not in seen and b["status"] != "ACCEPTED-LOST":
            print("  gone   %-13s        %-10s %s" % (b["status"], addr, b["name"]))


def cmd_match(args):
    all_units = source_files()
    want = set()
    for u in args.units:
        u = u.replace("\\", "/")
        u = re.sub(r"^src/", "", u)
        u = re.sub(r"\.(c|cpp)$", "", u)
        if u not in all_units:
            sys.exit("no unit %s" % u)
        want.add(u)
    objdir = BUILD / "obj"
    results = compile_all({u: all_units[u] for u in want}, objdir, args.jobs)
    broken = compile_report(results)
    objs = {}
    for u, p in all_units.items():
        o = objdir / (u + ".obj")
        if u in results:
            if results[u][0]:
                objs[u] = results[u][0]
        elif o.exists():
            objs[u] = o
    img, uobjs, res, rows, _ = run_compare(objs, all_units, only=want)
    rule_findings({k: v for k, v in uobjs.items() if k in want}, rows)
    for e in res.errors + res.data_errors:
        print("PROBLEM: " + e)
    print_rows(rows, True)
    print_summary(summarize(rows, img, ledger=""))   # these units alone: the ledger's counts are the build's
    # Refresh these units' rows in the last build's report, so the tools that read it (cards.py, packet.py,
    # openwork.py) see the current source without a full build. The rest of the report stays as it was.
    path = BUILD / "report.json"
    if path.exists():
        old = load_report()
        merged = [r for r in old["functions"] if r.get("unit") not in want] + rows
        write_report(summarize(merged, img), merged)
    else:
        print("no build/report.json to refresh: the tools that read it (statics.py renumber, cards.py, packet.py, "
              "openwork.py) need a `build` first")
    return 1 if broken else 0


class DiffError(Exception):
    pass


class DiffSession:
    """The side-by-side diffs of several functions from one load of the units: `decomp.py diff` starts a process
    and reads every unit's annotations and object for each function, which is most of its time.

    The units the functions' annotations sit in are recompiled first (a header's or an included file's owner is
    found after loading). Call it with the checkout's lock held (funcsource.CheckoutLock) outside decomp.py."""

    def __init__(self, targets):
        all_units = source_files()
        objdir = BUILD / "obj"
        homes = {u for u, p in all_units.items() for e in scan(u, p) if any(self.wanted(t, e) for t in targets)}
        self.compile_failed = bool(homes) and bool(compile_report(
            compile_all({u: all_units[u] for u in homes}, objdir)))
        objs = {u: objdir / (u + ".obj") for u in all_units if (objdir / (u + ".obj")).exists()}
        # Load every unit (annotations resolve referents); only the asked functions are compared.
        self.img = pe.Image(target_image())
        self.units = load_units(objs, all_units)
        self.res = Resolver(self.img, self.units, extra_symbols())
        prepare_copies(self.res, objs)

    @staticmethod
    def wanted(target, e):
        return e.kind == "FUNCTION" and (target in (e.name, e.decorated) or (
            target.lower().startswith("0x") and e.addr == int(target, 16)))

    def text(self, target, size=None):
        """The diff `decomp.py diff <target>` prints; DiffError when there is nothing to compare."""
        return self.compare(target, size)[0]

    def compare(self, target, size=None):
        """(the diff text, the function's result row, its first differing instruction pair or None): the pair is
        (ours, retail) as the diff prints them, "" for the side that has no instruction there."""
        hit = next(((u, e) for u in self.units.values() for e in u.entries if self.wanted(target, e)), None)
        if hit is None:
            known = any(self.wanted(target, e) for e in header_entries()) or any(
                self.wanted(target, e) for u, p in source_files().items() for e in scan(u, p))
            raise DiffError("%s is annotated but no compiled unit holds it: run build first" % target if known
                            else "no annotated function %s" % target)
        u, entry = hit
        sym = u.symbols.get(entry.addr)
        if sym is None:
            raise DiffError(u.errors.get(entry.addr))
        sec = u.obj.section(sym.section)
        start, end = u.obj.function_extent(sym)
        unresolved = {}
        relocs = [r for r in sec.relocs if start <= r[0] < end]
        agree = agreeing_sites(sec.data[start:end], relocs, start, self.img.read(entry.addr, end - start))
        data, problems = self.res.relocate(u, sec, start, end, entry.addr, unresolved=unresolved,
                                           trusted=agree.__contains__)
        retail = self.img.read(entry.addr, max(len(data), size or 0)) or b""
        a = ["%08X  %-7s %s" % x for x in name_unresolved(disasm_sized(data, entry.addr), data, entry.addr,
                                                           unresolved)]
        b = ["%08X  %-7s %s" % x for x in disasm(retail, entry.addr)]
        sm = difflib.SequenceMatcher(None, [x[10:] for x in a], [x[10:] for x in b], autojunk=False)
        w = 60
        out = ["%-*s | %s" % (w, "compiled " + (sym.name), "retail 0x%X" % entry.addr)]
        first = None
        for op, i1, i2, j1, j2 in sm.get_opcodes():
            n = max(i2 - i1, j2 - j1)
            for k in range(n):
                l = a[i1 + k] if i1 + k < i2 else ""
                r = b[j1 + k] if j1 + k < j2 else ""
                mark = " " if op == "equal" else ("|" if l and r else ("<" if l else ">"))
                if first is None and op != "equal":
                    first = (l[10:].strip(), r[10:].strip())
                out.append("%-*s %s %s" % (w, l[:w], mark, r))
        out += ["problem: " + p for p in sorted(set(problems))]
        if unresolved and data != retail[:len(data)]:
            out.append("note: a problem's retail address is retail's dword at ours' operand; where the layouts differ "
                       "it is not an address")
        row, _ = compare_function(self.res, u, entry)
        if row.get("wall_note"):
            out.append("wall: %s (%s)" % (row["wall"], row["wall_note"]))
        out.append("status %s  score %.1f" % (row["status"], row["score"]))
        return "\n".join(out), row, first

    def summary_line(self, target, size=None):
        """One line for a triage pass: the function, status, score, the first differing instruction pair and the
        wall class walls.py reads."""
        _, row, first = self.compare(target, size)
        line = "%s  %s  %s  %.1f" % (target, row["name"], row["status"], row["score"])
        if first:
            line += "  | %s  vs  %s" % (first[0] or "-", first[1] or "-")
        if row.get("wall"):
            line += "  | wall: %s" % row["wall"]
        return line


def diff_targets(args):
    """The functions a `diff` command names: the positional ones, then those of `--list FILE` (one address or name
    per line, `#` starts a comment), in order and without repeats."""
    targets = list(args.function)
    if args.list:
        for line in Path(args.list).read_text(encoding="utf-8").splitlines():
            line = line.split("#")[0].strip()
            if line:
                targets.append(line)
    seen = set()
    return [t for t in targets if not (t in seen or seen.add(t))]


def cmd_diff(args):
    targets = diff_targets(args)
    if not targets:
        sys.exit("diff: name a function, or give --list FILE")
    session = DiffSession(targets)
    if session.compile_failed:
        return 1
    if len(targets) == 1 and not args.summary:
        try:
            print(session.text(targets[0], args.size))
        except DiffError as e:
            sys.exit(str(e))
        return 0
    failed = 0
    for t in targets:
        try:
            if args.summary:
                print(session.summary_line(t, args.size))
            else:
                print("=== %s ===" % t)
                print(session.text(t, args.size))
                print()
        except DiffError as e:
            failed += 1
            print("%s  ERROR  %s" % (t, e) if args.summary else "%s" % e)
    return 1 if failed else 0


def write_report(summary, rows):
    """build/report.json, compact: the tools read it, and the indented form took seconds to write and to load."""
    BUILD.mkdir(exist_ok=True)
    tmp = BUILD / "report.json.tmp"
    with open(tmp, "w", encoding="utf-8") as f:
        f.write(json.dumps({"summary": summary, "functions": rows}))
    os.replace(tmp, BUILD / "report.json")


def load_report():
    path = BUILD / "report.json"
    if not path.exists():
        sys.exit("no build/report.json: run build first")
    return load_json(path)


def cmd_report(args):
    rep = load_report()
    print_summary(rep["summary"])
    rows = rep["functions"]
    by_unit = {}
    for r in rows:
        by_unit.setdefault(r["unit"], []).append(r)
    for unit in sorted(by_unit):
        rs = by_unit[unit]
        m = len([r for r in rs if r["status"] == "MATCH"])
        print("  %-48s %4d / %-4d exact" % (unit, m, len(rs)))
    if args.update_baseline:
        base_path = CONFIG / "baseline.json"
        old = load_json(base_path) if base_path.exists() else {}
        new = {}
        for r in rows:
            prev = old.get(r["addr"], {})
            best = r["score"] if prev.get("src") != r["src"] else max(r["score"], prev.get("best", 0))
            new[r["addr"]] = {"name": r["name"], "unit": r["unit"], "status": r["status"],
                              "pending": any(x["tier"] == "match" for x in r.get("rules", [])),
                              "score": r["score"], "best": best,
                              "hist": max(r["score"], prev.get("hist", 0)), "src": r["src"]}
        for addr, prev in old.items():
            if addr not in new:
                new[addr] = prev
        with open(base_path, "w", encoding="utf-8", newline="\n") as f:
            json.dump(dict(sorted(new.items(), key=lambda kv: int(kv[0], 16))), f, indent=1)
            f.write("\n")
        print("baseline updated: %d functions" % len(new))
    return 0


def cmd_regress(args):
    """Score drops against the baseline. A function that was MATCH and no longer is always counts, edited or not: an
    unedited function can lose its match through a header, an include or a referent (AppWrap's `_$E` numbers shift
    with any header it includes). `--accept ADDR` lets the user adjudicate a loss (a wrong claim corrected elsewhere
    that shows an old match was false) or an edited function's score dip (an evidence-backed spelling that scores
    lower). A function listed in config/declaration_sensitive.txt whose source is unchanged and whose diff is only a
    kind its line allows is accepted without `--accept`, on a "declaration-sensitive" line (sensitive_flip)."""
    rep = load_report()
    base_path = CONFIG / "baseline.json"
    base = load_json(base_path) if base_path.exists() else {}
    accepted = {int(a, 16) for a in (getattr(args, "accept", None) or [])}
    listed = declaration_sensitive_listed()
    bad = 0
    for r in rep["functions"]:
        b = base.get(r["addr"])
        if not b:
            continue
        edited = r["src"] != b["src"]
        if b["status"] == "MATCH" and r["status"] != "MATCH" and sensitive_flip(r, edited, listed):
            print("declaration-sensitive %s %s lost MATCH (%s %.1f, %s), source unchanged: accepted, listed in "
                  "config/declaration_sensitive.txt" % (r["addr"], r["name"], r["status"], r["score"], r["flip"]))
            continue
        if b["status"] == "MATCH" and r["status"] != "MATCH":
            ok = int(r["addr"], 16) in accepted
            print("%s %-10s %s lost MATCH (%s, %.1f)%s" % ("accepted" if ok else "REGRESSION", r["addr"], r["name"],
                                                         r["status"], r["score"],
                                                         "" if edited else ", source unchanged"))
            bad += not ok
        elif r["score"] < b["score"]:
            ok = int(r["addr"], 16) in accepted
            label = "dip (unchanged source)" if not edited else "accepted" if ok else "REGRESSION"
            print("%s %-10s %s %.1f -> %.1f" % (label, r["addr"], r["name"], b["score"], r["score"]))
            bad += edited and not ok
    present = {r["addr"] for r in rep["functions"]}
    lost = [a for a in base if base[a]["status"] == "MATCH" and a not in present]
    for a in lost:
        ok = int(a, 16) in accepted
        print("%s %-10s %s: matched in baseline, no longer annotated" % ("accepted" if ok else "REGRESSION", a,
                                                                          base[a]["name"]))
        bad += not ok
    print("regressions: %d" % bad)
    return 1 if bad else 0


def sensitive_flip(row, edited, listed):
    """True when regress accepts the row's lost match by itself: the function is listed in
    config/declaration_sensitive.txt, its source is unchanged, walls.py classes its diff as register, and the diff is
    only a kind its line allows (row["flip"], set by compare_bytes)."""
    entry = listed.get(int(row["addr"], 16))
    return (entry is not None and not edited and row.get("status") == "DIFF" and row.get("wall") == "register"
            and row.get("flip") in entry[1])


def git_dirty():
    out = subprocess.run(["git", "status", "--porcelain"], cwd=ROOT, capture_output=True, text=True).stdout
    return [l for l in out.splitlines() if l.strip()]


def cmd_gate(args):
    dirty = git_dirty()
    if dirty and not args.allow_dirty:
        print("GATE FAILED: working tree not clean (commit first):")
        print("\n".join(dirty[:20]))
        return 1
    data = target_image()
    gate_dir = BUILD / "gate"
    if gate_dir.exists():
        shutil.rmtree(gate_dir)
    gate_dir.mkdir(parents=True)
    all_units = source_files()
    units = units_to_compile(all_units)
    results = compile_all(units, gate_dir / "obj", args.jobs, cache=False)   # the gate trusts no cache
    if compile_report(results):
        print("GATE FAILED: compile errors")
        return 1
    objs = {u: o for u, (o, _) in results.items()}
    img, uobjs, res, rows, matched = run_compare(objs, all_units, cache=False)   # the gate trusts no cache
    if res.errors or res.data_errors:
        print("GATE FAILED: symbol conflicts, or data annotations that don't hold")
        print("\n".join(res.errors + res.data_errors))
        return 1
    if not matched:
        print("GATE FAILED: nothing matched; refusing an empty work list")
        return 1
    # Splice every matched function into a copy of the retail image and require the image hash to hold.
    out = bytearray(data)
    spliced = 0
    for addr, code in sorted(matched.items()):
        off = img.offset(addr)
        if off is None or img.offset(addr + len(code) - 1) is None:
            print("GATE FAILED: 0x%X has no file bytes" % addr)
            return 1
        out[off:off + len(code)] = code
        spliced += len(code)
    built = gate_dir / "stbc.exe"
    built.write_bytes(out)
    digest = hashlib.sha1(built.read_bytes()).hexdigest()
    if digest != PROJECT["sha1"]:
        first = next(i for i in range(len(out)) if out[i] != data[i])
        owner = [a for a, c in matched.items() if img.offset(a) <= first < img.offset(a) + len(c)]
        print("GATE FAILED: image SHA-1 %s != %s; first difference at file offset 0x%X (function %s)" % (
            digest, PROJECT["sha1"], first, ", ".join("0x%X" % a for a in owner) or "?"))
        return 1
    # Independent check: the comparator's exact count and the splice record must agree.
    exact = [r for r in rows if r["status"] == "MATCH"]
    if len(exact) != len(matched) or sum(r["size"] for r in exact) != spliced:
        print("GATE FAILED: comparator (%d functions) and splice record (%d) disagree" % (len(exact), len(matched)))
        return 1
    rule_findings(uobjs, rows, cache=False)   # the gate trusts no cache
    s = summarize(rows, img)
    write_report(s, rows)
    print_rows(rows, False)
    tag ="GATE OK (dirty tree, nothing banked)" if dirty else "GATE OK"
    print("%s: image SHA-1 %s holds with %d functions (%d bytes) built from source" % (
        tag, digest, len(matched), spliced))
    print_summary(s, tag + ": ")
    return 0


def cmd_copies(args):
    """Every datum retail's own instructions read at more than one address, or where another datum lives
    (decide_copies): the address chosen as its copy, and each address's reading functions with their status in the
    last build's report. A function reading the datum away from its copy compares as WRONG-REFERENT (or DIFF)."""
    all_units = source_files()
    objdir = BUILD / "obj"
    objs = {u: objdir / (u + ".obj") for u in all_units if (objdir / (u + ".obj")).exists()}
    img = pe.Image(target_image())
    units = load_units(objs, all_units)
    res = Resolver(img, units, extra_symbols())
    conflicts = prepare_copies(res, objs)
    status = {}
    path = BUILD / "report.json"
    if path.exists():
        status = {int(r["addr"], 16): r for r in load_report()["functions"]}
    names = {e.addr: e.name for u in units.values() for e in u.entries}
    for datum, cands, chosen in sorted(conflicts, key=lambda c: c[0]):
        fns = {f for a in cands.values() for _, f in a}
        wrong = {f for a, fs in cands.items() if a != chosen for _, f in fs}
        if not args.all and not any(status.get(f, {}).get("status") in ("MATCH", "WRONG-REFERENT") for f in wrong):
            continue
        print("%s: copy %s" % (datum, "0x%X" % chosen if chosen is not None else "none (see the problem text)"))
        for a, fs in sorted(cands.items(), key=lambda kv: (kv[0] != chosen, kv[0])):
            row = ", ".join("%s 0x%X [%s]" % (names.get(f, "?"), f, status.get(f, {}).get("status", "?"))
                            for f in sorted({f for _, f in fs}))
            print("   %s0x%X: %s" % ("* " if a == chosen else "  ", a, row))
    print("%d data read away from their copy; %d functions read them" % (len(conflicts), len(
        {f for _, c, _ in conflicts for fs in c.values() for _, f in fs})))
    return 0


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("-j", "--jobs", type=int, default=None)
    sub = ap.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("build")
    p.add_argument("-v", "--verbose", action="store_true")
    p.add_argument("-s", "--summary", action="store_true", help="print only the changes against config/baseline.json")
    p = sub.add_parser("match")
    p.add_argument("units", nargs="+")
    p = sub.add_parser("diff")
    p.add_argument("function", nargs="*")
    p.add_argument("--list", metavar="FILE", help="more functions, one address or name per line (# comments)")
    p.add_argument("--summary", action="store_true",
                   help="one line per function: status, score, first differing instruction pair, wall class")
    p.add_argument("--size", type=lambda x: int(x, 0), help="show at least this many retail bytes")
    p = sub.add_parser("report")
    p.add_argument("--update-baseline", action="store_true")
    p = sub.add_parser("regress")
    p.add_argument("--accept", nargs="*", metavar="ADDR", help="lost matches the user adjudicated as expected")
    p = sub.add_parser("gate")
    p.add_argument("--allow-dirty", action="store_true", help="tooling development only")
    p = sub.add_parser("rules")
    p.add_argument("units", nargs="*")
    p = sub.add_parser("copies", help="data read at more than one retail address: the copy chosen and the readers")
    p.add_argument("--all", action="store_true", help="also data whose misreading functions are all open")
    args = ap.parse_args()
    # One command at a time per checkout: build, match, diff, gate and rules write build/obj and read every unit's
    # object there, and a second one started meanwhile reads half-written objects (a PermissionError on a .obj).
    # The second waits. A child of a command that holds the lock (funcsource.run, the gate's own builds) inherits
    # STBC_CHECKOUT_LOCKED and doesn't wait for its parent.
    import contextlib
    import funcsource
    held = os.environ.get("STBC_CHECKOUT_LOCKED") == str(ROOT)
    with contextlib.nullcontext() if held else funcsource.CheckoutLock():
        os.environ["STBC_CHECKOUT_LOCKED"] = str(ROOT)
        if args.cmd == "rules":
            import rules
            return rules.main(args.units)
        code = {"build": cmd_build, "match": cmd_match, "diff": cmd_diff, "report": cmd_report,
                "regress": cmd_regress, "gate": cmd_gate, "copies": cmd_copies}[args.cmd](args)
        save_scan_cache()
        return code


if __name__ == "__main__":
    sys.exit(main())
