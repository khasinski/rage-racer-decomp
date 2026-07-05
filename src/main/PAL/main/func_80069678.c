#include "common.h"


s32 *func_80069678(s32 *matrix, s32 *vec, s32 *out) {
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
