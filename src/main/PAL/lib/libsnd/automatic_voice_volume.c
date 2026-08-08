#include <sys/types.h>

#include "common.h"
#include "psyq/snd_types.h"
#include "psyq/snd.h"

#include "psyq/snd_internal.h"

/* The three identity asms and the one on stepArg are load-bearing: each keeps
 * the local copy a distinct pseudo from the parameter it was taken from, so
 * the entry block reads the argument registers while the two join blocks read
 * the copies.  Without them cse1 merges each pair into one register.  */
void SpuVmAutoVol(long voiceArg, long startArg, long targetArg, long stepArg) {
    long voice;
    long start;
    long target;
    long step;
    long start16;
    long target16;
    long step16;

    voice = voiceArg;
    __asm__ volatile("" : "=r"(voice) : "0"(voice));
    start = startArg;
    __asm__ volatile("" : "=r"(start) : "0"(start));
    target = targetArg;
    __asm__ volatile("" : "=r"(target) : "0"(target));
    step = stepArg;

    start16 = (short)startArg;
    target16 = (short)targetArg;

    if (start16 == target16) {
        return;
    }

    {
    long offset = (short)voiceArg * 52;
    *(volatile short *)(((u_char *)g_SndVoiceState + 28) + offset) = 1;
    *(volatile short *)(((u_char *)g_SndVoiceState + 36) + offset) = start;
    *(volatile short *)(((u_char *)g_SndVoiceState + 38) + offset) = target;
    }

    __asm__ ("" : "=r"(stepArg) : "0"(stepArg));
    step16 = (short)stepArg;
    if (start16 - target16 >= 0) {
        if (start16 - target16 < step16) {
            goto small;
        }
    } else if (target16 - start16 < step16) {
        goto small;
    }
    goto big;

small:
    {
    long offset;
    long quotient;
    quotient = (short)step / ((short)start - (short)target);
    offset = (short)voice * 52;
    *(volatile short *)(((u_char *)g_SndVoiceState + 30) + offset) = 1;
    *(volatile short *)(((u_char *)g_SndVoiceState + 32) + offset) = quotient;
    *(volatile short *)(((u_char *)g_SndVoiceState + 34) + offset) = quotient;
    return;
    }

big:
    {
    long offset;
    long quotient;
    quotient = (short)start;
    quotient -= (short)target;
    quotient /= (short)step;
    offset = (short)voice * 52;
    *(volatile short *)(((u_char *)g_SndVoiceState + 32) + offset) = 0;
    *(volatile short *)(((u_char *)g_SndVoiceState + 30) + offset) = quotient;
    }
}


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

    ((u_short *)g_SndVoiceRegs)[registerOffset] = leftVolume;
    ((u_short *)g_SndVoiceRegs)[registerOffset + 1] = rightVolume;
    g_SndVoiceFlags[voice] |= 3;
}

/* Same shape as SpuVmAutoVol above; see the note there about the asms. */
void SpuVmAutoPan(long voiceArg, long startArg, long targetArg, long stepArg) {
    long voice;
    long start;
    long target;
    long step;
    long start16;
    long target16;
    long step16;

    voice = voiceArg;
    __asm__ volatile("" : "=r"(voice) : "0"(voice));
    start = startArg;
    __asm__ volatile("" : "=r"(start) : "0"(start));
    target = targetArg;
    __asm__ volatile("" : "=r"(target) : "0"(target));
    step = stepArg;

    start16 = (short)startArg;
    target16 = (short)targetArg;

    if (start16 == target16) {
        return;
    }

    {
    long offset = (short)voiceArg * 52;
    *(volatile short *)(((u_char *)g_SndVoiceState + 40) + offset) = 1;
    *(volatile short *)(((u_char *)g_SndVoiceState + 48) + offset) = start;
    *(volatile short *)(((u_char *)g_SndVoiceState + 50) + offset) = target;
    }

    __asm__ ("" : "=r"(stepArg) : "0"(stepArg));
    step16 = (short)stepArg;
    if (start16 - target16 >= 0) {
        if (start16 - target16 < step16) {
            goto small;
        }
    } else if (target16 - start16 < step16) {
        goto small;
    }
    goto big;

small:
    {
    long offset;
    long quotient;
    quotient = (short)step / ((short)start - (short)target);
    offset = (short)voice * 52;
    *(volatile short *)(((u_char *)g_SndVoiceState + 42) + offset) = 1;
    *(volatile short *)(((u_char *)g_SndVoiceState + 44) + offset) = quotient;
    *(volatile short *)(((u_char *)g_SndVoiceState + 46) + offset) = quotient;
    return;
    }

big:
    {
    long offset;
    long quotient;
    quotient = (short)start;
    quotient -= (short)target;
    quotient /= (short)step;
    offset = (short)voice * 52;
    *(volatile short *)(((u_char *)g_SndVoiceState + 44) + offset) = 0;
    *(volatile short *)(((u_char *)g_SndVoiceState + 42) + offset) = quotient;
    }
}


