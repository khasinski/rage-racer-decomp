#include <sys/types.h>

#include "common.h"

extern short *D_8009A588;
extern u_char *D_8009A588_bytes asm("D_8009A588");
extern volatile u_char D_8009DF20[];
extern volatile u_char D_8009DF22[];
extern volatile u_char D_8009DF24[];
extern volatile u_char D_8009DF28[];
extern volatile u_char D_8009DF2A[];
extern volatile u_char D_8009E0A0[];
extern short D_8009E0C4[];
extern short D_8009E0CA[];
extern u_char D_8009E0CC[];
extern short D_8009E0CE[];
extern u_char D_801E4BDC;
extern short D_801E4BE6;
extern short D_801E4BEA;

long SpuVmVSetUp(long arg0, long arg1) asm("func_80073314");
long SpuVmAutoVol(long arg0, long arg1, long arg2, long arg3) asm("func_80074D1C");
long SpuVmAutoPan(long arg0, long arg1, long arg2, long arg3) asm("func_8007521C");
long SpuVmApplyPitchBendToVoice(long arg0, long arg1, long arg2, long arg3, long arg4) asm("func_80075CB0");
u_short SpuVmCalculateTonePitch(long arg0, long arg1) asm("func_80074A6C");

long SsUtPitchBend(long arg0, long arg1, long arg2, long arg3, u_short arg4) asm("func_80078130");
long SsUtChangePitch(long arg0, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6) asm("func_800781C0");
long SsUtChangeADSR(long arg0, long arg1, long arg2, long arg3, long arg4, long arg5) asm("func_80078300");
long SsUtGetDetVVol(long arg0, short *arg1, short *arg2) asm("func_800783D8");
long SsUtSetDetVVol(long arg0, short arg1, short arg2) asm("func_80078430");
long SsUtSetVVol(long arg0, short arg1, short arg2) asm("func_80078528");
long SsUtAutoVol(long arg0, long arg1, long arg2, long arg3) asm("func_800785B4");
long SsUtAutoPan(long arg0, long arg1, long arg2, long arg3) asm("func_80078608");

long SsUtPitchBend(long arg0, long arg1, long arg2, long arg3, u_short arg4) {
    long id;
    u_short extra;
    long x;
    long y;
    long ret;

    id = arg0;
    extra = arg4;
    x = (short)arg1;
    y = (short)arg2;

    SpuVmVSetUp(x, y);
    D_801E4BE6 = 0x21;
    ret = SpuVmApplyPitchBendToVoice((short)id, 0x21, x, y, extra);
    return -(((u_long)(ret << 16)) < 1U);
}

