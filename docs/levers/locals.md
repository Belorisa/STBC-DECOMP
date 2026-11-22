# Locals and evaluation order

Named locals, declaration order and evaluation order: what moves loads, frame slots and register choices. A wall
reported as `register` starts here or in x87.md; the order to check things in is [triage.md](triage.md), and
`tools/framemap.py` says first whether the frame is in question at all. Read `README.md` for how entries are written.

## Locals and evaluation order

### Locals and evaluation order: named locals and call order

- **A member read retail makes between two calls of one argument list is a named local:** `long id =
  object->GetObjID();` after `char *className = object->GetClassName();` puts the ID load before GetObjType and moved
  the list position to a stack slot as retail has it (TGObject::FixUpLoadedObjects); the same pair, `long id =
  GetObjID(); char *className = GetClassName();`, holds the ID in a block-local callee-saved register across the call
  (STMenu::FixUpPointers). A call's result named before a virtual call that takes it ( `HDC dc = BeginPaint( ... ); if
  ( app->OnWindowDraw( dc ) )`, the lineage's own form) keeps the vtable load after the call instead of spilling it
  (NiMessageLoop::WndProc *(open)*).
- **A nested call or load retail makes before the outer call's other arguments is a named local.** VC6 evaluates
  arguments right to left and a nested call argument after the constant arguments are pushed, so retail calling it
  first means the source named it first: the parent's module dict (UnloadOtherModules, one merged `add esp`); a
  `detailPath` (ImageManagerClass::GetImage); a returned pointer passed into a three-argument call
  (`NiImage *headImage = ...SetImage( ... )`, CharacterClass::ReplaceBodyAndHead); the argument of an override that
  only calls the base ( `TGPane::SetFocus( GetNthChild( 0 ), layout )`, ConsoleWindow::SetFocus); both arguments when
  retail calls `__ftol` for one before pushing the reference arguments (SetClass::StartPick); a `child` from
  GetChildSubsystem before the recursive call's outer arguments (ShipClass::Unknown5B3670, FindSubsystemsInRange); a
  float read from a stream before the target pointer is loaded (MultiplayerWindow::ReceiveGameInitializeMessage); a
  `GetString` call before the pool allocation of `new X( a, GetString(...) )` (TacWeaponsCtrl::Unknown548A00 *(open)*);
  a call result retail fetches before the constants are pushed (`call X`, then `push imm; push eax`) set in its own
  statement (TopWindow::ForceTacticalVisible, NamedReticleWindow::SetTargetID); a call's
  result before passing it to a virtual (`NiNode *flash = CreateFlash( t ); SetNiObject( flash );`: VC6 then loads the
  vptr after the inner call, WarpFlash::Init, ReadFromStream); a virtual result pushed before the other arguments
  ( `TGMessage *copy = message->Copy();`); `STSubPane *subPane = (STSubPane *)GetSubPane(); subPane->F( true );`
  fixing the push order around a virtual call (STTopLevelMenu::SetVisible).
- **Arguments of a constructor are evaluated right to left; members filled one by one, left to right.** Retail
  computing a NiFrustum's left, right, top, bottom in that order is `NiFrustum frustum; frustum.left = ...;`, and its
  NiRect port's constant stores scheduled among them with both slots live is the port declared first
  (ZoomCameraObjectClass::Create).
- **A load retail schedules early is a named local too:** a named local before a call ( `NiRender *renderer`, `window`,
  a per-second step, `const NiPoint3 &forward`) moves a load across the call the way retail has it. Members read into
  locals ahead of a call reproduce retail's load order (WarpFlash's ship constructor); a constructor argument loaded
  before the pool allocation is declared before the `new` (each key angle of RotateBonesController::SetTarget in a
  float local before its `new NiRotKey`); a table load scheduled before a CRT call
  (GetDetailTexturePath); a lookup scheduled before the error code's load (TGNetwork::SendTGMessage); a function-pointer
  table entry read into a local before the allocation call gives retail's `mov ebp,[table]` (TGAnimTrack::CopyFrom); a
  member read retail does before the lookups (the current ammo type in TacWeaponsCtrl::Unknown548120 *(open)*); a
  hoisted id before a list walk (`long id = object->GetObjID();`, CameraMode::GetLaggedRotation); a `TGPane *container =
  GetParent();` declared after the first calls gave retail's early `[this+0x14]` load (EngRepairPane::
  HandleSelectOption); `PyObject *x = NULL;` just before `CallFunction` (CharacterClass::SetStatus *(open)*). A global
  loaded once, stored and tested is a local copy (`AILogScope *parent = current;`, the AILog constructor), and so is a
  getter called once into a local and tested, then passed (EngPowerCtrl's `GetCurrentPlayer()`). A value retail reloads
  across a string-op sequence is a named local (`char *subdirectoryName = sub->GetName();` before the `strcpy`/`strcat`
  run: TGSystemWrapperClass::DeleteDirectoryContents, CopyDirectory). A float getter's result passed to
  `stream->WriteFloat` goes through a named local (fstp, mov, push; BridgeSet::WriteToStream). A property named before
  its float getter ( `X *prop = p->GetProperty();`) gives retail's fld/fstp where the chained call gives integer moves
  (ShipSubsystems::Unknown5B6170 *(open)*). The local must mean something; an unnamed temporary is R17 fake code.
- **A word member read through an inline accessor is extended at each use; a direct read fuses the extension into
  the load.** Retail's `mov di, word ptr [ecx]` with the raw word spilled and `and edi, 0xffff` at each use (and
  `mov r16, [m]` ... `and r32, 0xffff` for a second word) is an `unsigned short Get...() const` accessor whose result
  each use converts on its own; `xor reg, reg; mov reg16, [m]` with the extended value CSE'd is the member read
  directly, or a short local copied from it (VoxelSlice::GetSpan and IsSolid through GetSolidCount/GetSurfaceCount:
  MATCH).
  A header inline whose `?:` arm extends before the join, where retail extends after it, is an `if ( ... ) return
  ...; return 0;` body (VoxelSlice::GetRowCount). *Evidence required:* the accessor's name, or mark it invented.
  {asm: mov di, word ptr top; mov ebp, 0; mov dword ptr top, edi; and edi, 0xffff}
- **An array member reached through a pointer local keeps the field offset in the displacement.**
  `const VoxelSliceSpan *solidSpans = spans;` and `solidSpans[middle].firstRow` give retail's
  `lea eax, [eax + eax*2]; mov ebx, [ecx + eax*4 + 0xc]`; `spans[middle].firstRow` folds the offset into the index
  as one more element (`lea eax, [eax + eax*2 + 3]; mov ebx, [ecx + eax*4]`), and inline accessors returning the
  array fold the same way (VoxelSlice::GetSpan: MATCH; VoxelSlice::GetSurfaceSpan's `surfaceSpans` *(open)*).
  {asm: lea eax, [eax + eax*2]}
- **The reverse: a value retail reloads at every use has no local.** A getter written at each call
  (`subsystem->GetParentShip()`) closed STRepairButton::SetSubsystem; re-reading an event accessor
  ( `event->GetDestination()` twice) where retail reloads it, a cached local folding the reload
  (ReticleManagerWindow::ShipDestroyedHandler); re-reading an accessor per test also fixes a vtable register pick
  (OptimizedTorpsReady::SystemDisabled); an expression written at both uses (`( current + 1 ) % numLights`)
  where a `next` local doesn't match, because retail recomputes it after a store VC6 assumes may alias the operand
  (PulsingLight::Update). Retail keeping zero in EBX (`xor ebx, ebx`, then `cmp reg, ebx` / `push ebx`) where this
  build uses `test` / `push 0` means a long-lived local the source has took EBX: a pointer retail reloads from a member
  each time (`warpSequence->` rather than a local copy), or one local merged across branches that retail splits per
  branch (STWarpButton::HandleWarpEvent). {asm: xor ebx, ebx}
- **Retail loading every argument field in each branch before one shared call** is one local per argument, set in each
  branch (DamageCollisionTest::VirtualSlot01); a virtual getter called separately in each branch is a local per branch
  (CharacterClass::MorphBody).
- ** `x->F( x->G() )` as one expression shares the vtable load;** two statements reload it as retail does
  (STMissionLog).
- **An inline accessor as the object of an inline setter is evaluated before the setter's argument:**
  `GetAppWindow()->SetHeight( GetSystemMetrics( ... ) )` loads the window before the call; `appWindow->` loads it
  after (UtopiaApp::UtopiaApp).
- **Reading a global manager through its accessors instead of its members changes the load schedule**
  (TGSound::SetVolume; `IsEnabled()` covers `unknown1C && enabled`).
- **Every `sprintf` argument evaluated before the first push means the original named the arguments in locals,**
  declared in retail's load order: `char *className = object->GetClassName(); long id = object->GetObjID();` puts the
  ID load after the virtual call (TGObject::FixUpLoadedObjects). The order is decided by the bytes, so test both
  (Episode::WriteToStream/ReadFromStream).
- **A 16-bit local compared but the full dword pushed** to an int parameter: the call re-reads the accessor
  ( `LaunchEvent( event->GetUnicode(), ... )`, STMenu::KeyboardHandler).
- **Height computed before width but width stored first** is a two-argument inline call (arguments evaluate right
  to left; TGIconManager::SetScreenSize in ReticleManagerWindow::SetZoom).
- **An event's setter order follows retail's stores** (SetEventType before SetDestination).
- **A per-iteration float computed right after the iterator call, before its null test,** belongs inside the loop
  body, not hoisted above it (ApplySplashDamage).
- **A load hoisted above an early-exit test** ( `NiTListIterator item = list.GetHeadPos();` before
  `if ( root != NULL )`) reproduces retail's entry order (FocusManager::RemoveAllObjectsUnder).
- **A loop invariant computed before a plain `for` fixes its schedule;** an `if`-guard plus `do/while` is the
  decompiler's shape, not the source's (SetRedbookCDVolume).
- **Where a post-increment's store lands shows a helper call around it.** A counter stored before the destination is a
  post-increment through a returning helper: the class's static member `static long GetNextID() { return nextID++; }`
  (WeaponPayload.h) then `id = GetNextID();`; `id = nextID++` stores the id first (PhaserBank::RetargetBeam,
  TractorBeamProjector::UpdateTarget). A post-increment argument read before a later argument's call and stored only
  after it (the counter read, the call, then a reload, `inc` and store just before the outer call) is the argument list
  of an inline helper that makes the outer call, whose call is the sequence point; `sprintf( text, fmt, ...,
  numSaved++ )` stores at once. TGObject::WriteToStream and LoadObject are exact that way (FormatSaving/FormatLoading,
  invented names, with the member read directly, `id` rather than `GetObjID()`, to place its load after the call) but
  MATCH-PENDING, since nothing outside the bytes shows the helper (user's ruling, 2026-11-22).
- **`count++; return count - 1;`** gives retail's `inc word [m]; mov si, [m]; lea eax, [esi-1]` where
  `return count++` doesn't (SimpleFeatureCollector::AddVertex). {asm: inc word; mov; lea}
- **Accumulate in place where retail spills a sum and reloads it:** `end.x += width; if ( end.x > wrapWidth )`
  matched where a named local for the sum did not (TGParagraph::Unknown732D50, 0x732EA0).
- **A named local for a scale product** ( `float scale = width * 0.0015625f;`) stops VC6 folding two constants into
  one (TGInputManager::GetMouseX).
- ** `(float)flag` reloaded through a float slot before each call** is one reused named float local
  (PlacementEditorMode::Update); `and bl,1` before `and al,bl` is `bool result = true; result &= A(); result &= B();`.
- ** `PyObject_Str` into a local before `Py_DECREF`:** the macro would evaluate it twice.
- ** `strcmp`'s first argument** is the string loaded into EAX; inline `strcmp` in a `CompareKey` gives retail's
  unrolled two-character loop (TGPlayerList, GetIfRegistered).
- **Operand order of a pointer compare** decides `cmp [esp],reg`: `action == movieAction` (TGMovieManager::IsPlaying).
- **`this` in EBX where retail has EBP, with both allowed and no cost on either, is a spelling, not a bound:** trials
  dipped, and the SDK debug copy's statements closed all three seen (NiTriStrip::VirtualSlot23 0x80DDE0 and
  NiTriangles::VirtualSlot23 through GetVertexCount, GetTriangleCount and GetWorldVertices; NiAnimBlender::AddSequence
  0x7988E0). For a NetImmerse function, compare with its debug copy (`tools/sdkdebug.py fn <addr>`) before parking.

### Locals and evaluation order: declaration order and frame slots

- **A local whose address is taken interferes from the start of its enclosing block:** to share a dead parameter's
  slot as retail does, it sits in a nested block after the parameter's last read, e.g. the body under
  `if ( set != NULL ) { ... }` rather than after an early return (OptimizedFireScript::WeaponTooDangerous).
- **Arrays and structs pack as one block: a `NiPoint3 col[3]` array packs in index order; named points pack in the
  constructors' push order**, whatever their declaration order (CameraObjectClass::Unknown42C030/42C0B0). So two
  struct locals in retail's slots in the order opposite to ours, or packed as one block, are one array or one struct:
  `ProximityEndpoint endpoints[2]` for the lower and upper end (ProximityManager::AddObject, as ProximityEntry keeps
  them per axis); three floats read red-first into neighbouring slots are a `NiColor`, three direction floats a
  `NiPoint3` that shares a slot with another case's `NiPoint3` (LightObjectClass::ReadFromStream *(open: register)*).
- **`framemap` read and write counts are evidence of roles:** a retail slot with more reads is a different variable used
  at those sites (STFileDialog::CreateScreenshotArea's bars sized to `blockHeight` *(open)*); a slot written as an
  integer copy is the copy, not the computed value (TractorBeamProjector::HoldTarget: `strength` the product, `needed`
  its copy).
- **`str + " "` (the implicit TGString conversion) and `str + TGString( " " )`** give different temporary slots; the
  first gives retail's `lea` (WeaponsDisplay::Unknown54C200).
- **Open wall: a single-use float sum retail stores before an intervening call** where ours forward-substitutes it into
  the later argument (TacWeaponsCtrl::Init, Unknown548A00 *(open)*); cause not identified.

- **Two same-size buffers in swapped slots can be a missing statement, not a declaration order.** Past 0x80 bytes the
  packer sorts by references per byte (`c2trace --frame`; walls.md, "How VC6 SP5 orders frame slots"), so one more
  reference moves a buffer: retail's lone `mov byte ptr [buffer + 0x20], 0` was `name[32] = 0;` (the player name cut
  to 32 characters), and with it `name` sorted before `address` as retail's frame has it (UtopiaApp::Initialize *(open)*). Read the stores only retail makes
  into a buffer before calling its slot a wall. An initialiser retail stores after a guarded early return is declared
  after the guard (`int hostMode = HOST_MODE_NONE;` at the command-line test, same function).
- **Stores scheduled in retail's order come from statement order:** `float previousTime = 0.0f;` declared before
  `previousVolume`, and `passed++` after the paired volume assignment, closed TGSound::Update's last two swaps; a
  parameter read before a call that crosses its block ( `listenPort = port;` before the GameSpy delete) puts it in a
  callee-saved register as retail does (UtopiaModule::InitializeNetwork).
- **Declaration order of locals** sets frame slots where it moves their first reference (a declaration with an
  initialiser is a reference; the packer's order is first reference, not declaration: walls.md, "How VC6 SP5 orders
  frame slots"); retail's load order is a good first guess (slot 49's vertex loop:
  radius, colors, sizes, vertices, normals, then the corner offsets). It also decides register picks: `low` before
  `high` (a bisection sum), width read before stride (an imul), `slot = index++` before `icons = pane`, dividing in
  place (`x /= length; NiACos( x )` shares one frame slot) (PlaceByDirectionMode, ApplyColorTable, WeaponsDisplay). And
  which float locals stay in FPU registers: `float high = ..; float low = ..;` kept both, the reverse spilled one
  (TGStream's length codec 0x6D3A90). `tools/framemap.py` reading "frame and callee-saved registers agree" means the
  difference is elsewhere: declaration order is not the lever then.
- **Declaration order sets the entry stores:** the order of the NULL/false initial stores at entry follows it
  (NiReadSGIFile). `unsigned int i = 0;` declared beside the other zeroed locals fixes the order of the entry stores,
  where `for ( i = 0; ...)` moves the `i` store later (NiKeyframeManager::LinkObject). A zeroed ID cleared with `xor`
  before the stream setup is declared before the stream (ReceiveCreateTorpMessage). A zeroing store scheduled after a
  call is the local declared after the call: `unsigned int hash = TGHashString( key ); unsigned int low = 0;`
  (TGLocalizationDatabase::FindString), never a second `low = 0;`. A constant store early in the frame, in an
  argument's slot, can be a local's initialiser declared before the allocations, not an exception-state store
  (TGOverlayManager::BuildRandomSequence).
- **Block scope decides slot sharing.** A struct local at function scope gets its own frame slot; declared in a block
  it shares one with a later local (TGRedbookClass::CloseRedbookCD); retail sharing a slot between a temporary and a
  later local puts the later one in a block scope (TorpCameraMode::GetIdealDirection *(open)*). A named local whose
  dead slot ours reuses for a later statement's temporary, where retail reuses an earlier temporary's slot, sits in a
  block of its own: `{ NiPoint3 position = ...; object->SetTranslate( &position ); }` before `AlignToVectors(
  camera->GetWorldForward(), ... )` (PlacementEditor::PlaceInFrontOfCamera). Retail's frame smaller by a NiPoint3 or
  more, its loop temporaries in an early local's slots: the early locals sit in a block of their own (or an inlined
  helper's body; PhaserBank::FindFiringPosition's angle vectors). A block of its own also lets its address-taken
  neighbours share dead parameter slots (PlacementEditor::WriteRegionScript's LoadPlacements block: count in
  fileName's slot, written in setName's *(open: register)*). `framemap.py` shows the shared slots. Large buffers in
  the wrong frame slots: scope the error-path locals inside their `if` blocks; declaration order doesn't move them
  (Game::ClearMissionTempDir). Locals two loops share are one function-scope declaration (retail's shared frame slots,
  EngRepairPane::UpdateAreas), and so is one pointer reused through a run of like statements where retail has one slot
  (`NiNode *bone` for all six bones, RotateBonesController::SetTarget). A message printed into a buffer the function
  already has: a frame one buffer smaller than the source's means the not-found message reuses the same `line` buffer
  (TGTraceOptions::ReadOptions).
- **Loop counters: one per function or one per loop.** NetImmerse declares one `unsigned short i` at the top and reuses
  it in every loop; per-loop counters change the address-mode operand order (NiGeometryData IsEqual/LoadBinary/
  SaveBinary, NiGeometry::ApplyTransform). Game code can want the opposite: two loops in one block with separate
  counters ( `for ( int j ... )`, `for ( int k ... )`) fixed a swapped `[edi+ebx]` operand order in the second
  (ShipSubsystems::Unknown5B6330). One counter per loop (VC6 for-scope) is not a free frame-slot lever: it moved the
  inline budget instead (NiBezierTriangle3::Unknown83B370).
- **Parameter slots reused for locals.** VC6 reuses a parameter's stack home for a local once the parameter is dead or
  in a register (the BMP bottom-up flag, the TGA flags; VisibleDamagePiece's constructor and
  MetaSurfacePolygonizer::Unknown4B98B0, both *(open)*): a frame offset in
  the parameter area is not a parameter, nor a sign that the source reused the parameter. A float local whose address
  goes to a call can live in a dead reference parameter's stack slot: a frame smaller than the sum of its locals by
  exactly that local's size means this, not a missing local (GetTurnBetweenFrames' `angle`). A parameter's stack slot
  receiving a callee's out-pointer means a local reuses that slot; the test after the call is on the parameter itself
  (EngRepairPane::AddSubsystem).
- **Several arrays read into locals before a run of `memcpy`s** give retail's frame (TGUIBatch::Grow *(open)*).
- **Local char arrays with an element list compile to per-element stores**; read the array base from the call's
  `lea`, adjusted for the pushes already made (TGLocalizationDatabase::WriteSample's "ID1" keys).
- **A local struct array walked with `i < sizeof( a ) / sizeof( a[0] )`** gives retail's unsigned `jb` bound
  (NetFile::RequestChecksums); a byte store at a buffer's last element before a `sprintf` is
  `buffer[sizeof( buffer ) - 1] = 0;` (UITreeDialog::AddTreeElement).
- **Index math on the parameter, loop bounds on the member:** retail's beam constructors use `numSides_` for the
  vertex index and `numSides` for the bound (PhaserBeam, TractorBeamShape *(open)*).

- **A loop invariant retail computes before the outer loop's entry test, after the other entry stores, is declared
  inside the outer loop body:** VC6 hoists it (`float halfTile = tileSize * 0.5f;`, AsteroidField::CreateSlots
  *(open)*).

- **Register roles follow first-use order:** a pointer fetched once before a branch takes the other callee-saved
  register from the same fetch made inside each arm (VC6 merges the two loads); retail's "size compare first, entry
  load after" with ebx = entry came from fetching inside each arm (TGPhonemeSequence::Update).

- **An ID re-read for a lookup right after a virtual call lands in eax through an inline accessor** (`GetTargetID()`),
  in ecx/edx as a bare member read (Torpedo::VirtualSlot69; its twin StartFiringEvent::WriteToNetStream is exact only
  through an invented `GetTargetID`, MATCH-PENDING by the user's ruling).
- **Components set one per nested loop into one `NiPoint3` local, then copied whole,** give each component its own
  frame slot, where three float locals let the innermost share a temporary's slot (AsteroidField::CreateSlots *(open)*).
- **Each call's result tested in its own arm, jumping straight into a shared block,** is `if ( Call() ) flag = true;`
  per arm then `if ( flag )`; `flag = Call();` per arm leaves one shared test (WeaponsDisplay::UpdateWeaponTargeting).

- **Unused frame bytes can be array sizes, not unused locals:** compare `c2trace --frame` with retail's slot offsets;
  a 4-byte gap after each buffer means `_MAX_PATH` (260) buffers where the source has 256 (CharacterAction::
  ConvertFolderLWVs). In the same function, a fail branch that jumps past a `delete` to the epilogue is the original's
  leak (a `// BUG:` note), not a layout to steer.

