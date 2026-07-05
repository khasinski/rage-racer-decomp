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
