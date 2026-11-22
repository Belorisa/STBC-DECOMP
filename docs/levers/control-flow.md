# Control flow

Branch order, early returns, shared tails, loops and switches. A wall reported as `control-flow` starts here; the
order to check things in is [triage.md](triage.md). Read `README.md` for how entries are written.

## Control flow

### Control flow: loops

- **A loop retail leaves unrotated (test at the top, jump back, one call site) is `while ( true ) { if ( !cond )
  break; ... }`;** `for ( ;; )` with the same body doesn't give it (FindIniValue, SetLocalAddress,
  RegisterUnicodeKey, UnloadOtherModules, ModelMerge_Merge's group search). A walk laid out `test; je exit; ... jmp
  top` is `if ( p ) { while ( true ) { ...; p = p->next; if ( p == NULL ) break; } }` (OBBModel's extra-data walks,
  BuilderAI::DeleteBlocks's root walk, TractorBeamGlow::VirtualSlot35 and PhaserGlow::VirtualSlot35's pick walks). A
  fetch loop with a `jmp` into the call is `while ( true ) { x = Get(); if ( x == NULL ) break; ... }`, not
  `while ( ( x = Get() ) != NULL )` (Episode::OnIdle). The reverse: retail's rotated loop (the test at the bottom,
  exits threaded) is `for ( ;; )` where `while ( true )` leaves it at the top (STSubPane::MoveSelection *(open:
  registers)*). Flip the two first when a loop's layout differs, but expect little: the flip alone rarely closes a
  loop. Didn't help in: TGBroadcastHandlerList::FindFirst (VC6 rotates `while`, `while ( true )` and `for ( ;; )` alike
  there), TGMovieAction::PlayFrameActions (its wall was the count/items locals, below). {asm: test; je; jmp top}
- **An empty-bodied `while` that retail tests at the bottom** is `do { end--; } while ( text[end] == ' ' );`
  (TGConfigMapping).
- **Reverse loops and countdowns.** A loop guarded by `dec reg; js` is `while ( --count >= 0 )` or
  `for ( count--; count >= 0; count-- )`, where `for ( i = count - 1; i >= 0; i-- )` gives `lea; cmp; jl`; the two
  also spend the inline budget differently (which NiPointer member destructor stays out of line in ~SetClass). Seen as
  `while ( --count >= 0 )` in Mission::AddPrecreatedShip, and for a memory-resident index after
  `GetClassObjects( set, &count )`. Yet a countdown `dec eax; js; inc eax` is `for ( int i = count - 1; i >= 0; i-- )`
  (CollisionEvent::ReadFromStream), and stream count loops are `for ( i = stream->ReadInt() - 1; i >= 0; i-- )`
  (TGMusic::ReadFromStream): try both forms. {asm: dec; js}
- **Loop shapes.** A retry flag with `break` (AbortAllActions, SkipActionsFor); a loop written
  `do { i++; ... } while ( i < count );` after a `count < 1` return (AllocateSamples). A loop whose `result` test sits
  after the increment is `for ( i = 0; result && i < count; i++ )` (Backdrop::SaveAll). A dead-end search loop jumping
  straight to the epilogue is the body inside the loop with `return;`; `break` plus a bound test adds a `cmp`
  (MultiplayerGame::DeletePlayerHandler *(open)*). A countdown with no entry test is `do { ...; p++; } while ( --count
  );` over the array base (MetaVolumePool::GetVolumesInBox); the `for` third clause sets the increment order
  ( `index++, cellX++`). A long `jle` past the whole loop means the loop sits inside the `if` guarding its setup
  ( `if ( count > 0 ) { SetSize( count ); for ... }`); a short jump to the loop test means it follows the `if`
  (SubtitleWindow::ReadFromStream).
- **A per-cell index (`row * columns + column`) instead of a running counter:** a loop test against 9 around an inner
  count-3 loop is `row * 3 + column` indexing, not a running `index++` (NiBezierRectangle3::Unknown84ACF0); a
  vertex index computed per cell gives retail's per-array pointer walks and frame, and a named `sinTheta` then one
  `NiPoint3( ... )` assignment gives a normal's store order (SunEffect::CreateSphere *(open)*).
- **A search loop whose 100th miss goes straight to the fallback print** (no retest of the counter after the loop)
  leaves with `goto noLine;`, both prints written out (MSJExceptionHandler::ImagehlpStackWalk *(open: registers)*).
- **A `bool` flag that skips the fallback after a `break`** jump-threads to retail's direct jump, where a `goto`
  scores lower (KeyboardBinding::BindKey *(open)*).
- **An early-exit search loop with one epilogue** is `bool hit = false; ... hit = Test(); if ( hit ) return hit; ...
  return hit;`; `return true` / `return false` gave two (DamageCollisionTest::TestVertices).
- **VC6 /O2 turns a self-recursive tail call into a loop by itself:** write the lineage recursion as it is
  (NiTriShape::IndexQSort). Retail's recursion through a result variable returned once doesn't become a loop
  (CountVertices).

- **An array walk retail strength-reduces (a pointer stepped by the element size, the count run down with `dec;
  jne`) is indexed with an `int`;** an inline accessor taking an `unsigned short` index truncates it (`and eax, 0xffff`)
  and blocks the reduction, costing a frame slot and a branch (NiBinaryVoxelData::CreateSolidLines: MATCH, `&slice->spans[run]`
  as its twin NiBinaryVoxelData::CreateSurfaceLines walks *(open)*). {asm: add reg, 0xc; dec; jne}

- **A search loop with one call at its head and `return true` laid out before `return false`** is `while ( true ) { x =
  Get(); if ( !x ) break; Use( x ); if ( Done() ) return true; } return false;`; `do { } while ( !Done() )` rotates the
  loop and duplicates an inlined callee's null test, `for ( ;; )` is worse (NiD3DTextureManager::Unknown7D5910).
- **`pre-test jne exit; ...; bottom je top; pop ... ret` is a `while` loop;** a guard `if ( x < y ) return;` then
  `do {} while` gives `jne exit; jmp top` instead (TGPhonemeSequence::Update).

### Control flow: list and array walks

- **NiTList walks.** A walk that advances at the bottom is `for ( pos = GetHeadPos(); pos; pos = GetNextPos( pos ) )`;
  a `while` with GetNext advances first (CollisionRecorder::GetPoints). A `while ( pos )` loop that keeps GetNextPos's
  null test steps with `list.GetNext( pos );`, its result discarded (TGIconManager::Purge). `pos = list.GetNextPos(
  pos )` + `return list.GetNext( pos )` when retail nulls the position on running off the end (TGPane::GetNthChild);
  NiTList has GetTailPos/GetTail/GetPrev/GetNextPos/GetPrevPos/Get/InsertBefore. The inline NiTList::GetNext carries
  its own NULL test: retail's `if ( iterator == NULL ) return NULL;` before the pop is just `return list.GetNext(
  iterator );`, and BC's NiTList::Get tests its position too (`pos ? element : T()`, unlike Gamebryo 1.1.2's): a walk
  to the n-th element is `list.Get( pos )` re-read after each `GetNextPos` (WeaponSystem::GetFiringChain). A walk
  whose element and next position retail reads under one null test, element first, zeroing both on NULL even inside
  `while ( pos )`, is `element = list.Get( pos ); next = list.GetNextPos( pos );` (FocusManager::Tab,
  ShipSubsystems' slot 3); GetNextPos first, or the caller's own `pos ? Get( pos ) : 0`, loads next first or splits the
  test. `NiTList<NiPointer<T>>::GetNext` returns into a named local
  ( `NiPointer<T> light = lights.GetNext( pos );`), not a temporary used in place (BridgeObjectClass::AlertLightsOn/Off).
- **Removing while walking a list:** with `GetNext`,
  `if ( pos == NULL ) { delete GetTail(); RemoveTail(); } else { delete RemovePos( GetPrevPos( pos ) ); }`
  (WeaponSystem::UpdateTargetList); with positions, `element = Get( pos ); next = GetNextPos( pos ); ... RemovePos(
  pos ); pos = next;` (VisibleDamagePiece::Unknown4B3D70, FocusManager::AddObjectToTabOrder). Per-element ID-to-pointer fix-up is `list.AddHead( (T
  *)GetTGObjectPtr( (long)saved ) ); list.Remove( saved );` over a `GetNext` walk (AsteroidField::PostFixUp).
- **A search with one shared NULL test after it** is `while ( p != NULL && p->key != value ) { p = p->next; }
  if ( p != NULL ) { ... }` (TGEventQueue::ClearSource).
- **SortedArray:** a search re-reading the element's ID each step is `Find( T *item )`, not a key lookup
  (SetClass::RemoveCameraFromSet, DeleteLightFromSet); a delete or remove by key whose `items` check comes after the
  search, with no index test, is `DeleteAt( Find( key ) )` / `RemoveAt( Find( key ) )`, retail's own bug on a missing
  key (TGPlayerList, TGModelManager::FreeModel).
- **Dead writes into a GetAt out-parameter vanish:** `if ( map.GetAt( key, v ) ) return true;` per map
  (LODModelManager::Contains).
- **`if ( !GetAt( k, v ) ) { v = new X; SetAt( k, v ); } return v;`** rather than an early return gives retail's
  register choice (TGConfigMapping::Unknown71DA20); `NiTMap::GetAt` then `SetAt` gives retail's "return if the key
  exists, else make" shape (Backdrop::SetName).
- **A shared tail store means the original assigned from the register it just wrote:**
  `head = node->next; tail = head;` (TGLinkedList::RemoveAt).
- **The null-pair test in IsEqual walks:** `( a && !b ) || ( !a && b )`, then `a && !a->IsEqual( b )`, not an
  if/else-if (NiAVObject::IsEqual, NiCamera::IsEqual).
- **A destructor running `RemoveAll` twice** has an explicit `member.RemoveAll();` as its last statement before the
  member's own destructor (EngPowerCtrl).
- **One `cmp size, i; jbe skip` before NiTArray::GetAt in a child walk** is NiNode::GetAt's own bounds test, with no
  outer `if ( i < GetArrayCount() )`; VC6 threads its NULL result into the `child != NULL` test
  (SetClass::WriteToStream *(open)*).
- **Two AddTail copies around a sorted insert** are a head test (AddTail on an empty list) and a loop that breaks after
  InsertBefore or after an AddTail at the end of the walk; one AddTail at the loop top gives another call set
  (SetClass::Unknown413CB0's three NewItem sites: call set retail's, frame and registers *(open)*).

- **Count and items loaded before each loop test** are a count/items local pair fetched before the loop and again at
  its end, when the unit walks its arrays that way elsewhere (TGMovieAction::PlayFrameActions, after WriteToStream).

### Control flow: branch order and layout

- **Layout follows the written order.** Retail laying the positive body out as the fall-through wants
  `if ( x ) { ... } else { ... }` written that way round (NiAnimBlender::GatherKeys); retail laying out the
  "not X" path first wants `if ( !x ) { ... } else { ... }` (SetClass::SetBackgroundModel); writing the `!= NULL`
  branch first fixed EnableGlossMaps' block order. Negate the condition so the body retail lays out first comes first
  (failure body first: UtopiaModule::ProcessInternalLoad, with the alert switch as an if chain *(open)*). A branch laid
  out last comes from an inverted if/else (PlacementObject::SetStatic).
- **Which test comes first decides the epilogues.** A lookup with separate "found" and "not found" epilogues is
  written with the missing-entry test first: `if ( entry == NULL ) return 0; return entry->...;` (the six
  TGAnimationManagerClass lookups). A type-mismatch guard return ( `if ( value->type != X ) return 0;`) keeps retail's
  separate epilogues where a positive `==` test merges them (TGConfigMapping getters).
  `if ( x != NULL ) return A; return NULL;` versus `if ( x == NULL ) return NULL; return A;` decides whether retail
  returns the already-zero register (DamageDisplay::GetShip). A guarded forward is the positive
  `if ( p != NULL ) return p->F(); return false;` (TGTimerManager wrappers), and a guarded return the positive
  `if ( index >= 0 ) return GetAt( index ); return NULL;` (SetManager::GetSet).
- **Two guard tests with one default return laid first** (the default as the fall-through, the result at the jump
  target) are two separate `if ( a == NULL ) return ZERO; if ( b == 0 ) return ZERO;`; one `||` guard or the
  positive `&&` lays the result first (ShipClass::GetTargetAngularVelocity).
- **Separate early returns, not a nested if/else:** independent `if ( a == NULL && b != NULL ) return ...;` tests
  re-test the member the way retail does (OBBNode::FindRoughOBBIntersections).
- **An early return after a handled event, where retail falls into the second test, is a plain `if`;** a shared
  CallNextHandler after a `jmp` is an if/else (STNumericBar::HandleKeyboard, STLoadDialog::HandleDeleteButton).
- **A sign test with three outcomes** in retail's `jg`/ `jne` order is nested:
  `if ( p <= 0 ) { if ( p == 0 ) ... else ... } else ...` (CharacterClass::PlayAnimation). A key compare ending
  `je; jl` is `if ( key == x ) return 0; if ( key < x ) return -1; return 1;`; `key > x` gives `jle`
  (TGMovieAction::DeleteFrameAction).
- **Binary search and bisection.** `if ( id <= target ) low = mid + 1; else high = mid;` gives retail's `jg` layout
  (TGBroadcastHandlerList::FindInsertIndex). A bisection whose midpoint is updated in each branch
  ( `low = t; t = ( high + t ) * 0.5f;`) reproduces retail's separate tails; one shared midpoint after the branches
  doesn't (ShipClass::TurnTowardDifference).
- **A test VC6 repeats where it could thread** (`cmp esi, ebx; jne` on a value already tested) is a second
  condition with its operands the other way round, read in place rather than through locals
  (SortedRegionMenu::Compare: `other->missionName != NULL && missionName == NULL`). {asm: cmp esi, ebx; jne}
- **A match that skips the later "found" test** comes from that case breaking out itself
  (`holder = focus; pos = next; break;`), not from a shared `if ( holder != NULL )` (FocusManager::Tab).
- **Nested positive conditions sharing one false tail** closed the NiSwitchNode/NiLODNode collision tests. One
  `( a && b ) || ( c && d )` condition gives retail's shared-body jump layout (TGPrompt::KeyboardHandler). Two guarded
  cases sharing one call ( `a < 0` with type 0, `a > 0` with type 1, then the same send) are one
  `if ( ( a < 0 && type == 0 ) || ( a > 0 && type == 1 ) )`: retail's two tests jump to a shared tail; two `if/else`
  arms give the same instructions in another block order (ProximityCheck::CheckProximity).
- **A state compared several times is read through its accessor at each test:** `GetField( MASK, POS ) == A ||
  GetField( MASK, POS ) == B` compiles to a word read with the unshifted mask (`and ax, 0x30; cmp ax, 0x20`) and keeps a
  redundant later compare; a local holding the state shifts and folds it (ExplodeParticleController::Update).
  {asm: and ax; cmp ax}
- **`( f & A ) || ( f & B )` loads the flags into a register and tests the combined mask; `f & ( A | B )` tests
  memory** (a trace macro's option tests, TGTrace::Output).
- **A bool flag assigned from a comparison** (`bool merge = f() == 0; if ( !merge ) ... if ( merge )`) gives
  retail's `sete al; test al, al; jne`, where a plain `if ( f() != 0 )` branches directly
  (NiTriShape::CollapseVertices: MATCH, with the debug copy's loop test folded into the `for`, lineage.md).
  {asm: sete al; test al, al; jne}
- **Polygon clipping's "store the corner, then jump back" layout** comes only when the corner-inside case is split on
  the previous corner, with the corner store repeated in both branches; comparing straight in the ifs, not through
  bool locals, keeps the x87 status word in AH (BinaryVoxelizer::Step *(open)*).
- **A control-flow wall can come from the unit, not the spelling:** TGModelPropertySet's loader constructor body
  compiled in AppWrap (0x5DEA10) matches with retail's duplicated epilogue, while the same body in its own unit
  (0x6EB040) does not *(open)*.

### Control flow: returns, tails and merged code

- **A switch case that jumps to the default's return block** is `case X: default: return;`; separate returns give
  separate destructor blocks and change the inline sites (BridgeObjectClass::DoCrewReactions *(open)*).
- **A list walk where retail reads the item's `next` after the body** is `Get( pos )` with `pos = GetNextPos( pos )`
  at the loop's end, which also frees GetNext's inline cost (PyEmbed::RunGetStateFunctions, RunSetStateFunctions);
  where retail keeps GetNext's null test, GetNext stays (OptimizedFireScript::ChooseListedSubsystem *(open)*).
- **A countdown loop with no reload of a bound the stores could alias** read the bound into a local first:
  `int remaining = solidCount % 32;` (BinaryVoxelizer::SliceData::GetSolidRowOffsets, GetSurfaceRowOffsets).
- **A search loop with `xor eax,eax` before the loop test and its own NULL return at the exit** is
  `ObjectClass *result = NULL; ... result = object; break; ... return result;` (Editor::FindPrev/NextSelectable).
- **Early returns versus a single exit:** retail's separate `xor al,al` blocks are early returns; one shared return is
  a flag variable (TGScriptAction::Create needed the early returns written out). A single-exit result variable (
  `int index = -1; if ( cond ) { ...; index = n++; } return index;`, or a `done` flag) reproduces retail's early
  `or eax,-1` / `xor al,al` value (FuzzyLogic). `damage *= time; return damage;` keeps one join where
  `return damage * time;` duplicates the return.
- **One shared failure return at the end is positive nesting:** the success path nested under a positive test, with
  one `return false;` / `return NULL;` after it, gives retail's single failure tail or common epilogue where early
  returns duplicate it. `if ( A() && B() ) { ...; return true; } return false;` (TGMusicFadeEvent::Copy);
  `if ( IsTypeOf( ... ) ) { copy; return Base::Copy( ... ); } return false;` (WeaponHitEvent::Copy);
  `if ( test ) { ...; return a || b; } return false;` (OBBNode slots 1 and 2); `if ( lod != NULL ) { ... } return
  false;` sharing the early-out's `return false` (DamageableObject::Unknown592960); `if ( x != NULL ) { ...; return y;
  } return NULL;` with retail's `je` to the end (BridgeObjectClass::Create); `if ( x != NULL ) { ... } return x;`
  returning NULL through the common epilogue (SetClass::RemoveObjectFromSet); nested `if`s with `return x;` inside and
  one `return NULL;` giving a shared `xor eax, eax` tail, where early returns duplicate the destructor tail
  (TGFrame::AddPoly *(open: registers)*); `if ( file ) { ...; } return true;`, the second success path falling through to the shared
  return (TGModelManager's incremental load step).
- **One shared `delete; return NULL` at the end is a `goto fail`, with the success part in a block;** section locals
  left unbraced live to the end, and retail's end-of-function NiPointer releases and frame size show which sections
  had no block (GlowRenderer::Create *(open)*).
- **A cleared pointer returned through a register** (`xor esi, esi; mov eax, esi`) is if/else arms (success /
  `delete p; p = NULL;`) joining one `return p;`; returning in each arm folds it to `xor eax, eax`
  (TGLocalizationDatabase::Create). Likewise a NULL result kept live in a register at the failure exit is one
  result variable assigned in the success path, declared before the other locals (its zeroing follows the
  declaration order) (OptimizedFireScript_GetWeapons). {asm: xor esi, esi; mov eax, esi}
- **A NiPointer member tested `== 0` (through `operator==`) gets its own `xor eax,eax` return;** `!raw` reuses the
  tested register (NiImage::CopyRawImageData).
- **A member pushed from the register it was just tested in** ( `push eax` of `directInput`, where this build pushes
  an immediate 0) is a `return member;` inside the `if` as well as after it (TGSystemWrapperClass::GetDirectInput).
- **Retail pushing every callee-saved register before a parameter test** (where this build shrink-wraps the pushes
  past it) is a guard written as an early return, `if ( goal == NULL ) { return; }`, not the body nested in
  `if ( goal != NULL ) { ... }`; the same for a later `if ( menu == NULL ) { ...; return; }` (Episode::RegisterGoal
  and RemoveGoal).
- **An early return on NULL in an inline gives retail's jump threading past the argument's release; `if ( p ) {...}`
  doesn't** (NiPropertyList::SetProperty 0x4E1160; a lead for the parked AddHead).
- **A lookup's result split into two fields with an `else` that NULLs them** (`if ( node != NULL ) { a = node->x;
  b = node->y; } else { a = NULL; b = NULL; }`) lets VC6 thread the inlined lookup's NULL returns straight into the
  `else` arm and lay the found path out of line; `= NULL` initialisers followed by `if ( node != NULL )` keep a join and
  a re-test of the node (TGAnimationEntry::Unknown6CBD80, with SortedArray::Get's early return). Spellings that failed
  before a shared inline changed are worth one more trial after it.
- **A membership test that returns straight to the function's end when found** (`cmp element, object` and the found
  branch jumping to the epilogue) is the walk written out with an early return, `pos = list.GetHeadPos(); while ( pos )
  { if ( list.GetNext( pos ) == object ) return object; }`; `if ( list.FindPos( object ) == NULL )` keeps FindPos's
  pos copy and re-test (TGObject::LoadObject, MATCH-PENDING on its invented FormatLoading helper).
- **`if ( test ) return c; return c;` keeps its call and test in retail:** label it `// BUG: redundant:`
  (TGSound::GetDurationSeconds).
