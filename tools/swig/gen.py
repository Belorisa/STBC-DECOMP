# Generate src/game/AppWrap.cpp and the class headers from specs.json.
import json, re, os, sys, collections, struct
SD = 'build/swig/'
ROOT = 'C:/Recomp/stbc-wt/slot1/'
specs = json.load(open(SD + 'specs.json'))
types = json.load(open(SD + 'types.json'))
methods = json.load(open(SD + 'methods.json'))
exclude = set(json.load(open(SD + 'exclude.json'))) if os.path.exists(SD + 'exclude.json') else set()
BUILTIN = {'char', 'int', 'float', 'double', 'short', 'long', 'void', 'unsigned char', 'unsigned int', 'unsigned short',
           'unsigned long', '__int16', 'bool', 'FILE'}
SKIPCLS = re.compile(r'^Ni')


def base_of(t):
    return t[:-2] if t.endswith(' *') else None


def usable(s):
    if 'skip' in s or s['name'] in exclude:
        return False
    if s['cls'] is None:
        return False          # free functions: owner header unknown yet
    if SKIPCLS.match(s['cls']):
        return False
    for p in s['params'] + [s['ret']]:
        t = p[1]
        if p[0] == 'ptr':
            b = base_of(t)
            if b is None or not re.fullmatch(r'[A-Za-z_]\w*', b) or b in ('HWND', 'PyObject'):
                return False
    return True


use = [s for s in specs if usable(s)]
# one declaration per (class, method): drop name clashes with different signatures/targets
seen = {}
final = []
for s in use:
    key = (s['cls'], s['meth'])
    sig = (s['target'], json.dumps(s['params']), json.dumps(s['ret']), s['static'])
    if key in seen and seen[key] != sig:
        continue
    seen[key] = sig
    final.append(s)
use = final
byname = {s['name']: s for s in use}


def decl(t, name):
    return ('%s%s' % (t, name)) if t.endswith('*') else ('%s %s' % (t, name))


def swigtype(p):
    return 'SWIGTYPE' + p[2]


TYPEMAP_CHARP = '''    {
        if ($source == Py_None) {
            $target = NULL;
        } else if (PyString_Check($source)) {
            $target = PyString_AsString($source);
        } else {
            PyErr_Format(PyExc_TypeError, "Expected None or string, %s found", $source->ob_type->tp_name);
            return NULL;
        }
    }'''


TYPEMAP_OUT_CHARP = '''    {
        if (!$source) {
            $target = Py_None;
        } else {
            $target = PyString_FromString($source);
        }
    }'''


def defval(p):
    v = p[3]
    k = p[0]
    if k == 'float':
        f = struct.unpack('<f', struct.pack('<I', v))[0]
        for d in range(1, 10):
            t = '%.*g' % (d, f)
            if struct.unpack('<f', struct.pack('<f', float(t)))[0] == f:
                return fmtfloat(float(t))
        return fmtfloat(f)
    if k == 'double':
        return fmtfloat(v)
    if k == 'bool':
        return {0: 'false', 1: 'true'}[v]
    if k == 'cstr':
        if v == 0:
            return 'NULL'
        raise ValueError('string default')
    if v >= 0x80000000:
        v -= 0x100000000
    return str(v)


def fmtfloat(f):
    r = repr(f)
    if 'e' not in r and '.' not in r:
        r += '.0'
    return r


