# Matching decompilation: agent guide

Reconstruct original source so the original compiler, with the original flags, emits object code that matches
the retail binary byte for byte, function by function. Correct structure outranks any score: classes, types,
ownership, storage, control flow, calling conventions and referents.

The point of the project is source a reader can understand. Matching bytes prove the source is faithful; they
don't make it readable. Every function must pass the reader test of the `decomp-code-rules` skill: would a
programmer on the original team have typed this? Source that matches through tricks (a fakematch) is not a
match.

Project: **Star Trek: Bridge Commander** (`stbc.exe`, patch 1.1, 2002-04-09). The skills (`matcher`,
`wall-identifier`, `helper-recovery`, `c2-residue`, `orchestrator`) use the generic verbs defined below.

## Where the rest is

This file holds what every agent needs: the project's commands, statuses, annotations, code rules and profile, and
the rules that always apply. Read the rest when the task needs it:

| read | when |
|---|---|
| `.claude/skills/matcher/references/AGENTS-template.md` | the generic rules in full, under the headings the skills cite as `AGENTS.md` sections: "Objective and scores", "Workflow", "The byte gate" (its blind spots, batch and bisect), "Matching rules that are easy to get wrong", "Source rules", "Data", "Tests and verification", "Repository hygiene". They apply here as written, with the project's additions below. Its "Project adapter" and "Project profile" are blank templates: this file fills them |
| `docs/levers/README.md` | a wall the packet's quoted levers don't explain: the index of what earlier lanes learned, one file per compiler decision |
| `docs/toolchain.md` | a unit doesn't compile, a unit's flags or compiler are in question, setting up or removing a worktree, syncing names into Ghidra, a tool's options |
| `docs/c2-backend.md` | an inline, register or frame wall: the rules VC6's back end follows (inline cost and budget, register priorities, frame packing), read from `C2.DLL` and `C1XX.DLL`, and how to read `tools/c2trace.py`'s output |
| `docs/rulings.md` | relying on a policy below: accepting a lost match or a dip, an invented helper or name, a flag or precompiled-header question, a declaration-sensitive function, what is listed apart, a NetImmerse name or statement: the user's rulings in force and the cases they decided, by topic |
| `docs/evidence.md` | naming a class, member or function, claiming an identity, adopting code from Python, the CRT, GameSpy, Gamebryo or nif.xml |
| the `orchestrator` skill | launching, reviewing or merging lanes, the baseline and the README numbers |

## Project adapter

| verb | meaning | this project's command |
|---|---|---|
| `build` | compile every unit, compare every annotated function | `python tools/decomp.py build` (`-s`: only the changes against the baseline, what a worker reads; `-v`: exact functions too) |
| `match <unit>` | compile and compare one unit (the inner loop) | `python tools/decomp.py match python/Objects/listobject` |
| `diff <function>` | side-by-side disassembly, compiled against retail | `python tools/decomp.py diff PyList_GetItem` (or `0x748EE0`; `--size N`) |
| `report` | refresh the progress report | `python tools/decomp.py report` (orchestrator only: `--update-baseline`) |
| `gate` | the checks that must hold before a commit or merge | `python tools/decomp.py gate`, then `python tools/decomp.py regress` |
| `disasm` / `xref` / `class` | retail assembly, callers and callees, vtables | the `ghidra` MCP tools (`disassemble_function`, `get_xrefs_to`, `decompile_function_by_address`, `list_strings`) |
| `ghidra names` | push the source's names and types into Ghidra | refreshed at every merge; then `ApplyStbcAll.java` in Ghidra |
| `permute <function>` | a permuter | none: the `c2-residue` skill reads the back end's decisions with `c2trace`, then tries hand-written spellings with `trial` |
| `merge <slot>` | the orchestrator's lane merge | `python tools/merge_lane.py merge <slot> -m "merge: lane/x (...)"`; a worker runs `merge_lane.py check` first |
| `packet <function>` | everything a worker needs for one function, in one file | `python tools/packet.py 0x6C4D90 [...]`: read it instead of exploring |
| `c2trace <function>` | the real back end's decisions for one function: an inline, register or frame wall becomes arithmetic (inline sites with cost, budget and verdict; `--regs`, `--frame`, `--cost`, `--whatif`, `--retail`) | `python tools/c2trace.py 0x59A2C0 [--regs] [--frame] [--cost]` (options: `docs/toolchain.md`; rules: `docs/c2-backend.md`) |
| `framemap <function>` | a frame or register wall: retail's frame slots and callee-saved registers beside ours, ours named, with a one-line reading | `python tools/framemap.py 0x712A60` (`--rev <rev>`, `--source <file>`) |
| `similar <function>` | matched functions whose retail code looks alike: worked examples | `python tools/similar.py 0x5AD450` |
| `trial` | fast variant compiles of one function from a preprocessed unit, bounded by the user's ruling (one variant shown to move the output first, at most 20 hand-written variants a run; an exact one is a clue, not an answer) | `python tools/trial.py 0x59D5B0 a.cpp [b.cpp ...] [--show]` |
| `openwork` / `triage` / `phases` | what is open, and who takes it | `python tools/openwork.py class ShipClass`, `triage [Class]`, `phases`, `groups [ID]` (shared walls), `headers [NAME]` (header leads across the cards: changed or only pointed at), `untried [LO HI] [MIN]` |
| `rules [<unit>]` | the rule checker: [match] and advisory findings | `python tools/decomp.py rules [<unit>]` |
| `cards` | each open function's history | `python tools/cards.py show <addr>`; after a try that didn't match, `cards.py add <addr> --by <model> --how "..." [--wall <class>] [--group <id>]` |
| `levers` | what earlier lanes learned | `docs/levers/` (index: `docs/levers/README.md`); `packet` quotes the closest entries |

