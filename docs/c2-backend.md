# VC6 SP5's back end (C2.DLL): the rules behind inline, register and frame walls

What the back end of our compiler decides, read from its code: `tools/vc6/VC98/BIN/C2.DLL` (737,329 bytes,
2000-08-23, build 8966, image base 0x10700000, no symbols). Each rule names the routine it comes from (addresses as
Ghidra prints them; `sub_` names are ours). `tools/c2trace.py` runs the real back end on one of our functions and
prints these decisions as they happen; use it before guessing at a budget or a register wall. Under each budget
verdict it prints what would flip it, `--whatif` replays the inline arithmetic below on its recorded trace
(`site=K,+1|-1`, `own=+N|-N`), `--retail` marks the callees whose out-of-line count differs from retail's, and
`--cost --worth N` lists the statements worth N tuples, `--sched` traces the instruction scheduler and
`--header-override HEADER=FILE` measures a header edit from a scratch copy (`docs/toolchain.md`, "Tools in detail").
`tools/c2order.py <addr>` finds the deciding site of a schedule difference by rerunning the scheduler in retail's
input order ("Scheduling").
In Git Bash, write a scratch file's path `C:/...` and set `MSYS_NO_PATHCONV=1` for `--flags`.

The back end's source files are named by its assertion strings (`E:\8966\vc98\p2\src\P2\<file>.c`). The DLL was
linked with a profile-guided function order, so a file's functions are scattered and many functions are split into
a hot and a cold part (the inline driver's cold half sits at 0x10793E48); the anchors don't bound file ranges.

Prior art: byte-tactics read Visual C++ 5.0 SP3's C2.EXE the same way (`docs/c2-regalloc.md` there). Where a rule
below says "VC5 agrees", its findings hold for VC6 unchanged.

## Pipeline

`sub_10757FC2` (main.c) compiles the unit's functions one at a time, in the order the front end hands them over
(the `.sy`/`.ex` IL files). For each function: inline expansion (`sub_107180FD`), then with global optimisation
(`/Og`, part of `/O2`; flag `0x107AC058`) the global optimiser, then the global register allocator
(`sub_1072FB58`, color.c), then code generation with the temporary allocator (`sub_107336F4`, regasg.c), then the
frame layout (`sub_10733B7B` -> `sub_10733BF8` -> `sub_1074B617`, stack.c), then the instruction scheduler
(`sub_107374AA`, schedmd.c), then emission.

The two passes share nothing but the IL files of one temporary prefix (`-il <TMP>NNNN`: `ex` the code tuples,
with source-line markers `4F 01 80 <u16 line>`; `gl` globals and file paths; `sy` symbols and names; `in` an index;
`db` with `/Zi`). `cl /B1`/`/B2 <program>` replaces a pass with any program, which gets the pass's command line in
`MSC_CMD_FLAGS`; `tools/c2il.py` captures the IL that way (bw1-decomp's `docs/msvc6_passes.md`, CC0; checked here:
blank lines and comments leave every file equal once the line markers are left out, a renamed local changes `gl`
and `sy` only, a pair of braces changes `ex`).

Hidden back-end options (table at 0x107A35C8, passed by `cl /d2<option>`): `-vol#` (default 3), `-isize#` (4),
`-il#`, `-Loop#` (100), `-Gs#` (4096), `-ehopt`/`-noehopt`, `-QIfist`, `-QIfdiv` and others. Our builds pass none.

## Inline expansion (inline.c)

### Cost, candidates and budget

- **A callee's cost is computed by the front end, not by C2.** C1/C1XX writes it into the function's symbol record
  (read at `sub_10741B6D`: symbol +0x6D, a 16-bit count, beside +0x6B, the parameter count). It grows with the IL
  of the body: observed values are 20-24 for an empty or one-load function (`NiPoint3()`, `IncRefCount`,
  `operator->`), 26-36 for one store or one call (`SetScale`, `SetZBufferTest`), 41 for `~NiPointer`, 47 for
  `NiPoint3( x, y, z )`, 53 for the NiPointer constructor, 57 for `NiPoint3::operator=`, 66 for `DecRefCount`,
  91 for `NiPointer::operator=`. Every inline function in a header has one fixed cost wherever it is called.
- **The cost is the number of IL tuples the front end emitted for the function** (C1XX.DLL, image base 0x10400000:
  every tuple goes through `sub_10415BCE( opcode, node )`, which adds 1 to `0x104E2E8C`; `sub_10416705` zeroes it
  when a function's IL starts, `sub_10418D02` stores it when it ends, and the symbol writer `sub_10401DAB` writes it
  as the 16-bit cost, 0xFFFF from 65536 up). Under `/Ob2` the front end also marks a function below 175 tuples
  (`0x104D7D34`) as an automatic inline candidate. `c2trace --cost` lists a function's tuples. Read from scratch
  compiles (each count equal to the cost C2 read):
  - 12 for an empty function (prologue and epilogue), +1 per parameter; a member function adds 4 for `this` (+1
    more when it is `const`);
  - a variable read is 2 tuples (symbol, load), a variable written 1 (symbol), a constant 1, an operator 1, a
    conversion between arithmetic types 1; a comparison used as a value 1 more than as a condition;
  - `p->m` and `p[i]` add 3 to the pointer (constant offset, add, load), even at offset 0;
  - an assignment statement is 2 plus its operands (assign, discard): `a = a + 1` is 7 tuples, `a += 1`, `a++` and
    `--a` 4, so the compound spellings cost 3 less;
  - a call is 3 (callee, call, sequence point) plus 1 per argument and the arguments' own tuples, plus 1 when its
    value is discarded; a virtual call loads the vtable entry (1-2 more);
  - `if` adds 6 to its condition (and 4 more for an `else`), `while` 6; a `for` loop about 11 beyond its init,
    condition, step and body;
  - a local declaration costs nothing beyond its initialiser's assignment;
  - **braces are tuples**: a `{ }` block is a scope and an end-scope tuple, so bracing a one-statement `if` or `else`
    body costs 2 a block with identical bytes (`if ( p < 0 ) { p = 0; } else if ( p > 1 ) { p = 1; }` 49 tuples, 45
    unbraced). A header inline's braces are part of its cost wherever it is called;
  - **parentheses around a float expression are a tuple** (op 0x59), one per pair, also around a lone operand
    (`((a)) + b`): `float k = ( a + b );` is 26 tuples, `float k = a + b;` 25. Around an integer expression they cost
    nothing. Usually the bytes don't change, but the tuple is a node the scheduler sees (below, "Scheduling");
  - a float store through a pointer parameter: a flat member `t->f = 0.0f` 7, an element `t->m[3] = 0.0f` or
    `t->m[0]` 9, a nested member `t->row.x = 0.0f` 9; a chain `t->m[0] = t->m[1] = t->m[2] = 0.0f` 23 against 27
    for three statements.

  bw1-decomp (openblack's matching decompilation of Black & White on the same C2.DLL build, CC0;
  `docs/msvc6_inliner.md` there) measured the brace and store costs with a threshold ruler (10 for an element or
  nested-member store, 8 for a flat member, about 2 a brace block); the counts above are ours, read with
  `c2trace --cost`, and agree on the differences. The parenthesis tuple is from their scheduler note (below).
- **Candidates** (`sub_10718A2A`, walking the caller's IL in order): a direct call to a function whose IL the back
  end holds (symbol +0x36 bit 0x4000), not currently being expanded (+0x73 bit 0x10: no recursion). There is a size
  limit `0x10 << vol` (128 with the default `-vol3`; `0x10799280`, set in `sub_10757FC2`), but the test reads
  `[0x107AC0B4] != 0 || cost < limit` (0x10718BD7, its cold half at 0x107941E1): the limit applies only when
  `0x107AC0B4` is 0. That flag is bit 23 of the function's option word (decoded by `sub_1071BC73`), and it was 1
  under every flag set tried (`/O2`, `/O1`, `/Ox`, `/Od`, `/Os`, with `/Ob1` or `/Ob2`), so **our builds have no size
  limit**: a callee above 128 is a candidate and is expanded whenever the budget holds it
  (BridgeObjectClass::SetupLighting expands three of its four `NiTMap::GetNext` sites, cost 149, and retail calls
  0x6655B0 once, the fourth). c2trace prints `size limit on` under a function's header if the flag is ever 0.
- **Starting budget** (`sub_107180FD`): `max( 1000, 2 * own cost )`, where own cost is the function's own front-end
  cost. If `2 * own cost > 35000` nothing is inlined in that function. The running total `0x1079F234` starts at the
  own cost.
- **The decision at each site** (`sub_107181A8`, at 0x1071871E): the site is expanded unless
  - its depth exceeds the site's `inline_depth` (below), or
  - the callee isn't `__forceinline` (+0x73 bit 0x2000) and either `cost > 40 && cost > budget left` or the total
    already exceeds 35000, or
  - the call passes a different number of arguments than the callee's parameters.
  A callee of cost 40 or less is free: it never fails on the budget and isn't charged. A callee above 40 is charged
  its cost (`budget -= cost`); a callee whose cost equals the budget left still fits. Every expansion adds its cost
  to the total (`__forceinline` ones are neither charged nor counted).
- **Late rejections** after the copy: a callee returning a UDT through `___$ReturnUdt` in an EH context, or one
  whose EH state can't be merged (frees the copy at 0x10718418 / 0x10793F40); and when `0x107AC0B4` (bit 23 of the
  option word, above) is 0, a size check (`sub_10794A84`: the expanded body's statement count against
  `(arguments + 2) * vol`), so never in our builds. The cost is charged before the copy (0x10718784), so a late
  rejection still spends the budget; a rejection after the gate but before the copy (c2trace: "rejected after the
  budget gate", an address-taken or `setjmp` site) spends nothing.

### Order and the nested budget

- **Depth first, in IL order.** The driver visits the caller's sites in the order the calls appear in its IL. When
  it expands a site it first copies the callee's IL and recurses into the copy (`sub_107181A8( copy, depth + 1,
  budget, ... )`, at 0x1071888B) before moving to the next site.
- **A nested level gets `budget left / sites not yet visited`** (`idiv` at 0x1071887F), where the divisor counts
  the current site and every site after it at this level. The copy's own sites then spend from that share, and what
  they consume (the driver's return value) is subtracted from the caller's budget and added to the total.
- Consequences, all measured with c2trace:
  - the early sites of a function with many sites get tiny nested budgets (`2661 / 107 = 24` in
    MetaNebula::CreateExterior) so their callees' callees above 40 go out of line, while the last sites get the
    whole remainder (`68 / 1`): **the last sites keep their deep inlines**. This is the "nested sites in reverse
    source order" seen on scratch units (docs/levers/inline.md, "The inline budget"): the order is forward, the
    divisor shrinks;
  - inside one expanded callee the same division applies: in NiZBufferProperty's constructor (three sites, budget
    24) the second site's SetBit gets `24 / 2 = 12` and fails, the third gets `24 / 1`;
  - a bigger function gets a bigger budget (`2 * own cost` above 500): adding statements to a function can make it
    expand *more* (the glow-constructor probes saw this);
  - every site counts in the divisor, including free ones (cost <= 40) and ones that will fail, so adding or
    removing any inline call before a site changes the nested budget of every site before it.
- VC5 agrees on all of this (byte-tactics: "costs its IL size if 41 or more; its own sites start from
  (budget left - cost) / R").
- **Reading a margin.** The share is `L / R` with `L` the budget left at the parent site after its own charge and
  `R` the parent level's sites not yet visited, so a nested verdict moves with `L` in steps of `R` units, or with
  `R` itself. c2trace prints both for every budget verdict: a nested rejection "needs >= N more units here (share
  S -> S+N: >= X more left at [i/n] above, or K fewer sites from there on)", a close expansion "flips with >= N fewer
  units here (... or K more sites after it there)". At the top level a tuple of own cost is two units of budget
  (above the 1000 floor), or a unit spent by an earlier expansion. `--whatif` replays the whole function (every
  later share moves too) and reproduced the compiler's budgets and verdicts at every site of BridgeObjectClass,
  PhaserGraphic, ShipClass and MetaNebula::CreateExterior.

- **A nested level's in/out pattern constrains its site order.** Inside one expanded callee every child gets `left /
  sites not yet visited`, and only an expansion lowers `left`, so for equal-cost children the shares can only fall
  after an expansion and rise while the children go out of line. Retail's pattern "out, in, in, in, out, out, out, in"
  over eleven equal reads (NiTGAReader's GetPixelRGBA) is impossible in that order: `in` at the 5th needs `left >= 7 x
  45`, and three expansions later the 10th still has `left / 2 >= 90`. The original had the cases in another order
  (32 before 24, as its GetPixel24): reorder until retail's pattern is monotone, then fit the budget.
- **A non-inline member is never a site, even under `/Ob2`** (scratch, `/O2 /Ob2 /GX`): defined in the unit before
  or after its caller it is called out of line, while the same body declared `inline` is expanded wherever it is
  defined. A retail call to a COMDAT copy with ample budget left is therefore a late rejection, not a definition
  order (NiFrameRate::SetupProperties' NiRawImageData::operator(), which NiReadTGA expands).

### `#pragma inline_depth` belongs to the function that holds the site

A site's depth limit is the `inline_depth` in effect where the function *containing the call* was defined: the front
end writes a marker tuple (opcode 0x1B8) into each function's IL and `sub_10718A2A` copies its low byte into every
site of that function (the site node's +8). The default is 8. So a `#pragma inline_depth( 1 )` above an inline
function's definition stops the expansion of *its* callees at depth 2 in every caller, while the caller's own pragma
state applies only to the caller's direct sites (scratch: `P::P` inside `B::B` defined under `inline_depth(1)` is
called, inside `C::C` defined after `inline_depth()` it is expanded).

### `/Ob1`, `/Ob2`, `__forceinline` and the warnings

- Which functions have IL in the back end (are inline candidates at all) is the front end's decision (not read in
  C1XX: `/Ob1` passes functions declared `inline` or defined in the class, `/Ob2` also others it judges small).
  Symbol +0x73 bit 0x80 marks a declared `inline`, bit 0x2000 `__forceinline`.
