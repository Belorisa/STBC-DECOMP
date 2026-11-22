# The class model

Vtables, constructors and destructors, member and parameter types read from the code. A wall reported as `structure`
starts here; the order to check things in is [triage.md](triage.md). Read `README.md` for how entries are written.

## The class model

### The class model: types from the code

- **Return and parameter types from the calls.** A "void" stream slot ending in a discarded `GetPos()` returns int;
  one override's `xor eax,eax` on failure proves it (PhysicsObjectClass slots 68–72). A vtable slot called with
  `ret 4` that returns `mov al,1` is `bool Xxx( TGStream * )` (ObjectClass slots 64/65). An early-return epilogue with
  no `xor al, al` before it means a `bool` return passed straight through from the last call: the claimed `void` was
  wrong (TGMouseEvent::GetPickRay). A `void` Save function whose callers use `eax` returned the saved value in the
  original (SaveStdout/SaveStderr): the callee compiles the same either way, so the callers are the evidence. A setter
  the SWIG wrapper calls with an `int` takes `int`, even when an enum names its values. A cdecl static still pushes an
  argument its body ignores: retail's push and `add esp` give the real parameter list (PyEmbed::FinishLoad). Callers
  indexing a table with an `unsigned char` result mask it ( `and eax, 0xFF`); retail callers without the mask mean the
  function returns `int` (MetaVolumePolygonizer::GetCornerBits). TGStream's slots 0x68/0x6C are ReadLong/WriteLong and
  0x60/0x64 ReadInt/WriteInt: pick the call from the slot (SubtitleWindow's stream).
- **Parameter types from the pushes.** A parameter pushed with `fstp` is a float, whatever the callee's draft said
  (VisibleDamage::SetUnknown00). A parameter pushed as a raw dword from a word array and masked to 16 bits by the
  callee is `unsigned short` (the NiBezier patch slots' first-corner argument, NiBezierRectangle::VirtualSlot47
  `...EEGPAE`). A claim's "bool" argument can be a NULL pointer: `Unknown45C290( object, false )` compiled the same
  with the real `const char *` suffix; read how the body uses a flag before trusting its type (EnableGlowAlphaMaps).
  "Retail keeps X in ebx and tests Y from eax" in a two-pointer guard can mean swapped parameters: read which argument
  the callee hashes or uses as the key (TGOverlayManager::AddController's `( controller, name )`).
- **A parameter type the bytes don't see** can still be wrong: `DamageableObject::VirtualSlot81`'s source is a
  `TGObject *` (Torpedo and PhaserBank pass themselves; the body only calls IsTypeOf and the Cast functions).
  Retyping it changed no byte and unblocked the whole PhaserBank beam cluster. A pointer parameter that receives an
  object of another class (the camera passed to RegisterObject) means the parameter type is a common base: retype it,
  don't cast (NiRender::RegisterObject takes `NiAVObject *`). The reverse also happens: one parameter threaded through
  a chain can be two different parameters. Check every leaf use before choosing a common type (the OBB ray tests carry
  the VisibleDamage callback, the sphere tests an array of OBBTriangleList).
- **The calling convention from the caller and the `ret`.** A `mov ecx, this` before a call marks a non-static member,
  even when the callee never reads ecx (`ret N`): UtopiaModule::RegisterHandlerNames, AddStateFunctions,
  CloakingSubsystem::Unknown55E840; a claim decorated `SA` for such a function is wrong (`QAE`). No `add esp` after the
  call means thiscall, even when the callee overwrites ECX at once and never uses `this` (OBBLeaf 0x46B020, 0x468770).
  A thiscall member that never touches `this` but returns through `ret N` is a non-static member
  (FloatRangeWatcher::SendEvent), and so is a member that reads nothing but whose callers load ecx
  (GraphicsMenu::GetChosenMode*); a "static" member that ends in `ret 4` and whose caller loads `ecx` is `__thiscall`
  (MapWindow::Unknown500990).
- **Member types from their uses.** Signed/unsigned of IDs and counts decides `jl` versus `jb`; retail's
  `and eax,0xFFFF` then dword store means a 32-bit member (TGKeyboardEvent's character). A float-looking member
  compared with `ja` is an `unsigned int` (GraphicsModeInfo::width). A byte store of a zero result into a member (
  `mov [esi+0xD8], al`) means the member is a `bool` (SetClass::GetDotProductToVector). Bit fields set through SetField
  take the field's width unshifted where direct reads use shifted masks: NiTimeController's out-of-line SetField
  copies; NiFogProperty's fog function, whose constructor sets the depth first (SetClass's copy).
- **Aggregates.** A `mov word; mov byte` zero pair is a 3-byte memset of an array or struct member (TGInputManager's
  mouse buttons); a count reloaded after an indexed store into a static array means the array and count are one
  aggregate (TGGamepad's joystick list). A dword store of 0x01010101 into a bool array is `memset( &flags[8], true, 4
  )`; separate stores stay four byte stores (TractorBeamProperty::Unknown697BC0 *(open)*). {asm: mov word; mov byte}
- **A vtable pointer followed by a double pads the first member to 8:** retail's members at 0x08 and 0x10 with nothing
  stored at 0x04 mean no member there (TimeSliceProcess's `double delay`).
- **A key type's Register filling table index 6** means the tables have 7 entries; check the table size before
  treating a register as an overrun (NiKeyType6FloatKey).
- **VC6 rejecting `delete p[i][j][k]`** on a `T ***` means one pointer level is missing from the declared type.
- **A by-value class argument filled through an out-of-line 16-byte copy** proves the parameter is an object, not four
  floats: `TGColorA( NiColorA )` at 0x5AF390 is byte-identical to NiColorA's four-float constructor; only the caller
  shows the difference. An EH-tracked by-value class argument (retail stores the argument's address before the copy)
  means the class declares a copy constructor (NiColorA).
- **TGPoint3 has a user destructor, NiPoint3 none, and the EH frame tells them apart.** A `TGPoint3` local, return
  value or by-value argument brings an EH frame and state stores; a function without them uses `NiPoint3`, one local
  reused rather than several ( `NiPoint3 direction = camera->GetWorldLocation(); direction = head->GetWorldTranslate()
  - direction;`, CharacterClass::Update; AsteroidTile::CreateAsteroid). A zeroed return-constructed flag or frame slot
  at entry in a function returning a vector by value means the return type is `TGPoint3` (CameraMode::GetAttrPoint,
  BaseObjectClass::VirtualSlot41); a by-value argument copied with no EH state store is `NiPoint3`
  (ShipClass::VirtualSlot90 *(open)*, from PlanetCollisionHandler), and so is a point returned by value into a local
  with no destructor EH state (OptimizedFireScript::PredictTargetLocation *(open)*, which fixed CheckGoodShot's state
  numbering) and a temporary with no unwind state of its own (ChaseCameraMode::GetIdealDirection's view offset). A
  handler copying an event's point into an EH-free local before a virtual call means the callee takes `NiPoint3 *`,
  not `TGPoint3 *` (WeaponSystem StartFiring). A `TGMatrix3 * TGPoint3` product assigned to an `NiPoint3` local
  reproduces retail's extra 12-byte copy (ChaseCameraMode::VirtualSlot32).
- **NiPoint3's copy constructor (0x423480) called out of line with a member's address** can be `NiPoint3 x =
  GetPosition();` where the inline accessor returns TGPoint3 by value (PhaserProperty::Unknown687350).
- **A pointer-to-member data member** ( `void ( Class::*fn )( float, float )`, called `( this->*fn )( u, v )`) gives a
  thiscall through a table-chosen pointer without changing the layout (NiBezierPatch).
- **A destroy-once guard on an exit destructor means a static data member;** without one it is a namespace-scope
  global (NiBezier's vertex cache, 0x8324D0).
- **Access is in the decorated name**: making a protected member public turns `IAE` into `QAE`; rebuild its defining
  unit (TGUIObject::DeleteBatches).
- **Names shadow silently.** Inside a member function, a free function named like a member accessor is shadowed
  (UtopiaModule's `GetDataPath()` compiled to a load of `dataPath`): give the free helper another name. Renaming a
  member to a name a parameter already uses shadows the member: rename the parameter (STWarpButton::Unknown53E990).

### The class model: vtables and slots

- **Virtual slot index to name:** `tools/vtable_ids.py`, or the relocations of `??_7Class@@6B@` in a compiled
  `build/obj/*.obj`; check a guessed slot against a compiled vtable (STFileMenu slot 79 is `SetFocus`, not the
  STMenu guess). Read the vtable before trusting a header's slot count: a `call [eax+0x60]` in a wrapper means the
  method is virtual (WarpSequence::SetEventDestination, slot 24; the header said 24 slots, retail has 25 or more). A
  vtable's neighbour in `.rdata` belongs to the next class: count a class's real slots before trusting a
  `VirtualSlot82` claim (DamageDisplay's 82 slots run into RadarDisplay's vtable).
- **VC6 lays overloaded virtuals out in reverse declaration order:** pick the overload from the slot retail calls
  (Torpedo::Create, `SetTranslate( const TGPoint3 & )` at 0x7C; of `Rotate( TGMatrix3 * )` and `Rotate( const
  NiMatrix3 & )`, the reference overload is the lower slot, 0x6C).
- **A constructor verifies only when its whole vtable does.** Its vptr store is a relocation to the class's vtable,
  which resolves only when every slot is defined or claimed: until then the constructor is REFERENT or WRONG-REFERENT
  with identical bytes (EngRepairPane, STRepairButton, WeaponsDisplay, TopWindow's stream slots 4-7). Diff the class's
  retail vtable against its base's to list the overrides; write them (GetObjType, IsTypeOf, the three class strings,
  GetClassHandlers, the handler table) or claim each one's decorated name in `config/symbols.txt` with the vtable slot
  as evidence (StarSphere, vtable 0x893C08; DamageDisplay, ShieldsDisplay, STFileMenu, SortedRegionMenu). A
  constructor calling only the base compiles to the base's vtable.
- **An implicit destructor or `??_G` that won't bind** in a unit without the class's constructor: the vtable isn't
  emitted there. Write the constructor and they bind.
- **`??_G` against `??_E`.** A compiler-generated `??_G` is annotated directly above its class as `// FUNCTION: STBC
  <addr> ??_G<Class>@@UAEPAXI@Z @<unit>`, the unit being the one whose vtable retail kept. The vtable holds the scalar
  `??_G` when the class's virtual destructor is user-declared and defined in the unit, and `??_E` when the destructor
  is implicit or declared but not defined there (CharacterAction). A claim written as `??_E` whose slot holds `??_G`
  is wrong (Waypoint 0x440F60). A name annotated at two addresses that are not COMDAT copies of two units is a SYMBOL
  CONFLICT that fails the gate.
- **A derived class's inline IsTypeOf that expands the whole base chain** means the base's IsTypeOf is inline in its
  header too (LightObjectClass::IsTypeOf, seen from PulsingLight::IsTypeOf). It binds only once the base's constructor
  or destructor emits the base vtable (SetClass blocks BridgeSet::IsTypeOf).
- **A claimed non-virtual "HandleX" that is `mov eax, [ecx]; jmp [eax+N]`** is the vcall thunk a
  `(TGHandlerMethod)&Class::Handler` registration emits for a virtual handler: `??_9@$B<offset>@AE` (0x158 ->
  `$BBFI`), annotated above the class with `@unit`, one address per name (NamedReticleWindow's handlers).
  {asm: mov eax, ecx; jmp eax}
- **`call [<vtable address>]` on a stack object** is a virtual call through a pointer with a known vptr; a direct
  member call is expanded inline (DamageableObject::VirtualSlot83, MATCH-PENDING: the original spelling is unknown).
- **Ghidra names a bare `jmp` stub after its target** (0x403350 shown as `TGEvent::~TGEvent`): read the bytes.

### The class model: constructors and destructors

- **Init list or body: the vptr store tells.** Members set in the mem-initialiser list are stored before the vptr,
  body assignments after it (EffectController, EffectAction, BridgeEffectAction, KeyArray; NiPoint2 members in the
  Editor constructor; `OBBNode() : left( NULL ), right( NULL )`; Backdrop's `modelName( NULL )`; a derived member's
  `level( 0.0f )` in TGDimmerController). A member stored before the vptr that the class doesn't own is a base
  class's, set by the base constructor (CollisionTest).
- **NiPointer members in constructors.** An inline constructor assigning a NiPointer member in its body gives
  `mov [p], 0`, the EH state, then a conditional refcount load/inc/store; an init list `: shape( shape_ )` gives `inc
  dword ptr` (OBBTriangleList). A constructor that assigns `member = NULL` to a NiPointer member keeps the EH state
  retail shows around the `new`; without the assignment the EH frame disappears (NiMultiTextureProperty::Map, used by
  LoadBinary).
- **A `new X` emits an EH frame only when X's constructor isn't known to be nothrow:** a constructor of the same
  unit defined *before* the new-expression is seen as nothrow and the frame disappears; defined after it, retail's
  frame appears (FloatRangeWatcher's constructor, FloatRangeCheck defined after it). The same mechanism drops EH-state
  stores after an inline's out-of-line copy: walls.md, the unit-order entry.
- **Building an object through the constructor that takes the source,** rather than default construction plus a
  setter, adds the EH state retail shows (NiD3DPixelFormatEntry: NiDDImage::CreateFromRawFile).
- **A derived class whose constructor only stores its vptr after the base's** (vtable slots 1..n the base's) is a
  game class with an empty constructor (EdgeMap over NiTMap<__int64, unsigned short>).
- **Destructors.** An explicit destructor, empty or not, inline or in the `.cpp`, stores the vptr first (`mov [ecx],
  vtable; jmp ~Base`: ~SubtitleAction, the TG message classes); an implicit one is a bare `jmp ~Base`, annotated above
  the class with its decorated name (the camera modes). A derived implicit destructor compiled as `jmp ~Base` needs
  the base's destructor declared explicitly ( `virtual ~BackdropSphere()`) and its body claimed. An implicit virtual
  destructor of a class with destructible members compiles to an out-of-line `??1` with no vptr store of its own,
  called from `??_G` (EffectControllerData); a destructor of only `mov [ecx], vtbl; ret` that unwinding calls is a
  derived class's implicit destructor storing the base vtable (CollisionTest 0x893F10, DamageCollisionTest 0x594530).
  EH states in a destructor count the members that have destructors (TorpedoTubeProperty's two TGPoint3), though VC6
  elides the stores between reverse-order member frees (VisibleDamagePiece's six bit arrays). A `delete` with no NULL
  test means the class declares no destructor; one with a user-declared empty destructor gets the test (NiPosKey,
  NiFloatKey in TGAnimTrack's destructor).
- **`delete` on an incomplete type** emits `operator delete` instead of the virtual deleting destructor
  (`call [edx]` with 1): a delete wrapper that differs only there needs the class definition with its virtual
  destructor.
  Recovering such a class is cheap when its SWIG family waits on it: declaring the class with its base and overrides
  matched new_, Create and delete_ together (TGPythonInstanceWrapper, PythonMethodProcess, WarpFlash, the file
  dialogs).
- **A destructor that stores no vtable of its own on entry means the polymorphic map is a member, not a base:** VC6
  stores a derived class's own vptr first thing in its destructor. TGConfigMapping's 0x71C720 stores only the map's
  vtables (0x896B1C, then 0x896B30) after its body: `NiTStringMap<TGConfigSection *> sections` at 0, no vtable. A map
  whose hooks and `??_G` are exactly NiTStringMap's, with one vtable, is a plain instance or a member of a class
  without a vtable; a destructor of its own that runs before the map's (TGConfigSection 0x71C950 deletes the values,
  then the map's vtables follow) makes it a member of a non-polymorphic class, not a typedef.
- **A NULL-tested `lea reg,[this+N]` in a destructor** is a non-polymorphic base at offset N, not a member
  (TGModelPropertyList derives from NiTList). A map global whose constructor stores only the template vtables has no
  vtable of its own: the map is a member, not a base (TextureAnimManager).
- **A header destructor that deletes the elements and then calls RemoveAll** explains two out-of-line RemoveAll calls
  (NetFileNameList).
- **A dynamic initialiser `new X` with per-member EH state stores** shows X's members have destructors (GraphicsModeInfo,
  from graphicsMenuInfo's initialiser).
- **An out-of-line TGString assignment called from a template** means TGString declares `operator=` (0x6F4EE0).
- **A default argument shows in a closure:** retail's default-constructor closure for an array calling `TGString("")`
  means the constructor was declared `TGString( char *string = "" )`.
- **`delete [] elems` over 4-byte elements through the vector-delete helper with `0x402020`** is an array of
  `NiPointer<...>`: 0x402020 is every NiPointer's destructor body (TGIconGroup).
- **The TG message family:** the type slot (an inline returning the type), Serialize and GetBufferSpaceRequired in the
  NameChange shape, and a copy constructor that copies only TGMessage; Copy is `new X( *this )`, then copies the
  class's own members (TGBootPlayerMessage's `bootReason`).
- **A constructor's store order is decided by the scheduler's ties on input order:** stores of a register-held
  constant (`xor eax, eax`, `mov ecx, 0x3f800000`) group ahead of the vtable store and of immediate stores, each group
  in the order of its first statement. Reorder the assignments by the group retail stores first (BackdropSphere: the
  1.0 tiles before the 10.0 tiles, `decimate` before the blend modes); an init-list member is stored before the
  vtable and a body statement after it, so retail's `zeroes, vtable, argument` is the zeroes in the list and the
  argument's member first in the body (TGBufferedFileStream). `c2trace --sched` names the tie that decides.
- **A conversion left implicit and an explicit functional cast are different temporaries:** as an operator's
  `const T &` argument, `a * T( m )` passes the constructor's return in eax and packs the temporary in another frame
  slot, `a * m` (the converting constructor applied by the compiler) passes it by `lea` in its own slot
  (LockedPositionMode::Update).

### The class model: casts and type tests

- **`NiDynamicCast( T, expr )` evaluates `expr` twice**: two back-to-back calls in retail (GetObjectByName) with no
  `if` between them are that macro, not hand-written code (CharacterClass::GetHeadHeight). A result tested after the
  `sbb`/ `and` is the macro's own NULL test, so the source has no explicit one before the cast (CharacterClass's
  MarkSkinControllers, AttachLights); an extra `test eax,eax` after it means the cast wasn't nested under an earlier
  NULL test of the same object (TGAnimNode::SetTarget). Retail shows both calls for a cast of a call (TGAnimNode
  FindNode, SetRootNode).
- **A NiDynamicCast tree walk matches only without an explicit `if ( object == NULL )`:** the casts' own null tests
  merge into retail's single test (FindVertex, CountVertices). Separate early returns that each test the cast's
  input are the same null test: dropping them matched TGSystemWrapperClass::TakeScreenshot. The reverse also occurs:
  ModelPass::FindShapes needs `if ( object == NULL ) return;` before its two casts (retail tests the argument before
  pushing its last register); try both.
- **A recursive node walk** matches with `NiDynamicCast` for the root and `NiIsKindOf( &NiNode::rtti,
  child->GetRTTI() )` per child (BaseObjectClass::ContainsNode); a loop bound at `[node+0xd8]` is `GetChildCount()`.
- **An `NiIsKindOf` rtti target is identity evidence:** EnableGlowAlphaMaps compares NiLODNode's rtti (0x9A21B0), so
  the cast is to NiLODNode, not NiNode.
- **A virtual `IsTypeOf` with no NULL check** is a plain cast after `IsTypeOf`, not the out-of-line `X::Cast`.
- **A child read through NiNode::GetAt keeps GetAt's own size test** (`[node+0xD4]`, `children.GetSize()`; the
  child count is 0xD8). The weapon-property updaters are `NiPointer<NiLines> lines = (NiLines *)node->GetAt( i );`:
  the funclet's destructor (0x58FE20, beside the GetAt temporary's 0x401EA0) names the smart pointer's type.
- **A direct `[shape+0xD0]` read is `GetShapeData()`;** `GetModelData()` is virtual and emits a vtable call.
- **A `NiDynamicCast` result moved to a callee-saved register and tested again (`mov edi, eax; test edi, edi`)**
  where ours reuses the `and`'s flags means the walk has no `if ( object == NULL ) return;` of its own: the casts'
  NULL tests thread to the end (ModelMerge_FindShapes, BridgeObjectClass::ApplyGamma; PrepareGamma, the twin, too).

### The class model: modules, creators and helpers

- **Loader-created and header-only classes are emitted by the class creator retail keeps,** a `static TGObject
  *CreateX( long id ) { return new X( id ); }` (43 bytes: `push 0; push str; push size; mov ecx, pool; call; ...; call
  ctor(J)`) in the registering unit, usually UtopiaModule's. Annotated there, it emits the class's vtable, inline
  virtuals, `??_G`, destructors and `X( long id ) : Base( id ) {}` constructor in that unit; annotate them in the
  class header with `@game/UtopiaModule` (WarpSequence, SubtitleAction, ObjectGroup 0x43EF40, ObjectExplodingEvent
  0x43F880, CharacterSpeakingQueue, CharacterAction, Planet, Sun). The registration function (0x43BDF0) pushes each
  creator with its type id, the header's `*_TYPE`: that names the creator, and `sizeof` in its allocation checks the
  header's size (SpaceCamera was 4 bytes short). When the inline constructor's vtable chain comes out WRONG-REFERENT
  (PlacementObject), claim `??0X@@QAE@J@Z` in symbols.txt on the vtable-store evidence until the inline virtuals are
  annotated. A disposable scratch unit calling `new X( id )` checks them first.
- **A module's RegisterClasses** is `new X( id )` per creator, with `X( long id ) : Base( id ) {}` emitted out of line in
  the module's unit; default-constructed members give the zeroing, NiTList members need `( 2 )`, and the creators'
  allocation sizes reveal members the headers lack (InterfaceModule, UtopiaModule).
- **Empty statics reached by a tail jump** are another class's empty RegisterHandlerNames/RegisterHandlers, defined
  out of line in that class's unit (PhaserBank).
- **A large helper whose only copy sits in the caller's unit and is called out of line from every site** is a
  non-inline member defined in that .cpp, not a header inline (SimpleFeatureCollector::AddVertex / AddTriangle).
- **A retail function that nothing calls has external linkage:** VC6 drops an unreferenced `static` function, so
  the dead helper is a plain free function (CharacterClass.cpp's texture-property walker 0x66D830 *(open)*).
- **Header math helpers seen whole inside SWIG wrappers** are inline statics of a utility class named by the SWIG
  strings (TGModelUtils, TGGeomUtils); where chained inline members stay a call, spell the whole body as one
  expression (LocalToLocalUnitVector).
- **A cross with constants folded in** ( `y - 0.0`, `0.0 - x`, z an immediate 0) is a local `NiPoint3 up( 0, 0, 1 )`
  passed to an inline UnitCross (CharacterClass::UpdateWatching; CharacterClass::LookAtMe *(open)*, where retail calls
  Length out of line).
- **A delete wrapper or `Create` copying float arguments to stack temporaries before another call** is an inline
  `static Create` wrapper: `STRoundedButton::CreateW( text, event, w, h, false )` (STLoadDialog::CreateButtons).
- **`NiTList::FindPos( T, NiTListIterator start = 0 )` keeps the lineage's `start` argument:** retail's copy tests it
  first (0x5AA2B0); the signature closed five functions across classes.
- **A TGWindow subclass's inline calling a child accessor directly (not through the vtable)** calls the qualified
  `TGPane::GetNthChild`: TGWindow's overrides go to the subpane (STMenu_GetText, STButton_GetText).
- **In broadcast handlers, the object is the event's destination (+0x0C),** not its source (ObjectGroup).
- **A stream writer loading `[ebx]` twice** (once for the ID, once for the test) is `WriteID( GetSavedID(
  entry->tile ) )` followed by a fresh local `T *tile = entry->tile; if ( tile != NULL )` (AsteroidField::WriteToStream).
- **Hand-written `%native` App functions live in PyEmbed.cpp** as `extern "C" _wrap_X`, parsing a bare "O" with no
  ":name" (IsNull, DeleteBuffer; Breakpoint is ORIGINAL-ASM `__asm int 3`).
- **A Python method whose shared `Py_None` tail sits at the end** is `if ( PyArg_ParseTuple( ... ) != -1 ) { ...;
  return ...; }` (ObjectGroup's tuple methods).

### The class model: headers and pragmas

- **Windows headers.** When the bytes need a real system type as a member ( `in_addr`, `WSADATA`), include the header
  and `#undef GetClassName`, `GetObject` and `PlaySound` after it, as TGApplication.h does; otherwise forward-declare
  (TGWinsockNetwork). Never include `<windows.h>` in game headers: its PlaySound macro breaks AppWrap. winbase.h's
  `GetCurrentTime()` macro renames an accessor of that name to `GetTickCount`: `#undef GetCurrentTime` after
  `<windows.h>` (NiApplication). A unit that reaches `<windows.h>` through a third-party header renames
  `GetClassName`/`GetMessage` methods: an extra vtable slot (R13) or a broken call elsewhere; keep the SDK header out
  of shared game headers and include it last in the `.cpp` (GameSpy). Include the renderer (Windows/D3D) headers after
  the game headers: with `Glow.h` (NiD3DRender.h) first in SetClass.cpp every `ObjectClass` virtual call shifted by 4,
  and RemoveObjectFromSet and VirtualSlot30 lost their match; last, both match (lead, not traced: VC6 groups
  same-named virtuals, so a `GetClassName` renamed to `GetClassNameA` can move slots).
- **Header dependencies.** Keep NiApplication.h out of widely included headers (in TGInputManager.h it broke
  TGInputModule's string-pool referents). A header inline assigning an `NiPointer<T>` needs T complete in that header:
  VC6 compiles the body in every includer, so a forward declaration breaks them all (TGIconManager::SetCamera).
- **A C++ function with an inline `__asm` block** gets retail's frame (all three callee-saved registers pushed, nothing
  enregistered) with `#pragma optimize( "g", off )` (_UnwindNestedFrames, ORIGINAL-ASM).
- **Unoptimized functions in an optimized unit are `#pragma optimize( "", off )` ... `( "", on )`**, not a unit
  of their own under `/Od`: NiProcessor.cpp holds the /O2 NiTransformVectorsPentium and, under the pragma, the
  plain transforms (Gamebryo lineage), with the same bytes as the former `/Od` unit.
- **Under `/Od` a local's frame slot order follows a hash of its name** (docs/c2-backend.md, "Frame layout at /Od").
  The hash narrows a name without proving it: a name chosen only for the slot order is marked `(name invented; fits
  the /Od frame order)`, as for AnimOnly's /Od copies; a lineage name that fits it is the original's (Gamebryo's
  `pFMatrix` in NiProcessorSpecificCode's Basic transforms, NiOBBox's `A`, `extA`, `rsum`, `C`, `D`, `V`, `dt`, the
  DX8 lineage's in NiD3DLightEntry::Update, *(open)* on its release walls).
- **Under `/Od`, `*( p + 1 )` and `p[1]` are different code**: in a product `m[1] * *( p + 1 )` the dereference
  form loads `p` and pushes the coordinate before loading `m`, the subscript loads both pointers first
  (NiProcessorSpecificCode's TransformPointsBasic and TransformVectorsBasic, Gamebryo 1.1.2's text).
