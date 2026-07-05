#include "common.h"

extern s16 D_8019CA68;

void SpuVmDamperOff(void) asm("func_800731A8");
void SpuVmDamperOn(void) asm("func_800731B8");

void SpuVmDamperOff(void) {
    D_8019CA68 = 0;
}

void SpuVmDamperOn(void) {
    D_8019CA68 = 2;
}

INCLUDE_ASM("asm/nonmatchings/PAL/main", SsSeqCalledTbyT);
