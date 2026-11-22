# Lineage source and compilers

What the Gamebryo 1.1.2 SDK headers and 1.2.2 source give, and which compiler builds which units. Read `README.md` for
how entries are written.

## Gamebryo lineage source (1.1.2 SDK headers, 1.2.2 source)

- **A helper invented for a behaviour may exist in the lineage under another owner:** search Gamebryo for the verb
  (Swap*, Copy*, Set*) over the types involved, not only the invented name. GlowAccumulator's invented SwapProperties
  was 1.1.2's `NiPropertyState::SwapProperties( NiPropertyList & )` (NIPROPERTYSTATE.INL). A header member added on
  lineage evidence can flip declaration-sensitive functions: run `build -s` before committing and list the flips.
- **Gamebryo 1.2.2's DX8 renderer (`CoreLibs/NiDX8Renderer/`) is lineage for BC's NiD3D renderer:**
  NiDX8LightManager's local flags (`bAttenuation`, `bChanged`) produce retail's shared blocks, the compiler threading
  the constant flag into a jump to the shared SetLight or attenuation code, and its constants settle referents
  (`GetDimmer() < 0.01f`: NiD3DLightEntry::Update and NiD3DLightManager::Unknown7D1570, both *(open)*, with
  UpdateLights' `NiColorA` ambient accumulator (alpha never read: retail's 4 extra frame bytes) and the LightEntry
  `GetIndex()`/`GetD3DLight()` accessors). NiDX8VBManager::GenerateVertexParameters' single stride accumulator moved
  NiD3DVertexBufferManager::Unknown7CB8E0. Coverage ends there: 1.1.2 ships no DX7 renderer (DX8/DX9 headers only),
  and the texture manager, texture pipeline, NiDDImage and NiD3DDirectDraw have no DX8 counterpart.
- **Moving an out-of-line body into its header as an inline** keeps its matched copy when `comdat.py layout` places the
  address in a unit's template run; annotate it with that `@unit` (SetFromDDPixelFormat 0x7C7830 @NiDDImage).
- **A 1.2.2 assert can be live code in BC's older NetImmerse:** NiTriIntersect's parallel-edge tests end in
  `else if ( fabsf( m.Dot( diff0 ) ) <= epsilon ) contactPoint = ...;` in retail, an assert plus an unconditional tail
  in 1.2.2 (PEdgeQFaceIntersect, PFaceQEdgeIntersect *(open)*).
- **A frame copy of a member passed to a `const &` setter** is `SetX( GetX() )` through a by-value lineage getter
  (NiParticleSystemController::CopyMembers with GetInitialNormal).
- **Restore header and `.INL` bodies from the lineage literally:** a header inline that differs from its lineage body
  moves every caller's inline budget, so check the SDK's `.INL` files before calling a wall "budget". The 1.1.2 `.INL`
  headers are as useful as the 1.2.2 `.cpp`: closer to 2002 for templates and accessors. UnitizeVector's explicit
  `x*x + y*y + z*z` instead of SqrLength() closed NiTriShapeData's CalculateNormals (NiTriStripData::CalculateNormals
  *(open: x87)*); restoring `SetAnimType`'s `SetActive( type == APP_TIME )` (NITIMECONTROLLER.INL) and dropping the callers'
  explicit calls closed ViewScreenNode::SetStaticTextureIconGroup and TGAnimBlender::AddNode; NiPlane::WhichSide from
  NIPLANE.INL (int, explicit components, else-if), NiMatrix3's column constructor through SetCol, and
  `GetCenter()`/`GetRadius()` in NiBound's code. A fewer-site spelling isn't always closer: NiTArray::Add taking
  `const T &` (Gamebryo 1.1.2) breaks the five NiPointer Add copies and six callers; NiTArray::Add is `void Add( T )`
  in this version (copy 0x7E60C0). 1.2.2's `GetCol( i, NiPoint3 & )` loses NiCamera::UpdateWorldData (GetCol: below).
- **Gamebryo 1.1.2 property types:** `virtual int Type()` plus an inline `static int GetType()`; indexing with
  `X::GetType()` spends the budget as retail does (NiPropertyState::Reset). Viewer strings read through the lineage
  accessors (`GetStencilOn()`, `GetSourceMode()`), and `NiBool` (unsigned char) picks another NiGetViewerString
  overload.
