#include <sys/types.h>

#include "common.h"
#include "game/audio.h"

extern short *g_SndSpuRegs;
/* One 16-byte block per voice, so voice n's left volume is
 * g_SndVoiceRegs[n * 8] and its right volume is g_SndVoiceRegs[n * 8 + 1]. */
extern short g_SndVoiceRegs[];
extern volatile u_char g_SndVoiceRegsVolRight[];
extern volatile u_char g_SndVoiceRegsPitch[];
extern short g_SndVoiceStateNote[];
extern short g_SndVoiceStateProg[];
extern u_char g_SndVoiceStateTone[];
extern short g_SndVoiceStateVabId[];
extern u_char g_SndCurrentTone;
extern short g_SndCurrentSeqSep;
extern short g_SndCurrentVoice;

long SpuVmVSetUp(long arg0, long arg1);
long SpuVmAutoVol(long arg0, long arg1, long arg2, long arg3);
long SpuVmAutoPan(long arg0, long arg1, long arg2, long arg3);
long SpuVmApplyPitchBendToVoice(long arg0, long arg1, long arg2, long arg3, long arg4);
u_short SpuVmCalculateTonePitch(long arg0, long arg1);

long SsUtPitchBend(long arg0, long arg1, long arg2, long arg3, u_short arg4);
long SsUtChangePitch(long arg0, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6);
long SsUtChangeADSR(long arg0, long arg1, long arg2, long arg3, u_short arg4, u_short arg5);
long SsUtGetDetVVol(long arg0, short *arg1, short *arg2);
long SsUtSetDetVVol(long arg0, short arg1, short arg2);
long SsUtSetVVol(long arg0, long arg1, long arg2);
long SsUtAutoVol(long arg0, long arg1, long arg2, long arg3);
long SsUtAutoPan(long arg0, long arg1, long arg2, long arg3);

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
    register long tmp asm("v0");
    long x;
    long y;
    long flags;

    id = arg0;
    stackA = (u_short)arg5;
    stackB = (u_short)arg6;

    if (!((u_short)id >= 0x18U)) {

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
        return ret;
    }

    y = *(short *)((u_char *)g_SndVoiceStateProg + voiceOffset);
    if (y != (short)arg2) {
        ret = -1;
        return ret;
    }

    if (!(*(short *)((u_char *)g_SndVoiceStateNote + voiceOffset) != (short)arg3)) {

    SpuVmVSetUp(x, y);
    g_SndCurrentSeqSep = 0x21;
    g_SndCurrentVoice = id;
    g_SndCurrentTone = *(u_char *)(g_SndVoiceStateTone + voiceOffset);
    *(volatile short *)(g_SndVoiceRegsPitch + (index << 4)) = SpuVmCalculateTonePitch(stackA, stackB);
    flags = g_SndVoiceFlags[index];
    flags |= 4;
    /* RAW() keeps this store ahead of the return value -- see common.h. */
    RAW(g_SndVoiceFlags[index]) = flags;
    ret = 0;
    return ret;

    }
    }
    ret = -1;

    return ret;
}

long SsUtChangeADSR(long arg0, long arg1, long arg2, long arg3, u_short arg4, u_short arg5) {
    long ret;
    long tmp;
    long index;
    long voiceOffset;
    long volOffset;
    long field;

    if ((u_short)arg0 >= 0x18U) {
    } else {

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
        return ret;
    }

    field = *(short *)((u_char *)g_SndVoiceStateProg + voiceOffset);
    if (field != (short)arg2) {
        ret = -1;
        return ret;
    }

    field = *(short *)((u_char *)g_SndVoiceStateNote + voiceOffset);
    if (field != (short)arg3) {
    } else {

    volOffset = index << 3;
    g_SndVoiceRegs[volOffset + 4] = arg4;
    g_SndVoiceRegs[volOffset + 5] = arg5;
    field = g_SndVoiceFlags[index];
    field |= 0x30;
    /* RAW() keeps this store ahead of the return value -- see common.h. */
    RAW(g_SndVoiceFlags[index]) = field;
    ret = 0;
    return ret;

    }
    }
    ret = -1;

    return ret;
}

long SsUtGetDetVVol(long arg0, short *arg1, short *arg2) {
    register long offset asm("$3");
    u_char *base;

    if ((u_short)arg0 < 0x18U) {
        offset = (arg0 << 16) >> 12;
        base = (u_char *)g_SndSpuRegs;
        *arg1 = *(u_short *)(offset + (long)base);
        base = (u_char *)g_SndSpuRegs;
        offset += (long)base;
        *arg2 = *(u_short *)(offset + 2);
        return 0;
    }

    return -1;
}

long SsUtSetDetVVol(long arg0, short arg1, short arg2) {
    register long ret asm("$2");
    long index;
    long j;
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
        g_SndVoiceRegs[j + 1] = arg2;
        flags = g_SndVoiceFlags[index];
        g_SndVoiceRegs[j] = valueX;
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

long SsUtSetVVol(long arg0, long arg1, long arg2) {
    /* This pin is load-bearing after the other pins are removed. */
    register long ret asm("$2");
    long x;
    long y;
    long index;
    long volOffset;
    u_char flags;

    y = arg2;
    if ((u_short)arg0 >= 0x18U) {
        ret = -1;
    } else {
        ret = (short)arg1;
        x = (ret << 7) + ret;
        ret = (short)y;
        y = (ret << 7) + ret;
        index = (short)arg0;
        volOffset = index << 3;
        g_SndVoiceRegs[volOffset + 1] = y;
        flags = g_SndVoiceFlags[index];
        ret = 0;
        g_SndVoiceRegs[volOffset] = x;
        flags |= 3;
        g_SndVoiceFlags[index] = flags;
    }

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
