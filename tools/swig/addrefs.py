import re,sys
SYM='C:/Recomp/stbc-wt/slot1/config/symbols.txt'
have={l.split()[0] for l in open(SYM) if l.strip() and not l.startswith('#')}
haveaddr={}
for l in open(SYM):
    p=l.split()
    if len(p)>1 and not l.startswith('#'): haveaddr.setdefault(int(p[1],16),[]).append(p[0])
cur=None; out={}
for l in open(sys.argv[1]):
    m=re.match(r'\s+(\S+)\s+[\d.]+\s+0x([0-9A-F]+)\s+(\S+)',l)
    if m: cur=m.group(3) if m.group(1)=='REFERENT' else None; continue
    m=re.search(r'unknown referent (\S+) \(retail 0x([0-9A-F]+)\)',l)
    if m and cur and m.group(1) not in have:
        out.setdefault(m.group(1),(int(m.group(2),16),set()))[1].add(cur.replace('_wrap_',''))
lines=[]
for k,(a,w) in sorted(out.items(),key=lambda x:x[1][0]):
    if a in haveaddr: print('ADDRESS ALREADY NAMED',k,hex(a),haveaddr[a]); continue
    lines.append('%s 0x%08x   # evidence: SWIG wrapper %s\n'%(k,a,', '.join(sorted(w))))
if '--write' in sys.argv:
    open(SYM,'a').writelines(lines)
print(len(lines),'new')
