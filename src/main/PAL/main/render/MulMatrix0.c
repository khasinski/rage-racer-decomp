#include "common.h"

/* HANDWRITTEN_ASM - PSY-Q libgte hand-asm (matrix/GTE), excluded from progress (docs/ASM_AND_GTE_POLICY.md). */


void *MulMatrix0(s32 *matrix, void *src, void *dst) asm("func_80068B98");
void *MulMatrix0(s32 *matrix, void *src, void *dst) {
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
        "lhu $8,0($5)\n"
        "lw $9,4($5)\n"
        "lw $10,12($5)\n"
        "lui $1,0xFFFF\n"
        "and $9,$9,$1\n"
        "or $8,$8,$9\n"
        "mtc2 $8,$0\n"
        "mtc2 $10,$1\n"
        "nop\n"
        "cop2 0x486012\n"
        "lhu $8,2($5)\n"
        "lw $9,8($5)\n"
        "lh $10,14($5)\n"
        "sll $9,$9,16\n"
        "or $8,$8,$9\n"
        "mfc2 $11,$9\n"
        "mfc2 $12,$10\n"
        "mfc2 $13,$11\n"
        "mtc2 $8,$0\n"
        "mtc2 $10,$1\n"
        "nop\n"
        "cop2 0x486012\n"
        "lhu $8,4($5)\n"
        "lw $9,8($5)\n"
        "lw $10,16($5)\n"
        "lui $1,0xFFFF\n"
        "and $9,$9,$1\n"
        "or $8,$8,$9\n"
        "mfc2 $14,$9\n"
        "mfc2 $15,$10\n"
        "mfc2 $24,$11\n"
        "mtc2 $8,$0\n"
        "mtc2 $10,$1\n"
        "nop\n"
        "cop2 0x486012\n"
        "andi $11,$11,0xFFFF\n"
        "sll $14,$14,16\n"
        "or $14,$14,$11\n"
        "sw $14,0($6)\n"
        "andi $13,$13,0xFFFF\n"
        "sll $24,$24,16\n"
        "or $24,$24,$13\n"
        "sw $24,12($6)\n"
        "mfc2 $8,$9\n"
        "mfc2 $9,$10\n"
        "andi $8,$8,0xFFFF\n"
        "sll $12,$12,16\n"
        "or $8,$8,$12\n"
        "sw $8,4($6)\n"
        "andi $15,$15,0xFFFF\n"
        "sll $9,$9,16\n"
        "or $9,$9,$15\n"
        "sw $9,8($6)\n"
        "swc2 $11,16($6)"
        :
        : "r"(m0), "r"(m1), "r"(m2), "r"(m3), "r"(m4), "r"(src), "r"(dst)
        : "memory");
    asm volatile("move $2,$6");
}

/* HANDWRITTEN_ASM - PSY-Q libgte hand-asm (matrix/GTE), excluded from progress (docs/ASM_AND_GTE_POLICY.md). */


void *func_80068CA4(void *arg0, void *arg1) {
    asm volatile(
        "lhu $8,0($4)\n"
        "lw $9,4($4)\n"
        "lw $10,12($4)\n"
        "lui $1,0xFFFF\n"
        "and $9,$9,$1\n"
        "or $8,$8,$9\n"
        "mtc2 $8,$0\n"
        "mtc2 $10,$1\n"
        "nop\n"
        "cop2 0x486012\n"
        "lhu $8,2($4)\n"
        "lw $9,8($4)\n"
        "lh $10,14($4)\n"
        "sll $9,$9,16\n"
        "or $8,$8,$9\n"
        "mfc2 $11,$9\n"
        "mfc2 $12,$10\n"
        "mfc2 $13,$11\n"
        "mtc2 $8,$0\n"
        "mtc2 $10,$1\n"
        "nop\n"
        "cop2 0x486012\n"
        "lhu $8,4($4)\n"
        "lw $9,8($4)\n"
        "lw $10,16($4)\n"
        "lui $1,0xFFFF\n"
        "and $9,$9,$1\n"
        "or $8,$8,$9\n"
        "mfc2 $14,$9\n"
        "mfc2 $15,$10\n"
        "mfc2 $24,$11\n"
        "mtc2 $8,$0\n"
        "mtc2 $10,$1\n"
        "nop\n"
        "cop2 0x486012\n"
        "andi $11,$11,0xFFFF\n"
        "sll $14,$14,16\n"
        "or $14,$14,$11\n"
        "sw $14,0($5)\n"
        "andi $13,$13,0xFFFF\n"
        "sll $24,$24,16\n"
        "or $24,$24,$13\n"
        "sw $24,12($5)\n"
        "mfc2 $8,$9\n"
        "mfc2 $9,$10\n"
        "andi $8,$8,0xFFFF\n"
        "sll $12,$12,16\n"
        "or $8,$8,$12\n"
        "sw $8,4($5)\n"
        "andi $15,$15,0xFFFF\n"
        "sll $9,$9,16\n"
        "or $9,$9,$15\n"
        "sw $9,8($5)\n"
        "swc2 $11,16($5)"
        :
        : "r"(arg0), "r"(arg1)
        : "memory");
    asm volatile("move $2,$5");
}

