#include <sys/types.h>

#include "common.h"

extern volatile u_char g_SndVoiceStateAutoVol[] asm("D_8009E0D4");
extern volatile u_char D_8009E0D6[];
extern volatile u_char D_8009E0D8[];
extern volatile u_char D_8009E0DA[];
extern volatile u_char D_8009E0DC[];
extern volatile u_char D_8009E0DE[];

void SpuVmAutoVol(long arg0, long arg1, long arg2, long arg3) asm("func_80074D1C");
void SpuVmAutoVol(long arg0, long arg1, long arg2, long arg3) {
    register long voice asm("$11");
    register long start asm("$8");
    register long target asm("$9");
    register long step asm("$10");
    register long offset asm("$2");
    long delta;
    register long smallDenom asm("$3");
    long quotient;
    register long start16 asm("$5");
    register long target16 asm("$6");
    register long stepForSmallDiv asm("$4");
    long step16;

    voice = arg0;
    __asm__ volatile("" : "=r"(voice) : "0"(voice));
    start = arg1;
    target = arg2;
    __asm__ volatile("" : "=r"(start) : "0"(start));
    __asm__ volatile("" : "=r"(target) : "0"(target));
    step = arg3;
    start16 = (short)arg1;
    target16 = (short)arg2;

    if (start16 == target16) {
        return;
    }

    offset = (((((short)arg0 * 2) + (short)arg0) * 4) + (short)arg0) * 4;
    *(volatile short *)(g_SndVoiceStateAutoVol + offset) = 1;
    *(volatile short *)(D_8009E0DC + offset) = start;
    *(volatile short *)(D_8009E0DE + offset) = target;

    step16 = (short)arg3;
    delta = start16 - target16;
    if (delta < 0) {
        offset = target16 - start16;
        if (offset >= step16) {
            goto large;
        }
    } else if (delta >= step16) {
        do {
        } while (0);
        goto large;
    }

small:
    stepForSmallDiv = (short)step;
    smallDenom = (short)start;
    offset = (short)target;
    smallDenom -= offset;
    quotient = stepForSmallDiv / smallDenom;
    smallDenom = (short)voice;
    offset = (((smallDenom * 2) + smallDenom) * 4 + smallDenom) * 4;
    *(volatile short *)(D_8009E0D6 + offset) = 1;
    *(volatile short *)(D_8009E0D8 + offset) = quotient;
    *(volatile short *)(D_8009E0DA + offset) = quotient;
    return;

large:
    stepForSmallDiv = (short)start;
    offset = (short)target;
    stepForSmallDiv -= offset;
    offset = (short)step;
    quotient = stepForSmallDiv / offset;
    smallDenom = (short)voice;
    offset = (((smallDenom * 2) + smallDenom) * 4 + smallDenom) * 4;
    *(volatile short *)(D_8009E0D8 + offset) = 0;
    *(volatile short *)(D_8009E0D6 + offset) = quotient;
}

typedef struct SpuVoice {
    u_char pad0[0x1C];
    short autoVolume;
    short volumeStep;
    short volumeCounter;
    short volumeCounterReload;
    short currentVolume;
    short targetVolume;
    u_char pad28[0xC];
} SpuVoice;

typedef struct SvmCurrent {
    u_char field0[4];
    u_char currentVolume;
    u_char pan;
    u_char field6[4];
    u_char masterVolume;
    u_char masterPan;
    u_char fieldC;
    u_char volume;
    u_char currentPan;
} SvmCurrent;

typedef struct VabHeader {
    u_char pad0[0x18];
    u_char masterVolume;
} VabHeader;

extern u_short g_SndVoiceRegs[] asm("D_8009DF20");
extern u_char g_SndVoiceFlags[] asm("D_8009E0A0");
extern SpuVoice g_SndVoiceState[] asm("D_8009E0B8");
extern VabHeader *g_SndCurrentVabHeader asm("D_801E413C");
extern SvmCurrent g_SndCurrentAttr asm("D_801E4BD0");
extern short g_SndMonoMode asm("D_801E3FB0");

