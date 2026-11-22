# Levers archive

Entries moved out of the levers (docs/levers/) because they no longer apply, were superseded, or recorded a one-off. Kept for the
record only: workers and tools don't read this file. Each entry keeps its original text, under the reason it moved.

## one constant of one unit (MapWindow), matched; the general rule is the `.rdata` constants entry

- **The game's own `1.0f` timer delay at 0x88E5F4** is a named unit constant, not the merged 1.0 at 0x888860
  (MapWindow's periodic update interval).

## a one-time identification of the CRT library members, done

- **LIBCMT.LIB members match retail with relocations masked:** that named 56 CRT routines at once. Byte-identical
  pairs need object layout or relocation targets to resolve.

## superseded: the inline `const TGPoint3 &` overload (Spelling section) closed these wrappers

- **The SWIG wrappers that pass a `TGPoint3` to a `const NiPoint3 &` method** (eight: ShipClass SetImpulse, SetSpeed,
  SetTargetAngularVelocityDirect, TurnTowardLocation, TurnTowardDirection; ProximityManager::GetNearObjects; the two
  property SetOrientation): retail copies the point into a frame temporary, passes its address, and pushes the float
  argument through fld/fstp, so one inline level sits between the wrapper and the out-of-line method. Tried and
  failed: an explicit temporary in the wrapper (fixes the copy, leaves the float), an inline overload taking
  `TGPoint3` by value (chosen, but not inlined under `/GX`), an inline overload taking `NiPoint3` by value over a
  pointer method (worse, and it moves ShipClass's own callers). The wrapper bodies are generated, so the answer is in
  the declarations.

## superseded: one decorated name may bind one address per unit (`@unit`), and both copies of NiPointer<NiAVObject>::operator= are annotated that way; the entry on annotating every unfolded copy holds

- **Never annotate a name retail emitted at two addresses** (NiPointer<NiAVObject>::operator= at 0x416FA0 and
  0x46C4E0): one decorated name binds one address, and the callers of the other copy turn DIFF.

## done: NiPoint2.h declares operator-

- **Open: NiPoint2 lacks `operator-` (out-of-line copy 0x4BAF80, beside `+` 0x4BAF50 and `*` 0x4BAFB0)**; four
  MetaSurfacePolygonizer drafts spell it by components until NiPoint2.h has it.

## done: the map at 0x68 is NiTMap<__int64, DamageVertexIndices>

- **Open: MetaVolumePolygonizer's map at 0x68 holds six unsigned shorts, not an NiPoint3** (SetValue 0x4C3D60 copies
  three dwords; readers stop at 0xFFFF). The retype renames NiTMap.h's instance annotations and blocks 0x4BE640,
  0x4C2F80 and ~MetaVolumePolygonizer (0x4BD4D0).

## superseded: the shared NiLightLoadRecord level (inline.md, "The inline budget", the load-record entry) is the general rule for the lights' CreateObject

- **An empty `XLoadRecord : public NiDynamicEffectLoadRecord {}`** with `stream.SetLoadRecord( new XLoadRecord )`
  gives retail's CreateObject for the lights.

## one-off negative result for one function, with a score snapshot; the try belongs on the function's card

- **Out-of-line Replace*Anim(NULL, 0, NOINTERP, true) three times** is not the inline `ClearData` (50% vs 79%)
  (TGAnimationEntry).

## settled: EngPowerCtrl::MoveSelection matched by writing each direction's branch whole (control-flow.md, the VC6 merge entry)

- *Wall:* two branches ending in the same `SetFocus( child, true ); return;` get one merged tail in this build
  where retail keeps both (EngPowerCtrl::MoveSelection *(open)*).

## stale: TGTimerList's constructor and destructor now match defined in retail's order, not last in the file (from inline.md)

- **TGTimerList's constructor, destructor and ResetTime matched only once defined last in the file,** where retail
  calls them out of line from their own unit. Order alone doesn't stop VC6 expanding a later function (above), so the
  cause is open: the move changes the budget of the functions it passes.

## done: the three RegisterClasses matched once TGEventHandlerObject's classTables was read directly (user's ruling), a one-off (from inline.md)

- **RegisterClasses' SetAtGrow budget:** spelling parent tables as `&P::classHandlers` instead of `P::GetClassTable()`
  moved UtopiaModule::RegisterClasses *(open)* (53 out-of-line SetAtGrow calls against retail's 51): retail spends
  fewer inline sites per link than the all-accessor form. Lead for InterfaceModule and TGUIModule's RegisterClasses
  too.

## stale: its only example, NiTriShapeData::LoadBinary, now matches; no other case recorded (from walls.md)

- **Instruction selection** `add eax,eax` versus `shl eax,1` (NiTriShapeData::LoadBinary): no source lever known.
  {asm: shl eax, 1}

## one-off fix, done: the stub-ownership rule (statics.md, "A stub's owner is the unit whose `.bss` its stores land in") and `statics.py place-all` cover it

- **Check kept stub annotations against the DATA in their store range:** several UI units were annotated one unit
  late (STRoundedButton/STSaveDialog, the STTargetMenu..STWarpButton chain).

## superseded: x87.md's "A second copy shared across units is a division by a constant" explains this case (Blink divides by 1000.0f, so the if-clamp in CharacterClass::Update no longer moves its referent)

- **A function that loses its match with unchanged source after a neighbour is added** can be a shared float constant:
  a ternary clamp `x < 0.001f ? 0.001f : x` in the new function pooled its constant where an `if` clamp did not, and
  moved the neighbour's referent (CharacterClass::Update against CharacterClass::Blink).

## superseded: the CalculateRoughDirection family matched with the return converted explicitly and `direction =`, not a named result and a new local; inline.md's no-NRVO entry keeps the mechanism (from inline.md)

- **The named-result spelling of the CalculateRoughDirection family:** `TGPoint3 result = rotation * TGPoint3(
  direction ); return result;` gave retail's copy of the operator's temporary into the return slot, and the second sum
  went into a new local rather than `direction =` (no operator= site). PulseWeapon's and PhaserBank's matched with
  `return TGPoint3( ... );` and `direction =` instead, and TractorBeamProjector's now uses the same.

## superseded: walls.md's frame-slot entry now gives the packer as read from C2.DLL (docs/c2-backend.md, "Frame layout"); the size-first sort and the reversed ties were measured from outside and are not what the packer does (from walls.md)

- **How VC6 SP5 orders frame slots** (scratch compiles, /O2 /GX): memory locals go by size (smallest nearest esp),
  then by reference count (most first; a read-modify-write counts 2), and equal counts keep the order in which the
  locals were FIRST REFERENCED in code (arguments are evaluated right to left, so the last argument's local comes
  first). Declaration order alone moves nothing. In frames over 0x80 bytes equal-count ties come out reversed. Read
  retail's slot order as first-reference order and use counts: a swapped pair means one more or one fewer reference,
  or another first use (WeaponsDisplay::UpdateWeaponTargeting: `localTarget` had 5 references in ours, 4 in retail's;
  CanHitTarget's output direction was a local of its own, not `localTarget`, and the function matched).
- **Struct-local stack order isn't declaration order:** reordering twelve declarations
  (STFileDialog::CreateScreenshotArea) or struct locals (ProximityManager::AddObject) never moved VC6's slot layout
  (both *(open)*). The packer places locals in the back end's creation order and, past 0x80 bytes, re-sorts by
  references per byte (docs/c2-backend.md, "Frame layout"): read `c2trace --frame` before reordering anything.

## merged: the c2trace window entries of inline.md (three entries on `--whatif site`/`own`) are one entry now, "`c2trace --whatif` turns an inline wall into a window"

- **`c2trace --whatif site=N,+1` applied several times** turns "which inline went out of line" into a count of sites or
  tuples (TractorBeamProjector::Fire *(open)*, DisruptorGraphic, TacWeaponsCtrl); above 500 own cost the budget is
  twice it.
- **Run c2trace on register-classed walls first:** several are inline-budget walls (one callee's call count differs);
  `--whatif own=+-N` gives an exact window to aim spellings at (PhaserProperty::Unknown686960). Free accessor sites
  still count in the "sites not yet visited" divisor, so removing them changes nested shares: keep the site count when
  trimming cost. A spelling chosen only to land in the window, where the family writes it otherwise, stays MATCH-PENDING
  (PositionOrientationProperty::Unknown689530's `!( i % 2 )`).
- **Bound the spelling search with `c2trace --whatif own=+N` first:** CreateSurfaces needed its own cost in 533..568
  (below, operator='s share misses; above, SetAtGrow inlines), which picked the candidate spellings before any compile.
  The window only narrows the search: a spelling inside it still needs its own evidence.
