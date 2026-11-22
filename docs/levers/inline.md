# The inline and call set

Which calls VC6 expands and which it leaves out of line. A wall reported as `inline` starts here; the order to check
things in is [triage.md](triage.md). Read `README.md` for how entries are written.

## Spelling that changes the inline and call set

### Spelling that changes the inline and call set: accessors

- **Accessor instead of member read.** Spelling a member read through its inline accessor ( `GetTargetID()`,
  `GetParentSubsystem()`, `GetNumReady()`, `GetTime()`, `GetValue()`) changes register choice, argument width and
  whether a copy goes through the FPU. Closed TorpedoTube::Update, ShipClass::Update, NiKeyframeController::
  ResetTimeExtrema, NiTCBFloatKey::Interpolate, NiLinFloatKey::Interpolate. Try it per comparison on 16-bit flags. A
  base member read as `GetName()` gave retail's reload register (ObjectClass::GetAffiliationColor). A float member
  compared with `fld; fld; fcompp` instead of `fcomp [mem]` is read through its accessor (TGAnimNode::FindBlender;
  `NiPick::Record::GetDistance()` in OBBModel::SortResults; `GetTime()` on key times in TGAnimationEntry's three key
  blenders), and the converse, `fcom [mem]`, is a direct member read (MetaVolume's radiusSquared in
  MetaSurfacePolygonizer). An int converted through an inline accessor (`(float)GetLineLength()`) is copied through a
  register and a stack temporary before `fild`; a member read directly converts with `fild [mem]`
  (GridClass::Unknown498FD0). `member` against `Accessor()` also changes where a call argument is evaluated: reaching
  the set through `GetContainingSet()` at both uses fixed the argument order (Waypoint::UpdateCurve). Two `and`s on one
  flags byte are two accessor round-trips: VC6 folds `x &= ~A; x &= ~B;` into one `and`; retail's two came from
  `SetCollisionFlags( GetCollisionFlags() & ~... )` twice (ProximityCheck constructors). Twin functions differ: the
  Phaser property builders use accessors, the updaters members. Not a blanket fix: a sweep putting accessors in place
  of member reads over 86 open functions matched none. {asm: fld; fld; fcompp}
- **`shr eax, N; test al, 1` on a flags word is the class's bit accessor** (`IsOpenable()`, `IsOpened()`), not
  `flags & MASK`: the raw test scored lower (SortedRegionMenu::Unknown517A00). {asm: shr eax; test al, 1}
- **Open-coded hit tests are TGUIObject's `IsWithin`** (TGButtonBase::MouseHandler; STTopLevelMenu::MouseHandler kept
  it without a change in score). Didn't help in: STMenu::MouseHandler (retail compares `width < point.x`, the
  open-coded form), TGDialogWindow::MouseHandler.
- **A table-sqrt length is `NiPoint3::VectorLength`** (OptimizedSelectTarget::GetTargetRating).
- **An out-of-line `UtopiaApp::GetApp` plus `TGApplication::GetGameTime` pair** is `UtopiaApp::GetApp()->GetGameTime()`,
  not `g_kUtopiaModule.GetGameTime()`.
- **A handler that passes the event on through its destination** is `event->GetDestination()->CallNextHandler( event )`:
  retail's tell is `mov ecx, [esi+0xC]; push esi; call 0x6D90E0` with `this` unused (SortedRegionMenu's handlers).
  {asm: push esi; call 0x6d90e0}
- **Qualified calls** ( `pane->TGPane::GetNthChild()`) give the direct, non-virtual call retail uses
  (STMissionLog's closing `TGPane::SetNotVisible`).

### Spelling that changes the inline and call set: vector expressions

- **The shape of a NiPoint3 expression decides which operators stay out of line.** Componentwise spellings and
  in-place updates keep the code inline: `NiPoint3( r * f.x + p.x, ... )` by components expands the constructor where
  `r * f + p` calls it out of line (0x414540); `v *= k; v += p;` for a ray end or a push-out stays expanded where
  `v * k + p` calls the constructor (ShipSubsystem::UpdateDamagePoint); `v = outward * extend; v += cut;` expands where
  `cut + outward * extend` doesn't (PhaserBeam and TractorBeamShape slot 35 *(open)*); `position *= s; position += p;`
  keeps retail's order where `p + position * s` did not (ChaseCameraMode::GetIdealPosition); quad vertex arrays are
  `vertices[i] = NiPoint3( x, y, z )`, not per-member stores (BlinkingLightUnit::Unknown4DABA0 *(open)*). The reverse
  also occurs: in NetImmerse-style code the whole expression written at once (`*direction = a / d * k + -v * f`)
  gave retail's call set where a named temporary moved the scalar multiply's constructor out of line
  (Planet::GetEnvironmentalDamage *(open)*). Pick the spelling by the shape retail shows.
- **Member against friend `operator*`.** An accessor written `member * scalar` brings retail's out-of-line
  `NiPoint3::operator*( float )` into every caller that expands it; `scalar * member` (the friend) doesn't
  (PhysicsObjectClass::GetAngularVelocity, in ShipClass::TurnTowardDifference). `forward * scalar` (the member) copies
  the scalar before the call, the friend `scalar * forward` loads it after (a warp-speed velocity,
  WarpEngineSubsystem::Update). Didn't help in: ExplosionPlumeController::VirtualSlot39 (the friend form scored
  lower).
- **`SetTranslate( <NiPoint3 temporary> )` binds the TGPoint3& overload (slot 31) and adds an EH frame;** retail's
  slot 30 takes a named NiPoint3 by pointer (TractorBeamProjector::DockTarget *(open)*).
- **Assign a returned TGPoint3 into the zero-initialised result and reuse it** (`point = e->GetPosition(); point = R *
  ( s * point ) + T;`): a TGPoint3 temporary inside the expression adds an EH frame
  (TractorBeamSystem::GetDockingPoint).
- **An out-of-line NiPoint3 constructor blocks CSE:** retail evaluates a cross product's length twice when the
  constructor is a call (CameraObjectClass::StartTurn *(open)*).
- **Matrix columns set with `SetCol` keep the matrix inline;** the `NiMatrix3( col, col, col )` constructor is called
  out of line (TGModelContainer::ExtractModel's camera). SetCol by components (`rotation.SetCol( 0, d.x, d.y, d.z )`)
  costs less than by point and gives retail's call set where the point form spends too much budget
  (TriShapeOrientedParticles::UpdateQuads *(open: register)*). Didn't help in: NiCamera::LookAtWorldPoint (SetCol is
  then called out of line).
- **`fabs` in C++.** `fabs( x ) < K` compares a double and pulls an extended-precision constant; retail's dword
  compare is `(float)fabs( x ) < K` with `K` a file-static `const float` (MetaVolumePolygonizer, 0x88B9C8) or `fabsf( x )`
  (VC6's math.h has no float overload of `fabs`; `fmodf` likewise). `fabsf` is itself an inline wrapper in VC6 C++
  (`(float)fabs((double)x)`), one level more: when retail stores an integer before the `fabs`, write `(float)fabs( x )`
  (BinaryVoxelizer::BuildTree, FindOppositePlane). Didn't help in: BinaryVoxelizer::VoxelBspNode::Split, MetaNebula::CreateExterior (the
  pointer load moves first as in retail, but `fabs` is emitted), nor in eight other game units where the swap was
  tried.
