#include "common.h"

extern u8 *D_8009A588;

s32 SsUtGetDetVVol(s32 arg0, s16 *arg1, s16 *arg2) asm("func_800783D8");

s32 SsUtGetDetVVol(s32 arg0, s16 *arg1, s16 *arg2) {
    register s32 offset asm("$3");
    register u8 *base asm("$2");

    if ((u16)arg0 < 0x18U) {
        offset = (arg0 << 16) >> 12;
        base = D_8009A588;
        *arg1 = *(u16 *)(offset + (s32)base);
        base = D_8009A588;
        offset += (s32)base;
        *arg2 = *(u16 *)(offset + 2);
        return 0;
    }

    return -1;
}