def wrapper(s):
    n = s['name']
    L = []
    L.append('// FUNCTION: STBC 0x%06X' % s['addr'])
    L.append('static PyObject *_wrap_%s(PyObject *self, PyObject *args) {' % n)
    loc = ['    PyObject *resultobj;']
    for i, p in enumerate(s['params']):
        if len(p) > 3:
            loc.append('    %s = %s;' % (decl(p[1], 'arg%d' % i), defval(p)))
        else:
            loc.append('    %s;' % decl(p[1], 'arg%d' % i))
    for i, p in enumerate(s['params']):
        if p[0] == 'ptr':
            loc.append('    PyObject * argo%d = 0;' % i)
        elif p[0] == 'bool':
            if len(p) > 3:
                loc.append('    int tempbool%d = (int) %s;' % (i, defval(p)))
            else:
                loc.append('    int tempbool%d;' % i)
        elif p[0] == 'cstr':
            loc.append('    PyObject * obj%d = 0;' % i)
    if s['ret'][0] != 'void':
        loc.append('    %s;' % decl(s['ret'][1], 'result'))
    L += loc
    L.append('    ')
    refs = []
    for i, p in enumerate(s['params']):
        refs.append({'ptr': '&argo%d', 'bool': '&tempbool%d', 'cstr': '&obj%d'}.get(p[0], '&arg%d') % i)
    L.append('    if(!PyArg_ParseTuple(args,"%s"%s)) return NULL;' % (s['fmt'], ''.join(',' + r for r in refs)))
    for i, p in enumerate(s['params']):
        if p[0] == 'ptr':
            L.append('    if ((SWIG_ConvertPtr(argo%d,(void **) &arg%d,%s,1)) == -1) return NULL;' % (i, i, swigtype(p)))
        elif p[0] == 'bool':
            L.append('    arg%d = (bool ) tempbool%d;' % (i, i))
        elif p[0] == 'cstr':
            if len(p) > 3:
                L.append('    if (obj%d)' % i)
            L.append(TYPEMAP_CHARP.replace('$source', 'obj%d' % i).replace('$target', 'arg%d' % i))
    args = ['arg%d' % i for i in range(len(s['params']))]
    if s['thiscall']:
        call = 'arg0->%s(%s)' % (s['meth'], ','.join(args[1:]))
    elif s['static']:
        call = '%s::%s(%s)' % (s['cls'], s['meth'], ','.join(args))
    else:
        call = '%s(%s)' % (s['meth'], ','.join(args))
    r = s['ret']
    if r[0] == 'void':
        L.append('    %s;' % call)
        L.append('    Py_INCREF(Py_None);')
        L.append('    resultobj = Py_None;')
    else:
        L.append('    result = (%s )%s;' % (r[1], call))
        if r[0] == 'ptr':
            L.append('    resultobj = SWIG_NewPointerObj((void *) result, %s);' % swigtype(r))
        elif r[0] in ('float', 'double'):
            L.append('    resultobj = PyFloat_FromDouble(result);')
        elif r[0] == 'cstr':
            L.append(TYPEMAP_OUT_CHARP.replace('$source', 'result').replace('$target', 'resultobj'))
        else:
            L.append('    resultobj = PyInt_FromLong((long)result);')
    L.append('    return resultobj;')
    L.append('}')
    L.append('')
    L.append('')
    return '\n'.join(L)


# ---- class headers
cls_methods = collections.OrderedDict()
for s in sorted(use, key=lambda s: s['addr']):
    cls_methods.setdefault(s['cls'], []).append(s)


from manual import MANUAL


SHADOW = json.load(open(SD + 'shadow.json'))


def bases_of(cls):
    return SHADOW.get(cls, {}).get('bases', [])


def tg_chain_ok(cls):
    # every ancestor is a game class (NetImmerse headers are not available yet)
    for b in bases_of(cls):
        if b.startswith('Ni') or not tg_chain_ok(b):
            return False
    return True


def header(cls, ms):
    man = MANUAL.get(cls, {})
    fwd = set()
    lines = []
    for s in ms:
        ps = s['params'][1:] if s['thiscall'] else s['params']
        for p in ps + [s['ret']]:
            if p[0] == 'ptr':
                b = base_of(p[1])
                if b not in BUILTIN and b != cls:
                    fwd.add(b)
        ptxt = ', '.join(p[1] for p in ps)
        ptxt = ' %s ' % ptxt if ptxt else ' void '
        rt = s['ret'][1]
        pre = 'static ' if s['static'] else ''
        lines.append('\t%s%s%s%s(%s);' % (pre, rt, '\t' if True else '', s['meth'], ptxt))
    guard = '__%s_H__' % cls.upper()
    out = ['/*', '===============================================================================', '',
           '\t%s.h' % cls, '',
           '\tThe members of %s that the SWIG wrappers %s_* call. Only what the' % (cls, cls),
           '\twrappers prove is declared: no data members and no virtual functions yet.', ''] + (
        man['note'] + [''] if 'note' in man else []) + [
           '===============================================================================', '*/', '',
           '#ifndef %s' % guard, '#define %s' % guard, ''] + man.get('pre', [])
    bs = bases_of(cls)
    inherit = bs and tg_chain_ok(cls)
    if inherit:
        for b in bs:
            out.append('#include "%s.h"' % b)
        out.append('')
    fwd -= set(bs)
    for f in sorted(fwd):
        out.append('class %s;' % f)
    if fwd:
        out.append('')
    if inherit:
        out.append('// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).')
        out.append('class %s : %s {' % (cls, ', '.join('public ' + b for b in bs)))
    else:
        if bs:
            out.append('// The SWIG interface derives %s from %s (NetImmerse, not declared yet).' % (cls, ', '.join(bs)))
        out.append('class %s {' % cls)
    out.append('public:')
    out += lines
    if 'members' in man:
        out.append('')
        out += man['members']
    out.append('};')
    out.append('')
    out += man.get('post', [])
    out.append('#endif /* !%s */' % guard)
    out.append('')
    return '\n'.join(out)