- **A TGPoint3 copied into a NiPoint3 frame temporary before a `const NiPoint3 &` call** is an inline overload taking
  `const TGPoint3 &` that passes `(NiPoint3)point` on: TGPoint3 arguments pick it, NiPoint3 rvalues
  ( `-GetModelForward()`) bind the out-of-line method directly. A float argument copied to a local first is the
  overload's parameter. It closed NetworkAI::Update, the SWIG ShipClass SetImpulse/SetSpeed/
  SetTargetAngularVelocityDirect/TurnTowardLocation/TurnTowardDirection wrappers,
  TractorBeamProperty/PulseWeaponProperty::SetOrientation, AlignToVectors and _wrap_ProximityManager_GetNearObjects. A
  C++ caller that casts `(TGPoint3 *)&point` shows the real method takes `NiPoint3 *`: the TG-typed form is an inline
  overload for the scripts, copying (`NiPoint3 point = *p; F( &point );`: ShipClass::SetTargetAngularVelocityFraction,
  TurnTowardDifference) or not (SetClass::IsLocationEmptyTG over IsLocationEmpty; CameraObjectClass::LookToward, whose
  float argument goes through the FPU as an inline's parameter does). Check every in-tree caller that passes a TGPoint3
  before adding one: a caller binding a `*location` TGPoint3 to the NiPoint3 method directly needs the pointer method
  under its own name (SetClass::IsLocationEmptyTG against ProximityManager::GetNearObjects's overload). Not a by-value
  inline: see the by-value entry below.

### Spelling that changes the inline and call set: helpers

- **A repeated expression or test is an inline helper the original called.** Retail's bare `fistp` sequence is an
  inline float-to-int helper; the saved-ID lookups are `TGObject::GetSavedID`/ `GetSavedObject`,
  `TGEvent::GetSavedEvent`; group walks go through `TGSound::IsInGroup`; the pixel rounding is `RoundToPixelX/Y`. Two
  script float calls sharing one frame slot are one `static inline` helper with an early `return defaultValue`
  (GetTacticalFloat in ReticleWindow and MultiplayerWindow). TopWindow::ResolutionChangeHandler's two copies of the
  resolution menu's mode test, as a file `static inline` helper, left the renderer temporary's DecRefCount out of line
  as retail does (the same test in GraphicsMenu::GetResolutionMenu doesn't move with it). A repeated out-parameter call
  with its own locals is one too: retail's frame fits only with the five stats locals inside the helper
  (SetClass::VirtualSlot25 *(open)*, 20 bytes over with function-scope locals). A helper wrapping a repeated pair of counter
  updates is an inline site that moves the budget ( `AddBytesSent`, TGNetwork::SendMessages *(open)*).
- **/Ob2 marks only bodies under 175 IL tuples as automatic inline candidates.** A `.cpp` function (not declared
  `inline`) that retail calls out of line at every site with budget to spare has source of 175 tuples or more;
  `callset.py fn <addr>` finds them (retail calls, ours expands) and `c2trace --cost` gives the callee's cost.
  It is a diagnostic, not a spelling: padding a body to cross the limit (helpers written out by hand where the class
  calls them, `else { return 0; }` arms added only for tuples) is a fakematch. The SDK debug copy
  (`tools/sdkdebug.py`) is where the longer spelling comes from: NiD3DTexturePipeline's GetBlendPasses through
  NiD3DRenderState's blend-mode getters, and Unknown7CE580's and Unknown7CE780's stage operations, as AnimOnly writes
  them, cost over 175 and stay calls as retail's (all three, their caller Unknown7CE040 and
  NiD3DTexturePipeline::Unknown7CE9C0, whose frame closed with the rest of the debug copy's statements: MATCH).
- **A Gamebryo inline written out by hand is a budget wall:** a four-arm `fabs(x) < fabs(y)` UnitCross ladder was
  `direction.Perpendicular()`, and per-entry matrix stores `m[r][0..2] = p.x/y/z` were `SetRow( r, p )`; restoring
  the calls changed the call set and the nested shares (EmitterController::SetEmitPositionAndDirection matched).
  A function whose code equals two existing functions inlined calls them: NiAVObject::CullShow is its two culling
  halves, Unknown7DCA70 and Unknown7DCB00, defined above it and expanded by /Ob2 (MATCH once the halves read their
  locals as the SDK debug copies do).
- **A SWIG wrapper that loads the object into a callee-saved register before evaluating an argument call** wraps an
  inline member taking the object as `this`, not the expression written out in the wrapper
  (_wrap_ScriptObject_LoadDatabaseSound: exact with an inline `LoadDatabaseSound`, which needs TGLocalizationDatabase
  complete in ScriptObject.h; _wrap_Game_LoadDatabaseSoundInGroup the same). A store in a wrapper's inline copy that
  the header inline lacks belongs to the inline's body (SetManager::DeleteAllSets also clears `renderedSet`).
- **An `a || b` test that only matches as `bool t = a; if ( !t ) t = b; if ( t )`** is an inline `bool` helper
  returning the `||`: VC6 materialises the helper's result, which keeps retail's separate returns, and the x87 order
  of the rest of the function follows. SWIG's `Class_IsX` names list such helpers ( `TGRect::IsEmpty`, whose wrapper
  inlines `width == 0 || height == 0`, closed TGRect::Unknown739F00). A single-flag helper (
  `int IsEnabled() { return flags & TGUIO_ENABLED; }`) compiles exactly like the open-coded test: write it for the
  name, not for the bytes.
- **Written out flat, not through a helper.** Some repeated blocks only fit retail open-coded: a flood fill written as eight
  near-identical inline blocks can't come from a helper per row (a header helper stops expanding after about three
  rows and `__forceinline` isn't honoured; MetaVolumePolygonizer::Unknown4C0830 *(open)*); loop or clip blocks retail expands fully
  were written out flat, where inline helpers push their nested NiPoint3 operators out of line (BinaryVoxelizer::Step *(open)*);
  destructors that repeat a free loop instead of calling it (calling the helper left an EH frame and an out-of-line
  destructor; DamageMesh), with explicit stores and unrolled `delete a[i]; a[i] = NULL;` where Reset() or a loop
  didn't match. Check such a spelling against the reader test before it counts.
- **An invented helper over 40 tuples is charged at every site.** A one-line helper costing 41 or more is no longer
  free: each of its sites spends its cost from the top-level budget, and its own nested call gets only a share.
  VoxelPolygon's invented `Add( v ) { vertices[count++] = v; }` (42) left the last of Split's twelve `operator=`
  8 short and pushed ClipBack's point operators out of line; the store written out at each site (no helper) gave
  retail's call set (VoxelBSPNode::Split and ClipBack, *(open)* on other walls). When `c2trace` shows a repeated helper of cost 41-60
  whose nested call retail expands, try the body written out before any budget arithmetic.
- **A braced body is a scope: two IL tuples of own cost, no code.** `{ }` around a single statement adds a
  scope/end-scope pair to the front end's count, so it moves the budget (2 x own) and, in a header inline, the
  callee's cost, without changing the code. When `c2trace --whatif own=-N` (or a callee `N` over its share) is the
  whole gap, count the single-statement bodies: BridgeObjectClass::Update needed 16 fewer own tuples, its eight
  braced single-statement bodies (MATCH); SwitchToMovieMode's nine gave its DecRefCount out of line *(open)*;
  NetFileNameList's destructor loop without braces costs 94, not 96, the share ReceiveScriptChecksums gives it
  (MATCH); TGTraceOptions::ReadOptions took braceless arms (lineage.md has 1.2.2's braceless ifs). A braced if/else
  costs 4 more than the braceless form. One pair alone changes nothing visible: test the whole set at once.
  Sweep: count the function's single-statement bodies (n), run `--whatif own=-2n`, and keep the edit only when the
  verdicts that flip are the ones `callset.py fn` says retail wants. Unbracing every one closed
  CharacterClass::~CharacterClass (nine, the overlay release's DecRefCount) and NiTriIntersect::Find (Gamebryo 1.2.2
  writes `if ( !FindN() ) return false;` there), and gave retail's call set to BridgeObjectClass::CollectTextures and
  DoCrewReactions, VoxelBspNode::Split, NiBezierRectangle::VirtualSlot48, NiParticleSystemController::Unknown7B0330
  and FlareController::CreateTarget *(open, other walls)*. Unbrace all of a function's single-statement bodies, not a
  chosen few: which pairs go is not evidence, only their count is. A function-local static's decorated name numbers
  its scope (`?BH@`), so the braces before it are fixed by the DATA annotation (MetaSurfacePolygonizer::Unknown4B5ED0
  keeps them).
- **A member reached through a nested struct costs two IL tuples more per use than a member array:**
  `*triangle1.vertex[0]` (a `Triangle` struct member) is `this constant member constant member`, `*q[0]` (a
  `NiPoint3 *q[3]` member) `this constant member`, with the same code. When the own-cost gap is twice the number of
  uses of one nested member, check the class layout against the lineage and the decorated names: NiTriIntersect's
  `Triangle` members as Gamebryo 1.1.2's `m_apkP[3]`/`m_apkQ[3]` (DamageTool's constructor takes `NiPoint3 **`,
  copied with 1.2.2's `memcpy`) closed PEdgeQFaceIntersect, PFaceQEdgeIntersect and PFaceQFaceIntersect (14
  tuples less own cost against a need of 12-16) and, with 1.2.2's trailing `else { assert( 0 ); }`, LocateIntersection (MATCH);
  the constructor and the callers in OBBNode and NiTriBasedGeom stayed exact.
- **A header constructor's body decides its callers' budgets: one constructor with a default argument, not two.**
  `DamageBitArray() { count = 0; bits = NULL; }` costs 34 (free); the count constructor called with a default `0`
  costs 88 and folds to the same two stores. MetaVolumePolygonizer's constructor, whose two arrays sit before the grid
  list, needed exactly that spend to call the list constructor out of line as retail (MATCH). Where retail
  spent 150-200 units more before a site, look at the header constructors of the members built before it.
- **A NULL test only on a find loop's found path**, the exhausted loop jumping straight to the "make one" code, is an
  inline find helper returning the element or NULL ( `OBBTriangleList::Find`); a find-or-make helper with `new` inside
  is not expanded.
- **A cheap inline called out of line where the budget left is far above its cost sits inside a helper.** A site
  called straight from the function fails only when its cost exceeds the budget left; inside a helper it gets the
  helper's share (`budget left / sites left` at the helper's site); one callee out of line early and expanded later is
  the wrapper case below. Read the helper's boundary from the share retail needs (what must fit before the first
  failure, what must fail after it): MetaSurfacePolygonizer::ProcessDamageTri's edge cut needs a share of 273-317 and its corner
  SetAt fails at 157 against a cost of 192 (the invented CutEdge and EdgeMap::AddEdge gave ProcessTri and
  ProcessDamageTri retail's call set, *(open)* on frame and registers). The boundary comes from arithmetic, not
  evidence: such a helper is marked `(invented)`, and a match it closes is MATCH-PENDING.
- **Out-of-line operator calls at the top of a small function can come from an inline helper, not the budget:**
  DamageVoxelGrid::GetBox (`origin + NiPoint3( ... )`) moved MetaSurfacePolygonizer::Unknown4BA030 *(open)*.
- **A helper returning NiPoint3 by value stops expanding where the same helper writing through an out-parameter
  expands** (MetaVolumePolygonizer::Polygonize *(open)*).
- **A cheaper callee out of line while a dearer one after it expands can't come from one falling budget:** once a
  site fails, every later dearer site at the same level fails too, so retail visits the sites in another order, or
  the cheaper one sits at another level (next entry). Count calls per callee, not their layout, before believing it: a
  run of identical blocks ends in one shared call (cross-jumping), so a `Get` with no `Add` after it in the layout is a
  tail jump, not an expanded `Add`, and the object's call count can be one below the out-of-line site count.
  NiBezierTriangle::VirtualSlot47's tail reads `Get Add x5, Get, jmp, Get Add x3` on both sides: retail only has its
  site [65] Get out of line as well, which one falling budget gives at own cost -15..-50 (`--whatif own=-43` gives
  exactly retail's call set) *(open)*. It and NiBezierRectangle::VirtualSlot47 (own -191..-227 for 198 uses) *(open)*
  fit about one tuple less per vertex-cache use than the owner's member form; NiBezierCylinder::VirtualSlot49 does not
  (own -45 already flips an Add) *(open)*; AnimOnly's plain static pointer is two less and overshoots
  (`hdr-bezier-cache`). IL probes (`c2trace --cost` on a scratch file): a static member, `this->` on it, and a const,
  volatile or array pointer cost what the plain global does, a base-class call through a derived pointer what the
  member form does; only a global reference to the pointer sits between (+1, no evidence for it).
- **A site retail calls out of line where a top-level share would expand it sits one inline level lower,** in a
  wrapper where it fails on the nested share. A cost-47 `NiPoint3( x, y, z )` called at depth 1 while later sites of
  cost 57 or more expand is a lineage float overload (`NiBound::SetCenter( x, y, z )`: TGUIBatch::TGUIBatch MATCH;
  `NiAVObject::SetTranslate( x, y, z )`) or another by-value wrapper. A unit where retail seems to expand less
  everywhere is the same per-function budget, not flags or a depth pragma: in PhaserGraphic no /Ob, /O or
  `inline_depth`/`auto_inline` setting reproduces retail's call set, and `c2trace` shows every site retail calls there
  costs over 40 and fails on its nested share (retail's shares ran 40-49 where ours ran 59); matched functions expand
  the same callees at depth 3-4. When ours expands nested sites retail calls, look for sites retail has and ours
  lacks: point and colour temporaries spelled as constructors, locals, wrapper overloads; each extra site lowers every
  earlier share. The PhaserGlow::PhaserGlow and TractorBeamGlow::TractorBeamGlow outer corners are this case with the
  wrapper unidentified *(open)*: `TGPoint3( x, y, z ) : NiPoint3( x, y, z )` fits by the shares but loses
  TGPoint3::UnitCross 0x581E60 in its cross's construction (y kept on the x87 stack), and UnitCross respelt over it
  (Cross, member stores, SetXYZ) stays short. NiPoint3::operator= out of line at a top-level site with a budget near
  1000 is the same case ("NiPoint3::operator= called out of line" below).
- **Leading calls of one helper out of line and the trailing ones expanded is a free inline wrapper around it** (the
  budget only falls, so two top-level sites of one callee can't do it): the wrapper (cost 40 or less) is a top-level
  site, and its nested call gets `budget left / sites left`, which grows as the sites run out, so only the last few
  fit. The camera mode constructors set their attributes through
  `CameraMode::SetAttrFloat` and `SetAttrPoint`, virtual setters defined in the class (each with its out-of-line
  copy, 0x41E9F0 and 0x41EA80, emitted next to each other as COMDATs) and called directly in a constructor: five
  constructors matched at once and ChaseCameraMode's came out exact but MATCH-PENDING (its extra NiPoint3 site is a
  budget-only spelling, `config/exceptions.md`), where spelling `floatAttrs.SetAt( ... )` expanded the first sites instead. The
  same holds for `GetAttrFloat` in CameraMode::GetLaggedPosition. A wrapper's pointer argument passed as
  `&TGPoint3( ... )` (VC6 takes a temporary's address) expands the TGPoint3 conversion that a by-value
  `pointAttrs.SetAt( attr, TGPoint3( ... ) )` keeps out of line (LockedPositionMode, PlaceByDirectionMode).
- **An inline returning a UDT with a destructor is never expanded under /GX, but its cost is charged.** A TGPoint3
  returned by value goes through `___$ReturnUdt`, and c2trace prints "expansion undone (late check)" after the cost
  has been charged to the budget like an expanded site's. Where retail copies the point inline into a 12-byte
  temporary, the original passed the NiPoint3 member to a `const TGPoint3 &` parameter, and the conversion built the
  temporary: `warpEvent->SetExitPoint( exitPoint )` with `SetExitPoint( const TGPoint3 & )` (WarpSequence::Play:
  MATCH), where `SetExitPoint( GetExitPoint() )` is late-rejected. The charge is a budget lever of its own: a header
  inline such as GetPositionTG spends it at each call (ShipSubsystem::GetWorldLocation, below), and a late-rejected
  UnitCross charges 224 in TractorBeamProjector::CalculateRoughDirection *(open)*.
- **A class's own accessor read at each use adds the sites a share needs:** `device->GetName()` (Ni3DDeviceDesc's,
  AnimOnly 0x513C20) at both uses in SetClass::CanGlowBeEnabled put the first renderer release's DecRefCount out of
  line (914/15 < 68) and the name load in retail's place (MATCH with one shared `return false` after positive
  nesting).
- **A base call is written as the base call once the base member is inline:** `CameraMode::SetAttrPoint( attr,
  point )` matches ChaseCameraMode::SetAttrPoint, CameraMode's being defined in the class.
- **Default-constructed locals declared before a run of charged sites lower the run's divisor:** NiQuaternion
  `from` and `to` declared before IdealControlledCameraMode::Unknown421910's two UnitCross calls (their free
  constructors become earlier sites) give the first UnitCross a share of 118 and retail's expanded Length *(open)*.
- **A class's own virtual called through the vtable inside its constructor** ( `call [eax+0x100]` on `this`) means an
  inline helper made the call; VC6 calls it directly otherwise (NamedReticleWindow's `PrependChild`, MapWindow's
  `AddChild`).
- **`NiTList::Remove( element )` rather than FindPos then RemovePos** gives retail's inline FindPos with an
  out-of-line RemovePos (ShipClass::Unknown5AFD70); it is also what makes the out-of-line RemovePos copy appear
  (TGEvent::RemoveObjectReference, NiTList<TGEvent*>::RemovePos, NiTList<int>::RemovePos for
  ProximityAxis::Unknown5A8CC0 *(open)*).

- **Game code open-codes the TGModelUtils transforms:** `node->GetWorldRotate() * ( p * node->GetWorldScale() ) +
  node->GetWorldTranslate()` and its inverse/vector forms are `TGModelUtils::LocalToWorldPoint`/`WorldToLocalPoint`/
  `LocalToWorldVector`/`WorldToLocalVector`. Signature in retail: the out-of-line triple `operator*( float )`
  0x41CBD0, `NiMatrix3 * NiPoint3` 0x813A40, `operator+` 0x426C30 (inverse: `operator-` 0x4695F0, then 0x41CBD0). The
  helper adds an inline level, so budget walls move (PhaserBank::Fire, PulseWeapon::Fire, LockedPositionMode::Update
  and CollisionEvent::WriteToNetStream matched; ReticleWindow::Unknown515310, AsteroidField::IsShipNearSlot,
  TractorBeamProjector::Fire and DockTarget *(open)*). Pass the node as `object->GetNode()` at each call: a cached local missed
  DamageCollisionTest::Unknown5884A0's match.
- **A transform whose sum constructor sits one level deeper than LocalToWorldPoint's is a budget, not always a helper.**
  `point * scale` (constructor expanded), `rotate * that` (0x813A40), `translate + rotated` (constructor called) needs
  LocalToWorldPoint's share in [201, 223). ShipSubsystem::GetWorldLocation gets it from GetPositionTG as a header
  inline, whose late-rejected UDT return is still charged (above), here 56 at each of two calls (871 / 4 = 217):
  MATCH.
  ShipClass::Unknown5B3740 (both constructors called, the damage point read in place) needs [129, 176): three or four
  more sites after the transform, which no evidenced accessor gives yet; the Nebula/MetaNebula texture loops' {scale,
  NiMatrix3, NiPoint3} transform stays an identity-TODO *(open)*. The glow constructors' outer corners are a level
  case: see "A site retail calls out of line where a top-level share would expand it" above.