void SpuVmAutoVolTick(short voice) asm("func_80074ECC");
void SpuVmAutoVolTick(short voice) {
    short currentVolume;
    u_long leftVolumeTemp;
    u_long rightVolumeTemp;
    u_short leftVolume;
    u_short rightVolume;
    short registerOffset;
    long scaledMasterVolume;

    registerOffset = voice * 8;
    if (g_SndVoiceState[voice].volumeCounter != 0) {
        if (g_SndVoiceState[voice].volumeCounterReload-- > 0) {
            return;
        }
        g_SndVoiceState[voice].volumeCounterReload =
            g_SndVoiceState[voice].volumeCounter;
    }

    g_SndVoiceState[voice].currentVolume += g_SndVoiceState[voice].volumeStep;
    if (g_SndVoiceState[voice].volumeStep > 0) {
        if (g_SndVoiceState[voice].currentVolume >=
            g_SndVoiceState[voice].targetVolume) {
            g_SndVoiceState[voice].currentVolume =
                g_SndVoiceState[voice].targetVolume;
            g_SndVoiceState[voice].autoVolume = 0;
        }
    } else if (g_SndVoiceState[voice].volumeStep < 0) {
        if (g_SndVoiceState[voice].currentVolume <=
            g_SndVoiceState[voice].targetVolume) {
            g_SndVoiceState[voice].currentVolume =
                g_SndVoiceState[voice].targetVolume;
            g_SndVoiceState[voice].autoVolume = 0;
        }
    }

    currentVolume = g_SndVoiceState[voice].currentVolume;
    g_SndCurrentAttr.currentVolume = currentVolume;
    scaledMasterVolume = g_SndCurrentVabHeader->masterVolume * 0x3FFF;

    leftVolumeTemp =
        ((currentVolume * scaledMasterVolume) / 0x7F) / 0x7F;
    rightVolumeTemp =
        ((currentVolume * scaledMasterVolume) / 0x7F) / 0x7F;

    leftVolumeTemp =
        ((leftVolumeTemp * g_SndCurrentAttr.masterVolume *
          g_SndCurrentAttr.volume) /
         0x7F) /
        0x7F;
    rightVolumeTemp =
        ((rightVolumeTemp * g_SndCurrentAttr.masterVolume *
          g_SndCurrentAttr.volume) /
         0x7F) /
        0x7F;

    if (g_SndCurrentAttr.currentPan < 0x40) {
        leftVolume = leftVolumeTemp;
        rightVolume =
            (rightVolumeTemp * g_SndCurrentAttr.currentPan) / 0x40;
    } else {
        leftVolume =
            (leftVolumeTemp * (0x7F - g_SndCurrentAttr.currentPan)) / 0x40;
        rightVolume = rightVolumeTemp;
    }

    if (g_SndCurrentAttr.masterPan < 0x40) {
        rightVolume = (rightVolume * g_SndCurrentAttr.masterPan) / 0x40;
    } else {
        leftVolume =
            (leftVolume * (0x7F - g_SndCurrentAttr.masterPan)) / 0x40;
    }

    if (g_SndCurrentAttr.pan < 0x40) {
        rightVolume = (rightVolume * g_SndCurrentAttr.pan) / 0x40;
    } else {
        leftVolume = (leftVolume * (0x7F - g_SndCurrentAttr.pan)) / 0x40;
    }

    if (g_SndMonoMode == 1) {
        if (rightVolume > leftVolume) {
            leftVolume = rightVolume;
        } else {
            rightVolume = leftVolume;
        }
    }

    g_SndVoiceRegs[registerOffset] = leftVolume;
    g_SndVoiceRegs[registerOffset + 1] = rightVolume;
    g_SndVoiceFlags[voice] |= 3;
}

/*
 * SpuVmAutoPan declared these volatile, SpuVmAutoPanTick plain; every access in
 * both goes through an explicit (volatile short *) cast, so the plain form keeps
 * the accesses volatile and both members byte-exact.
 */
extern u_char g_SndVoiceStateAutoPan[] asm("D_8009E0E0");
extern u_char D_8009E0E2[];
extern u_char D_8009E0E4[];
extern u_char D_8009E0E6[];
extern u_char D_8009E0E8[];
extern u_char D_8009E0EA[];

