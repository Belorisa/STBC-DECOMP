---
name: c2-residue
description: Close a function that is complete but below 100% by reading the decisions of VC6's back end (C2.DLL build 8966) instead of guessing spellings. Trace the deciding site with tools/c2trace.py (inline budget, register priorities, frame slots, operand order, two-register addresses, schedule), write what retail needed as arithmetic, find the evidence-backed spelling worth exactly that, and check it with bounded hand-written trials (tools/trial.py). Use once wall-identifier has routed a function to register/schedule, or to inline/call set with the body complete; when a near-miss resists the levers; or before parking a wall as bounded. This project has no permuter.
---

# c2-residue: ask the compiler, then find the spelling

Project guide: the repository's `AGENTS.md` (commands, rulings, statuses). The rules behind every trace are in
`docs/c2-backend.md`; this skill is the order to use them in.

This pass comes after reconstruction and wall classification (`matcher`, `wall-identifier`). The body is complete:
its calls, its branch skeleton and its referents already agree with retail's, or the only call that differs is one
inline site expanded on one side and called on the other. A function whose control flow or referents still differ
goes back to those passes first.

The back end is deterministic. A residue is a decision it took differently for our source than for the original:
a site that fit the inline budget, a register candidate with a higher priority, a slot packed in another order, a
commutative operand sorted first, an instruction scheduled one place earlier. The trace shows that decision and the
numbers behind it, so the question becomes "what in the original source made this number different", not "which
spelling scores higher".

## 1. Trace the deciding site

Pick the trace from the first real divergence in the diff (the packet's "What the diff shows" names its kind):

| first divergence | trace | rules (`docs/c2-backend.md`) |
|---|---|---|
| a call inlined on one side, called on the other | `python tools/c2trace.py <addr>`: each site in visiting order, its cost, the budget left, the verdict; `--retail` marks the callees whose out-of-line count differs from retail's | "Inline expansion" |
| what a statement costs, what a site would need | `--cost` (IL tuples per statement), `--cost --worth N` (the statements worth exactly N); `--whatif site=K,+1\|-1` or `own=+N` replays the budget with a site or a cost changed, without compiling | "Cost, candidates and budget" |
| a value in another callee-saved register, ESI/EDI swapped | `tools/framemap.py <addr>` first (frame or scratch?), then `--regs`: the candidates by name, their priorities, the choice | "Registers" |
| frame slots in another order or size | `framemap`, then `--frame`: the packer's locals, slots and offsets | "Frame layout" |
| the two operands of an add, multiply or compare swapped | `--operands`: each comparison's operands, weights and winner | "Operand order of commutative operations" |
| `[base+index]` with base and index swapped | `--addr`: which operand became the base, the hashes and the symbol-number shifts that flip it | "Base and index of a two-register address" |
| the same instructions, one scheduled elsewhere | `--sched`: per region the input order, each pick with its priority and ready list | "Scheduling" |
| an x87 load or store copied or kept | `python tools/c2float.py <unit> <function> --il`: why the float is copied through the FPU or integer moves, and the statement tuples after inlining | "Float copies" |
| a frame or register flip that moves with the declarations the unit sees | `python tools/ctxscan.py <addr>` (a report: it never edits `src/`) | `docs/levers/walls.md` |

A scratch `.cpp` can be traced the same way (`c2trace.py C:/scratch/x.cpp Name --flags "/O2 /GX"`); a header can be
swapped for a scratch copy with `--header-override` without touching `src/`. To check that a readability edit is
byte-neutral, `python tools/c2il.py compare <unit|addr> A [B]` says whether two spellings give identical IL.

## 2. Write the need as a number

Before changing any source, write on the card what retail needed at the deciding site, as the trace's arithmetic:
"site 3 needs a share under 66: 3 fewer tuples before it, or one more site after it"; "ESI goes to `node` when its
priority beats `count`'s 14: one more use inside the loop". Record it with
`python tools/cards.py add <addr> --by <model> --how "<the need>" --wall <inline|register|...>`.

The number narrows the search to spellings worth exactly that much. A spelling with no such effect is not tried.

## 3. Find the spelling the original had

Look for a construct that has its own evidence and is worth the number:

- the class's real accessor or helper, called where retail calls it elsewhere (`python tools/nocall.py`,
  `python tools/callset.py fn <addr>`), or an inline body in its header that differs from retail's (`callset`);
- the Gamebryo 1.1.2 lineage text of a NetImmerse body (`docs/levers/lineage.md`), or the upstream text of a Python,
  CRT or SWIG body;
- a habit the matched code of the module shows (`python tools/habits.py`), or the closest matched functions
  (`python tools/similar.py <addr>`);
- a type, width or lifetime that the callers and the data show (a member modelled with the wrong type moves
  register priorities too);
- a real declaration or include the original plausibly had, for a declaration-count function: only under
  AGENTS.md's ruling on declaration-count functions, with its evidence in a comment, and listed at merge.

Never: a statement, local, block, cast or parenthesis added only to move the number (R17 fake code); dummy
declarations; `volatile`; a body written out by hand where retail calls the helper. A trick that reaches the number
is a clue to the construct above, not the answer.

## 4. Check with bounded trials

`python tools/trial.py <addr> a.cpp` compiles a hand-written variant of the whole definition in about 0.3 s. The
user's bounds hold and the tool enforces them: one variant first, shown to move the output; then at most 20
hand-written variants a run; no generated mutations. An exact variant is a clue: make the change in `src/`, then
`python tools/decomp.py match <unit>` decides. Delete scratch variants afterwards.

## 5. Stop and park

Stop when the trace shows that no evidence-backed construct is worth the number, or the deciding quantity sits
outside the function (another unit's headers, the symbol numbering). Then:

- keep the most faithful source, not the highest-scoring one;
- put `// PARKED: <the wall and the number>` under the annotation ("inline budget: site 3 needs 4 fewer tuples; no
  accessor of the class is worth it");
- add the card entry with `--wall`;
- a wall described as declaration-sensitive needs a `ctxscan` run or a header-only A/B that flips it first (for a
  function under 1000 bytes), and only the user adds a function to `config/declaration_sensitive.txt`.

Record a reusable back-end mechanism in `docs/c2-backend.md` or the right `docs/levers/` file, with the A/B that
shows it. No campaign logs.
