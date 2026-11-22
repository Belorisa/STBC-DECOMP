---
name: orchestrator
description: Run a matching-decompilation campaign with parallel workers. Pin the project's code-rules profile, pick disjoint lanes, launch matcher workers in separate worktrees, run each lane through plain matching, helper recovery, structural and finishing passes, review results against the decomp-code-rules reader test, and merge every result into the main branch through a fixed merge procedure (rebase, build, gate, rule check, merge). Use when asked to start, steer or merge matching workers, to push a module or score band toward 100%, to clear MATCH-PENDING fakematches or run a readability sweep, or to integrate agent branches.
---

# orchestrator: run the workers, merge the results

Project guide: the repository's `AGENTS.md`. If the repository has none, use [AGENTS-template.md](../matcher/references/AGENTS-template.md) and ask the user to fill in its project adapter.

You plan lanes, launch workers, relay rulings and merge. Workers do the matching, guided by:
- `AGENTS.md`;
- the `matcher` and `helper-recovery` skills;
- `decomp-code-rules`.

Use `wall-identifier` when a function is stuck, when triaging plateaus, or when asked "why won't this match".

**What the campaign produces is readable source.** The headline number is **matched under the rules**: functions
whose bytes match *and* that pass every [match] rule. Bytes-matched is the second number. Report both, every
time. `MATCH-PENDING` functions (exact but failing a rule, usually a fakematch) are open work, not progress.

Keep the user informed with short, concrete reports:
- functions closed;
- both score numbers;
- fakematches found and cleared;
- what is open and why.

## Before the first lane: pin the profile

Workers write to the project profile in `AGENTS.md` (`decomp-code-rules`, "Project profile"). Make sure it states:
- **Dialect:** what the compiler accepts and rejects.
- **Naming and layout style, per module:** from evidence when there is any. Otherwise propose a default from
  `decomp-code-rules`' `style-defaults.md` and get the user's ruling.
- **Placeholder patterns.**
- **The `rules` command,** which runs the rule checker.
- **Line information:** whether the binary records source lines, which makes layout changes [checked].
- **The parking mechanism** for unfinished functions.

Style is a project decision, not a worker's. Never let parallel workers each pick their own style: their output
would mix styles, and that is worse than either one. Relay the ruling to every worker, and record it in
`AGENTS.md`.

Ensure that you use the correct compiler. If the user is unsure what the correct compiler is, inspect the binary yourself and list likely options.

If the user doesn't have the compiler installed, instruct them to look on Internet Archive. For console games, inform the user that it's likely the compiler came bundled with the SDK.

Before beginning matching lanes, identify any tooling that would dramatically improve matching time and efficiency. After every 3 lanes, check again to see if there is any tooling that should be built for efficiency. Write performance-critical inner loops in C, measured first (profile where the time goes before rewriting anything), and keep the rest of the tooling in Python.

## Slots and branches

- **Worktrees:** each worker gets its own git worktree with its own `build/` directory. Reuse a small fixed set of
  worktrees (for example three), parked at the main branch between lanes. Add more only when the user asks, and
  remove extras after their merge.
- **Branches:** lane branches stay local until their merge. Delete them afterwards.
- **Preparing a slot:**
  1. Create the lane branch from the main branch.
  2. Run one `build`; a fresh worktree may copy `build/` from a built checkout first.
  3. Check that the build tools work in worktrees: some wrappers have hard-coded paths.

## Lanes

- **Lanes are disjoint.** Split them by module, unit or score band, never overlapping another running lane, and
  never two writers on the same files or shared headers. List the lane's functions (address, name, score, rule
  status) in the prompt.
- **Renames cross units.** A rename of a type, member, global or shared function touches every user of that name.
  A naming or type-recovery lane owns the shared header it renames. Run such lanes alone, or after the lanes that
  include that header have merged.
- **Good lanes:**
  - units close to complete;
  - the lowest-scoring band;
  - lost matches;
  - **`MATCH-PENDING` functions:** the bytes are exact and the source is fake. Each one is a helper-recovery or
    structural problem with a known answer key;
  - a recorded to-do;
  - a helper-recovery sweep over sampled functions;
  - **a finishing sweep** of a unit whose functions all match:
    - banners and names from evidence;
    - named constants;
    - documented structs;
    - quirk labels.

    All of these are [free] or [checked] cleanups (`matcher`, "Finish the source").

## Phases

