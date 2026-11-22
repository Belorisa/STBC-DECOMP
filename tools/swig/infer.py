# Infer SWIG 1.3.6 wrapper specs from retail bytes.
import sys, struct, json, re, collections
sys.path.insert(0, 'C:/Recomp/stbc-wt/slot1/tools')
import pe, capstone
SD = 'build/swig/'
img = pe.Image(open('C:/Recomp/stbc-wt/slot1/orig/stbc.exe', 'rb').read())
cs = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
names = {int(k, 16): v for k, v in json.load(open(SD + 'names.json')).items()}
types = json.load(open(SD + 'types.json'))
SWT = 0x991068
NONE = 0x95dd20
STRTYPE = 0x95e960
RUNTIME = {0x5bae00: 'ConvertPtr', 0x5bb0e0: 'NewPointerObj', 0x74e310: 'ParseTuple', 0x74c6e0: 'PyInt_FromLong',
           0x74fa70: 'PyFloat_FromDouble', 0x7506c0: 'PyString_FromString', 0x7507d0: 'PyString_AsString',
           0x753260: 'PyErr_Format'}
methods = json.load(open(SD + 'methods.json'))
wr = sorted([(f, n) for n, f, fl, v in methods if 0x5b0000 <= f < 0x650000])
starts = [f for f, n in wr] + [0x648030]
classes = set(t['str'][:-2] for t in types if t['str'] and t['str'].endswith(' *'))


def cstr(va):
    b = img.read(va, 300)
    return b[:b.find(b'\0')].decode('latin-1')


def typestr(i):
    return types[i]['str']


def infer(f, n, end):
    ins = list(cs.disasm(img.read(f, end - f), f))
    while ins and ins[-1].mnemonic in ('nop', 'int3'):
        ins.pop()
    fmt = None
    events = []      # in code order
    pend = None
    for k, i in enumerate(ins):
        op = i.op_str
        if i.mnemonic == 'push' and re.fullmatch(r'0x[0-9a-f]+', op):
            v = int(op, 16)
            if 0x8bb000 <= v < 0x9b6000:
                try:
                    s = cstr(v)
                    if s.endswith(':' + n):
                        fmt = s
                except Exception:
                    pass
        m = re.search(r'\[(0x[0-9a-f]+)\]', op)
        if m:
            a = int(m.group(1), 16)
            if SWT <= a < SWT + 349 * 4 and (a - SWT) % 4 == 0:
                pend = (a - SWT) // 4
        if i.mnemonic == 'cmp' and op.endswith(hex(STRTYPE)):
            events.append(('str', k))
        if i.mnemonic == 'setne':
            events.append(('bool', k))
        if i.mnemonic == 'call':
            if re.fullmatch(r'0x[0-9a-f]+', op):
                t = int(op, 16)
                r = RUNTIME.get(t)
                if r == 'ConvertPtr':
                    events.append(('conv', k, pend)); pend = None
                elif r == 'NewPointerObj':
                    events.append(('newptr', k, pend)); pend = None
                elif r:
                    events.append((r, k))
                elif t in names:
                    events.append(('pyapi', k, names[t]))
                else:
                    events.append(('call', k, t))
            else:
                events.append(('icall', k, op))
    return ins, fmt, events


def depths(ins):
    # ESP depth (bytes below the entry ESP) before each instruction, following early returns
    dep = []
    d = 0
    at = {}
    dead = False
    for i in ins:
        if dead:
            if i.address not in at:
                return None
            d = at[i.address]
            dead = False
        elif i.address in at and at[i.address] != d:
            return None
        dep.append(d)
        m, op = i.mnemonic, i.op_str
        if m == 'push':
            d += 4
        elif m == 'pop':
            d -= 4
        elif m == 'sub' and op.startswith('esp, '):
            d += int(op.split(', ')[1], 16)
        elif m == 'add' and op.startswith('esp, '):
            d -= int(op.split(', ')[1], 16)
        elif m.startswith('j') and re.fullmatch(r'0x[0-9a-f]+', op):
            at.setdefault(int(op, 16), d)
            if m == 'jmp':
                dead = True
        elif m == 'ret':
            dead = True
    return dep


