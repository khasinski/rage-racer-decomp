#include "common.h"

/*
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 *
 * Symbol:   func_80069458  (libgte matrix x matrix multiply, cc region 2.6.3)
 * Reason:   hand-written PSY-Q libgte routine (.set noreorder style).
 * Evidence: reached DIFFS=9 as clean C + register pinning + GTE macros, but the
 *           irreducible residual is pure handwritten-asm signature:
 *             - `lui $at; and rN,rN,$at` masks: the assembler expanding an
 *               `and rN,rN,0xffff0000` pseudo-op via the reserved $at temporary;
 *               cc1 never emits $at (it materialises -65536 into a GPR).
 *             - a deliberately unfilled `jr ra; nop` delay slot (GCC -O2 always
 *               fills it), plus in-order stores.
 *           Same family/representation as siblings func_80068E70, func_800696C8,
 *           ApplyMatrix, func_800690E0.
 * Revisit:  only with proof it was compiler-generated C.
 */

INCLUDE_ASM("asm/nonmatchings/PAL/main", func_80069458);
