#include "common.h"

s32 SpuVmVibrate(s32 arg0, s32 arg1, s32 arg2, s32 arg3) asm("func_80074D1C");

s32 SsUtVibrateOn(s32 arg0, s32 arg1, s32 arg2, s32 arg3) asm("func_800785B4");

s32 SsUtVibrateOn(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    if ((u16)arg0 < 0x18U) {
        SpuVmVibrate((s16)arg0, (s16)arg1, (s16)arg2, (s16)arg3);
        return 0;
    }

    return -1;
}
