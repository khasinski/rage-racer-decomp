#include "common.h"


/*
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 *
 * Symbol:   SquareRoot0
 * Address:  0x800689A8 (PAL/main, retail range [0x591A8, 0x5922C))
 * Reason:   GTE-LZC fixed-point square-root helper. Reads leading-sign-bit
 *           count from LZCR ($31), normalises the operand by an even shift,
 *           looks up a mantissa table at 0x80094B1C, then rescales by half the
 *           exponent. The algorithm is expressible in C, but the retail bytes
 *           are not compiler output.
 * Evidence:
 *   - `li at,32; beq v0,at` holds $1 (assembler temp) as a live comparison
 *     operand across the branch. GCC reserves $1 and never allocates it to a
 *     value -> invalid register for compiler-generated C.
 *   - Trapping `addi`/`sub` (4x: 0x2c,0x34,0x4c,0x54). The rebuilt cc1-psx
 *     (both 2.6.3 and 2.7.2) only ever emits non-trapping `addiu`/`subu`;
 *     verified against every committed matched function.
 *   - Dead `andi t0,v0,1` (0x1c): result never consumed; no -O2 compiler
 *     emits a pure dead computation here.
 *   - Unconditional `b` (beq $0,$0) at 0x44 where the compiler emits `j`.
 *   - Unfilled branch delay slots (nop at 0x18 after beq, 0x3c after bltz).
 *   - Two independent `jr ra` epilogues (0x74 and 0x7c) rather than a single
 *     shared/cross-jumped return.
 * Why C+PSYQ macros are insufficient: the trapping arithmetic and the live use
 *   of $1 cannot be produced by the available compilers, so no C source can be
 *   byte-exact. A register-pinned C rewrite reaches the correct registers and
 *   algorithm but still floors at ~22 instruction diffs from these tool-level
 *   differences.
 * Current representation: generated assembly stub (INCLUDE_ASM).
 * Revisit condition: an original PSY-Q ccpsx (trapping-arith) cc1 becomes
 *   available to the build, or the routine is confirmed as a shippable
 *   library .s.
 */
INCLUDE_ASM("asm/PAL/main/nonmatchings/lib/libgte/square_root_0", SquareRoot0);