- **A by-value result reaches the return slot the way the source spells it: VC6 has no NRVO.** A named local
  returned is copied into the slot (a NiPoint3 local through the FPU, fld/fstp; a TGPoint3 by its copy constructor,
  0x581F20, a site that moves every earlier share); `operator=` into a local followed by a copy into the slot is
  `first = second * first; return first;`, with no third local (GetTurnBetweenFrames: MATCH). Retail building an
  out-of-line operator's result in a frame temporary and copying it into the slot with integer moves (`mov edx,
  [eax]; mov [ecx], edx` ... `mov eax, ecx`) is the result converted explicitly, `return NiPoint3(
  node->GetWorldRotate() * MODEL_FORWARD );`; the bare `return expr;` constructs in place, and a `?:` shares the copy
  with the other arm (BaseObjectClass::GetWorldForward, GetWorldUp, GetWorldRight: MATCH). The same holds for a
  TGPoint3 result, `return TGPoint3( GetParentShip()->GetWorldRotation() * TGPoint3( direction ) );`, with
  `sinf`/`cosf` (each a free inline site, which `(float)sin` is not), each sum written as one expression and the
  class's `GetParentShip()` adding the last site a share needed (PulseWeapon::CalculateRoughDirection and
  PhaserBank::CalculateRoughDirection: MATCH; TractorBeamProjector::CalculateRoughDirection takes the same return, its
  cross product in a named `NiPoint3 right` whose temporaries' EH state drops to -1 before the multiply, as retail's
  *(open: budget)*). A named result is still right where retail's copy keeps part of the point on the FPU
  (TractorBeamProjector::GetBeamOrigin *(open)*).
- **The helper often closes the gap outright where the open-coded form caches the node** (GetDockingPoint,
  DamageableObject::AddDamageVolume, ShipClass::ApplyWeaponHit and Unknown5AFAD0 matched). `( 1/s2 ) * ( ( R1 * ( p *
  s1 ) + T1 - T2 ) * R2 )` is `TGModelUtils::LocalToLocalPoint( from, to, p )`: a repeated `Outer( a, Inner( b, p ) )`
  whose inner body retail calls out of line is that composite (its nested WorldToLocalPoint fails the budget and is
  called: OBBNode::BoxIntersectsBox's call set, OBBLeaf::FindRoughOBBIntersections' corners *(open, other walls)*;
  0x4358F0 and 0x435960 are LocalToWorldPoint's and WorldToLocalPoint's out-of-line copies kept from ObjectClass,
  annotated `@game/ObjectClass`; ObjectClass::GetLocalRandomPointAndNormalOnModel goes through LocalToLocalPoint and
  LocalToLocalUnitVector). When retail copies a point before
  transforming it, keep the copy and transform in place (`point = LocalToWorldPoint( node, point );`). The helper is
  not universal: where retail builds the point through the constructor 0x414540 plus 0x813A40 with no 0x41CBD0/0x426C30,
  the open-coded form is right (ExplosionPlumeController::VirtualSlot39, MetaSurfacePolygonizer::AddVolume): read
  retail's call set first. `array[i]->` read at each use, not through a cached element, gives retail's reloads through
  a pointer register (AsteroidTile::IsAsteroidPlaceFree *(open)*).
- **An out-of-line NiPoint3 constructor (0x414540) at a sum, difference or scale means an operator was written
  there** (its nested constructor goes out of line when the budget runs out). 0x4234A0 is the friend
  `operator*( float, const NiPoint3 & )`: `0.5f * ( a + b )`. Dot products are `NiPoint3::operator*( const NiPoint3 & )`,
  and operand order matters (`forward * offset` came closer than `offset * forward`, PulseWeapon::IsInArc *(open)*).
  `torque.x = added.x + torque.x` code is `operator+=` (Force::operator-=).

- **A copy retail makes of a `TGPoint3` return before a call taking `const NiPoint3 &`** can be the class's inline
  TGPoint3 overload casting `(NiPoint3)`: pass the temporary straight to the call
  (`SetImpulse( 0.0f, TGPoint3::GetModelForward(), 0 )`, ShipClass::StopInSystemWarp).

- **`rand() * ( 2.0f / RAND_MAX ) - 1.0f` is NiSymmetricRandom (NiMath.inl):** it gives retail's y, x, z argument
  order in a constructor (TGFuzzyClusterInnerController::Update, MetaNebula::AddSphere *(open)*), and VC6 then reads a
  `static const float` factor from memory as retail does (Nebula::CreateOverlay *(open)*). It is an inline level, so it can cost
  budget elsewhere (ExplosionPlumeController::VirtualSlot39 stays open-coded).
- **`position += x; return position;` rather than `return position + x;`** expands retail's sum into the return and
  moves `operator*=` out of line (ZoomTargetMode::GetIdealPosition *(open)*: its call set agreed before the body took
  retail's branch structure, which then expanded `operator*=` again).

- **Two-component maths on NiPoint2 is its operators even where retail shows copy-constructor calls:** `a = b - c`
  gives copy(*this), fsub x/y, copy(return): two NiPoint2 copy calls (0x46C2A0) per result, VC6 having no named return
  value optimisation. `x -= pt.x` (fld x; fsub pt.x) is `operator-=`, `fld pt.x; fadd x` is `operator+=`
  (TGIconPoly::SetClipRect *(open)* on a frame slot: its call set and code order agree with NiPoint2.h's Gamebryo
  `operator-=`).

### Spelling that changes the inline and call set: where an inline is defined

- **Header inline for a function emitted beside other inline copies.** A function retail keeps as a COMDAT copy in
  several units (GetObjType, IsTypeOf, class-name getters, `??_G`) is an inline in the header, annotated with
  `@<unit>` naming the unit whose copy retail kept ( `tools/comdat.py <unit>` finds them). A header-inline constructor
  can be expanded in one unit and kept out of line in another (TGStringEvent, `@game/Game`, expanded in Episode). The
  out-of-line copy of an inline (SetModelABV 0x7DCD60, NiD3DPixelFormat 0x7CAF40) exists because one caller didn't
  inline it: keep the inline; the copy is annotated where a unit still emits it, or parked.
- **A function defined in the `.cpp` without `inline` is expanded inside its own unit under `/Ob2` and called out of
  line from the others,** and its out-of-line copy appears in that unit: a function written `inline` in a .cpp whose
  copy retail keeps is a plain definition (NiStream::InsertObject, RemoveAllObjects); small getters defined in the
  `.cpp` stay out of line in other units, as retail calls them (TriShapeParticles); a constructor game units call out
  of line whose retail copy lies inside its own class's unit was defined in the `.cpp`, not the header
  (NiTextureProperty, 0x809FE0; NiRawImageData's 3-argument constructor, expanded in Unknown7DECF0, where moving it to
  the header loses its out-of-line copy). Definition order inside the unit doesn't stop it: `/Ob2` expands functions
  defined later in the unit too, file-`static` and member alike (checked on a scratch unit), so an out-of-line copy in
  retail (the TGA reader's GetPixel24) is the inline budget, not definition order or linkage.
- **/Ob2 expands a non-inline function defined earlier in the unit at some call sites and not others** (GetRemoteCam
  in ViewScreenObject). Writing its body out by hand matches but fails the reader test: those two functions are
  MATCH-PENDING until a spelling makes VC6 expand the call *(open)*.
- **Game units inline only header bodies across classes:** a member defined in the .cpp (CameraMode::IsTypeOf) isn't
  expanded in a derived class's chain, so retail's inlined parent chain means a header inline whose out-of-line copy
  still fills the vtable slot.
- **By-value classes block expansion.** VC6 SP5 never expands a function taking a class with a user copy constructor
  by value, even `__forceinline` (only the Processor Pack c2 does). A header inline that returns a class by value
  ( `NiPointer`) stays out of line in a game unit under `/GX` (UtopiaApp::GetRenderer, TGIconManager::GetCamera), and
  so does an inline taking a `TGPoint3` (which has a destructor) by value. In AppWrap, an inline that returns or takes
  by value a class with a user-declared destructor (TGPoint3, TGRect, TGString, TGColorA, NiPointer) is called through
  an out-of-line COMDAT retail keeps from AppWrap (Cross, UnitCross, GetExitPoint, SetFillColor). An inline `new` of a
  class with a constructor under global `new` likewise (the AI `Create`s); pool-allocated (TGPOOL) Creates are
  expanded.
- **Event constructors inside a `new` stay out of line** even when declared inline (TGBoolEvent, TGIntEvent); their
  COMDAT copies verify, which is expected. The inline budget decides per function: the same `new` can be expanded in
  one function and called out of line in another of the same unit (TGIntEvent).
- **`inline` on template members doesn't matter:** VC6 /Ob2 treats NiTArray and NiTList template members the same
  either way, and `/Ob1` didn't move the template copies behind `Ni3DDeviceDesc::operator=` either.
- ** `#include <new>`** gives `operator delete` a `throw()` declaration: it removes a spurious EH state store at the
  end of destructors (NiFile, NiProvider_Info, TGFontManager).
