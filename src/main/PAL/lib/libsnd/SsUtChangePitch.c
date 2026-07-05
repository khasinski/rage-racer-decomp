#include "common.h"

extern volatile u8 D_8009DF24[];
extern volatile u8 D_8009E0A0[];
extern s16 D_8009E0C4[];
extern s16 D_8009E0CA[];
extern u8 D_8009E0CC[];
extern s16 D_8009E0CE[];
extern s16 D_801E4BE6;
extern s16 D_801E4BEA;
extern u8 D_801E4BDC;

s32 SpuVmVSetUp(s32 arg0, s32 arg1) asm("func_80073314");
u16 SpuVmCalculateTonePitch(s32 arg0, s32 arg1) asm("func_80074A6C");

s32 SsUtChangePitch(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) asm("func_800781C0");

s32 SsUtChangePitch(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    register s32 id asm("s2");
    register s32 index asm("s1");
    register s32 voiceOffset asm("s0");
    register s32 stackA asm("s3");
    register s32 stackB asm("s4");
    register s32 ret asm("v0");
    register s32 tmp asm("v0");
    register s32 x asm("a0");
    register s32 y asm("a1");
    register s32 flags asm("v1");

    id = arg0;
    stackA = (u16)arg5;
    stackB = (u16)arg6;

    if ((u16)id >= 0x18U) {
        goto fail_late;
    }

    tmp = arg0 << 16;
    index = tmp >> 16;
    tmp = index << 1;
    tmp += index;
    tmp <<= 2;
    tmp += index;
    voiceOffset = tmp << 2;

    x = *(s16 *)((u8 *)D_8009E0CE + voiceOffset);
    if (x != (s16)arg1) {
        ret = -1;
        goto done;
    }

    y = *(s16 *)((u8 *)D_8009E0CA + voiceOffset);
    if (y != (s16)arg2) {
        ret = -1;
        goto done;
    }

    if (*(s16 *)((u8 *)D_8009E0C4 + voiceOffset) != (s16)arg3) {
        goto fail_late;
    }

    SpuVmVSetUp(x, y);
    D_801E4BE6 = 0x21;
    D_801E4BEA = id;
    D_801E4BDC = *(u8 *)(D_8009E0CC + voiceOffset);
    *(volatile s16 *)(D_8009DF24 + (index << 4)) = SpuVmCalculateTonePitch(stackA, stackB);
    flags = D_8009E0A0[index];
    flags |= 4;
    D_8009E0A0[index] = flags;
    __asm__ volatile("" ::: "memory");
    ret = 0;
    goto done;

fail_late:
    __asm__ volatile("" ::: "memory");
    ret = -1;

done:
    return ret;
}
