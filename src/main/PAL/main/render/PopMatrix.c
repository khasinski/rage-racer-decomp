#include "common.h"
#include "psyq/gte_macros.h"

s32 *ApplyMatrix(s32 *matrix, s32 *vec, s32 *out);
s16 *func_800696C8(s32 *matrix, void *vec, s16 *out);

/* HANDWRITTEN_ASM - PSY-Q libgte hand-asm (matrix/GTE), excluded from progress (docs/ASM_AND_GTE_POLICY.md). */


/* libgte PopMatrix: index -= 32, then ctc2 $0..$7 back from the matrix
 * stack at 0x80094CAC; errors when the index is already 0. */
void PopMatrix(void) asm("func_800692D4");
void PopMatrix(void) {
    asm volatile(
        ".set noreorder\n"
        ".set noat\n"
        "lui $14,0x8009\n"
        "lw $14,0x4CA8($14)\n"
        "nop\n"
        ".word 0x1DC0000A\n"
        "lui $1,0x8009\n"
        "sw $31,0x4C9C($1)\n"
        "lui $4,0x8009\n"
        ".word 0x0C0059D3\n"
        "addiu $4,$4,0x4F5D\n"
        "lui $31,0x8009\n"
        "lw $31,0x4C9C($31)\n"
        "nop\n"
        "jr $31\n"
        "nop\n"
        "addi $14,$14,-32\n"
        "lui $1,0x8009\n"
        "sw $14,0x4CA8($1)\n"
        "lui $15,0x8009\n"
        "addu $15,$15,$14\n"
        "addiu $15,$15,0x4CAC\n"
        "lw $8,0($15)\n"
        "lw $9,4($15)\n"
        "ctc2 $8,$0\n"
        "ctc2 $9,$1\n"
        "lw $8,8($15)\n"
        "lw $9,12($15)\n"
        "ctc2 $8,$2\n"
        "ctc2 $9,$3\n"
        "lw $8,16($15)\n"
        "nop\n"
        "ctc2 $8,$4\n"
        "nop\n"
        "lw $8,20($15)\n"
        "lw $9,24($15)\n"
        "lw $10,28($15)\n"
        "ctc2 $8,$5\n"
        "ctc2 $9,$6\n"
        "ctc2 $10,$7");
}

/* Read GTE rotation matrix + translation (control regs $0..$7) into p[0..7]. */
void func_80069374(volatile u32 *p) {
    register u32 t0 asm("$8");
    register u32 t1 asm("$9");
    register u32 t2 asm("$10");
    register u32 t3 asm("$11");
    register u32 t4 asm("$12");

    gte_cfc2(t0, 0);
    gte_cfc2(t1, 1);
    gte_cfc2(t2, 2);
    gte_cfc2(t3, 3);
    gte_cfc2(t4, 4);
    p[0] = t0;
    p[1] = t1;
    p[2] = t2;
    p[3] = t3;
    p[4] = t4;
    gte_cfc2(t0, 5);
    gte_cfc2(t1, 6);
    gte_cfc2(t2, 7);
    p[5] = t0;
    p[6] = t1;
    p[7] = t2;
}

/* Read GTE light matrix + back-color (control regs $8..$15) into p[0..7]. */
void func_800693BC(volatile u32 *p) {
    register u32 t0 asm("$8");
    register u32 t1 asm("$9");
    register u32 t2 asm("$10");
    register u32 t3 asm("$11");
    register u32 t4 asm("$12");

    gte_cfc2(t0, 8);
    gte_cfc2(t1, 9);
    gte_cfc2(t2, 10);
    gte_cfc2(t3, 11);
    gte_cfc2(t4, 12);
    p[0] = t0;
    p[1] = t1;
    p[2] = t2;
    p[3] = t3;
    p[4] = t4;
    gte_cfc2(t0, 13);
    gte_cfc2(t1, 14);
    gte_cfc2(t2, 15);
    p[5] = t0;
    p[6] = t1;
    p[7] = t2;
}

/* Read GTE color matrix + far-color (control regs $16..$23) into p[0..7]. */
void func_80069404(volatile u32 *p) {
    register u32 t0 asm("$8");
    register u32 t1 asm("$9");
    register u32 t2 asm("$10");
    register u32 t3 asm("$11");
    register u32 t4 asm("$12");

    gte_cfc2(t0, 16);
    gte_cfc2(t1, 17);
    gte_cfc2(t2, 18);
    gte_cfc2(t3, 19);
    gte_cfc2(t4, 20);
    p[0] = t0;
    p[1] = t1;
    p[2] = t2;
    p[3] = t3;
    p[4] = t4;
    gte_cfc2(t0, 21);
    gte_cfc2(t1, 22);
    gte_cfc2(t2, 23);
    p[5] = t0;
    p[6] = t1;
    p[7] = t2;
}

/* trailing alignment padding (3 nops) that fills this TU to the next subsegment */
const u32 func_8006944C[3] __attribute__((section(".text"))) = { 0, 0, 0 };

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

/*
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 *
 * Symbol:   func_80069568  (libgte matrix x matrix multiply, cc region 2.6.3)
 * Reason:   hand-written PSY-Q libgte routine (.set noreorder style); sibling of
 *           MulMatrix / func_80068CA4 / func_800696C8 / func_800690E0.
 * Evidence: `lui $1,0xffff; and rN,rN,$1` masks twice (not CSE'd) using the
 *           reserved assembler temp $at, which cc1 never allocates; register
 *           file is t0-t8 exclusively (GCC prefers v0/v1/a0-a3); reached only
 *           DIFFS=30 as register-pinned C with an irreducible >=3 floor from the
 *           $at/non-CSE constant.
 * Revisit:  only with proof it was compiler-generated C.
 */

INCLUDE_ASM("asm/nonmatchings/PAL/main", func_80069568);

/* HANDWRITTEN_ASM - PSY-Q libgte hand-asm (matrix/GTE), excluded from progress (docs/ASM_AND_GTE_POLICY.md). */


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


s16 *func_800696C8(s32 *matrix, void *vec, s16 *out) {
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
