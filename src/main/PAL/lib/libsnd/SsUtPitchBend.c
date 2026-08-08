#include <sys/types.h>

#include "common.h"
#include "game/audio.h"

extern short *g_SndSpuRegs;
/* One 16-byte block per voice, so voice n's left volume is
 * g_SndVoiceRegs[n * 8] and its right volume is g_SndVoiceRegs[n * 8 + 1]. */
extern short g_SndVoiceRegs[];
extern volatile u_char g_SndVoiceRegsPitch[];
extern short g_SndVoiceStateNote[];
extern short g_SndVoiceStateProg[];
extern u_char g_SndVoiceStateTone[];
extern short g_SndVoiceStateVabId[];
extern u_char g_SndCurrentTone;
extern short g_SndCurrentSeqSep;
extern short g_SndCurrentVoice;

long SpuVmVSetUp(long vab_id, long program);
long SpuVmAutoVol(long voiceArg, long startArg, long targetArg, long stepArg);
long SpuVmAutoPan(long voiceArg, long startArg, long targetArg, long stepArg);
long SpuVmApplyPitchBendToVoice(long voice, long note, long vab_id, long program, long bend);
u_short SpuVmCalculateTonePitch(long center, long fine);

long SsUtPitchBend(long voice, long vab_id, long program, long note, u_short pbend);
long SsUtChangePitch(long voice, long vab_id, long program, long old_note, long old_fine, long new_note, long new_fine);
long SsUtChangeADSR(long voice, long vab_id, long program, long tone, u_short adsr1, u_short adsr2);
short SsUtGetDetVVol(short voice, short* left, short* right);
short SsUtSetDetVVol(short voice, short left, short right);
short SsUtSetVVol(short voice, short left, short right);
long SsUtAutoVol(long voice, long start_vol, long end_vol, long delta_time);
long SsUtAutoPan(long voice, long start_pan, long end_pan, long delta_time);

long SsUtPitchBend(long voice, long vab_id, long program, long note, u_short pbend) {
    long id;
    u_short extra;
    long x;
    long y;
    long ret;

    id = voice;
    extra = pbend;
    x = (short)vab_id;
    y = (short)program;

    SpuVmVSetUp(x, y);
    g_SndCurrentSeqSep = 0x21;
    ret = SpuVmApplyPitchBendToVoice((short)id, 0x21, x, y, extra);
    return -(((u_long)(ret << 16)) < 1U);
}

long SsUtChangePitch(long voice, long vab_id, long program, long old_note, long old_fine, long new_note, long new_fine) {
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

    id = voice;
    stackA = (u_short)new_note;
    stackB = (u_short)new_fine;

    if (!((u_short)id >= 0x18U)) {

    tmp = voice << 16;
    index = tmp >> 16;
    tmp = index << 1;
    tmp += index;
    tmp <<= 2;
    tmp += index;
    voiceOffset = tmp << 2;

    x = *(short *)((u_char *)g_SndVoiceStateVabId + voiceOffset);
    if (x != (short)vab_id) {
        ret = -1;
        return ret;
    }

    y = *(short *)((u_char *)g_SndVoiceStateProg + voiceOffset);
    if (y != (short)program) {
        ret = -1;
        return ret;
    }

    if (!(*(short *)((u_char *)g_SndVoiceStateNote + voiceOffset) != (short)old_note)) {

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

long SsUtChangeADSR(long voice, long vab_id, long program, long tone, u_short adsr1, u_short adsr2) {
    long ret;
    long tmp;
    long index;
    long voiceOffset;
    long volOffset;
    long field;

    if ((u_short)voice >= 0x18U) {
    } else {

    tmp = voice << 16;
    index = tmp >> 16;
    tmp = index << 1;
    tmp += index;
    tmp <<= 2;
    tmp += index;
    voiceOffset = tmp << 2;

    field = *(short *)((u_char *)g_SndVoiceStateVabId + voiceOffset);
    if (field != (short)vab_id) {
        ret = -1;
        return ret;
    }

    field = *(short *)((u_char *)g_SndVoiceStateProg + voiceOffset);
    if (field != (short)program) {
        ret = -1;
        return ret;
    }

    field = *(short *)((u_char *)g_SndVoiceStateNote + voiceOffset);
    if (field != (short)tone) {
    } else {

    volOffset = index << 3;
    g_SndVoiceRegs[volOffset + 4] = adsr1;
    g_SndVoiceRegs[volOffset + 5] = adsr2;
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

short SsUtGetDetVVol(short voice, short *left, short *right) {
    if (voice >= 0 && voice < 24) {
        *left = g_SndSpuRegs[voice * 8];
        *right = g_SndSpuRegs[voice * 8 + 1];
        return 0;
    }

    return -1;
}

short SsUtSetDetVVol(short voice, short left, short right) {
    if (voice >= 0 && voice < 24) {
        g_SndVoiceRegs[voice * 8 + 1] = right;
        g_SndVoiceRegs[voice * 8] = left;
        g_SndVoiceFlags[voice] |= 3;
        return 0;
    }
    return -1;
}

short SsUtGetVVol(short voice, short *volLeft, short *volRight) {
    short *ptr;
    short left;
    short right;

    if ((u_short)voice < 24U) {
        ptr = &g_SndSpuRegs[voice * 8];
        left = ptr[0];
        right = ptr[1];
        *volLeft = left / 129;
        *volRight = right / 129;
        return 0;
    }

    return -1;
}

short SsUtSetVVol(short voice, short left, short right) {
    u_short scaledLeft;
    u_short scaledRight;

    if (voice >= 0 && voice < 24) {
        scaledLeft = left * 0x81;
        scaledRight = right * 0x81;
        g_SndVoiceRegs[voice * 8 + 1] = scaledRight;
        g_SndVoiceRegs[voice * 8] = scaledLeft;
        g_SndVoiceFlags[voice] |= 3;
        return 0;
    }
    return -1;
}

long SsUtAutoVol(long voice, long start_vol, long end_vol, long delta_time) {
    if ((u_short)voice < 0x18U) {
        SpuVmAutoVol((short)voice, (short)start_vol, (short)end_vol, (short)delta_time);
        return 0;
    }

    return -1;
}

long SsUtAutoPan(long voice, long start_pan, long end_pan, long delta_time) {
    if ((u_short)voice < 0x18U) {
        SpuVmAutoPan((short)voice, (short)start_pan, (short)end_pan, (short)delta_time);
        return 0;
    }

    return -1;
}
