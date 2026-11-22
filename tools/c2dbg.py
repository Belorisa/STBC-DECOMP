"""c2dbg: run a compile of one unit under a small Win32 debugger and stop VC6's back end (C2.DLL) at chosen addresses.

    python tools/c2dbg.py game/PhaserGraphic --bp 0x1075CC23 --regs ecx,edx --ret
    python tools/c2dbg.py --source scratch.cpp --flags "/O2 /Ob2 /W4" --warnings
    python tools/c2dbg.py game/PhaserGraphic --functions

The harness knows nothing of the back end's data structures: it launches CL.EXE exactly as tools/decomp.py compiles a
unit (the unit's flags and environment, the unit's compiler; the object goes to build/c2dbg/, never over build/obj),
follows any child process, and writes int3 breakpoints into the live process once the named DLL has loaded (nothing
in tools/vc6 is touched). At each hit a Python callback gets the registers and memory readers; on exit or error the
compiler is terminated.

Addresses: a value at or above the module's preferred image base (C2.DLL: 0x10700000) is a Ghidra address and is
relocated by the actual load base; `+0x1234` (or `rva:0x1234`) is an RVA.

Plugging decoders in (tools/c2trace.py): a decoder is a function `handler(dbg, ctx)`, registered by address:

    import c2dbg

    @c2dbg.probe(0x1075CC23)                  # Ghidra address in C2.DLL (module="c2.dll" is the default)
    def on_expand(dbg, ctx):
        fn = c2dbg.current_function(ctx)     # the function the back end is compiling (SP5 C2.DLL only)
        print(fn, hex(ctx.ecx), ctx.arg(0), ctx.struct(ctx.edx, SITE))
        ctx.on_return(lambda dbg, rctx: print("returned", rctx.eax))

    dbg = c2dbg.Debugger(c2dbg.compile_command("game/PhaserGraphic"), env=c2dbg.vc6.env(), cwd=str(c2dbg.OUT))
    c2dbg.install(dbg)                        # every registered probe (or install(dbg, [probes]))
    rc = dbg.run()

`ctx` holds the stopped thread's registers (`ctx.eax` ... `ctx.eip`, `ctx.set(reg, value)`), `ctx.hit` (this
breakpoint's hit number, from 1), memory readers (`u8 u16 s16 u32 s32 read cstr struct`), `arg(i)` (stack argument i
at a function's first instruction) and `on_return(cb)` (a one-shot breakpoint at the return address; `cb(dbg, ctx)`
sees eax/edx there). A handler may call `dbg.add`, `dbg.remove` or `dbg.stop()`. `dbg.hits` counts hits per address.
Lower level: `dbg.on_module(name, cb(base))` and `dbg.add(runtime_address, handler)`, as in the first c2trace.py.
"""
import argparse
import ctypes
import ctypes.wintypes as wt
import os
import struct
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import vc6  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
OUT = ROOT / "build" / "c2dbg"

# ---------------------------------------------------------------------------------------------------------------------
# pure parts: addresses, memory readers, struct layouts (tested in tools/tests/test_c2dbg.py)


def parse_address(text):
    """'0x1075CC23' -> ('va', 0x1075CC23); '+0x5CC23' or 'rva:0x5CC23' -> ('rva', 0x5CC23)."""
    t = text.strip().lower()
    if t.startswith("rva:"):
        return "rva", int(t[4:], 16)
    if t.startswith("+"):
        return "rva", int(t[1:], 16)
    return "va", int(t, 16)


def relocate(kind, value, preferred_base, actual_base):
    """The runtime address of a Ghidra address or an RVA in a module loaded at actual_base."""
    if kind == "rva":
        return actual_base + value
    if value < preferred_base:
        raise ValueError("0x%08X is below the image base 0x%08X: give an RVA as +0x%X" % (value, preferred_base, value))
    return actual_base + (value - preferred_base)


def pe_header(data):
    """(ImageBase, TimeDateStamp, SizeOfImage) of a PE image's first bytes."""
    pe = struct.unpack_from("<I", data, 0x3C)[0]
    if data[pe:pe + 4] != b"PE\0\0":
        raise ValueError("not a PE image")
    stamp = struct.unpack_from("<I", data, pe + 8)[0]
    image_base, = struct.unpack_from("<I", data, pe + 24 + 28)
    size, = struct.unpack_from("<I", data, pe + 24 + 56)
    return image_base, stamp, size


TYPES = {"u8": "<B", "s8": "<b", "u16": "<H", "s16": "<h", "u32": "<I", "s32": "<i", "ptr": "<I", "f32": "<f",
         "f64": "<d"}