- **Where VC6 merges identical code, and where it doesn't.** Identical statements at the end of two arms that flow
  into one join are sunk into the join; identical `...; return` blocks are cross-jumped back to their first
  difference; the same expression in both arms is tail-merged (one shared `div` with the divisor's register picked
  per arm comes from `x = ( x << 8 ) / span` written in each branch, FadeRedbookCDVolume). Retail keeping identical
  calls apart means the arms neither end in the same statements nor return the same way:
  - each direction's branch written whole, `if ( found ) A; else if ( c ) B;`, instead of `if ( found ) { A;
    return; }` per branch plus one shared `B` after the if/else (EngPowerCtrl::MoveSelection, whose two
    `SetFocus( child, true ); return;` tails this build merged until then);
  - two key tests as separate `if`s, not `else if`, which also lays the shared tail right after the first arm
    (TacWeaponsCtrl::KeyboardHandler);
  - each arm building and returning its own result, retail sharing only the last call (BuildDetailFilePath's
    strcats);
  - a nested call argument fetched into a local first ( `text = db->GetString( "Weapons" ); label->SetString(
    text, true );`): retail calls it before pushing the constant argument, and arms that differ only in the pushed
    literal then cross-jump (EngPowerCtrl::CreateBar);
  - failures as explicit `return false;` (an `else { return false; }` for the outer one) with success falling to
    the final `return true;`: the two false returns cross-jump, the success shares the last epilogue
    (NiD3DRender::Unknown7C45B0).
- **Duplicated tails are written out.** `if ( !x ) A(); else { loop }` followed by one shared call gives retail's
  prologue pushes and duplicated epilogue (STSubsystemMenu::BuildItems, STStylizedWindow::Scroll). Retail's duplicated
  epilogue after an `if` is an else branch with its own calls, not an early return. A tail call (CallNextHandler)
  duplicated per branch is duplicated in the source as early returns; VC6 doesn't duplicate it on its own
  (ShieldsDisplay::Update). Retail duplicating a whole else-tail ( `new`, SetDisabled, return per branch) is a return
  per path; a jump to a shared tail is an if/else (GraphicsMenu::GetColorDepthToggle against GetSizeToggle). Moving a
  shared tail's call into each case before `break` moves the `push ebp` save to retail's place
  (GraphicsMenu::HandleButtons). Several epilogues mean each case returns itself: `count++; return true;` per
  insertion case gave TGTimerList::AddTimer's four. A block that appears twice, once per branch, is written out
  twice: a `static inline` helper stays out of line (MultiplayerWindow::DisconnectHandler).
- **Two calls that retail merges into one call site** (pushes per arm, `jmp` to one call) are two calls in the
  source, one per arm; one call after the if lays the blocks out differently (PhysicsObjectClass::GetPredictedRotation). A
  call written per branch rather than a value selected into a local gives retail's per-branch pushes with one shared
  tail ( `icon->SetColor( ... )` per branch, WeaponsDisplay::Unknown54BFD0). A
  retail jump into a tail with a `mov ecx, ...` hoisted before the `jcc` is the same: a call written in both branches,
  nested ifs, and VC6 cross-jumps them (STTargetMenu::AddTarget).
- **A `je` from several tests to one shared failure block** in a Python-facing function is nested `if`/`else` with a
  `PyErr_Print` on each failure path: VC6 merges the identical tails (PyEmbed::IsInstance, IsSubclass).
- **A hand-written Python wrapper keeps its `bool created` flag in memory only with one return:** `if ( group !=
  NULL ) { ... } else { Py_INCREF( Py_None ); result = Py_None; } return result;`. An early return lets VC6 fold the
  flag (_wrap_ObjectGroup_ForceToGroup).
- **An explicit `if ( p != NULL ) delete p;`** where retail tests before the destructor call, though `delete` tests on
  its own (NetFile::SendNextFile *(open: an inline wall elsewhere)*); a `test`/ `je` before `delete [] p` is the same
  explicit test.
- **Two loads feeding one shared `delete` are separate `if ( p != NULL ) { delete p; p = NULL; }` blocks per branch,**
  tail-merged by VC6 (Waypoint::UpdateCurve).

- **Callee-saved pushes after a guard (retail's early exit with its own pop-free epilogue) need every guard of the
  body nested as a positive `if`:** nesting only the first test leaves the later `return;`s pulling the pushes to
  entry; `if ( network != NULL ) { ...; if ( object != NULL && ... ) { ...; if ( damageable == NULL || ... ) { ... } } }`
  matched (MultiplayerGame::ReceiveSendObjectMessage). {asm: test; je; push ebx}
- **A switch whose shared tail retail partly repeats** (each case storing its `new` result itself, one copy of the
  tail's first test shared by some cases and another for the case laid last) is the tail written out in each case
  before its `break`; VC6 cross-jumps the copies into retail's shape (NiBezierMesh::Unknown82E5C0 and 82F2B0).
- **A loop's `break` that retail sends straight to the next outer iteration** (no retest of the index after the inner
  search) is a `bool` set before the `break` and tested after the loop with `continue`; `if ( i < count ) continue;`
  retests (ShipClass::InSystemWarp's ignored-type search; the function *(open)*).
- **`not cl; test cl, 1; je` on a flag bit** is an inline bool returning `( mask & flags ) == 0` tested at the use
  ( `IsFlagClear( INDEPENDENT_ADDRESSING )`); `!( flags & BIT )`, `!Can()` and De Morgan forms in the condition fold
  to a plain `test; jne` (NiD3DTexturePipeline::Unknown7CD1A0). {asm: not cl; test cl, 1}

- **Callee-saved push placement follows the tail's control flow:** an early `return` inside the block that alone uses
  a register makes VC6 save it at entry; the block nested under its guarding test (`if ( block != NULL ) { memcpy }`)
  gives retail's late save (TGPoolManager::Unknown7179C0).

- **Which copy VC6 keeps when it merges identical tails:** of two copies that both end in an explicit mid-function
  `return`, the earlier becomes a jump into the later; a copy that only falls through to the function's end (an `else`,
  the last statement) is merged into the one that ends in `return` (OBBModel::AddShapes: the node walk as the `else`
  instead of `return;`; MetaVolumePolygonizer::Unknown4C2AF0: an early `return true` keeps the shared tail).
- **An `||` failure block moves to the function's end; two separate checks with their own identical failure blocks keep
  it inline after the second test** (MSJExceptionHandler::GenerateExceptionReport).
- **Statement order inside a case decides tail sharing:** `state = X; flag = -1;` keeps its own tail, `flag = -1;
  state = X;` or a fall-through into `state = X` merges with the plain case (`__strgtold12`).
- **A trailing call whose argument register differs by branch is written once per branch** (`strcat( p, separator )`
  in one arm, `strcat( p, path )` in the other: ImageManagerClass::FindDetailPath *(open)*).
- **Check a PARKED note against the current diff before working from it:** one described a wall that was gone (the
  real difference was a NULL test retail never makes: Torpedo::Unknown578800), another hid missing logic
  (NiNode::UpdateWorldBound lacked its `else` branch).

- **An early "no source or target" return that jumps into the final return's out-of-line NiPoint3 copy** is
  `if ( a && b ) { ... } return x;` with the middle returns kept (ZoomTargetMode::GetIdealPosition *(open: inline)*, as
  TargetCameraMode's).
- **Float tail calls (open):** retail does `call; ret` where ours tail-jumps on a float-returning call
  (TractorBeamSystem::GetNormalPowerWanted, PulseWeapon::GetPowerScaled); a `?:` double conversion doesn't block the jump.

- **A guard `if ( p == NULL ) { return x; }` after a statement that uses p** gives retail's callee-saved push at entry,
  where `if ( p != NULL ) { body }` shrink-wraps it (LightObjectClass::WriteToStream).
- **`a == NULL || !test( a )` with one fallback** is a nested if/else with the fallback call written in both else arms:
  VC6 cross-jumps them and hoists the shared `mov ecx` above both jumps (ShipSubsystem::GetDisplayName).

- **A cached accessor local lets VC6 thread a failed `x == 0 && y` test straight to the final else;** re-reading the
  inline accessor at each test keeps retail's fall-through into the next test (TGConsole::KeyboardHandler).

- **A search's NULL kept in its own local stops VC6 threading the NULL paths past the final test:** with `subsystem =
  NULL` written straight into the result, every miss jumps to the `xor; store` tail; a `sibling` local searched and
  then copied ( `subsystem = sibling;`) sends them through retail's shared `test esi; je` (OptimizedFireScript::
  ChooseTargetSubsystem, with the ID as `subsystem != NULL ? subsystem->GetObjID() : 0`).
- **An early return inside the body that uses the callee-saved registers moves their pushes to entry,** and a guard
  return before it then cross-jumps into the shared (EH) epilogue: `if ( device == NULL || device->modes == NULL )
  return menu;` instead of nesting the walk (GraphicsMenu::GetResolutionMenu *(open: frame)*). A guard written before
  the register users alone does not do it (Torpedo::Update's `if ( light == NULL ) { ...; return; }` still pushes late) *(open)*.
  The same holds for a guard on a value the body computes into a callee-saved register: `if ( power == NULL ) return
  PS_NORMAL;` where `if ( power != NULL ) { ... }` pushed edi late (OptimizedManagePower::CallPreprocessingMethod), and
  an `if ( !listed ) { ...; return; }` in place of an if/else also gives retail's frame slot order
  (NetFile::ReceiveChecksumRequest). Guards ahead of the users that each repeat a tail call (`CallNextHandler( event );
  return;`) push early but keep separate tails (MapWindow::GroupChangeHandler) *(open)*.
- **An `x ? a : b` source operand of an intrinsic `strcpy` evaluates after the destination's address in retail when the
  original wrote one `strcpy` per arm** (VC6 merges the two copies' tails): BridgeObjectClass::ShakeCrew.
- **Which copy VC6 keeps, in an if/else:** two `X; return a;` tails where the later one is reached by falling to the
  function's final `return a;` merge into the earlier explicit one: the sufficient-main case `if ( takes ) { ...;
  return amount; }` and the sufficient-backup case's `if ( takes ) { ...; }` falling to `return amount;`
  (PowerSubsystem::DeductPower; both explicit returns merged the other way round).
- **A loop's second exit test kept once** ( `test prev; je exit` inside, the main test at the bottom) is `while ( a ) {
  if ( p->previous == NULL ) break; p = p->previous; }`; `while ( a && p->previous != NULL )` and an `if`-guarded
  `do/while` both duplicate the second test (FloatRangeWatcher::AddRangeCheck).
- **Recursion or a call whose arguments a branch picks, each with its own `return true`,** is written once per arm
  ( `if ( d <= 0.0f ) { if ( Call( start, &x ) ) return true; } else { if ( Call( &x, end ) ) return true; }`): VC6
  cross-jumps the pushes and the return tails as retail does; assigning `start`/`end` first and one call after keeps
  separate returns (VoxelBSPNode::IsSegmentInside *(open: x87)*).
- **Switch case order is read from the jump table's targets, not the case values:** WndProc's `>= 0x105` table sends
  SYSKEYUP, SYSCHAR and COMMAND to descending addresses, so the source has COMMAND, SYSCHAR, SYSKEYUP
  (NiMessageLoop::WndProc *(open)*).
- **`x -= y` right after `y = 0.0f` compiles to a store of x to itself** ( `mov eax, [x]; mov [x], eax`): retail's
  self-copy is the original's subtraction after a zeroing, a `// BUG:` (PowerSubsystem::DeductPower).

