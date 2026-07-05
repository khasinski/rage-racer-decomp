#include "common.h"

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