class Memory:
    """Readers over `read(addr, n)`; a subclass supplies `read` (the debuggee, or a test's fake)."""

    def read(self, addr, n, partial=False):
        raise NotImplementedError

    def _num(self, fmt, addr):
        return struct.unpack(fmt, self.read(addr, struct.calcsize(fmt)))[0]

    def u8(self, addr):
        return self._num("<B", addr)

    def u16(self, addr):
        return self._num("<H", addr)

    def s16(self, addr):
        return self._num("<h", addr)

    def u32(self, addr):
        return self._num("<I", addr)

    def s32(self, addr):
        return self._num("<i", addr)

    def dwords(self, addr, n):
        return list(struct.unpack("<%dI" % n, self.read(addr, 4 * n)))

    def cstr(self, addr, limit=512):
        """The NUL-terminated string at addr (latin-1), None for a null pointer; cut at limit bytes."""
        if not addr:
            return None
        out = b""
        while len(out) < limit:
            chunk = self.read(addr + len(out), 64, partial=True)
            if not chunk:
                break
            i = chunk.find(b"\0")
            if i >= 0:
                return (out + chunk[:i]).decode("latin-1")
            out += chunk
        return out[:limit].decode("latin-1")

    def struct(self, addr, layout):
        """A dict read by a layout: [(name, offset, type)], type one of TYPES, 'cstr' (a pointer to a string, read
        through), 'bytes:N', or a nested layout (a pointer read through: None when null)."""
        out = {}
        for name, off, typ in layout:
            if isinstance(typ, (list, tuple)):
                p = self.u32(addr + off)
                out[name] = self.struct(p, typ) if p else None
            elif typ == "cstr":
                out[name] = self.cstr(self.u32(addr + off))
            elif isinstance(typ, str) and typ.startswith("bytes:"):
                out[name] = self.read(addr + off, int(typ[6:], 0))
            else:
                out[name] = self._num(TYPES[typ], addr + off)
        return out


class FakeMemory(Memory):
    """Memory from {address: bytes} blocks (tests, and decoders run on a saved dump)."""

    def __init__(self, blocks):
        self.blocks = dict(blocks)

    def read(self, addr, n, partial=False):
        for base, data in self.blocks.items():
            if base <= addr < base + len(data):
                got = data[addr - base:addr - base + n]
                if len(got) < n and not partial:
                    raise OSError("read past 0x%08X" % (base + len(data)))
                return got
        if partial:
            return b""
        raise OSError("unmapped 0x%08X" % addr)


def pick_return(pending, tid, esp):
    """The index of the pending return breakpoint a hit with this thread and esp completes, else None. A pending
    entry is (tid, entry_esp, callback): the call returned when esp is just above its entry esp (the return address
    popped, plus whatever stdcall arguments the callee popped); with recursion the innermost (largest entry esp
    still at or below esp - 4) wins."""
    best = None
    for i, (t, entry, _cb) in enumerate(pending):
        if t == tid and entry + 4 <= esp <= entry + 4 + 0x400 and (best is None or entry > pending[best][1]):
            best = i
    return best


def env_block(env):
    """A CREATE_UNICODE_ENVIRONMENT block: sorted case-insensitively, NUL-separated, double-NUL ended."""
    items = sorted(env.items(), key=lambda kv: kv[0].upper())
    return "".join("%s=%s\0" % kv for kv in items) + "\0"


def compile_command(unit=None, source=None, flags=(), compiler=None, out=OUT, extra=()):
    """(argv, compiler) for CL.EXE: a unit compiled as tools/decomp.py compiles it (its flags and compiler), or a
    loose source file with the given flags. The object goes to out/, never to build/obj."""
    out = Path(out)
    out.mkdir(parents=True, exist_ok=True)
    if unit is not None:
        import decomp
        unit = unit.replace("\\", "/").removeprefix("src/")
        unit = unit.rsplit(".", 1)[0] if unit.endswith((".c", ".cpp")) else unit
        files = decomp.source_files()
        if unit not in files:
            raise SystemExit("no unit %s" % unit)
        path = files[unit]
        compiler = decomp.compiler_of(path) if compiler is None else compiler
        obj = out / (unit.replace("/", "_") + ".obj")
        args = ["/c", *decomp.cl_args(unit, path, pch=obj.with_suffix(".pch")), "/Fo" + str(obj), *extra]
        decomp.fresh_pch(args)   # a /YX unit creates its .pch beside the object, afresh
    else:
        path = Path(source).resolve()
        obj = out / (path.stem + ".obj")
        lang = "/Tp" if path.suffix.lower() in (".cpp", ".cxx", ".cc") else "/Tc"
        args = ["/nologo", "/c", *flags, *extra, lang, str(path), "/Fo" + str(obj)]
    return [str(vc6.COMPILERS[compiler] / "CL.EXE"), *args], compiler


