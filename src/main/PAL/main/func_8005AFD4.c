#include "common.h"

extern u8 D_801E8AFC;

s32 func_8007B014(void *arg0, s32 arg1);

s32 func_8005AFD4(void) {
    register u8 *base asm("$16");
    register s32 value0 asm("$3");
    register s32 value1 asm("$2");

    base = &D_801E8AFC;
    value1 = func_8007B014(base, 0);
    value1 = (value1 << 9) + (s32)base;
    value0 = *(s16 *)(value1 + 0x800);
    value1 = *(s16 *)(value1 + 0xC00);

    if (value0 < 0) {
        value0 = -value0;
    }
    if (value1 < 0) {
        value1 = -value1;
    }

    asm("" : "=r"(value0), "=r"(value1) : "0"(value0), "1"(value1));
    value0 <<= 16;
    value1 = (value1 << 16) >> 16;
    return value0 | value1;
}

asm(".globl func_8005B004\nfunc_8005B004 = func_8005AFD4 + 0x30");
