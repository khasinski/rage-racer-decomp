#include "common.h"

extern volatile u8 D_8009DF28[];
extern volatile u8 D_8009DF2A[];
extern volatile u8 D_8009E0A0[];
extern s16 D_8009E0C4[];
extern s16 D_8009E0CA[];
extern s16 D_8009E0CE[];

s32 SsUtChangeADSR(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) asm("func_80078300");

s32 SsUtChangeADSR(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    register s32 ret asm("v0");
    register s32 tmp asm("v0");
    register s32 index asm("t0");
    register s32 voiceOffset asm("a0");
    register s32 volOffset asm("v0");
    register s32 left asm("t1");
    register s32 right asm("t2");
    register s32 field asm("v1");

    __asm__ volatile("addiu $sp,$sp,-8" ::: "memory");

    tmp = (u16)arg0 < 0x18U;
    __asm__ volatile(
        "lhu %0,0x18($sp)\n\t"
        "lhu %1,0x1c($sp)"
        : "=r"(left), "=r"(right)
        :
        : "memory");

    if (!tmp) {
        goto fail_late;
    }

    tmp = arg0 << 16;
    index = tmp >> 16;
    tmp = index << 1;
    tmp += index;
    tmp <<= 2;
    tmp += index;
    voiceOffset = tmp << 2;

    field = *(s16 *)((u8 *)D_8009E0CE + voiceOffset);
    if (field != (s16)arg1) {
        ret = -1;
        goto done;
    }

    field = *(s16 *)((u8 *)D_8009E0CA + voiceOffset);
    if (field != (s16)arg2) {
        ret = -1;
        goto done;
    }

    field = *(s16 *)((u8 *)D_8009E0C4 + voiceOffset);
    if (field != (s16)arg3) {
        goto fail_late;
    }

    volOffset = index << 4;
    *(volatile s16 *)(D_8009DF28 + volOffset) = left;
    *(volatile s16 *)(D_8009DF2A + volOffset) = right;
    field = D_8009E0A0[index];
    field |= 0x30;
    D_8009E0A0[index] = field;
    __asm__ volatile("" ::: "memory");
    ret = 0;
    goto done;

fail_late:
    __asm__ volatile("" ::: "memory");
    ret = -1;

done:
    __asm__ volatile("addiu $sp,$sp,8" ::: "memory");
    return ret;
}
