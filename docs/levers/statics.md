# Static initialisers and unit layout

`_$E` numbering, registration stubs, guard bytes, and what the static layout says about unit boundaries. Read
`README.md` for how entries are written; `tools/units.py` and `tools/comdat.py layout` read the layout this file
describes.

## Static initialisers and unit layout

### Static initialisers and unit layout: shapes and numbering

- **The shapes of `_$E` functions.** A unit including `<iostream>` gets eight, the `ios_base::Init` quad then the
  `_Winit` quad, each laid out (hi, body, atexit thunk, exit destructor) in object order and usually numbered body n,
  exit n+1, thunk n+2, hi n+3. A handler table's four are (hi, body, atexit thunk, exit), numbered body n, exit n+1,
  thunk n+3, hi n+4 when no `_$S` sits between; a `.cpp`-scope `classHandlers` gives four, the fourth a 0x30-byte
  guard variant (Episode, Game, Mission). `static float X = NI_INFINITY;` is a pair (body n, `jmp` hi n+1), 0x20
  apart. Every unit including `<string>` or `<iostream>` also carries the locale-id pair: a 32-byte test-and-set (`mov
  cl, [0x9B3E34]`) and a 16-byte atexit (`push 0x402330`), sharing the guard byte of `std::ctype<unsigned short>::id`.
  Dynamic initialisers of the unit's own globals are annotated with the names its `.obj` gives them (`_$E173`,
  ArtificialIntelligence's aiMap): the higher number of a body/`jmp` pair is the `jmp`.
- **Runs of `jmp; call NiXxx::Register; mov [static], eax; ret` are the NetImmerse headers' class registrations**
  (`NiRegisterStream` in NiStreamMacros.h, a header static per streamable class; Gamebryo 1.1.2 keeps the form in
  NiAnimationKeyMacros.h). A unit's run is the post-order of its NetImmerse includes; NiMain.h gives the alphabetical
  full set (140 slots). The stubs are byte-identical and `.bss` resolves at retail's address, so a byte match doesn't
  prove placement. Registration runs give header include order: NiNode.h includes NiDynamicEffect.h; NiCamera.h
  includes NiRender.h, NiNode.h, then NiScreenPolygon.h (Gamebryo 1.2.2 agrees).
- **Run sizes name the headers:** a 70-stub NetImmerse run is NiMain.h; 73 adds NiListener/NiSoundSystem/NiSource;
  117-120 add NiAnimation.h. A run the unit's `.bss` owns but its object fits only in part means the unit lacks one
  of those includes. A UI or ST unit whose registration run has holes
  is missing `#include "NiMain.h"`; add it after the unit's includes and run `statics.py place-all --units ...`. The
  NiListener / NiSoundSystem / NiSource triple follows the NiMain run, so those headers go after NiMain.h; included
  earlier, the triple lands on the neighbouring unit's (ModalDialogWindow 0x5040F0). A class's parameter types can
  explain a small registration block: `NiRotKey::KeyType` parameters in TGAnimationEntry.h give
  TGAnimationManagerClass its three key registrations, while the NiMain run before it is TGAnimBlender's (its rtti and
  `<iostream>` bytes lie among the stores).
