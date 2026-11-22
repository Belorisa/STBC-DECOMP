# disposable experiment: resolve .bss statics per symbol, run match, restore the tool
cd /c/Recomp/stbc-wt/slot1
python - <<'PY'
p='tools/decomp.py'
s=open(p).read()
s=s.replace('''        base = hint - sym.value
        if not self.set_base(u, sym.section, base):''','''        base = hint - sym.value
        if u.obj.section(sym.section).is_bss and sym.name != u.obj.section(sym.section).name:
            return hint, None
        if not self.set_base(u, sym.section, base):''')
open(p,'w').write(s)
PY
python tools/decomp.py match game/AppWrap > "$1" 2>&1
git checkout -q tools/decomp.py
tail -2 "$1"
