#include <sys/types.h>

#include "common.h"
#include "psyq/snd.h"
#include "game/audio.h"

#include "psyq/snd_internal.h"

void SpuVmPitchBendVoice(long voice, long bend) {
    long voiceCopy = voice;
    u_char voiceByte = voiceCopy;
    long dfIndex = voiceByte * 8;
    long voiceOffset;
    long x;
    long sh4;
    long seg;
    u_short note;
    u_short pitch;
    long beaVal;
    u_char *pBd7;

    if (voiceByte >= 0x18U) {
        return;
    }

    voiceOffset = ((((voiceByte * 3) << 2) + voiceByte) << 2);
    pBd7 = &g_SndCurrentProgActual;
    *pBd7 = ((u_char *)g_SndVoiceState + 16)[voiceOffset];
    g_SndCurrentTone = ((u_char *)g_SndVoiceState + 20)[voiceOffset];
    beaVal = voiceCopy & 0xFF;
    g_SndCurrentVoice = beaVal;

    sh4 = *pBd7 * 16;
    seg = (long)g_SndCurrentTone + sh4;
    x = (short)bend;
    if (x >= 0) {
        u_char *e = g_SndCurrentToneTable;
        long prodA = x * e[(seg * 32) + 0xD];
        long qA = prodA / 127;
        note = *(u_short *)((u_char *)((u_char *)g_SndVoiceState + 12) + voiceOffset) + qA;
        pitch = prodA - qA * 127;
    } else {
        u_char *e = g_SndCurrentToneTable;
        long prodB = x * e[(seg * 32) + 0xC];
        long qB = prodB / 127;
        note = *(u_short *)((u_char *)((u_char *)g_SndVoiceState + 12) + voiceOffset) + qB - 1;
        pitch = qB + 127;
    }

    *(volatile short *)(((u_char *)g_SndVoiceRegs + 4) + (dfIndex << 1)) = SpuVmCalculateTonePitch(note, pitch);
    g_SndVoiceFlags[voiceCopy & 0xFF] |= 4;
}

void SpuVmPitchBendNoOpA(void) {
}

void SpuVmPitchBendNoOpB(void) {
}