# ---------------------------------------------------------------------------------------------------------------------
# probes: decoders registered by address


class Probe:
    def __init__(self, addr, handler, module="c2.dll", name=None):
        self.kind, self.value = parse_address(addr) if isinstance(addr, str) else ("va", addr)
        self.handler, self.module, self.name = handler, module.lower(), name or getattr(handler, "__name__", "")


REGISTRY = []


def probe(addr, module="c2.dll", name=None):
    """Decorator: register handler(dbg, ctx) at a Ghidra address (int or '0x...') or an RVA ('+0x...')."""
    def wrap(fn):
        REGISTRY.append(Probe(addr, fn, module, name))
        return fn
    return wrap


def install(dbg, probes=None):
    """Arm the probes (default: the registry) in every process that loads their module."""
    for p in (REGISTRY if probes is None else probes):
        dbg.add_module_bp(p.module, p.kind, p.value, p.handler)


# ---------------------------------------------------------------------------------------------------------------------
# the Win32 debugger

k32 = ctypes.WinDLL("kernel32", use_last_error=True) if os.name == "nt" else None

DEBUG_PROCESS = 0x1
DEBUG_ONLY_THIS_PROCESS = 0x2
CREATE_UNICODE_ENVIRONMENT = 0x400
CREATE_NO_WINDOW = 0x08000000
STARTF_USESTDHANDLES = 0x100
DBG_CONTINUE = 0x00010002
DBG_EXCEPTION_NOT_HANDLED = 0x80010001
WAIT_TIMEOUT = 258

EXCEPTION_DEBUG_EVENT = 1
CREATE_PROCESS_DEBUG_EVENT = 3
EXIT_PROCESS_DEBUG_EVENT = 5
LOAD_DLL_DEBUG_EVENT = 6

BREAKPOINTS = (0x80000003, 0x4000001F)   # native, and WOW64's for 32-bit code
SINGLE_STEPS = (0x80000004, 0x4000001E)

WOW64_CONTEXT_SIZE = 0x2CC
WOW64_CONTEXT_FULL = 0x10007
REG = {"edi": 0x9C, "esi": 0xA0, "ebx": 0xA4, "edx": 0xA8, "ecx": 0xAC, "eax": 0xB0,
       "ebp": 0xB4, "eip": 0xB8, "eflags": 0xC0, "esp": 0xC4}
TRAP_FLAG = 0x100


class STARTUPINFOW(ctypes.Structure):
    _fields_ = [("cb", wt.DWORD), ("lpReserved", wt.LPWSTR), ("lpDesktop", wt.LPWSTR), ("lpTitle", wt.LPWSTR),
                ("dwX", wt.DWORD), ("dwY", wt.DWORD), ("dwXSize", wt.DWORD), ("dwYSize", wt.DWORD),
                ("dwXCountChars", wt.DWORD), ("dwYCountChars", wt.DWORD), ("dwFillAttribute", wt.DWORD),
                ("dwFlags", wt.DWORD), ("wShowWindow", wt.WORD), ("cbReserved2", wt.WORD),
                ("lpReserved2", ctypes.c_void_p), ("hStdInput", wt.HANDLE), ("hStdOutput", wt.HANDLE),
                ("hStdError", wt.HANDLE)]


class PROCESS_INFORMATION(ctypes.Structure):
    _fields_ = [("hProcess", wt.HANDLE), ("hThread", wt.HANDLE), ("dwProcessId", wt.DWORD),
                ("dwThreadId", wt.DWORD)]


class DEBUG_EVENT(ctypes.Structure):
    # the union is read by offset: on x64 it starts at 16
    _fields_ = [("dwDebugEventCode", wt.DWORD), ("dwProcessId", wt.DWORD), ("dwThreadId", wt.DWORD),
                ("_pad", wt.DWORD), ("u", ctypes.c_ubyte * 160)]