- **Two parameter stores to members:** swapping the statements swaps the store order and which parameter slot a later
  float temporary reuses (PhaserGraphic::Start *(open)*).
- **A one-instruction schedule shift right after a region at its 81-tuple cap** (`c2trace --sched` shows which cut a
  diff sits on) means retail had a different tuple count upstream in that region: look for a non-emitting tuple, not
  at the local expression (PhaserGraphic::Start *(open)*). The class's existing accessors where the code reads members
  closed such a flip where a matched twin uses them (TractorBeamProperty/PhaserProperty::WriteInit, as
  EnergyWeaponProperty::WriteInit). In TGTextButton's frame trio inlined accessors moved no cut, and the tuples that
  can are op1a0 (after a constructor call), op162 (a substituted float held on the FPU) and labels *(open)*: which
  accessors leave a tuple is not settled.

- **A parent walk through the accessor at each use** (`while ( pane->GetParent() ... ) pane = pane->GetParent();`) keeps
  the step read through the walked pointer's register; a named `parent` step variable lets VC6 copy-propagate it into
  another register (ViewScreenObject::IsMenuUp).

- **Arguments copied to the stack before `operator new`** are the class's inline static `Create( args )`
  (STFileDialog::CreateContentArea).
- **A pointer argument stored to a frame slot at entry** is a local initialised from it before the calls
  (`polyColor = color; if ( !polyColor ) polyColor = white;`: TGIconManager::CreateIconPoly).
