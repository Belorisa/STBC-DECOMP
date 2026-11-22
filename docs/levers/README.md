# Levers and walls

What earlier lanes learned about this compiler and this code, one file per compiler decision. Start from the
function's packet (`tools/packet.py`): it quotes the entries closest to the function's note. Open a file below
when the quotes don't explain the wall, by the wall class `build` prints under the DIFF row. [triage.md](triage.md)
gives, per wall class, the order to check things in and when to stop and park.

| file | wall class | what it covers |
|---|---|---|
| [triage.md](triage.md) | all | Per wall class, what to check first, linked to the entries below, and when to park |
| [inline.md](inline.md) | `inline` | Which calls VC6 expands and which it leaves out of line |
| [control-flow.md](control-flow.md) | `control-flow` | Branch order, early returns, shared tails, loops and switches |
| [locals.md](locals.md) | `register` | Named locals, declaration order and evaluation order: what moves loads, frame slots and register choices |
| [x87.md](x87.md) | `register` | Operand order on the FPU stack, and whether a float is copied through the FPU or with integer moves |
| [data.md](data.md) | `referent` | Float constants, string pools, IAT slots, COMDAT copies and identity claims |
| [statics.md](statics.md) | `referent` | `_$E` numbering, registration stubs, guard bytes, the inline flag a run's shape names, and what the static layout says about unit boundaries |
| [class-model.md](class-model.md) | `structure` | Vtables, constructors and destructors, member and parameter types read from the code |
| [lineage.md](lineage.md) |  | What the Gamebryo 1.1.2 SDK headers, the 1.2.2 source and the SDK's debug NetImmerse 3.1 (AnimOnly) give, and which compiler builds which units |
| [crt.md](crt.md) |  | The C runtime, the C++ EH runtime and the floating-point library: what the VC6 sources lack |
| [sibling.md](sibling.md) | `register` | The demo build as a second compile: what a context flip looks like, what triggers it, which open walls it predicts |
| [walls.md](walls.md) |  | Walls to park on: try the named levers once, then stop and record the wall on the function's card |

## How entries are written

- One entry per fact: the spelling or the reading, then one or two example functions that proved it. No scores,
  lane names, dates or history. An example marked *(open)* moved toward retail without closing yet.
- A fact that already has an entry is added to it, never written again elsewhere. File it under the compiler
  decision it moves, not the lane that found it.
- A lever that adds code or changes a flag (a `friend`, a cast, an extension, a per-unit option) says what
  evidence it needs: *Evidence required:*. A trick that closes a gap is a clue to the original's spelling, not the
  spelling (`AGENTS.md`, "What a MATCH does not prove").
- Open work goes on the function's card (`tools/cards.py add`), not here. Entries that stop applying move to
  `docs/levers-archive.md` with the reason, which nobody needs to read.
- An entry may end in an instruction key, `{asm: test; je; jmp top}`: the pattern its wall shows in a diff, one
  instruction per `;`, mnemonic first; of the other words only registers (x87's as `st(1)`), numbers and size words
  are compared (`top` stands for any operand). A number keeps its sign and is compared by its 32-bit value: `-1`
  matches `-1` and `0xffffffff`, not `1`; `[esp - 4]` reads `-4`. `tools/packet.py` quotes the entries whose key
  occurs in the function's diff (on consecutive instructions, at least one of them on a row that differs) a different
  number of times on the two sides first, marked "asm key in this diff", then the entries closest by text: a pattern
  both sides have as often (an `fld; fstp` copy moved by the residue) doesn't tell them apart.
- An entry that names a lever tried elsewhere without effect ends its text with "Didn't help in: ..." and the
  functions, so a worker doesn't repeat the try.
- Entries sit under the `###` subheading of the topic they belong to, inside their `##` section.
- Write `st`, not `st(1)`, in a key: a word ending in a parenthesis is never found in the diff.
- The orchestrator adds each lane's findings at merge. `tools/fncontext.py` (the packet's closest levers) reads
  these files by their headings and their `- ` entries: keep the headings. A `###` subheading starts with its `##`
  section's name ( `### Control flow: loops`), because the ranking reads an entry's section by its name. Lists
  outside entries (triage.md) are numbered, not bulleted, so the tools don't read them as entries.

## House habits, measured

`python tools/habits.py` counts paired spellings in matched code, per module. The leading spelling is the one to try
first; the bytes still decide. The game (Totally Games) and NetImmerse (NDL) differ in places:

| habit | game | netimmerse | python | crt |
|---|---|---|---|---|
| pointer test | `p == NULL` (85%) | `!p` / `p` (92%) | `p == NULL` (80%) | `p == NULL` (69%) |
| `this` in a comparison | `x == this` (95%) | `this == x` (few cases) | | |
| else-if chain vs separate `if`s that return | separate (73%) | separate (67%) | separate (71%) | even |
| `&&` in one condition vs nested `if`s | `&&` (75%) | `&&` (61%) | `&&` (65%) | nested (65%) |
| increment | `i++` (98%) | `i++` (100%) | `i++` (81%) | `i++` (80%) |
| `x += y` vs `x = x + y` | `+=` (84%) | mixed: `+=` 58% | `+=` (92%) | `+=` (95%) |
| exits per function | one (76%) | one (86%) | several (63%) | one (58%) |
Two more habits seen in matches, not counted by the scan:

- **The same guard repeated on each path** instead of one shared `return false`:
  `if ( !f ) { f = ...; if ( f ) return f(...); } if ( f ) return f(...); return false;`
  (NiBoundingVolume::TestIntersect and FindIntersect).
- **Field-by-field copies** of points and colors (TGPoint3::MultMatrix).
- **SWIG argument defaults:** `"OO|i"` is `int arg2 = (int)0`, and the prologue's zero stores are the defaults;
  `"|Oi"` is `PyObject *argo3 = 0; int tempbool4 = (int)false;`, with `ConvertPtr` still run on a NULL argo. Zero
  stores then one store of a constant address are a default like `NiColorA *arg9 = &g_kInterfaceBorderColor;`.

## Evidence sources, in order of trust

1. SWIG wrapper strings (`Class_Method`), the App constant table and variable getters (`tools/swig/*.json`). A
   wrapper's body shows what an inline it calls does (`_wrap_TGRect_IsEmpty` at 0x5CC320).
2. Registration, log and error strings; viewer strings (`m_iIndex` → `index`).
3. Retail's own data: vtables (`config/vtables.txt`), tables such as the exception list, RTTI names.
4. Gamebryo lineage for NetImmerse (1.1.2 SDK `C:\Recomp\gamebryo-1.1.2`, 1.2.2 source `C:\Recomp\gamebryo-1.2.2`, per-lane maps from `C:\Recomp\gamebryo-1.2.2-lanes\map.py`); id Tech 4 style for the game's invented names, marked invented.
5. SWIG inline wrappers: the `swig_types` index a wrapper loads (`dword [0x991068 + 4*i]`, swig_types[0] at 0x991068)
   gives an inline accessor's return type, and `tools/swig/classmap.json` "inline" offsets with the swig_types index
   each wrapper pushes give member names and types (TacticalControlWindow).
6. Lineage source for image code: the TGA reader is netpbm's tgatoppm and the SGI reader the classic SGI
   expandrow/getbyte family; check such public code first for other format readers.
