#include "common.h"

extern volatile u8 D_8009E0D4[];
extern volatile u8 D_8009E0D6[];
extern volatile u8 D_8009E0D8[];
extern volatile u8 D_8009E0DA[];
extern volatile u8 D_8009E0DC[];
extern volatile u8 D_8009E0DE[];

void func_80074D1C(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    register s32 voice asm("$11");
    register s32 start asm("$8");
    register s32 target asm("$9");
    register s32 step asm("$10");
    register s32 offset asm("$2");
    s32 delta;
    register s32 smallDenom asm("$3");
    s32 quotient;
    register s32 start16 asm("$5");
    register s32 target16 asm("$6");
    register s32 stepForSmallDiv asm("$4");
    s32 step16;

    voice = arg0;
    __asm__ volatile("" : "=r"(voice) : "0"(voice));
    start = arg1;
    target = arg2;
    __asm__ volatile("" : "=r"(start) : "0"(start));
    __asm__ volatile("" : "=r"(target) : "0"(target));
    step = arg3;
    start16 = (s16)arg1;
    target16 = (s16)arg2;

    if (start16 == target16) {
        return;
    }

    offset = (((((s16)arg0 * 2) + (s16)arg0) * 4) + (s16)arg0) * 4;
    *(volatile s16 *)(D_8009E0D4 + offset) = 1;
    *(volatile s16 *)(D_8009E0DC + offset) = start;
    *(volatile s16 *)(D_8009E0DE + offset) = target;

    step16 = (s16)arg3;
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
    stepForSmallDiv = (s16)step;
    smallDenom = (s16)start;
    offset = (s16)target;
    smallDenom -= offset;
    quotient = stepForSmallDiv / smallDenom;
    smallDenom = (s16)voice;
    offset = (((smallDenom * 2) + smallDenom) * 4 + smallDenom) * 4;
    *(volatile s16 *)(D_8009E0D6 + offset) = 1;
    *(volatile s16 *)(D_8009E0D8 + offset) = quotient;
    *(volatile s16 *)(D_8009E0DA + offset) = quotient;
    return;

large:
    stepForSmallDiv = (s16)start;
    offset = (s16)target;
    stepForSmallDiv -= offset;
    offset = (s16)step;
    quotient = stepForSmallDiv / offset;
    smallDenom = (s16)voice;
    offset = (((smallDenom * 2) + smallDenom) * 4 + smallDenom) * 4;
    *(volatile s16 *)(D_8009E0D8 + offset) = 0;
    *(volatile s16 *)(D_8009E0D6 + offset) = quotient;
}

typedef struct SpuVoice {
    u8 pad0[0x1C];
    s16 autoVolume;
    s16 volumeStep;
    s16 volumeCounter;
    s16 volumeCounterReload;
    s16 currentVolume;
    s16 targetVolume;
    u8 pad28[0xC];
} SpuVoice;

typedef struct SvmCurrent {
    u8 field0[4];
    u8 currentVolume;
    u8 pan;
    u8 field6[4];
    u8 masterVolume;
    u8 masterPan;
    u8 fieldC;
    u8 volume;
    u8 currentPan;
} SvmCurrent;

typedef struct VabHeader {
    u8 pad0[0x18];
    u8 masterVolume;
} VabHeader;

extern u16 D_8009DF20[];
extern u8 D_8009E0A0[];
extern SpuVoice D_8009E0B8[];
extern VabHeader *D_801E413C;
extern SvmCurrent D_801E4BD0;
extern s16 D_801E3FB0;

