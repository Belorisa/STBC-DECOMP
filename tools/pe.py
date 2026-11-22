"""Minimal PE32 image reader: sections, virtual-address mapping, base relocations."""
import struct


class Image:
    def __init__(self, data):
        self.data = bytes(data)
        pe = struct.unpack_from("<I", data, 0x3C)[0]
        if data[pe:pe + 4] != b"PE\0\0":
            raise ValueError("not a PE image")
        nsec, = struct.unpack_from("<H", data, pe + 6)
        opthdr, = struct.unpack_from("<H", data, pe + 20)
        opt = pe + 24
        self.base, = struct.unpack_from("<I", data, opt + 28)
        reloc_rva, reloc_size = struct.unpack_from("<II", data, opt + 96 + 5 * 8)
        self.sections = []
        off = opt + opthdr
        for _ in range(nsec):
            name = data[off:off + 8].rstrip(b"\0").decode("latin-1")
            vsize, va, rawsize, rawptr = struct.unpack_from("<IIII", data, off + 8)
            self.sections.append((name, self.base + va, vsize, rawptr, rawsize))
            off += 40
        self.text = next(s for s in self.sections if s[0] == ".text")
        self.relocs = self._relocs(reloc_rva, reloc_size)
        self.import_dir = struct.unpack_from("<II", data, opt + 96 + 1 * 8)
        self._imports = None

    def imports(self):
        """{IAT slot address: (dll, name)} from the import directory; `name` is "#<ordinal>" for an import by
        ordinal."""
        if self._imports is not None:
            return self._imports
        out = {}
        rva, _ = self.import_dir
        p = self.offset(self.base + rva) if rva else None
        while p is not None:
            lookup, _, _, name_rva, iat = struct.unpack_from("<IIIII", self.data, p)
            if not name_rva:
                break
            dll = self._cstr(name_rva)
            table = lookup or iat
            i = 0
            while True:
                entry = self.dword(self.base + table + 4 * i)
                if not entry:
                    break
                name = "#%d" % (entry & 0xFFFF) if entry & 0x80000000 else self._cstr(entry + 2)
                out[self.base + iat + 4 * i] = (dll, name)
                i += 1
            p += 20
        self._imports = out
        return out

    def _cstr(self, rva):
        off = self.offset(self.base + rva)
        return self.data[off:self.data.index(b"\0", off)].decode("latin-1")

    def _relocs(self, rva, size):
        out = set()
        if not rva:
            return out
        p = self.offset(self.base + rva)
        end = p + size
        while p < end:
            page, block = struct.unpack_from("<II", self.data, p)
            if block == 0:
                break
            for i in range((block - 8) // 2):
                entry, = struct.unpack_from("<H", self.data, p + 8 + 2 * i)
                if entry >> 12 == 3:
                    out.add(self.base + page + (entry & 0xFFF))
            p += block
        return out

    def section_of(self, va):
        for s in self.sections:
            if s[1] <= va < s[1] + max(s[2], s[4]):
                return s
        return None

    def offset(self, va):
        """File offset of a virtual address, or None when it has no file bytes (.bss tail)."""
        s = self.section_of(va)
        if s is None or va - s[1] >= s[4]:
            return None
        return s[3] + va - s[1]

    def read(self, va, n):
        off = self.offset(va)
        if off is None or self.offset(va + n - 1) is None:
            return None
        return self.data[off:off + n]

    def dword(self, va):
        b = self.read(va, 4)
        return None if b is None else struct.unpack("<I", b)[0]