The report and lead tools (`callset`, `sdkdebug`, `sibling`, `ctxscan`, `dupescan`, `nocall`, `c2il`, `linkcheck`,
`clanglint`, `eaxcheck`, `linkready`, `layoutcheck`, `vtslots`, `advisory`, `names`, `levercheck`, `units`,
`comdat`, `statics`, `annotate-inlines`, `trace`) are in `docs/toolchain.md`, "Report and lead tools"; options and
procedures for every tool are in its "Tools in detail".

A unit is a source path under `src/` without its extension. Only files with at least one annotation are
compiled. Generated output goes to `build/` (ignored): `build/obj/`, `build/report.json`, `build/gate/`.

**Statuses** printed by `build`/`match`:
- `MATCH`: instructions, constants and every relocation target identical; the padding up to the next boundary
  of the object section's own alignment (16 bytes for `/O2` code, 1 for the packed CRT library) is fill. Shown as `MATCH-PENDING` when the rule checker has a [match] finding.
- `REFERENT`: bytes identical, but a referent's identity isn't recorded yet (`unknown referent _Name (retail
  0x...)`). Record it (see "Referents") and it becomes `MATCH`. Not counted until then.
- `WRONG-REFERENT`: bytes identical, but a referent resolves to the wrong address or holds different content.
  A defect.
- `DIFF <score>`: navigation score (instruction-sequence similarity). `NO-SYMBOL`/`NO-OBJ`: an annotation or
  compile problem. Under a `DIFF` row, `wall: <class> (<what differs>)` is the wall-identifier's first routing step
  read from the two disassemblies (`tools/walls.py`): where to look first, not a verdict that the wall is bounded.
- `PATCHED <score>`: a function under a `// PATCHED:` note (Policies): the original, compared with retail's patched
  bytes. Never spliced by the gate; the summary, `openwork` and `rules` list it apart, neither matched nor open.

**Referents.** Every relocation target is resolved, never masked:
- functions and data annotated anywhere in `src/` resolve to their annotated address;
- `config/symbols.txt` lists decorated names that have no source definition yet (`_malloc 0x00718d60  #
  evidence`). An entry is an identity claim. Add it only with evidence (a string, a call pattern, upstream
  source matching the callee), and replace it with an annotation once the definition exists in `src/`;
- unit-local and compiler-generated referents (string literals, `static` data and functions, EH handlers,
  float constants) take retail's address and are then **verified by content**, recursively through their own
  relocations. Only a site whose instruction is retail's own (the same bytes at the same place, the address field
  aside) places such a referent: a function misaligned against retail reads a meaningless address at ours' offset,
  so what it places is undone when its comparison ends. An inline or template COMDAT whose own copy doesn't verify (the linker kept another unit's copy,
  compiled with that module's flags) is accepted when some other unit's copy of the same name verifies at retail's
  address.
