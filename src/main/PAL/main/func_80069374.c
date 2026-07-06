#include "common.h"

void func_80069374(void *out) {
    asm volatile(
        "cfc2 $8,$0\n"
        "cfc2 $9,$1\n"
        "cfc2 $10,$2\n"
        "cfc2 $11,$3\n"
        "cfc2 $12,$4\n"
        "sw $8,0($4)\n"
        "sw $9,4($4)\n"
        "sw $10,8($4)\n"
        "sw $11,12($4)\n"
        "sw $12,16($4)\n"
        "cfc2 $8,$5\n"
        "cfc2 $9,$6\n"
        "cfc2 $10,$7\n"
        "sw $8,20($4)\n"
        "sw $9,24($4)\n"
        "sw $10,28($4)"
        :
        : "r"(out)
        : "memory");
}

void func_800693BC(void *out) {
    asm volatile(
        "cfc2 $8,$8\n"
        "cfc2 $9,$9\n"
        "cfc2 $10,$10\n"
        "cfc2 $11,$11\n"
        "cfc2 $12,$12\n"
        "sw $8,0($4)\n"
        "sw $9,4($4)\n"
        "sw $10,8($4)\n"
        "sw $11,12($4)\n"
        "sw $12,16($4)\n"
        "cfc2 $8,$13\n"
        "cfc2 $9,$14\n"
        "cfc2 $10,$15\n"
        "sw $8,20($4)\n"
        "sw $9,24($4)\n"
        "sw $10,28($4)"
        :
        : "r"(out)
        : "memory");
}

void func_80069404(void *out) {
    asm volatile(
        "cfc2 $8,$16\n"
        "cfc2 $9,$17\n"
        "cfc2 $10,$18\n"
        "cfc2 $11,$19\n"
        "cfc2 $12,$20\n"
        "sw $8,0($4)\n"
        "sw $9,4($4)\n"
        "sw $10,8($4)\n"
        "sw $11,12($4)\n"
        "sw $12,16($4)\n"
        "cfc2 $8,$21\n"
        "cfc2 $9,$22\n"
        "cfc2 $10,$23\n"
        "sw $8,20($4)\n"
        "sw $9,24($4)\n"
        "sw $10,28($4)"
        :
        : "r"(out)
        : "memory");
}

asm(".globl func_8006944C\n"
    "func_8006944C:\n"
    "nop\n"
    "nop\n"
    "nop");