- **A game unit that calls the CRT `_iswspace` (0x85C816) includes `<ctype.h>` before `<iostream>`/`<wchar.h>`:**
  otherwise wchar.h's C++ inline expands it to `iswctype` (TGParagraph).

## The inline budget

VC6 stops expanding inline functions after a per-function budget. The rules are read from the compiler itself and
written in `docs/c2-backend.md`; `tools/c2trace.py <addr>` prints them at work on one function (each site's cost,
the budget left and the verdict; `--cost` prices a spelling in IL tuples). In short: a callee's cost is its IL tuple
count; 40 or less is free and 128 or more never expands; the budget is max(1000, 2 x the function's own cost); sites
are visited depth first in source order; a nested level gets `budget left / sites not yet visited`, free sites
counted. The entries below are what was measured from outside before the tool existed: read them as examples.

- **A class's own accessor or setter at every use adds one site per use:** only the COUNT of sites after a nested site
  moves its share, not their cost, and free sites count in the divisor too, so removing them changes nested shares
  (keep the site count when trimming cost): an accessor written at both uses instead of read into a local adds one
  site and shrinks every earlier share (`unknown0C.GetAt( index )` in both compares,
  NiKeyframeManager::Sequence::Unknown7A24B0: MATCH). When `c2trace` shows one or two sites missing after a nested
  site, look first for an evidenced accessor beside a raw member access: `GetContainingSet()` in its test and its call
  (SpaceCamera::CreateSpaceDust), `IsStaticOn()` (ViewScreenNode::SetOffTexture), Gamebryo's `SetIndex()` for `index
  =` (NiLODNode::VirtualSlot35; ViewScreenNode::SetOffTexture's is exact only under /YX, an accepted loss of the
  no-PCH game build). A missing lineage inline in our headers can be the missing site (NiAVObject::SetTranslate( x, y,
  z ) closed CameraObjectClass::Unknown42A290; NiTimeController::SetBeginKeyTime gave WarpFlash::CreateFlash retail's
  call set *(open)*). When several equal shares straddle a callee's cost, work every site: a site whose nested
  expansion is no longer charged hands its units to the next shares (TacWeaponsCtrl::Unknown548E60's five NiColorA
  copies at exactly 69 *(open)*).
- **The budget is arithmetic:** in a function above 500 own cost, saving N tuples of its own body lowers the budget by
  2N: a `scale` local for a three-times-written `visibility * 0.8f` saved 6 (MetaNebula::CreateExterior), `if ( gamma
  )` for `gamma != NULL` (operator T * instead of operator!=) saved 2 and put NiLight::SetAmbientColor (cost 42) out
  of line (BridgeObjectClass::SetupLighting). A late-rejected UDT expansion is charged too ("An inline returning a
  UDT" above).

- **Padding doesn't move the budget:** dead statements, dead inline accessor calls, a live store and eight live one-store
  setters ahead of the body left NiApplication::Terminate's NiPointer release inlined; what moves it is which nested
  NiPointer/NiObject sites exist and how they are spelt. A NiPointer test spelt `p == NULL` ( `operator==( T * )`)
  rather than `!p` changed which releases expand and share a tail (NiApplication::CreateRenderer *(open)*): take
  the spelling from the lineage source.

- **The budget limits how large the function being compiled may grow, not how many expansions it makes:** once the
  function is big even small callees stop fitting while smaller ones still do, and the last sites, dividing what is
  left among fewer sites, keep their deep inlines (AIManager's first TGIntEvent out of line, the second
  inline); sites nested in an inline wrapper that holds the new-expression ( `TGIcon::Create`) forward
  (PlayerReticleWindow's first eight icons inline).
- **The shared NetImmerse inlines have no evidence-free header fix left; their walls are each caller's own budget.**
  `callset.py` shows NiPoint3's constructor and operators, `NiObject::DecRefCount`, the NiPointer members and
  `NiProperty::SetField`/`SetBit` disagreeing with retail in both directions, yet their bodies equal Gamebryo's and
  retail's own out-of-line copies. Freeing one inline site in a function can expand an earlier site rather than the
  nearest one (TGAnimNode::SetExclusiveAnimation). Work such a function through its own inline sites. The one header
  source that moved them is the SDK debug copy's text (DecRefCount's assert, below; user's ruling).
  Didn't help in: every header variant A/B'd over the tree, each a net loss (measured before the NetImmerse headers
  took the debug copy's text and DecRefCount its assert):
  - NiPoint3's copy constructor and `operator=` are user-declared in this NetImmerse (Gamebryo declares neither):
    dropping them loses hundreds of matches, since VC6 then copies bitwise and never calls them.
  - In-class against after-class (`.inl`-style) definitions of NiPoint3's inlines change no inline decision.
  - TGPoint3's constructors assign the members; delegating to NiPoint3's loses TGPoint3::UnitCross and the
    0x429650 copy.
  - Where a NiPointer member is defined is a budget lever: defined outside the class it costs the caller more, and
    the `inline` keyword changes nothing. `~NiPointer` or the `T *` constructor out of class matches
    PulseWeaponProperty::Unknown68CDF0 and TractorBeamProperty::Unknown697970 and loses more; every member out
    (Gamebryo's layout) and both `operator=` in the class are net losses.
  - `operator=( T * )` taking the reference through the member (Gamebryo's body) matched GlowRenderer::RenderPass and
    NiApplication::Terminate and four more, and lost nine (SetClass's constructor and destructor, NiFrameRate
    Init/Shutdown, NiD3DDirectDraw::Create, NiPropertyState::Reset): it stays the argument form. RenderPass and
    Terminate matched instead through DecRefCount's assert (next item).
  - `DecRefCount`'s body is settled: `int count = --refCount; if ( count == 0 ) delete this; assert( count >= 0 );
    return count;`, the assert from the debug copy (AnimOnly 0x403780). It costs 68 with the assert (66 without), and
    retail's shares need at least that (RenderPass, Terminate); SetClass's and BridgeObjectClass's destructors and
    ~NiTMap 0x417270 are the accepted losses it costs (user's ruling). Moving `IncRefCount` out of the class is
    budget-neutral.
- **One expression, inline early and out of line late:** in a large function retail expands an inline in the first
  sites and calls it in the later ones, same source, budget spent: the vertex cache's `Get` and `NiTSet<G>::Add`
  (NiBezierTriangle::VirtualSlot47 and NiBezierRectangle::VirtualSlot47 *(open)*); NiTArray's `Add`/`SetAtGrow`
  (RotateBonesController::SetTarget); in 2.5-5 KB functions small inline virtuals late in the body
  (`TGBufferedFileStream::WriteID`/`ReadID` in UtopiaModule::ProcessSave and ProcessInternalLoad *(open)*). Match the
  sites in retail's order, not one spelling for all; accessor spellings earlier in the function didn't reproduce the
  late calls.
- **A CreateObject that expands its load record's constructors less deeply than ours is missing a record level:**
  the records mirror the object classes, and each empty level adds its (dead) vtable store to the budget. A class of
  its own (NiBoneLoadRecord: the vtable 0x899868 has one reference), a derivation through the parent's record
  (NiFltAnimationNode's from NiSwitchNode's) and a shared level for a family (NiLightLoadRecord under the four lights,
  NiTriBasedGeomLoadRecord under NiParticles/NiTriShape/NiTriStrip(s)/NiTriangles) closed seven CreateObjects. Probe
  with empty levels first, then find the class the depth fits; a level that breaks a matched sibling is the wrong place
  (one under NiDynamicEffectLoadRecord breaks NiPointLight). NiTextureEffect's, which no level explained, closed once
  the NetImmerse headers took the SDK debug copy's text: compare with the debug copy before inventing a level.
- **Accessor calls count against the budget.** `GetMenu()`/ `IsMenuEnabled()` instead of member reads left the nested
  `SortedArray::Get` out of line as in retail (CharacterClass::MenuEventHandler); accessor reads on the source object
  of an `operator=` ( `mode.GetDevice()`, `GetDesc()`) spend it the way retail does, so the NiTList members behind
  `Ni3DDeviceDesc::operator=` stay out of line (GraphicsModeInfo::operator=, exact through invented setters and so
  MATCH-PENDING; unused accessor calls, `const`, `/Ob1` or dropping `inline` change nothing there). The reverse, in budget-limited NetImmerse units: reading fields
  directly (with a `friend` declaration where needed) gave NiOBBLeaf::TestTriangles retail's pattern of which
  constructors are expanded (NiOBBLeaf is a `friend` of NiOBBRoot and NiOBBox for this). *Evidence required:* the
  lineage source reads the fields directly, or the class already declares the friend; a `friend` added only to move
  the budget is R17.
- **The geometry's own accessor is cheaper than the data's:** `shape->GetVertexCount()` where the source had
  `shape->GetShapeData()->GetVertexCount()` left room for the first cast's IsKindOf, as retail expands it
  (SpecularPass::CreateShape). Wall: MotionBlurPass::CreateShape and Display keep one site each on the wrong side;
  retail's choices there (the last of several equal setter calls expanded, the first not) don't fit a budget spent
  in reverse source order alone.
- **Inline setters add a level.** An inline setter instead of a member store inside a header constructor moves the
  budget of every function that expands that constructor: NiAlphaProperty's `SetTestRef( 0 )` closed three functions
  at once. An inline setter wrapping an `NiPointer` assignment makes VC6 call DecRefCount out of line, as retail does
  (Sequence::SetTextKeys); wrapping an inline Find + RemoveAt in an inline `Remove( item )` likewise moves RemoveAt out
  of line (TGNetwork's destructor). Setter/getter pairs ( `dest->SetX( GetX() )`, `SetPathData`-style setters) give
  retail's out-of-line NiPointer assignment and SetBit copies and turn float copies into fld/fstp
  (NiPathController::CopyMembers). A setter whose body calls something under `if ( b )`, called with a constant
  `false`, explains an EH frame without state stores (the NiPathController constructor).
- **`array.Add( x )` is an inline site of its own;** `SetAtGrow( GetSize(), x )` spends the budget differently
  (DamageableObject::BreakOffPiece *(open)*).
- **AddChild versus `InsertChild( 0x7FFFFFFF, ... )` moves the inline budget in long UI builders:** one group spelt as
  InsertChild matched STSaveDialog::CreateControls. Keep the adds whose float arguments go through fld/fstp as
  AddChild: the inline wrapper is what routes a float global through the FPU. Didn't help in:
  TacWeaponsCtrl::Unknown548A00.
- **A temporary NiPointer from GetAt assigned straight to a raw pointer gets no EH state;** a named `NiPointer<T>`
  loop local does (ViewScreenNode::SetStaticTextureIconGroup).
- **A chained getter in a `new X( ... )` + AddTail leaves NiTList::NewItem out of line:** reading the member into a
  local first (`HullClass *hull = ship->GetHull(); if ( hull != NULL ) ...`) inlined NewItem as retail does, with the
  NiTListBlock constructor out of line (annotate it `@unit`) (WeaponSystem::VirtualSlot34).
- **A "NiTList::NewItem inline budget" note can hide a structure error:** before parking one, read retail's node
  links. Data at +0, next at +4 and prev at +8 set to 0 with next taking the old head is `AddHead`; the other way is
  `AddTail`. Look too for a membership test on `this` before a parent walk, and for a member read again after the
  first insert (TGRootPane::GainTrueFocusPath matched; TGUIObject::GetCommonAncestor's call set became retail's).
- ** `/Ob1` per unit** when retail calls a same-unit constructor, `SetText` or `ReplaceAnim` out of line that `/Ob2`
  inlines: the animation controllers, the key classes, NiGravity, the colliders, NiListener. Set it in
  `config/project.json` `overrides`; never module-wide. *Evidence required:* the flag is the library's
  (`docs/toolchain.md`, "Compilers and flags": NiAnimation and NiOldParticle are `/Ob1`); a unit whose library is
  `/Ob2` takes `/Ob1` on the outcome alone, which is open. The converse, a game unit built `/Ob2`, is taken on /YX's
  terms (user's ruling): both its matches and its retail section order improve, the evidence recorded beside the
  `overrides` entry (statics.md, "The `<iostream>` run's shape names the inline flag").
- **A dead out-of-line copy in an `/Ob1` unit beside a caller with the same body in place:** write the function
  (it has its own retail copy) and keep the caller's open-coded body; under `/Ob1` a call would stay a call and lose
  the caller (NiOBBLeaf::FaceFaceCollision 0x827930 with FindCollisions).
- **A SWIG wrapper that pushes constants for parsed arguments** means the inline it expands ignores those parameters
  (STButton::Create/CreateW, a `// BUG:`): read the inline's body before calling it a register wall.
- **`c2trace --whatif` turns an inline wall into a window before any spelling is compiled.** `site=N,+1` applied
  several times turns "which inline went out of line" into a count of sites (TractorBeamProjector::Fire *(open)*,
  DisruptorGraphic, TacWeaponsCtrl); `own=+N` / `own=-N` gives the range of the function's own cost that yields
  retail's call set (NiD3DFrameBuffer::CreateSurfaces needed 533..568: below it operator='s share misses, above it
  SetAtGrow inlines; PhaserProperty::Unknown686960); above 500 own cost the budget is twice the own cost. Run it on
  register-classed walls too: several are inline-budget walls (one callee's call count differs). The window only
  narrows the search: a spelling inside it still needs its own evidence, and one chosen only to land in it, where the
  family writes it otherwise, stays MATCH-PENDING (PositionOrientationProperty::Unknown689530's `!( i % 2 )`).

- **A unit's own non-inline helper is expanded under /Ob2 with its arguments evaluated before its body:** an early
  member load ahead of an inlined memset is that helper's argument (NiD3DFrameBuffer::CreateFlippingSurfaces with
  GetSurfaceDesc); an argument evaluated ahead of the others is an inline call (NiD3DRender::VirtualSlot23).
- **A base class's inline accessors used in a derived method supply missing inline sites;** a bool passed through a
  stack temporary marks an accessor's return (UtopiaApp::Initialize with NiApplication's GetFullscreen *(open)*).