- **Setters that add a lineage level.** NiLight-family setters call `IncRevisionID()` (NILIGHT.INL, NIPOINTLIGHT.INL,
  NISPOTLIGHT.INL), not `revision++`; the extra inline level moves callers' budgets (PhaserGraphic::Start rose with
  it). NiMaterialProperty's setters call `SetChanged( true )` (NIMATERIALPROPERTY.INL; the user's ruling takes the
  lineage over the declaration-sensitive match it flipped): it matched TGAnimNode::Unknown6C5520 and
  MetaVolumePolygonizer::CreateDebugNode (MATCH-PENDING on its invented SetDebugNode) and moved
  CharacterClass::Unknown668000 *(open)* toward retail (SetBit out of line); Editor::HighlightNode, which it lost,
  matched again from its own NiPointer tests. Didn't help in: WarpTrail::WarpTrail (IncRevisionID;
  BridgeObjectClass::SetupLighting matched later from its own NiPointer test), and it dipped
  LightObjectClass::CreateNiSpotLight, PhaserBeam::PhaserBeam and MetaNebula::CreateExterior with their source
  unchanged.
- **Stream reads go through the lineage templates.** `NiStreamLoadBinary(stream, T&)` calls
  `NiBinaryStreamLoad(binstream, T*, count)`, whose empty `if ( count == 0 ) { }` (beside a release-dead `assert`) is
  the original text and makes VC6 form the value's address before it loads the input stream: it closed the LoadBinary
  family (NiFloatController::LoadBinary). Enums go through `NiStreamLoadEnum` (an `int` temporary, then a cast), read
  straight into the destination element (NiAnimBlender::LoadBinary); saves through `NiBinaryStreamSave`.
- **"Retail rebuilds the element's address for each argument"** is the per-index accessors ( `data->GetKeys( i )`,
  `GetNumKeys( i )`; NiMorpherController::GenMorphInterp).
- **Member layout from 1.2.2:** separate `NiPoint3` members instead of an array remove an
  `eh vector constructor iterator` retail lacks; local `float[3]` arrays instead of a struct wrapper change VC6's
  auto-inline choice (NiTriIntersect's constructor; NiTriIntersect::Find, MATCH). Its `Triangle` pointer members
  as 1.1.2's arrays: inline.md, "A member reached through a nested struct".
- **Open-coded loops, early returns and `result == TERMINATE_COLLISIONS` shapes** replace invented helpers (NiOBBNode,
  NiOBBLeaf); a centroid `/ 3.0f` through NiPoint3::operator/ matches (NiOBBNode constructor).
- **Braces change codegen, line breaks don't:** 1.2.2's braceless `if ( x < lower.x ) lower.x = x; else if ...`
  matches and the braced form doesn't (NiOBBNode::ComputeOBB, `RULE-EXCEPTION: R35`).
- **A saved pointer read as a "has data" flag**, the local then reused for the created object, explains a slot shared
  by two meanings (NiAVObject::LoadBinary).
- **Retail can be a subset of 1.2.2:** NiOBBox::TestIntersection lacks 1.2.2's motion axes *(open)*. 1.2.2 does not
  help pure x87 walls where its expression equals ours (its NiOBBox world accessors and `operator*` dot product
  compile like ours in TestRay), inline-budget walls (GetViewerStrings, CreateObject), or bodies whose algorithm
  changed (QLAlgorithm, Slerp, the particle modifiers).
- **Gamebryo 1.2.2's NiOptimize tool functions (`ToolLibs/NiOptimization`) and the NiGeometryData index sort were
  NiTriShape members in NetImmerse 3.1** (the weld family: IndexSort, IndexQSort, ChoosePivot, CollapseVertices).
  Win32/NiSystem.cpp's function order (NiRotr, NiStandardizeFilePath, NiGetFileSize) settled the NiSystem/NiFile
  unit boundary.

- **`NiPoint3::Length` is `NiSqrt( x*x + y*y + z*z )`,** Gamebryo 1.1.2's `float( sqrt( value ) )` from NiMath.h
  (NIMATH.INL), not `sqrtf`: the inline's float parameter rounds the sum and stores the root straight off `fsqrt`
  (`fst` before the pops) as retail does (_wrap_NiPoint3_Perpendicular, SetClass::GetDistance,
  CollisionEvent::WriteToNetStream, NiCamera::ScreenSpaceBoundBound). NiPoint3.h includes NiMath.h, as Gamebryo's
  does. `VectorLength( NiPoint3 & )` writes its sum out (through `SqrLength()` the caller called SqrLength out of line:
  OptimizedFireScript::TargetVisible). `x * scalar` and `scalar * x` compile identically (VC6 orders a commutative
  float multiply itself); the text follows Gamebryo. UnitCross is the exception (below).

- **An inline's body text can decide its callers' budget where its placement doesn't:** moving the NiObject-family
  and math accessors after their classes as Gamebryo's .INL files place them (NiCamera, NiAVObject, NiNode,
  NiTimeController, NiProperty, NiGeometryData, NiTriShapeData, NiPoint2/3, NiMatrix3, NiColor, NiPlane) moved no real
  match, only the declaration-count functions (walls.md). Writing NiNode::GetAt as NINODE.INL does (`if ( size <= i )
  return 0; else return ...;`) closed PulseWeaponProperty::Unknown68CDF0 and TractorBeamProperty::Unknown697970.
  A lineage type can settle an identity: NiCamera::SetScene( NiNode * ) made 0x416FA0 NiPointer<NiNode>::operator=.