if k32 is not None:
    k32.CreateProcessW.argtypes = [wt.LPCWSTR, wt.LPWSTR, ctypes.c_void_p, ctypes.c_void_p, wt.BOOL, wt.DWORD,
                                   ctypes.c_void_p, wt.LPCWSTR, ctypes.POINTER(STARTUPINFOW),
                                   ctypes.POINTER(PROCESS_INFORMATION)]
    k32.WaitForDebugEvent.argtypes = [ctypes.POINTER(DEBUG_EVENT), wt.DWORD]
    k32.ContinueDebugEvent.argtypes = [wt.DWORD, wt.DWORD, wt.DWORD]
    k32.ReadProcessMemory.argtypes = [wt.HANDLE, ctypes.c_void_p, ctypes.c_void_p, ctypes.c_size_t,
                                      ctypes.POINTER(ctypes.c_size_t)]
    k32.WriteProcessMemory.argtypes = k32.ReadProcessMemory.argtypes
    k32.FlushInstructionCache.argtypes = [wt.HANDLE, ctypes.c_void_p, ctypes.c_size_t]
    k32.OpenThread.restype = wt.HANDLE
    k32.OpenThread.argtypes = [wt.DWORD, wt.BOOL, wt.DWORD]
    k32.Wow64GetThreadContext.argtypes = [wt.HANDLE, ctypes.c_void_p]
    k32.Wow64SetThreadContext.argtypes = [wt.HANDLE, ctypes.c_void_p]
    k32.CloseHandle.argtypes = [wt.HANDLE]
    k32.GetFinalPathNameByHandleW.argtypes = [wt.HANDLE, wt.LPWSTR, wt.DWORD, wt.DWORD]
    k32.TerminateProcess.argtypes = [wt.HANDLE, wt.UINT]
    k32.DebugSetProcessKillOnExit.argtypes = [wt.BOOL]


class Context(Memory):
    """One thread's registers at a stop, plus the stopped process's memory."""

    def __init__(self, dbg, hthread, tid, hit=0):
        self.dbg, self.hthread, self.tid, self.hit = dbg, hthread, tid, hit
        self.buf = ctypes.create_string_buffer(WOW64_CONTEXT_SIZE + 16)
        self.base = ((ctypes.addressof(self.buf) + 15) & ~15) - ctypes.addressof(self.buf)
        struct.pack_into("<I", self.buf, self.base, WOW64_CONTEXT_FULL)
        if not k32.Wow64GetThreadContext(hthread, ctypes.addressof(self.buf) + self.base):
            raise OSError(ctypes.get_last_error(), "Wow64GetThreadContext")
        self.dirty = False

    def __getattr__(self, name):
        if name in REG:
            return struct.unpack_from("<I", self.buf, self.base + REG[name])[0]
        raise AttributeError(name)

    def regs(self, names=None):
        return {n: getattr(self, n) for n in (names or REG)}

    def set(self, name, value):
        struct.pack_into("<I", self.buf, self.base + REG[name], value & 0xFFFFFFFF)
        self.dirty = True

    def flush(self):
        if self.dirty and not k32.Wow64SetThreadContext(self.hthread, ctypes.addressof(self.buf) + self.base):
            raise OSError(ctypes.get_last_error(), "Wow64SetThreadContext")
        self.dirty = False

    def read(self, addr, n, partial=False):
        return self.dbg.read(addr, n, partial)

    def arg(self, i):
        """The i-th stack argument (0-based) at a function's first instruction."""
        return self.u32(self.esp + 4 + 4 * i)

    def args(self, n):
        return [self.arg(i) for i in range(n)]

    def on_return(self, callback):
        """Call callback(dbg, ctx) once, when the function this breakpoint entered returns (valid at its first
        instruction: [esp] is the return address)."""
        self.dbg.add_return(self.u32(self.esp), self.tid, self.esp, callback)


class _Bp:
    __slots__ = ("orig", "handlers", "returns")

    def __init__(self, orig):
        self.orig, self.handlers, self.returns = orig, [], []


class _Process:
    def __init__(self, pid, hprocess):
        self.pid, self.hprocess = pid, hprocess
        self.bps = {}          # address -> _Bp
        self.step_rearm = {}   # thread id -> address to re-arm after a single step
        self.modules = {}      # lowercase basename -> base


