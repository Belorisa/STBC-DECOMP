# Triage by wall class

What to check first for a function that won't close, per wall class: the class `build` prints under the DIFF row
(`tools/walls.py`) or the packet names. Work down the list; each step links to the entries that back it. The order
comes from what closed functions: structure first (loops and branches, accessors, types, lineage bodies, unit
order), then the tools that decide early, and codegen spellings (declaration order, operand order, named temporaries)
last. Read `README.md` for how lever entries are written; this file has no lever entries of its own.

## The common order

1. **Loop and branch structure.** Compare the blocks before the instructions: an unrotated or rotated loop, a tail
   VC6 merged or kept apart, a test written the other way round. A schedule or operand difference next to a structure
   difference is often its consequence. [control-flow.md](control-flow.md): [loops](control-flow.md#control-flow-loops),
   [branch order](control-flow.md#control-flow-branch-order-and-layout),
   [returns and tails](control-flow.md#control-flow-returns-tails-and-merged-code); x87.md's "Fix the control flow
   before the x87 order" ([x87.md](x87.md#x87-operand-order-which-local-holds-a-value)).
2. **Accessor against member read,** per use, not as a sweep (a sweep over 86 open functions matched none):
   [inline.md, accessors](inline.md#spelling-that-changes-the-inline-and-call-set-accessors); the FPU copy rules in
   [x87.md](x87.md#floats-copied-through-the-fpu); the budget side in
   [inline.md, the inline budget](inline.md#the-inline-budget) ("Accessor calls count against the budget").
3. **Types from the SWIG wrapper and the callers:** the return type (the wrapper's result cast), the parameter type
   (`bool` against `int`, `NiPoint3 *` against `TGPoint3 *`, a common base), whether a point is TGPoint3 or NiPoint3
   (the EH frame tells). [data.md, SWIG wrappers](data.md#constants-data-and-referents-swig-wrappers);
   [class-model.md, types](class-model.md#the-class-model-types-from-the-code).
4. **The lineage body and missing class levels:** the Gamebryo 1.1.2 `.INL`/header body of every inline the function
   expands, a setter that calls `IncRevisionID()`/`SetChanged()`, a load-record level the CreateObject needs.
   [lineage.md](lineage.md#gamebryo-lineage-source-112-sdk-headers-122-source); the load-record entry in
   [inline.md, the inline budget](inline.md#the-inline-budget).
5. **Unit order and unit boundaries:** what precedes the function in retail's unit decides dropped EH states and
   frames; a guard byte, a quad or a second run says the `.cpp` is two units. `tools/units.py`;
   [walls.md, EH frames and unit order](walls.md#walls-that-are-bounded-eh-frames-and-unit-order);
   [statics.md, unit boundaries](statics.md#static-initialisers-and-unit-layout-unit-boundaries).
6. **The tools that decide early:** `tools/c2trace.py <addr>` (the back end's own numbers: each inline site's cost,
   budget and verdict; `--regs`, `--frame`, `--cost`; rules in `docs/c2-backend.md`), `tools/callset.py fn <addr>`
   (which shared inline's call set disagrees: an inline-budget map across functions), `tools/framemap.py <addr>` (frame or not: a reading of "frame and callee-saved
   registers agree" means declaration order is not the lever; other readings name the lever), `tools/units.py`,
   `tools/similar.py` (a matched twin) and `tools/sibling.py`.
7. **Only then codegen spellings:** named locals, declaration order, operand order, parenthesisation, named
   temporaries ([locals.md](locals.md), [x87.md](x87.md)).

## Register and frame walls

1. Run `tools/framemap.py <addr>` before anything else (retail's slots and registers beside ours), then
   `tools/c2trace.py <addr> --regs --frame` for why ours came out as they did: each candidate's priority and allowed
   registers, each local's size, reference count and slot. "Frame and callee-saved registers agree": the difference is
   in scratch registers, the schedule or an expression; skip declaration order and go to 2. A slot shared, reused,
   swapped or missing: the reading names the lever
   ([locals.md, declaration order and frame slots](locals.md#locals-and-evaluation-order-declaration-order-and-frame-slots),
   [temporaries](locals.md#locals-and-evaluation-order-temporaries-and-returned-values)). An FPU copy: go to the x87
   list. An EH state count: go to step 5 of the common order.
2. Loop and branch structure (common order 1): a register swap is often a block laid out differently.
3. Accessor against member read (common order 2), then the evaluation order of calls: a nested call or a load retail
   makes first is a named local
   ([locals.md, named locals and call order](locals.md#locals-and-evaluation-order-named-locals-and-call-order)).
4. Types (common order 3): an `int` where retail has `unsigned short`, a `bool` result passed through.
5. Declaration order, block scope, one variable reused across loops: only when framemap shows a frame difference.
6. **Stop and park** when the frame agrees and several spellings were A/B'd without a change, or when retail's own
   sites disagree with each other (the NiTListBlock operand order); when the function flips with headers only
   (declaration-sensitive: never tune headers); when the residue is a `SetName`-style swap. A NetImmerse register
   wall is compared with its SDK debug copy first (`tools/sdkdebug.py`): several closed there. [walls.md, register, frame and schedule](walls.md#walls-that-are-bounded-register-frame-and-schedule).

## x87 walls

1. Loop and branch structure first: wrong-order `fadd`s can follow from a block difference.
2. Accessor against member read on the float operands (`fld; fld; fcompp` against `fcomp [mem]`, `top + GetHeight()`),
   and whether retail copies through the FPU: [x87.md, floats copied through the FPU](x87.md#floats-copied-through-the-fpu)
   (the nesting rule, the destination rule).
3. The lineage body of the vector inline (UnitizeVector's explicit sum, NiSqrt): [lineage.md](lineage.md).
4. Operand order: which side is evaluated first (compares, NiPoint3 operators, sums of squares), the
   parenthesisation of a product, a literal against a `static const`
   ([x87.md, compares](x87.md#x87-operand-order-compares), [sums and vectors](x87.md#x87-operand-order-sums-products-and-vectors);
   [data.md, float constants](data.md#constants-data-and-referents-float-and-integer-constants)).
5. The identity of a local: a reused local, a reciprocal in place, a repeated expression
   ([x87.md, which local holds a value](x87.md#x87-operand-order-which-local-holds-a-value)).
6. **Stop and park** on an x-only flip in a commutative sum, the Processor Pack reciprocal, an FPU copy of an unnamed
   one-level value, or when each operand order has been A/B'd once: [walls.md, x87](walls.md#walls-that-are-bounded-x87).

## Inline walls

1. `tools/callset.py fn <addr>`: which out-of-line calls differ, and whether the same callee disagrees in other open
   functions (one shared inline body to fix rather than many functions). Then `tools/c2trace.py <addr>`: at the site
   that differs, read the callee's cost and the budget share it got, and write down what retail needed ("a share
   under 68: 3 fewer units, or one more site after it"). `--cost` prices a spelling in IL tuples before you try it
   (`docs/c2-backend.md`). Put the arithmetic on the card even without a match.
2. Loop and branch structure (a duplicated tail can hold an extra inline site).
3. Accessor against member read, and accessors in place of field reads in the inlines themselves: each accessor is an
   inline site ([inline.md, accessors](inline.md#spelling-that-changes-the-inline-and-call-set-accessors),
   [the inline budget](inline.md#the-inline-budget)).
4. Types: a by-value class argument or return blocks expansion, and TGPoint3 against NiPoint3 changes the overload
   ([inline.md, where an inline is defined](inline.md#spelling-that-changes-the-inline-and-call-set-where-an-inline-is-defined),
   [vector expressions](inline.md#spelling-that-changes-the-inline-and-call-set-vector-expressions)).
5. The lineage body of every inline involved, and missing class or load-record levels
   ([lineage.md](lineage.md#gamebryo-lineage-source-112-sdk-headers-122-source)).
6. Where the inline is defined: header, `.cpp` of its own unit, or another unit; and unit order
   ([inline.md, where an inline is defined](inline.md#spelling-that-changes-the-inline-and-call-set-where-an-inline-is-defined);
   `tools/units.py`).
7. A helper the original called, or the opposite, a block written out flat
   ([inline.md, helpers](inline.md#spelling-that-changes-the-inline-and-call-set-helpers)); the shape of a vector
   expression.
8. **Stop and park** when the differing call is a budget effect late in a large function (expanded at the first sites,
   out of line at the last), when several spellings moved the budget without closing it, or when a header-wide change
   would be needed (a shared header body used at hundreds of sites, as TGPOOL's `operator new` was): [walls.md, inline budget](walls.md#walls-that-are-bounded-inline-budget).

## Control-flow walls

1. Loops: `while ( true )` against `for ( ;; )`, the `if ( p ) { while ( true ) ... }` walk, countdown forms
   ([control-flow.md, loops](control-flow.md#control-flow-loops)); expect few gains from flipping alone.
2. Branch order and epilogues: which test comes first, the positive nesting with one shared return, a guard as an
   early return ([branch order](control-flow.md#control-flow-branch-order-and-layout),
   [returns and tails](control-flow.md#control-flow-returns-tails-and-merged-code)).
3. Merged or duplicated code: identical tails VC6 sinks or cross-jumps; calls written in each arm
   ([returns and tails](control-flow.md#control-flow-returns-tails-and-merged-code)).
4. Choices and bool results: `?:` against if/else, the `? true : false` idiom
   ([conditions and bool results](control-flow.md#control-flow-conditions-choices-and-bool-results)).
5. Accessors and types in the conditions (a flag accessor per test, `int` against `bool`), then the unit: the same body
   can match in one unit and not in another.
6. **Stop and park** when the block layout matches and only the rotation or one merged tail differs after the flips
   and if/else forms were tried: [walls.md, control flow](walls.md#walls-that-are-bounded-control-flow).

## Structure walls

1. The vtable: count the class's slots, name each from a compiled vtable, check overload order
   ([class-model.md, vtables and slots](class-model.md#the-class-model-vtables-and-slots)).
2. Types from the SWIG wrapper and the callers: return type, parameter types, the calling convention from `ret N` and
   `ecx` ([class-model.md, types](class-model.md#the-class-model-types-from-the-code)).
3. Constructors and destructors: init list against body, implicit against explicit, a member against a base
   ([constructors and destructors](class-model.md#the-class-model-constructors-and-destructors)).
4. Casts and type tests: `NiDynamicCast` evaluates twice, its own NULL test
   ([casts](class-model.md#the-class-model-casts-and-type-tests)).
5. The lineage class levels (1.1.2 headers), then the unit that emits the class (creators, modules).
6. **Stop and report** instead of guessing when the change is a class layout, an ownership question or a shared
   header outside the lane.

## Referent walls

1. Read the unknown or wrong referent's address: a float constant (literal, pooled entry or unit `static const`), a
   string in the pool or beside the vtables, an IAT slot, a COMDAT copy
   ([data.md](data.md#constants-data-and-referents-float-and-integer-constants),
   [strings and imports](data.md#constants-data-and-referents-strings-statics-and-imports)).
2. COMDAT copies: which unit's copy, which instantiation; annotate every unfolded copy with its `@unit`, and run
   `build -s` after adding one ([COMDAT copies](data.md#constants-data-and-referents-comdat-copies-and-claims)).
3. Static initialisers: numbering shifts and run ownership ([statics.md](statics.md)).
4. **Stop** when the referent needs an identity the evidence doesn't give: leave it REFERENT with an identity-TODO,
   never a guessed claim.
