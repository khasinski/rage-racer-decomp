#include "common.h"
#include "psyq/gte_macros.h"

/*
 * libgte ApplyMatrix / ApplyMatrixSV (func_80069678 / func_800696C8).
 * v1 = m * v0 through MVMVA; ApplyMatrix stores a LONG vector, ApplyMatrixSV a
 * SHORT vector. Not byte-matchable to a named PSY-Q 3.5 object symbol, so this
 * TU keeps its descriptive name rather than an mtx_NN.o label.
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 */

s32 *ApplyMatrix(s32 *matrix, s32 *vec, s32 *out);
/* libgte ApplyMatrixSV: SVECTOR in, SVECTOR out, returns out. */
s16 *ApplyMatrixSV(s32 *matrix, void *vec, s16 *out) asm("func_800696C8");

s32 *ApplyMatrix(s32 *matrix, s32 *vec, s32 *out) asm("func_80069678");
s32 *ApplyMatrix(s32 *matrix, s32 *vec, s32 *out) {
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