class Debugger:
    """Launch argv under the debug API; follow child processes; int3 breakpoints with handlers."""

    def __init__(self, argv, env=None, cwd=None, follow_children=True, stdout=None):
        self.argv, self.env, self.cwd = list(argv), env, cwd
        self.follow, self.stdout = follow_children, stdout
        self.procs = {}             # pid -> _Process
        self.cur = None             # the process whose event is being handled
        self.module_cbs = {}        # lowercase basename -> [callback(base)]
        self.module_bps = []        # (module, kind, value, handler)
        self.hits = {}              # runtime address -> permanent-handler hit count
        self.loaded = []            # (pid, name, base, image_base, stamp, size)
        self.stop_requested = False
        self.root_pid = None
        self.pending = None         # (pid, tid) of the event being handled, until it is continued

    @property
    def hprocess(self):
        return self.cur.hprocess if self.cur else None

    # -- memory -------------------------------------------------------------------------------------------------
    def read(self, addr, n, partial=False):
        buf = ctypes.create_string_buffer(n)
        got = ctypes.c_size_t(0)
        ok = k32.ReadProcessMemory(self.hprocess, addr, buf, n, ctypes.byref(got))
        if not ok and not partial:
            raise OSError(ctypes.get_last_error(), "ReadProcessMemory 0x%08X" % addr)
        return buf.raw[:got.value]

    def write(self, addr, data):
        got = ctypes.c_size_t(0)
        if not k32.WriteProcessMemory(self.hprocess, addr, data, len(data), ctypes.byref(got)):
            raise OSError(ctypes.get_last_error(), "WriteProcessMemory 0x%08X" % addr)
        k32.FlushInstructionCache(self.hprocess, addr, len(data))

    # -- breakpoints --------------------------------------------------------------------------------------------
    def _bp(self, addr):
        bp = self.cur.bps.get(addr)
        if bp is None:
            bp = self.cur.bps[addr] = _Bp(self.read(addr, 1))
            self.write(addr, b"\xCC")
        return bp

    def add(self, addr, handler):
        """A breakpoint at a runtime address of the current process (call it from a module callback or a handler)."""
        self._bp(addr).handlers.append(handler)

    def remove(self, addr, handler=None):
        bp = self.cur.bps.get(addr)
        if bp is None:
            return
        if handler is not None and handler in bp.handlers:
            bp.handlers.remove(handler)
        elif handler is None:
            bp.handlers.clear()
        if not bp.handlers and not bp.returns:
            del self.cur.bps[addr]
            self.write(addr, bp.orig)

    def add_return(self, ret_addr, tid, entry_esp, callback):
        self._bp(ret_addr).returns.append((tid, entry_esp, callback))

    def on_module(self, basename, callback):
        """callback(base) when a module of that basename loads (in any debugged process)."""
        self.module_cbs.setdefault(basename.lower(), []).append(callback)

    def add_module_bp(self, module, kind, value, handler):
        """A breakpoint at a Ghidra address ('va') or an RVA ('rva') of a module, armed when it loads."""
        self.module_bps.append((module.lower(), kind, value, handler))

    def module_base(self, name):
        return self.cur.modules.get(name.lower()) if self.cur else None

    def stop(self):
        """Ask the loop to terminate the debuggee at the next event (a handler's way out)."""
        self.stop_requested = True

    # -- loop ---------------------------------------------------------------------------------------------------
    def _module_path(self, hfile):
        if not hfile:
            return ""
        buf = ctypes.create_unicode_buffer(1024)
        n = k32.GetFinalPathNameByHandleW(hfile, buf, 1024, 0)
        return buf.value[:n].removeprefix("\\\\?\\") if n else ""

    def _export_name(self, base):
        """A DLL's own name from its export directory (when the event carries no file handle)."""
        try:
            pe = struct.unpack("<I", self.read(base + 0x3C, 4))[0]
            exp = struct.unpack("<I", self.read(base + pe + 24 + 96, 4))[0]
            if exp:
                name = struct.unpack("<I", self.read(base + exp + 12, 4))[0]
                return Memory.cstr(self, base + name) or ""
        except OSError:
            pass
        return ""

    def _loaded(self, base, path):
        name = os.path.basename(path).lower() if path else self._export_name(base).lower()
        try:
            image_base, stamp, size = pe_header(open(path, "rb").read(0x1000)) if path else \
                pe_header(self.read(base, 0x1000))
        except (OSError, ValueError):
            image_base = stamp = size = None
        self.cur.modules[name] = base
        self.loaded.append((self.cur.pid, name, base, image_base, stamp, size))
        for module, kind, value, handler in self.module_bps:
            if module == name:
                self.add(relocate(kind, value, image_base, base), handler)
        for cb in self.module_cbs.get(name, ()):
            cb(base)

    def _startup(self):
        si = STARTUPINFOW()
        si.cb = ctypes.sizeof(si)
        inherit = False
        if self.stdout is not None:
            import msvcrt
            h = msvcrt.get_osfhandle(self.stdout.fileno())
            os.set_handle_inheritable(h, True)
            si.dwFlags = STARTF_USESTDHANDLES
            si.hStdOutput = si.hStdError = h
            inherit = True
        return si, inherit

    def run(self):
        """Run to the end of the root process; return its exit code (None when it was terminated by stop())."""
        si, inherit = self._startup()
        pi = PROCESS_INFORMATION()
        cmd = subprocess.list2cmdline(self.argv)
        envb = None
        if self.env is not None:
            block = env_block(self.env)
            envb = ctypes.create_unicode_buffer(block, len(block))
        flags = (DEBUG_PROCESS if self.follow else DEBUG_ONLY_THIS_PROCESS) | CREATE_UNICODE_ENVIRONMENT
        flags |= CREATE_NO_WINDOW
        if not k32.CreateProcessW(None, ctypes.create_unicode_buffer(cmd), None, None, inherit, flags, envb,
                                  self.cwd, ctypes.byref(si), ctypes.byref(pi)):
            raise OSError(ctypes.get_last_error(), "CreateProcessW")
        k32.DebugSetProcessKillOnExit(True)   # if Python dies, the compiler dies with it
        self.root_pid = pi.dwProcessId
        exit_code = None
        try:
            exit_code = self._loop()
        except BaseException:
            self._kill_all()
            raise
        finally:
            k32.CloseHandle(pi.hThread)
            k32.CloseHandle(pi.hProcess)
        return exit_code

    def _loop(self, timeout=0xFFFFFFFF):
        ev = DEBUG_EVENT()
        exit_code = None
        while True:
            if not k32.WaitForDebugEvent(ctypes.byref(ev), timeout):
                err = ctypes.get_last_error()
                if err == WAIT_TIMEOUT or err == 121:
                    return exit_code
                raise OSError(err, "WaitForDebugEvent")
            code, pid, tid, status = ev.dwDebugEventCode, ev.dwProcessId, ev.dwThreadId, DBG_CONTINUE
            self.pending = (pid, tid)
            u = bytes(ev.u)
            self.cur = self.procs.get(pid)
            if code == CREATE_PROCESS_DEBUG_EVENT:
                hfile, hproc, _hthr, base = struct.unpack_from("<QQQQ", u, 0)
                self.cur = self.procs[pid] = _Process(pid, hproc)
                path = self._module_path(hfile)
                if hfile:
                    k32.CloseHandle(hfile)
                self._loaded(base, path)
            elif code == LOAD_DLL_DEBUG_EVENT:
                hfile, base = struct.unpack_from("<QQ", u, 0)
                path = self._module_path(hfile)
                if hfile:
                    k32.CloseHandle(hfile)
                if self.cur is not None:
                    self._loaded(base, path)
            elif code == EXIT_PROCESS_DEBUG_EVENT:
                if pid == self.root_pid:
                    exit_code = struct.unpack_from("<I", u, 0)[0]
                self.procs.pop(pid, None)
                self.pending = None
                k32.ContinueDebugEvent(pid, tid, DBG_CONTINUE)
                if not self.procs:
                    return None if self.stop_requested else exit_code
                continue
            elif code == EXCEPTION_DEBUG_EVENT and self.cur is not None:
                exc_code, _flags, _rec, exc_addr = struct.unpack_from("<IIQQ", u, 0)
                status = self._exception(tid, exc_code, exc_addr)
            if self.stop_requested:
                self._terminate()
            self.pending = None
            k32.ContinueDebugEvent(pid, tid, status)

    def _terminate(self):
        for p in self.procs.values():
            k32.TerminateProcess(p.hprocess, 1)

    def _kill_all(self):
        """After an error in a handler: terminate every debuggee and drain its events (it then exits)."""
        self.stop_requested = True
        try:
            self._terminate()
            if self.pending:   # the event the error interrupted: the process can't exit until it is continued
                k32.ContinueDebugEvent(self.pending[0], self.pending[1], DBG_CONTINUE)
                self.pending = None
            self._loop(timeout=5000)
        except OSError:
            pass

    def _exception(self, tid, code, addr):
        proc = self.cur
        if code in BREAKPOINTS:
            bp = proc.bps.get(addr)
            if bp is None:
                return DBG_CONTINUE      # the loader's initial breakpoints
            hthread = k32.OpenThread(0x1FFFFF, False, tid)
            try:
                ctx = Context(self, hthread, tid)
                ctx.set("eip", addr)
                if bp.handlers:
                    self.hits[addr] = ctx.hit = self.hits.get(addr, 0) + 1
                for h in list(bp.handlers):
                    h(self, ctx)
                i = pick_return(bp.returns, tid, ctx.esp)
                if i is not None:
                    _t, _e, cb = bp.returns.pop(i)
                    cb(self, ctx)
                if proc.bps.get(addr) is bp:
                    self.write(addr, bp.orig)
                    if bp.handlers or bp.returns:
                        ctx.set("eflags", ctx.eflags | TRAP_FLAG)
                        proc.step_rearm[tid] = addr
                    else:
                        del proc.bps[addr]
                ctx.flush()
            finally:
                k32.CloseHandle(hthread)
            return DBG_CONTINUE
        if code in SINGLE_STEPS and tid in proc.step_rearm:
            a = proc.step_rearm.pop(tid)
            if a in proc.bps:
                self.write(a, b"\xCC")
            return DBG_CONTINUE
        return DBG_EXCEPTION_NOT_HANDLED


