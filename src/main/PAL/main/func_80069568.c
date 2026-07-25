#include "common.h"

/*
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 *
 * Symbol:   func_80069568  (libgte matrix x matrix multiply, cc region 2.6.3)
 * Reason:   hand-written PSY-Q libgte routine (.set noreorder style); sibling of
 *           func_80069458 / func_80068CA4 / func_800696C8 / func_800690E0.
 * Evidence: `lui $1,0xffff; and rN,rN,$1` masks twice (not CSE'd) using the
 *           reserved assembler temp $at, which cc1 never allocates; register
 *           file is t0-t8 exclusively (GCC prefers v0/v1/a0-a3); reached only
 *           DIFFS=30 as register-pinned C with an irreducible >=3 floor from the
 *           $at/non-CSE constant.
 * Revisit:  only with proof it was compiler-generated C.
 */

INCLUDE_ASM("asm/nonmatchings/PAL/main", func_80069568);
