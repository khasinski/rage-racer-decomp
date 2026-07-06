#include "common.h"
#include "psyq/kernel.h"


void SpuInit(void) asm("func_80078838");
void _SsInitTables(void) asm("func_80071B0C");

void ssinit(void) asm("func_80071BF4");
void ssinit(void) {
    KernelCallbackSlot3();
    SpuInit();
    _SsInitTables();
}

asm(".globl ssinit\nssinit = func_80071BF4");