void SpuVmAutoPanTick(long voice) {
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
    register long clampValue asm("$4");
    u_long envelope;

    asm(".globl func_80075420\nfunc_80075420 = SpuVmAutoPanTick + 0x54");
    channel = (short)voice;
    index8 = channel * 8;
    offset = channel * 52;
    originalArg = voice;
    if (*(short *)&((u_char *)g_SndVoiceState + 44)[offset] != 0) {
        counter = *(u_short *)&((u_char *)g_SndVoiceState + 46)[offset];
        *(u_short *)&((u_char *)g_SndVoiceState + 46)[offset] = counter - 1;
        if ((short)counter > 0) {
            return;
        }
        *(u_short *)&((u_char *)g_SndVoiceState + 46)[offset] = *(u_short *)&((u_char *)g_SndVoiceState + 44)[offset];
    }

    step = *(short *)&((u_char *)g_SndVoiceState + 42)[offset];
    sum = *(u_short *)&((u_char *)g_SndVoiceState + 48)[offset] + *(u_short *)&((u_char *)g_SndVoiceState + 42)[offset];
    *(u_short *)&((u_char *)g_SndVoiceState + 48)[offset] = sum;
    switch (0) { default:
    if (!(step <= 0)) {
    current = (u_long)sum << 16;
    limit = *(short *)&((u_char *)g_SndVoiceState + 50)[offset];
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
    limit = *(short *)&((u_char *)g_SndVoiceState + 50)[offset];
    current >>= 16;
    clampValue = limit;
    limit = limit < current;
    if (limit) {
        break;
    }

    }
    *(u_short *)&((u_char *)g_SndVoiceState + 48)[offset] = clampValue;
    *(u_short *)&((u_char *)g_SndVoiceState + 40)[offset] = 0;

    }
    envelope = ((u_char *)g_SndVoiceState + 48)[(short)originalArg * 52];
    {
    u_char *base;
    long level;
    long scaledLevel;
    long masterVolume;
    u_long pan;
    u_long left;
    u_long right;
    long mixed;

    base = (u_char *)g_SndCurrentVabHeader;
    g_SndCurrentPan = envelope;
    level = base[0x18];
    masterVolume = g_SndCurrentVolume;
    scaledLevel = level * 16383;
    limit = masterVolume * scaledLevel;
    limit = limit / 16129;
    limit = limit * g_SndCurrentMasterVolume;
    limit = limit * g_SndCurrentToneVolume;
    limit = (u_long)limit / 16129U;

    pan = g_SndCurrentTonePan;
    left = limit;
    if (pan < 0x40) {
        asm("" : "=r"(limit) : "0"(limit));
        left = limit;
        right = ((u_long)(limit * pan)) >> 6;
    } else {
        asm("" : "=r"(left) : "0"(left));
        right = left;
        left = ((u_long)(left * (0x7F - pan))) >> 6;
    }

    pan = g_SndCurrentMasterPan;
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
        limit = (u_short)right;
        positiveCompare = (u_short)left;
        if ((u_long)positiveCompare < (u_long)limit) {
            left = right;
        } else {
            right = left;
        }
    }

    clampValue = (short)index8 * 2;
    limit = (short)originalArg;
    *(u_short *)((u_char *)((u_char *)g_SndVoiceRegs + 2) + clampValue) = right;
    asm("" : : : "memory");
    *(u_short *)((u_char *)g_SndVoiceRegs + clampValue) = left;
    g_SndVoiceFlags[limit] |= 3;
    }
}
