#include "common.h"

void func_80069374(void *arg0) {
    asm("cfc2 $8,$0");
    asm("cfc2 $9,$1");
    asm("cfc2 $10,$2");
    asm("cfc2 $11,$3");
    asm("cfc2 $12,$4");
    asm("sw $8,0($4)");
    asm("sw $9,4($4)");
    asm("sw $10,8($4)");
    asm("sw $11,12($4)");
    asm("sw $12,16($4)");
    asm("cfc2 $8,$5");
    asm("cfc2 $9,$6");
    asm("cfc2 $10,$7");
    asm("sw $8,20($4)");
    asm("sw $9,24($4)");
    asm("sw $10,28($4)");
}

void func_800693BC(void *arg0) {
    asm("cfc2 $8,$8");
    asm("cfc2 $9,$9");
    asm("cfc2 $10,$10");
    asm("cfc2 $11,$11");
    asm("cfc2 $12,$12");
    asm("sw $8,0($4)");
    asm("sw $9,4($4)");
    asm("sw $10,8($4)");
    asm("sw $11,12($4)");
    asm("sw $12,16($4)");
    asm("cfc2 $8,$13");
    asm("cfc2 $9,$14");
    asm("cfc2 $10,$15");
    asm("sw $8,20($4)");
    asm("sw $9,24($4)");
    asm("sw $10,28($4)");
}

void func_80069404(void *arg0) {
    asm("cfc2 $8,$16");
    asm("cfc2 $9,$17");
    asm("cfc2 $10,$18");
    asm("cfc2 $11,$19");
    asm("cfc2 $12,$20");
    asm("sw $8,0($4)");
    asm("sw $9,4($4)");
    asm("sw $10,8($4)");
    asm("sw $11,12($4)");
    asm("sw $12,16($4)");
    asm("cfc2 $8,$21");
    asm("cfc2 $9,$22");
    asm("cfc2 $10,$23");
    asm("sw $8,20($4)");
    asm("sw $9,24($4)");
    asm("sw $10,28($4)");
}

u32 func_8006944C[3] __attribute__((section(".text"))) = {
    0,
    0,
    0,
};
