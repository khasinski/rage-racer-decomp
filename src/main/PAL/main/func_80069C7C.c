#include "common.h"

s32 Lzc(s32 arg0) asm("func_80069C7C");

s32 Lzc(s32 arg0) {
    asm("mtc2 $4,$30");
    asm("nop");
    asm("nop");
    asm("mfc2 $2,$31");
}