def parse_defaults(ins):
    # Track ESP from entry to the PyArg_ParseTuple call: constant stores to stack slots
    # (the defaults) and the slot addresses pushed as ParseTuple's out-arguments.
    depth = 0
    regs = {}
    init = {}
    pushed = []
    for i in ins:
        m, op = i.mnemonic, i.op_str
        if m == 'call':
            if op == hex(0x74e310):
                outs = pushed[::-1][2:]
                return {'init': init, 'args': outs}
            return None
        if m == 'sub' and op.startswith('esp, '):
            depth += int(op.split(', ')[1], 16); continue
        if m == 'push':
            depth += 4
            if op in regs and isinstance(regs[op], tuple):
                pushed.append(regs[op][1])
            else:
                pushed.append(None)
            continue
        mm = re.fullmatch(r'(e\w\w), (e\w\w)', op)
        if m == 'xor' and mm and mm.group(1) == mm.group(2):
            regs[mm.group(1)] = 0; continue
        mm = re.fullmatch(r'(e\w\w), (0x[0-9a-f]+|\d+)', op)
        if m == 'mov' and mm:
            regs[mm.group(1)] = int(mm.group(2), 0); continue
        mm = re.fullmatch(r'(e\w\w), \[esp(?: \+ (0x[0-9a-f]+|\d+))?\]', op)
        if m == 'lea' and mm:
            regs[mm.group(1)] = ('slot', int(mm.group(2) or '0', 0) - depth); continue
        mm = re.fullmatch(r'dword ptr \[esp(?: \+ (0x[0-9a-f]+|\d+))?\], (e\w\w|0x[0-9a-f]+|\d+)', op)
        if m == 'mov' and mm:
            slot = int(mm.group(1) or '0', 0) - depth
            src = mm.group(2)
            if src.startswith('e'):
                if isinstance(regs.get(src), int):
                    init[slot] = regs[src]
            else:
                init[slot] = int(src, 0)
            continue
        mm = re.fullmatch(r'(e\w\w), .*', op)
        if mm:
            regs.pop(mm.group(1), None)
    return None


