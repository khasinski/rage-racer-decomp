#include "common.h"


void *func_80068B98(s32 *matrix, void *src, void *dst) {
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
