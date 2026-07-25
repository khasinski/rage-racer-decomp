#include "common.h"

/*
 * The func_80068A2C TU begins with 12 bytes (three 0x00000000 words) of
 * inter-object alignment padding that splat over-split into three bogus
 * single-`nop` "functions" (func_80068A2C/A30/A34).  The previous TU
 * (func_800689A8, [0x591A8,0x5922C)) ends cleanly at 0x5922C with
 * `jr ra; li v0,0`, so these three words are leading pad, not its tail.
 * They are emitted here the same way every other pure-padding TU in this
 * binary is (compare func_80069CBC: `u32 x[3] __attribute__((section(".text")))`).
 */
u32 func_80068A2C[3] __attribute__((section(".text"))) = { 0, 0, 0 };

/*
 * func_80068A38 is a verbatim hand-inlined PSY-Q libgte matrix routine
 * (CompMatrix-family: R2 = R0 * R1, T2 = R0 * T1 + T0) built from three/four
 * MVMVA (cop2 0x486012, sf=1, mx=Rot, v=V0, cv=none) commands with the column
 * vectors packed out of the source MATRIX by hand.  It belongs to the same
 * family as its neighbours func_80068B98/CA4/D88/E70 and, like them, only
 * reaches a byte-exact match through register-pinned COP2 transfers: GCC 2.6.3
 * will not reproduce the batched 5x lw / 5x ctc2 load block nor the exact
 * IR/MAC extraction register schedule from natural C.
 */
void *func_80068A38(s32 *m0, void *m1, void *m2) {
    register s32 r0 asm("$8") = m0[0];
    register s32 r1 asm("$9") = m0[1];
    register s32 r2 asm("$10") = m0[2];
    register s32 r3 asm("$11") = m0[3];
    register s32 r4 asm("$12") = m0[4];

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
        "swc2 $11,16($6)\n"
        "lhu $13,20($5)\n"
        "lw $14,24($5)\n"
        "lw $10,28($5)\n"
        "sll $14,$14,16\n"
        "or $13,$13,$14\n"
        "mtc2 $13,$0\n"
        "mtc2 $10,$1\n"
        "nop\n"
        "cop2 0x486012\n"
        "sll $12,$12,16\n"
        "andi $8,$8,0xFFFF\n"
        "or $8,$8,$12\n"
        "sw $8,4($6)\n"
        "andi $15,$15,0xFFFF\n"
        "sll $9,$9,16\n"
        "or $9,$9,$15\n"
        "sw $9,8($6)\n"
        "mfc2 $8,$25\n"
        "mfc2 $9,$26\n"
        "mfc2 $10,$27\n"
        "lw $11,20($4)\n"
        "lw $12,24($4)\n"
        "lw $13,28($4)\n"
        "add $8,$8,$11\n"
        "add $9,$9,$12\n"
        "add $10,$10,$13\n"
        "sw $8,20($6)\n"
        "sw $9,24($6)\n"
        "sw $10,28($6)"
        :
        : "r"(r0), "r"(r1), "r"(r2), "r"(r3), "r"(r4),
          "r"(m0), "r"(m1), "r"(m2)
        : "memory");
    asm volatile("move $2,$6");
}
