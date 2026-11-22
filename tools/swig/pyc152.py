# Minimal Python 1.5.2 marshal reader (code objects) and class-hierarchy extraction from App.pyc.
import struct, sys, json
class R:
    def __init__(s, b): s.b = b; s.i = 0
    def byte(s): v = s.b[s.i]; s.i += 1; return v
    def l(s): v = struct.unpack_from('<i', s.b, s.i)[0]; s.i += 4; return v
    def h(s): v = struct.unpack_from('<h', s.b, s.i)[0]; s.i += 2; return v
    def obj(s):
        t = chr(s.byte())
        if t == '0': return None
        if t == 'N': return None
        if t == 'E': return Ellipsis
        if t == 'i': return s.l()
        if t == 'I': v = struct.unpack_from('<q', s.b, s.i)[0]; s.i += 8; return v
        if t == 'f': n = s.byte(); v = s.b[s.i:s.i+n]; s.i += n; return float(v)
        if t == 'x': n = s.byte(); v = s.b[s.i:s.i+n]; s.i += n; return complex(v.decode())
        if t == 'l':
            n = s.l(); d = 0
            for k in range(abs(n)): d |= s.h() << (15*k)
            return -d if n < 0 else d
        if t == 's': n = s.l(); v = s.b[s.i:s.i+n]; s.i += n; return v
        if t == '(' or t == '[':
            n = s.l(); return tuple(s.obj() for _ in range(n))
        if t == '{':
            d = {}
            while True:
                k = s.obj()
                if k is None and s.b[s.i-1] == ord('0'): break
                d[k] = s.obj()
            return d
        if t == 'c':
            c = {}
            c['argcount'] = s.h(); c['nlocals'] = s.h(); c['stacksize'] = s.h(); c['flags'] = s.h()
            c['code'] = s.obj(); c['consts'] = s.obj(); c['names'] = s.obj(); c['varnames'] = s.obj()
            c['filename'] = s.obj(); c['name'] = s.obj(); c['firstlineno'] = s.h(); c['lnotab'] = s.obj()
            return c
        raise ValueError('type %r at %d' % (t, s.i))
def ops(code):
    i = 0; out = []
    while i < len(code):
        op = code[i]
        if op >= 90:
            arg = code[i+1] | (code[i+2] << 8); i += 3
        else:
            arg = None; i += 1
        out.append((op, arg))
    return out
if __name__ == '__main__':
    b = open(sys.argv[1], 'rb').read()
    co = R(b[8:]).obj()
    o = ops(co['code'])
    classes = {}
    stack = []
    for k, (op, arg) in enumerate(o):
        if op == 89:  # BUILD_CLASS
            # walk back: LOAD_CONST name, LOAD_NAME bases..., BUILD_TUPLE n, LOAD_CONST code, MAKE_FUNCTION, CALL_FUNCTION
            j = k - 1
            seq = o[max(0, k-12):k]
            name = None; bases = []
            # find BUILD_TUPLE
            bt = max(x for x in range(max(0, k-12), k) if o[x][0] == 102)
            n = o[bt][1]
            bases = [co['names'][o[bt-n+m][1]].decode() for m in range(n)]
            name = co['consts'][o[bt-n-1][1]].decode()
            cc = co['consts'][o[bt+1][1]]
            meths = [x.decode() for x in cc['names'] if isinstance(x, bytes)]
            classes[name] = {'bases': bases, 'names': meths}
    json.dump(classes, open(sys.argv[2], 'w'), indent=0)
    print(len(classes))
    for n in ['ShipClass', 'TGObject', 'PhysicsObjectClass', 'WeaponSystem']:
        print(n, classes.get(n, {}).get('bases'))