- **Every retail caller calling an in-class body that costs under 40 out of line** means it was defined in the .cpp
  (TGBufferedFileStream::ReadID/WriteID): check the xrefs.
- **A ternary argument with a call in one arm** compiles to a float temporary; retail's two push tails are an if/else
  with one call per arm (SensorSubsystem::ScanAllObjects).

- **A destructor or NiPointer release retail runs twice on separate paths** is an early `return` inside the object's
  scope (BridgeObjectClass::CreateAlphaTexture, as CreateTexture).
- **An inline constructor that allocates in its initialiser list is called out of line from an EH context;** the same
  allocation as a body assignment is expanded (TextureAnimTrack::TextureAnimTrack).
- **Retail copying an inlined result from a temporary into a named local** means `T x; x = f();`, not initialisation;
  the operator= site moves the budget (CharacterClass::UpdateWatching; CharacterClass::LookAtMe *(open)*).
- **NiPoint3::operator= called out of line at a top-level site with a budget near 1000** can't be a budget decision:
  the copy sits one inline level down in retail, where the share is under 57. Most of `callset.py --callee 0x414560`'s
  functions aren't this case: their differing sites are the last ones of a large function, or follow an out-of-line
  constructor (a budget tail, worked site by site). The real cases, and what decides them:
  - A member store through a setter the class hides: ShipClass's `SetAcceleration( TGPoint3 )` (slot 91) hides
    PhysicsObjectClass's, so ShipClass code stores the acceleration as `PhysicsObjectClass::SetAcceleration( x )`, a
    non-virtual inline whose copy gets a small share (ShipClass::Unknown5ADBD0, all three stores *(open)*).
  - TGPoint3's copy assignment is written out member by member (cost 57, like the constructors), not the implicit
    one that forwards to NiPoint3's (cost 42, with NiPoint3's copy nested under it): ShipClass::VirtualSlot88 calls
    `??4TGPoint3` (0x5B5B40) out of line, and OptimizedFireScript::PredictTargetLocation's call set agrees (both
    *(open)*).
  - The axis copies of TractorBeamProperty::Copy and PhaserProperty::Copy: an inline holding both copies reproduces
    retail's calls in a probe (share 1040/39 = 27), and PulseWeaponProperty::Copy's same spelling expands only because its
    13 sites leave a share near 90. TGPoint3 locals give the same calls but an EH frame retail lacks. *(open: no
    evidence names the wrapper)*
  Didn't help: one struct copy of MetaVolumePolygonizer::SetVoxelData's two corners (share 65, and the expanded
  DecRefCount at [1] needs more budget, not less); one wrapper for both box copies of ShipClass::Unknown5ABAC0 (only
  the first goes out of line).