- C4711 ("selected for automatic inline expansion") is raised for an expanded callee with neither bit; C4710 / C4714
  ("not inlined") only for a rejected callee with one of them. C2 itself applies the same rules to both.

### Validation

- MetaNebula::CreateExterior (0x59A2C0): ours expands the last `~NiPointer<NiVertexColorProperty>`'s DecRefCount
  (nested budget 68, cost 66), retail calls it. Retail's nested budget there was below 66: three or more budget
  units more were spent before that site in retail, or one site fewer remained (a smaller divisor makes the last
  share larger, not smaller). The material-thunks dip (99.5 -> 98.3) fits: the SetChanged level changed costs before
  the last site.
- PhaserGlow::PhaserGlow (0x4E60C0): own cost 1550, budget 3100, 51 sites; ours gives the first NiBillboardNode
  constructor's nested SetMode -> SetField a share of `3025 / 51 = 59 >= 50`, so it is expanded; retail calls
  SetField (0x4D4F90). Retail's share was below 50, which needs a smaller own cost (a budget under ~2550: own cost
  below ~1275 or the 1000 floor) or more than 60 sites. Retail also calls `NiPoint3()` and NiColorA constructors of
  cost 20-56 at depth 2-3 that ours expands for free; a free (cost <= 40) callee fails only on depth, the 35000 total,
  or an argument mismatch, so those are depth limits: an `inline_depth` lower than ours in effect where the
  NetImmerse inline functions holding those sites were defined, or a different IL shape (the site not being a direct
  call).
- Register roles (BaseObjectClass::SetName, 0x4315C0): `--regs` gives `this` to ebx (priority 82, only callee-saved
  registers allowed: live across calls), the string argument to esi, as in retail.

## Registers (color.c, regasg.c)

### Global allocator (`sub_1072FB58`)

- **Candidates** are webs of locals, parameters, temporaries and constants, one 0x44-byte record each, hashed on
  `id & 0x3FF` (`0x1079D88C`, chain +0x2C; `sub_107258DA`). Fields: +0x00 operand (kind at its +4: 3 variable,
  4 temporary, 0xD constant), +0x0C priority, +0x10 register given (`&regs[r]`, `0x107AC730 + r * 0x54`), +0x14/+0x18
  list links, +0x1C id, +0x20 allowed-register set, +0x24 reference count, +0x34 preference list, +0x3C spill
  cost, +0x40 number of the last tuple writing it.
