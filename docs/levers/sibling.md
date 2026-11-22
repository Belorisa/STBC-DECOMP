# The demo build as a second compile

`stbcdemo.exe` (2002-02-09, `tools/sibling.py`) is the same source tree compiled two months before retail with the
same compiler and linker. Paired function by function, it is a natural experiment on which walls the function's own
source decides and which ones its unit's context decides. Read `README.md` for how entries are written.

## The demo build as a second compile: frame and register flips

### The demo build as a second compile: what a flip looks like

- **A context flip is a pairwise exchange, nothing else:** the same instruction sequence and frame size, with two
  spilled locals' frame slots exchanged and the callee-saved registers that load them exchanged with them, or one load
  scheduled one instruction away. GraphicsMenu::ChangeDisplayMode (0x554040: slots 0x20/0x40, EBX/EBP),
  MapWindow::Unknown500F50 (0x500F50: slots 0x18/0x1C, EDI/EBP), MultiplayerWindow::SelectServerHandler (0x506200:
  slots 0x30/0x34 and 0x40/0x44, EDI/EBP), OptimizedSelectTarget's constructor (0x4888A0: one `mov edx, [esp+0x1C]`
  before or after the `add esp, 4` of an operator delete). A diff with other instructions, another frame size or
  another call set is not this wall, even if part of it is a slot swap: Unknown500F50 also calls the NiPointer
  constructor out of line where retail doesn't, and that part is source. `tools/sibling.py flips --all` classes every
  annotated pair; `decomp.flip_kind` reads ours against retail the same way but also masks member displacements.
  {asm: mov edi, dword ptr [esp + 0x18]; mov ebp, dword ptr [esp + 0x1c]}

### The demo build as a second compile: what triggers a flip

- **The trigger is a declaration the unit sees, not the unit's code:** in three of the four flips (GraphicsMenu,
  MapWindow, OptimizedSelectTarget) every other function of the unit, its string literals and its static stubs are
  byte-identical between the builds and the unit spans the same number of bytes; only declarations that emit no
  code in that unit can differ. Across all 907 units, the 29 whose own code changed between the builds hold one
  flip and the 878 unchanged ones hold three (one in four flips by chance would land in a changed unit 25% of the
  time): an edit elsewhere in the unit is not what moves a function. This matches the A/B results in our own tree,
  where the same functions flip when a header gains an inline, a static member or an include and nothing in the unit
  changes (walls.md, "Frame-slot and register order that flips with the declarations a unit sees").
- **Only large functions flip:** of 22,575 paired written functions, 4 flip, all between 1036 and 1770 bytes: 4 of
  the 337 functions of 1000 bytes or more (1.2%), none of the 22,238 smaller ones, although declarations changed
  between the builds in widely included headers (TGNetwork's layout, used by 24 units' 27 large functions). A tie
  between two locals of equal weight needs many spilled locals; a small function's register wall is a spelling
  (locals.md) before it is context. Don't park a function under 1000 bytes on the declaration-sensitive reading
  without an A/B that flips it with a header change alone.
- **The four flip units share an include chain in this tree:** all four reach UtopiaApp.h, UtopiaModule.h,
  TGApplication.h, NiApplication.h, PyEmbed.h and TGEventManager.h (95 of 907 units include UtopiaApp.h; all four
  flips landing there by chance: 1.4%). The binary doesn't show which declaration changed: UtopiaApp's and
  UtopiaModule's constructors are identical in both builds, so it was not their member layout. The include sets are
  this tree's reconstruction, so the chain is a lead, not evidence of the original's includes.

### The demo build as a second compile: what it predicts for open walls

- **Large frame/register walls in that chain are the first context candidates:** when measured, about one open
  register wall in eight read as the same instructions with only frame slots and registers moved, and one in four as
  one schedule shift. The large ones of the first kind are CharacterClass::ReadFromStream (0x66E220),
  OptimizedSelectTarget::OptimizedSelectTarget (0x48A340), OptimizedFireScript::OptimizedFireScript (0x481CC0) and
  NiOBBox::TestIntersection (0x829860); the first three sit in units of the UtopiaApp.h chain and are expected to move
  as those headers gain retail's declarations, not from edits to their bodies. The reading is not proof: two of the
  same reading outside the chain matched from their own statements (NiBezierPatch::CreateSets 0x832C30 from its SDK
  debug copy, lineage.md; NiBezierTriangle3::Unknown83B370). Those three are the NiTListBlock
  operand-order walls (walls.md) that flip per caller: the same context reading fits them. Smaller walls of the same
  reading, and walls in units outside the chain (under a third of them were inside it, against 45% of all written
  functions), are not predicted by the demo: work them as spellings first.

## DamageTool's decorated names as signature evidence

### DamageTool's decorated names as signature evidence: what they settle

- **A DamageTool copy byte-identical to retail's settles the signature, not the body's spelling:** the types, const,
  references and the template behind a function retail can't see (NiTSimpleList<NiPointer<NiProperty> >::Remove is
  DamageTool's 0x438770, byte-identical to retail's 0x498250). A copy that differs (the damage code's Clip, Build,
  SetUpSceneGraph, ProcessTri's parameter counts) is an older revision: take nothing from it without retail's own
  pushes, `ret N` or loads. Making `this` const or non-const can move a body's registers in this tree even though the
  original's const-ness is proven: NiTriStrips::GetTriangleIndices (0x80EF80) matches const and loses its match
  non-const, while DamageTool's byte-identical GetPickInfo is non-const, so its body differs from the original's
  somewhere. Keep a match over such a change and note it at the declaration.