void SpuVmAutoPan(long arg0, long arg1, long arg2, long arg3) asm("func_8007521C");
void SpuVmAutoPan(long arg0, long arg1, long arg2, long arg3) {
    register long voice asm("$11");
    register long start asm("$8");
    register long target asm("$9");
    register long step asm("$10");
    register long offset asm("$2");
    long delta;
    register long smallDenom asm("$3");
    long quotient;
    register long start16 asm("$5");
    register long target16 asm("$6");
    register long stepForSmallDiv asm("$4");
    long step16;

    voice = arg0;
    __asm__ volatile("" : "=r"(voice) : "0"(voice));
    start = arg1;
    target = arg2;
    __asm__ volatile("" : "=r"(start) : "0"(start));
    __asm__ volatile("" : "=r"(target) : "0"(target));
    step = arg3;
    start16 = (short)arg1;
    target16 = (short)arg2;

    if (start16 == target16) {
        return;
    }

    offset = (((((short)arg0 * 2) + (short)arg0) * 4) + (short)arg0) * 4;
    *(volatile short *)(g_SndVoiceStateAutoPan + offset) = 1;
    *(volatile short *)(D_8009E0E8 + offset) = start;
    *(volatile short *)(D_8009E0EA + offset) = target;

    step16 = (short)arg3;
    delta = start16 - target16;
    if (delta < 0) {
        offset = target16 - start16;
        if (offset >= step16) {
            goto large;
        }
    } else if (delta >= step16) {
        do {
        } while (0);
        goto large;
    }

small:
    stepForSmallDiv = (short)step;
    smallDenom = (short)start;
    offset = (short)target;
    smallDenom -= offset;
    quotient = stepForSmallDiv / smallDenom;
    smallDenom = (short)voice;
    offset = (((smallDenom * 2) + smallDenom) * 4 + smallDenom) * 4;
    *(volatile short *)(D_8009E0E2 + offset) = 1;
    *(volatile short *)(D_8009E0E4 + offset) = quotient;
    *(volatile short *)(D_8009E0E6 + offset) = quotient;
    return;

large:
    stepForSmallDiv = (short)start;
    offset = (short)target;
    stepForSmallDiv -= offset;
    offset = (short)step;
    quotient = stepForSmallDiv / offset;
    smallDenom = (short)voice;
    offset = (((smallDenom * 2) + smallDenom) * 4 + smallDenom) * 4;
    *(volatile short *)(D_8009E0E4 + offset) = 0;
    *(volatile short *)(D_8009E0E2 + offset) = quotient;
}

extern u_char g_SndVoiceFlags[] asm("D_8009E0A0");
extern u_short g_SndVoiceRegs[] asm("D_8009DF20");
extern u_short g_SndVoiceRegsVolRight[] asm("D_8009DF22");
extern u_char D_801E4BD4;
extern u_char D_801E4BD5;
extern u_char D_801E4BDA;
extern u_char D_801E4BDB;
extern u_char D_801E4BDD;
extern u_char D_801E4BDE;
extern short g_SndMonoMode asm("D_801E3FB0");