- **Names.** A candidate's operand (+0x00; kind 3 variable, 4 temporary, 5 parameter, 0xD constant) points at its
  +0x00 to the symbol record the front end gave the variable, which holds its name at +0x18 (spelled `_total`:
  `sub_10742388` puts `_` before an automatic's C name), its frame offset at +0x0C and its size at +0x10. The pointer
  is null for the back end's own variables (induction variables, strength-reduced pointers, common
  subexpressions), so those print as `var #id`. The names are present without `/Zi` or `/Z7`. One local can stand
  behind several candidates: each inlined copy of a callee's local and each web of a variable is its own candidate
  (`database` three times in BaseObjectClass::SetName).
- **Priority** (`sub_10724B25`, per basic block `b`): `w(b)` is 1, or `1 << (block +0x6E)` (the loop nesting) when
  bit 23 of the optimisation word (`0x107AC0B4`) is set, as it is under `/O2`; `K(b)` = number of candidates live
  in `b` (a popcount of the live set) plus the ones referenced in `b`. A candidate referenced in `b` gains
  `w * K * (its reference costs in b)`; one live through `b` without a reference loses `w * K`. The spill cost +0x3C
  is the same sum without `K`. A reference's cost comes from `sub_107254DC` / `sub_10725751` (2 for a variable
  reference, less for constants). VC5 agrees (its weights 1/4/8 are `1 << depth` read differently: VC6 shifts by the
  block's stored depth).
- **Order** (`sub_10731D21`): the worklist is sorted by priority, then by +0x40, both descending; a new candidate is
  inserted before equal ones. VC5 agrees.
- **Drop**: a popped candidate whose spill cost is below 1 stays in memory (`sub_10732001`, at 0x1072FDC7).
- **Choice** (`sub_10732F7C`): each register starts at cost 0 (`0x1079D868`); every interfering candidate adds its
  preference weights to the registers it prefers, and one with a single register left adds `100 * its spill cost`
  to that register; the candidate's own preferences subtract. The cheapest allowed register wins, scanning the order
  table eax, ecx, edx, esi, edi, ebx, ebp (`0x107A09E8`) with a strict `<`, so ties go to the earlier entry. VC5
  agrees, table included.
- **Allowed set**: a candidate live across a call loses eax, ecx, edx (only esi, edi, ebx, ebp remain); ebp only
  without a frame pointer.
- **A byte-sized value live across a call can only have ebx.** A `char`, `unsigned char` or `bool` candidate never
  gets esi, edi or ebp (they have no byte form), so across a call its allowed set is ebx alone. It is then a
  single-register candidate from the start: every interfering candidate pays `100 * its spill cost` on ebx ("Choice"),
  so the others move to ebp, esi or edi and the byte value gets ebx even when it has the lowest priority. Scratch A/B
  (four ints and one value live across calls, the value's spill cost 4): as `char` it is allowed only ebx and gets it,
  the ints showing `ebx:400` and the third int going to ebp; as `int` the third int takes ebx and the value ebp.
  Retail shows it in PhysicsObjectClass::VirtualSlot73 (`unsigned char flags` in ebx across the stream calls,
  `framemap`). Not live across a call, a byte value has eax, ecx, edx and ebx (TGNetwork::SendMessages's `first`).
- **Constants compete like locals.** Each constant value used as an operand (a stored 0, -1 or 7, a float's bits) is
  one candidate (`const #N`) in the same sorted worklist as the variables, its priority from the same per-block sum
  with a smaller reference cost; when it wins, the register is loaded once (`xor ebx, ebx`, `mov ebx, -1`) and the
  stores read it. 0 and -1 aren't special: on a scratch loop of five member stores, 0, -1 and 7 each became a
  candidate and took ecx, and with a call in the loop the stored 0 (priority 31) took ebx ahead of a parameter
  (priority 21, sent to ebp). A value's compares join its web: in the loop of stored zeros, the `while ( s )` tests
  against 0 joined the same candidate (7 references, priority 16, against 5 and 24 for -1, whose tests' 0 was a
  candidate of its own at -1). The `framemap` tables of the packets show `constant 0` in ebx, ebp or edi in many
  matched and open functions (PhaserGraphic::Start's ebx, VirtualSlot73's ebp).

### Temporaries (regasg.c, `sub_10767FA8` / `sub_10768205`)

Expression temporaries use the writable copy of the same order table at `0x107ADFF4` (`0x1079D710` points at it,
set in `sub_107336F4`); the rotating-pointer rule byte-tactics describes for VC5 was not re-read for VC6.

## Frame layout (stack.c, `sub_1074B617`)

- Locals left in memory are placed one at a time in list order (`0x1079F220`, chained at +0x2C). Each first tries
  the fixed slots (kind 5, already placed: the first, lowest-index fixed slot at least as large and not interfering);
  then `sub_1074BAE6` tries the **newest earlier slot at least half its size** (`own size <= 2 * slot size`) that
  doesn't interfere, growing the slot to the larger size and adding the local's reference count to the slot's;
  else it opens a new slot. Slots are 20-byte entries (`0x1079F228`): +0 members, +4 interference, +8 size,
  +0xC reference count. VC5 agrees.
- If the frame's total exceeds 0x80 bytes, the non-fixed slots are re-sorted by `refs * 1000 / size`, descending,
  with a recursive middle-pivot quicksort (`sub_10761BF0`: unstable, so equal keys can swap). VC5 agrees.
- With a frame pointer or EH (function flags 0x10 / 0x600000), if any local joined a fixed slot and the frame is at
  least 0x70 bytes, the packing is redone without joining fixed slots.
- Offsets: without a frame pointer slots are assigned from index 0 upward; with one, from the last slot down.
- The packer runs only when some local isn't fixed (`sub_10733BF8`: local count `0x1079F20C` against fixed count
  `0x1079F21C`); otherwise the frame is the fixed slots alone.
- The list's records are operands (as the register candidates' +0x00: +4 kind, +0x20 size, +0x2C next, +0x34
  references, +0x38 id; +0x00 the source symbol, whose +0x18 is the name). The order observed with `c2trace --frame`
  is ascending id, the order the back end created them (fixed slots first in BaseObjectClass::SetName, then
  `temp #0` to `temp #36` in MetaNebula::CreateExterior), not a size sort. Final offsets: a temporary's goes into its
  symbol record's +0x0C, any other local's to its own +0x28.
- **A local with a destructor gets its id where the front end first emits its destruction**, not at its declaration:
  an inner block's object closes first, so `TGRect outer; ... { TGRect inner; }` numbers `inner` first and packs it
  lower (TGParagraph::VirtualSlot92). A `return` before the inner block emits the outer object's destructor there and
  numbers it first: TGIcon::Move matches with its two early returns (`if ( !IsVisible() ) { ...; return; }`), and
  TGUIObject::Move/SetPosition take retail's rectangle order the same way. Renaming or moving the declaration
  doesn't move the id.
- **A parameter reassigned from itself keeps its slot to itself.** `dx = PixelAlignValue( dx );` leaves the
  inlined `int pixels` out of `dx`'s fixed slot (it interferes there); `float alignedDx = PixelAlignValue( dx );`
  leaves `dx` dead after its read, and both the pixel count and `alignedDx` join `dx`'s slot, retail's
  `mov [esp+0x40], eax; fild [esp+0x40]` in TGIcon::Move and TGUIObject::Move.
- `c2trace --frame` prints the list in packing order, every slot as the packer leaves it (size, references, sort
  key, members) and each local's final offset; inlined copies of one callee's local show as several locals of one
  name.

## Frame layout at /Od (the front end's local order)

At /Od (no packer) a scope's named locals are laid out by a hash of their names, measured against the SDK's debug
NetImmerse 3.1 (AnimOnly.exe) by scratch compiles: `v = (v << 2) + (v >> 4) + c` over the name's characters (32-bit),
bucket `(v ^ (v >> 16)) % 16`. A lower bucket sits closer to ebp; within a bucket the later declaration comes first;
compiler `$S` temporaries come after the scope's named locals, and nested scopes are allocated after their parent.
So a local's /Od slot order narrows its name without proving it: names chosen to fit it are marked `(name
invented; fits the /Od frame order)` unless an assert or the lineage gives them (AGENTS.md, the ruling on NetImmerse
local names). A frame decided by `$S` numbering can't be fitted by names. The model and a solver (fewest renames
that give a target order) are kept as `tools/od_names.py`.

## Operand order of commutative operations (the global optimiser)

Which operand of a commutative operator (`0x107062EC` tells which; checked for float and integer `+` and `*`) is
evaluated first, and which one a `fmul`/`fadd` takes from memory, is a sort, not the source's spelling. Read by
bw1-decomp (`docs/msvc6_operand_order.md` there, CC0) on the same C2.DLL build; the addresses, the weight formula for
leaves and products, and the leaf fields below were confirmed on our DLL by breakpoints with `tools/c2dbg.py` on
scratch compiles (the constant and memory-reference cases are bw1-decomp's reading, not re-traced).

- **The sort** (`0x1070D7DD`, under `/Og` only): for every node of every expression tree after inlining, each operand
  gets a 32-bit weight (`sub_1070DA9A`, `ecx` = node, the weight returned in `eax` and kept at node +0x0C); a
  commutative operator's operand list is then sorted by weight, **descending**, with a stable merge sort (comparator
  `0x1070F6AE`, -1/0/1). Without `/Og` nothing is sorted: the scratch `a*b` against `b*a` compiled as written under
  `/Od` and identically under `/O2`, and so did `a->x*b->x + a->y*b->y + a->z*b->z` against the same terms in another
  order, and integer `a*b + c` against `c + b*a`.
- **The weight** is `height << 24 | size << 16 | hash16`. A leaf has height 0 and size 1; an expression's size is
  its operands' sizes plus one, and its height grows with its tallest operand. `hash16`: for a variable leaf (node
  kind 2, the low byte of +0x08) `symbol number << 5` (`<< 6` for some descriptor kinds: "Base and index of a
  two-register address" has the full table); for an expression the sum of `operand hash << i` over its
  operands in list order, plus `opcode - 0x145`; a constant's is its value (and constants lose the size bit, so they
  sort last); a memory reference with a non-zero offset is an expression over an address temporary. Measured: in
  `float h( float a, float b ) { return a * b; }` the leaves weigh 0x10040 (`_a`, number 2) and 0x10020 (`_b`,
  number 1), the product (op 0x16F) 0x10300AA = height 1, size 3, `0x40 + (0x20 << 1) + 0x2A`.
- **The symbol number** is the dword at +0x1C of the leaf's descriptor (node +0x14; the descriptor's +0x00 is the
  front end's symbol record, name at +0x18, as for the register candidates). Locals and parameters are numbered in
  order of first reference in the function's IL: `float x = F1(); float y = F2(); return x * y;` gives `_x` 1, `_y`
  2, so `y` sorts first whichever way the product is written. Descriptors come in chunks of 32 numbered from a
  counter reset for each function, and freed ones are reused first ("Base and index of a two-register address": a
  function before it in the unit leaves its numbers alone); bw1-decomp read that the inliner frees a callee's parameter
  descriptors when it binds them, so the next inline's `this`, parameters and locals take the freed numbers. That is
  why the same inline sum flips between callers, and why statements and inlines *before* a site move its order
  (docs/levers/x87.md, "x87 operand order: set by the source before the site").
- **What code generation does with the sorted list:** a sum evaluates its operands first to last, so the heaviest
  term is computed first and the lightest added last; a binary x87 operation loads the first (heavier) operand and
  takes the second as its memory operand, except when exactly one operand is a memory reference, which then becomes
  the memory operand.
- So two same-shaped leaves are ordered by their symbol numbers alone, and only a change of numbering (a local
  first referenced earlier or later, another inline expanded before the site, a temporary more or fewer) flips them;
  a textual swap never does. A helper spelling that moves the order is a lead, and needs its own evidence like any
  other (R17).

## Base and index of a two-register address (code selection)

Which register of `[eax+ecx-8]` is the base (printed first, the SIB base) is the commutative operand sort's order on
the sum that forms the address, not a code-selection preference: **the operand the sort puts first becomes the
base.** Read from C2.DLL and checked with breakpoints (`tools/c2trace.py --addr`, `tools/c2addr.py`).

- **Where.** After the global optimiser and before the global register allocator, the address matcher
  (`sub_1072AC94`, called from `0x1072936B`/`0x107294A4`/`0x107294E3`) starts at the register that holds an
  address and works backward through its defining tuples, folding adds and constants into one operand. It builds the
  operand with `sub_1070148F` (an indirect operand, node kind 6: `mov [eax+ecx-8], 0`) or `sub_10726CD5` (an address,
  kind 5: `lea edx, [eax+ecx+0xC]`): fastcall, ecx = IL op (0x14C one register, 0x14D base + index, 0x14E/0x14F
  scaled index), dx = type, then symbol, displacement (+0x24), base (+0x28), index (+0x2C). The scale's log2 is the
  low nibble of +0x10. The listing printer (`0x107753A9`, the `[%s` / `+%s` / `*%d` strings at 0x107A6D78..0x107A6DB4)
  prints +0x28 first; a one-register operand (op 0x14C) keeps its register at +0x2C. The register nodes are pseudo
  registers when the operand is built; allocation fills in the real ones later, and a spilled operand's slot is
  replaced by its reload, so the slot, not the register, is what code selection decides.
- **The rule.** The sum's add tuple is two-address: the register is loaded with the sort's second operand and the
  first is added to it (as a register or a memory operand). The matcher, chasing that register, makes the added
  operand the base and the register's earlier value the index. So the heavier operand is the base, whether it is a
  variable, a memory load or a common subexpression: in CharacterClass::ReadFromStream the `items` load sorts first
  (base `eax`), in CameraMode::ReadFromStream the `size * 12` subexpression does (base `ecx`), with the same registers.
- **The weights** are those of "Operand order of commutative operations"; two leaves differ only in `hash16`, which
  `sub_1070DB59` computes by node kind (jump table 0x1070E7E0, index table 0x1070E800):
  - a variable or temporary leaf (kinds 1-2) whose descriptor's +4 is 3: `number << 6` (seen on every unnamed
    temporary of these sums); any other descriptor kind: `fold(number)` mixed `<< 5`, `((v & 0x7FF) << 5) ^ ((v << 5)
    >> 16)` (the parameters `_a`/`_b` above, `this`, the named loop counter `i` of NiBezierTriangle::VirtualSlot45);
    node kind 3: `fold(number)`; a temporary whose descriptor +0x14 holds an expression (a common subexpression)
    hashes as that expression;
  - a memory reference or address (kinds 5-6): `fold(displacement) + op - 0x145 + (hash(its base) << 8)`; the index
    doesn't count, and of the base only the low byte survives the 16 bits: for a `number << 6` base, `number & 3`;
  - an expression (kind 12 and up): the sum of its operands' stored hashes `<< i` (i = 0..7, list order), plus
    `op - 0x145`;
  - `fold(x)` is `(x >> 16) ^ (x & 0xFFFF)`; every result is cut to 16 bits.
  So a temporary `#n` against a load through temporary `#m` compares `(n << 6) & 0xFFFF` with
  `((m & 3) << 14) + 7`: the order changes with `n mod 1024` and `m mod 4`.
- **The numbers.** A symbol's number is fixed when its 0x54-byte descriptor is created, not when it is used:
  descriptors come in chunks of 32 (`sub_107079C3`), numbered from a counter at `0x1079BC4C` that
  `sub_1071B9B0` resets to 0 for each function; a freed descriptor goes on a free list (`0x1079BC84`, linked at +0x28,
  last in first out) and keeps its number, and the next allocation takes it before a fresh one (locals of kind 3 have
  a list of their own, `0x1079BC60`). Numbers therefore follow creation order with reuse: the back end's own
  temporaries (common subexpressions, the pointer from `new`, induction variables, reloads) are created after the
  front end's symbols and take whatever is free, an inlined callee's parameter descriptors are freed when bound
  (bw1-decomp's reading), and an unused local or a no-code statement usually leaves the numbers as they were
  (its descriptor comes back to the list).
- **What `c2trace --addr` prints.** Per function, each two-register address the matcher builds (S1, S2... in build
  order) with its final registers; the two operands as the sort saw them, each hash taken apart (`[temp #1226]` is
  a load through temporary 1226, `temp #9{...}` a common subexpression); and the arithmetic of a flip: the nearest
  shift of every symbol number from the lower operand's up ("numbered before both"), and of the later operand's
  numbers only ("numbered between them"), that reverses the two. A table follows: for k = -4..+4 symbols more or
  fewer numbered from a point P on, which sites flip together. A site that needs a shift the table doesn't offer
  alone needs two changes (one point up, another down) or a different reuse of freed numbers: compiler state, not a
  spelling. An operand rewritten in place later (`mov [esi+4], 0x14D` at 0x1071F93A, 0x10754B05, 0x1077A8A4,
  0x1077AA8D: a register added to an existing operand, into a fixed slot) prints the pass instead; operands built by
  copying another print nothing yet *(open)*.
- **Measured.** On a scratch copy of NiTListBlock's constructor the last item's store is `[eax+ebx-12]` (the load
  through temporary 166 first, the `size * 12` temporary 165 second); `pad[5] = 1;` before the `new` creates two
  descriptors, the table's predicted +2, and the store becomes `[ebx+eax-12]`; +5 and +9 leave it, +6 flips it again,
  each as computed (`tools/tests/test_c2addr.py` runs the first pair).

## Float copies (fppeeps.c, the global optimiser's forward substitution)

Every 4-byte float assignment is lowered to an x87 pair, `fld src` (IL opcode 0x60) then `fstp dst` (0x63). Integer
moves come later, from one peephole; whether a copy goes through the FPU is decided by what stands between the two
tuples. `tools/c2float.py <unit|file.cpp> <function> [--il]` prints the statement tuples after inlining
(`0x10718EAF`) and after the optimiser (`0x107281CD`), then each fstp as the peephole sees it, with its verdict.

- **The peephole** (`sub_1072FEF2`, called twice from the register allocator `sub_1072FB58`; its cold half
  `0x107773AF` does the rewrite): an fstp whose *previous* tuple is the fld becomes one move (opcode 1, later a
  `mov reg, src; mov dst, reg` pair, or `mov`/`push`), and the fstp is deleted, when all hold:
  - neither the fld's source nor the fstp's destination is a register (operand kind 1);
  - both have the same type (a `double` source stored to a float keeps the FPU);
  - an indirect operand (kind 6) has no index register (operand +0x2C): **`float a = p[i];` and `arr[i]` keep
    `fld [base+index*4]; fstp` even at one copy level**;
  - the function's optimisation word doesn't have bit 18 (`0x107AC0A4`, set from the word in `sub_1071BC73`, and set
    by the inliner when it expands a callee whose word has it; not `/Op` and not `#pragma optimize("p")`).
  `/O2` (bit 23, `0x107AC0B4`) extends the rewrite to 8-byte doubles; without it only `float` (type 0x4004) is
  merged.
- **What separates the pair: an op 0x162 tuple.** The global optimiser (`sub_107130CB` -> `sub_10711AFA`) forward-
  substitutes a single-use definition into its use: `x = <expr>; ... y = x;` becomes `y = <expr>` and `x` goes.
  When the substituted value is a float (type 0x4xxx) it wraps it in an op 0x162 tuple, a float value held in a
  register (`sub_10701330( 0x162, ... )` in `sub_10711AFA`, after the liveness checks `sub_107428F6` and
  `sub_10742AD4`). Lowered, that is `fld src`, an st(0)-to-st(0) move (op 0x162, emitted as nothing), `fstp dst`:
  the fstp's previous tuple is the move, so the peephole leaves the pair alone.
  A plain variable on the right (`x = g_s.in.v`, `x = param`) is copy-propagated instead, with no 0x162: integer
  moves.
- **So, in source terms**, a float copy goes through fld/fstp when its value reaches the destination through an
  intermediate the optimiser substitutes away, and that intermediate was set from an expression that isn't a plain
  variable (a load through a pointer or `this`, `p->m`, an element, arithmetic):
  - a nested inline accessor: the inner inline's return value is a compiler temporary, so `float a = o->Get2();`
    with `Get2() { return in.Get(); }` is `$T = *(o+off); a = $T` after inlining and `a = 0x162(*(o+off))` after the
    optimiser. A one-level accessor `return w;` is substituted by the inliner itself (`a = *(o+off)` straight after
    inlining): adjacent pair, integer moves;
  - a named local copied into another (`float a = p->w; float b = a;`), an accessor that names its local
    (`float x = w; return x;`), an inline function's parameter copied into a local: the same shape;
  - the destination can be a named local, a global, `*p`, or the argument slot (`fstp [esp]` when the
    substituted variable's only use is a push).
  The intermediate has to be single-use: a temporary or local with two uses isn't substituted, keeps its frame slot
  and is copied with integer moves (`$T` in the EH case below).
- **The six hidden-level accessors** (x87.md): an inner inline (`return r.GetWidth();`), a named local in the
  accessor (`float x = w; return x;`) and a template value wrapper (`return w.Get();`) give the same tuples after the
  optimiser (`R = 0x162(*(this+off)); a = R`); before it they differ only in the intermediate's symbol (`_x` against
  an unnamed `$T`), which leaves nothing in the object. Their front-end costs differ by one (27, 28, 27 against 23 for
  `return w;`), all under 40, so they are free at every site and don't move a budget; the inner-inline and wrapper
  spellings add one nesting level (an `inline_depth` limit could tell them apart, nothing else). `return (double)w;`
  and a `double` return from an inner inline are folded back to integer moves.
- **Under EH** (a destructor object anywhere in the function), forward substitution is cut at each EH state marker:
  `sub_10711AFA` walks the tuples, and at a kind-20 tuple (the state changes `op 0x1A0`, `op 0x1A8`) or an op 0x187
  tuple (`0x10711FD2`) it drops (state 0xB, `0x10712166`) every pending candidate that hasn't seen exactly one use,
  and every one that has but whose value is a plain load (kind 1 or 2 with a defining symbol, `0x1071217E`). The
  candidate list spans the whole function, not a block, so a marker after the copy kills it wherever the
  destructor object is constructed. That is why `float a = o->Get2(); Use2( a, a );` gives integer moves into `$T`
  under EH. What survives under EH: a named local or an inline's parameter as the last link before a call argument
  (`float t = o->Get2(); m.F( t );`, `m.H( o->Get1() )` with `H( float t ) { float u = t; F( u ); }`) gives
  `fld; fstp [esp]`. Retail's `fld; fstp [slot]` followed by an integer reload and push (Editor::~Editor,
  UtopiaModule::UpdateWindows) and its `fld; fstp [slot]` of a multi-use local (EngPowerCtrl::HandlePowerChange,
  STFileMenu::Refresh) were not reproduced: every scratch spelling tried (one, two and three levels, a named local,
  a local copied into another, an inline parameter, before or after the destructor object) gives integer moves or
  `fstp [esp]` *(open)*.

## Float constants the back end folds

A float constant C1 writes is named for its float value (`1.0f / 255.0f` in the source: `__real@4@3ff78080810000000000`);
one C2 computes, from a constant division the inliner exposed (`Inv( 255.0f )` with `inline float Inv( float x ) {
return 1.0f / x; }`), is named for the extended-precision quotient (`__real@4@3ff78080808080808000`). Both hold
0x3B808081, but the names differ, so the linker keeps two copies: **a float that retail keeps twice can be a literal at
one site and a back-end fold at another** (scratch compile; bw1-decomp's `docs/msvc6_matching_tips.md`). Under `/O2`
**every division by a float constant is a multiply by a folded reciprocal**: `v / 10.0f` compiles to `fmul
[__real@4@3ffbccccccccccccd000]`, `v * 0.1f` to `fmul [__real@4@3ffbcccccd0000000000]`, the same 4 bytes (0x3DCCCCCD)
under two names; `v * Inv( 10.0f )` takes the division's copy. Only an exact reciprocal (`v / 4.0f`, `fmul [0.25]`)
shares the literal's name. So docs/levers/x87.md's open two-copy constants (0.1f, 0.05f) read as `* 0.1f` at the
sites of one copy and `/ 10.0f` (or `/ 20.0f`) at the other's.

## Scheduling (schedmd.c)

After the frame layout, the per-function loop runs one list scheduler over the machine instructions (`sub_107374AA`,
called from `sub_10757FC2` with its second argument 0; the FP-stack variant behind a nonzero argument isn't called in
our builds). `c2trace --sched` prints every decision below as it happens (`tools/c2sched.py`).

- **Regions.** `sub_10737A43` cuts each basic block into regions of at most 81 instructions (`0x51`, counted over
  every tuple, the ones that emit nothing included), ending at a label, a branch or the block's end; a call does not
  end a region. A region with fewer than two real instructions isn't scheduled (`sub_10737A97`).
- **The graph.** `sub_107396F6` gives each instruction a node (0x3C bytes, `sub_10739A2D`): +0x36 its **sequence
  number**, the position in the region's input order (the order the code generator emitted, i.e. IL order); +0x1C
  the instruction; +0x38 its latency and +0x39 low bits its pipe class, both from the per-model opcode table
  (`0x107A0DD8` for /G5 and /G6, 12 bytes per opcode: latency, busy, class); +0x3A bit 0 "reads memory", bit 1
  "writes memory". Edges carry a latency (+0x14) and a kind; register and memory dependences, the FP stack register
  (every x87 instruction reads or writes st(0), so **x87 instructions keep their input order**), and "soft" edges an
  address fold or an esp adjustment can break (`sub_10751EA0` rewrites the displacement when a node moves past a
  push or an `add esp`).
- **Priority** (`sub_1073A684`): `height` (+0x34) is 1 + the largest `successor height + edge latency` (the longest
  path to the region's end). The static priority (+0x28) is a sum of terms, each shifted by the model's table
  (`0x107A0D98`, eight shorts per model; a negative shift is a right shift): fold bit (+0x39 bit 7), height, number
  of successors, a critical-path bit, reads memory, and for an instruction whose type is float, writes memory. Under
  /G5 (CPU word `0x107AC0B0` = 5: our builds pass no `/G`, and the default is 5; model `5 - 3 = 2`, shifts
  `-1, 13, -5, -1, -2, 16, 16, 0`) that is **`height * 8192 + 65536 if it reads memory + 65536 if it is a float
  store`**; the successor count (`>> 5`) and the bits shifted right by 1 or 2 add nothing. /G6 (model 3) would
  weigh them otherwise (`fold << 23`, `height << 10`, `successors << 10`, `reads << 15`), and `/G4` and `/G3` give
  models 1 and 0. The dynamic priority (+0x2C, `sub_1073B368`) adds `1 << shift[4]`, which is 0 under /G5.
- **The ready list** (`sub_1073B147`, `sub_1073B107`) is kept sorted by dynamic priority, largest first, and **among
  equal priorities by sequence number, smallest first**: a tie goes to the instruction that came first in the input.
  A node joins it when its last hard predecessor is scheduled; its earliest cycle (+0x30) is the largest
  `predecessor's cycle + edge latency + the busy count of the predecessor's pipe`.
- **Picks** (`sub_1073B176`): cycle by cycle, up to the model's issue width (`0x10799250`: 2 under /G5) instructions.
  For each slot the first node in list order is taken that the pairing check accepts (`0x1073B3E0`: in the U slot a
  class below the pipe count, in the V slot only an instruction that pairs with the U instruction, `sub_1073B597`),
  that `sub_1073B4A8` doesn't hold back (a soft predecessor about to become ready) and whose earliest cycle has come.
  A node passed over keeps its place; a cycle with nothing eligible issues nothing.
- **What the source controls.** Three inputs, all upstream of the scheduler: the input order (statement order and
  the code generator's order within a statement), which decides every tie; the heights, which follow the dependence
  chains, including the anti-dependences the register choices create (a register reused by a later instruction makes
  its last reader a predecessor of that write); and the region cut (every tuple counts toward the 81, so a tuple that
  emits nothing can move a cut). Nothing keys on symbol ids or declarations: the probe `ctxscan` finding "invariant"
  on the schedule-only cards agrees.
- **Memory dependences: a store and a later load or store stay in order unless both are addressed from the same
  pointer variable.** Measured on scratch compiles (`c2trace --sched`, the store's height grows by the load's chain when the
  edge exists): after `invincible = v;` (a store through `this`), a load of another member through `this`, through a
  copy `S *p = this` (copy-propagated) or through the class's own inline accessor (`return subs.warp;`, whose `this`
  is bound to the caller's) has no edge and is hoisted above the store when its height is greater; a load through a
  pointer parameter, through `Subs *s = &subs`, or through an inline member function *of the embedded object*
  (`subs.GetWarp()`, whose `this` is the address `&subs`) keeps the edge and the store's place. Offsets don't
  matter: the rule is per base pointer. Stores order the same way: `x = v; *q = 3; y = v;` chains all three
  stores (heights 4, 3, 2), `x = v; in.a = 5; y = v;` leaves them free. **An inline setter's store counts as another
  pointer, an inline getter's load doesn't:** with `T *t` a local, `t->next = 0; if ( t->GetDuration() != 0.0f )`
  hoists the duration's load above the store, `t->SetNext( 0 ); if ( t->GetDuration() != 0.0f )` keeps the store
  first (the getter's `this` is substituted by `t`, the setter's is not). TGTimerManager::Update shows it (retail
  keeps `timer->next`'s store ahead of the duration's load; MATCH-PENDING, the setter is only evidenced by this). ShipClass::SetInvincible shows it: retail keeps the flag's store before the
  warp engines' load, which only the embedded ShipSubsystems object's own accessor gives.
- **A tuple that emits nothing takes an issue slot.** Under /G5 a no-code node (op 0x162, the float held in a
  register, and the other `opNNN` nodes `c2trace --sched` prints) is scheduled like an instruction: it is picked in
  the U slot of a cycle of its own, and an x87 instruction doesn't pair beside it, so a chain through one is a cycle
  longer. In TGIconGroup::GetIconScreenWidth `fld scale; op162; fmul` puts the fmul at cycle 2 instead of 1. Which
  source makes the op 0x162 node (scratch compiles, docs "Float copies" for the mechanism): a member at a non-zero
  offset of a global object read through an inline accessor (`g.GetScale()`; at offset 0 the inliner folds it to the
  variable), any accessor through a pointer (`m->GetScale()`), a single-use named local holding a float load
  (`float s = g.GetScale(); return s * x;`, or an accessor that names its local); the member read directly
  (`g.scale * x`) gives none. "Reading a global manager through its accessors instead of its members changes the load
  schedule" (docs/levers/locals.md) is this rule.
- **The deciding site, mechanically** (`tools/c2order.py <addr>`): the first divergence between ours and retail is
  classified (`reorder`, `reorder-regs`, `x87-swap`, `base-index`, `register`, `float-copy`, `frame`, `other`); for a
  reorder the tool maps each of our instructions to its scheduler node, rewrites the region's sequence numbers to
  retail's order and runs the real scheduler again (the probe: the scheduler reads the sequence number only to break
  ties). **Input order** means the probe gives retail's order, so retail's code generator emitted the nodes in
  another order (a statement order, an evaluation order inside a statement, a hoisted load placed elsewhere); it
  lists the ties that decide it with our source line (STMenu::GetDesiredSize and ReticleWindow's constructor are
  exact with the value retail loads first named in a local before the statement, MATCH-PENDING: a carrier local
  without evidence). **Not the input order** means the graph differs: a node that
  wasn't ready (the chain that held it, every no-code node on it marked) or had the lower priority (the heights to
  make up), and a pair of memory operations retail keeps in order with no dependence in ours is named as the memory
  rule above. `python tools/c2order.py triage <addr>... | --group G [--open]` prints the kind only, without the
  probe.
- **Expression temporaries rotate.** In STStylizedWindow's constructor the first value loaded into a scratch register
  after a constant took eax and the next one ecx, whatever was free: swapping two assignments swaps their registers
  as well as their order (the rotating-pointer rule byte-tactics read in VC5, `0x107ADFF4`; observed, not re-read).
  A store order that needs one value's load first and its store last is spelled by moving both statements.
- **Validation.** LODModelPart's constructor: the zero register's `xor ebx, ebx` (height 5) tied with the first
  float store (height 5) and lost on input order; `loaded = false;` first, as the default constructor begins, gives
  the xor the lower sequence number and matches. STStylizedWindow's constructor: the maximum width's store tied with
  eight other stores (height 8) and went in input order; assigning the maximum size after the scroll speed matches.
  TGActionModule::RegisterClasses: two hoisted loads tie (priority 0x14000) and go in input order, the order the
  inlined NiTArray::Add's loop names the members (a shared header). TGTextButton::TileFrame,
  OptimizedFireScript::ShouldFirePhasers and PhysicsObjectClass::VirtualSlot71 differ at an x87 store whose earliest
  cycle (fld/fadd + 2 to 4) or float-store bonus decides the pick; no spelling found *(open)*.
- **Reloads after a call follow the members' variable numbers.** After the inlined NiTArray::SetSize frees the old
  block, the enregistered size and maximum of a global array are reloaded in an order the front end's numbering of
  the two members sets: on a scratch copy, `holder::tables.Add( x )` reloads the size first (retail's order in
  TGActionModule::RegisterClasses and four other module registrations), while the same Add through an inline
  accessor returning `&tables` (pointer or reference) reloads the maximum first. The order of the statements inside
  the inlines doesn't move it.
- **Two null tests on one position merge only when the second tests the caller's own variable:** FocusManager's
  list walks load the next item and the element under one test when `GetNextPos( item )` comes first and
  `item != NULL ? Get( item ) : 0` second (the inline's test is on its parameter copy); written the other way round
  the tests split, so the element-first order retail shows needs another spelling *(open)*.
- **`/G6` (the GameSpy units)** weighs `(height + successors) << 10`, `reads << 15` and the fold bit `<< 23`, as the
  shift table says; bw1-decomp (built with `/G6`, `docs/msvc6_scheduler.md` there, CC0) logged the same formula on
  every region of a function, saw the fold bit only on the compare before a branch, and an issue model of one
  integer instruction a cycle with at most one x87 instruction beside it (pipe classes 0 and 2). A tuple that emits
  nothing still takes an integer slot there, so the float-parenthesis tuple (op 0x59, "Cost" above; their log shows
  a no-op node, op 0x162) can delay an `fstp` behind pushes: they matched a function by its redundant parentheses. Under our
  default `/G5` the pairing rules differ; read `c2trace --sched` before assuming either.
- **Not the scheduler.** A difference in the order of an address's base and index registers (`[ecx+eax-8]` against
  `[eax+ecx-8]`) is in the instruction before scheduling: it is the commutative sort's order on the address sum
  ("Base and index of a two-register address"), read with `c2trace --addr`.

## What a unit's object holds, and in which order

How the front end and C2 order a unit's sections. Read by bw1-decomp (`docs/msvc6_comdat.md`, `msvc6_data_order.md`
and `msvc6_pch.md` there, CC0) and re-run here on scratch files with our flags (`/O2 /GX`, no `/GR`); where ours
differ it says so. `tools/comdat.py order <unit>` compares a unit's order with retail's.

- **Every function is its own COMDAT under `/O2`** (`/Gy`): an out-of-line function NODUPLICATES, an inline function,
  implicit member, `??_G` or template instance ANY. A vftable is a COMDAT too (LARGEST with `/GR`, ANY without; ours
  have no `/GR`, so no RTTI and `??_7` at offset 0).
- **Out-of-line functions come in definition order; the inline functions one needs come right after it**, before the
  next. Template instances come at the end of the unit, after every other function, in first-call order (instances
  reached only from another instance's body after those). A `static` function is held back and emitted right after
  its first caller (scratch: `A, B, C, Helper, D` for a static `Helper` defined between A and B and first called by C).
- **Held-back statics: the rule, and what retail's order says about linkage** (2026-11-20). The front end defers
  every function with internal linkage (`static`, C and C++ alike) and, before each function with external linkage
  and at the end of the file, emits the deferred ones that are by then referenced, starting from the referenced
  roots and following their references (so not in definition order); one never referenced is not emitted. Scratch:
  a static whose address only a data initialiser takes after its definition (`fp gp = Helper;`, a static table)
  comes out at its definition; one referenced before its definition through a prototype (TGAssert's
  `assertHandler = DefaultAssertHandler`) still waits for the next external function; an extern function is never
  held. The same in every compiler and flag tried: SP5, SP4 (c1xx 8867, c2 8799), RTM, the Processor Pack back end,
  `/O1 /O2 /Ox /Od /Ob0 /Ob1 /Zi /Z7 /Gf /GR /YX`, and `c1` for C. **So a function retail places at its definition,
  before its first caller, had external linkage in the original** (a global or a class's static member; an
  anonymous namespace also places it there): no build setting gives that order to a `static`. A/B in the units
  where retail does it, `static` removed from the definitions and prototypes (experiment only, reverted): bytes
  unchanged in every unit, sections in retail's order before → after: TGAssert 6→8 of 8 (all five statics),
  CharacterClass 135→138 (MarkSkinControllers, FindSkinController, AttachLights), CollisionTestDisplay 9→10
  (TestTriangle, IsTriangleLarge), LensFlare 23→24 (CreateFan), LODModel 75→76 (BuildTexturePath), OBBNode 37→38
  (CompareSortKeys), SetClass 112→114 (GetDetailTexturePath, CompareDepthEdges), CameraMode 215→216
  (NebulaClearDistance), NiOBBLeaf 9→12 (all four), NiRendererDialog 6→10 (all five). Lineage confirms it for
  NiOBBLeaf: Gamebryo 1.1.2's `NICOLLISION11VC60.LIB` exports them as protected static members,
  `?ComputeBarycenter@NiOBBLeaf@@KAXABVNiPoint3@@000QAM@Z`, `TentativeCollision`, `TestEdgeTri` and two
  `FindIntersection` overloads (our ComputeBarycenter, FindSegmentContact, FindEdgeContact, FindTriangleContact by
  their parameter lists). Retail statics placed after their first caller (CollisionTestDisplay's TestPoint,
  NiClusterSorter's CompareDepth, NiSGIReader's GetByteChannel, NiDDImage's NextMipmapLevel) fit either linkage.
  Which class owns each of the game ones is *(open)*: an identity question for the class's lane, not an order fix.
- **A vftable group** (the vftable, then its inline virtuals, then `??_G`) follows the constructor or destructor whose
  vptr store survives optimisation. Its inline virtuals follow the source order of their bodies, not the slot order:
  inherited, non-overridden bodies first (the base is earlier in the source), then the class's own in declaration
  order, `??_G` last; then the base's group. Scratch (`D : A`, D declares `N1, A3, N2, A1`): `MakeD`, `A::A2`,
  `D::N1, A3, N2, A1`, `??_GD`, `A::A1, A3`, `??_GA`, then the next function. **Under `/GX` the implicit destructor
  of `D` (a compiler-generated `??1D` storing A's vptr) is called by `??_GD` and emitted right after it**; without
  `/GX` it is expanded into `??_GD`, as bw1-decomp's (`/GR`, no `/GX`) build shows.
- **`??_E`**: a vftable's slot 0 refers to a weak `??_E` that defaults to `??_G`; only a unit with `new X[n]` defines a
  strong `??_E` (with the vector constructor and destructor iterators) and no `??_G` (bw1-decomp; not re-run).
- **`.bss` without a precompiled header is in a hash order of the names**, the same whatever the definition order
  (scratch: `Alpha, Beta, Gamma, Delta, Cls::Mem, Decl` in two definition orders both gave `Mem, Gamma, Beta, Decl,
  Alpha, Delta`). With `/Yu` the order is the reverse of each symbol's first mention in the unit (bw1-decomp). Our
  `.bss` is placed at retail's address datum by datum, so this decides nothing we gate, but a real link would lay it
  out in the object's order. `tools/linkready.py` compares the two over every `.bss` datum an exact function
  reaches: the units with no PCH and the original names (Python, the CRT, the C++ runtime) are in retail's order in
  all 35 measured, so the hash order holds there; the game and NetImmerse units, built with `/YX` since 2026-11-18,
  are in another order in 552 of 591 (PyEmbed's statics are neither in hash, definition nor reverse order; its bools
  are packed together). Whether the original game build's order follows its names' hash, a PCH's first mentions or
  something else is *(open)*: renamed placeholders and missing statics both move it.
- **With a PCH** (bw1-decomp): the PCH's inline bodies are emitted when the unit's own IL stream passes their offset
  in the PCH's stream, so they come after the unit's own bodies in a small unit and in the middle of a large one;
  `_$E` numbers continue from the PCH's. The game and NetImmerse units build with `/YX` (the user's ruling,
  2026-11-18). A whole-tree A/B of their code order against retail's (`tools/linkready.py` over two builds, 2026-11-19):
  509 of 607 units are in the same order either way; NetImmerse is closer to retail with `/YX` in 29 units and
  without it in 1; the game is closer with `/YX` in 27 and without it in 41 (TGActionModule: 37 of 76 sections in
  retail's order with `/YX`, 68 without, its inline copies following their first callers as retail has them). Matches
  moved in 11 units (21,853 with `/YX`, 21,844 without). So the PCH mode explains a minority of the order differences;
  most are the source's definition order. Which game units the original built with a PCH, and where its boundary
  was, is *(open)*.
- **What places a PCH's inline copies** (2026-11-20, scratch with three headers and address-taken inlines; real units
  through a probe build). Three modes differ in a small scratch: no PCH, each copy right after its first caller; the
  compile that *creates* the `.pch` (what our build does, a fresh `.pch` per unit), every PCH copy at the end of the
  unit in header order; a compile that *uses* an existing `.pch`, the copies spread through the unit by the IL
  stream's offset. In real units creating and using give the same object (TGActionModule compiled twice against its
  own `.pch`: identical section order; a whole-tree build that way: identical matches and order), so a shared
  project `.pch` would change nothing by reuse. Nor does a `.pch` seeded by another unit: `/YX` recreates it when the
  unit's leading includes differ (TGActionModule after a seed of `<iostream>` + TGAction.h: `.pch` rewritten, 38 of 76
  as before). With `/YX`, then, a unit's PCH copies follow their first callers only for the headers after the
  boundary, and the boundary is the end of the leading include block unless the source ends it earlier (`#pragma
  hdrstop`, or code before an include). TGActionModule's sweep of `#pragma hdrstop` positions: 69 of 76 in retail's
  order with the boundary before TGAnimAction.h, falling to 63, 57, 49, 43 and 38 as each later header enters the
  PCH; matches unchanged. SP4 (c1xx 8867, c2 8799) gives the same order and bytes as SP5 in TGActionModule,
  UtopiaModule, TGAssert and OBBNode.
- **A/B over the tree** (sections with a retail address in retail's order, `comdat.py order`'s rising run summed
  over the units; matches without the `_$E` stubs, whose numbers move with the includes):

  | game units built | game in order (of 15,506) | units better / worse than `/YX` | matches vs `/YX` |
  |---|---|---|---|
  | `/YX` (today) | 14,448 | | |
  | no PCH (= `#pragma hdrstop` before the first include) | 14,792 | 42 / 28 (+395 / −51 sections) | +3 −5 |
  | PCH of NiMain.h only, then `#pragma hdrstop` | 14,795 | 43 / 28 (+397 / −50) | +4 −6 |

  NetImmerse: 3,028 of 3,128 with `/YX`, 2,991 without (27 units better with it, 1 without); its matches lose 8
  without it. The game's gain is mostly three registration units (UtopiaModule +188, InterfaceModule +42,
  TGActionModule +31); the units better with `/YX` gain 1 to 6 each (TGSoundModule, PlacementEditor). The matches that
  move are declaration-count flips, not order: without the PCH the game loses MetaVolumePolygonizer::CreateUnknown138,
  PhaserProperty's Unknown686C80 and Unknown687A60, PositionOrientationProperty::Unknown689830,
  ViewScreenNode::SetOffTexture (and BinaryVoxelizer's ~BinaryVoxelizer::BoundaryData with NiMain.h alone) and gains
  ServerListCallBack, DisableGlossMaps, GetArcPosition (and _wrap_ObjectGroup_ForceToGroup with NiMain.h);
  PhaserProperty's sweep holds its two only with TGPointerMap.h, and then NiMain.h, inside the PCH, whatever the
  order. So no one setting reproduces retail's copy placement: it is a per-unit boundary, the original's include
  block, which our reconstructed include order doesn't carry. *(open)*: whether a unit's boundary can be placed from
  its retail order (the last header whose copies sit at the end) with a real `#pragma hdrstop` or include order as
  the evidence, and whether that is admissible source.
  **Per-unit `/YX` (`auto_pch_units`, src/game).** A unit is added only when its matches improve with `/YX` (none lost
  anywhere) and its sections in retail's order increase (functions plus `_$E` stubs, `comdat.py order`). Whole-game
  sweep of every `src/game` unit, built with and without `/YX`; the units whose matches moved:

  | unit | matches without -> with | in retail's order (functions + stubs) without -> with | verdict |
  |---|---|---|---|
  | game/DamagePolygonizer (CreateUnknown138 0x4C2940) | +1 (51 -> 52 in the unit's own count) | 97 -> 98 | added |
  | game/PhaserProperty (Unknown686C80, Unknown687A60, Unknown687730) | +3 | 126 -> 126 | not added: order unchanged |
  | game/PositionOrientationProperty (Unknown689830) | +1 | 115 -> 115 | not added: order unchanged |
  | game/ViewScreenNode (SetOffTexture 0x676B20) | +1 | 116 -> 116 | not added: order unchanged |
  | game/GameSpyServerList (ServerListCallBack 0x69CF30) | -1 | 116 -> 116 | not added: match lost |
  | game/PhaserBank (GetArcPosition 0x571AB0) | -1 | 155 -> 155 | not added: match lost |
  | game/NetFile (0x6A4560) | -1 | 170 -> 167 (functions 60 -> 53, stubs 110 -> 114) | not added: match and function order lost |
  | game/ModelHelpers (DisableGlossMaps 0x45CD20) | 0 | 121 -> 121 | not added: no gain (the full-tree sweep's flip is absent in this tree) |

  No other game unit's matches change. Most units' order moves by a few sections either way (135 units with `/YX`
  better, 82 worse, by stubs mostly; UtopiaModule 500 -> 308 and STColors 180 -> 280 are the large ones), which
  alone doesn't qualify a unit.

- **What doesn't change the order**: bw1-decomp found RTTI order follows the front end's heap addresses (the checkout
  path's length moves it); we build without `/GR`, so there is no RTTI to order.

## Not read yet

- The scheduler's memory disambiguation itself (in `sub_107396F6`'s callees): the rules under "Scheduling" are
  measured on scratch compiles, not read. Open: when a frame object's store and a pointer load are ordered (retail
  orders TGConsole::KeepInView's copy of a TGRect through GetBounds' `*rect = bounds`, ours doesn't), and why an
  inline parameter bound to a variable keeps its own alias class while one bound to `&local` folds.
- The order the register allocator emits loads at a block's head (a loop's entry): `c2order` finds ties among
  them decided by input order (MetaSurfacePolygonizer::ProcessMesh, DamageVoxelGrid::Overlaps); on a scratch loop
  the first-used value takes the first register and is loaded first, but retail and ours differ there with the same
  registers, so the order has another key.
- The front end's tuples for constructs not measured above (EH, `new`/`delete`, temporaries of class type, string
  literals): measure them with `c2trace --cost` on a scratch file.
- What sets a local's id (and so its place in the frame list) when the front end creates it: byte-tactics found
  size, then reference count for VC5's list; VC6's observed order is creation order.
- The rest of fppeeps.c (the 0x5C-0x5E path and `sub_10763C4E`), the EH candidate list's exact lifetime, the
  temporary allocator's rotation (regasg.c), EH temporaries (ehgen.c), and how the declaration count reaches a frame
  or register choice (docs/levers/walls.md: modulo 64 in a front-end count; the operand sort's symbol numbers above are
  C2's own, per function; byte-tactics: symbol +0x28, ids from one file-wide counter in VC5).
