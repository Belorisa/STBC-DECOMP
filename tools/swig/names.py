import re,glob,json
names={}
for line in open('C:/Recomp/stbc-wt/slot1/config/symbols.txt'):
    m=re.match(r'(\S+)\s+0x([0-9a-fA-F]+)',line)
    if m and not line.startswith('#'): names[int(m.group(2),16)]=m.group(1)
for f in glob.glob('C:/Recomp/stbc-decomp/src/python/**/*.c',recursive=True)+glob.glob('C:/Recomp/stbc-wt/slot1/src/python/**/*.c',recursive=True):
    L=open(f,encoding='latin-1').read().split('\n')
    for i,l in enumerate(L):
        m=re.match(r'\s*//\s*(FUNCTION|DATA):\s*STBC\s+0x([0-9A-Fa-f]+)',l)
        if m:
            for j in range(i+1,min(i+8,len(L))):
                s=L[j].split('//')[0]
                mm=re.search(r'(\w+)\s*\(',s) if m.group(1)=='FUNCTION' else re.search(r'(\w+)\s*(=|;|\[)',s)
                if mm and not s.strip().startswith('/*'):
                    names.setdefault(int(m.group(2),16),'_'+mm.group(1)); break
json.dump({hex(k):v for k,v in names.items()},open('build/swig/names.json','w'),indent=0)
print(len(names))
for a in [0x74c6e0,0x74fa70,0x7506c0,0x74e310,0x752e80,0x95dd20,0x74d280]: print(hex(a),names.get(a))