- **one datum, one address, across the tree.** Equal bytes don't make the same datum. A compiler-generated constant
  or string (`__real@4@...`, `??_C@...`: a COMDAT, which the linker keeps once per name) has one retail address: the
  one most functions' own instructions read it at (the tree's vote, taken before any comparison). A site reading the
  name anywhere else reads another copy of an equal value (C2's folded reciprocal, a file static, a `.data` global)
  and is `WRONG-REFERENT`, as is a unit's private datum read where an annotated datum or another name's copy lives.
  `decomp.py copies` lists every such datum with its readers. The fix is a spelling that reads retail's datum
  (`docs/levers/x87.md`, "A second copy shared across units is a division by a constant"), never a relaxed rule.
- uninitialised (`.bss`) data has no content to verify, and a unit's `.bss` may still lack statics the
  source doesn't model, so each `.bss` datum is placed on its own: by its `DATA` annotation, else at retail's
  address. One datum must resolve to one address at every use, and two data of a unit never share one.

### Annotations

- `// FUNCTION: STBC 0x<address> [decorated-name]` goes on the line **directly above** the definition (its
  return-type line). Only comment lines may come between. The decorated name is needed only for overloads,
  templates and operators.
- `// DATA: STBC 0x<address> [decorated-name]` goes directly above a global's definition. Initialised data is
  verified by content. A function-local `static` needs its compiled name, which VC6 prefixes with an underscore
  (`_?roots@?1??PathLength@NiPathController@@IBEMIIM@Z@4PAMA`).
- A compiler-generated member (an implicit destructor, its `??_G`) is annotated directly above its class
  definition, and the decorated name is required.
- Annotations may also sit in headers (`.h`, `.hpp`, `.inl`), for functions defined there: each binds to the
  compiled unit whose object defines the symbol. That unit must carry at least one annotation of its own to be
  compiled.
- A `.c` file that other units `#include` (the CRT's `wincrt0.c` includes `crt0.c`) may carry annotations for
  functions only an includer defines: the annotation binds to the including unit, which is compiled for it.
- `@<unit>` at the end of an annotation (`// FUNCTION: STBC 0x0085ad7b @crt/wincrt0`) binds it to that including
  unit explicitly, when the file's own unit compiles a different variant of the same function. In a header it
  names the unit whose copy of an inline or template instance retail kept, when several units emit it. When retail
  kept more than one unit's copy (the linker didn't fold them: `TGStream::ReadBool` at 0x6D2030 and 0x6D3330), each
  copy gets its own annotation with its unit's `@unit`, and each unit's references resolve to its own copy; a unit
  without an annotated copy takes the copy retail names, the same one at every use. Only a COMDAT (inline or
  template) symbol may have several addresses, and one unit annotates it at one.
- `// MATCH-PENDING: <rule>, <reason>` inside a function records a reviewer's verdict the checker can't see (for
  example an unused local kept only to reproduce a frame slot): the checker reports it as a [match] finding.
- `// PARKED: <one-line reason>` directly under a `FUNCTION` annotation marks a bounded stop.
- `// DRAFT: <what is still off>` directly under a `FUNCTION` annotation marks a phase 1 draft: structure from
  evidence, score short of exact, budget spent (orchestrator skill, "Phases"). Like any non-exact function it stays
  out of the gate. `PARKED` replaces it once someone has worked the function and named its wall.
- `// ORIGINAL-ASM` under a `FUNCTION` annotation: the evidence shows the function was written in assembly, or is C
  with an inline `__asm` block of the original's (user's ruling, 2026-11-03: the block stays in the source and the
  function is listed apart, counted neither matched nor open; NetFile::GetFileChecksum's `rol`).

### Policies

The principles. The user's rulings behind them, with the cases each decided, are in `docs/rulings.md`: read its
section before relying on one (a merge acceptance, an invented helper, a flag, a NetImmerse name).

- Ghidra, decompiler included, is allowed for analysis. Its output is evidence, never source to paste.
- Running the game is allowed for behaviour questions.
- Commit messages: `match: <what>`, `cards: <what>` (a commit that only adds card entries), `tools: <what>`,
  `rules: <what>`, `docs: <what>`, `merge: <lane>`. A subject is one line; a merge's list of matches and raised
  scores goes in the body.
- Only the orchestrator updates `config/baseline.json` and the README numbers, at merge. A lane can't change the
  verifier: a hook refuses, inside a slot worktree, writes to the baseline, `tools/decomp.py`, `tools/rules.py`,
  `tools/merge_lane.py`, `orig/` and `build/gate/`. A lane that needs one changed reports it.
- **Lost matches go to the user**, with two standing exceptions accepted at merge and listed. A function in
  `config/declaration_sensitive.txt` (only the user adds one) may flip between its known byte states when its source
  is unchanged. A score dip in an edited, non-matching function is accepted with `--accept` when the change is an
  evidence-backed correction recorded on its card; a dip from a spelling experiment is not.
- **Declaration-count walls**: a function under 1000 bytes is never parked as declaration-sensitive without a
  `ctxscan` run or a header-only A/B that flips it. It may be restored by a real declaration or include only, with
  its evidence in a comment; dummy declarations are a fakematch, and headers are never tuned to hold a function.
- **Invented helpers** are marked `(invented)` at the code and listed at merge. The function a helper closes is
  `MATCH` when a sibling build, a debug copy, the lineage text, the inline `unsigned short` widening idiom or a
  `fld`/`fstp` copy retail has as an instruction of its own shows the helper or the named local existed, and `MATCH-PENDING` when only budget arithmetic, the schedule or the bytes do (the test and
  its cases: `docs/rulings.md`, "Invented helpers and invented text").
- **Names**: a placeholder is named from direct evidence or lineage. Behaviour-derived names wait until the tree
  matches; before then one is written only where it actively helps open work, marked `(name invented)` and proven
  byte-neutral with `c2il` and the gate.
- **Listed apart**, counted neither matched nor open and recorded in `config/exceptions.md`: `PATCHED` (binary-patched
  in retail, written as the original), `ORIGINAL-ASM` (assembly sources, CRT `.asm` routines included; no assembler
  is added), `IMPORT-THUNK`, `LIBRARY-DATA`.
- **Build flags are the user's**: game units build without a precompiled header, NetImmerse with `/YX`. A unit gets
  its own `/YX` or `/Ob2` only when both its matches and its retail section order (`comdat.py order`) improve, the
  evidence recorded beside the entry in `config/project.json`.
- **NetImmerse evidence**: the SDK's debug build (`tools/sdkdebug.py`) proves names, files and statements; the
  Gamebryo lineage text is followed, and where retail's bytes disagree with it the bytes win, with a comment at the
  code naming the functions that decide it.
- **Source is never shaped for the tooling's convenience**: not to avoid a `_$E` renumber, not to hold a
  declaration-sensitive match. A unit's own definitions follow retail's address order; a helper retail places at its
  definition has no `static`; `@unit` names the unit whose copy retail kept, strictly.
- **A constant retail keeps in two copies is two spellings**, chased in the source, never accepted by relaxing the
  referent rule.
- **Fast variant trials are bounded**: one variant first shown to move the output, then at most about 20
  hand-written variants per function, never generated mutations. A result is a clue; the spelling still needs
  evidence.
- **Cards** (`cards/<addr>.jsonl`) are a data record, not documentation. The merge writes their score entries;
  workers append tries with `tools/cards.py add`, never by editing old lines. A try that changes or A/B's a shared
  header carries `--group hdr-<slug>` on every card it moved, and names the header file.
- **The levers are audited every 9 lane merges** (`tools/levercheck.py due`).

## Rules that always apply

These are the generic rules' load-bearing lines (the template has them in full).

- **The byte gate decides.** A function is done when the gate, compiling every unit afresh, finds its bytes and
  every relocation target identical to retail's, and the retail image with every such function spliced in still
  hashes to the retail hash. Nothing else counts: not "looks equivalent", not a local diff of zero, not a closeness
  score, not an agent's verdict. Unmatched or non-matching source never enters the gated build. Write "matched" or
  "banked" only from the gate's printed success line. What that line does not prove is below.
- **Commit matched work the moment it exists**, and never gate on a dirty tree (other than tool-database churn).
  Stage only your own unit of work, and never commit build state.
- **Levers are disposable experiments.** Never keep probes, unused declarations, fake locals, `volatile` carriers,
  padding or distorted source to move a score. A trick that closes a gap is a clue to the real source, not the
  answer. A dip is not a rejection: keep a change that moves codegen toward retail's shape.
- **Keep every exact match, and count it honestly**: a style rule the bytes defeat gets a `RULE-EXCEPTION` and the
  function counts; a [match] rule makes it `MATCH-PENDING`, kept and listed as open work.
- **`historical > best`** in `config/baseline.json` is a lost match: recover it from version control before
  inventing anything new.
- **One class, one definition**, in a shared header; each function and global in its evidence-backed owner; access
  through typed members, never offset casts. Unclear identity: chase the evidence, else leave an identity-TODO,
  never fabricate. An invented name is marked as invented.
- **The original's bugs are kept and labelled, not fixed.** This branch is the byte-exact preservation build; fixes
  belong on a separate branch later (user's ruling, 2026-10-30). A statement that is the original's own bug carries a
  `// BUG: <what is wrong>` note on its line or in the comment block directly above it, and the rule checker then
  doesn't count it as fake code (R17). The note must describe a real defect that retail's bytes show; it is never
  a way to excuse a reconstruction guess (an empty stand-in loop, an unexplained local stays `MATCH-PENDING`).
  Redundant code of the original counts the same way (user's ruling, 2026-11-08): when retail's bytes contain the
  statement's own instruction (a store, a call, a test), the statement is the original's, and it carries
  `// BUG: redundant: <what it repeats>`. A statement that only steers the compiler toward a branch shape, with no
  instruction of its own in retail, stays R17 fake code. The checker can't tell a real BUG note from an excuse, so
  an empty note excuses nothing, and `merge_lane.py` lists every new BUG and RULE-EXCEPTION note, and every
  removed MATCH-PENDING note, for the orchestrator to read before merging.
- **Pick work from the notes first**: `tools/packet.py` for one function (it quotes the closest levers),
  `tools/openwork.py` for what a class or address range still needs, and the section of `docs/levers/` the packet
  points to when its quotes don't explain the wall.

## What a MATCH does not prove

The compare and the gate check bytes and relocation targets. That rules out a missing branch, a simplified
expression or a wrong callee: "similar code with a shortcut" is a DIFF. Three things stay outside it, and each needs
its own evidence:

- **The original spelling.** Many sources compile to the same bytes. A local added only to move a load, a helper
  invented to spend the inline budget, a `friend` or a cast added to reach a member, a flag chosen because it
  matched: all produce a MATCH. The rule checker catches a few patterns of fake code (R17), not intent. The
  reader test ("would the original team have typed this?") and the evidence behind each lever decide; a lever that
  closes a gap is a clue to the original's spelling, and its evidence goes with it.
- **What the bytes don't see.** Same-sized types (a `TGObject *` parameter against an `ObjectClass *`, `int`
  against an enum), which class owns a method, and the identity claims of `config/symbols.txt` can be wrong in a
  matching function. Import claims are checked against the import table; other claims rest on the evidence written
  beside them.
- **That the source is a program.** Nothing is linked: the gate splices the exact functions into a copy of retail.
  It doesn't prove that the units link together, that `.bss` data the source declares lands where retail's does
  (it is placed at retail's address), or that the tree builds a working executable on its own.
- Matching work runs no test suites: comparing against retail is the verification. Tooling changes run the touched
  tool's own tests.
- Docs describe current usage and contracts only: no diaries, score snapshots or campaign logs. Source comments
  explain the code, never its history.

## Code rules

The two laws and the [match] rules are the template's "Code rules" section (the catalogue is the user-level
`decomp-code-rules` skill, `references/rules.md`): the bytes win, with a `// RULE-EXCEPTION: <rule>, <reason>` where a
style rule loses to them; and a function counts as matched only when its bytes match *and* it passes R1, R2, R4,
R13, R17 and R18, else it is `MATCH-PENDING`. Naming (R7) is not required for a match.

Progress reports two numbers: **matched under the rules** (the headline) and **bytes matched**. Both count written
functions. Compiler-generated `_$E` static initialisers (the `<iostream>` quads, handler tables, the NetImmerse
headers' class registrations) are counted apart, on a line of their own, and `.text` from source includes their bytes
(user's ruling, 2026-11-09).

### Project profile

| item | this project |
|---|---|
| compiler dialect (R22) | VC6: C++98 with VC6 quirks (`for`-init variables leak into the enclosing scope, no partial template specialisation, no `long long`: use `__int64`, `bool` in C++ only). C units are C89 plus `//` comments: no designated initialisers, no compound literals, no declarations after statements. |
| naming style (R8) | **Game and TG framework (C++):** `style-defaults.md` Table 2 (id Tech 4), by the user's ruling of 2026-10-26, with evidence first. Class and method names are exactly as the SWIG strings give them (`ShipClass::GetAlertLevel`, `TGObject`). There is no `id` class marker: classes keep their evidenced names, and classes without evidence are `PascalCase`. Members and globals are `lowerCamel`, with no `m_` and no Hungarian; constants are `UPPER_SNAKE`; enum types are `lowerCamel_t`. The Hungarian prefixes in the Python scripts are script-side evidence only. **Python 1.5.2:** upstream style, unchanged. **CRT:** the VC6 CRT source style. **NetImmerse:** class names as the engine's strings give them (`Ni` prefix); everything else follows the game's default above (Table 2: `lowerCamel` members, no `m_`, no Hungarian), by the user's ruling of 2026-10-26. |
| layout style (R23, R32, R33) | Game: tabs (width 4); `if ( cond ) {` with spaces inside the parentheses; opening braces on the same line, for functions too; banner `/*` `=====` `Class::Method` *description* `=====` `*/`; a header comment per function (compiler-generated members and members defined on one line inside their class need none: user's ruling, 2026-11-17). Python and CRT keep their upstream layout, and the checker doesn't ask them for header comments. |
| placeholder patterns (R7) | `FUN_`, `DAT_`, `PTR_`, `LAB_`, `sub_`, `unk_`, `local_`, `param_`, `field_0x`, `m_Unk`, `UnkType_`, `iVar\d`, `uVar\d`, `var\d+`, and address-like identifiers. The project's spelling for evidence-free names: member `unknown<OFFSET>` (`unknown1C`), virtual slot `VirtualSlot<NN>`, function `<Class>::Unknown<ADDRESS>`. Each one is open work. |
| evidence sources for names (R7) | SWIG wrapper strings; error and log strings; the Python 1.5.2 and CRT sources; the scripts; NetImmerse strings. |
| line information (R23) | None: no PDB shipped, and no `__FILE__` paths or line-number asserts in game code. Layout is [free]. |
| parking mechanism | A function stays annotated; non-exact functions never enter the gate. `// PARKED: <reason>` marks a bounded stop. There is no `#ifdef` parking. |
| rule changes | **Surviving upstream source is exempt from R2, R4 and R17 line by line** (user's rulings, 2026-10-26/27): a finding on a line identical to the committed pristine upstream is skipped, because unchanged surviving source is authentic; changed lines (TG modifications, service-pack revisions) stay fully checked. Pristine trees: `upstream/python-1.5.2/` for `src/python`, `upstream/vc6-crt/` for `src/crt` (mirrored layout, `"upstream"`), and the SWIG 1.3.6 + r1429 runtime templates in `upstream/swig-1.3.6-r1429/` for the generated `game/AppWrap` (`"upstream_units"`); the rules are listed in `"upstream_exempt"`. This replaces the earlier module-wide R2 relaxation for `src/python`. Generated wrappers carry no hand-written header comments (R33 advisory off for `game/AppWrap`). Game and NetImmerse code have no exemptions. |

### Ledgers

- **Exceptions and pending matches:** `config/exceptions.md` (function, rule, deviation, status).
- **Score baseline:** `config/baseline.json` (current score, best for the current source, historical best).
- **Function cards:** `cards/<addr>.jsonl` (each open function's score moves and recorded tries).
- **Referent identities without source:** `config/symbols.txt`.