allcls = set(cls_methods)
todo = list(allcls)
while todo:
    c = todo.pop()
    if tg_chain_ok(c):
        for b in bases_of(c):
            if b not in allcls:
                allcls.add(b); todo.append(b)
for c in allcls:
    cls_methods.setdefault(c, [])
for cls, ms in cls_methods.items():
    open(ROOT + 'src/game/%s.h' % cls, 'w', newline='\r\n').write(header(cls, ms))

# ---- the unit
rt = open(SD + 'rt.cpp').read()
o = [rt.rstrip('\n'), '', '/* -------- TYPES TABLE (BEGIN) -------- */', '']
for t in types:
    o.append('#define  SWIGTYPE%s swig_types[%d] ' % (t['name'], t['i']))
o.append('static swig_type_info *swig_types[%d];' % (len(types) + 1))
o += ['', '/* -------- TYPES TABLE (END) -------- */', '', '',
      '/*-----------------------------------------------', '              @(target):= App.so',
      '  ------------------------------------------------*/', '#define SWIG_init    initApp', '',
      '#define SWIG_name    "App"', '']
for cls in sorted(cls_methods):
    o.append('#include "%s.h"' % cls)
o += ['', '#ifdef __cplusplus', 'extern "C" {', '#endif']
for s in sorted(use, key=lambda s: s['addr']):
    o.append(wrapper(s))
o.append('static PyMethodDef AppMethods[] = {')
for n, f, fl, v in methods:
    if n in byname:
        o.append('\t { "%s", _wrap_%s, METH_VARARGS },' % (n, n))
o.append('\t { NULL, NULL }')
o.append('};')
o += ['', '#ifdef __cplusplus', '}', '#endif', '', '/* -------- TYPE CONVERSION AND EQUIVALENCE RULES (BEGIN) -------- */', '']
for t in types:
    eq = ''.join('{"%s"},' % e[0] for e in t['eq'])
    o.append('static swig_type_info _swigt_%s[] = {{"%s", 0, "%s"},%s{0}};' % (t['name'], t['name'], t['str'], eq))
o.append('')
o.append('static swig_type_info *swig_types_initial[] = {')
for t in types:
    o.append('_swigt_%s, ' % t['name'])
o.append('0')
o.append('};')
o += ['', '', '/* -------- TYPE CONVERSION AND EQUIVALENCE RULES (END) -------- */', '',
      'static swig_const_info swig_const_table[] = {', '{0}};', '',
      'static PyObject *SWIG_globals;', '#ifdef __cplusplus', 'extern "C" ', '#endif',
      'SWIGEXPORT(void) initApp(void) {', 'PyObject *m, *d;', 'int i;', 'SWIG_globals = SWIG_newvarlink();',
      'm = Py_InitModule("App", AppMethods);', 'd = PyModule_GetDict(m);',
      'for (i = 0; swig_types_initial[i]; i++) {', 'swig_types[i] = SWIG_TypeRegister(swig_types_initial[i]);', '}',
      'SWIG_InstallConstants(d,swig_const_table);', '}', '']
open(ROOT + 'src/game/AppWrap.cpp', 'w', newline='\r\n').write('\n'.join(o))
print('wrappers', len(use), 'classes', len(cls_methods))
