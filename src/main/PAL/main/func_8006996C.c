#include "common.h"

void SetIR123(s32 arg0, s32 arg1, s32 arg2) asm("func_8006996C");

void SetIR123(s32 arg0, s32 arg1, s32 arg2) {
    asm("mtc2 $4,$9");
    asm("mtc2 $5,$10");
    asm("mtc2 $6,$11");
}