- **The no-CD crack's signature:** `xor ecx, ecx; inc ecx` (or `xor eax, eax`, `nop nop`) where the original's `setne`
  or branch was; check for it before chasing a "constant true flag" (CDCheckWindow::RunCheck, CheckDisc,
  FindDiscWithContents: PATCHED).
- **A reversed compare** (`jbe`/`jge` where ours has `jae`/`jle`) is the compare written member-first
  (`searchLower[0] > x`: MetaVolumePolygonizer::UpdateVoxel *(open: x87)*).
- **A head-then-walk search** over a list with an embedded head is `if ( head->x != v ) { for ( ... next ... ) if ( ... )
  return true; return false; } return true;` (ObjectClass::IsLitBy).
- **A temporary destroyed before the branch** (EH state -1 before `je`) is a temporary inside the condition, not a
  named local (OptimizedFireScript::IsLineOfFireBlocked, MATCH-PENDING on its `&TGPoint3( point )`).

- **An early return on a NULL NiPointer member** (`if ( lines == NULL ) { return; }` in place of a wrapping `if`) lets VC6
  skip the smart pointer destructor's own null test at the exit (PhaserProperty::Unknown687350; 686C80 and 687A60,
  matched with it under /YX, are accepted losses of the no-PCH game build, PARKED).
