#include "common.h"
#include "game/audio.h"

extern s16 g_SeqHandle;
void SsSeqPlay(s32 arg0, s32 arg1, s32 arg2);
void PlaySequence(void) { SsSeqPlay(g_SeqHandle, 1, 0); }

void SsSeqStop(s32 arg0);
void StopSequence(void) { SsSeqStop(g_SeqHandle); }

extern s32 g_ReverbFadeStep;
extern s32 g_SeqVolumeFadeStep;

void StartSequenceFadeOut(void) {
    g_SeqVolumeFadeStep = -4;
    g_ReverbFadeStep = -3;
}

extern s32 g_ReverbDepthL;
extern s32 g_ReverbDepthR;
extern s32 g_SeqVolume;

void SetReverbDepth(s32 arg0, s32 arg1);
void SetSequenceVolume(s32 arg0);
void SsSeqSetVol(s32 arg0, s32 arg1, s32 arg2);

void UpdateSequenceFadeOut(void) {
    register s32 *fadeStep asm("$4");
    s32 delta;
    s32 value;
    s32 arg0;
    s32 arg1;

    fadeStep = &g_ReverbFadeStep;
    asm volatile("" : "=r"(fadeStep) : "0"(fadeStep));
    delta = *fadeStep;
    if (delta != 0) {
        value = fadeStep[-2];
        value += delta;
        if ((fadeStep[-2] = value) < 0) {
            fadeStep[-2] = 0;
        }

        value = g_ReverbDepthR;
        value += delta;
        g_ReverbDepthR = value;
        if (value < 0) {
            g_ReverbDepthR = 0;
        }

        if ((fadeStep[-2] == 0) && (g_ReverbDepthR == 0)) {
            *fadeStep = 0;
        }
    }

    SetReverbDepth(g_ReverbDepthL, g_ReverbDepthR);

    value = g_SeqVolume;
    delta = g_SeqVolumeFadeStep;
    value += delta;
    g_SeqVolume = value;
    if (value <= 0) {
        g_SeqVolume = 0;
        g_SeqVolumeFadeStep = 0;
        StopSequence();
        CloseAudioSlot(6);
        arg0 = 0x28;
        arg1 = 0x28;
        SetReverbDepth(arg0, arg1);
    }

    SetSequenceVolume(g_SeqVolume);
}

void ApplyDuckedSequenceAudio(void) {
    s32 value;
    register s32 scaled asm("$2");
    s32 seq;
    s32 volume;

    value = g_SeqVolume;
    seq = g_SeqHandle;
    scaled = value * 2;
    {
        s32 rel = value;
        value = scaled + rel;
    }
    scaled = value;
    if (value < 0) {
        scaled = value + 3;
    }
    scaled <<= 0xE;
    volume = scaled >> 0x10;
    SsSeqSetVol(seq, volume, volume);
    SetReverbDepth(0x3C, 0x3C);
}
