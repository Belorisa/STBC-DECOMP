# SWIG App wrapper bootstrap

These scripts generated the first version of `src/game/AppWrap.cpp` and the game class headers in `src/game/`
from the retail wrappers (lane `swig-app`). They are kept as provenance. They are not part of the build.

- `infer.py` reads each retail wrapper and writes a spec (argument formats, callee, typemaps, defaults):
  `specs.json`, `types.json`, `methods.json`.
- `gen.py` writes the unit and the headers from the specs, `rt.cpp` (the SWIG 1.3.6 + r1429 runtime) and
  `manual.py` (hand-written header additions); `shadow.json` is the class hierarchy read from the shadow classes
  in the game's `scripts/App.pyc` with `pyc152.py` (a Python 1.5.2 marshal reader).
- `classmap.py` builds `classmap.json`/`classmap.txt`: per class, the direct, virtual (with vtable slot) and
  inline-accessor wrappers. Heuristic: a few slot entries come from calls through other objects.
- `addrefs.py` appends referent claims to `config/symbols.txt` from REFERENT rows.

Paths inside the scripts are those of the session that ran them (`SD`, `ROOT`). **Since the merge, the source in
`src/game/` is the source of truth and is edited by hand: never re-run `gen.py` over it.** The long-term
form of this unit is the SWIG interface (`App.i`), regenerated with SWIG 1.3.6 + r1429.