/* HANDWRITTEN_ASM - PSY-Q libgte hand-asm (matrix/GTE), excluded from progress (docs/ASM_AND_GTE_POLICY.md). */


void *func_80068D88(void *arg0) {
    asm volatile(
        "lw $8,0($4)\n"
        "lw $9,4($4)\n"
        "lw $10,12($4)\n"
        "andi $8,$8,0xFFFF\n"
        "lui $1,0xFFFF\n"
        "and $9,$9,$1\n"
        "or $8,$8,$9\n"
        "mtc2 $8,$0\n"
        "mtc2 $10,$1\n"
        "nop\n"
        "cop2 0x486012\n"
        "lhu $8,2($4)\n"
        "lw $9,8($4)\n"
        "lh $10,14($4)\n"
        "sll $9,$9,16\n"
        "or $8,$8,$9\n"
        "mfc2 $11,$9\n"
        "mfc2 $12,$10\n"
        "mfc2 $13,$11\n"
        "mtc2 $8,$0\n"
        "mtc2 $10,$1\n"
        "nop\n"
        "cop2 0x486012\n"
        "lhu $8,4($4)\n"
        "lw $9,8($4)\n"
        "lw $10,16($4)\n"
        "lui $1,0xFFFF\n"
        "and $9,$9,$1\n"
        "or $8,$8,$9\n"
        "mfc2 $14,$9\n"
        "mfc2 $15,$10\n"
        "mfc2 $24,$11\n"
        "mtc2 $8,$0\n"
        "mtc2 $10,$1\n"
        "nop\n"
        "cop2 0x486012\n"
        "andi $11,$11,0xFFFF\n"
        "sll $14,$14,16\n"
        "or $14,$14,$11\n"
        "sw $14,0($4)\n"
        "andi $13,$13,0xFFFF\n"
        "sll $24,$24,16\n"
        "or $24,$24,$13\n"
        "sw $24,12($4)\n"
        "mfc2 $8,$9\n"
        "mfc2 $9,$10\n"
        "andi $8,$8,0xFFFF\n"
        "sll $12,$12,16\n"
        "or $8,$8,$12\n"
        "sw $8,4($4)\n"
        "andi $15,$15,0xFFFF\n"
        "sll $9,$9,16\n"
        "or $9,$9,$15\n"
        "sw $9,8($4)\n"
        "swc2 $11,16($4)"
        :
        : "r"(arg0)
        : "memory");
    asm volatile("move $2,$4");
}

/* HANDWRITTEN_ASM - PSY-Q libgte hand-asm (matrix/GTE), excluded from progress (docs/ASM_AND_GTE_POLICY.md). */


void *func_80068E70(s32 *matrix, void *src) {
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
        "lhu $8,0($5)\n"
        "lw $9,4($5)\n"
        "lw $10,12($5)\n"
        "lui $1,0xFFFF\n"
        "and $9,$9,$1\n"
        "or $8,$8,$9\n"
        "mtc2 $8,$0\n"
        "mtc2 $10,$1\n"
        "nop\n"
        "cop2 0x486012\n"
        "lhu $8,2($5)\n"
        "lw $9,8($5)\n"
        "lh $10,14($5)\n"
        "sll $9,$9,16\n"
        "or $8,$8,$9\n"
        "mfc2 $11,$9\n"
        "mfc2 $12,$10\n"
        "mfc2 $13,$11\n"
        "mtc2 $8,$0\n"
        "mtc2 $10,$1\n"
        "nop\n"
        "cop2 0x486012\n"
        "lhu $8,4($5)\n"
        "lw $9,8($5)\n"
        "lw $10,16($5)\n"
        "lui $1,0xFFFF\n"
        "and $9,$9,$1\n"
        "or $8,$8,$9\n"
        "mfc2 $14,$9\n"
        "mfc2 $15,$10\n"
        "mfc2 $24,$11\n"
        "mtc2 $8,$0\n"
        "mtc2 $10,$1\n"
        "nop\n"
        "cop2 0x486012\n"
        "andi $11,$11,0xFFFF\n"
        "sll $14,$14,16\n"
        "or $14,$14,$11\n"
        "andi $13,$13,0xFFFF\n"
        "sll $24,$24,16\n"
        "or $24,$24,$13\n"
        "mfc2 $8,$9\n"
        "mfc2 $9,$10\n"
        "mfc2 $10,$11\n"
        "andi $8,$8,0xFFFF\n"
        "sll $12,$12,16\n"
        "or $8,$8,$12\n"
        "andi $15,$15,0xFFFF\n"
        "sll $9,$9,16\n"
        "or $9,$9,$15\n"
        "ctc2 $14,$0\n"
        "ctc2 $8,$1\n"
        "ctc2 $9,$2\n"
        "ctc2 $24,$3\n"
        "ctc2 $10,$4"
        :
        : "r"(m0), "r"(m1), "r"(m2), "r"(m3), "r"(m4), "r"(src)
        : "memory");
    asm volatile("move $2,$4");
}

