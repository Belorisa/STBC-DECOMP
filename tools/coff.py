"""Minimal reader for i386 COFF object files as emitted by MSVC 6."""
import struct

IMAGE_REL_I386_DIR32 = 0x06
IMAGE_REL_I386_DIR32NB = 0x07
IMAGE_REL_I386_REL32 = 0x14

SCN_CNT_UNINITIALIZED = 0x00000080
SCN_LNK_COMDAT = 0x00001000
SCN_MEM_EXECUTE = 0x20000000

SC_EXTERNAL = 2
SC_STATIC = 3
SC_LABEL = 6
SC_WEAK_EXTERNAL = 105


class Section:
    def __init__(self, index, name, data, size, flags, relocs):
        self.index = index      # 1-based, as symbols reference it
        self.name = name
        self.data = data        # b"" for uninitialised sections
        self.size = size
        self.flags = flags
        self.relocs = relocs    # list of (offset, symbol index, type)

    @property
    def is_code(self):
        return bool(self.flags & SCN_MEM_EXECUTE)

    @property
    def alignment(self):
        """The section's alignment in bytes (IMAGE_SCN_ALIGN_*); 16 when unspecified, as the linker assumes."""
        n = (self.flags >> 20) & 0xF
        return 1 << (n - 1) if n else 16

    @property
    def is_bss(self):
        return bool(self.flags & SCN_CNT_UNINITIALIZED)


class Symbol:
    __slots__ = ("index", "name", "value", "section", "type", "storage", "weak_default")

    def __init__(self, index, name, value, section, typ, storage):
        self.index = index
        self.name = name
        self.value = value
        self.section = section  # 1-based section number; 0 = undefined, negative = absolute/debug
        self.type = typ
        self.storage = storage
        self.weak_default = None  # symbol index a weak external falls back to

    @property
    def is_function(self):
        return (self.type >> 4) == 2

    @property
    def defined(self):
        return self.section > 0


class Coff:
    def __init__(self, data):
        machine, nsec, _, symptr, nsym, opthdr, _ = struct.unpack_from("<HHIIIHH", data, 0)
        if machine != 0x14C:
            raise ValueError("not an i386 COFF object")
        strtab = symptr + nsym * 18
        self._strtab = data[strtab:]

        self.sections = []
        off = 20 + opthdr
        for i in range(nsec):
            raw = data[off:off + 40]
            name = self._name(raw[:8])
            size, rawptr, relptr, _, nrel, _, flags = struct.unpack_from("<IIIIHHI", raw, 16)
            relocs = list(struct.iter_unpack("<IIH", data[relptr:relptr + 10 * nrel]))
            body = b"" if flags & SCN_CNT_UNINITIALIZED else data[rawptr:rawptr + size]
            self.sections.append(Section(i + 1, name, body, size, flags, relocs))
            off += 40

        self.symbols = {}
        i = 0
        while i < nsym:
            raw = data[symptr + 18 * i:symptr + 18 * (i + 1)]
            name = self._name(raw[:8])
            value, secnum, typ, storage, naux = struct.unpack_from("<IhHBB", raw, 8)
            self.symbols[i] = Symbol(i, name, value, secnum, typ, storage)
            if storage == SC_WEAK_EXTERNAL and naux:
                self.symbols[i].weak_default = struct.unpack_from("<I", data, symptr + 18 * (i + 1))[0]
            i += 1 + naux

    def _name(self, raw):
        if raw[:4] == b"\0\0\0\0":
            off = struct.unpack_from("<I", raw, 4)[0]
            end = self._strtab.index(b"\0", off)
            return self._strtab[off:end].decode("latin-1")
        return raw.rstrip(b"\0").decode("latin-1")

    def section(self, number):
        return self.sections[number - 1]

    def _starts(self, kind):
        """Per section, the sorted offsets of the symbols that bound an extent (built once)."""
        cache = self.__dict__.setdefault("_starts_cache", {})
        if kind not in cache:
            table = {}
            for s in self.symbols.values():
                if s.section <= 0:
                    continue
                if kind == "any":
                    if s.storage not in (SC_EXTERNAL, SC_STATIC, SC_LABEL) or s.name == self.section(s.section).name:
                        continue
                elif not s.is_function:
                    continue
                table.setdefault(s.section, set()).add(s.value)
            cache[kind] = {k: sorted(v) for k, v in table.items()}
        return cache[kind]

    def _next_start(self, sym, kind):
        import bisect
        offsets = self._starts(kind).get(sym.section, [])
        k = bisect.bisect_right(offsets, sym.value)
        return offsets[k] if k < len(offsets) else self.section(sym.section).size

    def extent(self, sym):
        """Byte range [start, end) of a defined symbol: up to the next symbol in its section, or the section end."""
        return sym.value, self._next_start(sym, "any")

    def function_extent(self, sym):
        """A function's range: a /Gy COMDAT section holds one function; otherwise up to the next function."""
        return sym.value, self._next_start(sym, "fn")