- **A global retail reloads after external calls has external linkage,** where a `static` would be hoisted
  (TorpedoController::UpdateFlares: MATCH).
- **A `push` inside a loop shifts every `[esp+N]` by 4:** check it before reading a loop variable as a mystery value
  (TractorBeamShape::SetDirection *(open)*).

- **A local that only renames a loop index or a member** can decide the register census (removing it closed
  TGStringToStringMap::AddMapping); it isn't universal (removing it made GetMaxAngularAccel and TGUIBatch::Grow worse,
  both *(open)*).
- **A store written by hand into both arms of an if/else is written once after it:** VC6 tail-duplicates it itself,
  and the register pick then matches (WeaponSystem::Update *(open)*).

- **A dead double store of an int around a 16-bit mask before an `fild` is a short-typed variable:** check the width
  of the data read (TGOverlayManager::ConvertLWVToLIP: the marker position is the 2 bytes read).
- **A sum on a hoisted common subexpression follows the matched twin's operand order** (`clock + ( duration - delay )`,
  as Play's `clock + duration`: TGTimedAction::Restart).
- **A member read where a twin calls the accessor flips a vtable register pick or a call's inline budget:**
  `STSubPane::Cast( GetParent() )` as STMenu::SetChosen (STButton::SetChosen), `GetContainingSet()`
  (BridgeObjectClass::ShakeCrew), `GetNode()` (AddAlertLight).
