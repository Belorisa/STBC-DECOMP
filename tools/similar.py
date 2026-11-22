"""Matched functions whose retail code looks most like a given function: worked examples to copy habits from.

    python tools/similar.py 0x5AD450                 # the closest matched functions, with their similarity
    python tools/similar.py 0x5AD450 -k 10 --source 2   # and the source of the closest two
    python tools/similar.py 0x464F50 0x5AD910        # any address: annotated, claimed or in an unwritten gap
    python tools/similar.py --rebuild                # rebuild the index now

The index holds every function matched under the rules (MATCH-PENDING ones are left out: their source is not a
model). Each one's retail bytes are disassembled and normalised: registers become their width, a memory operand
becomes stack, global or member memory of its size, immediates become a kind (0, 1 and -1 are kept), and branch
targets are dropped, so two functions that differ only in register allocation, offsets or addresses read alike. A
direct call keeps its callee's name when the callee has an identity: calling the same helper is the strongest sign of
the same idiom. A virtual call keeps its slot offset.

Features are the 1- to 4-grams of those tokens, weighted by TF-IDF; similarity is their cosine, scaled down when the
two sizes differ a lot. Functions with the same normalised code (template copies, generated accessors) collapse into
one result that says how many copies it stands for.

The index is build/similar.pkl, rebuilt when the matched functions or the known names changed. Retail's code never
changes, so each function's disassembly and features are kept in build/similar_cache.pkl and a rebuild only adds
the new ones. Run `decomp.py build` first.
"""

import argparse
import bisect
import hashlib
import math
import pickle
import re
import sys
from pathlib import Path

import numpy as np
from scipy import sparse

sys.path.insert(0, str(Path(__file__).resolve().parent))
import decomp  # noqa: E402
import funcsource as L  # noqa: E402
import openwork  # noqa: E402

INDEX = decomp.ROOT / "build" / "similar.pkl"
CACHE = decomp.ROOT / "build" / "similar_cache.pkl"   # retail-only work: raw tokens by span, features by code
NGRAMS = (1, 2, 3, 4)
FEATURES = 1 << 20          # hashed feature space
SOURCE_CHARS = 3000         # a longer example source is cut
MIN_SIMILARITY = 0.12       # below this a packet shows nothing: small wrappers match noise

REG32 = {"eax", "ebx", "ecx", "edx", "esi", "edi", "ebp", "esp"}
REG16 = {"ax", "bx", "cx", "dx", "si", "di", "bp", "sp"}
REG8 = {"al", "bl", "cl", "dl", "ah", "bh", "ch", "dh"}
SIZES = {"byte": "8", "word": "16", "dword": "32", "qword": "64", "tbyte": "80", "xword": "80"}


def operand(op):
    """One operand's kind: what it is, never which register or offset."""
    op = op.strip()
    if op in REG32:
        return "r"
    if op in REG16:
        return "r16"
    if op in REG8:
        return "r8"
    if op.startswith("st"):
        return "st0" if op in ("st", "st(0)") else "st"
    m = re.match(r"(?:(\w+) ptr )?\[(.*)\]$", op)
    if m:
        size = SIZES.get(m.group(1) or "", "")
        inner = m.group(2)
        if re.fullmatch(r"0x[0-9a-f]+", inner):
            where = "g"                     # a global
        elif re.search(r"\b(esp|ebp)\b", inner):
            where = "s"                     # a local or an argument
        else:
            where = "m"                     # through a pointer: a member or an element
        return "m%s%s" % (where, size)
    if re.fullmatch(r"-?(0x[0-9a-f]+|\d+)", op):
        v = int(op, 16) if "0x" in op else int(op)
        if v in (0, 1, 0xFFFFFFFF, -1):
            return "i%d" % (1 if v == 1 else 0 if v == 0 else -1)
        return "ia" if 0x400000 <= v < 0xC00000 else "i"
    return "?"


def raw_tokens(img, addr, size):
    """The function's tokens with the direct targets that leave it still as addresses: ("call", target) and
    (mnemonic, target). Only retail decides them, so they are kept between runs."""
    out = []
    for _, mnem, ops in decomp.disasm(img.read(addr, size) or b"", addr):
        if mnem in ("int3", "nop"):
            continue
        direct = re.fullmatch(r"0x[0-9a-f]+", ops)
        if mnem == "call":
            if direct:
                out.append(("call", int(ops, 16)))
            else:
                v = re.fullmatch(r"dword ptr \[\w+(?: \+ (0x[0-9a-f]+|\d+))?\]", ops)
                out.append("call v%s" % (v.group(1) or "0") if v else "call " + operand(ops))
            continue
        if mnem.startswith("j") and direct:
            t = int(ops, 16)
            out.append(mnem if addr <= t < addr + size else (mnem, t))
            continue
        out.append(mnem + (" " + ",".join(operand(o) for o in ops.split(",")) if ops else ""))
    return out


