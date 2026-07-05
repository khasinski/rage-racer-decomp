#include "common.h"

extern s16 *D_8009A588;
extern u8 *D_8009A588_bytes asm("D_8009A588");
extern volatile u8 D_8009DF20[];
extern volatile u8 D_8009DF22[];
extern volatile u8 D_8009DF24[];
extern volatile u8 D_8009DF28[];
extern volatile u8 D_8009DF2A[];
extern volatile u8 D_8009E0A0[];
extern s16 D_8009E0C4[];
extern s16 D_8009E0CA[];
extern u8 D_8009E0CC[];
extern s16 D_8009E0CE[];
extern u8 D_801E4BDC;
extern s16 D_801E4BE6;
extern s16 D_801E4BEA;

s32 SpuVmVSetUp(s32 arg0, s32 arg1) asm("func_80073314");
s32 SpuVmVibrate(s32 arg0, s32 arg1, s32 arg2, s32 arg3) asm("func_80074D1C");
s32 SpuVmAutoVolPan(s32 arg0, s32 arg1, s32 arg2, s32 arg3) asm("func_8007521C");
s32 SpuVmApplyPitchBendToVoice(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) asm("func_80075CB0");
u16 SpuVmCalculateTonePitch(s32 arg0, s32 arg1) asm("func_80074A6C");

s32 SsUtPitchBend(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u16 arg4) asm("func_80078130");
s32 SsUtChangePitch(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) asm("func_800781C0");
s32 SsUtChangeADSR(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) asm("func_80078300");
s32 SsUtGetDetVVol(s32 arg0, s16 *arg1, s16 *arg2) asm("func_800783D8");
s32 SsUtSetDetVVol(s32 arg0, s16 arg1, s16 arg2) asm("func_80078430");
s32 SsUtSetVVol(s32 arg0, s16 arg1, s16 arg2) asm("func_80078528");
s32 SsUtVibrateOn(s32 arg0, s32 arg1, s32 arg2, s32 arg3) asm("func_800785B4");
s32 SsUtAutoVol(s32 arg0, s32 arg1, s32 arg2, s32 arg3) asm("func_80078608");

s32 SsUtPitchBend(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u16 arg4) {
    s32 id;
    u16 extra;
    s32 x;
    s32 y;
    s32 ret;

    id = arg0;
    extra = arg4;
    x = (s16)arg1;
    y = (s16)arg2;

    SpuVmVSetUp(x, y);
    D_801E4BE6 = 0x21;
    ret = SpuVmApplyPitchBendToVoice((s16)id, 0x21, x, y, extra);
    return -(((u32)(ret << 16)) < 1U);
}

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

s32 SsUtGetDetVVol(s32 arg0, s16 *arg1, s16 *arg2) {
    register s32 offset asm("$3");
    register u8 *base asm("$2");

    if ((u16)arg0 < 0x18U) {
        offset = (arg0 << 16) >> 12;
        base = D_8009A588_bytes;
        *arg1 = *(u16 *)(offset + (s32)base);
        base = D_8009A588_bytes;
        offset += (s32)base;
        *arg2 = *(u16 *)(offset + 2);
        return 0;
    }

    return -1;
}

s32 SsUtSetDetVVol(s32 arg0, s16 arg1, s16 arg2) {
    register s32 ret asm("$2");
    register s32 index asm("$3");
    register s32 offset asm("$5");
    register s16 valueX asm("$7");
    register u8 flags asm("$4");

    __asm__ volatile("addiu $sp,$sp,-8" ::: "memory");
    valueX = arg1;

    if ((u16)arg0 >= 0x18U) {
        ret = -1;
        goto done;
    }

    index = (s16)arg0;
    offset = index << 4;
    *(volatile s16 *)(D_8009DF22 + offset) = arg2;
    flags = D_8009E0A0[index];
    ret = 0;
    *(volatile s16 *)(D_8009DF20 + offset) = valueX;
    flags |= 3;
    D_8009E0A0[index] = flags;

done:
    __asm__ volatile("addiu $sp,$sp,8" ::: "memory");
    return ret;
}

s16 SsUtGetVVol(s16 arg0, s16 *arg1, s16 *arg2) {
    s16 *ptr;
    s16 left;
    s16 right;

    if ((u16)arg0 < 24U) {
        ptr = &D_8009A588[arg0 * 8];
        left = ptr[0];
        right = ptr[1];
        *arg1 = left / 129;
        *arg2 = right / 129;
        return 0;
    }

    return -1;
}

s32 SsUtSetVVol(s32 arg0, s16 arg1, s16 arg2) {
    register s32 ret asm("$2");
    register s32 yarg asm("$3");
    register s32 x asm("$6");
    register s32 y asm("$3");
    register s32 index asm("$4");
    register s32 offset asm("$5");
    register u8 flags asm("$3");

    __asm__ volatile(
        "\t.set\tnoreorder\n"
        "addiu $sp,$sp,-8 # maspsx-keep\n"
        "andi  $2,%2,0xffff # maspsx-keep\n"
        "sltiu $2,$2,0x18 # maspsx-keep\n"
        "bnez  $2,1f # maspsx-keep\n"
        "addu  %0,%3,$zero # maspsx-keep\n"
        "j     2f # maspsx-keep\n"
        "addiu $2,$zero,-1 # maspsx-keep\n"
        "1: # maspsx-keep\n"
        "sll   $2,%4,16 # maspsx-keep\n"
        "sra   $2,$2,16 # maspsx-keep\n"
        "sll   %1,$2,7 # maspsx-keep\n"
        "addu  %1,%1,$2 # maspsx-keep\n"
        "sll   $2,%0,16 # maspsx-keep\n"
        "sra   $2,$2,16 # maspsx-keep\n"
        "sll   %0,$2,7 # maspsx-keep\n"
        "addu  %0,%0,$2 # maspsx-keep\n"
        "\t.set\treorder\n"
        : "=r"(yarg), "=r"(x)
        : "r"(arg0), "r"(arg2), "r"(arg1)
        : "$2", "memory");

    y = yarg;
    index = (s16)arg0;
    offset = index << 4;
    *(volatile s16 *)(D_8009DF22 + offset) = y;
    flags = D_8009E0A0[index];
    ret = 0;
    *(volatile s16 *)(D_8009DF20 + offset) = x;
    flags |= 3;
    D_8009E0A0[index] = flags;

    __asm__ volatile("2:");
    __asm__ volatile("addiu $sp,$sp,8" ::: "memory");
    return ret;
}

s32 SsUtVibrateOn(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    if ((u16)arg0 < 0x18U) {
        SpuVmVibrate((s16)arg0, (s16)arg1, (s16)arg2, (s16)arg3);
        return 0;
    }

    return -1;
}

s32 SsUtAutoVol(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    if ((u16)arg0 < 0x18U) {
        SpuVmAutoVolPan((s16)arg0, (s16)arg1, (s16)arg2, (s16)arg3);
        return 0;
    }

    return -1;
}