# ---------------------------------------------------------------------------------------------------------------------
# C2.DLL SP5 (build 8966) probes that need no decoding beyond a name. Evidence: Ghidra's decompilation in
# C:\Recomp\c2-ghidra\export\decomp (the per-function loop at 0x10757FC2, the warning routine at 0x10742BBE).

C2_SP5_STAMP = 0x39A4A91A
C2_CURRENT_FUNCTION = 0x107AC378   # the symbol of the function being compiled (the 0x10757FC2 loop sets it)
C2_SYM_NAME = 0x18                 # a symbol's decorated name (pointer to a string)
C2_FUNCTION_CONTEXT = 0x1071BA73   # fastcall: ecx = a function symbol; called by the loop for each function it
C2_FUNCTION_LOOP = (0x10757FC2, 0x10757FC2 + 2020)   # compiles, and by the inliner (0x107181A8) for each candidate
C2_WARNING = 0x10742BBE            # cdecl warning(level, number - 4000, ...format arguments)


def c2_va(dbg, va):
    """A C2.DLL Ghidra address relocated into the current process."""
    base = dbg.module_base("c2.dll")
    return base + (va - 0x10700000)


def ghidra_address(dbg, addr):
    """A runtime address back to the Ghidra address of the module that holds it (the address itself if none)."""
    for _pid, _name, base, image_base, _stamp, size in dbg.loaded:
        if image_base is not None and size and base <= addr < base + size:
            return addr - base + image_base
    return addr


