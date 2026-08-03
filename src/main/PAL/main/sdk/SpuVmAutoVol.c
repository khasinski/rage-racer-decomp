#include <sys/types.h>

#include "common.h"
#include "psyq/snd_types.h"

extern volatile u_char g_SndVoiceStateAutoVol[] asm("D_8009E0D4");
extern volatile u_char D_8009E0D6[];
extern volatile u_char D_8009E0D8[];
extern volatile u_char D_8009E0DA[];
extern volatile u_char D_8009E0DC[];
extern volatile u_char D_8009E0DE[];

void SpuVmAutoVol(long arg0, long arg1, long arg2, long arg3) asm("func_80074D1C");
void SpuVmAutoVol(long arg0, long arg1, long arg2, long arg3) {
    long voice;
    long start;
    long target;
    register long step asm("$10");
    register long offset asm("$2");
    long delta;
    register long smallDenom asm("$3");
    long quotient;
    long start16;
    long target16;
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

    switch (0) { default:
    step16 = (short)arg3;
    delta = start16 - target16;
    if (delta < 0) {
        offset = target16 - start16;
        if (offset >= step16) {
            break;
        }
    } else if (delta >= step16) {
        do {
        } while (0);
        break;
    }

    stepForSmallDiv = (short)step;
    smallDenom = (short)start;
    offset = (short)target;
    smallDenom -= offset;
    quotient = stepForSmallDiv / smallDenom;
    smallDenom = (short)voice;
    offset = ((smallDenom * 3) * 4 + smallDenom) * 4;
    *(volatile short *)(D_8009E0D6 + offset) = 1;
    *(volatile short *)(D_8009E0D8 + offset) = quotient;
    *(volatile short *)(D_8009E0DA + offset) = quotient;
    return;

    }
    stepForSmallDiv = (short)start;
    offset = (short)target;
    stepForSmallDiv -= offset;
    offset = (short)step;
    quotient = stepForSmallDiv / offset;
    smallDenom = (short)voice;
    offset = ((smallDenom * 3) * 4 + smallDenom) * 4;
    *(volatile short *)(D_8009E0D8 + offset) = 0;
    *(volatile short *)(D_8009E0D6 + offset) = quotient;
}

