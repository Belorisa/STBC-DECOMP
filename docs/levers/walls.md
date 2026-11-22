# Walls that are bounded

Walls to park on: try the named levers once, then stop and record the wall on the function's card. The order to
check things in before calling a wall bounded, and when to stop, is [triage.md](triage.md). Read `README.md` for how
entries are written.

## Walls that are bounded: park, don't grind

### Walls that are bounded: EH frames and unit order

- **Moving a definition into retail's order can move other functions of the unit:** a move changes the declarations
  every later function sees and where the inline copies land. Seen: a CameraMode constructor's and
  MultiplayerWindow::SelectServerHandler's scores dropped, ProximityManager::CollideDamageables turned WRONG-REFERENT
  on `_RESTING_SPEED`, and NiBinaryVoxelData's `_$E420`-`_$E424` stubs moved when VoxelPolygon's constructor (its
  `new NiPoint3[]`) moved earlier; the other way, TorpedoTube::CreateTorpedo rose. Match the unit before and after
  each move, and bisect a move that loses something.

- **A missing EH-state store or EH frame comes from the unit's order:** once a unit has compiled an inline's
  out-of-line copy (right after the first function calling it out of line), VC6 treats later calls to it as
  non-throwing and drops the state stores of regions whose only calls go to it, or the whole frame around a
  smart-pointer local. Look at what precedes the function in retail's unit, or whether it opens a unit of its own
  (MetaVolumePolygonizer::CreateUnknown138 after CreateDebugNode; MapWindow::Unknown500990 after Unknown4FF0F0
  *(open)*; `tools/units.py` reports the unit order). The automatic precompiled header (/YX) is the other half: under
  it the header inlines' out-of-line copies are not compiled ahead of the function, and CreateUnknown138,
  PhaserProperty::Unknown686C80 and Unknown687A60, PositionOrientationProperty::Unknown689830 and
  ViewScreenNode::SetOffTexture were exact (a NiPointer local whose scope calls only header-inline NiPoint3 helpers
  keeps its EH frame and an out-of-line `DecRefCount` as retail). Game units build without a PCH (user's ruling), so
  those are accepted losses, PARKED, except where a unit qualifies for `auto_pch_units` (both its matches and its
  section order improve: DamagePolygonizer, which holds CreateUnknown138). Moving the function inside the current file
  is a fakematch. The same mechanism decides a `new X`'s frame: class-model.md, constructors.

### Walls that are bounded: inline budget

- **Inline budget.** Retail calls NiPoint3's constructor (0x414540), `operator-=`, NiColorA's constructor (0x46C280),
  DecRefCount (0x40CFE0), NiTList::NewItem or a load record's constructor out of line where this build inlines them
  (or the reverse). Try the levers of "The inline budget" (inline.md) once, and `tools/callset.py fn <addr>` to see
  which shared inline disagrees; then park. The camera mode constructors looked like this wall for NiTMap::SetAt and
  were an inline wrapper instead (inline.md, "Leading calls of one helper out of line"): read the order of the
  out-of-line sites before parking. ShipSubsystem damage-point transforms keep NiPoint3's constructor and
  `operator+` out of line (ShipSubsystem::GetWorldLocation, PlayerReticleWindow::Unknown513050,
  ReticleWindow::Unknown515310): a by-value or reference GetDamagePoint, a member or friend `operator*` and a named
  copy all failed. The pool helper 0x40F030, called out of line after 8 inline allocations (PlayerReticleWindow's
  constructor *(open)*), is TGPOOL_ALLOCATION's `operator new` budget, now the two-level form (inline.md, "The pooled
  `operator new` is two-level"): what is left is each caller's own budget. An inline NiTArray<NiPointer> RemoveAll whose null temporary retail keeps in memory and
  destroys out of line (0x401EA0; MetaVolumePolygonizer::Polygonize), and the player list's Get called out of line once
  and expanded in a loop of the same function (MultiplayerGame::ChecksumCompleteHandler) are the same wall. One
  inline site too many in a big constructor or destructor (this build expanding a second NiTList constructor or a
  DecRefCount that retail calls) is not this wall until braces and levels are counted: accessor spellings didn't move
  CharacterClass's destructor, which closed with its single-statement bodies unbraced (inline.md, "A braced body is a
  scope"), and its constructor closed once the pooled `operator new` took its two levels. A
  header that gains an inline (NiMath.h's NiSymmetricRandom, even with an empty body) can move a neighbour's budget
  with its source unchanged (MapWindow::Unknown500F50).
- **Large property-building graphics constructors call every nested inline out of line** (PhaserGlow and
  TractorBeamGlow ctors from the start, TorpedoGraphic's from midway, all *(open)*): read it as a missing inline level
  first (inline.md, "A site retail calls out of line where a top-level share would expand it").
- **`object->IsKindOf( &T::rtti )` stays out of line (0x450530) in functions with many inline NiPointer and property
  sites,** where small functions expand it (UnshareTriShapeData, AddGlowPasses).
- **NiPoint3 operators and constructors called out of line late in large functions while later sites still expand**
  (TractorBeamProjector::TowTarget and DockTarget, ShipSubsystem::UpdateDamagePoint, all *(open)*): one falling budget
  can't give it (inline.md, "A cheaper callee out of line while a dearer one after it expands"), so look for the
  missing level or another visiting order before parking; one lead is that retail's NiPoint3 or TGModelUtils inlines
  cost more per site than ours.

### Walls that are bounded: register, frame and schedule

- **A schedule-only or operand-order function: run `tools/c2order.py <addr>` first.** It names the first divergence
  (a reorder, an x87 operand swap, base and index, a register, a float copy, the frame) and, for a reorder, reruns
  the real scheduler with retail's input order (docs/c2-backend.md, "Scheduling"). "INPUT ORDER" names the tie and
  the source line: move the statement or the evaluation that emits the node. When the node retail wants first is a
  value read for a store or a call whose pointer ours loads first, a local named before the statement gives the
  order (`float height = leftEndCap->GetHeight(); size->y = height;`, STMenu::GetDesiredSize; the window's `width`
  and `height` before each corner's AddChild, ReticleWindow::ReticleWindow), but a single-use carrier local is
  MATCH-PENDING without evidence: look for the construct that emits the value first. When the tied nodes are register
  loads at a block's head (`mov reg, [slot]` with nothing before them), the order is the register allocator's, not
  a statement's. "NOT THE INPUT ORDER" names the chain
  or the height; two rules close such cases. **A store kept ahead of a load** that ours hoists means retail's load
  went through another pointer than the store's (an embedded object's own inline accessor, `&member`, a parameter),
  or the store was an inline setter's (`t->SetNext( 0 )`, whose `this` isn't substituted as a getter's is):
  ShipClass::SetInvincible through ShipSubsystems' warp accessor, TGTimerManager::Update through TGTimer::SetNext
  (both MATCH-PENDING: only the schedule shows the helper). **A
  node ready one cycle early in retail** with an op 0x162 node on our chain means retail read the float without the
  hidden copy (the member itself, not a global object's accessor of a member at a non-zero offset, not a single-use
  named local): TGIconGroup::GetIconScreenWidth's scale moved a cycle that way but still needs one more *(open)*.
  Many "schedule-only" cards are other walls on a closer look: a float copy (c2float), a frame slot (framemap) or a
  register choice come first in the diff.
- **Register allocation** (ESI/EDI swapped, a result kept in a different register) and **frame slot order** of two
  temporaries: run `tools/framemap.py <addr>` first. A reading of "frame and callee-saved registers agree" means the
  difference is in scratch registers, the schedule or an expression, and declaration order is not the lever; other
  readings name the lever (a slot shared or reused, an FPU copy, an EH state, unused frame bytes). Park after the
  accessor, declaration-order and early-return levers. A wall described as register save order or scheduling is often
  a spelling wall: read the register candidates, their priorities and the choice with `tools/c2trace.py --regs`
  before the function is parked. Not a wall: `cmp reg, ebp` re-testing an NiDynamicCast result after its `and`, where
  ours branches on the `and`'s flags, is a NULL test of the argument the source has and the original didn't
  (NiDynamicCast tests it already; MetaSurfacePolygonizer::AddPieces matched without it).
- **How VC6 SP5 orders frame slots** (docs/c2-backend.md, "Frame layout", read from C2.DLL; `c2trace --frame` prints
  it): the packer places locals one at a time in the order the back end created them, each joining an earlier slot
  that doesn't interfere when it can, and re-sorts a frame over 0x80 bytes by references per byte (an unstable sort:
  equal keys can swap). Scratch compiles (/O2 /GX) show that order is the order the locals are first referenced in
  code (arguments are evaluated right to left, so the last argument's local comes first; a read-modify-write counts
  2 references), not the order they are declared in. Read a swapped pair as one more or one fewer reference, or
  another first use (WeaponsDisplay::UpdateWeaponTargeting: `localTarget` had 5 references in ours, 4 in retail's;
  CanHitTarget's output direction was a local of its own, not `localTarget`, and the function matched). Reordering
  twelve declarations (STFileDialog::CreateScreenshotArea) or struct locals (ProximityManager::AddObject) never moved
  the layout (both *(open)*): read `c2trace --frame` before reordering anything.
- **How VC6 SP5 picks registers** (scratch compiles): priority is weighted references; a value live across a call
  takes esi, edi, ebx, ebp in that order of priority; ties go to the value written first in the block. A loop
  reference outweighs about 2-3 straight-line ones and a nested-loop reference about 4-5 (VC5's published 3-4 and 12
  overstate VC6).
- **One x87 component flipping in an inlined NiPoint3 sum through two pointers follows a count of the function's own
  locals declared before the expression, period 8:** file-scope declarations don't move it, so `ctxscan` is blind to
  it; locals after the expression and unused ints don't count. Probe with k unused `const NiPoint3 &` locals above
  the statement in a scratch copy; a window that lands is a lead to a real local the original had, never kept as a
  dummy (PhaserBank::Fire: exact for k mod 8 = 1-6, wherever the extra local sits before the statement; it matched
  with a named `WeaponSystem *system` for its cast, by the user's ruling, the doubt written at the code: the bytes fix
  only the count, and the siblings and the demo build don't pick a local; WeaponHitEvent::GetWorldHitPoint: exact only
  at k = 4 *(open)*). The source path length doesn't move VC6's state (0-40 extra characters flat at two window edges; no /Gi here).
- **Frame-slot and register order that flips with the declarations a unit sees is decided by a count, modulo 64:**
  the number of front-end entities the unit declares before the function. Every declaration counts whatever its name
  or meaning. Measured at a window edge of `_wrap_ObjectGroup_ForceToGroup` (dummy `extern int`s plus one test
  declaration, scratch copies): a new `extern int`, typedef or forward-declared struct 1; a repeated declaration, or
  a forward declaration of a class already declared, 0; a free function declaration 1 plus 1 a parameter, named or
  not; an enum 1 plus 1 an enumerator; a one-line inline function 2; a struct definition 8 even when empty (its
  implicit members), plus 1 a data member, 2 a member function declaration with one parameter, 2 a static data
  member, 3 a one-line inline member; `#define`s, comments and line numbers count nothing, and nothing declared after
  the function counts. bw1-decomp (`docs/msvc6_symbol_numbering.md` there, CC0: the same compiler, the same period
  of 64) measured the member and enumerator costs alike but a forward declaration at 0 and a struct at 1 plus its
  members: their forward declaration named a class already declared, and our empty struct costs 8. They also found the count is taken before one *sensitive symbol*, a global the function reads (inserts after its declaration, even before the function, moved nothing), and bisected insertion points along the unit's include order to find it; `ctxscan` probes two points only (above the definition, `--top`), so a function that flips at `--top` but not above its definition has its sensitive symbol in between. Locals of the
  function count too. The function's bytes take one of exactly two states, and the state repeats with a period of
  64 in that count: `k` dummy `extern int` lines right above the definition give `_wrap_ObjectGroup_ForceToGroup`
  retail's frame slots for k = 0-8 and 60-63 (after the includes: 0-8, 60-72), GraphicsMenu::ChangeDisplayMode
  for every k but 18-25 and 45-46 (after the includes: flipped at 18-25 and again at 82-89),
  WeaponSystem_GetTargetList for k = 1-5 and 65-69, PyEmbed::SaveModuleState for k = 3, 5, 8-20, 22-33, 41-54,
  57-58. So no single declaration is "the trigger": the bisects recorded on these cards (a NiDDImage.h static, two
  PyEmbed.h declarations, TGObject.h dropping one) each found the declaration that carried the count across a window
  edge, and the reported -6 entities of the two PyEmbed.h functions and -1 of loadSizes land exactly on the windows
  above. Retail's unit saw a different count (its headers declared what ours don't, or the reverse); the state
  can't be steered honestly short of headers whose declarations are retail's, entity for entity. Dummy declarations
  that land a window are a fakematch. The mechanism is narrow: of the open register walls at 95 or more when it was
  measured, only SaveModuleState had a state that a count reaches (MultiplayerWindow::BuildMissionMenu flips between
  two wrong states); the NiTListBlock operand order below and the inline-budget flips (Editor::HighlightNode,
  MetaNebula::CreateExterior) don't move with the count. A function whose bytes change when only another part of
  the unit changes: probe it with dummy externs above its definition (`tools/ctxscan.py <addr>`, scratch copies
  only) before bisecting a header diff. Park it with a card; a lost match is accepted only under the
  declaration-sensitive rulings (AGENTS.md, Policies), and restored only by a real declaration or include with its
  evidence, never by tuning headers. Build the whole tree before keeping an include added to a widely included header.
- **The declaration count is not what holds the register walls:** `ctxscan` over every open register-wall function
  (392, k = 0..63, above the definition and at the top of the file) found more than one byte state in only 7, and an
  exact state in only three, all already listed in `config/declaration_sensitive.txt` (PyEmbed::SaveModuleState,
  WeaponSystem_GetTargetList, NiMultiTextureProperty::CreateClone: *(open)*, in their other byte state). Don't scan a register wall for a count again
  unless a header change is seen to flip it; start from `tools/c2order.py` and `c2trace --regs` instead.
- **A unit's registrations and unrelated includes don't move its functions:** making DamageVoxelGrid's,
  TorpedoGraphic's and TGModelManager's `_$E` registrations equal retail's run in count and order (NiPropertyState.h
  forward-declaring its fifteen property classes as Gamebryo's NIPROPERTYSTATE.H does, unit includes reordered) left
  every open function there byte-identical, register, control-flow and inline walls alike (TorpedoGraphic's
  constructor still inlines NiProperty::SetField and SetBit where retail calls them). Don't chase a frame, register
  or inline wall through include sets unless the included declarations change something the function uses (an
  inline body, a member, a signature); the only context effect seen is the declaration count above.
- **NiTListBlock's inlined constructor operand order (`items[...]` base/index) is compiler state, not source:** retail
  shows both orders for the same template, even within one loop (LensFlare, CameraMode::ReadFromStream,
  OptimizedFireScript's constructor), and it flips per caller. The lineage pointer walk (Gamebryo 1.1.2 CreateFreeMem's
  `item = &items[i]; item->next = item + 1`, which breaks 11 exact functions) and four index spellings were A/B'd over
  six units; the current spelling matches the most sites. Park the near misses as compiler state
  (CharacterClass::ReadFromStream 0x66E220, CameraMode::ReadFromStream 0x41F350, LensFlare::ReadFromStream 0x4E2E30,
  OptimizedFireScript::OptimizedFireScript 0x481CC0, OptimizedSelectTarget::OptimizedSelectTarget 0x48A340).
  The base is the operand the commutative sort puts first (docs/c2-backend.md, "Base and index of a two-register
  address"): the `size * 12` temporary against the load through the block pointer's temporary, compared by hash
  (`(n << 6) & 0xFFFF` against `7 + ((m & 3) << 14)`), so the order follows the two temporaries' descriptor numbers,
  which the back end hands out with reuse of freed ones. `c2trace --addr` prints the comparison and the shifts that
  flip each site: LensFlare::ReadFromStream and NiRender::NiRender flip alone with one or two descriptors more or
  fewer before the block; CameraMode needs a shift between two of its own sites; CharacterClass's site never flips
  without three matching ones; the constructors' loop stores (OptimizedSelectTarget, OptimizedFireScript) need the
  induction temporary numbered past 1024, by amounts no single shift gives. No one NiTList.h change fits them all;
  each is the function's numbering. NiBezierTriangle/NiBezierRectangle::VirtualSlot45, which needed one descriptor
  less and one more respectively before their store loops, closed once the NetImmerse headers took the SDK debug
  copy's text: a NetImmerse function's numbering moves with proven header text, never with probes.
- **An inlined NiTMap::GetAt on a static table pointer** loads the table into a scratch register and then copies it
  when the static is itself a register candidate (several reads in the caller: TGEvent's `objectEventTable`), and
  straight into the saved register when it is not (TGObject::GetTGObjectPtr). Not a header lever: TGEvent's
  Add/RemoveObjectReference(s) (0x6D5850, 0x6D5A80, 0x6D6320) are decided by their own reads of the static.
- **`SetName`-style string setters** ( `if ( n != name ) { delete name; name = NULL; name = new ...; strcpy }`): a
  named local for the new buffer restores retail's ESI/EBX allocation but moves the member store after the copy;
  storing first brings the swap back (SetFloatVarEvent::SetName, ExplodeParticleController::SetSceneRoot *(open)*).
  Park.

### Walls that are bounded: x87

- **x87 operand order and scheduling.** `fadd`/ `fsub`/ `fmul` operand order, where a spill lands, whether a value
  stays on the FPU stack. Try the levers of "x87 operand order" (x87.md) and a named local once; then park. A stray FPU
  value ( `fld a; fld b; fadd st(1) ... fstp st(0)` against retail's `fld a; fadd b`) is the same open residue
  (STTopLevelMenu::VirtualSlot114, STToggle::GetDesiredSize). An x-only flip in a commutative `a + b` (retail
  `fld right; fadd left`) survives every operand-order spelling tried (PlacementWatchMode::Update,
  PlaceByDirectionMode::Unknown427E50's `position += fraction * movement`); ViewScreenNode::ProjectPoint's closed
  instead with its viewport accessor read at each use, so try accessors on the operands first; only the x term's order differs in
  `( boxMin + boxMax ) * 0.5f` and `R * ( c * s ) + T` (MetaSurfacePolygonizer::AddVolume). The Eberly box tests (OBBNode
  0x4662C0, 0x466510) closed once written as Eberly's library writes them: each `|cross . axis|` stored as a float
  into an array element (`absCrossDot[i] = (float)fabs( ... )`) and the right-hand side into one `rhs` local, then
  `if ( absCrossDot[i] > rhs )`; the face tests' dots as `direction.Dot( axis[i] )` (this the line, not the axis).
  A named rhs alone, nested ifs, `&&`, reversed compares and swapped products had failed.
- **Processor Pack reciprocal:** retail computes `1/scale` as `fld 1.0; fdiv st(1)` and this build folds it into
  `fdivr [1.0]` (NiListener::Update, NiSource::Update *(open)*); operator/, `*=`, a named scale and four operator/=
  forms failed.
- **FPU copies without a known spelling:** the subsystem group, where retail copies an unnamed one-level value
  (conditionPercentage, condition, powerPercentage, a property's radius) through fld/fstp before a call
  (ShipSubsystem::IsDisabled, PhaserBank, PulseWeapon, ImpulseEngineSubsystem, PowerSubsystem::AddPower, the weapon
  properties' piece builders), and a long-lived local copied through the FPU (STFillGauge::RoundToPixels,
  STFileMenu::Refresh). A value that comes through two inline levels (or a named copy) is copied with fld/fstp:
  IsDisabled, AddPower and RoundToPixels are exact through an invented second level (a named local inside the
  accessor), marked `(invented)` and MATCH-PENDING by the user's ruling until evidence names the hidden level; the
  rest are open. fld/fstp argument pushes into a call whose value is returned suggest the original had no `return`
  there (the CreateFromTheme functions; an inference).

### Walls that are bounded: control flow

- **Loop rotation** (retail tests at the top where this build tests at the bottom) and **tail merge** of shared return
  blocks: try `while ( true )` against `for ( ;; )`, `if/else`, a flag, a `goto`-free early return; then park. Also
  parked: a value computed before a NULL test that VC6 sinks below it (GraphicsMenu::UpdateToggles), and an early
  `return NULL` joining a shared epilogue with every register saved up front (GraphicsMenu::GetResolutionMenu *(open)*).
- **NiTArray RemoveAt's out-of-line copy** keeps `setne; test` where this build folds it into the branch
  (NiKeyframeManager::Sequence destructor) *(open)*. {asm: setne; test}

### Walls that are bounded: dead code

- **Dead stores retail keeps** (SetClass::PostFixUp's loop counts, `_realloc`'s frame slot): an empty stand-in is
  MATCH-PENDING, never counted. The real fix is the macro or local the original had (SetClass::PostFixUp and
  DumpCollisions closed with a release-empty trace macro). Values the original built and never used (a TGString
  local, a discarded GetCurrentResolution) are real code: keep them with `// BUG:`.