def current_function(ctx):
    """The decorated name of the function C2.DLL SP5 is compiling now (None before the first)."""
    sym = ctx.u32(c2_va(ctx.dbg, C2_CURRENT_FUNCTION))
    return ctx.cstr(ctx.u32(sym + C2_SYM_NAME)) if sym else None


def undecorate(name):
    if not name or not name.startswith("?"):
        return name
    try:
        dbghelp = ctypes.WinDLL("dbghelp")
        buf = ctypes.create_string_buffer(1024)
        if dbghelp.UnDecorateSymbolName(name.encode("latin-1"), buf, 1024, 0):
            return buf.value.decode("latin-1")
    except OSError:
        pass
    return name


# ---------------------------------------------------------------------------------------------------------------------
# command line


def parse_regs(text):
    regs = [r.strip().lower() for r in text.split(",") if r.strip()] if text else []
    bad = [r for r in regs if r not in REG]
    if bad:
        raise ValueError("unknown register %s (one of %s)" % (", ".join(bad), ", ".join(REG)))
    return regs


def parse_bps(values):
    """--bp values (repeatable, comma-separated) -> [(kind, value, text)]."""
    out = []
    for v in values or ():
        for t in v.split(","):
            if t.strip():
                kind, value = parse_address(t)
                out.append((kind, value, t.strip()))
    return out


def format_hit(addr_text, hit, args, regs, where=None):
    parts = ["%s #%d" % (addr_text, hit)]
    if args is not None:
        parts.append("args=[%s]" % ", ".join("0x%X" % a for a in args))
    parts += ["%s=0x%X" % kv for kv in regs.items()]
    if where:
        parts.append("in %s" % where)
    return " ".join(parts)


def build_parser():
    ap = argparse.ArgumentParser(prog="c2dbg.py", description=__doc__.split("\n\n")[0])
    ap.add_argument("unit", nargs="?", help="a unit (game/PhaserGraphic), compiled with its own flags")
    ap.add_argument("--source", help="compile this file instead of a unit")
    ap.add_argument("--flags", default="/O2", help="cl flags for --source (default /O2)")
    ap.add_argument("--compiler", choices=[c for c in vc6.COMPILERS if c], help="another tools/vc6 compiler")
    ap.add_argument("--module", default="c2.dll", help="the module --bp addresses belong to (default c2.dll)")
    ap.add_argument("--bp", action="append", help="breakpoint: Ghidra address (0x1075CC23) or RVA (+0x5CC23)")
    ap.add_argument("--args", type=int, default=0, help="stack arguments to print at each hit")
    ap.add_argument("--regs", default="", help="registers to print, e.g. ecx,edx")
    ap.add_argument("--ret", action="store_true", help="also print eax when the function returns")
    ap.add_argument("--max-hits", type=int, default=0, help="print at most N hits per breakpoint (all are counted)")
    ap.add_argument("--where", action="store_true", help="name the function being compiled at each hit (SP5)")
    ap.add_argument("--function", help="print only hits while compiling a function whose name contains this (SP5)")
    ap.add_argument("--warnings", action="store_true", help="print C2's warning calls (C4710/C4711...) (SP5)")
    ap.add_argument("--functions", action="store_true", help="list the functions the back end compiles (SP5)")
    ap.add_argument("--stop-after", type=int, default=0, help="terminate the compile after N printed hits")
    ap.add_argument("--cl-output", action="store_true", help="print CL's own output at the end")
    ap.add_argument("--undecorate", action="store_true", help="undecorate function names")
    return ap