void func_80074ECC(s16 voice) {
    s16 currentVolume;
    u32 leftVolumeTemp;
    u32 rightVolumeTemp;
    u16 leftVolume;
    u16 rightVolume;
    s16 registerOffset;
    s32 scaledMasterVolume;

    registerOffset = voice * 8;
    if (D_8009E0B8[voice].volumeCounter != 0) {
        if (D_8009E0B8[voice].volumeCounterReload-- > 0) {
            return;
        }
        D_8009E0B8[voice].volumeCounterReload =
            D_8009E0B8[voice].volumeCounter;
    }

    D_8009E0B8[voice].currentVolume += D_8009E0B8[voice].volumeStep;
    if (D_8009E0B8[voice].volumeStep > 0) {
        if (D_8009E0B8[voice].currentVolume >=
            D_8009E0B8[voice].targetVolume) {
            D_8009E0B8[voice].currentVolume =
                D_8009E0B8[voice].targetVolume;
            D_8009E0B8[voice].autoVolume = 0;
        }
    } else if (D_8009E0B8[voice].volumeStep < 0) {
        if (D_8009E0B8[voice].currentVolume <=
            D_8009E0B8[voice].targetVolume) {
            D_8009E0B8[voice].currentVolume =
                D_8009E0B8[voice].targetVolume;
            D_8009E0B8[voice].autoVolume = 0;
        }
    }

    currentVolume = D_8009E0B8[voice].currentVolume;
    D_801E4BD0.currentVolume = currentVolume;
    scaledMasterVolume = D_801E413C->masterVolume * 0x3FFF;

    leftVolumeTemp =
        ((currentVolume * scaledMasterVolume) / 0x7F) / 0x7F;
    rightVolumeTemp =
        ((currentVolume * scaledMasterVolume) / 0x7F) / 0x7F;

    leftVolumeTemp =
        ((leftVolumeTemp * D_801E4BD0.masterVolume *
          D_801E4BD0.volume) /
         0x7F) /
        0x7F;
    rightVolumeTemp =
        ((rightVolumeTemp * D_801E4BD0.masterVolume *
          D_801E4BD0.volume) /
         0x7F) /
        0x7F;

    if (D_801E4BD0.currentPan < 0x40) {
        leftVolume = leftVolumeTemp;
        rightVolume =
            (rightVolumeTemp * D_801E4BD0.currentPan) / 0x40;
    } else {
        leftVolume =
            (leftVolumeTemp * (0x7F - D_801E4BD0.currentPan)) / 0x40;
        rightVolume = rightVolumeTemp;
    }

    if (D_801E4BD0.masterPan < 0x40) {
        rightVolume = (rightVolume * D_801E4BD0.masterPan) / 0x40;
    } else {
        leftVolume =
            (leftVolume * (0x7F - D_801E4BD0.masterPan)) / 0x40;
    }

    if (D_801E4BD0.pan < 0x40) {
        rightVolume = (rightVolume * D_801E4BD0.pan) / 0x40;
    } else {
        leftVolume = (leftVolume * (0x7F - D_801E4BD0.pan)) / 0x40;
    }

    if (D_801E3FB0 == 1) {
        if (rightVolume > leftVolume) {
            leftVolume = rightVolume;
        } else {
            rightVolume = leftVolume;
        }
    }

    D_8009DF20[registerOffset] = leftVolume;
    D_8009DF20[registerOffset + 1] = rightVolume;
    D_8009E0A0[voice] |= 3;
}

/*
 * func_8007521C declared these volatile, func_800753CC plain; every access in
 * both goes through an explicit (volatile s16 *) cast, so the plain form keeps
 * the accesses volatile and both members byte-exact.
 */
extern u8 D_8009E0E0[];
extern u8 D_8009E0E2[];
extern u8 D_8009E0E4[];
extern u8 D_8009E0E6[];
extern u8 D_8009E0E8[];
extern u8 D_8009E0EA[];

void func_8007521C(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    register s32 voice asm("$11");
    register s32 start asm("$8");
    register s32 target asm("$9");
    register s32 step asm("$10");
    register s32 offset asm("$2");
    s32 delta;
    register s32 smallDenom asm("$3");
    s32 quotient;
    register s32 start16 asm("$5");
    register s32 target16 asm("$6");
    register s32 stepForSmallDiv asm("$4");
    s32 step16;

    voice = arg0;
    __asm__ volatile("" : "=r"(voice) : "0"(voice));
    start = arg1;
    target = arg2;
    __asm__ volatile("" : "=r"(start) : "0"(start));
    __asm__ volatile("" : "=r"(target) : "0"(target));
    step = arg3;
    start16 = (s16)arg1;
    target16 = (s16)arg2;

    if (start16 == target16) {
        return;
    }

    offset = (((((s16)arg0 * 2) + (s16)arg0) * 4) + (s16)arg0) * 4;
    *(volatile s16 *)(D_8009E0E0 + offset) = 1;
    *(volatile s16 *)(D_8009E0E8 + offset) = start;
    *(volatile s16 *)(D_8009E0EA + offset) = target;

    step16 = (s16)arg3;
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
    stepForSmallDiv = (s16)step;
    smallDenom = (s16)start;
    offset = (s16)target;
    smallDenom -= offset;
    quotient = stepForSmallDiv / smallDenom;
    smallDenom = (s16)voice;
    offset = (((smallDenom * 2) + smallDenom) * 4 + smallDenom) * 4;
    *(volatile s16 *)(D_8009E0E2 + offset) = 1;
    *(volatile s16 *)(D_8009E0E4 + offset) = quotient;
    *(volatile s16 *)(D_8009E0E6 + offset) = quotient;
    return;

large:
    stepForSmallDiv = (s16)start;
    offset = (s16)target;
    stepForSmallDiv -= offset;
    offset = (s16)step;
    quotient = stepForSmallDiv / offset;
    smallDenom = (s16)voice;
    offset = (((smallDenom * 2) + smallDenom) * 4 + smallDenom) * 4;
    *(volatile s16 *)(D_8009E0E4 + offset) = 0;
    *(volatile s16 *)(D_8009E0E2 + offset) = quotient;
}