- **A float local used twice can be the drafter's:** retail's x87 slot for the second use (`fxch; fadd st(1)` leaving
  the older value) came from `offTime * 0.5f` written at both sites, CSE'd by VC6 (BridgeObjectClass::AddAlertLight).
- **Two values loaded onto the FPU before a branch and one popped after it are float locals** (register candidates):
  `float current = value; float interval = keyInterval;` (STNumericBar::Step *(open)*); member reads load in each arm.
- **A min of two `unsigned short` members kept in a 32-bit register (`and eax, 0xFFFF` after the pick) is an `unsigned
  int` local,** and `i++, j++` in a loop's increment orders the `inc`s (SimpleFeatureCollector::GetTriangles).
- **An if/else picking a flag adds front-end tuples a ternary doesn't** (about 15): enough to move a later inline
  decision across its budget share (ProximityManager::CollideDamageables, `c2trace --whatif own=+N` first).

- **`bool a[8] = { false, ... }` gives eight byte stores** where a clearing loop gives dword stores
  (MetaVolumePolygonizer::UpdateVoxel *(open)*); a float kept on the x87 stack across a fill loop is a local computed
  before it.
- **`NiPoint3 p = obj->GetWorldLocation(); offset = p - origin;`** keeps the copy on the FPU: retail's "load x, y, z,
  then subtract z, y, x" (BridgeObjectClass::DoCrewReactions, PlayerReticleWindow::Unknown512310,
  AsteroidField::IsShipNearSlot, all *(open)*); the one-line `obj->GetWorldLocation() - origin` doesn't.
