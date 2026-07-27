#include <sys/types.h>

#include "common.h"

extern short *g_SndSpuRegs asm("D_8009A588");
/* Byte view of the same pointer, needed because this TU addresses the voice
 * register file both as halfwords and by byte offset. */
extern u_char *g_SndSpuRegsBytes asm("D_8009A588");
extern volatile u_char g_SndVoiceRegs[] asm("D_8009DF20");
/* Halfword view of the same voice register file: one 16-byte block per voice,
 * so voice n's left volume is g_SndVoiceRegs16[n * 8] and its right volume is
 * g_SndVoiceRegs16[n * 8 + 1]. */
extern short g_SndVoiceRegs16[] asm("D_8009DF20");
extern volatile u_char g_SndVoiceRegsVolRight[] asm("D_8009DF22");
extern volatile u_char g_SndVoiceRegsPitch[] asm("D_8009DF24");
extern volatile u_char g_SndVoiceRegsAdsr1[] asm("D_8009DF28");
extern volatile u_char g_SndVoiceRegsAdsr2[] asm("D_8009DF2A");
extern volatile u_char g_SndVoiceFlags[] asm("D_8009E0A0");
extern short g_SndVoiceStateNote[] asm("D_8009E0C4");
extern short g_SndVoiceStateProg[] asm("D_8009E0CA");
extern u_char g_SndVoiceStateTone[] asm("D_8009E0CC");
extern short g_SndVoiceStateVabId[] asm("D_8009E0CE");
extern u_char g_SndCurrentTone asm("D_801E4BDC");
extern short g_SndCurrentSeqSep asm("D_801E4BE6");
extern short g_SndCurrentVoice asm("D_801E4BEA");

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
    g_SndCurrentSeqSep = 0x21;
    ret = SpuVmApplyPitchBendToVoice((short)id, 0x21, x, y, extra);
    return -(((u_long)(ret << 16)) < 1U);
}

long SsUtChangePitch(long arg0, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6) {
    long id;
    long index;
    long voiceOffset;
    long stackA;
    long stackB;
    long ret;
    /* This pin is load-bearing: removing it changes .text. */
    register long tmp asm("v0");
    long x;
    long y;
    /* This pin is load-bearing: removing it changes .text. */
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

    x = *(short *)((u_char *)g_SndVoiceStateVabId + voiceOffset);
    if (x != (short)arg1) {
        ret = -1;
        goto done;
    }

    y = *(short *)((u_char *)g_SndVoiceStateProg + voiceOffset);
    if (y != (short)arg2) {
        ret = -1;
        goto done;
    }

    if (*(short *)((u_char *)g_SndVoiceStateNote + voiceOffset) != (short)arg3) {
        goto fail_late;
    }

    SpuVmVSetUp(x, y);
    g_SndCurrentSeqSep = 0x21;
    g_SndCurrentVoice = id;
    g_SndCurrentTone = *(u_char *)(g_SndVoiceStateTone + voiceOffset);
    *(volatile short *)(g_SndVoiceRegsPitch + (index << 4)) = SpuVmCalculateTonePitch(stackA, stackB);
    flags = g_SndVoiceFlags[index];
    flags |= 4;
    g_SndVoiceFlags[index] = flags;
    /* This barrier is load-bearing: removing it changes .text. */
    __asm__ volatile("" ::: "memory");
    ret = 0;
    goto done;

fail_late:
    ret = -1;

done:
    return ret;
}

long SsUtChangeADSR(long arg0, long arg1, long arg2, long arg3, long arg4, long arg5) {
    long ret;
    long tmp;
    long index;
    long voiceOffset;
    long volOffset;
    long left;
    long right;
    /* This pin is load-bearing: removing it changes .text. */
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

    field = *(short *)((u_char *)g_SndVoiceStateVabId + voiceOffset);
    if (field != (short)arg1) {
        ret = -1;
        goto done;
    }

    field = *(short *)((u_char *)g_SndVoiceStateProg + voiceOffset);
    if (field != (short)arg2) {
        ret = -1;
        goto done;
    }

    field = *(short *)((u_char *)g_SndVoiceStateNote + voiceOffset);
    if (field != (short)arg3) {
        goto fail_late;
    }

    volOffset = index << 4;
    *(volatile short *)(g_SndVoiceRegsAdsr1 + volOffset) = left;
    *(volatile short *)(g_SndVoiceRegsAdsr2 + volOffset) = right;
    field = g_SndVoiceFlags[index];
    field |= 0x30;
    g_SndVoiceFlags[index] = field;
    /* This barrier is load-bearing: removing it changes .text. */
    __asm__ volatile("" ::: "memory");
    ret = 0;
    goto done;

fail_late:
    ret = -1;

done:
    __asm__ volatile("addiu $sp,$sp,8" ::: "memory");
    return ret;
}

long SsUtGetDetVVol(long arg0, short *arg1, short *arg2) {
    /* This pin is load-bearing: removing it changes .text. */
    register long offset asm("$3");
    u_char *base;

    if ((u_short)arg0 < 0x18U) {
        offset = (arg0 << 16) >> 12;
        base = g_SndSpuRegsBytes;
        *arg1 = *(u_short *)(offset + (long)base);
        base = g_SndSpuRegsBytes;
        offset += (long)base;
        *arg2 = *(u_short *)(offset + 2);
        return 0;
    }

    return -1;
}

long SsUtSetDetVVol(long arg0, short arg1, short arg2) {
    /* This pin is load-bearing: removing it changes .text. */
    register long ret asm("$2");
    long index;
    long j;
    /* This pin is load-bearing: removing it changes .text. */
    short valueX;
    u_char flags;

    valueX = arg1;

    if ((u_short)arg0 >= 0x18U) {
        ret = -1;
    } else {
        index = (short)arg0;
        j = index * 8;
        /* Indexing one shared halfword base at j and j+1 - rather than two
         * separate symbols - is what produces the retail 8-byte frame; see
         * docs/names.md 21a. */
        g_SndVoiceRegs16[j + 1] = arg2;
        flags = g_SndVoiceFlags[index];
        g_SndVoiceRegs16[j] = valueX;
        flags |= 3;
        g_SndVoiceFlags[index] = flags;
        ret = 0;
    }

    return ret;
}

short SsUtGetVVol(short arg0, short *arg1, short *arg2) {
    short *ptr;
    short left;
    short right;

    if ((u_short)arg0 < 24U) {
        ptr = &g_SndSpuRegs[arg0 * 8];
        left = ptr[0];
        right = ptr[1];
        *arg1 = left / 129;
        *arg2 = right / 129;
        return 0;
    }

    return -1;
}

long SsUtSetVVol(long arg0, short arg1, short arg2) {
    /* This pin is load-bearing: removing it changes .text. */
    register long ret asm("$2");
    long yarg;
    long x;
    long y;
    long index;
    long offset;
    u_char flags;

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
    *(volatile short *)(g_SndVoiceRegsVolRight + offset) = y;
    flags = g_SndVoiceFlags[index];
    ret = 0;
    *(volatile short *)(g_SndVoiceRegs + offset) = x;
    flags |= 3;
    g_SndVoiceFlags[index] = flags;

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