- **The `<iostream>` run's shape names the inline flag** (docs/toolchain.md). Under `/Ob2` each `<iostream>`
  initialiser is one function (`mov ecx; call Init; push exit; call atexit`), the locale pair one 0x20 function and
  the NiMain registrations have no `jmp` stubs: a NetImmerse run is five functions, the locale guard plus two init/exit
  pairs, against ten under `/Ob1`. About 151 NetImmerse units carry one, and about 35 (keys, controllers,
  NiListener/NiSoundSystem/NiSource, particles) have the quads in their `.bss`; no Gamebryo header explains it
  *(open: a shared header including `<iostream>`?)*. A game unit with the `/Ob2` shape is built with `/Ob2` on /YX's
  terms (user's ruling): an `overrides` entry only when both its matches and its retail section order (`comdat.py
  order`) improve, the evidence recorded beside it (TGFontGroup, the only one). Single registration stubs alone don't
  show the flag: under `/Ob1` their bodies match too, and the `jmp` stubs are left without a retail address.
- **A quad packed without alignment, its exit guard tested with `test byte ptr [guard], 1`,** is a definition
  compiled favouring size, `#pragma optimize( "s", on )` around it (HullClass::classHandlers, 0x560400): `/O2` gives
  16-byte slots and `mov cl, [guard]`. The evidence is the bytes alone, so the initialisers it closes are
  MATCH-PENDING (user's ruling; `config/exceptions.md`).
- **`??_H` (vector constructor iterator) is emitted wherever a local array or `new[]` of a class with a constructor
  and no destructor is compiled, even when the empty constructor's loop is inlined away** (CameraMode's
  `NiPoint3 points[4]` in IdealControlledCameraMode::Unknown421410, itself *(open)* on a register wall; the inline
  constructors of NiOBBox.h and TextureAnimManager.h's TextureAnimTrack): the object retail kept it from had such
  code (ChangeRenderedSetAction's copy at 0x401DC0, which nothing in that unit now explains).
- **`_$E` numbers count every static initialiser a unit sees, its headers' included,** so they shift whenever a header
  the unit includes gains one: a static data member (a class-scope `classHandlers` table; ProximityCheck's list; a
  NetImmerse header's static data member, such as NiBound's two, shifted every includer by +2), a class-scope `static
  const float` with an out-of-class definition (PhaserBank's three damage factors moved every includer by 3), or an
  inline function definition (eight NetImmerse accessors moved AppWrap by 8). A namespace-scope `extern`, an
  out-of-line declaration, member functions, enums and operators don't. Removing a static member's declaration shifts
  them the other way, except the stubs emitted before it (the `<iostream>` quads; TGObject::loadSizes made
  file-scope). A static added to the unit before an initialiser shifts it too (MetaVolumePolygonizer's `mesh`). Shifts
  stack across lanes. Run `statics.py renumber` after the build (`merge_lane.py` runs it); `regress` reports a missed
  shift as a lost match with unchanged source. Read a number from the unit's `.obj`, never guess, never order includes
  to keep a number, and never move data out of its class to avoid a renumber.

### Static initialisers and unit layout: guard bytes

- **A unit's statics share one `$S` guard byte until a function definition separates them:** Backdrop's handler
  table (guard 0x98D804) is defined first, then AddToSet..Cast, then the array and the map (their own guard,
  0x98D80C), in retail's `.text` order; defining the three together packed all guards in one byte. Two class statics
  separated by a function definition get separate bytes (WeaponSystem: classHandlers, Cast, visibleTargets), and two
  class statics whose quads sit apart with code between them are defined with that function between them
  (SensorSubsystem::sensorData). An inline function defined in the `.cpp` separates guard bytes too, without code in
  place: CameraMode's registry map took a guard byte of its own (bit 1 at 0x97EE18) once `CameraModeRegistration`,
  whose constructor is inline, was defined before `CameraMode::creators` rather than after; the stubs stay contiguous in
  `.text`. A guard byte apart from the NiRTTIs' means a function definition separates them (NiDDImage, NiImage). After
  such a move, check each `_$E` against `statics.candidates`: the stub of a group is its body's number plus the group
  size.
- **An exit-time destructor's guard bit is its definition order among the unit's guarded statics:** moving the
  registry map after the handler tables fixed 9 exit destructors (CameraMode); a missing earlier static shifts every
  later bit.
- **Several classes' handler tables sharing one guard byte** (bits 1, 2, 4, 8) are defined in one unit in retail's
  order (TopWindow.cpp holds MainWindow's, ConsoleWindow's and OptionsWindow's; DamageDisplay and DamageIcon at
  0x98A4B0, bits 1 and 2).
- **Guarded or not tells the kind of static.** A file-scope static's exit destructor has no `$S` guard; a class
  static's has one: an unguarded retail exit destructor means the original declared a file-scope `static` (LensFlare's
  list at 0x9861E0), and a guarded exit for a `NiPointer` static means a class static (NiDDImage, NiImage). An
  unguarded exit beside guarded ones in one constant group means namespace-scope globals mixed with class statics
  (TGPoint3's MODEL_* are `extern const` globals, ZERO and UNIT_X..UNIT_Z class statics on guard 0x980E00).
- **The default-property init object:** a file-scope object whose constructor runs `new` and whose unguarded exit
  releases it (the 15 properties' `<X>PropertyInit`, NiSoundSystem).
- **A unit's registration objects are defined before its handler table and module;** `.text` follows definition order
  (OptimizedSelectTarget).

### Static initialisers and unit layout: which unit owns a run

- **A stub's owner is the unit whose `.bss` its stores land in.** A unit's `.bss` follows object order, and its own
  `.bss` follows its registration stores: data in the gap after a block's last store places that block (every
  block its data decides agrees), unless the next unit registers nothing of its own (MSJExceptionHandler's data
  follows the run at 0x716130, which is the previous unit's). A block's registration stores are a `.bss` range apart
  from its quad bytes: the data among or just past the stores names the owner (FloatRangeWatcher::RID_INVALID,
  g_kVarManager, UITreeDialog's 0x98B7C4, WeaponPayload's nextID between two runs' store ranges), and so do the
  `<iostream>` bytes the stores surround. Two runs in a row are two units, even with the same include list (WarpSet
  0x4D6D80 and WarpTrail 0x4D8030; TGTraceStream and TGTraceOutput); two classHandlers in one store block are one unit
  (Planet's and Sun's at 0x98E308-0x98E45C). Without such data, the unit with code inside the block owns it, else the
  code before it. `statics.py place-all --write` after a build applies this, the surrounding `<iostream>` bytes
  included; rerun it whenever a unit gets its first annotated function or its includes change. An include added only
  for a misplaced stub shows up when the unit fits its corrected run exactly once the include is gone.
- **An `<iostream>` or handler quad belongs to the unit whose DATA range holds its `mov ecx, imm` variable,** even
  after another unit's code (LensFlare's run held 18 PhaserGraphic annotations); read the compiled numbers from the
  `.obj` (hi n+3, body n, thunk n+2, exit n+1). `place-all` doesn't move quads: move them by their `.bss` variable.
- **A class's NiRTTI can sit just before its unit's stores; its `$S` guard byte, among those stores, is the reliable
  evidence** (BoneStateController's rtti 0x9916E0 after BoneState's stores, its guard 0x991704 in its own run).
- **Header copies bound to a unit (`@unit`) are weak evidence of where its code is:** `.bss` data outranks them
  across blocks (Game's TGObjPtrEvent copies at 0x403290, SortedArray at 0x44B980 pulled runs into the wrong unit).

### Static initialisers and unit layout: unit boundaries

- **A unit's code is contiguous:** its functions, the COMDATs of the classes it constructs (UtopiaModule's object
  holds the class factories and inline copies such as the PowerSubsystem getters and MainWindow's destructor) and the
  static initialisers of the NetImmerse headers it includes; that tells you which unit to annotate a copy in. A
  factory registration followed by NiTMap SetAt calls is `<X>Module::RegisterClasses` (TGNetworkModule).
  NiXxx::Register static initialisers sit between a unit's functions at 16-byte slots (0x69EF60): not unit boundaries.
  Template copies the first object emits sit after that unit's run: TGAction's inline virtuals at 0x402770 place
  0x401450-0x40276F in ChangeRenderedSetAction. A template whose members retail keeps all out of line, unreferenced
  ones included, among one unit's code is an explicit instantiation in that unit: `template class
  SortedArray<TGNetPlayer, int>;` in ChangeRenderedSetAction.cpp (0x401630-0x401DB2; user's ruling), the placement
  evidence stated in a comment at the line, the original file that held it unknown.
- **A game unit's code follows its own quads and run; a NetImmerse unit's code precedes them.** A game unit usually
  has the `<iostream>` quads, the NiMain run, the handler-table or global-object quad, then code and template copies,
  then the locale-id pair at the end; but code can come first with the quad after it (TGOverlayController,
  TGDimmerController), and some game units are laid out functions first: global quad, code, `<iostream>` quads, then
  the run with the locale pair inside it (SayLineEvent, ViewScreenNode, TGModelManager, TGModelPropertyModule). A TG
  unit opens with its `<iostream>` cluster (the ctype guard and its atexit, then eight ios/Winit initialisers), with
  template COMDATs possibly between the guard and the eight; that marks unit boundaries (TGTimerList split from
  TGEventQueue at 0x6DE4F0), and quads after one unit's code open the next (TGConditionHandler 0x701500,
  TGScriptAction 0x6FF660). A game unit's run can end in its own global-object quad and code before the locale pair:
  the module's DATA beside the stores owns the run, not the next `.cpp` (TGLocalizationModule 0x6CE440-0x6CEFB0). A
  NetImmerse unit is laid out code, then its `<iostream>` run, then its registrations and inline copies: the run
  belongs to the code before it; its `<iostream>` bytes sit beside that unit's `rtti` data, the registrations' stores
  surround them, and the inline copies after it are called by the code before it (NiBezierPatch::UpdateWorldSets).
- **An `<iostream>` quad is not always at the head of its unit's stub run:** it can sit inside the unit's code
  (STTopLevelMenu 0x53CDC0), trail it (STTargetMenuSubPane, STTiledIcon, STToggle) or come before it (STWarpButton,
  DamageDisplay). An `<iostream>` pair right after one unit's code and right before a registration run is usually the
  next unit's head (AnimatedTriShapeParticles 0x4D8D90); a pair after a unit's last function with no run after it
  stays the unit's own (STLoadDialog, STSaveDialog). A unit's handler-table initialiser can open it before its
  registration run: `mov ecx, <classHandlers>` at 0x539A70 started STTargetMenuSubPane's unit, ahead of the run at
  0x539D80. An `<iostream>` quad followed by NiMain registrations in a unit's `.text` tail means the unit includes
  `<iostream>` and NiMain.h.
- **A quad/guard pair bounds a unit** (ShipClass.cpp spans three retail units by its pairs). A unit whose run sits
  apart from its code is two units (TGIconGroupRegistration at 0x747A60 was inside TGIconManager.cpp). A second run
  after a unit's code means the `.cpp` is two retail units (NiAVObject, NiBound, NiTriShapeData and NiTriStripsData,
  split into `_Win32.cpp` halves; NiApplication, NiTextureManager, NiPropertyState and others not split yet). Gamebryo
  1.2.2's `CoreLibs/NiMain/Win32/*_Win32.cpp` name the platform halves of split NetImmerse units (UpdateWorldData,
  CalculateNormals, Merge/Update). A run in the middle of a `.cpp`'s code is a unit boundary only when functions follow
  it before the next unit's own quads. Two COMDAT copies of one destructor don't prove two units: check that each unit
  would still be contiguous and carry its own locale pair before splitting (TGTrace, 0x720B50 against 0x720E20).
- **The locale-id pair closes a unit's trailing run:** a pair inside the run after a unit's code makes that run the
  unit's own, not the next unit's leading one (StartFiringEvent, 0x5B8440).
- **Stub-only units.** A run with its own quads and no code beside either neighbour is a stub-only unit. A unit can be
  only its `<iostream>` bytes: a locale pair plus quads whose `.bss` is just the two `<iostream>` bytes, between two
  units' blocks, is a stub-only unit even when code follows it directly (StaticsUnit4B3510). A stub-only NiMain unit is
  a 2,416-byte gap of an `<iostream>` quad, a locale pair and 70 stub pairs whose `<iostream>` bytes lie inside its own
  stores (StaticsUnit6BBC30, StaticsUnit6EE0D0). An unowned statics-only run belongs to the unit whose NiMain.h
  include places it whole (OBBRoot); a stub that fits only with one more include is evidence for that include
  (TGFuzzyTriShape's NiNode.h).
- **A class's vtable places its constructor:** the vtable sits in the `.rdata` of the first unit that emits it; one
  lying after a unit's own `.rdata` tables puts the constructor in that unit (MetaSurfacePolygonizer 0x88C698).
- **Funclets (`.text$x`) are grouped at the end of `.text`**, from 0x870FC0 (after the CRT's import thunks) to the end:
  the game units' EH unwind funclets (`lea ecx,[ebp-x]; jmp dtor`), nothing to write there. Their `jmp ~X` targets name
  which destructor copy each one calls. `python tools/comdat.py layout <unit> [lo hi]` lists a unit's sections in link
  order and fits them against retail.

### Static initialisers and unit layout: the statics tool

- **`statics.py place` for what `place-all` won't do:** `place-all` skips a unit without annotated code, so place a
  statics-only unit with `statics.py place <unit> lo hi --write` (StaticsUnit5BA010); the same fixes a block
  `place-all`'s default order gives to the wrong unit (strip the wrong unit's stubs first so their numbers are free),
  and annotates a run `place-all` leaves (Backdrop: 84 stubs). `place-all` keeps annotations that sit one unit late but
  match ("kept"); to re-home them, drop the stale annotations of the units concerned first, so their numbers are free,
  then run `place-all --units` (the ST units' runs).
- **`statics.py renumber` and failing rows:** it refreshes units with a failing `_$E` row before reading, shifts such a
  unit with its neighbours and prints it as "left failing" (CameraMode's 0x41DFB0); a `jmp` stub its stale targets
  settled wrongly joins the shift (TGObject's 0x6F08B0). It still refuses rows that really disagree ("no single shift
  fits"): shift those by hand, then `match`. Build before renumbering, since renumber reads only non-MATCH rows.