long SsUtChangePitch(long arg0, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6) {
    register long id asm("s2");
    register long index asm("s1");
    register long voiceOffset asm("s0");
    register long stackA asm("s3");
    register long stackB asm("s4");
    register long ret asm("v0");
    register long tmp asm("v0");
    register long x asm("a0");
    register long y asm("a1");
    register long flags asm("v1");

    id = arg0;
    stackA = (u_short)arg5;
    stackB = (u_short)arg6;

    if ((u_short)id >= 0x18U) {
        goto fail_late;
    }

    tmp = arg0 << 16;
    index = tmp >> 16;
    tmp = index << 1;
    tmp += index;
    tmp <<= 2;
    tmp += index;
    voiceOffset = tmp << 2;

    x = *(short *)((u_char *)D_8009E0CE + voiceOffset);
    if (x != (short)arg1) {
        ret = -1;
        goto done;
    }

    y = *(short *)((u_char *)D_8009E0CA + voiceOffset);
    if (y != (short)arg2) {
        ret = -1;
        goto done;
    }

    if (*(short *)((u_char *)D_8009E0C4 + voiceOffset) != (short)arg3) {
        goto fail_late;
    }

    SpuVmVSetUp(x, y);
    D_801E4BE6 = 0x21;
    D_801E4BEA = id;
    D_801E4BDC = *(u_char *)(D_8009E0CC + voiceOffset);
    *(volatile short *)(D_8009DF24 + (index << 4)) = SpuVmCalculateTonePitch(stackA, stackB);
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

long SsUtChangeADSR(long arg0, long arg1, long arg2, long arg3, long arg4, long arg5) {
    register long ret asm("v0");
    register long tmp asm("v0");
    register long index asm("t0");
    register long voiceOffset asm("a0");
    register long volOffset asm("v0");
    register long left asm("t1");
    register long right asm("t2");
    register long field asm("v1");

    __asm__ volatile("addiu $sp,$sp,-8" ::: "memory");

    tmp = (u_short)arg0 < 0x18U;
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

    field = *(short *)((u_char *)D_8009E0CE + voiceOffset);
    if (field != (short)arg1) {
        ret = -1;
        goto done;
    }

    field = *(short *)((u_char *)D_8009E0CA + voiceOffset);
    if (field != (short)arg2) {
        ret = -1;
        goto done;
    }

    field = *(short *)((u_char *)D_8009E0C4 + voiceOffset);
    if (field != (short)arg3) {
        goto fail_late;
    }

    volOffset = index << 4;
    *(volatile short *)(D_8009DF28 + volOffset) = left;
    *(volatile short *)(D_8009DF2A + volOffset) = right;
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

long SsUtGetDetVVol(long arg0, short *arg1, short *arg2) {
    register long offset asm("$3");
    register u_char *base asm("$2");

    if ((u_short)arg0 < 0x18U) {
        offset = (arg0 << 16) >> 12;
        base = D_8009A588_bytes;
        *arg1 = *(u_short *)(offset + (long)base);
        base = D_8009A588_bytes;
        offset += (long)base;
        *arg2 = *(u_short *)(offset + 2);
        return 0;
    }

    return -1;
}

long SsUtSetDetVVol(long arg0, short arg1, short arg2) {
    register long ret asm("$2");
    register long index asm("$3");
    register long offset asm("$5");
    register short valueX asm("$7");
    register u_char flags asm("$4");

    __asm__ volatile("addiu $sp,$sp,-8" ::: "memory");
    valueX = arg1;

    if ((u_short)arg0 >= 0x18U) {
        ret = -1;
        goto done;
    }

    index = (short)arg0;
    offset = index << 4;
    *(volatile short *)(D_8009DF22 + offset) = arg2;
    flags = D_8009E0A0[index];
    ret = 0;
    *(volatile short *)(D_8009DF20 + offset) = valueX;
    flags |= 3;
    D_8009E0A0[index] = flags;

done:
    __asm__ volatile("addiu $sp,$sp,8" ::: "memory");
    return ret;
}

short SsUtGetVVol(short arg0, short *arg1, short *arg2) {
    short *ptr;
    short left;
    short right;

    if ((u_short)arg0 < 24U) {
        ptr = &D_8009A588[arg0 * 8];
        left = ptr[0];
        right = ptr[1];
        *arg1 = left / 129;
        *arg2 = right / 129;
        return 0;
    }

    return -1;
}

long SsUtSetVVol(long arg0, short arg1, short arg2) {
    register long ret asm("$2");
    register long yarg asm("$3");
    register long x asm("$6");
    register long y asm("$3");
    register long index asm("$4");
    register long offset asm("$5");
    register u_char flags asm("$3");

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
    index = (short)arg0;
    offset = index << 4;
    *(volatile short *)(D_8009DF22 + offset) = y;
    flags = D_8009E0A0[index];
    ret = 0;
    *(volatile short *)(D_8009DF20 + offset) = x;
    flags |= 3;
    D_8009E0A0[index] = flags;

    __asm__ volatile("2:");
    __asm__ volatile("addiu $sp,$sp,8" ::: "memory");
    return ret;
}

long SsUtAutoVol(long arg0, long arg1, long arg2, long arg3) {
    if ((u_short)arg0 < 0x18U) {
        SpuVmAutoVol((short)arg0, (short)arg1, (short)arg2, (short)arg3);
        return 0;
    }

    return -1;
}

long SsUtAutoPan(long arg0, long arg1, long arg2, long arg3) {
    if ((u_short)arg0 < 0x18U) {
        SpuVmAutoPan((short)arg0, (short)arg1, (short)arg2, (short)arg3);
        return 0;
    }

    return -1;
}