/* HANDWRITTEN_ASM - PSY-Q libgte hand-asm (matrix/GTE), excluded from progress (docs/ASM_AND_GTE_POLICY.md). */


void *ApplyMatrixLV(void *mtx, void *vec, void *out) asm("func_80068F80");
void *ApplyMatrixLV(void *mtx, void *vec, void *out) {
    void *m = mtx;
    void *v = vec;
    /* This pin is load-bearing: removing it changes .text. */
    register void *o asm("$6") = out;

    asm volatile(
        ".set\tnoreorder\n"
        ".set\tnoat\n"
        "lw $8,0($4)\n"
        "lw $9,4($4)\n"
        "lw $10,8($4)\n"
        "lw $11,12($4)\n"
        "lw $12,16($4)\n"
        "ctc2 $8,$0\n"
        "ctc2 $9,$1\n"
        "ctc2 $10,$2\n"
        "ctc2 $11,$3\n"
        "ctc2 $12,$4\n"
        "lw $8,0($5)\n"
        "lw $9,4($5)\n"
        "lw $10,8($5)\n"
        "bgez $8,1f\n"
        "sra $11,$8,0xf\n"
        "negu $8,$8\n"
        "sra $11,$8,0xf\n"
        "andi $8,$8,0x7fff\n"
        "negu $11,$11\n"
        "b 2f\n"
        "negu $8,$8\n"
        "sra $11,$8,0xf\n"
        "1:\n"
        "andi $8,$8,0x7fff\n"
        "2:\n"
        "bgez $9,3f\n"
        "sra $12,$9,0xf\n"
        "negu $9,$9\n"
        "sra $12,$9,0xf\n"
        "andi $9,$9,0x7fff\n"
        "negu $12,$12\n"
        "b 4f\n"
        "negu $9,$9\n"
        "sra $12,$9,0xf\n"
        "3:\n"
        "andi $9,$9,0x7fff\n"
        "4:\n"
        "bgez $10,5f\n"
        "sra $13,$10,0xf\n"
        "negu $10,$10\n"
        "sra $13,$10,0xf\n"
        "andi $10,$10,0x7fff\n"
        "negu $13,$13\n"
        "b 6f\n"
        "negu $10,$10\n"
        "sra $13,$10,0xf\n"
        "5:\n"
        "andi $10,$10,0x7fff\n"
        "6:\n"
        "mtc2 $11,$9\n"
        "mtc2 $12,$10\n"
        "mtc2 $13,$11\n"
        "nop\n"
        "cop2 0x41e012\n"
        "mfc2 $11,$25\n"
        "mfc2 $12,$26\n"
        "mfc2 $13,$27\n"
        "mtc2 $8,$9\n"
        "mtc2 $9,$10\n"
        "mtc2 $10,$11\n"
        "nop\n"
        "cop2 0x49e012\n"
        "bgez $11,7f\n"
        "nop\n"
        "negu $11,$11\n"
        "sll $11,$11,0x3\n"
        "b 8f\n"
        "negu $11,$11\n"
        "7:\n"
        "sll $11,$11,0x3\n"
        "8:\n"
        "bgez $12,9f\n"
        "nop\n"
        "negu $12,$12\n"
        "sll $12,$12,0x3\n"
        "b 10f\n"
        "negu $12,$12\n"
        "9:\n"
        "sll $12,$12,0x3\n"
        "10:\n"
        "bgez $13,11f\n"
        "nop\n"
        "negu $13,$13\n"
        "sll $13,$13,0x3\n"
        "b 12f\n"
        "negu $13,$13\n"
        "11:\n"
        "sll $13,$13,0x3\n"
        "12:\n"
        "mfc2 $8,$25\n"
        "mfc2 $9,$26\n"
        "mfc2 $10,$27\n"
        "addu $8,$8,$11\n"
        "addu $9,$9,$12\n"
        "addu $10,$10,$13\n"
        "sw $8,0($6)\n"
        "sw $9,4($6)\n"
        "sw $10,8($6)\n"
        ".set\treorder\n"
        :
        : "r"(m), "r"(v), "r"(o)
        : "memory", "$8", "$9", "$10", "$11", "$12", "$13");
    return o;
}

