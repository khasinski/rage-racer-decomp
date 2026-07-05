#include "common.h"

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
