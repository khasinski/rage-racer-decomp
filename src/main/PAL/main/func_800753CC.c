#include "common.h"

extern u8 D_8009E0E0[];
extern u8 D_8009E0E2[];
extern u8 D_8009E0E4[];
extern u8 D_8009E0E6[];
extern u8 D_8009E0E8[];
extern u8 D_8009E0EA[];
extern u8 D_8009E0A0[];
extern u16 D_8009DF20[];
extern u16 D_8009DF22[];
extern u8 D_801E4BD4;
extern u8 D_801E4BD5;
extern u8 D_801E4BDA;
extern u8 D_801E4BDB;
extern u8 D_801E4BDD;
extern u8 D_801E4BDE;
extern u8 *D_801E413C;
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
    base = D_801E413C;
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