def named(raw, names):
    """raw_tokens with each target's name: a callee's identity, or "?"; a jump out to a known function, or "out"."""
    return [t if isinstance(t, str) else "%s %s" % (t[0], names.get(t[1], "?" if t[0] == "call" else "out"))
            for t in raw]


def tokens(img, addr, size, names):
    return named(raw_tokens(img, addr, size), names)


def features(toks):
    counts = {}
    for n in NGRAMS:
        for i in range(len(toks) - n + 1):
            key = "|".join(toks[i:i + n])
            h = int.from_bytes(hashlib.blake2b(key.encode(), digest_size=4).digest(), "little") % FEATURES
            counts[h] = counts.get(h, 0) + 1
    return counts


def all_names(rows, claims):
    names = {a: n for n, a, _ in claims}
    names.update({int(r["addr"], 16): r["name"] for r in rows if r.get("name")})
    names.update(openwork.INLINE_COPIES)
    return names


def clean_matches(rows):
    return [r for r in rows if openwork.status_of(r) == "matched" and r.get("size")]


def index_key(rows, names):
    """A digest of what the index is built from: the matched functions and every name a token can carry."""
    h = hashlib.sha1(Path(__file__).read_bytes())
    h.update(repr(sorted((r["addr"], r["name"], r["unit"], r["size"]) for r in clean_matches(rows))).encode())
    h.update(repr(sorted(names.items())).encode())
    return h.hexdigest()


def load_cache():
    version = hashlib.sha1(Path(__file__).read_bytes()).hexdigest()
    try:
        with open(CACHE, "rb") as f:
            cache = pickle.load(f)
        if cache["version"] == version:
            return cache
    except Exception:
        pass
    return {"version": version, "raw": {}, "feat": {}}


def build_index(img, rows, names, key=None):
    cache = load_cache()
    raw, feat = {}, {}   # what this build uses: the cache is rewritten with exactly that
    seen = {}            # normalised code -> entry index
    entries = []         # per distinct code: [rows], size, tokens
    for r in clean_matches(rows):
        addr = int(r["addr"], 16)
        span = (addr, r["size"])
        raw[span] = cache["raw"].get(span)
        if raw[span] is None:
            raw[span] = raw_tokens(img, addr, r["size"])
        toks = named(raw[span], names)
        if not toks:
            continue
        code = hashlib.sha1("\n".join(toks).encode()).digest()
        if code in seen:
            entries[seen[code]]["rows"].append((addr, r["name"], r["unit"]))
            continue
        seen[code] = len(entries)
        feat[code] = cache["feat"].get(code)
        if feat[code] is None:
            f = features(toks)
            feat[code] = (np.fromiter(f.keys(), dtype=np.int32, count=len(f)),
                          np.fromiter(f.values(), dtype=np.int32, count=len(f)))
        entries.append({"rows": [(addr, r["name"], r["unit"])], "size": r["size"], "feat": feat[code]})
    if raw.keys() != cache["raw"].keys() or feat.keys() != cache["feat"].keys():
        CACHE.parent.mkdir(parents=True, exist_ok=True)
        with open(CACHE, "wb") as f:
            pickle.dump({"version": cache["version"], "raw": raw, "feat": feat}, f, protocol=pickle.HIGHEST_PROTOCOL)
    df = np.zeros(FEATURES, dtype=np.float32)
    data, ind, ptr = [], [], [0]
    for e in entries:
        ks = e["feat"][0].astype(np.int64)
        vs = e["feat"][1].astype(np.float32)
        df[ks] += 1
        ind.append(ks)
        data.append(1.0 + np.log(vs))
        ptr.append(ptr[-1] + len(ks))
        del e["feat"]
    idf = np.log((1 + len(entries)) / (1 + df)).astype(np.float32) + 1.0
    ind = np.concatenate(ind)
    data = np.concatenate(data) * idf[ind]
    m = sparse.csr_matrix((data, ind, np.array(ptr)), shape=(len(entries), FEATURES), dtype=np.float32)
    norms = np.sqrt(np.asarray(m.multiply(m).sum(axis=1)).ravel())
    m = sparse.diags(1.0 / np.maximum(norms, 1e-9)).dot(m).tocsr()
    index = {"entries": entries, "matrix": m, "idf": idf, "key": key or index_key(rows, names)}
    INDEX.parent.mkdir(parents=True, exist_ok=True)
    with open(INDEX, "wb") as f:
        pickle.dump(index, f, protocol=pickle.HIGHEST_PROTOCOL)
    return index


def load_index(img, rows, names, rebuild=False):
    key = index_key(rows, names)
    if not rebuild and INDEX.exists():
        try:
            with open(INDEX, "rb") as f:
                index = pickle.load(f)
            if index.get("key") == key:
                return index
        except Exception:
            pass   # unreadable: build it again
    return build_index(img, rows, names, key)