def main(argv=None):
    a = build_parser().parse_args(argv)
    if bool(a.unit) == bool(a.source):
        raise SystemExit("give a unit or --source")
    regs = parse_regs(a.regs)
    bps = parse_bps(a.bp)
    sp5_only = a.where or a.function or a.warnings or a.functions
    cmd, compiler = compile_command(unit=a.unit, source=a.source, flags=a.flags.split(), compiler=a.compiler)
    if sp5_only and compiler is not None:
        raise SystemExit("--where/--function/--warnings/--functions know SP5's C2.DLL only (unit uses %r)" % compiler)
    log = OUT / ("%s.cl.txt" % Path(a.source or a.unit).stem)
    printed = [0]
    name = (lambda s: undecorate(s) if s else s) if a.undecorate else (lambda s: s)

    def where(ctx):
        return name(current_function(ctx)) if (a.where or a.function) else None

    def selected(ctx):
        if not a.function:
            return True
        fn = current_function(ctx) or ""
        return a.function in fn or a.function in (undecorate(fn) or "")

    def emit(dbg, line):
        print(line, flush=True)
        printed[0] += 1
        if a.stop_after and printed[0] >= a.stop_after:
            print("-- stopping after %d hits" % printed[0])
            dbg.stop()

    probes = []
    for kind, value, text in bps:
        def handler(dbg, ctx, text=text):
            if not selected(ctx) or (a.max_hits and ctx.hit > a.max_hits):
                return
            hit = ctx.hit
            emit(dbg, format_hit(text, hit, ctx.args(a.args) if a.args else None, ctx.regs(regs), where(ctx)))
            if a.ret:
                ctx.on_return(lambda dbg, r: print("%s #%d -> eax=0x%X edx=0x%X" % (text, hit, r.eax, r.edx),
                                                   flush=True))
        probes.append(Probe("+0x%X" % value if kind == "rva" else value, handler, a.module))
    if a.warnings:
        def on_warning(dbg, ctx):
            level, number = ctx.s32(ctx.esp + 4), ctx.s32(ctx.esp + 8)
            if selected(ctx):
                emit(dbg, "C%d level %d %s" % (number + 4000, level, name(ctx.cstr(ctx.arg(2)))))
        probes.append(Probe(C2_WARNING, on_warning))
    if a.functions:
        def on_function(dbg, ctx):
            caller = ghidra_address(dbg, ctx.u32(ctx.esp))
            if C2_FUNCTION_LOOP[0] <= caller < C2_FUNCTION_LOOP[1]:   # not the inliner's call for a candidate
                emit(dbg, "function %s" % name(ctx.cstr(ctx.u32(ctx.ecx + C2_SYM_NAME))))
        probes.append(Probe(C2_FUNCTION_CONTEXT, on_function))
    if not probes:
        raise SystemExit("nothing to do: give --bp, --warnings or --functions")

    with open(log, "w") as out:
        dbg = Debugger(cmd, env=vc6.env(), cwd=str(OUT), stdout=out)
        install(dbg, probes)
        rc = dbg.run()
    for pid, mod, base, image_base, stamp, _size in dbg.loaded:
        if mod == a.module.lower():
            note = "" if stamp != C2_SP5_STAMP else " (SP5 build 8966)"
            print("-- %s at 0x%08X (image base 0x%08X, timestamp %08X%s)" % (mod, base, image_base or 0, stamp or 0,
                                                                          note))
    counts = ", ".join("0x%08X: %d" % (ghidra_address(dbg, addr), n) for addr, n in sorted(dbg.hits.items()))
    print("-- hits %s" % (counts or "none"))
    print("-- cl %s" % ("terminated" if rc is None else "exit %d" % rc))
    if a.cl_output:
        print(log.read_text(errors="replace"), end="")
    return 0 if rc in (0, None) else 1


if __name__ == "__main__":
    sys.exit(main())
