# The user's rulings in force

`AGENTS.md`, "Policies", states the principles every agent works by. This file holds the rulings behind them and the
cases they decided, by topic. Each keeps its date. A ruling that a later one replaced is removed, not kept beside it
(version control has it). Only the user adds or changes a ruling.

## Merge acceptance: lost matches and dips

- **Declaration-sensitive lost matches are accepted at merge** (user's ruling, 2026-11-11). Some functions' frame-slot
  order flips with the declarations their unit sees (GraphicsMenu::ChangeDisplayMode, PyEmbed::SaveModuleState). A
  lost match is accepted with `merge_lane.py merge --accept <addr>` when all three hold: the function's source is
  unchanged, its diff is only frame slots or register picks, and the trigger is an evidence-backed header addition
  the lane needs. The function gets a `// PARKED:` note naming the wall and a card entry, and the merge report lists
  it. Never tune headers to hold such a function. One named function is covered for a schedule shift too: in
  WeaponSystem_GetTargetList (0x585F40), one load scheduled one instruction apart, which flips with other units'
  headers (user's ruling, 2026-11-15). Any other lost match still goes to the user.
- **The functions ruled declaration-sensitive are listed in `config/declaration_sensitive.txt`** (user's ruling,
  2026-11-16). `merge_lane.py` accepts a listed function's lost match by itself when its source is unchanged and its
  diff is only frame slots, register picks or (for the functions so marked) one schedule shift; it writes the card
  entry and lists the flip. The summary prints the listed functions' state on a line of their own, which explains
  the headline's moves when they flip (they stay counted in it; a frame-size change is still a regression). Only the
  user adds a function to the list.
- **Evidence-backed dips in drafts are accepted at merge** (user's ruling, 2026-11-11). A score dip in an edited,
  non-matching function is accepted with `merge_lane.py merge --accept <addr>` when the change is an evidence-backed
  correction (a signature, a type, a structure) recorded on the function's card; the merge report lists it. A
  dip from a spelling experiment is not covered, and lost matches stay under the rules above.

## Declaration-sensitive and declaration-count functions

- **Declaration-count functions** (user's rulings, 2026-11-17; mechanism in `docs/levers/walls.md`, probe
  `tools/ctxscan.py`). A function under 1000 bytes is never parked as declaration-sensitive without a `ctxscan` run or a
  header-only A/B that flips it. A listed function's lost match is accepted only between its known byte states. A
  count-decided function may be restored by a REAL declaration or include only: a header the original plausibly
  included, or a declaration that exists in the lineage or the tree, with its evidence in a comment; each use is
  listed at merge like an invented helper. Dummy declarations stay a fakematch.
- **DisableGlossMaps is declaration-sensitive** (user's ruling, 2026-11-19): its loop's back edge lands before or after
  the inlined GetAt's bounds check with the declarations ModelHelpers sees (ctxscan: exact with 1-63 more, 99.9 with
  0), flipped by NiDynamicEffectState.h's lineage Get/SetCachedState with NiPropertyState.h's SwapProperties; 0x45CD20
  joins `config/declaration_sensitive.txt`.
- **NiMorphController::CreateObject is declaration-sensitive** (user's ruling, 2026-11-20): exact only at some
  declaration counts (ctxscan: 13, 26-33, 39-60), at the edge of a window, so NetImmerse header additions flip it;
  0x7A8DD0 joins `config/declaration_sensitive.txt` (frame/register).

## Invented helpers and invented text

One test, stated from the user's rulings of 2026-11-16, 2026-11-18, 2026-11-21 and 2026-11-22. An inline helper,
accessor or macro that nothing in retail names is marked `(invented)` in a comment at the code, and `merge_lane.py`
lists every new one beside the BUG and RULE-EXCEPTION notes. Whether the function it closes counts depends on what
proves that the helper *existed*; the name may be invented in every row, and stays marked.

| evidence that the helper existed | the function it closes |
|---|---|
| a sibling or debug build shows it (DamageTool's `Slice::GetNumBoundaryEntries`, an AnimOnly body), or the lineage text has it | `MATCH` |
| retail's inline `unsigned short` widening idiom (`mov reg16, [m]; and reg32, 0xffff`, which VC6 emits only for an inline function's `unsigned short` result), even without a sibling | `MATCH` |
| an existing accessor whose callers copy its `float` result through `fld`/`fstp` in retail: the spill shows a second copy level inside the accessor, and only its spelling is unknown (an inner inline, a named local or a value wrapper give the same code) | `MATCH`; the spelling is marked `(invented)` at the accessor |
| retail copies a `float` through `fld`/`fstp` where the plain expression gives integer moves: the copy is an instruction of its own in retail, so the copy level is the original's, whether it sits in the accessor (a second or a third level) or in the caller as a member or an accessor result read into a named local (`float top = clientArea.top;`) | `MATCH`; an invented spelling inside an accessor is marked `(invented)` |
| only inline-budget arithmetic, the schedule, or the bytes closing | `MATCH-PENDING`: the exact bytes kept in the tree, a ledger row in `config/exceptions.md` |
| the evidence points at a call, and the callee's body is written out by hand instead | `MATCH-PENDING` |
| retail's bytes show the member read directly | no helper: the callers read the member |

Invented text follows the same marking: string literals nothing in retail shows (the arguments of a compiled-out
trace) are marked `(invented)` and listed at merge.

The cases ruled so far:

- `MATCH` on a sibling: VoxelSlice's solid and surface count accessors (retail re-extends the word at each use;
  DamageTool shows the class had such accessors) close GetSpan, IsSolid and CreateSolidLines (2026-11-21, confirmed
  after a correction).
- `MATCH` on the widening idiom alone: SimpleFeatureCollector's GetVertexCount and GetTriangleCount close
  MetaSurfacePolygonizer::CollectFeatures (2026-11-22).
- `MATCH` on the copy level (2026-11-22, replacing the 2026-11-18 verdict for these accessors): the fpu-copy
  two-level accessors GetCondition, GetConditionPercentage, GetMainBatteryPower, GetBackupBatteryPower,
  GetPowerPercentageWanted, GetLastUpdateLength, ImpulseEngineProperty::GetMaxSpeed, GetCurMaxSpeed,
  TGIconManager's GetScreenWidth and GetScreenHeight, EnergyWeapon::GetChargeLevel, and the 20 callers they close.
- `MATCH` on a copy retail shows as its own instruction (2026-11-22): GetPowerPercentage's third copy level
  (ImpulseEngineSubsystem::Update's four spills), and a member read into a named local before the call
  (STTopLevelMenu::VirtualSlot114 and TGParagraph::VirtualSlot92, both `top`). A local that only moves a load, with
  no copy of its own in retail, is still a carrier and stays MATCH-PENDING (STMenu::GetDesiredSize,
  ReticleWindow::ReticleWindow, TorpedoController::Update).
- `MATCH-PENDING`, evidence-free (2026-11-18, 2026-11-22): GraphicsModeInfo's setters, SetHasSurfaces,
  TGRootPane's by-value accessor,
  StartFiringEvent::GetTargetID, NiNode::SetVisual, DamagePolygonizer's debugNode accessor, the Property::Copy
  axis-copy wrapper, GlowAccumulator::SwapProperties, TGObject's sprintf wrappers, TGModelManager's AddUser;
  OptimizedFireScript::IsLineOfFireBlocked's `&TGPoint3( point )` until other evidence.
- `MATCH-PENDING`, a body written out where the evidence points at a call: ViewScreenObject's GetRemoteCam
  (2026-11-16).
- Read directly: TGEventHandlerObject's `classTables`; the invented `GetClassTables()` accessor went (2026-11-18).

- **A loop whose body compiles to nothing is a release-empty trace macro** (user's ruling, 2026-11-16): one invented,
  marked macro in a shared game header that expands to nothing in the release build, used as the original's
  statement (ProximityManager::DumpCollisions, SetClass::PostFixUp). An empty loop body without it stays R17 fake code.

## Names

- **Placeholder names wait for evidence while their unit is open** (user's ruling, 2026-11-16). The mechanical name
  evidence is used up (`tools/names.py`): in a unit with open functions, lanes name a placeholder only from direct
  evidence or Gamebryo lineage.
- **Behaviour-derived names wait until the tree matches, unless a rename helps the remaining work** (user's
  ruling, 2026-11-22, after the naming pilot). The pilot's 21 names (7 fully matched game units, marked
  `(name invented)`, byte-neutral) are accepted and stay. No further naming lane runs until matching is complete or
  as close as it can get. Before then a placeholder takes a behaviour-derived name only where the name actively
  helps open work (an open function's reading depends on knowing what the member, slot or function is): the lane
  doing that work makes the rename, marks it `(name invented)`, proves it byte-neutral (`tools/c2il.py compare`,
  then the gate, since a name reaches the bytes through stringizing and decorated names) and says in its report what
  the name unblocked. Direct evidence and lineage name a placeholder at any time, as before.
- **A NetImmerse local keeps its original Hungarian name where the debug copy's frame order proves it** (user's
  ruling, 2026-11-20): `uiWidth`, `pSaveBase`, `bResult` where AnimOnly's /Od slot order needs that name; every other
  local and every member stays in the project's style. Asserts are restored with `<assert.h>`'s `assert()`.
  A name chosen only because its hash puts the /Od frame slots in AnimOnly's order (the slot order follows a hash
  of the names, so it narrows a name without proving it) is kept, marked `(name invented; fits the /Od frame order)`
  (user's ruling, 2026-11-20). The same holds for a name chosen to fit the frame order of a function retail itself
  built at /Od (NiProcessorSpecificCode's Transform*Basic `r`; user's ruling, 2026-11-21).

## Listed apart: neither matched nor open

- **A function binary-patched in retail is written as the original and listed apart as PATCHED** (user's ruling,
  2026-11-16). FindDiscWithContents (0x460880) reads `test; nop; nop; return 0` with the original body left dead after
  the return; no compiler emits that. Its source is the original function, reconstructed from the dead body, under a
  `// PATCHED: <what retail's bytes show instead>` note below the annotation. It is recorded in `config/exceptions.md`
  with status PATCHED and counted neither matched nor open, like ORIGINAL-ASM.
- **CRT routines written wholly in assembly are listed as ORIGINAL-ASM, not written** (user's ruling, 2026-11-15).
  memcpy, strlen, _ftol, the ll* helpers, chkstk, exsup and the like come from VC6's own `.asm` sources: each is
  recorded in `config/exceptions.md` (address, name, the VC6 `.asm` file) with status ORIGINAL-ASM, keeps its
  `config/symbols.txt` identity, and is counted neither matched nor open. No assembler is added to the toolchain.
- **Import thunks are listed apart as IMPORT-THUNK** (user's ruling, 2026-11-16). The 6-byte `jmp [__imp_X]` stubs come
  from the import libraries, not from source: each keeps its `config/symbols.txt` identity, is recorded in
  `config/exceptions.md` with status IMPORT-THUNK, and is counted neither matched nor open, like ORIGINAL-ASM. CRT
  math routines with a C source in VC6's CRT are written; the assembly-only ones are ORIGINAL-ASM.
- **Library data linked into `.text` is listed apart as LIBRARY-DATA** (user's ruling, 2026-11-18). TGUIBatch's
  0x78B830 and 0x78C8B0 are `dinput.lib`'s DIOBJECTDATAFORMAT tables, not source: each is recorded in
  `config/exceptions.md` (address, the library object, byte count) with status LIBRARY-DATA and counted neither
  matched nor open, like IMPORT-THUNK.

## Build flags and precompiled headers

- **Game units build without a precompiled header; NetImmerse keeps /YX** (user's ruling, 2026-11-20, replacing the
  2026-11-18 ruling that put both on /YX): `auto_pch` off for src/game, on for src/netimmerse. Over the tree it puts 344 more
  game sections in retail's order; it regains GetArcPosition, ServerListCallBack and DisableGlossMaps and loses
  DamagePolygonizer::CreateUnknown138, PhaserProperty's Unknown686C80 and Unknown687A60,
  PositionOrientationProperty::Unknown689830 and ViewScreenNode::SetOffTexture: accepted losses, PARKED with cards
  (declaration-count flips; `docs/c2-backend.md`, "A/B over the tree"). A game unit may build with /YX on its own
  (`"auto_pch_units"` in config/project.json) only when BOTH its matches and its retail section order
  (`comdat.py order`) improve with it, the evidence recorded beside the entry (user's ruling, 2026-11-20).
- **A game unit may build with /Ob2 on its own on the same terms as /YX** (user's ruling, 2026-11-22): an `"overrides"`
  entry in config/project.json only when both its matches and its retail section order (`comdat.py order`) improve, the
  evidence recorded (TGFontGroup: the only game unit whose `<iostream>` quads have /Ob2's merged shape; 75/75 initialisers,
  order 52/75 against 47/140). HullClass's size-optimisation pragma around its handler table (0x560400) is kept as
  MATCH-PENDING (bytes-only evidence), recorded in config/exceptions.md.

## NetImmerse: the debug build and the Gamebryo lineage

- **The SDK's debug NetImmerse 3.1 is evidence for NetImmerse names, files and statements** (user's rulings, 2026-11-20;
  `tools/sdkdebug.py`, `docs/evidence.md`). Members named by its asserts take the original name in the project's
  style (`m_pVertex` -> `vertex`, `m_pChunk` -> `chunk`; NiDDImage's `unknown58` -> `formatted`), in a rename lane run
  alone. Units take the original file names (NiD3DRender -> NiDirect3D, NiTextureManager -> D3DTextureManager,
  NiAutoNormalParticles -> NiAutoParticles...); class names stay as the engine's strings give them. NiTArray::Compact
  takes the text its debug copy proves (Gamebryo's shape, locals `j` and `pSaveBase`), and the loss of
  TGIconManager::DeleteIconGroup (0x72A210) that its inline cost causes is accepted at merge, PARKED with a card.
  NiObject::DecRefCount likewise carries its proven `assert( count >= 0 )` (retail's costs need it: Terminate and
  RenderPass); SetClass's and BridgeObjectClass's destructors and ~NiTMap 0x417270 are accepted losses whose source
  differs from the original somewhere. The proven asserts of NiTArray SetSize/SetAt (their losers are /Od-identical)
  and NiTList RemoveHead/RemovePos stay out, each with its outcome at the code.
- **A statement the debug build proves keeps its `// BUG: redundant:` label even where release drops it** (user's
  ruling, 2026-11-22): NiBezierCylinder::VirtualSlot49's second `split = true` (AnimOnly 0x4F40A1) stays, its note naming
  the debug copy as the instruction of its own; the label still never excuses a statement no build shows.
- **The bytes win over the Gamebryo 1.1.2 text where they disagree** (user's ruling, 2026-11-17). BC's NetImmerse is
  older than 1.1.2: a body that must differ from the lineage text to match (NiACos's single `else`, NiPoint2::Length's
  `sqrtf`, UnitCross without `else`) keeps retail's form with a comment at the code naming the functions that decide
  it; `docs/levers/lineage.md` lists them. Missing lineage members (NiPoint2's operators, NiMath's functions) are added
  as Gamebryo writes them; one whose BC spelling the text can't settle (NiUnitRandom) waits for the bytes.
- **Gamebryo lineage over a declaration-sensitive match** (user's ruling, 2026-11-16): the NiMaterialProperty setters
  call `SetChanged( true )` as NIMATERIALPROPERTY.INL does, and Editor::HighlightNode's resulting NiPointer-copy
  flip is accepted at merge like the declaration-sensitive losses above.
- **Gamebryo's NiTimeController::GetStartTime is added** (user's ruling, 2026-11-18): NiMultiTextureProperty::CreateClone's
  resulting flip joins `config/declaration_sensitive.txt`. The SetAtGrow brace trade and a NiColor copy constructor are
  not taken. WarpEvent is header-only, its copies annotated `@game/UtopiaModule`. TGDirectory's 0x280-byte find data
  stays an identity-TODO.

## Source shape and placement

- **Source is never shaped to avoid a `_$E` renumber** (user's ruling, 2026-11-16). A static moved out of its class,
  an include left out, a cast on a forward-declared type: none is justified by the renumber it saves. The renumber
  is tooling work (`statics.py renumber`); a lane that can't run one reports the change for a lane that can.
- **Helpers retail places at their definition lose `static`** (user's ruling, 2026-11-20): a function retail emits
  at its definition, before its first caller, had external linkage in the original (`docs/c2-backend.md`,
  "Held-back statics"), so it is written without `static`; NiOBBLeaf's four are Gamebryo 1.1.2's protected static
  members under their lineage names. Which class owns each game helper stays an identity question.
- **`@unit` names the unit whose copy retail kept, strictly** (user's ruling, 2026-11-18 night): a header inline is never
  annotated to another unit's byte-identical copy (NiD3DPixelFormatEntry::Register 0x7CAFD0 stays NO-SYMBOL until its
  frame-buffer caller calls it out of line).
- **Definitions follow retail's order** (user's rulings, 2026-11-19). A unit's own definitions are placed in retail's
  address order (`tools/linkready.py`, `tools/comdat.py order`), which is the original file's definition order and
  the order a real link needs; the file-scope declarations they use (`#define`, `static` helpers and tables, enums)
  move with them within the `.cpp`. A move is kept only when no function loses its match and no open function's
  score drops; inline copies, `_$E` stubs and headers are not moved by hand.
- **Static-initialiser runs are placed by the `.bss` they write** (user's ruling, 2026-11-15). A run of `_$E`
  stubs held by annotations its `.bss` stores contradict, or held by no unit, is re-placed in its owning unit
  (`statics.py place-all --evict`), with a stub-only unit under an invented, marked name where the owner has no
  source yet. Stub losses this causes are accepted at merge and listed; written functions are never moved this way.
- **SortedArray<TGNetPlayer, int>'s explicit instantiation is written in ChangeRenderedSetAction.cpp** (user's ruling,
  2026-11-22): retail keeps all its members out of line, unreferenced ones included, among that unit's initialisers
  (0x401630-0x401DB2), which only an explicit instantiation emits; retail keeping this unit's copies is the placement
  evidence, stated in a comment at the line. The original file that held it stays unknown.
- **The SortedArray::Get trade is taken** (user's ruling, 2026-11-18): Get with its early return (+4); a lane chases
  _wrap_TGNetGroup_Validate's operand order, and if it can't be recovered its loss is accepted and recorded on its card.

## Referents and upstream text

- **A constant retail keeps in two copies is two spellings** (user's ruling, 2026-11-17): the linker folds one name
  to one copy, so a site reading the other copy is chased as a source spelling (a named `const float`, a double
  literal), not accepted by relaxing the referent rule.
- **The service-pack CRT is the pristine for the files retail took from it** (user's ruling, 2026-11-19): retail's heap
  follows VC6 SP5, so `upstream/vc6-crt/` holds the SP5 text (`vc98/crt/src`) of the ten files SP5 changed (calloc.c,
  free.c, heapinit.c, malloc.c, msize.c, realloc.c, sbheap.c, strftime.c, tzset.c, winheap.h) and the RTM text of the
  rest; a `src/crt` unit of those files is written from the SP5 text (realloc.c).

## Process

- **Fast variant trials are a bounded tool** (user's ruling, 2026-11-18): one variant first shown to move the output, then
  at most about 20 hand-written variants per function, never generated mutations; results are clues, and the spelling
  still needs evidence.
- **A lane can't change the verifier** (user's ruling, 2026-11-19). A PreToolUse hook (`.claude/settings.json`,
  `tools/hooks/protect_verifier.py`) refuses, inside a slot worktree, any write to `config/baseline.json`,
  `tools/decomp.py`, `tools/rules.py`, `tools/merge_lane.py`, `orig/` or `build/gate/`, and `decomp.py report
  --update-baseline`. A lane that needs one changed reports it; the main checkout is not covered.
- **The levers are audited every 9 lane merges** (user's ruling, 2026-11-21): `tools/levercheck.py due` counts the
  merges since the last `docs: lever audit` commit. The orchestrator then checks every entry's claims against the
  functions' current status and the rulings, removes score snapshots and diary text, merges duplicates, and archives
  stale entries to `docs/levers-archive.md` with a reason; the commit subject starts `docs: lever audit`.
- **Cards** (`cards/<addr>.jsonl`) are a data record like `config/baseline.json`, not documentation, so the
  rule against logs in docs doesn't apply to them (user's ruling, 2026-11-09). The merge writes their score entries;
  workers append their tries with `tools/cards.py add`, never by hand-editing old lines.
  A try that changes or A/B's a shared header carries a `--group hdr-<slug>` (`hdr-nitlistblock-ctor`) on every card it
  moved, and names the header file; `openwork.py headers` and the packet then show other lanes that the header
  question was settled, or that no lane has taken it (2026-11-21).