extern u8 D_8009E0A0[];
extern u16 D_8009DF20[];
extern u16 D_8009DF22[];
extern u8 D_801E4BD4;
extern u8 D_801E4BD5;
extern u8 D_801E4BDA;
extern u8 D_801E4BDB;
extern u8 D_801E4BDD;
extern u8 D_801E4BDE;
extern s16 D_801E3FB0;

void func_800753CC(s32 arg0) {
    s32 stack[6];
    register s32 originalArg asm("$8");
    s32 offset;
    register s32 channel asm("$3");
    register s32 index8 asm("$9");
    u16 counter;
    s32 sum;
    s32 step;
    register s32 limit asm("$3");
    register s32 current asm("$2");
    register s32 positiveCompare asm("$2");
    register s32 negativeCompare asm("$3");
    register s32 clampValue asm("$4");
    u32 envelope;

    asm(".globl func_80075420\nfunc_80075420 = func_800753CC + 0x54");
    channel = (s16)arg0;
    index8 = channel << 3;
    asm("" : "=r"(index8) : "0"(index8));
    offset = channel * 52;
    asm("" : "=r"(offset) : "0"(offset));
    originalArg = arg0;
    if (*(s16 *)&D_8009E0E4[offset] != 0) {
        counter = *(u16 *)&D_8009E0E6[offset];
        *(u16 *)&D_8009E0E6[offset] = counter - 1;
        if ((s16)counter > 0) {
            return;
        }
        *(u16 *)&D_8009E0E6[offset] = *(u16 *)&D_8009E0E4[offset];
    }

    step = *(s16 *)&D_8009E0E2[offset];
    sum = *(u16 *)&D_8009E0E8[offset] + *(u16 *)&D_8009E0E2[offset];
    *(u16 *)&D_8009E0E8[offset] = sum;
    if (step <= 0) {
        goto checkNegativeStep;
    }
    current = (u32)sum << 16;
    limit = *(s16 *)&D_8009E0EA[offset];
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
    current = (u32)sum << 16;
    limit = *(s16 *)&D_8009E0EA[offset];
    current >>= 16;
    clampValue = limit;
    negativeCompare = limit < current;
    asm("" : "=r"(negativeCompare) : "0"(negativeCompare));
    if (negativeCompare) {
        goto envelopeDone;
    }

clampEnvelope:
    *(u16 *)&D_8009E0E8[offset] = clampValue;
    *(u16 *)&D_8009E0E0[offset] = 0;

envelopeDone:
    envelope = D_8009E0E8[(s16)originalArg * 52];
    {
    register u8 *base asm("$2");
    register s32 level asm("$3");
    register s32 scaledLevel asm("$2");
    register s32 masterVolume asm("$4");
    register s32 dividend asm("$3");
    register u32 volume asm("$3");
    u32 pan;
    u32 left;
    u32 right;
    s32 mixed;
    register s32 outputOffset asm("$4");
    register u32 compareLeft asm("$2");
    register u32 compareRight asm("$3");
    register s32 flagIndex asm("$3");

    asm("" : : : "memory");
    base = (u8 *)D_801E413C;
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
    volume = (u32)volume / 16129U;

    pan = D_801E4BDE;
    left = volume;
    if (pan < 0x40) {
        asm("" : "=r"(volume) : "0"(volume));
        left = volume;
        right = ((u32)(volume * pan)) >> 6;
    } else {
        asm("" : "=r"(left) : "0"(left));
        right = left;
        left = ((u32)(left * (0x7F - pan))) >> 6;
    }

    pan = D_801E4BDB;
    if (pan < 0x40) {
        mixed = (u16)right * pan;
        if (mixed < 0) {
            mixed += 63;
        }
        right = (u32)mixed >> 6;
    } else {
        mixed = (u16)left * (0x7F - pan);
        if (mixed < 0) {
            mixed += 63;
        }
        left = (u32)mixed >> 6;
    }
    pan = envelope;
    if (pan < 0x40) {
        mixed = (u16)right * pan;
        if (mixed < 0) {
            mixed += 63;
        }
        right = (u32)mixed >> 6;
    } else {
        mixed = (u16)left * (0x7F - pan);
        if (mixed < 0) {
            mixed += 63;
        }
        left = (u32)mixed >> 6;
    }

    if (D_801E3FB0 == 1) {
        compareRight = (u16)right;
        compareLeft = (u16)left;
        if (compareLeft < compareRight) {
            left = right;
        } else {
            right = left;
        }
    }

    outputOffset = (s16)index8 << 1;
    flagIndex = (s16)originalArg;
    *(u16 *)((u8 *)D_8009DF22 + outputOffset) = right;
    asm("" : : : "memory");
    *(u16 *)((u8 *)D_8009DF20 + outputOffset) = left;
    D_8009E0A0[flagIndex] |= 3;
    }
}
