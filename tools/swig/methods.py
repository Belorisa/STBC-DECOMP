import sys,struct,json
sys.path.insert(0,'C:/Recomp/stbc-wt/slot1/tools')
import pe
img=pe.Image(open('C:/Recomp/stbc-wt/slot1/orig/stbc.exe','rb').read())
def cstr(va):
    b=img.read(va,256)
    if b is None: return None
    n=b.find(b'\0')
    if n<=0: return None
    try: s=b[:n].decode('ascii')
    except: return None
    return s if s.isprintable() else None
# walk backward and forward from table entry 0x8eeed8
start=0x8eeed8
def ent(va):
    a,f,fl=struct.unpack('<IIII',img.read(va,16))[:3]
    return a,f,fl
va=start
while True:
    a,f,fl=ent(va-16)
    if not(0x401000<=f<0x888000) or cstr(a) is None: break
    va-=16
first=va
out=[]
va=first
while True:
    a,f,fl=ent(va)
    if a==0: break
    out.append((cstr(a),f,fl,va)); va+=16
print(hex(first),hex(va),len(out))
json.dump([(n,f,fl,v) for n,f,fl,v in out],open(sys.argv[1],'w'))
fs=sorted(o[1] for o in out); print(hex(fs[0]),hex(fs[-1]))
print(out[:5],out[-5:])