- **A retail out-of-line copy of a unit's own helper with no callers and no pointer references** means every caller
  inlined it: the callers' hand-written copies of its body are calls to it, which lowers their own cost and moves the
  budget (NiD3DFrameBuffer::AttachSurfaces with GetSurfaceDesc and GetSurfaceSize matched, and with them the
  NiD3DPixelFormat copies). `tools/nocall.py` lists such copies with the open callers that hold their referents.
  Under /Ob2 (all of NetImmerse) an ordinary member's uncalled copy is the same signal. An uncalled *gap* function is
  the best lead of all: an unidentified helper a caller writes out (NiD3DTexturePipeline::Unknown7CE4E0, out of
  Unknown7CE040's else branch).
- **An out-of-line copy of a non-inline member sitting among a unit's COMDAT copies** means the member was an inline in
  its header: retail expands it where the budget allows and calls the copy where it doesn't (NiD3DPixelFormat::operator=
  0x7CAF80: expanded in NiD3DFrameBuffer::CreateSurfaces, called from AttachSurfaces). `tools/comdat.py layout` shows
  the run.

- **Statement order moves the budget:** placing `direction.Unitize()` after the head-vertex copy made VC6 expand
  Length inline as retail does (SparkParticleController::VirtualSlot35).
- **A test's spelling is inline cost: a truth test `if ( p )` rather than `p != NULL`** (and `if ( !p ) return NULL;`
  rather than the `== NULL` form) saves 2 tuples of own cost, on a NiPointer or a raw pointer alike; above 500 own
  cost that lowers the budget too, and nested budgets tip on it (Editor::HighlightNode, SortedArray::Delete;
  MultiplayerGame::DeletePlayerHandler *(open)*). Run
  `c2trace --whatif own=-N` first: when it flips exactly the differing sites, convert the tests
  (MetaSurfacePolygonizer::Unknown4B8AE0, AddGlowPasses); when it doesn't, they don't help (CloakingSubsystem::Unknown55E840).
