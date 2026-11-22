# Toolchain, worktrees and Ghidra sync

Read this when a unit doesn't compile, when its flags or compiler are in question, when setting up a worktree, or
when syncing names into Ghidra. `AGENTS.md` is the project guide; this file holds the details it points to.

## Worktrees

`orig/`, `tools/vc6/` and `ref/` are not committed. A worktree needs `orig/stbc.exe` copied in,
and `tools/vc6` as a junction to the main checkout's copy
(`cmd /c mklink /J tools\vc6 C:\Recomp\stbc-decomp\tools\vc6`). Reference trees are read from the main
checkout: `C:\Recomp\stbc-decomp\ref\Python-1.5.2` is pristine upstream
(`https://www.python.org/ftp/python/src/py152.tgz`, SHA-256 `c6651472...ab6265`).

To remove a worktree, delete the `tools\vc6` junction first (`cmd /c rmdir <worktree>\tools\vc6`, which removes the
link only), then `git worktree remove --force <worktree>`: a recursive delete that follows the junction empties the
main checkout's compiler.

Don't use `git stash` in a worktree: every worktree shares one stash list, so a `git stash pop` after a stash that
saved nothing applies another lane's entry. Commit, or copy the files aside, instead.

## Object cache

`build`, `match` and `diff` reuse an object from `build/objcache` (per checkout) when its fingerprint matches: the
compiler, its arguments, the unit's source and every project file it reaches through `#include` (forced includes
too, and includes inside `#if` blocks, so the set errs on the side of recompiling). Files outside the repository and
the VC6 headers aren't followed. Entries unused for 14 days are pruned (a sweep at most once a day); deleting the folder is always safe. The gate
never reads the cache: it compiles every unit afresh into `build/gate`.

The rule checker caches the same way. A file's active lines (`cl /E`) are kept in `build/objcache` under the same
fingerprint (`.lines`), and each function's findings in `build/rulecache.pkl`, keyed by a digest of everything the
check reads: `tools/rules.py` itself, the definition's text and line, the module's settings, the pristine upstream,
the active lines over the definition, the duplicated types and its class's vtable findings. Deleting either is safe,
and the gate reads neither: it preprocesses and checks every function afresh.

Three more files in `build/` only save repeated reading, and deleting any of them is safe: `includes.pkl` (each
file's digest and resolved `#include`s, valid while the file keeps its timestamp and size and no file under `src/`
was added or removed), `typedefs.pkl` (the duplicated-type table, valid while no source file changed) and
`similar_cache.pkl` (retail disassembly and features for `similar.py`). A cached object already in `build/obj` with
the cache entry's timestamp and size isn't copied again.

## Compilers and flags

- MSVC 6.0 SP5: `cl` 12.00.8804 with `c1`/`c1xx`/`c2` build 8966, matching the Rich header (`README.md`). The
  portable install in `tools/vc6` is driven by `tools/vc6.py`. Nothing is linked: the gate splices the functions the
  comparator found exact into a copy of the retail image (`AGENTS.md`, "What a MATCH does not prove").