void SpuVmAutoPanTick(long arg0) asm("func_800753CC");
void SpuVmAutoPanTick(long arg0) {
    long stack[6];
    register long originalArg asm("$8");
    long offset;
    register long channel asm("$3");
    register long index8 asm("$9");
    u_short counter;
    long sum;
    long step;
    register long limit asm("$3");
    register long current asm("$2");
    register long positiveCompare asm("$2");
    register long negativeCompare asm("$3");
    register long clampValue asm("$4");
    u_long envelope;

    asm(".globl func_80075420\nfunc_80075420 = SpuVmAutoPanTick + 0x54");
    channel = (short)arg0;
    index8 = channel << 3;
    asm("" : "=r"(index8) : "0"(index8));
    offset = channel * 52;
    asm("" : "=r"(offset) : "0"(offset));
    originalArg = arg0;
    if (*(short *)&D_8009E0E4[offset] != 0) {
        counter = *(u_short *)&D_8009E0E6[offset];
        *(u_short *)&D_8009E0E6[offset] = counter - 1;
        if ((short)counter > 0) {
            return;
        }
        *(u_short *)&D_8009E0E6[offset] = *(u_short *)&D_8009E0E4[offset];
    }

    step = *(short *)&D_8009E0E2[offset];
    sum = *(u_short *)&D_8009E0E8[offset] + *(u_short *)&D_8009E0E2[offset];
    *(u_short *)&D_8009E0E8[offset] = sum;
    if (step <= 0) {
        goto checkNegativeStep;
    }
    current = (u_long)sum << 16;
    limit = *(short *)&D_8009E0EA[offset];
    current >>= 16;
    positiveCompare = current < limit;
    asm("" : "=r"(positiveCompare) : "0"(positiveCompare));
    clampValue = limit;
    if (positiveCompare) {
        goto envelopeDone;
    }
    goto clampEnvelope;

checkNegativeStep:
    if (step >= 0) {
        goto envelopeDone;
    }
    current = (u_long)sum << 16;
    limit = *(short *)&D_8009E0EA[offset];
    current >>= 16;
    clampValue = limit;
    negativeCompare = limit < current;
    asm("" : "=r"(negativeCompare) : "0"(negativeCompare));
    if (negativeCompare) {
        goto envelopeDone;
    }

clampEnvelope:
    *(u_short *)&D_8009E0E8[offset] = clampValue;
    *(u_short *)&g_SndVoiceStateAutoPan[offset] = 0;

envelopeDone:
    envelope = D_8009E0E8[(short)originalArg * 52];
    {
    register u_char *base asm("$2");
    register long level asm("$3");
    register long scaledLevel asm("$2");
    register long masterVolume asm("$4");
    register long dividend asm("$3");
    register u_long volume asm("$3");
    u_long pan;
    u_long left;
    u_long right;
    long mixed;
    register long outputOffset asm("$4");
    register u_long compareLeft asm("$2");
    register u_long compareRight asm("$3");
    register long flagIndex asm("$3");

    asm("" : : : "memory");
    base = (u_char *)g_SndCurrentVabHeader;
    asm("" : : : "memory");
    D_801E4BD5 = envelope;
    asm("" : : : "memory");
    level = base[0x18];
    masterVolume = D_801E4BD4;
    scaledLevel = level * 16383;
    asm("" : "=r"(scaledLevel) : "0"(scaledLevel));
    dividend = masterVolume * scaledLevel;
    volume = dividend / 16129;
    volume = volume * D_801E4BDA;
    volume = volume * D_801E4BDD;
    volume = (u_long)volume / 16129U;

    pan = D_801E4BDE;
    left = volume;
    if (pan < 0x40) {
        asm("" : "=r"(volume) : "0"(volume));
        left = volume;
        right = ((u_long)(volume * pan)) >> 6;
    } else {
        asm("" : "=r"(left) : "0"(left));
        right = left;
        left = ((u_long)(left * (0x7F - pan))) >> 6;
    }

    pan = D_801E4BDB;
    if (pan < 0x40) {
        mixed = (u_short)right * pan;
        if (mixed < 0) {
            mixed += 63;
        }
        right = (u_long)mixed >> 6;
    } else {
        mixed = (u_short)left * (0x7F - pan);
        if (mixed < 0) {
            mixed += 63;
        }
        left = (u_long)mixed >> 6;
    }
    pan = envelope;
    if (pan < 0x40) {
        mixed = (u_short)right * pan;
        if (mixed < 0) {
            mixed += 63;
        }
        right = (u_long)mixed >> 6;
    } else {
        mixed = (u_short)left * (0x7F - pan);
        if (mixed < 0) {
            mixed += 63;
        }
        left = (u_long)mixed >> 6;
    }

    if (g_SndMonoMode == 1) {
        compareRight = (u_short)right;
        compareLeft = (u_short)left;
        if (compareLeft < compareRight) {
            left = right;
        } else {
            right = left;
        }
    }

    outputOffset = (short)index8 << 1;
    flagIndex = (short)originalArg;
    *(u_short *)((u_char *)g_SndVoiceRegsVolRight + outputOffset) = right;
    asm("" : : : "memory");
    *(u_short *)((u_char *)g_SndVoiceRegs + outputOffset) = left;
    g_SndVoiceFlags[flagIndex] |= 3;
    }
}