/* HANDWRITTEN_ASM - PSY-Q libgte hand-asm (matrix/GTE), excluded from progress (docs/ASM_AND_GTE_POLICY.md). */


s32 func_800690E0(s32 *arg0, s32 *arg1, s32 arg2) {
    /* These pins are load-bearing: removing any one changes .text. */
    register s32 xy asm("$8");
    register s32 z asm("$9");

    xy = arg0[0];
    z = arg0[1];
    asm volatile(
        "mtc2 $8,$0\n"
        "mtc2 $9,$1\n"
        "nop\n"
        "cop2 0x486012\n"
        "swc2 $9,0($5)\n"
        "swc2 $10,4($5)\n"
        "swc2 $11,8($5)"
        :
        : "r"(xy), "r"(z), "r"(arg1)
        : "memory");
    asm volatile("move $2,$6");
}

/*
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 *
 * Symbol:   func_80069110 = ScaleMatrixL (PSY-Q libgte; see include/psyq/gte.h).
 *           m[i][j] *= v[i], the row-scaling twin of ScaleMatrix
 *           (func_80069728) - Sony's Run-Time Library Reference, 8-151.
 *           Unreferenced in this image.
 * Reason:   hand-written libgte SDK assembly, not compiler C.
 * Evidence: every fixed-point product uses a NARROW unsigned multiply
 *           (multu + mflo, no mfhi) - the asm author's idiom. GCC 2.6.3 AND
 *           2.7.2 (this repo's cc1) canonicalise every truncated 32-bit
 *           multiply to signed mult; narrow multu is unreachable from C, and
 *           NO matched function in the project emits it. It also folds
 *           (s16)word / word>>16 into lh loads vs the retail's single-lw +
 *           andi/sll/sra extraction. Sibling RotMatrix = RotMatrix (14x
 *           multu), a documented PSY-Q libgte asm routine.
 * Revisit:  only if the exact gcc-2.7.2 variant that emits narrow multu is
 *           obtained AND verified not to regress already-matched functions.
 */

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/render/MulMatrix0", func_80069110);

/* HANDWRITTEN_ASM - PSY-Q libgte hand-asm (matrix/GTE), excluded from progress (docs/ASM_AND_GTE_POLICY.md). */


/* libgte PushMatrix: cfc2 $0..$7 into the matrix stack at 0x80094CAC,
 * indexed by 0x80094CA8, then index += 32; errors once index reaches
 * 0x280 (20 levels). PopMatrix (func_800692D4) is the exact inverse. */
void PushMatrix(void) asm("func_80069234");
void PushMatrix(void) {
    asm volatile(
        ".set noreorder\n"
        ".set noat\n"
        "lui $14,0x8009\n"
        "lw $14,0x4CA8($14)\n"
        "nop\n"
        "slti $1,$14,0x280\n"
        "bnez $1,1f\n"
        "lui $1,0x8009\n"
        "sw $31,0x4C9C($1)\n"
        "lui $4,0x8009\n"
        ".word 0x0C0059D3\n"
        "addiu $4,$4,0x4F2C\n"
        "lui $31,0x8009\n"
        "lw $31,0x4C9C($31)\n"
        "nop\n"
        "jr $31\n"
        "nop\n"
        "1:\n"
        "lui $15,0x8009\n"
        "addu $15,$15,$14\n"
        "addiu $15,$15,0x4CAC\n"
        "cfc2 $8,$0\n"
        "cfc2 $9,$1\n"
        "sw $8,0($15)\n"
        "sw $9,4($15)\n"
        "cfc2 $8,$2\n"
        "cfc2 $9,$3\n"
        "sw $8,8($15)\n"
        "sw $9,12($15)\n"
        "cfc2 $8,$4\n"
        "nop\n"
        "sw $8,16($15)\n"
        "cfc2 $8,$5\n"
        "cfc2 $9,$6\n"
        "cfc2 $10,$7\n"
        "sw $8,20($15)\n"
        "sw $9,24($15)\n"
        "sw $10,28($15)\n"
        "addi $14,$14,32\n"
        "lui $1,0x8009\n"
        "sw $14,0x4CA8($1)");
}