- Flags live in `config/project.json`, per source directory (longest prefix wins), with per-unit `overrides`. A
  module may set `"compiler"` to build with another VC6 compiler instead of SP5: `"rtm"` (`tools/vc6/_rtm_backup/BIN`,
  12.00.8168) or `"sp4"` (`tools/vc6/_sp4/BIN`: `c1`/`c1xx` 8867 and the Enterprise `c2` 8799, which stamps objects
  with the build 8799 that the Rich header gives for 147 C++ objects). A single unit may set its own compiler with
  `"unit_compiler"`: `"pp"` (`tools/vc6/_pp/BIN`) is SP5's front end with the Processor Pack back end, `c2` 13.00.9044
  (SP5's `cabs/os/system/vcpp.dll`, renamed `C2.DLL`). It builds the README's four "prod 49, build 9044" objects, the
  sound units NiListener, NiSoundSystem, NiSource and NiProvider_Info: their EH tables put the unwind map first, they
  tail-jump, and their float compares test for NaN.
  - `src/python`: `/O2 /W3 /MT`, defines `NDEBUG WIN32 _WINDOWS USE_DL_EXPORT MS_NO_COREDLL` (a static build).
    `/MT`: errno is read through `_errno()` and `getc` is a call. `MS_NO_COREDLL`: `_PyObject_NewVar` takes two
    arguments and allocates itself.
  - `src/gamespy` (the GameSpy SDK, C): `/O2 /G6 /W3 /MT`, defines `NDEBUG WIN32 _WINDOWS`, includes `src/gamespy`,
    `src/gamespy/goaceng`, `src/gamespy/qr` (the game module includes the same three). `/G6` is the SDK's own: without
    it darray.c matched 5 of 19 functions, with it 19 (accepted on that evidence, user's ruling, 2026-11-16). The SDK calls the CRT's malloc and free, not the pools (no
    `TGMemory.h`). Unchanged lines of `upstream/gamespy-ign-2011/` (darray, hashtable) are exempt from R2, R4, R17.
  - Per-unit overrides (an `/I` override is relative to the repository root): `python/PC/import_nt` adds
    `/Isrc/python/Python` for `importdl.h`; `python/Modules/getbuildinfo` defines `DATE` and `TIME` as the
    original build's `__DATE__`/`__TIME__` ("Jan 17 2002", "03:24:04", the retail strings at 0x975E68 and
    0x975E74), which upstream `getbuildinfo.c` takes in place of the compiler's own.
  - `src/game`: `/O2 /W3 /GX /MT /FITGMemory.h`, C++, no `/GR`; defines as `src/python` (the wrappers include
    `Python.h`). Proven by the SWIG runtime and 1371 wrappers; `/GX` is required by the EH frames of inline header
    functions emitted in the unit (not byte-proven yet). `/FITGMemory.h` models the game-wide prelude that routes
    `malloc`/`free` to `g_kPoolManager`. `game/AppWrap` adds `/DSWIG_GLOBAL /DSTATIC_LINKED` (another unit calls
    the SWIG runtime). `game/TGFontGroup` adds `/Ob2`: its static initialisers have the `/Ob2` shape, the only game
    unit's that do (each `<iostream>` initialiser merged with its atexit at 0x743DB0 and 0x743DE0, the locale pair one
    function at 0x744670, the NiMain registrations without `jmp` stubs); with it and `<iostream>` its 9 functions
    still match, all 75 initialisers match, and 52 of them fall in retail's order against 47 (`comdat.py order`).
  - `src/netimmerse`: `/O2 /Ob2 /W3 /GX /MT` (`/Ob2` proven by NiMatrix3's inlining). NetImmerse shipped as
    separate libraries (Gamebryo 1.1.2's `sdk/*.LIB` exports name each class's library), and the inline level is a
    per-library setting, written as per-unit `/Ob1` overrides because the tree isn't split by library:
    - **NiAnimation** (the controllers, the keys, the key data, NiAnimBlender, NiKeyframeManager): `/Ob1`. Retail calls
      `ReplaceAnim` and constructors out of line that `/Ob2` inlines, and every unit of the library matches under
      `/Ob1`, the base key classes and NiSequenceStreamHelper included.
    - **NiOldParticle** (NiGravity, NiParticleBomb, NiParticleSystemController, the two colliders): `/Ob1`.
    - **NiMain**, **NiCollision**, **NiSystem** and the renderer: `/Ob2`. `/Ob1` on NiCollision loses matches in
      NiOBBNode, NiOBBRoot and NiTriIntersect.
    - **The `<iostream>` run's shape names a unit's inline level** (every NetImmerse unit includes `<iostream>`
      through `NiRTLib.h`): five functions (the locale guard and two init/exit pairs) under `/Ob2`, ten (the guard,
      its atexit and two quads) under `/Ob1`. NiForce and NiBox are `/Ob1` by their run's shape.
    - Two overrides don't follow their library, and rest on their own unit's bytes alone: **NiOBBLeaf** (`/Ob1` in
      NiCollision) and the Miles audio units **NiListener, NiSoundSystem, NiSource** (`/Ob1`, where NiProvider_Info, of
      the same Processor Pack build, loses a match under it). Open: the evidence for these is the outcome, not the
      library, and the run shapes disagree, so the flags stay as they are:
      - NiOBBLeaf's run (0x827ED0-0x827F4F, its registrations from 0x827F50) has the `/Ob2` shape: no `jmp` stubs.
        Under `/Ob2` its stubs match (renumbered one up) but FindSegmentContact (0x8266B0) is expanded into its one
        caller, FindEdgeContact, where retail calls it; `/Ob2` with `#pragma auto_inline( off )` from
        FindSegmentContact on matches every function and stub, but no evidence shows that pragma.
      - NiProvider_Info's run after its code (0x790E50-0x790FFA) has the `/Ob1` shape (the guard's `jmp`, the quads'
        call-and-`jmp` pairs, a `jmp` stub per registration). Under `/Ob1` its destructor calls Close and a base
        destructor out of line where retail expands both, and Close (0x790E00) sits in source order, an ordinary
        function auto-inlined, not a header inline's copy. `netimmerse/renderer/NiTextureManager` adds `/Isrc/game /FITGMemory.h`: it is the only NetImmerse unit
    with C allocation calls, and they go to the pool manager (no NetImmerse code calls the CRT's malloc/calloc/free).
  - `src/crt`: the floating-point units, `crt/fpexcept`, `crt/ieeemisc` and `crt/util` (user's ruling, 2026-11-15) and
    the math functions `crt/floor`, `crt/ceil`, `crt/frexp`, `crt/modf`, `crt/ldexp`, `crt/fabs` and `crt/hypot`, add
    `/Op` (the ruling extended by the user, 2026-11-16, to the floating-point CRT units whose bytes show this pattern). The evidence is their bytes alone: every function of them matches with it, with FP results stored to frame
    temporaries and reloaded and FP comparisons reading their operands from memory (floor: 91.5 without it). VC6's CRT makefiles
    don't cover the floating-point tree, so there is no build record. `/Oa` was rejected: nothing else supports it.
  - `"auto_pch"` (a module's switch; on for `src/netimmerse` by the user's ruling of 2026-11-18 night, off for
    `src/game` since the ruling of 2026-11-20: the game units fit retail's order better without it; a module's
    `"auto_pch_units"` list turns it on for single units, under the ruling's two proofs): build the module with VC6's automatic precompiled header, `/YX`, as the Developer Studio projects of the
    time did. It is on because retail's `_$E` stub and inline-copy placement fits it (an explicit `/Yu` contradicts
    it): over the whole tree it gains MetaVolumePolygonizer::CreateUnknown138, ViewScreenNode::SetOffTexture,
    PositionOrientationProperty::Unknown689830, NiBezierRectangle3::Unknown84ACF0 and NiFile::NiFile, and loses
    PhaserBank::GetArcPosition and GameSpy::ServerListCallBack (accepted losses, PARKED). The unit's leading include
    block is precompiled, and that changes where the compiler emits the headers' inline copies relative to the unit's
    own functions, which decides when an inline is known not to throw (an EH state) and where its out-of-line copy
    lands. Under the switch `cl_args` adds `/YX /Fp<pch>`. Each compile creates its `.pch` afresh beside its object
    (`build/obj/<unit>.pch`, `build/gate/obj/<unit>.pch`) and deletes it afterwards, so parallel compiles never share
    one. A `.pch` left by another compile is never used (VC6 recreates a `.pch` whose include list differs from the
    unit's, a shorter prefix included, so a shared project `.pch` builds each unit as its own would). The fingerprint
    covers the flag through the arguments (with the default path `build/pch/<unit>.pch`); the `.pch` content follows
    from the headers it already digests. `cl /E` runs without `/YX` (`cl_args(..., pch=False)`). The trace tools
    (`c2trace`, `c2dbg`, `framemap`, `ctxscan`, `c2il`, `ghidra/export_types.py`) pass the same flags with a `.pch`
    in their own work folders; `trial.py` compiles every variant of a `/YX` unit, the base included, from the scratch
    copy of the source, because the preprocessed text no longer shows where the precompiled block ends. The
    switch is the user's ruling (`docs/rulings.md`, "Build flags and precompiled headers").
  - `/Z7` and `/Zi` can change codegen with this compiler (3 of 41 functions in `listobject.c` were once seen to): never
    add them to a unit's flags. `framemap.py` compiles a diagnosis copy with `/Z7` under `build/framemap/`, never
    gated, and compares its code with the gated object's function by function (it warns when they differ).
- Codegen seen in game code: frame-pointer omission, `/GX` EH frames, 16-byte function alignment filled with
  `0x90`, string literals in `.data` (`/Gf`).

## Report and lead tools

The verbs `AGENTS.md`'s adapter table doesn't list: reports and leads a lane reaches for when its task needs one. Each is a report unless its row says otherwise.

| verb | meaning | this project's command |
|---|---|---|
| `c2trace <function>` | the real back end's decisions for one function (an inline, register or frame wall becomes arithmetic): inline sites in visiting order with cost, budget left and verdict; `--regs` register candidates (named), priorities and choices; `--frame` the packer's locals, slots and offsets; `--cost` the front end's IL tuple count per statement, which is the inline cost (`--worth N`: the statements worth N); under each budget verdict what flips it; `--whatif` replays the inline arithmetic with a site added or removed or another own cost; `--retail` marks the callees whose out-of-line count differs from retail's | `python tools/c2trace.py 0x59A2C0 [--regs] [--frame] [--cost [--worth N]] [--whatif site=K,+1\|-1\|own=+N] [--retail]`, or a scratch `.cpp` and a function name (rules: `docs/c2-backend.md`) |
| `c2order <function>` | the deciding site of a schedule or operand-order difference: the first divergence's kind (reorder, x87 swap, base and index, register, float copy, frame); for a reorder the scheduler region and nodes, then the real scheduler rerun in retail's input order: INPUT ORDER (the ties and source lines to move) or NOT (the chain, a no-code node on it, the height, or a memory pair retail keeps in order); `triage` prints the kinds only | `python tools/c2order.py 0x6DC490 [--verbose]`, `python tools/c2order.py triage <addr>... \| --group G [--open]` (rules: `docs/c2-backend.md`, "Scheduling") |
| `callset` | which inline callees retail and ours disagree on across the open functions: the header bodies to check first | `python tools/callset.py [--min-score N] [--callee NAME\|ADDR]`, `callset.py fn <addr>` |
| `sdkdebug` | the SDK's debug NetImmerse 3.1 (AnimOnly.exe): asserts with the original file and line, the original files' function runs, pairs with our functions, our units compiled at /Od against its bodies (an inline's or a statement's spelling the release bytes can't settle) | `python tools/sdkdebug.py [asserts \| files \| map \| fn <addr> \| od \| cmp <addr> \| find <scratch.cpp> \| rtti]` |
| `sibling` | other builds of the same code as siblings (the demo, the SDK's ModelPropertyEditor, DamageTool, STBCExporter): paired functions side by side, call sets that differ, sibling-only helpers; per open function, which siblings side with retail's call set against ours; DamageTool's decorated names against ours | `python tools/sibling.py [callsets [--open] \| new \| fn <addr>] [--build demo\|mpe\|damagetool\|exporter\|all]`, `open [--strong] [--unit RE]`, `syms [ADDR ...]` |
| `ctxscan` | a frame/register flip that moves with the declarations a unit sees: the counts of dummy declarations before the function that give retail's bytes (a report; it never edits `src/`) | `python tools/ctxscan.py 0x554040 [--top] [--range 0 63]` |
| `dupescan` | unmatched functions and gaps whose retail bytes (relocations masked) equal an already-matched function's: free matches | `python tools/dupescan.py [--kind diff\|claimed\|gap] [--tier exact\|shape] [--json F]` |
| `nocall` | retail copies of a unit's own helpers that nothing calls (every caller inlined them), with the unit's open callers whose retail body holds the helper's referents: callers that should call the helper instead of writing it out | `python tools/nocall.py [--unit U] [--all] [--gaps] [--json F]` |
| `c2il` | whether two spellings give identical IL, the same code, or different code (proves a readability edit byte-neutral) | `python tools/c2il.py compare <unit\|0xADDR> A [B]` |
| `linkcheck` | a link dry-run over the compiled objects: duplicate definitions and unresolved externals (report only) | `python tools/linkcheck.py [--top N] [--all]` |
| `clanglint` | clang-cl as a linter over the game and NetImmerse units (VC6's headers, each unit's defines): a function that falls off its end, an integer used as a pointer, a member hiding a base virtual, delete without a virtual destructor, constructs clang rejects (report only; VC6 compiles the bytes) | `python tools/clanglint.py [UNIT ...] [--module DIR] [--list] [--flag F] [--all] [--json F]` |
| `eaxcheck` | functions declared `void` whose callers read EAX after the call in retail: a return type modelled wrong (report only) | `python tools/eaxcheck.py [--callee ADDR] [--json F]` |
| `linkready` | how far a real link of the objects is from retail's layout: each unit's code sections against retail's order and offsets, its `.data`/`.rdata`/`.bss` against retail's placement, the data's object order against the code's, and the externals a link couldn't resolve (report only) | `python tools/linkready.py [--unit U] [--list text\|data\|order\|unresolved] [--json F]` |
| `layoutcheck` | our class layouts against retail's code: member access widths, signedness and float/integer, member calls by class, allocation sizes, vtable lengths (report only) | `python tools/layoutcheck.py [--class C] [--check member\|size\|vtable] [--weak] [--json FILE]` |
| `vtslots` | each declared virtual against retail's vtable slot by slot: our slots from CodeView and a vcall-thunk probe (`&Class::Method` compiled with the owning unit's flags), retail's slot targets named by the annotations; moved, wrong or missing overrides, classes whose slots differ between units, and unidentified slot targets with the name our slot gives them (report only) | `python tools/vtslots.py [--class C] [--all] [--leads] [--no-probe] [--json F]` |
| `advisory` / `finish` | the readability backlog: advisory findings by rule, unit or class; the finished units it can be cleaned in | `python tools/openwork.py advisory [RULE] [UNIT\|Class]`, `finish [N]` |
| `names` | the evidence a name could come from, for each placeholder of a class or unit (a report; it renames nothing) | `python tools/names.py ShipClass [--direct]`, `--summary [--list direct]` |
| `levercheck` | the lever entries whose cited examples disagree with the functions' status, and whether a lever audit is due (every 9 lane merges) | `python tools/levercheck.py [due] [--json F]` |
| `units` | unit boundaries against retail: static runs inside a unit's code, units split by others' code, definition order | `python tools/units.py [boundaries\|scattered\|order] [UNIT]` |
| `comdat <unit>` | where retail kept a unit's inline and template copies, and the unit's emission order against retail's | `python tools/comdat.py <unit> [lo hi]`, `comdat.py layout <unit>`, `comdat.py order <unit>` |
| `statics` | renumber and place `_$E` static initialisers | `python tools/statics.py renumber`, `place-all [--write]` |
| `annotate-inlines` | annotate claimed functions already inline in their header | `python tools/annotate_inlines.py [--apply]` |
| `trace` | runtime leads | `python tools/trace.py` |

## Tools in detail

The options and procedures behind `AGENTS.md`'s adapter table.

- **`diff`** (`decomp.py diff <function>... [--list FILE] [--summary] [--size N]`): one function prints its
  side-by-side diff. Several (positional, and one address or name per line of `--list FILE`, `#` starts a comment)
  share one load: the lock is taken once, the units that hold them are compiled once, and each diff follows a
  `=== <function> ===` header (a function that isn't annotated prints its error and the rest go on; the exit code is
  1). `--summary` prints one line per function instead: address, name, status, score, the first differing
  instruction pair (ours `vs` retail, `-` for none) and the wall class `walls.py` reads. A session costs about 7
  seconds of loading and each further function under half a second, so triage lists go through `--list`. The load
  reads `build/scan.pkl` (each file's annotations, kept while its timestamp and size and `decomp.py` are
  unchanged; deleting it is always safe).

- **`packet`** (`tools/packet.py <addr> [<addr>...] [-o build/packets]`): the source, the PARKED note, the card, the
  diagnosed diff, the closest levers, the callees by name, a matched function of the unit, the class declaration and
  Ghidra's decompilation; for a claimed function, its declaration and the retail assembly with names. For an
  address with no annotation and no claim, a gap packet: the extent (Ghidra's function there, else a scan to the
  first return before the next known address), retail's assembly with names, its callers (direct calls, and stored
  pointers outside `.text`: vtable slots), its callees, the annotated functions before and after it with their
  units, the closest matched functions and Ghidra's decompilation. Lever entries whose `{asm: ...}` key occurs in the
  function's diff are quoted first (docs/levers/README.md, "How entries are written"). Several
  addresses in one call share one load (about a second each): write a lane's packets in one go.
- **`c2trace`** (`tools/c2trace.py <unit|0xADDR|scratch.c[pp]> [name] [--regs] [--frame] [--sched] [--cost [--worth N]]
  [--operands] [--addr] [--all] [--list] [--raw] [--flags "..."] [--header-override HEADER=FILE ...] [--margin N] [--whatif EDIT ...] [--retail]
  [--recorded JSON]`): compiles the unit with its own flags under a small Win32 debugger (`tools/c2dbg.py`: int3 bytes written
  into CL.EXE's memory once C2.DLL loads, so `tools/vc6` is never modified) and prints what the back end decided for
  the functions whose undecorated name contains `name` (an address takes its annotation's unit and name). Default:
  the inline driver, one line per call site in the order it visits them (depth first, IL order) with the callee's
  front-end cost, the budget left, the verdict and its reason, and each expansion's nested share
  (`left / sites not yet visited`) and what it consumed; a site's `inline_depth` is shown when it isn't 8. `--regs`
  adds the global register allocator: each candidate as it is popped (its source name when it stands for a local or
  parameter, kind and id, priority, spill cost, references, allowed registers) and the register given with the costs
  the chooser compared; a candidate without a name is the back end's own (an induction variable, a common
  subexpression). `--frame` adds the frame packer: the locals left in memory in the order it visits them (name, size,
  references), the slots it leaves (size, references, sort key, members) and each local's final offset, or "nothing
  to pack" when every local is fixed. `--cost` prints the front end's cost of the function, the IL tuples C1XX
  emitted for it (its opcodes counted, then one line per statement): compare two spellings of a body to see which
  costs more; `--worth N` then lists the statements worth exactly N tuples and the adjacent pairs summing to N (the
  statements are cut at a discard, return, jump, branch or sequence point: a heuristic, the IL has no statement
  marks), the candidates when the trace says a site needs N units. Under a site the inline trace prints what its
  verdict needs to flip (`-> rejected: needs >= N more units here (...)` at every budget rejection, `-> inlined: flips
  with >= N fewer units here (...)` at an expansion whose margin is below `--margin`, default 10): at the top level
  in own-cost units, at a nested level as the share it comes from (`>= X more left at [i/n] above, or K fewer sites
  from there on`; `K more sites after it there`). Every inline trace is recorded in
  `build/c2trace/<unit>--<name>.trace.json`. `--whatif` replays the driver's arithmetic on that record without
  compiling the edit: `site=K,+1[,cost=N]` adds a site before position K (`K.J` inside the expansion of site K; cost
  24, free, unless given), `site=K,-1` removes site K, `own=+N`/`own=-N` changes the function's own cost; it lists
  the sites whose verdict flips (a site that flips to expanded takes its copy's sites from another expansion of the
  same callee in the record, else says they are unknown) and first warns if replaying the record disagrees with the
  compiler. `--retail` (an address target) sets the trace's out-of-line sites against retail's calls per callee from
  `tools/callset.py` (callees paired by decorated name), marks the callees whose counts differ and names the sites of
  ours that can account for the difference (the tightest expansions when retail calls more, the nearest rejections
  when it calls fewer); a callee without an address shows `?`, and retail's calls to code without an identity are
  listed beside them as leads. It reads `build/report.json` and `build/obj`, so `match` the unit first (a count that
  disagrees with the trace is flagged as a stale object). `--recorded JSON` runs `--whatif`/`--retail` on a saved
  record. A `.c`/`.cpp` path is a scratch file compiled with `--flags` (default
  `/O2 /GX`); in Git Bash set `MSYS_NO_PATHCONV=1`, or `--flags "/O2"` is rewritten into a path and the file is
  compiled without optimisation (no register or frame output); write the scratch path `C:/...` (a `/c/...` path
  is converted). `--sched` adds the instruction scheduler (`tools/c2sched.py`): per region the input order (each
  instruction's sequence number, height, memory-read bit and priority), then each pick cycle by cycle (U and V slot)
  with the instructions it passed over and why (not ready until a later cycle, and the predecessor that bounds it; no
  V pairing), and what decided it against the next ready instruction behind it (a tie broken by input order, or the
  priority gap in heights), then the output order and the instructions that moved against the input.
  `--operands` adds the global optimiser's commutative operand sort (`tools/c2operands.py`): each distinct comparison
  the sort made in the function, the two operands (a variable by its name and symbol number, anything else by its
  IL opcode) with their weights as height, size and hash, the one that goes first on the left (`=` a tie); the
  heavier operand is computed first, so an x87 operand-order wall reads as which symbol number is higher
  (docs/c2-backend.md, "Operand order of commutative operations", after bw1-decomp's reading).
  `--addr` adds the two-register addresses (`[base+index+disp]`, `tools/c2addr.py`): each one the address matcher
  builds in the function, labelled S1, S2..., with its final registers, the two operands the sort compared (each hash
  taken apart down to its symbol numbers), the nearest symbol-number shifts that reverse them, and a table of the
  shifts at each numbering point that flip which sites (docs/c2-backend.md, "Base and index of a two-register
  address"). An operand rewritten in place by a later pass prints the pass instead.
  `--header-override HEADER=FILE` (repeatable) compiles the unit with a repository header replaced by a scratch copy:
  the header's directory is shadowed in `build/c2trace/override/` and takes its place in the include search (the
  unit's own source too, when it sits there), so a header edit is measured without touching `src/`. SP5 units only;
  about 5-20 s a unit. The rules it reads are in `docs/c2-backend.md`.
- **`trial`** (`tools/trial.py <addr> VARIANT [VARIANT ...] [--show] [--jobs N]`): a few hand-written spellings of
  one function compiled against retail, bounded as AGENTS.md's ruling says ("Fast variant trials are a bounded
  tool", 2026-11-18). Its exact hits hold in the tree, but its partial scores can differ from `match` on the real unit
  (70.4 against 81.0 seen): compare variants with each other, not with `match`. A variant is a file holding the function's whole definition as it would stand in the source.
  The unit is preprocessed once with its flags (`cl /E`, cached in `build/trial/` by the unit's fingerprint); each
  variant replaces the definition's lines in that text and is compiled with the unit's flags minus `/I`, `/D` and
  `/FI` (0.2 s for ObjectGroup, 0.75 s for ShipClass against 0.3 s and 1.5 s from source). A variant that names an
  identifier the preprocessed text never spells (a macro: `NULL`, `Py_DECREF`) is compiled from a scratch copy of
  the source instead, marked `(from the source)`. A function defined in a header (an inline, a
  class member: its annotation sits in the `.h`) is spliced into the header's own text: from the preprocessed text
  by the header's `#line` markers, or, for a macro or a `/YX` unit, from a scratch include directory holding the
  header with the variant (the directory's other headers hard-linked, searched before the original, the unit's
  source unchanged). A member defined in its class ends at its own closing brace. A unit whose compile is large
  (AppWrap: about 25 s a run, base and variant in parallel) costs what `match` on it costs. The unmodified text is compiled as the base, and a base that differs
  from `build/obj`'s function is warned about. The output ranks the variants by a navigation score (addresses, call
  targets and zero relocation fields normalised: close to `match`'s score, not equal to it), says whether each
  moved the bytes against the base, and marks an exact one (relocation fields masked) as a clue, not an answer: the
  spelling still needs its evidence, and the change is made and matched through `decomp.py match`. The bounds:
  variants are files (no generator; a file repeating another is refused); a run of one variant records in
  `build/trial/<ADDR>.json` whether it moved the output, keyed by the function's current source, and a run of more
  than one is refused until such a record exists for that source; at most 20 variants a run. After the x87 lane's
  fast-variant script and bw1-decomp's `tiebreak-probe.py`/`variant-sweep.py` (CC0).
- **`c2il`** (`tools/c2il.py compare <unit|0xADDR> A [B]`): whether two spellings give the back end the same input.
  `cl /B2 <batch>` runs the batch in place of C2.DLL (the pass's command line in `MSC_CMD_FLAGS`, starting `-il
  <prefix>`); the batch copies the IL files away, so a compile gives its IL and no object. Both sides compile from one
  scratch path with the unit's flags. `identical` (every IL file equal, the code IL's line markers `4F 01 80 <line>`
  left out: every function of the unit comes out the same, inline budgets and all; blank lines and comments),
  `same code` (the code IL equal, the symbol files differ: a renamed local), `different` (the code IL differs, with
  the first differing byte: a pair of braces is two tuples). With an address the files are definition variants, as
  `trial` takes them. About a second a side. Use it to show a readability edit is byte-neutral for the whole unit,
  not only for the function it touches. After bw1-decomp's `c2-il.py` (CC0). `c2trace --cost` counts one function's
  tuples under a debugger; this compares whole units without one.
- **`dupescan`** (`tools/dupescan.py [--kind diff|claimed|gap] [--tier exact|shape] [--min-size N] [--twins N]
  [--json FILE]`): retail functions no matched source covers yet (annotated but not MATCH, claimed without source, or
  found in an unwritten gap, split at each return as `openwork gaps` scans them) whose retail bytes, relocations
  masked, equal a matched function's. Relocations are the image's base relocations plus the rel32 of every call or
  branch that leaves the body; an address inside the body counts as its offset. `exact`: every relocation names the
  same target too (the twin's source should match as it stands: another unit's copy, an unfolded duplicate);
  `shape`: the bytes agree and some targets differ (a template for it, its referents changed). Bodies under
  `--min-size` (8) are left out. Retail signatures are cached in `build/dupescan_cache.pkl`; reads
  `build/report.json`, so `build` first. After bw1-decomp's `asm-dupe-scan.py` (CC0). A report only.
- **`nocall`** (`tools/nocall.py [--unit U] [--all] [--gaps] [--limit N] [--json FILE]`): retail out-of-line copies
  of a unit's helpers that nothing calls (no `call`/`jmp`/`jcc` rel32 lands on them, no base-relocated dword holds
  their address), with the unit's open functions. A helper is a header-defined inline or template, a `static` or
  `inline` function of its unit, or (`ob2`) any function of a unit built with `/Ob2` (NetImmerse); deleting
  destructors are left out. VC6 emits the copy for every helper a function names before the back end inlines the
  calls, so an uncalled copy means its callers called it: a caller that writes the body out by hand should call it
  (docs/levers/inline.md). Each open caller is marked `holds its referents` when its retail body reaches every callee
  and global the helper's does (a referent-free helper: every member displacement and vtable slot it reads, stack
  slots aside) and is at least twice its size, and `names it` when its source already spells the call; without
  `--all` a candidate is listed when an open caller that doesn't name it holds its trace (a distinctive referent,
  reached from at most 40 places, or only displacements), or when it is NO-SYMBOL. One distinctive referent or one
  displacement is a weak trace: read both bodies before writing the call. `--gaps` adds the functions of unwritten gaps held the same way: helpers without
  an identity yet. Reads `build/report.json` and the retail signatures dupescan caches. A report only.
- **`framemap`** (`tools/framemap.py <addr|name> [--rev <git rev>] [--source <file>]`): a frame or register wall read
  slot by slot. Retail's frame slots (ESP tracked through pushes, pops and calls, a call popping what its callee's
  `ret N` says; normalised to the entry, so the return address is 0, parameters +4 and up and locals below 0) beside
  ours, ours named from the CodeView records of a `/Z7` diagnosis copy (`S_BPREL32` locals, `.debug$T` types and
  sizes, COFF line numbers). Slots are paired by the instructions that use them (the two functions aligned with frame
  offsets, registers and addresses left out), then by order of first use. The table gives each slot's offsets, our
  variable (`desc+0x24` inside an aggregate), its type and block scope, how each side accesses it (int, byte, float,
  address taken, `this`, argument, writes as float or int) and a note: `ours at`, `swapped with`, `retail has one more
  4-byte slot here`, `ours shares this slot between A and B`, `retail keeps it in X with Y`, `written once from X: a
  copy of it`, `written as float in retail, as int in ours`. The callee-saved registers follow: the push order, each
  value of ours (named from the source line that sets it: the local assigned there, `this`, a constant, a reload of a
  named slot) and the retail register the aligned instructions give it, and the swaps. The first line is a reading
  when the tables show a pattern of `docs/levers/locals.md` or `walls.md` (one extra slot, a slot shared on one side
  only, two slots swapped, unused frame bytes, an FPU copy against an integer one, register roles swapped), else
  "agree": then the wall is in the scratch registers, the schedule or an expression, and declaration order is not the
  lever. `--rev` and `--source` compile the unit's file as it was at a revision, or another file, beside the unit (so
  quoted includes resolve; the headers stay current). `packet.py` includes the map for a register wall up to 0x600
  bytes; `walls.py`'s register note names a frame-size difference. Blind spots: values held only in scratch
  registers, spills VC6 makes for register variables (no CodeView record names them), the order of instructions, and
  a slot whose instructions differ on both sides so that no aligned pair votes for it (it is paired by first use).
- **`similar`** (`tools/similar.py <addr> [-k 10] [--source 2]`): any address (annotated, claimed or in a gap);
  normalised opcode n-grams with callee names, clones collapsed. Index `build/similar.pkl`, rebuilt when the matched
  functions or the known names change.
- **`openwork`**: `class <Class>`, `range <lo> <hi>`, `gaps [N]` (unwritten bytes per class before the gap: the
  phase 1 map), `classes`, `parked <score>`, `walls [inline|control-flow|register|referent|structure [MIN]]` (open
  functions by wall class, after `build`), `phases`, `drafts [MIN]` (the phase 2 queue), and `triage [Class]`: a
  Sonnet worker takes near-misses with a spelling wall, ready claimed functions and open functions with no wall
  noted, Opus structural walls, parked spelling walls and large functions; register, x87 and scheduling
  residue is parked until a new lever. `groups [ID]` lists the shared-wall groups the cards name (functions, scores,
  wall, last try). `headers [NAME]` reads the cards' tries for the headers they name (`NiTList.h`, or a
  `hdr-<slug>` group): per header the open functions citing it, the functions with a try that changed or A/B'd it
  ("worked") and with one that only pointed at it ("noted": off-limits, not tried), and the unworked leads; with a
  NAME, every entry. The split reads free text, so judge the entries. The packet prints the same summary for the
  headers a function's card names, with the earliest A/B or compiler-state verdict as "settled". `untried [LO HI] [MIN] [--ignore sonnet,merge_lane] [--since 2026-11-05]` lists the open written
  functions with no try on their card by anyone outside `--ignore` since `--since`. A function whose card's last
  wall is `compiler-state` stays open in every total, but `walls`, `drafts`, `parked` and `triage` (tier `state`)
  list it apart and `phases` counts it on its own line. `range` and `gaps` start the gap after a function that
  doesn't match at retail's end of it (scanned as `ready` scans a claimed body); where no end is found, the gap is
  marked as possibly that function's tail.
  The readability backlog: `advisory` tables the advisory findings (written functions, from the rule cache in
  `build/report.json`, plus the file-level R11/R16) by rule: findings, units, functions, how many sit in finished
  units, what the rule asks and whether a fix is [free] or [checked]; R7 is split by placeholder family
  (`Unknown<ADDR>`, `unknown<OFF>`, `VirtualSlot<NN>`). `advisory R7` ranks the units for one rule; `advisory
  game/ShipClass` or `advisory ShipClass [R7]` prints the findings themselves. `finish [N]` lists the units whose
  written functions all match (nothing open or pending), most findings first: a finishing lane cleans these without
  touching open work. `_$E` initialisers have no source and are never checked.
- **`names`** (`tools/names.py <Class|unit> [--direct]`, `--summary [--list direct|lineage|weak|none]`): the name
  evidence for each placeholder of a class, or of the placeholders a unit uses; a report, it renames nothing. A
  placeholder is a (class, spelling) pair. Sources, read mechanically: a SWIG wrapper whose one game call is the
  placeholder (`direct`), a member wrapper `<Class>_<member>_get/_set`, a one-statement `Get`/`Is`/`Set` accessor
  whose own name a wrapper or a script evidences (`direct`; `weak` otherwise), a string literal of the function's
  body naming `<Class>::<Name>` (`direct`; other log text `weak`), the Gamebryo 1.1.2 headers' member and virtual
  order aligned between names both declarations share (`lineage`), and the BSim run against the Gamebryo libraries
  (`lineage`, the same class, similarity >= 0.8, significance >= 15, no rival within 0.05). The game scripts only
  corroborate (`[scripts: N files]`). A candidate the class or a base already declares is a conflict (`weak`); a
  candidate already spelled in a file that uses the placeholder is flagged, since a local or parameter there would
  shadow the member (`this->ship`). Index cached in `build/names.pkl` by `src/`'s timestamps.
- **Referent placement** (`tools/decomp.py`, AGENTS.md "Referents"): a unit-local referent takes the address retail
  holds at the first site that places it, and only a site whose instruction is retail's own (the same bytes at the
  same place, the relocation field aside; every site of a function that agrees with retail apart from its fields)
  may place it. A function misaligned against retail reads garbage at ours' offset; its sites still resolve (so it
  compares as before), but what they place, verify or choose is journaled and undone when its relocation ends, so a
  garbage address that happens to hold the same bytes (0.0f) can't fail a matching function of the unit. Within a
  function the agreeing sites are resolved first. One datum still has one address, and a second retail copy is still
  a mismatch.
- **One copy per datum across the tree** (`decomp.py`'s `prepare_copies`, before any function is compared): every
  agreeing site of every annotated function that reads initialised data votes the retail address it reads
  (`collect_reads`, cached per unit in `build/referent_reads.pkl` by the object's stamp and its functions'
  annotations; the gate recomputes it). `decide_copies` then gives each COMDAT data name (`__real@`, `??_C@`, an
  unannotated vtable) the address most functions read it at (ties broken by units; a name whose candidates all hold an
  annotated datum, or that as many functions read at two addresses, has none), and marks a unit-private datum (a file
  static, an unannotated global) read at an annotated datum's address, another name's copy, or another unit's datum.
  In `resolve`, an agreeing site reading a name away from its copy, or a marked datum, is a problem (bytes equal:
  `WRONG-REFERENT`); a site that can't place takes the name's copy instead of its noise address. A literal read only
  once in the tree as a static (or the reverse) has no rival vote and isn't caught. `python tools/decomp.py copies
  [--all]` prints each datum read at more than one address or at another datum's, the chosen copy (`*`) and every
  address's readers with their status in the last report; without `--all`, only the data a MATCH or WRONG-REFERENT
  function misreads. `diff` trusts the same sites `match` does.
- **`callset`** (`tools/callset.py [--min-score N] [--top N] [--callee NAME|ADDR]`, `fn <addr> [--all]`): for every
  open (DIFF) function, the direct call targets (import slots included, virtual calls not) of retail's code against
  ours, read as `walls.py` reads them; a call of ours without an address is placed at the annotated copy retail calls.
  Two tables by callee, ranked by open functions then mean score: what retail calls out of line where ours doesn't
  (ours expands it, or lacks the call), and what ours calls where retail doesn't (retail expands it). A callee wrong
  at many sites is one header body to check against lineage. `--callee` lists its functions with scores; `fn` shows
  one function's differing callees. Loads every unit once (about 15 seconds), then caches in `build/callset.json`
  until `build/report.json` changes.
- **`sdkdebug`** (`tools/sdkdebug.py [--exe PATH] asserts [--file RE] [--expr RE]`, `files [--all] [--objects]`, `map
  [--json F]`, `fn <addr>`, `od [--unit RE]`, `cmp <addr> [--debug <addr>]`, `search <addr> | <unit> <symbol>
  [--file RE | --range LO HI] [--top N]`, `search --all-unpaired [--open-only] [--unit RE]`, `find <scratch.cpp>
  [--flags F] [--only RE]`, `rtti [--prefix RE] [--all]`; `cmp`, `od`, `search` and `find` take `--no-mask-frame`): the BC SDK's `AnimOnly.exe` (`C:/Recomp/bcsdk-1.1/Tools/AnimOnly`, April 2001), a DEBUG
  build of NetImmerse 3.1, read as evidence (never run, never copied; path from `--exe`, `STBC_ANIMONLY` or
  `"animonly"` in `config/project.json`). `asserts` lists its 1231 `assert` sites (expression, `__FILE__` path, line,
  containing function); `files` the runs of each original `.cpp` (`--objects` widens a run to its object, from the
  NiRTTI initializers on). `map` pairs AnimOnly's functions with retail's annotated ones (NiRTTI vtables slot by slot,
  CreateObject registrations, unique strings, then calls and definition order to a fixpoint; checked against the
  assert files, no wrong pair seen) and writes `build/sdkdebug/map.json`: each pair's original file, our units against
  the original files, and member names read from asserts. `fn` shows one AnimOnly function with its file, asserts,
  pair and named callees. `od` compiles our units under AnimOnly's debug flags (`/Od /MLd /D_DEBUG`, and again with
  `/GZ`: NiDirect3D and NiSurface were built with it, NiMain, NiAnimation, NiCollision, NiIK without) and finds each
  function, COMDAT inline copies included, byte for byte in AnimOnly with relocated fields masked
  (`build/sdkdebug/od.json`, about 50 seconds). AnimOnly's NiAnimation was built with the VC6 Processor Pack back
  end (its float compares read `test ah, 5; jp` and `and eax, 0x4100`), so a unit paired with NiAnimation files is
  compiled with `pp` as well. An assert's line operand is layout and is masked; a paired function the byte search
  misses (a line under 128 pushed as an imm8, a start the image cut missed, a body under 8 bytes) is compared with
  its pair instruction by instruction (`same_body`). It lists the open paired functions whose /Od body equals the
  original's (their wall is release-only) and those whose /Od body differs (a statement differs from the original's).
  `cmp` prints one such function side by side, ours at /Od beside AnimOnly's, callees named, in the /Od variant
  (`/GZ` or not, each back end) that compares best; `--debug <addr>` compares against a debug function the map
  hasn't paired. Comparisons read listings with callees dropped and **frame slots masked** (the displacement of
  every ebp-based operand: `[ebp - 0x10]`, `[ebp + 8]` read `[ebp X]`, a frame array `[ebp + eax*4 - 0x98]` reads
  `[ebp + eax*4 X]`), and print two ratios: masked (the statements) and with frame slots (the frame order, which the
  names of the locals decide: the Hungarian-name rulings in AGENTS.md), the second counted along the masked
  alignment (the lines it pairs whose slots differ), so moved locals never show as moved statements;
  `--no-mask-frame` ranks and shows with the slots, along the same alignment. A function's listing ends at its
  first jump table, on both sides: AnimOnly's at `jmp [reg*4 + table]`'s run of relocated dwords (a prologue right
  after the tables starts a function the start scan missed), ours at the lowest place the function's own
  relocations point forward to; `od`'s instruction compare checks the tables apart. A constant read from read-only
  data shows its value (`fdiv dword ptr [=90.0]`; the comparison still reads it as an address), and `cmp` lists the
  lines it reads alike whose constants differ. `od` sets each differing pair's two ratios in `od.json` (`differs`) and counts the pairs identical
  but for frame slots. `search` compiles one of our functions at /Od (every variant) and ranks every debug function
  of `--file` (a regex over the object runs' files), `--range LO HI` or the whole image against it, printing the best
  `--top` with both ratios, their file, whether that is a file of our unit's pairs, and any pair the debug function
  already has; `--all-unpaired` does it for every NetImmerse function without a map pair that is open or has no
  byte-identical /Od copy (`build/sdkdebug/search.json`; about 1850 functions). The ranking refines upper bounds
  best first (instruction bags over all debug functions at once, then a bit-parallel longest common subsequence,
  then difflib), so its result equals a full scan; about five minutes for all of them. It also writes
  `build/sdkdebug/search_pairs.txt`: the clear hits (95% alike masked or more, 5 points ahead of the next, a debug
  function no pair or other hit holds) and the unpaired functions whose /Od body `od` found at one address only. A hit
  read with `cmp <retail> --debug <addr>` and confirmed goes in `config/sdkdebug_pairs.txt` (`0x<retail> 0x<debug>
  # evidence`); `map` adds those pairs after its own rounds and runs calls and order again from them, listing any its
  own pairs contradict (`map --pairs F` tries another file, with `--json` to keep `map.json`). `find` compiles a scratch file under `--flags` and looks for its functions (`#line N` before an assert
  gives the original's line operand); a function with no byte hit is looked up frame-masked, and one identical but
  for frame slots is listed with its addresses. `rtti` reads a `/GR` build's class hierarchy descriptors (`--exe` DamageTool.exe or
  ModelPropertyEditor_s.exe) against our classes. A `/Od` body pins what release bytes can't: the order of
  statements and declarations, the frame order of named locals (the names themselves decide it), every inline call,
  and so the inline cost of a spelling (docs/evidence.md, "The BC SDK's debug NetImmerse"). In Git Bash set
  `MSYS_NO_PATHCONV=1` before passing `--flags "/Od ..."`.
- **`sibling`** (`tools/sibling.py [--build B] [--path PATH] [--refresh]`, `fn <addr>`, `callsets [--open] [--min-score
  N] [--top N]`, `new`, `flips [--all]`, `open [--min-score N] [--top N] [--unit RE] [--strong] [--all]`, `syms [ADDR
  ...] [--all]`): other builds of the same code read as siblings of retail. `--build` picks one: `demo`
  (`stbcdemo.exe`, 2002-02-09, the DEMO branch, same linker; the default), `mpe` (the SDK's
  `ModelPropertyEditor_s.exe`, February 2002: the TG framework, NetImmerse and its own SWIG App module, no symbols
  and no base relocations), `damagetool` (the SDK's `DamageTool.exe`, April 2001, with its COFF symbol table),
  `exporter` (the SDK's `STBCExporter.dlu`, March 2002, NetImmerse 3), or `all`. AnimOnly, the debug build, is
  `sdkdebug`'s. Each path is `--path` (the demo's also `--demo`), else `STBC_<BUILD>`, else `"siblings": {build:
  path}` in `config/project.json` (the demo's also `"sibling"`), else the default under `C:/Recomp`; the images are
  only read, never run or copied into the tree. Every image is cut into functions the same way (entry point, code
  pointers in data and pushed as immediates, call targets, and the code after a `ret`/`jmp` that no earlier branch
  or jump table reaches; retail adds every annotated or listed address), hashed with relocated dwords and outside
  branches masked. An image linked without base relocations (`mpe`) gets them rebuilt from a linear sweep (operand
  dwords that point into the image, the jump tables the sweep finds) and the aligned pointers of `.data`/`.rdata`:
  on DamageTool and the exporter, whose real tables check it, over 99% agree. Pairing starts from names both builds
  give: DamageTool's decorated names against our annotated symbols, and vtables named by NetImmerse's NiRTTI, a
  `/GR` build's MSVC RTTI or DamageTool's `??_7` symbols against retail's NiRTTI and `config/vtables.txt` (slot by
  slot); then unique bytes, references at the same index in paired functions, vtable slots, unique strings, and
  link order between anchors. Once that settles, three passes find what the link order can't, and it settles
  again: `name` (DamageTool) pairs a `Class::Method` both sides give one function, DamageTool's classes renamed as
  ours (`CLASS_RENAMES` in the tool, from the class comments: `BinaryVoxel` is `NiBinaryVoxelData`, its `Slice`
  `VoxelSlice`, `Damage` `VisibleDamage`, `OBBForest` `OBBModel`, ...; completed by a class whose functions two or
  more pairs found without names put in one class of ours, printed as the cache's `renames`); `neighbour` pairs the
  unpaired callees and callers of a pair with its counterpart's, each the other's single most alike candidate;
  `refs` pairs a function whose bytes and code references (every one paired) are the same and unique on both sides,
  wherever the link order put it. Two functions whose masked bytes differ pair only when their instructions
  (addresses masked) are at least 40% alike, and that similarity is kept and printed; looser where other evidence
  holds (30% in step between anchors holding as many functions on each side, 25% for neighbours whose paired
  references agree, 20% for a name among overloads, 10% for a name unique on each side). An `order` or `neighbour`
  pair is refused where both builds name the two as members of different classes; stronger evidence still pairs
  them and `syms` shows the clash, which is the check on false pairs (and on our class names). With no command it prints coverage by
  status (paired, identical when masked, call set differs). `fn` prints retail and the sibling side by side, both
  sides' addresses named through the pairing (`--build all`: each sibling's pair, similarity and differing calls);
  `callsets` lists annotated functions whose direct call sets (as `callset.py` reads them) differ once the sibling's
  targets are mapped to retail, by function and by callee; `new` lists sibling-only functions that paired functions
  call (with DamageTool's name); `flips` classes the open functions (`--all`: every annotated one) whose sibling copy
  differs: `frame/register` or `schedule` (the same source compiled under other declarations moved only frame
  slots, register picks or one load: evidence for a declaration-sensitive wall), `layout` (member offsets or vtable
  slots changed), or `code`. `open` reads every sibling against the open functions: a summary (per sibling: open
  functions paired, identical or differing, and of those whose call sets differ between retail and ours, how many
  the sibling sides with retail, with ours, or mixed), then the functions where a sibling takes a side, ranked by
  the number of siblings siding with retail, each with retail's and ours' differing callees and, per sibling, its
  pair, how it paired, its similarity, its verdict and the calls only it makes. A sibling sides with retail on a
  callee when it calls it as often as retail does; a side that rests on the sibling *not* calling it is `unknown`
  while the sibling makes calls the pairing can't map. A copy whose masked bytes equal retail's maps its calls by
  position, so it always sides with retail: the evidence is that another build, under other declarations, made
  retail's decision too. Two or more siblings siding with retail (`--strong`) is a strong pointer that the call is
  the source's, not a budget or declaration accident. `syms` (DamageTool) prints, for the open functions (`--all`:
  every annotated one; or the addresses given), the decorated name of the paired DamageTool function against ours,
  both undecorated (dbghelp), and what differs: name (a placeholder of ours named there is `placeholder name`),
  parameter types, `const`, return type, access, virtual/static, calling convention. DamageTool's linker folded
  identical bodies, so an address can carry several names; the closest to ours is shown, and a year of revisions
  lies between the builds, so a difference is a lead for retail's bytes to confirm, never a correction by itself.
  First run about two and a half minutes for all four (retail's cut, about 20 seconds, is shared in
  `build/sibling-retail.json`), then cached per sibling in `build/sibling-<build>.json` until its image,
  `build/report.json` or the tool itself changes (`open` about 3 seconds). The demo made the same inlining decisions nearly everywhere:
  its differences are mostly the source revisions between February and April 2002.
- **`ctxscan`** (`tools/ctxscan.py <addr> [<addr>...] [--top] [--range LO HI] [--jobs N]`, report only): whether a
  function's code depends on the count of declarations before it (docs/levers/walls.md, "decided by a count, modulo
  64"). For each k in the range (default 0 63) it compiles a scratch copy of the function's unit with k lines
  `extern int ctxScanProbe<i>;` directly above the function's annotation stack (`--top`: at the top of the file; a
  function annotated in a header is always probed at the top of its unit's file), and compares the function's bytes
  with retail's, relocation fields masked on both sides. It prints the k windows where the bytes are exact and each
  distinct byte state with its k and its kind against retail (`decomp.flip_kind`: `frame/register`, `schedule`, or
  `other`). The copies are compiled under `build/ctxscan/` with the source's directory first on the include path,
  and deleted afterwards; `src/` is never edited; k = 0 is checked against the unit's object in `build/obj` and a
  difference printed as a warning. About 5 to 20 seconds per function. A dummy declaration is never a fix: a count
  that lands in the window is a lead to the original's own declarations.
- **`units`** (`tools/units.py [boundaries|scattered|order] [UNIT]`): unit boundaries read from retail addresses and
  the annotations, a unit's own `.cpp` definitions only. `boundaries`: a static-initialiser run with `<iostream>`
  quads (a call of `ios_base::Init::Init` or `_Winit::_Winit`) or a locale guard between two of a unit's functions,
  with no other unit's code between them: a candidate for two retail units, `quads` first, `locale` (a guard only)
  after; confirm by the `.bss` stores (docs/levers/statics.md). `scattered`: units that other units' code splits
  into several runs. `order`: units whose definition order differs from retail's address order, the functions
  outside the longest ordered run listed, ranked by how many are open (docs/levers/walls.md, the EH-state entry;
  moving a definition only to move codegen is a fakematch).
- **`cards`** (`tools/cards.py show|add|tag`): `add <addr> --by <model> --how "<what changed, what it showed>"
  [--wall <class>] [--group <id>]`, committed with the work. `--wall compiler-state` records a wall proven bounded
  (several spellings A/B'd, retail's own sites contradicting each other). `--group` puts the function in a
  shared-wall group, a short slug naming one fix that would close or fail them together; `tag <addr>... --group <id>
  --by <model> --how "<why>"` does so without a try. `merge_lane.py check` stops on a changed open function without an
  entry.
- **`merge_lane.py`**: `check` in a worker's slot runs everything the merge will (build, prune, regress, the lane's
  [match] findings, the gate) and lists the lane's new PARKED, BUG and RULE-EXCEPTION notes, its removed MATCH-PENDING notes and every added line whose comment says `invented` (`(invented)` helpers, names, text), grouped by file and cut after 40 with a count; `merge <slot> -m "..."
  [--accept <addr>...] [--allow-tooling]` rebases, gates, fast-forwards main, updates the baseline, cards and README counts, and
  refreshes the Ghidra exports. The orchestrator reads every listed note before accepting.
  - **Declaration-sensitive flips.** `config/declaration_sensitive.txt` lists the functions ruled declaration-
    sensitive (address, name, allowed kinds: `frame/register`, `schedule`). For a listed function whose diff
    walls.py classes `register`, `build` and `match` store in its row (`"flip"`) whether the two disassemblies differ
    only in registers and frame offsets (`frame/register`), or hold the same such instructions in another order
    (`schedule`). `regress` accepts a listed function's lost match by itself when its source is unchanged and its
    flip is a kind its line allows, on a `declaration-sensitive <addr> ...` line; anything else is still a
    regression. `check` and `merge` print those under "declaration-sensitive flips accepted", write each one's card
    entry (once per lane) and commit the cards. The summary of `build`, `match` and `gate` prints the listed
    functions' state on a line of its own (`declaration-sensitive (listed): N of M matched`); the headline counts them
    as it counts every function, and the line is there to explain the headline's jitter. Only the user adds a
    function to the list. The merge judges a lane with main's tools, flags and baseline: after the rebase it stops when the lane changes `config/baseline.json` (only the merge writes it), leaves ignored files under `tools/` other than the compiler junction and bytecode caches, or changes `tools/` or `config/project.json` without `--allow-tooling`, which the orchestrator passes only after reviewing that diff (a lane that needs a per-unit flag is such a case). `check` warns the worker of the same.
- **`comdat`** (`tools/comdat.py <unit> [lo hi]`): candidate `@unit` annotations with their decorated names;
  `layout <unit> [lo hi]` lists the unit's code sections in link order and fits them against retail (relocation
  targets checked); `order <unit> [--all] [--statics] [--search]` reads the unit's code sections in object order with
  the retail address each one's annotation gives (its own file's entries and the `@unit` copies naming it; with
  `--search` a COMDAT without one is found by its bytes, a single hit only), takes the longest rising run of those
  addresses as the order both agree on, and prints every other section with the run's neighbours retail places it
  between, its COMDAT selection (`nodup` out-of-line, `any` inline or template copy) beside it. Functions and `_$E`
  stubs are ordered as two groups (the stubs trail the unit's code; `--statics` lists theirs). An out-of-order
  out-of-line function is a definition order that differs from retail's (SetManager's constructor is defined after
  Update and the others in ours, first in retail); an out-of-order copy is an inline emitted after another function
  than retail's, or one whose annotation is another unit's copy. The emission rules are in docs/c2-backend.md, "What a
  unit's object holds, and in which order". After bw1-decomp's `comdat-scan.py order` (CC0).
- **`statics`**: see "Static initialisers" below.
- **`annotate-inlines`** (`tools/annotate_inlines.py [--apply]`): `--apply` annotates claimed functions whose body is
  already inline in their class header, drops their claims, builds, keeps what matches and commits it (no model).
  Run it whenever new claims land in `config/symbols.txt`, before planning lanes.
- There is no permuter: spellings are tried by hand with `trial`, after `c2trace` has said what the deciding site
  needs (the `c2-residue` skill).
- **`trace`** (`tools/trace.py targets`, then `run` while the user plays, then `report` into
  `build/runtime/report.txt`; Frida, `pip install frida`): the class of `this`, the callers and the argument classes
  of unidentified functions in the running game. Leads to check against the disassembly, not evidence on their own:
  a `this` seen in a derived class names the owner only up to the common base.
- **`linkcheck`** (`tools/linkcheck.py [--top N] [--all]`, after `build`): a link dry-run over build/obj with
  tools/coff.py, no exe produced. It counts duplicate definitions (an external defined, not as a COMDAT or common
  symbol, in more than one object: a function or datum in two units, which the byte gate can't see) and unresolved
  externals (referenced, defined by no object, named by no annotation and claimed by no symbols.txt line: what the
  source still lacks), then lists the top units and names of each. A weak external (storage class 105: a
  vftable's slot 0 names a weak `??_E` that falls back to the class's `??_G`, and VC6 emits a `??_E` body only where a
  unit deletes an array of the class) resolves when its fallback does; one that doesn't is listed with its fallback. A
  name defined only by a unit and the units that `#include` its file (crt0.c in wincrt0.c) is counted apart as a
  variant, and so is a name only such a unit references (crt0's `_main`); `__imp_` imports no claim names are counted
  apart too, since an import library resolves them. A line before the counts gives the names only a symbols.txt claim
  resolves (imports, and the CRT, system-library or not-yet-written code and data a real link must still get from a
  library or the source). A report only: nothing gates on it.
- **`clanglint`** (`tools/clanglint.py [UNIT ...] [--module DIR] [--list] [--flag F] [--sign] [--all] [--json FILE]`;
  about 20 s for the tree): clang-cl (LLVM, `C:/Program Files/LLVM/bin`) parses each unit with its include paths,
  defines and forced includes, `_MSC_VER` 1200 and VC6's headers, and reports the findings in src/ (a header's once):
  `-Wreturn-type`, the pointer/integer casts, `-Wreorder-ctor`, the non-virtual destructor deletes,
  `-Woverloaded-virtual`, and the constructs clang rejects (mostly VC6's dialect). VC6's for-scope is filtered out.
  The few lines of VC6's C++ headers clang rejects are patched in copies under build/clanglint/include/ (the list
  is in the tool's docstring); the originals are untouched. Units of modules with an upstream text are left out
  unless `--all`. A report only: a finding is a lead about a type or a signature, and the gate still decides.
- **`eaxcheck`** (`tools/eaxcheck.py [--callee ADDR] [--json FILE]`, after `build`; about 10 s): the callees whose
  decorated name declares `void` (the report's symbols and config/symbols.txt's claims), and the direct call sites in
  annotated functions' retail bodies where the same block reads EAX, AX or AL before writing it. VC6 never reads EAX
  after a call without writing it, so each site says the callee returns a value (or the claimed callee is the wrong
  one). Virtual and tail calls are not seen. A report only.
- **`linkready`** (`tools/linkready.py [--unit U] [--list text|data|order|unresolved] [--top N] [--json FILE]`, after
  `build`; about 15 s): what a real LINK 6 run of build/obj would lay out differently from retail, which the gate
  (exact functions spliced into retail, data placed at retail's addresses) can't see. Code: each unit's sections in
  object order against retail's addresses (`comdat.py order` for every unit; own functions and inline copies counted
  apart), and the base each function implies over the object's `.text` sections laid end to end (one base: the unit
  would link where retail has it, sizes included; a copy annotated to another unit is dropped, as LINK keeps one).
  Data: the base each `.data`, `.rdata` and `.bss` datum an exact function reaches implies (retail's relocated dword
  less the addend, against the datum's offset), and the units whose data falls out of their code's object order.
  Unresolved: linkcheck's count (the same weak-external, variant and import rules), except that a name an
  annotation gives but no object defines counts here, where linkcheck lists it on a line of its own. Not measured:
  COMDAT selection and folding, padding between objects, imports, resources.
- **`layoutcheck`** (`tools/layoutcheck.py [--class C] [--check member|size|vtable] [--weak] [--top N] [--json FILE]
  [--fresh]`, after `build`; about 30 s warm): our class layouts against what retail's code does with them, to find
  wrong types in the headers. Our side is the compiler's: `ghidra/export_types.py`'s `/Z7` build into build/types/
  (`build_objects(stale_only=True)`: a unit is compiled again only when its build/obj/ object is newer; `--fresh`
  rebuilds all, about a minute) and its CodeView reader, the merged types cached in build/layoutcheck_types.pkl.
  Three checks: **member** follows, by a must-analysis over each annotated function's retail blocks, the registers
  that hold `this` or a pointer whose type our source gives (a pointer member, a signature parameter, a typed global,
  a typed return), resolves each `[reg+disp]` to its leaf member and judges width, signedness and float against
  integer, and judges direct member calls on a subobject by the callee's class; **size** reads the pushed size of
  each allocation (operator new, TGPoolManager::GetPool, AllocateObject) whose result reaches a constructor;
  **vtable** counts retail's vtable (from the matched functions' `??_7` relocations and config/vtables.txt) up to the
  next code-referenced address and names the slots that differ. Each finding line gives the class, member and
  offset, our type, retail's evidence by reason with counts, the matched/open split and two sites; `--json` keeps
  every site. A contradiction in a matched function counts one grade lower (our source compiled to those bytes, so a
  cast there explains it), and the weak grade (casts to derived classes, array walks past sizeof) shows only with
  `--weak`. The module docstring lists what is never a finding (VC6's narrowing of masks, narrow destinations,
  struct copies, __int64 halves, bitfields, virtual bases). It can't see casts or accessor return types in the
  source: read a finding's sites before changing a type. A report only.
- **`vtslots`** (`tools/vtslots.py [--class C ...] [--all] [--leads] [--no-probe] [--fresh] [--top N] [--json FILE]`,
  after `build`; about a minute warm, the probes cached in build/vtslots/): every declared virtual against retail's
  vtables slot by slot, which layoutcheck's length check and the byte gate don't see when no matched function makes
  the call. Our slots come from two readings of the compiler. CodeView: the `/Z7` objects layoutcheck builds give
  each class's methods (access, virtual mode, an introducing virtual's offset); a class's slots are its primary
  base's plus its own introductions, with its overrides (same name and parameters) replacing the base's entries.
  VC6's CodeView is wrong for overloads: every member of an overload group with a virtual introduction is recorded
  as introducing, at the group's last offset, non-virtual members included. The probe resolves them: per owning unit
  (a unit named after the class, else the unit defining most of the classes) one source `#include`s the unit's .cpp
  (its flags, defines and include path; `private`/`protected` defined as `public`, dropped if the unit's own code
  then fails) and passes `&Class::Method` of every virtual to an undefined `template<class T> void vtslots_take(T)`;
  an overload goes through a cast to its own member pointer type, spelled from CodeView in each `bool`/`unsigned
  char` reading (CodeView records `bool` as `unsigned char`). VC6 emits a virtual's address as the vcall thunk
  `??_9@$B<offset>AE` (the offset in the decorated-name number encoding: `3` is 4, `CE@` is 0x24, `A@` is 0), and the
  instantiated `vtslots_take`'s decorated parameter `P8<owner>@@<cv>E<signature>` names the final overrider's class
  and its exact decorated name. A probe that doesn't compile is dropped by its error line. The thunk offsets are set
  against the CodeView slots (`tool:` lines report a disagreement). Retail: the `??_7` vtables layoutcheck discovers
  (matched functions' relocations, config/vtables.txt) and symbols.txt's claims, length as layoutcheck reads it;
  each slot's target is named by the annotations and claims at that address. Verdicts: `moved` (our method is
  annotated at the address retail holds in another slot of this vtable), `wrong` (annotated elsewhere), `name`
  (retail holds another method of the chain), `override` (retail's class has its own function where ours inherits
  the base's, or the other way round, read against the primary base's retail vtable; identical bodies are flagged:
  a second kept copy of an inline looks the same), `pure`, `retail-only`/`ours-only`, `agree`, `agree-name` and
  `unknown` (no identity on either side; `--leads` lists those retail targets with the method our slot names there,
  candidates for annotation). It also reports `inconsistent` classes whose slots differ between two units that
  define them (one class, two definitions; a `<windows.h>` A/W macro renaming a virtual is the usual cause), for
  every polymorphic class. Out of scope: secondary vtables of multiple inheritance (only the vtable at offset 0,
  `??_7C@@6B<base>@@@`, is checked) and classes with virtual bases. A report only.
- **`c2dbg`** (`tools/c2dbg.py <unit> | --source <file> [--flags "..."]`, then `--bp <addr> [--args N] [--regs
  ecx,edx] [--ret] [--max-hits N] [--stop-after N]`, `--where`, `--function <substring>`, `--warnings`,
  `--functions`, `--cl-output`, `--undecorate`): a debugger harness over the Win32 debug API that compiles one unit
  with CL.EXE exactly as `build` does (the unit's flags, compiler and environment; the object goes to build/c2dbg/,
  never build/obj) and stops the back end, C2.DLL, at the given addresses: int3 bytes written into the live process
  once the DLL loads, so tools/vc6 is never touched. An address is a Ghidra address of C2.DLL (relocated by the load
  base) or an RVA written `+0x...`; `--module` names another DLL. Each hit prints the address, its hit number, the
  stack arguments and registers asked for, and with `--ret` the return value; at the end, the hit counts and the
  module's load base and timestamp (SP5's C2.DLL is 39A4A91A, build 8966). For SP5 only: `--where` names the
  function the back end is compiling at each hit, `--function` keeps the hits inside matching functions,
  `--warnings` prints its warning calls (C4710/C4711 with the function name), `--functions` lists the functions it
  compiles in order. As a library (tools/c2trace.py): `@c2dbg.probe(address)` registers a `handler(dbg, ctx)`,
  `install(dbg)` arms the registry, and `ctx` gives the registers, memory readers (`u32`, `cstr`, `struct` by a
  layout), `arg(i)` and `on_return(cb)`; the module docstring has the contract. Child processes are followed, and the
  compiler is terminated when a handler raises or the run is interrupted. In Git Bash, set `MSYS_NO_PATHCONV=1` so
  `--flags "/O2"` isn't rewritten as a path.
- **Import claims**: `build`, `match` and the gate check every `__imp_` entry of `config/symbols.txt` against the
  image's import table, and report a slot that imports another name as a `DATA:` problem.

## Ghidra sync (`ghidra names`)

`merge_lane.py` refreshes the three exports at every merge (by hand, after `build`: `python tools/ghidra/export_types.py`,
`export_names.py`, `export_data.py`); then run `ApplyStbcAll.java` in Ghidra's Script Manager and save. It runs, in
this order, `ApplyStbcTypes.java` on `build/ghidra_types.json` (every class, struct, union and enum from the
compiler's own `/Z7` type records: flattened members, typed `<Class>_vftable` structures), `ApplyStbcNames.java` on
`build/ghidra_names.csv` (function names, and signatures from the decorated names; it creates functions Ghidra's
analysis missed) and `ApplyStbcData.java` on `build/ghidra_data.csv` (globals, typed through their decorated
names); then save. Workers may also rename single matched functions through the MCP `rename_function_by_address`.

## Static initialisers (`statics.py`)

Both commands read `build/report.json` and `build/obj`, so run a `build` (or `match` of the units) first. `match`
refreshes its units' rows in the report (when a report exists); `diff` compiles without refreshing them, so `renumber`
leaves any unit whose object is newer than the report and says to `match` it.

- `python tools/statics.py renumber` rewrites `_$E` annotations whose compiled numbers shifted (a header the unit
  includes gained a static). `merge_lane.py check` and `merge` run it. It settles each failing address that exactly
  one of the unit's initialisers fits, bytes and relocation targets, then one shift per unit (or one per block of
  the unit's annotations, in text order, when the rows agree block by block), then names every `jmp` stub of a
  renumbered unit after the body it enters (the stub is not told apart by its bytes). A row that was already failing
  in `config/baseline.json` doesn't vote: it takes its block's shift and is printed `LEFT ... left failing: <addr>`,
  and the unit is renumbered anyway. A shift keeps a run's internal order whatever it is: a failing row whose bytes
  fit its own name and a twin's (byte-identical exit destructors, numbered in either address order) votes with its
  allowed shifts when the other rows alone leave more than one (MetaSurfacePolygonizer's 0x4B5620 `_$E435` and
  0x4B5630 `_$E434`, both +1). An address settled alone whose number another annotation ends with (its targets
  were read from the stale annotations) joins the shift instead. An annotation that no initialiser of its unit fits
  is a source problem, not a shift, and is left alone. A unit whose rows really disagree ("no single shift fits"),
  or whose renumbering would still give one number to two annotations, is left as it was, and printed.
- `python tools/statics.py place-all [--write] [--evict] [--units <unit>... | --changed [<base>]]` places the NetImmerse registration stubs
  (NiStreamMacros.h: a `call NiXxx::Register; mov [static], eax; ret` body and the `jmp` stub entering it) in their
  units. The linker lays out `.text` and `.bss` in the same object order, so the bodies cut into blocks wherever every
  earlier store lies below every later one; between two such cuts the stores interleave, and the block is one unit's.
  A block goes to the one unit with `.bss` data among its stores (its DATA annotations, the statics its matched
  functions reach and those its own non-registration `_$E` initialisers write, `<iostream>` quads excepted), else the
  one unit with annotated code among its stubs, else the one unit with data between the block's last store and the
  next block's first that registers something there (a unit's own statics follow its registration stores), else the
  unit whose blocks surround it. A unit that owns a block by data inside it owns no other block by code or gap data.
  An annotated stub moves only when that evidence places it elsewhere and the numbers it needs are free; one it can't
  re-home stays, and `--evict` drops it instead (a lost match, for `regress --accept`). Stubs nobody holds then go to
  the unit of the code right before the block, or to the next unit when unannotated code comes first. A unit takes a
  stub only when its object fits it, bytes and Register target, each initialiser once. Without `--write` it prints the
  units it would change; it is idempotent, so regenerate its commit after a rebase by rerunning it. Rerun it when a
  unit gets its first annotated function or new matched functions that reach its `.bss`.
  - A unit whose registration annotations don't all match (its numbers shifted) is left as it is and printed: run
    `renumber` first. Placing around stale numbers adds stubs under the wrong ones.
  - `--units` limits what changes to the named units, and `--changed [<base>]` to the units whose `.c`/`.cpp` the
    branch changed against `<base>`'s merge base (default `main`), working tree included. The evidence still comes
    from every unit, but a stub moves only between units of the scope, and only they are written: a lane places its
    own units' stubs without touching the units other lanes work on. A stub another unit holds stays there; name
    both units to move it. `merge_lane.py` doesn't run `place-all`: placing is an ownership decision, made in a lane
    and reviewed with it.
- `python tools/statics.py place <unit> <lo> <hi> [--write]` annotates one unit's registration stubs in a range, when
  the caller has the evidence of ownership.

The tool's rules have tests: `python -m unittest discover -s tools/tests`.