The campaign runs breadth first, in three phases (user's ruling, 2026-11-09). `python tools/openwork.py phases`
shows where each stands. Every lane prompt names its phase; the `lane-worker` agent sets its budget from it.

1. **Draft.** Cover the code without source (`openwork range` gaps, `openwork ready` claims), in Sonnet lanes by
   class or address range. The bar is the structure (types, members, vtables and callees from evidence or an
   identity-TODO, no offset casts) at around 80%, in at most three tries; the rest gets `// DRAFT:`. An exact
   match is banked as always. Already-open functions wait for phases 2 and 3. Plan a class's constructor
   together with its vtable: the vptr store resolves only once every overridden slot is declared, and that class
   work, not the body, is where a constructor's budget goes.
2. **Refine.** Cut the drafts into small groups linked by what they share (a header, an inline helper, a caller
   chain, the same diff shape: `tools/similar.py`), and push each group toward 100% in a few tries per function. A
   fix to the shared piece moves the whole group. `openwork drafts [MIN]` is the queue.
3. **Walls.** The near-misses left (`openwork parked 95`), with as much effort as each needs, mostly in Opus lanes.
   `openwork walls` counts the open functions by wall class and `openwork walls <class> [MIN]` lists one class:
   plan a lane per class (one lever family), and read the class as where to look first, not as a verdict.

**Cards.** Each open function has a card, `cards/<addr>.jsonl` (`tools/cards.py`): the score moves of every merge,
written by `merge_lane.py merge`, and the workers' tries, one line each, written with `cards.py add`. The packet
shows it, and a worker reads it before repeating a try. Cards are a data record like `config/baseline.json`, not
documentation (user's ruling, 2026-11-09). Plan phase 2 and 3 lanes from them: the tries spent, the walls
diagnosed, and the functions that keep moving without closing.

## Worker prompt

Launch a subagent or a separate session in the slot. The prompt holds only:
- the slot path and branch;
- "read `AGENTS.md` first; it takes precedence";
- the skill to start with;
- the goal and the lane's function list;
- what the other lanes own, including shared headers and names that are off-limits;
- the working rules:
  - do the work yourself, and commit on the branch as you go; never spawn sub-agents, forks or workflows (user's
    instruction, 2026-11-16: a worker's own agents exhausted the usage limit mid-round);
  - no push or PR;
  - write source that passes `decomp-code-rules`; a fakematch is reported as open, not as matched;
  - run the project's pre-merge check (`tools/merge_lane.py check`) once, before the final report, and quote its
    lines;
  - report per function the fix, the rule status and what is left, plus any lever or wall worth adding to
    `docs/levers/` (the file of its compiler decision; open work goes on the function's card).

Do not add per-step verification requirements to a worker prompt; the checks run once, at merge.

**Model per lane.** Launch every worker as `subagent_type: "lane-worker"` (`.claude/agents/lane-worker.md`,
Sonnet), never as `general-purpose`. The byte gate and the rule checker make a wrong answer harmless, so Sonnet is
the default. Pass `model: "opus"` only for a lane whose main work is class and layout recovery (vtables, members,
ownership), shared-header changes or a structural wall; say why in the lane plan. Merge reviews stay in the
orchestrator session.

Plan Sonnet lanes from `python tools/openwork.py ready`: it lists, per class, the claimed functions whose callees all
have an identity (ready) and those that call unidentified code (blocked). Give a Sonnet lane the classes with the most
ready functions; give blocked clusters (the unidentified callees they share) to an Opus lane to identify. The
`lane-worker` agent itself skips blocked functions and caps each function at five attempts.

Sonnet does well writing new units and new functions for classes whose layout is already recovered, and on
generated code. It does poorly on sweeps that retry parked functions with known levers: it gives up early with
nothing closed. Give those sweeps to an Opus lane or skip them.

Judge the split by the measured cost per matched function (tokens, Sonnet counted at its relative price), not by
habit. Once the ready lists are down to leftovers (large bodies, FPU walls, half-modelled classes), Sonnet's cost per
function rises above Opus's on structural recovery, and Opus lanes that recover a class and then write it whole are
the cheaper way forward.

A Sonnet lane that hits a structural question stops and reports it rather than guessing; the orchestrator hands the
question to an Opus lane.

## Passes

1. **Plain matching** (`matcher`).
2. **Helper recovery.** When the worker returns with walls or `MATCH-PENDING` functions, merge its work. Then
   resume the same worker (it keeps its context) on the open functions with `helper-recovery`: inline helpers,
   accessors, macros, scoped blocks. Most fakematches hide a missing helper or a merged variable.
3. **Structural pass.** If it is still stuck, resume it for a structural pass: types and ownership, API level,
   function boundaries, control structure, surviving sources. Not permutation.
4. **Finishing pass.** For the lane's functions that are now matched, resume the worker on `matcher`, "Finish the
   source": rule checker, reader test, then the [free] finishing rules in their own commit.
5. **Stop** when the worker reports no open structural lead. Before calling a wall bounded, check that the
   deciding site was traced (`c2-residue`) and its number written on the card; a few flat spellings prove nothing.

## Merging a lane

In the lane's slot:

1. **Rebase onto the main branch.**
   - Conflicts in generated files (reports, the README's numbers): take main's side and regenerate.
   - Conflicts in the score ledger: merge per row, so each row comes from the side that actually achieved it.
     Never take either side wholesale; that drops achieved scores or revives stale ones.
   - After rebasing, confirm that the lane's reported scores are still there.
   - If another merge already landed the same fix, drop the duplicate commit, or cherry-pick only the lane's
     unique commits.
2. **Gate against the main branch's baseline before updating the baseline.** After an update, the gate would
   compare the lane with itself and could not see a drop.
   - A real regression blocks: an *edited* function whose score fell, or **any** function that lost an exact
     match, edited or not. An unedited function can lose its match through a header, an include or a referent
     that another unit changed, and nothing else would notice. Score dips in unchanged functions that were not
     exact are informational. The user can accept a lost match explicitly (a wrong claim corrected elsewhere
     that shows an old match was false).
   - A regression is fixed in the lane, or the user adjudicates it before the merge.
   - Formatting-only and naming commits are gated too: `__LINE__`, line information, stringized names and RTTI
     can move bytes (R23, R7).
3. **Run the byte gate from a clean rebuild** of the rebased lane (`AGENTS.md`, "The byte gate"). Batch the
   lane's matches into one gate run; on red, drop the units the byte attribution names, or bisect, then re-gate.
   Report banked counts only from the gate's printed success line.
4. **Run `rules`** over the lane's units, and compare against main.
   - A function the lane reports as matched but that the checker flags is `MATCH-PENDING`: correct the report.
   - New findings in functions the lane didn't touch mean a shared header change broke them. Fix that in the
     lane.
5. **Review the diff against the reader test.** Read a sample of the lane's new and changed functions,
   prioritizing jumps to 100%. Look for Section F patterns the checker can't see:
   - merged variables;
   - temporaries with no meaning;
   - confident invented names;
   - quirk labels (R36) without evidence;
   - `RULE-EXCEPTION`s that excuse fake code.

   A lane that changes `tools/` or `config/project.json` merges only with `--allow-tooling`: read that diff first,
   since the lane's own tools and flags then judge it. A lane never changes `config/baseline.json`.

   Read every new `// BUG:` and `RULE-EXCEPTION` note, and every removed `MATCH-PENDING` note, in full:
   `merge_lane.py` lists each one with its file and line. A BUG note silences R17 on its statement, so it must name a defect retail's code shows (or, for
   `redundant:`, the instruction of its own the statement has in retail); a note that only explains why a
   statement helps the bytes is fake code under a label.

   Send fakematches back to the worker as `MATCH-PENDING`. Never rewrite them during the merge.
6. **Update the baseline and the reports** with both numbers, and commit them.
7. **Merge** (squash or PR, per project policy), listing each function's change and rule status, then park the
   slot.

Never commit or build in a half-finished rebase: check `git status` for unmerged paths before every build.

## Ledgers and rulings

- **Exceptions and pending matches:** a byte-exact function is always kept. It is never thrown away because of a
  rule. How it counts depends on which rule it breaks:
  - **A style rule the bytes defeat:** the function counts as matched. It carries a
    `// RULE-EXCEPTION: <rule>, <reason>` at the code, and the project's exception record lists it.
  - **A [match] rule:** the function is `MATCH-PENDING`. The source is kept as the best-known state, and the
    function goes to the open-work list for a helper-recovery or structural lane. It doesn't enter the matched
    count. If the gate needs an allow entry to admit its bytes, record that the entry is for a pending match.
  - **Original assembly:** a function the evidence shows was written in assembly is `ORIGINAL-ASM`, listed apart
    and counted neither way.

  Never leave such a decision unrecorded.
- **Rule changes:** tightening or relaxing a rule is the user's ruling. It is recorded in `AGENTS.md` with the
  reason, and the checker changes in the same commit.
- **Rulings and tooling fixes:** relay the user's rulings, including style rulings, to every running worker. When
  a tooling fix that affects running lanes lands on main (a gate or rule-checker fix, for example), send each
  worker the commit to cherry-pick.
- **Worker claims about tools are leads, not facts.** If a worker avoided a correct change because a gate, the
  rule checker or another tool objected, check the tool: a wrong gate steers every worker. A checker that flags
  genuine original code (hardware `volatile`, struct-prefix inheritance, a real vtable slot) is a checker bug.
  Never treat it as a reason to distort the source.