def body_size(img, addr, rows, claims):
    """A matched or compiled function's size from the report, else a scan of retail to its end."""
    for r in rows:
        if int(r["addr"], 16) == addr and r.get("size") and r["status"] == "MATCH":
            return r["size"]
    known = sorted({int(r["addr"], 16) for r in rows} | {a for _, a, _ in claims})
    i = bisect.bisect_right(known, addr)
    stop = known[i] if i < len(known) else addr + openwork.SCAN_CAP
    return openwork.callees(img, addr, stop)[1]


def query(index, img, addr, size, names, k=5):
    """[(similarity, entry)] of the k closest distinct matched codes, the function itself left out."""
    feat = features(tokens(img, addr, size, names))
    if not feat:
        return []
    ks = np.fromiter(feat.keys(), dtype=np.int64, count=len(feat))
    vs = (1.0 + np.log(np.fromiter(feat.values(), dtype=np.float32, count=len(feat)))) * index["idf"][ks]
    vs /= max(float(np.sqrt((vs * vs).sum())), 1e-9)
    q = sparse.csr_matrix((vs, ks, [0, len(ks)]), shape=(1, FEATURES), dtype=np.float32)
    sims = np.asarray(index["matrix"].dot(q.T).todense()).ravel()
    sizes = np.array([e["size"] for e in index["entries"]], dtype=np.float32)
    ratio = np.minimum(sizes, size) / np.maximum(np.maximum(sizes, size), 1)
    sims = sims * np.power(ratio, 0.25)
    out = []
    for i in np.argsort(-sims):
        e = index["entries"][i]
        rows = [r for r in e["rows"] if r[0] != addr]
        if not rows:
            continue
        out.append((float(sims[i]), dict(e, rows=rows)))
        if len(out) >= k:
            break
    return out


def source_of(addr):
    try:
        path, ann = L.find_annotation(addr)
        lines = path.read_text(encoding="utf-8", errors="replace").splitlines(keepends=True)
        start, end = L.function_span(lines, ann)
    except SystemExit:
        return None, None
    text = "".join(lines[start:end]).replace("\r\n", "\n").rstrip()
    if len(text) > SOURCE_CHARS:
        text = text[:SOURCE_CHARS] + "\n\t// ... (cut)"
    return path.relative_to(L.ROOT).as_posix(), text


def describe(sim, e):
    addr, name, unit = e["rows"][0]
    more = len(e["rows"]) - 1
    return "- %.2f  0x%X %s  (%s, %d bytes%s)" % (
        sim, addr, name, unit, e["size"], ", +%d with the same code" % more if more else "")


def section(addr, k=5, with_source=2, img=None, rows=None, claims=None):
    """The packet section: the closest matched functions, and the source of the first few."""
    if rows is None:
        rows, claims = openwork.load()
    img = img or decomp.pe.Image(decomp.target_image())
    names = all_names(rows, claims)
    index = load_index(img, rows, names)
    hits = [h for h in query(index, img, addr, body_size(img, addr, rows, claims), names, k)
            if h[0] >= MIN_SIMILARITY]
    if not hits:
        return []
    out = ["## Matched functions whose retail code looks most like this one (similarity 0-1)",
           "Same idioms, helpers and call shapes: a solved function to copy the structure from, not the names."]
    out += [describe(s, e) for s, e in hits]
    for s, e in hits[:with_source]:
        a, name, _ = e["rows"][0]
        path, text = source_of(a)
        if text:
            out += ["", "%s (%.2f), %s:" % (name, s, path), "```cpp", text, "```"]
    return out + [""]


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("functions", nargs="*", help="retail addresses, e.g. 0x5AD450")
    ap.add_argument("-k", type=int, default=5, help="how many results (default 5)")
    ap.add_argument("--source", type=int, default=0, help="print the source of the closest N")
    ap.add_argument("--rebuild", action="store_true", help="rebuild the index")
    args = ap.parse_args()
    try:
        sys.stdout.reconfigure(encoding="utf-8")
    except AttributeError:
        pass
    rows, claims = openwork.load()
    img = decomp.pe.Image(decomp.target_image())
    names = all_names(rows, claims)
    index = load_index(img, rows, names, args.rebuild)
    if not args.functions:
        print("index: %d distinct matched codes (%d functions)" % (
            len(index["entries"]), sum(len(e["rows"]) for e in index["entries"])))
        return 0
    for text in args.functions:
        addr = int(text, 16)
        size = body_size(img, addr, rows, claims)
        print("# 0x%X %s (%d bytes)" % (addr, names.get(addr, "(no identity)"), size))
        hits = query(index, img, addr, size, names, args.k)
        for s, e in hits:
            print(describe(s, e))
        for s, e in hits[:args.source]:
            a, name, _ = e["rows"][0]
            path, src = source_of(a)
            if src:
                print("\n%s (%.2f), %s:\n```cpp\n%s\n```" % (name, s, path, src))
        print()
    return 0


if __name__ == "__main__":
    sys.exit(main())