- **One pointer plus a per-row `(src - dst)` in retail** is one int offset indexing both images,
  `dst[off] = f( src[off] ); off += stride;` (BridgeObjectClass::ApplyAlphaTable).
- **A shared subexpression retail spills to a named slot** (`vertex - numColumns - 1`) is a named local,
  `int above = vertex - numColumns` (AtmosphereShockWave/AtmosphereStreaks constructors).
- **An expression CSE'd across stores that could alias its operands** (`index - across` over `unsigned short` stores,
  `across` address-taken) was a named local (BackdropSphere/StarSphere::VirtualSlot71). An element compared in a
  search loop read into a local (`filed = list.Get( before )`) fixes the argument loads' registers (PyEmbed).
- **`char *copy = new char[...]; strcpy( copy, src ); arr[i] = copy;`** gives retail's store after the copy; filing
  first (`arr[i] = new ...; strcpy( arr[i], ... )`) reloads (PyEmbed::SetUnsavedModules).
- **A NiPointer declared, then assigned from `new`,** shows as EH state 0 and the NULL init before the `new`
  (TGIconGroup::LoadIconTexture). A lineage forwarding accessor (`shape->GetVertexCount()`) turns two inline sites
  into one, which can flip an earlier site's share (BridgeObjectClass::PrepareGamma).
- **A member operator evaluates its argument before the object:** when retail evaluates `sinf( w )` before the
  vector-returning call, the spelling is `GetOrientationRight() * sinf( w )`, not `sinf( w ) * ...`
  (PhaserBank::CalculateRoughDirection: MATCH).
- **Getters fetched in the order of use** give retail's register picks (ShipClass::SetTargetSubsystem: phasers,
  torpedoes, pulse, tractor). An unrolled quad written `colors[vertex++] = c;` four times, the last increment dead,
  gives retail's per-use `and 0xffff` masking (TGIconPoly::SetColor).