- **NiTArray null tests compare against `T( 0 )`:** SetSize compares `base[i] != T( 0 )`, and NiPointer has
  `operator==`/ `operator!=` taking `const NiPointer &`; VC6 keeps the temporary (setne, out-of-line DecRefCount)
  (NiBSPNode::CreateClone/CreateObject, NiFlipController::SetImage, NiTArray::SetAt, AddFirstEmpty).
- **The pooled `operator new` is two-level:** TGPOOL_ALLOCATION calls the inline TGPoolManager::AllocateObject
  (invented name; its out-of-line copy is 0x40F030, 30 callers), which calls GetPool()->Allocate(). The extra level
  moves the budget of every allocating function: DamageableObject::DeadObjectRemovalCheck needs `TGEvent::Create()`
  and `SqrLength()`; the faithful header wins over a match it costs (NetFile::SendNextFile).
- **The verdicts at several sites solve for a shared inline's cost; then look for the spelling with that cost.** Within
  one function, partial expansion across sibling sites bounds it: four SetChannel sites with R and G expanded, B and A
  called need `L >= 2s + 4x`, `L < 3s + 4x` and `L < 4s + 3x` (L the budget left, s SetChannel's cost, x
  GetShiftAndBits'; EnumTextureFormatPalette8). Across functions, matched callers pin it: when sibling functions flip
  expansions in one pattern, or each matched caller's verdict bounds the cost from one side, the window is the cost the
  header body must have (NiTSet::Realloc with 1.1.2's nested size test, cost 127 against 120, gave three Bezier
  VirtualSlot44s retail's call set; NiTMap::SetAt's 195-196 window: lineage.md, "A brace pair costs two tuples"). The
  spelling with that cost comes from the lineage or the debug copy, never from padding.