- **Where BC's NetImmerse differs from the 1.1.2 text** (the bytes win, by the user's ruling, with a comment at the code
  naming the deciding functions): UnitCross keeps its early return without 1.1.2's `else` (the `else` loses
  _wrap_NiPoint3_Perpendicular: retail tail-merges the zero result and calls Length out of line); NiACos has one
  `else` (Gamebryo's two lose PlaceByDirectionMode::Update, the form without one loses NiQuaternion::Squad);
  NiMatrix3::GetCol( col, f0, f1, f2 ) reads through GetEntry (direct entries lose NiCamera::UpdateWorldData);
  NiPoint2::Length keeps `sqrtf` (NiSqrt loses DisableGlossMaps' loop branch target); NiSymmetricRandom multiplies by
  a folded 2/RAND_MAX; NiRect's viewer string uses `%f`; NiMatrix3::MakeZero and MakeRotation( angle, axis ) and the
  NiQuaternion members are out of line in retail. NiTArray::SetSize stores the new maximum before it saves the old
  block (the store and the copy tie in the scheduler: TGIconGroup::TGIconGroup, TGUIModule::RegisterClasses,
  OTREventHandlerModule::VirtualSlot00) and writes its early return without braces (two tuples of inline cost:
  NiTransparentProperty::GetViewerStrings expands its third Add's SetAt, LODModel::AddGlowPassTexture keeps SetSize out
  of line). The same saving in SetAtGrow is not taken (user's ruling): it would match NiGeometryData::GetViewerStrings,
  NiRawImageData::GetViewerStrings and NiMultiTextureProperty's constructor, and lose LODModel::AddGlowPassTexture,
  NiImage::GetViewerStrings, NiMultiTexturePropertyInit and NiMultiTextureProperty::CreateClone. NiTList::Get tests
  its position (`pos ? element : T()`, as GetNextPos does; 1.1.2's Get only asserts): the walk shapes it gives are in
  control-flow.md, "NiTList walks".

- **A brace pair costs two tuples of inline cost, so a lineage body's unbraced `if`/`else` is the bytes' evidence, one
  body at a time.** NiTList::RemoveHead and RemoveTail are unbraced as in NiTPointerListBase (four units less:
  ConditionalAI's destructor expands NiTMap::RemoveAll after four RemoveHead sites). The rest of the lineage's unbraced
  bodies don't carry over: AddTail's if/else (gains UtopiaModule::AddStateFunctions, loses TGFontFamily::ReadFromStream
  and NiTListBlock's constructor copy), RemovePos's and FindPos's ifs (lose NiD3DLightManager::RemoveLight) and
  NiTMap::GetFirstPos's if (loses BridgeObjectClass::SetupLighting) stay braced. A compiled-out `assert` still costs
  two units (`((void)0)`), and a dead local costs its declaration and uses. Gamebryo's `const` on NiTMap's getters and
  hooks adds one unit each and moves nothing. NiTMap::SetAt is bounded at 195-196 units by LODModelManager::
  ReadFromStream (out of line at its second site) and SetClass::ReadFromStream (TGPointerMap's sites); Gamebryo's
  uiDepth counter and assert give 200 *(open)*.

- **NiMath.h and NiPoint2.h carry Gamebryo 1.1.2's members** (NiAbs, NiSin, NiCos, NiExp, NiATan2, NiSqr, NiMin/NiMax...;
  NiPoint2's `-=`, `/=`, `/`, unary `-`, dot `*`, ComponentProduct). An open-coded `(float)exp( ... )` or `fabs` whose
  retail code copies through an inline's float parameter is the wrapper (NiGravity::Apply matched with NiExp); it is
  an inline level, so it also moves the caller's budget: the float-parameter copy in retail's code is the evidence,
  not the score.

## Compilers

- **SP5** builds everything except: the C and C++ runtimes (RTM, `"compiler": "rtm"`), and the four sound units
  NiListener, NiSoundSystem, NiSource, NiProvider_Info, which take the Processor Pack back end (
  `"unit_compiler": "pp"`: EH tables with the unwind map first, tail jumps, NaN-aware float compares).
- **Processor Pack units** (NiListener, NiSoundSystem, NiSource, NiProvider_Info): retail reuses one set of float
  locals across groups ( `RULE-EXCEPTION` R30); a Miles out-parameter is filled straight into an NiPoint3; bool
  results are `if ( !x ) return false; return true;`; a named float result in one branch blocks tail merging.
- **Precompiled headers:** game units build without one, NetImmerse units with VC6's automatic `/YX` (user's ruling); a
  game unit takes `/YX` alone (`"auto_pch_units"`) only when both its matches and its section order improve with it.
- **SP4 is a dead end:** SP4, SP5 and RTM generate identical code for the game units.
- **The GameSpy SDK C module is `/O2 /G6`:** without `/G6` darray matched 5 of 19 functions, with it 19 of 19
  (trivial functions whose only residue was schedule). A third-party C library can carry its own CPU-tuning flag.
- **VC6 emits a file's static functions where they first become needed,** so retail's order inside an object isn't
  the definition order: use the order for object boundaries only (gserverlist).

## The SDK's debug NetImmerse 3.1 (AnimOnly.exe) statement by statement

- **Write the debug copy's statements first, then read the release diff.** `sdkdebug.py cmp <retail> --debug <addr>`
  with the copy's listing beside it (callees and constants read from AnimOnly) closed release walls that looked like
  register, x87 or loop residue, because a statement the release bytes can't see moves the inline costs and the
  operand numbering after it: NiBezierPatch::CreateSets' `unknown11C |= SETS_TEXTURES` after `= 0` (an imul's
  operand order), NiMatrix3::QLAlgorithm's NiAbs/NiSqrt and tqli's `cs *= ( sn = 1.0f / r )` (the x87 stack),
  NiTriShape's Collapse/ReallocateVertexArrays reading `this`'s NiGeometry accessors instead of a data local, the
  loop test folded into the `for` (CollapseVertices), NiD3DLightManager::UpdateLights' `pos = 0; if ( effects )`,
  NiD3DTextureManager::Unknown7D5B30's first eviction through Unknown7D5910 (expanded there, called for the second)
  and Unknown7D5640's `? true : false` for a comparison returned as `bool` (control-flow.md, "Bool results").
- **A statement in the debug copy that retail's bytes contradict is another revision; one they are silent on is the
  original's.** When the debug copy has statements retail lacks and adding them changes the release bytes (NiBox::
  CreateFromData's fabs and covariance difference, NiD3DRender::SetFrameBuffer without the restore, both *(open)*),
  the copy is another revision there: compare the release call set before adopting one. A redundant statement the
  debug copy proves and the release build drops without a byte of difference stays, with its `// BUG: redundant:`
  note naming the debug copy as its instruction (user's ruling: NiBezierCylinder::VirtualSlot49's second `split =
  true`, AnimOnly 0x4F40A1); the label still never excuses a statement no build shows.
- **Slot differences alone don't reach /O2.** When our /Od slots map one to one onto the copy's (same census), only
  local names and declaration order differ, and the release build sees neither (NiD3DLightEntry::Update *(open)* on
  other walls); for the names the /Od order narrows, class-model.md, "Under `/Od` a local's frame slot order".
