# Constants, data and referents

Float constants, string pools, IAT slots, COMDAT copies and identity claims. A wall reported as `referent` starts
here; the order to check things in is [triage.md](triage.md). Read `README.md` for how entries are written.

## Constants, data and referents

### Constants, data and referents: float and integer constants

- **A float constant's home names its spelling.** A literal or `#define` gives an immediate ( `push 0x42FA0000`,
  Planet::Create) or the program-wide merged pool entry that every unit shares ( `return 0.0f` / `-1.0f` from
  0x888B54, about 1200 users; PhaserGlow and TractorBeamGlow VirtualSlot35). A value only this unit reads, in the
  unit's own `.rdata` beside its vtables or strings, is a unit or class `static const float` with a DATA annotation (
  `UPDATE_INTERVAL`, `AI_TURN_TIME`, `NiTriIntersect::epsilon`, Planet's 7.5 and 0.5 at 0x8942B0/B4, DamageIcon's
  blink period, the refresh delays of TacWeaponsCtrl and EngPowerCtrl, WeaponsDisplay's). Adjacent equal values are separate statics
  (PhaserBank's discharge factors), and a PI next to a class's vtable is a per-unit `static const float PI`, which VC6
  doesn't fold. Check the xrefs before adding the DATA: a named static at an address other than the pooled entry makes
  matched neighbours that share the literal WRONG-REFERENT (0x893F28 against 0x88BF28, DamageableObject::
  Unknown5952D0 breaking DeadObjectRemovalCheck). A DATA annotation can also move an unannotated literal neighbour of
  the same unit's `.rdata` next to it (MIN_SPIN, which broke TGFuzzyClusterInnerController::Reset): leave such a const
  unannotated and let the referent take retail's address.
- **The instruction tells a literal from a `static const float`.** `fld var; fmul const` is a literal; a static
  multiplier loads first ( `fld const; fmul var`, TractorBeamProjector::PullTarget, PushTarget). `fdiv [unit .rdata]`
  where a literal would give `fmul` by the reciprocal is a static (Planet). A float pushed as an immediate to an
  out-of-line `operator*( float, const NiPoint3 & )` is a literal ( `( 1.0f / 3.0f )` gives `push 0x3EAAAAAB`),
  written in the friend form `scalar * point` (NiBezierTriangle2::Unknown8395C0 *(open)*); a static is loaded and pushed
  instead. A `static const float` is read at run time, not folded, in a dynamic initialiser and in `const * literal`
  arithmetic: when retail loads a constant from the unit's `.rdata` there, a named const object is the spelling
  (CollisionTestDisplay's `maxEdgeLength` at 0x8939A0, Nebula's 14.0f at 0x893F2C). Spelled as named statics, a unit's
  literals beside its vtable can flip the x87 operand order instead (ShipClass::Unknown5AFAD0's 500.0f and 900.0f at
  0x8944B8): try both.
- **A unit's file-static `const float` objects are laid out in definition order,** which must follow retail's
  `.rdata` order, or the referent is WRONG-REFERENT (OptimizedSelectTarget's RATING_DISTANCE defined first). When a
  function moves into retail's order, its file-scope `const float` moves with it to just above its first user:
  ProximityManager::CollideDamageables, moved alone, read `_RESTING_SPEED` at 0x8942D4 instead of retail's address
  (WRONG-REFERENT); moving the `RESTING_SPEED` definition with it restored the match.
- **When retail reads another copy of an "equal" constant, read retail's dword first:** the `__real` at 0x888A78 was
  0.01f, not 0.0.
- **`pow` and float constants.** `(float)pow( ... )` keeps float constants (`fmul dword`); without the cast VC6 makes
  them doubles (NiBezierCylinder::Unknown84D020). `255.0f / (float)pow( 255.0, e )` keeps a dword next to the pow's
  double; without the cast VC6 folds both 255.0 constants into one qword (BuildGammaTable). A dead `fst` of a pow
  result into a frame slot is a named float local (BuildGammaRangeTable).
- **An `extern const` global is loaded from memory where its definition isn't visible, folded where it is.** A mask
  loaded from .rdata in some functions and an immediate in others is an `extern const` defined in its unit: the
  functions after the definition fold it, other units load it, and so does a header inline parsed before the
  definition (TCW_MOUSE_PICK_FIRE 0x88E9F4, TacticalControlWindow::GetMousePickFire).
- **Event and flag constants** come from the SWIG constant table (App.pyc) and are written as the named constant. App
  WC_/ET_ values are in the binary: find the name string, then the dword store of its pointer; the value is the
  immediate of the next `c7 05` store. Constants set from registers (MWT_*, SM_*) are in the constant-table init near
  0x6542C0, in value order.
- ** `GetRandomNumber` returns unsigned** (retail converts with `fild qword`, zero high word): cast at the call. An
  unsigned `jae` against a constant on an `int`-returning call, with signed compares elsewhere, is an `(unsigned int)`
  cast on that call (CharacterClass::CreateRandomAnimation).
- **A big-endian short read** is `unsigned char bytes[2]; Read( bytes, 2 ); return ( bytes[0] << 8 ) + bytes[1];`
  (retail's `xor ax,ax / mov al,cl`); reading an `unsigned short` and swapping does not match (NiReadSGIFile).
  {asm: xor ax, ax; mov al}
- **A small constant `for` loop is unrolled by VC6:** four NULL stores into a member array are
  `for ( i = 0; i < 4; i++ ) a[i] = NULL;`; `memset` and four statements both differ (STToggle constructors).
- **`BITMAPFILEHEADER h = { 0 };` gives retail's dword zero stores before the field stores** (SaveBMPFile).
- **A member read `[height*4 + i]`** of a 1-D array is a 2-D array `[height][i]` (g_kSTButtonColors). A DATA
  annotation inside another global's extent is a row of that global: the four TacToggle colours were
  `g_kSTButtonColors[2]`, filled by its initialiser (TacWeaponsCtrl).
- **A one-byte message built in a 0x40-byte frame** is a `char data[64]` buffer (MultiplayerWindow::ConnectHandler).

### Constants, data and referents: strings, statics and imports

- **The string-literal pool follows source order:** a literal WRONG-REFERENT is fixed by writing the expressions in
  retail's order (PlacementEditor::EditWaypointSpeed). A `switch` whose string literals sit in a given pool order
  follows it too: retail's pool is in first-use order, so the `case` order follows the pool
  (KeyboardBinding::GenerateMappingFile: EVENT, BOOL, INT, FLOAT).
- **A pointer to a string in .rdata beside the unit's vtables**, not in the 0x8Dxxxx literal pool, is a unit
  `static const char NAME[]` (LODModel's texture-path builder).
- **`""` pushed as 0x97DC28** is the shared empty string literal, not a global ( `TGString()` and
  `TGParagraph::SetString( "", ... )`).
- **A counter read by one function only** is a function-local static (PyEmbed::SaveModuleState's `objectNumber`,
  0x99AE04; the function *(open)*, declaration-sensitive on its frame). A function-local `static` table carries its compiled name, with VC6's leading underscore (
  `_?roots@?1??PathLength@NiPathController@@IBEMIIM@Z@4PAMA`).
- **A static used only by an unwritten caller isn't emitted** (NO-SYMBOL): write the caller first.
- **Pool calls carrying the TGMEMORY file string** are plain `malloc`/ `realloc`/ `free` in game source: the
  TGMemory.h prelude routes them.
- **IAT claims.** Check an IAT claim against the import table, not the caller's guess: swapped `timeGetTime`/
  `GetTickCount` claims still matched byte for byte (TGRootPane, TGSystemWrapperClass). An IAT call at an unexpected
  slot is a different API (console I/O through WriteConsoleA/ReadConsoleA, not WriteFile/ReadFile: PyEmbed's
  exception console). Win32 and COMCTL32 calls through IAT slots need `__imp__` entries in config/symbols.txt.
- **A method with no xref and no pointer in the image is a dead method of the class around it:** write it as
  `<Class>::Unknown<ADDR>` (GraphicsMenu, PhaserBank, PowerSubsystem, RepairSubsystem).
- **A Ghidra gap that starts with an EH prologue right after a draft's end** means the draft is shorter than the
  retail function: its score is partly a missing tail (NiBezierRectangle3::VirtualSlot46 *(open)*).

### Constants, data and referents: COMDAT copies and claims

- **Retail has no identical-COMDAT folding** (no `/OPT:ICF`): it holds 68 byte-identical `~NiPointer` bodies, and each
  `??_G` is referenced from its own class's vtable only. Byte-identical functions at two addresses are two source
  functions (MetaVolumePolygonizer 0x4C3680/0x4C38C0), two instantiations, or two units' copies of one instantiation that
  the linker kept (`~NiPointer<NiAVObject>` at 0x401EA0 and 0x416F80): tell them apart by their callers. A COMDAT that
  only EH vector iterators with element size 0xC reference is `NiTListItem<NiPointer<T>>`'s constructor or destructor,
  not `NiPointer`'s. A second out-of-line copy of a NiTArray template function at another address is a different
  instantiation, not a folded COMDAT: use it to pick the element type (OptionsWindow).
- **A claim or annotation for one unit's copy of a COMDAT takes every unit's callers.** Annotating a template copy
  `@unitA` makes every unit without its own annotated copy resolve to A's address, so an identical copy kept by another
  unit then breaks (CharacterClass's NiTListBlock<int>, which captured CharacterClass::AddPhoneme's callee;
  SortedArray::DeleteAll against ~SortedArray in TGNetwork); a claim for NiPointer<NiAVObject>'s `??_F` at 0x7E6500
  moved NiParticleSystemController::SetNumParticleObjects off its own copy (0x7AF440); with only one copy of
  ~NiPointer<NiAVObject> (0x401EA0, 0x416F80) or its assignment (0x416FA0, 0x46C4E0) annotated, the other units'
  callers turn WRONG-REFERENT. Annotate every unfolded copy, each with its `@unit`, or leave such copies unclaimed, and
  run `build -s` after adding one. A PROBLEM line "annotated at X and listed at Y" means a claim names another unit's
  copy: check which unit emits the copy before annotating (NiTListBlock<ObjectClass*>, 0x41BB20 vs 0x586BB0).
- **Seven EH vector-iterator calls pass 0x407DD0 as the element destructor** (a `~NiPointer` copy, from
  NiTArray<NiPointer<T>> `??_G`, `??1` and SetAtGrow). Never annotate a per-type `??1NiPointer<T>` copy there: it
  repins the referent and breaks matched users (DamageableObject::InitObject, ~MetaVolumePool).
- **Don't claim `?CreateClone@NiObject` at 0x458780:** several data classes keep their own `return this` copies, and
  the claim made 14 functions WRONG-REFERENT.
- **A COMDAT a vtable references is kept from the first unit in link order that creates it** (TGFuzzyTriShape keeps
  NiTriShapeLoadRecord's and NiObjectNETLoadRecord's `??_G`); a copy's `@unit` isn't read from unwind callers alone.
  Old `@unit` bindings on byte-identical header copies often name a far unit whose copy merely compiles alike: don't
  use them as layout evidence.
- **Two identical template bodies in one late unit are two element types:** the linker keeps the first copy per name
  it meets, so an earlier-linked copy pins which name is taken (TGProfilingInfo's NiTList NewItem copies 0x719E80 and
  0x719F40 against NiTList<long> at 0x434810 and <int> at 0x486880).
- **Which element type a template copy carries** is settled by `comdat.py layout` together with the callers in
  Ghidra (unwind funclets, `atexit` thunks, the order in which a destructor releases members). A fit by bytes alone is
  not enough: two LODModel copies that fitted (0x4540B0, 0x454510) broke matched callers that expect the names
  elsewhere (0x416F50, 0x453850); `regress` shows it. Template COMDAT runs in a gap are fit by consecutive sizes
  (`tools/comdat.py`: seven basic_string<unsigned short> members at once in SetClass's range); unwind funclets that
  call an out-of-line `~SortedArray` tell its copies from the DeleteAll copies.
- **A template copy only EH unwinds call** (`lea ecx, [ebp-N]; jmp copy`) is traced to its owner: the unwind-map
  entry, its FuncInfo (magic 0x19930520), the `mov eax, FuncInfo` handler stub, and that stub's one data xref, which
  is the owning function. The local's frame offset then names the type; `[ebp+X]` off `this` in a constructor is a
  member (SetClass's and OptimizedAvoidObstacles' list and smart-pointer destructors; 0x417B30/0x417CE0/0x417530 are
  the `NiTList<ObjectClass *>` instance, not Backdrop's or ShipClass's).
- **Byte-identical COMDAT copies** are told apart by a caller: 0x423480 is NiPoint3's copy constructor (a caller
  copies into the hidden return slot) and 0x414560 its `operator=`, which local axis copies call as
  `NiPoint3 forward; forward = x;`. A unit-local COMDAT can verify at the wrong copy's address and hide a source error
  (NiPosKey's constructor assigns `pos = pos_` in its body). Of two byte-identical NiTList copies in a unit, the one a
  function's end calls is `RemoveAll` (the destructor is expanded there); the uncalled twin is `??1`, kept for the
  unwind funclet (SpecularPass's unit, 0x4F0240 and 0x4EFC40).
- **A COMDAT map or list whose vtable only one class's constructor and destructor use** marks a unit holding just that
  class; the vtable addresses tell which instantiation each copy is (the NetFile maps). Several owners sharing one
  template vtable and SetAt are one instantiation; the type that decides it is the one the out-of-line `NiPointer<T>`
  callees take (the image map is `NiTStringMap<NiPointer<NiImage>>` for ImageManagerClass, TGIconManager,
  TGModelManager and NiStream). Map hooks are matched to vtables by the constructor's vtable store order (base map,
  string map, then the next member), which settles which identical copy is which (TGOverlayManager, TGObject).
- **A unit's NiTMap and NiTList COMDAT copies** annotated in the template headers with `@unit` land in bulk once the
  value types are right: the item size in `SetAt`'s `operator new` and what the value's destructor does give the type
  (MetaVolumePolygonizer's maps, the one at 0x68 `NiTMap<__int64, DamageVertexIndices>`: SetValue copying three
  dwords was the value type, not an NiPoint3).
- **A `const NiRTTI X::rtti( "X", &Base::rtti );` definition gives a four-initialiser group** (stub, 32-byte body,
  atexit thunk, guarded exit) where the definition sits among the functions; such a run in a unit's code is evidence
  for the definition at that spot (the particle controllers' rtti).

### Constants, data and referents: SWIG wrappers

- **A SWIG wrapper's types come from its `[0x99xxxx]` SWIGTYPE loads**: index = (address - 0x991068) / 4. A `char *`
  result is `if ( !result ) Py_None else PyString_FromString` with no INCREF; a member setter returns None. `if ( dest
  != NULL ) SetDestination( dest ); else SetDestination( object );` matched where a ternary didn't. A wrong index
  shows as WRONG-REFERENT. A wrapper's `(T )` result cast is the declared return type: `_wrap_UtopiaModule_CDCheck`'s
  `(bool )` makes UtopiaModule's inline forwarder return bool over TGApplication's char; the char forwarder keeps the
  result in a register where retail spills it to the dead `args` slot.
- **SWIG wrappers through an inline accessor:** the member read or written at the offset (`fld`/`fstp`/`mov` against
  the object pointer) names the member; add `T GetX( void ) { return x; }` to the class header and the wrapper
  matches with no diff work. A wrapper whose callee is an inline setter copies the value in place
  (STRoundedButton `Set*Color`). Setter wrappers that store through fld/fstp temporaries call inline setters that
  build a temporary: `color = NiColorA( r, g, b, a )`, `gravity = NiPoint3( x, y, z )` (TGFrame_SetNiColor,
  EmitterController_SetGravity).
- **SWIG pointer-to-base accessors:** `TGColorA_*_set/get` and `TGPoint3_*_set/get` take `NiColorA *` / `NiPoint3 *`
  with `SWIGTYPE_p_NiColorA` / `SWIGTYPE_p_NiPoint3`, and the format string names the base ( `"Of:NiColorA_r_set"`).
  A WRONG-REFERENT on a `TGColorA_...` format string means this.
- **Bool and int in SWIG wrappers.** A wrapper that pushes a `bool` argument's dword without `and 0xff` calls a method
  taking `bool`, not `int` (CharacterClass::Create); without a declared bool the wrapper takes `int arg1`. A wrapper
  returning bool from a flag test ( `test mem, imm; setne`) calls an accessor returning `int` ( `return flags & X;`)
  and casts, `result = (bool)arg0->IsX();`, and an accessor whose wrapper zero-extends or converts a flag bit with
  `test; setne` returns `int` (CharacterClass::IsMenuEnabled, STSubPane::IsRadioGroup as `flags &
  STSPF_RADIO_GROUP`); a `bool` accessor with `!= 0` gives `shr eax, 1Fh; and al, 1` (TGParagraph::IsReadOnly).
  TGRect IsEmpty/IsClear wrappers use a bool result.
- **SWIG defaults.** The format string's leading `_` in Ghidra's symbol name is `|` or `:`: a leading `|` makes every
  argument optional, and the prologue's zero stores are the declared defaults ( `float arg = 0;`, new_NiColor). A bool
  parameter with a default is `bool arg1 = true; int tempbool1 = (int) true;` with `"O|i:"`, then
  `arg1 = (bool) tempbool1;`: retail's `mov dword ptr [esp+N], 1` before PyArg_ParseTuple shows the default. Float and
  int defaults show the same way ( `float arg = 0.05f` from `mov [esp+N], 0x3D4CCCCD`); which of several int defaults
  is 1 is settled by swapping and rebuilding (CharacterClass_SayLine). A default `char *` (NULL) takes the
  `if ( objN ) { None / PyString_Check }` block, a default pointer the plain ConvertPtr with a NULL argument. Default
  immediates name their flags: 0x80000000 TGPF_READ_ONLY or STBF_FILL_PARENT_WIDTH, 0x60000000 STMF_DEFAULTS; 0x97DC28
  in a register is the shared `""` default.
- **A raw `if ( objN ) argN = PyString_AsString( objN );`** with no None or type check is its own typemap, apart from
  the standard `char *` one (TGAnimNode_UseAnimation). A function-pointer argument goes through ConvertPtr with
  `SWIGTYPE_TGAnimCallbackPtr` (no `p_`).
- **A wrapper returning a constant where the method looks void** shows the method's own return: TGSound::SetPriority
  returns `true`, TGPlayerList::AddPlayer passes SortedArray::Insert's 1 on.
- **A wrapper whose `operator new` result is passed as a call's hidden argument** wraps a by-value return:
  `new NiPoint3( obj->GetX() )`, `new X( arg0->GetX() )`; a by-value return per branch also closed
  TGSound::GetPosition's separate-blocks wall. A static returning NULL is `result = (T *)NULL` (CreateNull).
- **A hit-test inline returning `a >= 0 && a <= w && ...`** gives the stack-bool `mov [..], 1` placement
  (TGUIObject::IsWithin); a `bool within = true` local doesn't.
- **AppWrap's turnaround:** a new DATA definition in a unit needs that unit built before `game/AppWrap`, else the
  wrapper shows REFERENT; a defined wrapper drops its `_wrap_X` prototype near the method table.
