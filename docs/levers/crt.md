# The C and C++ runtimes

What the CRT, the C++ EH runtime and the floating-point library need beyond the VC6 sources. Read `README.md` for
how entries are written.

## Runtime reconstruction

- **VC6 ships no source for the C++ EH runtime or the floating-point library** (frame.cpp, trnsctrl.cpp,
  typinfo.cpp, cvt.c, util.c ...): their function names, order and statics come from the symbol tables of VC6's own
  `FRAME.OBJ`, `TRNSCTRL.OBJ` and the like in `VC98/CRT/SRC/PLATFORM`, and the bodies from retail.
- **Where an `__asm` block takes a C label's address** (`mov x, offset Label`), the original compiled that function
  under `#pragma optimize( "g", off )` and switched it back on right after (trnsctrl's `_UnwindNestedFrames`,
  `_CallSETranslator`; their neighbours match only with it on).
- **VC6 itself names the vector constructor and destructor iterators `??_L` and `??_M`:** the CRT spelling
  `__ehvec_ctor`/`__ehvec_dtor` compiles to the right symbol without tricks.
- **A LIBCMT member whose masked bytes match no C source here is the `.asm` version** (strncmp): list it as
  ORIGINAL-ASM (AGENTS.md Policies), don't tune the C.
- **The floating-point library units fpexcept, ieeemisc and util are `/Op`** (docs/toolchain.md): the tells are each
  `?:` result or FP assignment stored to a frame temporary and reloaded, and FP comparisons reading their operands
  from memory. *Evidence required:* every function of the unit matching under the flag.
- **fpexcept copies doubles as bits** (`*(u_double *)&exc.arg1 = *(u_double *)&arg1`) so a signaling NaN never goes
  through the FPU: that is what retail's integer moves in `_umatherr` are.
- **VC6 merges identical switch-case tails by jumping from the earlier case into the later one:** the jump's
  direction gives the cases' source order (`_raise_exc`'s rounding switch).
- **The ORIGINAL-ASM routines were found mechanically:** every LIBCMT member assembled by MASM 6.13.7299
  (`@comp.id` 0x000E1C83) whose `.text`, relocations masked, occurs exactly once in retail. Identical objects are told
  apart by where retail's callers go (memcpy 0x85FFD0 against memmove 0x859530).
