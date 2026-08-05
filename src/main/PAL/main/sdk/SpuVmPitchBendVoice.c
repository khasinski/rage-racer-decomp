#include <sys/types.h>

#include "common.h"

extern u_char g_SndVoiceStateProgActual[];
extern short g_SndVoiceStateNote[];
extern u_char g_SndVoiceStateTone[];
extern volatile u_char g_SndVoiceRegsPitch[];
extern volatile u_char g_SndVoiceFlags[];
extern u_char g_SndCurrentProgActual;
extern u_char g_SndCurrentTone;
extern short g_SndCurrentVoice;
extern u_char *g_SndCurrentToneTable;

u_short SpuVmCalculateTonePitch(long arg0, long arg1);

void SpuVmPitchBendVoice(long arg0, long arg1) {
    long voice = arg0;
    u_char voiceByte = voice;
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
    *pBd7 = g_SndVoiceStateProgActual[voiceOffset];
    g_SndCurrentTone = g_SndVoiceStateTone[voiceOffset];
    beaVal = voice & 0xFF;
    g_SndCurrentVoice = beaVal;

    sh4 = *pBd7 * 16;
    seg = (long)g_SndCurrentTone + sh4;
    x = (short)arg1;
    if (x >= 0) {
        u_char *e = g_SndCurrentToneTable;
        long prodA = x * e[(seg * 32) + 0xD];
        long qA = prodA / 127;
        note = *(u_short *)((u_char *)g_SndVoiceStateNote + voiceOffset) + qA;
        pitch = prodA - qA * 127;
    } else {
        u_char *e = g_SndCurrentToneTable;
        long prodB = x * e[(seg * 32) + 0xC];
        long qB = prodB / 127;
        note = *(u_short *)((u_char *)g_SndVoiceStateNote + voiceOffset) + qB - 1;
        pitch = qB + 127;
    }

    *(volatile short *)(g_SndVoiceRegsPitch + (dfIndex << 1)) = SpuVmCalculateTonePitch(note, pitch);
    g_SndVoiceFlags[voice & 0xFF] |= 4;
}

void SpuVmPitchBendNoOpA(void) {
}

void SpuVmPitchBendNoOpB(void) {
}