extern u_short g_SndVoiceRegs[] asm("D_8009DF20");
extern u_char g_SndVoiceFlags[] asm("D_8009E0A0");
extern SpuVoice g_SndVoiceState[] asm("D_8009E0B8");
extern VabHdr *g_SndCurrentVabHeader asm("D_801E413C");
extern SvmCurrentAttr g_SndCurrentAttr asm("D_801E4BD0");
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
    if (g_SndVoiceState[voice].volume_counter != 0) {
        if (g_SndVoiceState[voice].volume_counter_reload-- > 0) {
            return;
        }
        g_SndVoiceState[voice].volume_counter_reload =
            g_SndVoiceState[voice].volume_counter;
    }

    g_SndVoiceState[voice].start_volume += g_SndVoiceState[voice].volume_step;
    if (g_SndVoiceState[voice].volume_step > 0) {
        if (g_SndVoiceState[voice].start_volume >=
            g_SndVoiceState[voice].end_volume) {
            g_SndVoiceState[voice].start_volume =
                g_SndVoiceState[voice].end_volume;
            g_SndVoiceState[voice].auto_volume = 0;
        }
    } else if (g_SndVoiceState[voice].volume_step < 0) {
        if (g_SndVoiceState[voice].start_volume <=
            g_SndVoiceState[voice].end_volume) {
            g_SndVoiceState[voice].start_volume =
                g_SndVoiceState[voice].end_volume;
            g_SndVoiceState[voice].auto_volume = 0;
        }
    }

    currentVolume = g_SndVoiceState[voice].start_volume;
    g_SndCurrentAttr.volume = currentVolume;
    scaledMasterVolume = g_SndCurrentVabHeader->mvol * 0x3FFF;

    leftVolumeTemp =
        ((currentVolume * scaledMasterVolume) / 0x7F) / 0x7F;
    rightVolumeTemp =
        ((currentVolume * scaledMasterVolume) / 0x7F) / 0x7F;

    leftVolumeTemp =
        ((leftVolumeTemp * g_SndCurrentAttr.master_volume *
          g_SndCurrentAttr.tone_volume) /
         0x7F) /
        0x7F;
    rightVolumeTemp =
        ((rightVolumeTemp * g_SndCurrentAttr.master_volume *
          g_SndCurrentAttr.tone_volume) /
         0x7F) /
        0x7F;

    if (g_SndCurrentAttr.tone_pan < 0x40) {
        leftVolume = leftVolumeTemp;
        rightVolume =
            (rightVolumeTemp * g_SndCurrentAttr.tone_pan) / 0x40;
    } else {
        leftVolume =
            (leftVolumeTemp * (0x7F - g_SndCurrentAttr.tone_pan)) / 0x40;
        rightVolume = rightVolumeTemp;
    }

    if (g_SndCurrentAttr.master_pan < 0x40) {
        rightVolume = (rightVolume * g_SndCurrentAttr.master_pan) / 0x40;
    } else {
        leftVolume =
            (leftVolume * (0x7F - g_SndCurrentAttr.master_pan)) / 0x40;
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
extern u_char g_SndVoiceStatePanStep[] asm("D_8009E0E2");
extern u_char g_SndVoiceStatePanCounter[] asm("D_8009E0E4");
extern u_char g_SndVoiceStatePanCounterReload[] asm("D_8009E0E6");
extern u_char g_SndVoiceStateStartPan[] asm("D_8009E0E8");
extern u_char g_SndVoiceStateEndPan[] asm("D_8009E0EA");

void SpuVmAutoPan(long arg0, long arg1, long arg2, long arg3) asm("func_8007521C");
void SpuVmAutoPan(long arg0, long arg1, long arg2, long arg3) {
    long voice;
    long start;
    long target;
    register long step asm("$10");
    register long offset asm("$2");
    long delta;
    register long smallDenom asm("$3");
    long quotient;
    long start16;
    long target16;
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
    *(volatile short *)(g_SndVoiceStateStartPan + offset) = start;
    *(volatile short *)(g_SndVoiceStateEndPan + offset) = target;

    switch (0) { default:
    step16 = (short)arg3;
    delta = start16 - target16;
    if (delta < 0) {
        offset = target16 - start16;
        if (offset >= step16) {
            break;
        }
    } else if (delta >= step16) {
        do {
        } while (0);
        break;
    }

    stepForSmallDiv = (short)step;
    smallDenom = (short)start;
    offset = (short)target;
    smallDenom -= offset;
    quotient = stepForSmallDiv / smallDenom;
    smallDenom = (short)voice;
    offset = ((smallDenom * 3) * 4 + smallDenom) * 4;
    *(volatile short *)(g_SndVoiceStatePanStep + offset) = 1;
    *(volatile short *)(g_SndVoiceStatePanCounter + offset) = quotient;
    *(volatile short *)(g_SndVoiceStatePanCounterReload + offset) = quotient;
    return;

    }
    stepForSmallDiv = (short)start;
    offset = (short)target;
    stepForSmallDiv -= offset;
    offset = (short)step;
    quotient = stepForSmallDiv / offset;
    smallDenom = (short)voice;
    offset = ((smallDenom * 3) * 4 + smallDenom) * 4;
    *(volatile short *)(g_SndVoiceStatePanCounter + offset) = 0;
    *(volatile short *)(g_SndVoiceStatePanStep + offset) = quotient;
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
    long originalArg;
    long offset;
    long channel;
    long index8;
    u_short counter;
    long sum;
    long step;
    register long limit asm("$3");
    long current;
    register long positiveCompare asm("$2");
    register long negativeCompare asm("$3");
    register long clampValue asm("$4");
    u_long envelope;

    asm(".globl func_80075420\nfunc_80075420 = SpuVmAutoPanTick + 0x54");
    channel = (short)arg0;
    index8 = channel * 8;
    offset = channel * 52;
    originalArg = arg0;
    if (*(short *)&g_SndVoiceStatePanCounter[offset] != 0) {
        counter = *(u_short *)&g_SndVoiceStatePanCounterReload[offset];
        *(u_short *)&g_SndVoiceStatePanCounterReload[offset] = counter - 1;
        if ((short)counter > 0) {
            return;
        }
        *(u_short *)&g_SndVoiceStatePanCounterReload[offset] = *(u_short *)&g_SndVoiceStatePanCounter[offset];
    }

    step = *(short *)&g_SndVoiceStatePanStep[offset];
    sum = *(u_short *)&g_SndVoiceStateStartPan[offset] + *(u_short *)&g_SndVoiceStatePanStep[offset];
    *(u_short *)&g_SndVoiceStateStartPan[offset] = sum;
    switch (0) { default:
    if (!(step <= 0)) {
    current = (u_long)sum << 16;
    limit = *(short *)&g_SndVoiceStateEndPan[offset];
    current >>= 16;
    positiveCompare = current < limit;
    clampValue = limit;
    if (positiveCompare) {
        break;
    }

    } else {
    if (step >= 0) {
        break;
    }
    current = (u_long)sum << 16;
    limit = *(short *)&g_SndVoiceStateEndPan[offset];
    current >>= 16;
    clampValue = limit;
    negativeCompare = limit < current;
    if (negativeCompare) {
        break;
    }

    }
    *(u_short *)&g_SndVoiceStateStartPan[offset] = clampValue;
    *(u_short *)&g_SndVoiceStateAutoPan[offset] = 0;

    }
    envelope = g_SndVoiceStateStartPan[(short)originalArg * 52];
    {
    u_char *base;
    long level;
    long scaledLevel;
    long masterVolume;
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

    base = (u_char *)g_SndCurrentVabHeader;
    D_801E4BD5 = envelope;
    level = base[0x18];
    masterVolume = D_801E4BD4;
    scaledLevel = level * 16383;
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
        right = (u_long)mixed / 64;
    } else {
        mixed = (u_short)left * (0x7F - pan);
        if (mixed < 0) {
            mixed += 63;
        }
        left = (u_long)mixed / 64;
    }
    pan = envelope;
    if (pan < 0x40) {
        mixed = (u_short)right * pan;
        if (mixed < 0) {
            mixed += 63;
        }
        right = (u_long)mixed / 64;
    } else {
        mixed = (u_short)left * (0x7F - pan);
        if (mixed < 0) {
            mixed += 63;
        }
        left = (u_long)mixed / 64;
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

    outputOffset = (short)index8 * 2;
    flagIndex = (short)originalArg;
    *(u_short *)((u_char *)g_SndVoiceRegsVolRight + outputOffset) = right;
    asm("" : : : "memory");
    *(u_short *)((u_char *)g_SndVoiceRegs + outputOffset) = left;
    g_SndVoiceFlags[flagIndex] |= 3;
    }
}