- **`size.x = size.x + position.x`, not `+=` or `position.x + size.x`,** with the point filled by an out-parameter
  accessor, gives retail's integer copy of only the y term (EngRepairPane::ResizeToContents).
- **Both operands of `&&` loaded before the first test** are named locals for both (NiKeyframeManager::IsEqual's file
  names); a value returned into a different frame home than the variable it was passed from is a second local of its
  own meaning (NiD3DRender::DrawStrips' pass buffer).
- **A bool stored to a frame slot as a byte and then pushed as a dword** is a named `bool` local passed by value, not
  the expression written in the call (NiD3DFrameBuffer::CreateSurfaces' `tripleBuffer`, in a braced `case` block).
- **A loaded `obj->member` pointer reusing the base register** (`mov ecx, [ecx+0xc]`) where ours takes a fresh one
  points at an inline accessor (NiD3DFrameBuffer::CreateImages: `GetDirectDraw()` on NiD3DDirectDraw, which the SDK's
  debug build confirms, AnimOnly 0x51E66E).
- **`strlen` written at each use, with no length local,** gives retail's repeated `repne scasb`
  (TGConfigMapping::SaveConfigFile *(open)*).
- **An inline accessor flips a commutative float add's operand order** where neither source order does
  (`row->GetBottom()` against `GetTop() + GetHeight()`: EngPowerCtrl::Unknown54D410 *(open)*).

- **A call's result named before it is compared with a member** makes VC6 load the member after the call into a
  scratch register; `Call() == member` loads the member first into a callee-saved one (SetManager::MakeRenderedSet).
- **For a list walk that removes by value, keep the `next` local** (`id = Get( pos ); next = GetNextPos( pos ); ...
  pos = next;`): it threads the loop's NULL test into Get's test (TacticalControlWindow::PostFixUp).

- **The frame-time clamp** is `float delta = time - last; if ( delta < MIN ) delta = MIN; elapsed = delta;`, as
  ShipClass::Update writes it (`fcom` plus `fld [pool]`); the `?:` form and a clamp on the member don't
  (PhysicsObjectClass::Update; Torpedo::Update and SetClass::Update are candidates). A unit with another 0.001f use can
  hit retail's second copy at 0x894A1C: that copy is a division (`/ 1000.0f`, x87.md), so the clamp and
  Blink coexist (CharacterClass::Update).
- **A slot-order wall where one variable is reassigned in a nested block can be that block's own same-named local,**
  which splits the reference count the packer sorts by (walls.md, "How VC6 SP5 orders frame slots";
  NamedReticleWindow::Unknown5103F0 *(open)*).

- **A temporary bound to a const-reference argument shares its slot with earlier block locals; a named object with a
  destructor doesn't:** retail's smaller frame points to the temporary (OptimizedAvoidObstacles::Unknown480250
  *(open)*).

- **One of our locals pairing 50/50 with two retail slots (framemap) is two variables** (two HKEYs:
  GetRegistryInstallPath); two block-scoped locals sharing a slot where retail keeps two are overlapping
  function-scope declarations.
- **A register zeroed at entry and moved to eax at a shared NULL return** is a result local initialised NULL with one
  return (ShipClass_GetSubsystems, as ArtificialIntelligence_GetAllAIsInTree).

- **A search written into the result variable** (`name = &s[i]; break;`) gives retail's one register; a helper that
  returns the pointer splits it across two (TGHandler::CallPython).
- **A cached member address used for the first field only, the other fields read from `this`,** is separate setters
  (`SetTarget( id ); SetTargetOffset( offset )`), not one struct overload (PulseWeapon::Fire).
- **A 12-byte slot retail has beyond ours is a named NiPoint3 local** where a sibling names one (a temporary with no
  unwind state of its own is a NiPoint3, not a TGPoint3: class-model.md): the cross product into `NiPoint3 right` before
  `right.UnitCross( *forward )`, as TorpCameraMode::GetIdealDirection writes it (ChaseCameraMode::GetIdealDirection).

- **Two named NiPoint3 edge vectors whose `SqrLength`s retail tests only after both are built** are single-use float
  squared lengths of temporaries (TestTriangle). A chained `b[i] = a[i] = T( u, v )` stores the FPU value into both
  and copies the field from the first (StarSphere::VirtualSlot71).

- **A per-row address `(T *)bits + y * pitch`** loads the row after the zero-trip test and frees the counter's slot; a
  pointer advanced with `row += pitch` doesn't (NiRawImageData::Unknown7DF9A0).
- **A scratch NiColor/NiColorA filled member by member** gives integer-move copies where the constructor gives fld/fstp;
  one pair reused for two colours gives retail's shared slots (STNumericBar::UpdateGauge *(open)*).
- **A separate down-counter on a constant-3 loop** follows the arrays' base expression (pointer locals give it, but as
  single-use renames they are R17) (MetaVolumePolygonizer::Unknown4C2CC0, OBBNode::DivideTriangles *(open)*).

- **An output-pointer argument passed a live local:** check whether the callee writes it; a framemap 50/50 pairing
  plus retail passing the argument from a slot of its own means a separate output local
  (WeaponsDisplay::UpdateWeaponTargeting with PulseWeapon::CanHitTarget's direction: a meaning fix).
- **`X->GetRight()` for `X->GetWidth() + X->GetLeft()`** moved an fadd next to its fld (STButton::Layout *(open)*);
  decided per function (GetBottom made STMissionLog worse).

- **EH state -1 stored before the next call** means the object was a temporary of the full expression
  (`x = T( arg )`), not a named local (AttachSurfaces' NiD3DPixelFormatEntry).
- **One of our loop counters spread over several retail slots (framemap)** was declared in each loop,
  `for ( int row ... )` in an inner block (NiBezierTriangle3::Unknown83BFB0).

### Locals and evaluation order: elements, pointers and references

- **Two arrays written with one index compile to one pointer plus a constant difference** (`mov [esi+ecx], ...;
  mov [ecx], ...; add ecx, 4`, ESI holding the arrays' distance). Where retail shows that shape, spell it `a[i] = ...;
  b[i] = ...; i++`, not two `*p++` walks (LensFlare::Build *(open)*).

- **Element and pointer locals.** A per-loop element local ( `entry = entries[i];`) gives one load where
  `entries[i]->` twice reloads it; keep a loaded element in a local ( `ShipClass *ship = ships[j];`) when retail
  returns the register it just loaded (MultiplayerGame::GetShipFromPlayerID). Retail reloading an NiTArray's base
  before its size after an inline SetAt is the compared element named first, `Backdrop *current = backdrops.GetAt( i
  ); if ( current == this )` (Backdrop::MoveForward, MoveBack). `NiBound *bound = &worldBound;` declared after the
  bool flag local, with the child walk merging through it, gives retail's registers in world-bound walks
  (NiNode::UpdateDownwardPass, NiNode::UpdateWorldBound; NiBone::UpdateDownwardPass). A pointer-array
  element passed to a call with `&count` is named first ( `SetClass *set = sets[i];`,
  MultiplayerGame::ReceiveClientReadyMessage). A member assigned from `new` and then called through is a local first (
  `TGPane *bars = new TGPane( ... ); unknown88 = bars; bars->AddChild( ... );`): retail keeps the pointer in a
  register (PlayerReticleWindow::PlayerReticleWindow *(open)*).
- **A reference local to an array element (`T &e = arr[i]`) blocks retail's hoisting of a common load;** index at each
  use (ProximityAxis::UpdateEntry).
- **Members reached through reference and pointer locals** ( `NiPoint3 &boxCenter = center; NiPoint3 *boxAxis = axis;`)
  give retail's `lea edi, [ecx+4]; lea esi, [ecx+0x10]` before a call: Gamebryo's `NiPoint3 &center = box.center`
  idiom carried into member code (OBBNode::ComputeOBB). {asm: lea edi; lea esi}
- **A loop over a static array through a global pointer** strength-reduces only with local copies of the pointer and
  the count (Backdrop::DeleteAll); an index-dependent loop inside an inline blocks strength reduction of the caller's
  loops (NiSkinController).
- ** `*normals++ = normal` in an `unsigned short` loop** steps by pointer as retail does; `normals[i]` indexes. A
  copy-out written `result[i++] = RemoveHead()` puts the pointer copy after the loop's entry test, where `*out++`
  puts it before (SetClass::GetTargetableObjects).
- **`xor reg, reg; mov reg16, [...]`** is an `int` local loaded from an `unsigned short` array ( `int triangle =
  order[i];`); `mov reg16; and 0xFFFF` is the element used as it is. {asm: xor; mov word}
- **A dword bit-mask local with byte ORs** ( `or dword [esp], 4`) is an `int` returned as `unsigned char`
  (GetCornerBits).
- **A raw 16-bit compare after one 16-bit load** ( `cmp cx, 0x800`) is several flag accessors CSE'd into one load:
  `IsButtonEvent()`, `GetButtonNum() == MEF_BUTTON_LEFT`, `GetFlags() & MEF_BUTTON_DOWN` (Editor and TGUIObject
  MouseHandler), not a local copy of the flags.
- **A lookup written out inside a destructor** lets `SortedArray::Find` expand where an inline helper's extra level
  keeps it out of line (~CharacterAction).
- **A byte walk whose pointer starts at `+2` and reads `[p-2]`, `[p-4]`, `[p-3]` around its `add p, 3`** is
  `bytes[i * 3]`, `bytes[i * 3 + 1]`, `bytes[i * 3 + 2]` indexed by the loop counter, not a `bytes += 3` walk
  (BridgeObjectClass::ApplyGamma, PrepareGamma).
- **Two arrays walked in one loop that retail keeps as two pointers** (ours merges them into one index, `sub ecx,
  esi` then `[ecx + esi]`) were written with a pointer of their own for one of them, `refs++` in the loop's increment
  (BinaryVoxelizer::BuildData *(open)*).
- **An inline `strcmp` reads its first argument through eax and the second through esi:** where retail reads a
  literal through eax the original wrote the literal first, `strcmp( ".py", &name[length - 3] )`
  (PyEmbed::SaveModules *(open)*).
- **A `unsigned short` count read as `xor ebx, ebx; mov bx, [..]`** (not `mov bx; and ebx, 0xFFFF`) is an `int`
  local, as the matched twin writes it (BridgeObjectClass::PrepareGamma, as ApplyGamma).

### Locals and evaluation order: vectors and copies

- **Copy construction versus member-wise assignment:** `TGPoint3 pt = *this;` (copy constructor) and
  `TGPoint3 pt; pt.x = x; pt.y = y; pt.z = z;` compile differently; the second closed TGPoint3::MultMatrix, and
  `TGPoint3 pt( x, y, z )` matches neither. When retail copies a vector or color field by field, try it. A
  NiPoint3 built from read floats with integer `mov`s is `p.x = x; p.y = y; p.z = z;`, where the constructor gives
  fld/fstp (MultiplayerGame::ReceiveDamageVolumeMessage). A bare `NiPoint3 axis;` with member stores gives retail's
  frame where the constructor-argument form allocates a larger one (ObjectClass::RandomOrientation);
  `member = NiColorA( 0, 0, 0, 0 )` orders the member stores before the next load where four float stores don't
  (TopWindow::FadeIn).
- **Float pairs copied with interleaved `mov eax, [a]; mov [b], eax; mov ecx, [a+4]; mov [b+4], ecx` are one
  NiPoint2 (or struct) member copied whole** (TGInputManager's cursor and move points). Two adjacent float
  out-parameters kept away from a dead slot are an NiPoint2 (Editor::ShowObjectName). Three ReadFloat results side by
  side in a frame one NiPoint3 larger were read into a NiPoint3 local (CameraObjectClass::ReadFromStream); one matrix
  frame slot reused for two rotations is one reused local, a frame 36 bytes larger two matrices.
- **Retail writing a point through a register holding `&member`, right after passing `&member` to a call,** is an
  inline `GetPosition( NiPoint2 * )`-style helper (Editor::MouseHandler). Spilled float locals followed by a zeroed
  `NiRect` local point to an inline out-parameter getter (ViewScreenNode::GetScreenRect in
  ViewScreenObject::Unknown678A80, MATCH-PENDING on its written-out GetRemoteCam).
- **Evaluation order of three calls in one statement.** `NiPoint3( f(), f(), f() )` with an inline float `f`
  evaluates y, x, z: retail's random axes (y from the first rand) are `NiPoint3( NiSymmetricRandom(),
  NiSymmetricRandom(), NiSymmetricRandom() )` (Gamebryo's helper, NiMath.h); open-coded `rand()` expressions give z
  first (SpinController::CreateClone, SunEffect::Create). `x.SetXYZ( r(), r(), r() )` gives three stack spills and the
  last value stored straight from the FPU (right-to-left argument evaluation).
- **Chained assignments store right to left:** `a = b = v` stores `b` first (`fst b; fstp a`), so write the chain in
  retail's store order (TGIconPoly::UpdateVertices *(open)*); `points[0] = points[1] = x;` stores the second element
  first (CameraObjectClass::SweepTo), and `a.x = a.y = a.z = 0.0f` stores z, y, x where the NiPoint3 constructor or
  separate member stores give x, y, z (CreateFan). What a chain does to the operand census is x87.md's "The order of
  two memory operands".
- **Wall: reading a NiPoint3 component-wise through the pointer `operator*` returns:** no named copy, reference or
  helper reproduced it (ShipClass::VirtualSlot80 *(open)*).

### Locals and evaluation order: temporaries and returned values

- **Temporaries.** A temporary destroyed right after its last use was unnamed: a TGString freed before the next
  statement is a full-expression temporary ( `GetName().Unknown6F56F0()`), not a named local
  (STSubsystemMenu::TargetButtonClicked *(open)*). Implicit conversion versus an explicit temporary:
  `*reason + fileName` matched where `+ TGString( fileName )` gave other temporary slots
  (NetFile::ReceiveChecksumFailure). `TGString( a ) + TGString( b )` builds b first; retail building the left operand
  first is `charPtr + TGString( b )`, the conversion inside operator+ (STSaveDialog, STLoadDialog paths). An implicit
  `TGColorA = NiColorA` conversion copies the converted temporary from its stack slot; an explicit `TGColorA( ... )`
  copies through the constructor's returned pointer (ShipClass::ShowShieldHit). A smart-pointer temporary released
  right after a raw pointer is taken from it is a named raw local ( `NiRender *renderer = UtopiaApp::GetRenderer();`,
  TGMovieManager::LoadForcedMovie). `const T &` stream save parameters: VC6 binds `true`, `false` and accessor results
  to `NiStreamSaveBinary( stream, const T & )`, and those temporaries share one frame slot as in retail's SaveBinary
  copies. Full-expression temporaries share frame slots: `f( NiPoint3( MODEL_FORWARD ), NiPoint3( MODEL_UP ) )` closed
  WarpSet's constructor frame.
- ** `a + b` with a member operator** evaluates `a` first as a call argument and `b` first as a named local's
  initializer (PlacementEditor::MoveCameraToObject); a sum temporary sharing frame slots with the others is VC6's `&(
  a + b )` (C4238, silent at /W3; PlacementEditorMode::Update), never a bare block. *Evidence required:* C4238 is a
  VC6 extension; use it only where retail's frame shares the sum's slot with the other temporaries and no standard
  spelling gives it.
- **Retail's EH states show where a temporary dies:** split the statement there (ChaseCameraMode::GetIdealDirection).
- **Destructor order shows the scope of smart-pointer locals.** When retail releases a group of `NiPointer` locals
  before a later statement, the original had a `{ }` block there, and the block reproduces it
  (MetaNebula::CreateExterior, Nebula::CreateOverlay, both *(open)*).
- **A vector sum retail keeps on the FPU is a named intermediate** ( `NiPoint3 lookAt = a + b; forward = lookAt -
  r;`, not `a + b - r`); the tell is a TGPoint3 temporary whose EH state ends after the sum
  (PlacementWatchMode::Update *(open)*).
- **Returned values: through the returned pointer or a named local.** Assignment through a returned reference
  ( `controllers.RemoveAt( i ) = 0;`) reproduces retail's use of the returned pointer, where a named local does not. A
  function result stored into a member goes through the returned pointer; a named local first (
  `NiPoint3 position = ...; pos[i].pos = position;`) gives retail's direct frame reads (NiAnimBlender::SamplePose). A
  returned NiMatrix3 passed on by address needs a named local (`NiMatrix3 turn = F(); Rotate( turn );`): passing the
  call directly reuses the returned pointer and costs a register. A named local copy passed by reference
  ( `NiMatrix3 rotate = node->rotate; SetRotate( rotate );`) passes `lea` of the local, where an explicit temporary
  `NiMatrix3( x )` passes the constructor's returned EAX (TGAnimationEntry 0x6CC400). An out-of-line constructor whose
  return value is pushed straight into the next call is a temporary built in the argument ( `ApplyForce( &Force( ...
  ) )`), where a named local gives `lea` of the local (ShipClass::ApplyWeaponHit).
