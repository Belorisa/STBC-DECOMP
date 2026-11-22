import json, re, collections, sys
sys.path.insert(0, '.')
from infer import infer, wr, starts, types
specs = {s['name']: s for s in json.load(open('specs.json'))}
ex = set(json.load(open('exclude.json')))
shadow = json.load(open('shadow.json'))
classes = set(t['str'][:-2] for t in types if t['str'] and t['str'].endswith(' *'))
cm = collections.defaultdict(lambda: {'direct': [], 'virtual': {}, 'inline': {}, 'other': [], 'static': 0})
for k, (f, n) in enumerate(wr):
    s = specs[n]
    ins, fmt, ev = infer(f, n, starts[k + 1])
    pre = n[4:].split('_')[0] if n.startswith('new_') else (n[7:].split('_')[0] if n.startswith('delete_') else n.split('_')[0])
    cls = pre if pre in classes or pre in shadow else '(free functions)'
    meth = n[len(pre) + 1:] if cls != '(free functions)' else n
    if n.startswith('new_'): meth = '(new)'
    if n.startswith('delete_'): meth = '(delete)'
    c = cm[cls]
    if 'skip' not in s and n not in ex:
        c['direct'].append(meth)
        if s['static']: c['static'] += 1
        continue
    icalls = [i for i in ins if i.mnemonic == 'call' and not re.fullmatch(r'0x[0-9a-f]+', i.op_str)]
    if icalls:
        m = re.search(r'\[(e\w\w)(?: \+ (0x[0-9a-f]+|\d+))?\]', icalls[0].op_str)
        if m:
            slot = int(m.group(2) or '0', 0) // 4
            c['virtual'].setdefault(slot, []).append(meth)
            continue
    convs = [e for e in ev if e[0] == 'conv']
    calls = [e for e in ev if e[0] == 'call']
    if convs and not calls:
        last = convs[-1][1]
        off = None
        for i in ins[last + 1:]:
            m = re.search(r'\[(e(?!sp)\w\w) \+ (0x[0-9a-f]+|\d+)\]', i.op_str)
            if m:
                off = int(m.group(2), 0); break
            m = re.search(r'\[(e(?!sp)\w\w)\]', i.op_str)
            if m and i.mnemonic in ('mov', 'fld', 'fstp', 'movsx', 'movzx'):
                off = 0; break
        if off is not None:
            c['inline'].setdefault(off, []).append(meth)
            continue
    c['other'].append(meth)
json.dump(cm, open('classmap.json', 'w'), indent=0)
tot = collections.Counter()
rows = []
for cls in sorted(cm, key=lambda c: -(len(cm[c]['direct']) + sum(map(len, cm[c]['virtual'].values())) + sum(map(len, cm[c]['inline'].values())) + len(cm[c]['other']))):
    c = cm[cls]
    nv = sum(map(len, c['virtual'].values())); ni = sum(map(len, c['inline'].values()))
    tot['direct'] += len(c['direct']); tot['virtual'] += nv; tot['inline'] += ni; tot['other'] += len(c['other'])
    rows.append((cls, len(c['direct']), nv, ni, len(c['other']), ' '.join('%d:%s' % (k, '/'.join(sorted(set(v)))) for k, v in sorted(c['virtual'].items(), key=lambda x: int(x[0])))))
print(tot, len(cm))
with open('classmap.txt', 'w') as f:
    for r in rows:
        f.write('%s\tdirect=%d virtual=%d inline=%d other=%d\tslots %s\n' % r)
for r in rows[:15]: print(r[:5], r[5][:150])
