#include "common.h"


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
