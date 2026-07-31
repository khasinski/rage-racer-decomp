#include "common.h"
#include "psyq/gte_macros.h"

s32 *ApplyMatrix(s32 *matrix, s32 *vec, s32 *out);
/* libgte ApplyMatrixSV: SVECTOR in, SVECTOR out, returns out. */
s16 *ApplyMatrixSV(s32 *matrix, void *vec, s16 *out) asm("func_800696C8");

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
 *           Same family/representation as siblings func_80068E70, ApplyMatrixSV,
 *           ApplyMatrix, func_800690E0.
 * Revisit:  only with proof it was compiler-generated C.
 */

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/render/PopMatrix", func_80069458);

/*
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 *
 * Symbol:   func_80069568  (libgte matrix x matrix multiply, cc region 2.6.3)
 * Reason:   hand-written PSY-Q libgte routine (.set noreorder style); sibling of
 *           MulMatrix / func_80068CA4 / ApplyMatrixSV / func_800690E0.
 * Evidence: `lui $1,0xffff; and rN,rN,$1` masks twice (not CSE'd) using the
 *           reserved assembler temp $at, which cc1 never allocates; register
 *           file is t0-t8 exclusively (GCC prefers v0/v1/a0-a3); reached only
 *           DIFFS=30 as register-pinned C with an irreducible >=3 floor from the
 *           $at/non-CSE constant.
 * Revisit:  only with proof it was compiler-generated C.
 */

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/render/PopMatrix", func_80069568);

/* HANDWRITTEN_ASM - PSY-Q libgte hand-asm (matrix/GTE), excluded from progress (docs/ASM_AND_GTE_POLICY.md). */


s32 *ApplyMatrix(s32 *matrix, s32 *vec, s32 *out) asm("func_80069678");
s32 *ApplyMatrix(s32 *matrix, s32 *vec, s32 *out) {
    /* These pins are load-bearing: removing any one changes .text. */
    register s32 m0 asm("$8") = matrix[0];
    register s32 m1 asm("$9") = matrix[1];
    register s32 m2 asm("$10") = matrix[2];
    register s32 m3 asm("$11") = matrix[3];
    register s32 m4 asm("$12") = matrix[4];

    asm volatile(
        "ctc2 $8,$0\n"
        "ctc2 $9,$1\n"
        "ctc2 $10,$2\n"
        "ctc2 $11,$3\n"
        "ctc2 $12,$4\n"
        "lwc2 $0,0($5)\n"
        "lwc2 $1,4($5)\n"
        "nop\n"
        "cop2 0x486012\n"
        "swc2 $25,0($6)\n"
        "swc2 $26,4($6)\n"
        "swc2 $27,8($6)"
        :
        : "r"(m0), "r"(m1), "r"(m2), "r"(m3), "r"(m4), "r"(vec), "r"(out)
        : "memory");
    asm volatile("move $2,$6");
}

/* HANDWRITTEN_ASM - PSY-Q libgte hand-asm (matrix/GTE), excluded from progress (docs/ASM_AND_GTE_POLICY.md). */


s16 *ApplyMatrixSV(s32 *matrix, void *vec, s16 *out) {
    /* These pins are load-bearing: removing any one changes .text. */
    register s32 m0 asm("$8") = matrix[0];
    register s32 m1 asm("$9") = matrix[1];
    register s32 m2 asm("$10") = matrix[2];
    register s32 m3 asm("$11") = matrix[3];
    register s32 m4 asm("$12") = matrix[4];

    asm volatile(
        "ctc2 $8,$0\n"
        "ctc2 $9,$1\n"
        "ctc2 $10,$2\n"
        "ctc2 $11,$3\n"
        "ctc2 $12,$4\n"
        "lwc2 $0,0($5)\n"
        "lwc2 $1,4($5)\n"
        "nop\n"
        "cop2 0x486012\n"
        "mfc2 $8,$9\n"
        "mfc2 $9,$10\n"
        "mfc2 $10,$11\n"
        "sh $8,0($6)\n"
        "sh $9,2($6)\n"
        "sh $10,4($6)"
        :
        : "r"(m0), "r"(m1), "r"(m2), "r"(m3), "r"(m4), "r"(vec), "r"(out)
        : "memory");
    asm volatile("move $2,$6");
}

u32 func_80069724 __attribute__((section(".text"))) = 0;