- **A delete of a node whose item has a destructor keeps its null test when the branch tested the member, not the
  local:** `if ( next ) { second = next; ...; delete second; }` gives retail's `test ebx, ebx` before the item's
  release; `second = next; if ( second ) { ...; delete second; }` lets VC6 fold it (NiTSimpleList::Remove, 0x498250).
  {asm: inc dword ptr [eax + 4]; test ebx, ebx; je top}
- **One tail call after an if/else** instead of the call repeated before a `return` in one arm gives retail's scratch
  register rotation (PhysicsObjectClass::Update).
- **One exit, as the twin does:** a branch that sets the result flag and falls to the single `return hit;` fixes a
  register pick an early `return true;` breaks: a known-zero flag register stops doubling as the NULL constant
  (OBBNode::VirtualSlot03, as VirtualSlot05).

### Control flow: conditions, choices and bool results

- **`if/else` versus `?:`:** the two tail-merge differently. Retail's shared tail after two calls is
  `if ( !x ) A(); else B();`; a single store chosen by a condition is `?:`. A call argument chosen by a test, where
  retail branches to two push sequences sharing one call ( `je; push reg; jmp; push imm; call`), is an if/else with a
  full call in each arm; every `?:` spelling gives `neg/sbb/and` or `mov eax, imm` (TGMovieManager::AddMovie,
  NetFile::ReceiveChecksumRequest, CharacterClass::VirtualSlot66, WarpSequence's constructor, SetPlacement).
  The same holds for a pointer test: `if ( p ) WriteID( p->id ); else WriteID( 0 );` gives two pushes and one call
  (SubtitleWindow::WriteToStream). A member list chosen by an int flag is `if ( flag == 0 ) a else b`; `?:` and
  default-then-override differ (TGModelPropertyManager). `if ( i & 1 ) flags[i] = false; else flags[i] = true;` gives
  retail's branchy store; `?:` gives `not; and`. A default then an override, `bool b = true; if ( !cond ) b = false;`,
  gives `mov [x], 1; jne; mov [x], 0` (WeaponsDisplay::Unknown54C110), as `int state = 1; if ( level == 0 ) { state =
  0; }` gives `mov 1; test; jne; mov 0` where `level != 0 ? 1 : 0` gives setne, and `char *s = "b"; if ( flag ) s =
  "a";` gives `mov; je; mov` where the ternary gives `jne` (PyEmbed::RunInDebugger). One call setup hoisted above a
  branch with one shared call ( `fsubr; je; fsub [y]`) is `if ( c ) f( a, x ); else f( a, x - y );`; a ternary
  argument, a local or a helper spills (SubtitleWindow::Layout). A `jne/jmp` around an `fld` of the other branch is a
  ternary into a float local (`float h = c ? f() * s : o;`), not an `if` reassigning a member. A clamp ternary into a
  new local sharing the old one's slot gives a self-copy; assigning in place drops it. {asm: neg; sbb; and}
- **Bool results.** `return x ? true : false;` is this code's normalising idiom (retail's `test al,al; setne al`):
  `return ( flags & bit ) ? true : false;` gives `test [mem],reg; setne al` where `!= 0` gives `and; neg; sbb; neg`
  (WeaponProperty::IsMemberOfGroup); `a == b ? true : false` gives `cmp; sete al` (TopWindow::IsBridgeVisible);
  `return flag ? false : true;` gives a bare `test; sete al` where `== 0` and `!` add an `xor eax,eax`
  (SetClass::IsLocationEmptyTG); `return n > count ? false : true;` gives `setle` without the `xor eax, eax` that
  `return n <= count` emits (TGTimerList::IsValid); `return x == 0 ? true : false;` fixes the tail duplication of a
  bool return (TGConsole::Execute); a flag bit ending in `and al,1` is `( flags >> n ) & 1 ? true : false`, where the
  bare `& 1` gives `and eax,1` (STTiledIcon::IsTileSize). A CRT int result ending `xor ecx,ecx; test; sete cl; mov
  al,cl` is `return result == 0;` (TGSystemWrapperClass::FileSeek); a Win32 BOOL as bool with `neg; sbb; neg` is
  `return Call(...) != 0;` (MakeDirectory); `mov al,1` / fall-through `al` is
  `if ( A || B ) return true; return false;` (PlacementEditorMode::CanSelect). The idiom also holds for calls:
  `return f() ? true : false;` on a bool callee (VisibleDamage::IsCellFilled), `return Base::F() ? true : false;`
  (Mission::FixUpPointers) and `return x->IsTypeOf( T ) ? false : true;` (ZoomTargetMode::IsValid), where `!`,
  `!= 0`, `!!` or `== false` give `neg/sbb`. An unsigned comparison returned as `bool` gives `cmp; setb al` written
  `if ( a < b ) { return true; } return false;` (NiD3DTexturePipeline::Unknown7CE040) or `return a < b ? true : false;`
  (the SDK debug copy's form, NiD3DTextureManager::Unknown7D5640), where `return a < b;` gives `sbb eax, eax; neg
  eax`. {asm: neg; sbb; neg}
- **`return f() == -1 ? false : true;` gives `cmp eax, -1; setne al` after the EH restore;** `!= -1` sets the result
  through ecx first (CPyDebug_LoadInstance). {asm: cmp eax, -1; setne al}
- **Comparisons:** an unsigned compare against the last value ( `unsigned int next; next > LAST`) gives `cmp 3 / ja`
  (STToggle::NextState); a `jle` loop bound is `j <= STEPS`, not `j < STEPS + 1` (ShieldsDisplay::AddEvents); `> 0` on
  an unsigned count gives `jbe` where a bare truth test gives `je`; `count > i` instead of `i < count` at one site.
  `( n & 7 ) > 0` gives `xor dl, dl; cmp dl, cl; sbb; neg` where `!= 0` gives `neg; sbb`; an `unsigned int` product
  local shifted gives `shr` where the inline `unsigned short` expression gives `sar` (DamageBitArray,
  MetaVolumePolygonizer), and `(unsigned int)member >> 1` gives `shr` where the signed shift gives `sar`
  (BackdropSphere::GetGridSize). `cmp count, 1; jbe` into the call first is `if ( count > 1 ) Call(); else { ... }`, and `jbe`
  against 0 is `count > 0` (OBBLeaf::VirtualSlot04). {asm: xor; cmp; sbb; neg}
- **`i % 2 == 0` on a signed index** gives `and 0x80000001 ... dec/or/inc`; `& 1` gives a plain test (both occur).
  {asm: and 0x80000001}
- **`else if ( level > 1 )` after `if ( level == 1 )` reuses the `cmp 1` flags** (`jle`); `level > 0` adds a test.
- **A conditional divisor (`fild; jne; fadd 1.0; fdivr [m]`) is `if ( c ) m /= count + 1.0f; else m /= count;`;**
  `?:` and a divisor local keep `fld; fdiv st(1)` (GetCombinedConditionPercentage). {asm: fild; jne; fadd; fdivr}
- **Saved-ID lookups.** `id != 0 ? GetTGObjectPtr( id ) : NULL` gives retail's `je null` layout, where GetSavedObject's
  early return gives `jne` (RepairSubsystem::PostFixUp); `p = p ? Get( p ) : NULL` compiles to `xor eax,eax; mov`,
  where `GetSavedObject` gives the other shape (DamageIcon::PostFixUp). A `pos ? pos->field : 0` read that tests then
  loads with no `jmp`/ `xor` is an early-return inline (GetSavedID); `x != NULL ? ... : 0` gives the `jmp`/ `xor`
  form, and `x == NULL ? NULL : ...` the first form again. A NULL-guarded ID written through `GetSavedID( p )` matched
  where `p ? p->GetObjID() : 0` did not (ShipDisplay::WriteToStream).
- **`new NiPoint3` then `*p = *q` has no NULL test; `new NiPoint3( *q )` has one** (TestPoint shows both).


### Control flow: switches

- **A switch over an HRESULT laid out "OK first, error inline" is a saved result:** `if ( r != A && r != B ) { fail }
  if ( r == A ) ... else ...`; VC6 tests switch cases in signed value order whatever the source order
  (NiD3DFrameBuffer::CreateZBuffer).

- **Switches.** A `switch` with a separate `default` arm gives retail's jump table (QueryTextureFormats); case bodies
  are laid out in source order, so read the jump table's targets to recover the case order, and a case laid out last
  is last in the source (NetFile::ReceiveMessageHandler). A jump table with entries for values that reach the default
  body means each case had its own arm with an identical body (STSubsystemMenu::ShowUnknownName). A two-case `switch`
  ( `case '-'`, `case '='`) gives retail's compare chain where if/else-if does not (PlacementEditorMode::KeyTyped). An
  early-return switch tested first against one value is `if ( x != V ) { switch ... }` (TGNetwork::Connect).
  Cases ending `mode = m; break;` before one shared call duplicate the epilogue per case as retail does;
  `call; return;` per case merges them with the default (SubtitleWindow::SetPositionForMode).
- **A case merged into the next case's tail is a real fall-through** in the source (STFillGauge::UpdateGauge, a
  labelled `// BUG:`).