def spec_of(f, n, end):
    ins, fmt, ev = infer(f, n, end)
    why = lambda s: {'name': n, 'addr': f, 'skip': s}
    if fmt is None:
        return why('no format string')
    pfmt = fmt.split(':')[0]
    nreq = pfmt.index('|') if '|' in pfmt else None
    defaults = None
    if nreq is not None:
        defaults = parse_defaults(ins)
        if defaults is None:
            return why('defaults not recovered')
        pfmt = pfmt.replace('|', '')
    calls = [e for e in ev if e[0] == 'call']
    if len(calls) != 1:
        return why('%d direct calls' % len(calls))
    if any(e[0] in ('icall', 'pyapi') for e in ev):
        return why('indirect call or other Python API')
    ck = calls[0][1]
    target = calls[0][2]
    if target in (0x718cb0, 0x718cf0):
        return why('operator new/delete (by-value object)')
    # arguments
    convs = [e for e in ev if e[0] == 'conv' and e[1] < ck]
    strs = [e for e in ev if e[0] == 'str' and e[1] < ck]
    bools = [e for e in ev if e[0] == 'bool' and e[1] < ck]
    params = []
    ci = si = 0
    nb = 0
    for ch in pfmt:
        if ch == 'O':
            # the next event in code order among conv/str decides
            nxt = sorted([('conv', convs[ci][1], convs[ci][2])] if ci < len(convs) else [], key=lambda e: e[1])
            cands = []
            if ci < len(convs):
                cands.append(('conv', convs[ci][1]))
            if si < len(strs):
                cands.append(('str', strs[si][1]))
            if not cands:
                return why('O argument without conversion')
            kind = min(cands, key=lambda e: e[1])[0]
            if kind == 'conv':
                ti = convs[ci][2]
                ci += 1
                if ti is None:
                    return why('ConvertPtr without type')
                params.append(('ptr', typestr(ti), types[ti]['name']))
            else:
                si += 1
                params.append(('cstr', 'char *', None))
        elif ch == 'i':
            params.append(('int', 'int', None))
        elif ch == 'f':
            params.append(('float', 'float', None))
        elif ch == 'd':
            params.append(('double', 'double', None))
        elif ch == 'l':
            params.append(('long', 'long', None))
        elif ch == 'h':
            params.append(('short', 'short', None))
        elif ch == 'c':
            params.append(('char', 'char', None))
        elif ch == 'b':
            params.append(('uchar', 'unsigned char', None))
        else:
            return why('format %s' % ch)
    if ci != len(convs) or si != len(strs):
        return why('conversion count mismatch')
    if bools:
        ints = [k for k, p in enumerate(params) if p[0] == 'int']
        if len(bools) == len(ints):
            for k in ints:
                params[k] = ('bool', 'bool', None)
        else:
            pd = parse_defaults(ins)
            dep = depths(ins)
            if pd is None or dep is None:
                return why('bool ambiguity')
            for b in bools:
                slot = None
                t = ins[b[1] - 1]
                if t.mnemonic == 'cmp':
                    mm = re.search(r'\[esp(?: \+ (0x[0-9a-f]+|\d+))?\]', t.op_str)
                    if mm:
                        slot = int(mm.group(1) or '0', 0) - dep[b[1] - 1]
                elif t.mnemonic == 'test':
                    reg = t.op_str.split(',')[0]
                    for j in range(b[1] - 2, max(b[1] - 8, 0), -1):
                        if ins[j].mnemonic == 'mov' and ins[j].op_str.startswith(reg + ','):
                            mm = re.search(r'\[esp(?: \+ (0x[0-9a-f]+|\d+))?\]', ins[j].op_str)
                            if mm:
                                slot = int(mm.group(1) or '0', 0) - dep[j]
                            break
                if slot is None or slot not in pd['args']:
                    return why('bool ambiguity')
                k = pd['args'].index(slot)
                if params[k][0] != 'int':
                    return why('bool ambiguity')
                params[k] = ('bool', 'bool', None)
    # calling convention: thiscall when ECX is loaded (and not pushed) before the call
    thiscall = False
    k = ck - 1
    while k >= 0 and ins[k].mnemonic != 'call':
        i = ins[k]
        if i.mnemonic == 'push' and i.op_str == 'ecx':
            break
        if i.mnemonic in ('mov', 'lea') and i.op_str.startswith('ecx,'):
            thiscall = True
            break
        k -= 1
    # return
    after = [e for e in ev if e[1] > ck]
    kinds = [e[0] for e in after]
    span = ins[ck + 1:]
    if 'newptr' in kinds:
        e = [x for x in after if x[0] == 'newptr'][0]
        if e[2] is None:
            return why('NewPointerObj without type')
        ret = ('ptr', typestr(e[2]), types[e[2]]['name'])
    elif 'PyInt_FromLong' in kinds:
        k2 = [x for x in after if x[0] == 'PyInt_FromLong'][0][1]
        seg = ' | '.join(i.mnemonic + ' ' + i.op_str for i in ins[ck + 1:k2])
        if re.search(r'movsx \w+, al|movsx \w+, byte', seg):
            ret = ('char', 'char', None)
        elif re.search(r'and \w+, 0xff\b|movzx \w+, al|movzx \w+, byte|mov byte ptr', seg):
            ret = ('bool', 'bool', None)
        elif re.search(r'movsx \w+, ax|movsx \w+, word', seg):
            ret = ('short', 'short', None)
        elif re.search(r'and \w+, 0xffff\b|movzx \w+, ax|movzx \w+, word|mov word ptr', seg):
            ret = ('ushort', 'unsigned short', None)
        else:
            ret = ('int', 'int', None)
    elif 'PyFloat_FromDouble' in kinds:
        k2 = [x for x in after if x[0] == 'PyFloat_FromDouble'][0][1]
        seg = ' | '.join(i.mnemonic + ' ' + i.op_str for i in ins[ck + 1:k2])
        if re.search(r'fstp dword', seg) or re.search(r'sub esp, 8 \| fstp qword ptr \[esp\]', seg):
            ret = ('float', 'float', None)
        else:
            ret = ('double', 'double', None)
    elif 'PyString_FromString' in kinds:
        ret = ('cstr', 'char *', None)
    else:
        if any(hex(NONE) in i.op_str for i in span):
            ret = ('void', 'void', None)
        else:
            return why('unknown return')
    # owner
    cls = None
    meth = n
    static = False
    if thiscall:
        if not params or params[0][0] != 'ptr':
            return why('thiscall without object')
        cls = params[0][1][:-2]
        if not n.startswith(cls + '_'):
            return why('object type %s does not prefix the name' % cls)
        meth = n[len(cls) + 1:]
    else:
        pre = n.split('_')[0]
        if pre in classes and n.startswith(pre + '_'):
            cls = pre
            meth = n[len(pre) + 1:]
            static = True
    if nreq is not None:
        if len(defaults['args']) != len(params):
            return why('ParseTuple argument count')
        for k in range(nreq, len(params)):
            slot = defaults['args'][k]
            p = params[k]
            if p[0] == 'double':
                lo, hi = defaults['init'].get(slot), defaults['init'].get(slot + 4)
                if lo is None or hi is None:
                    return why('double default not found')
                v = struct.unpack('<d', struct.pack('<II', lo, hi))[0]
            else:
                v = defaults['init'].get(slot)
                if v is None:
                    return why('default not found for argument %d' % k)
            if p[0] == 'ptr':
                # the default lives in arg<k>, which the stack slot of argo<k> does not show
                return why('optional pointer argument')
            params[k] = tuple(p) + (v,)
    if not re.fullmatch(r'[A-Za-z]\w*', meth):
        return why('method name')
    if meth.startswith('new_') or meth.startswith('delete_') or n.startswith('new_') or n.startswith('delete_'):
        return why('constructor/destructor')
    return {'name': n, 'addr': f, 'fmt': fmt, 'params': params, 'ret': ret, 'thiscall': thiscall,
            'cls': cls, 'meth': meth, 'static': static, 'target': target}


if __name__ == '__main__':
    out = []
    for k, (f, n) in enumerate(wr):
        out.append(spec_of(f, n, starts[k + 1]))
    json.dump(out, open(SD + 'specs.json', 'w'), indent=0)
    c = collections.Counter(o.get('skip', 'OK') for o in out)
    for k, v in c.most_common(30):
        print(v, k)
