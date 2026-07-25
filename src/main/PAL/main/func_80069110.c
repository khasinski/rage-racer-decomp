#include "common.h"

/*
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 *
 * Symbol:   func_80069110  (PSY-Q libgte matrix routine, func_80069xxx region)
 * Reason:   hand-written libgte SDK assembly, not compiler C.
 * Evidence: every fixed-point product uses a NARROW unsigned multiply
 *           (multu + mflo, no mfhi) - the asm author's idiom. GCC 2.6.3 AND
 *           2.7.2 (this repo's cc1) canonicalise every truncated 32-bit
 *           multiply to signed mult; narrow multu is unreachable from C, and
 *           NO matched function in the project emits it. It also folds
 *           (s16)word / word>>16 into lh loads vs the retail's single-lw +
 *           andi/sll/sra extraction. Sibling func_80069D18 = RotMatrix (14x
 *           multu), a documented PSY-Q libgte asm routine.
 * Revisit:  only if the exact gcc-2.7.2 variant that emits narrow multu is
 *           obtained AND verified not to regress already-matched functions.
 */

INCLUDE_ASM("asm/nonmatchings/PAL/main", func_80069110);
