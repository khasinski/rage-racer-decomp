#include "common.h"
#include "game/audio.h"

extern s16 g_SeqHandle asm("D_801E6D90");
void SsSeqPlay(s32 arg0, s32 arg1, s32 arg2) asm("func_800725F0");
void PlaySequence(void) asm("func_8005E88C");
void PlaySequence(void) { SsSeqPlay(g_SeqHandle, 1, 0); }

void SsSeqStop(s32 arg0) asm("func_800728A0");
void StopSequence(void) asm("func_8005E8B8");
void StopSequence(void) { SsSeqStop(g_SeqHandle); }

extern s32 g_ReverbFadeStep asm("D_801E6D8C");
extern s32 g_SeqVolumeFadeStep asm("D_801E6D9C");

void StartSequenceFadeOut(void) {
    g_SeqVolumeFadeStep = -4;
    g_ReverbFadeStep = -3;
}

extern s32 g_ReverbDepthL asm("D_801E6D84");
extern s32 g_ReverbDepthR asm("D_801E6D88");
extern s32 g_SeqVolume asm("D_801E6D94");

void func_8005B190(s32 arg0, s32 arg1);
void func_8005E600(s32 arg0);
void func_8005E7A0(s32 arg0);
void func_8005E8B8(void);
void SsSeqSetVol(s32 arg0, s32 arg1, s32 arg2) asm("func_80072698");

void UpdateSequenceFadeOut(void) asm("func_8005E900");
void UpdateSequenceFadeOut(void) {
    /* This pin is load-bearing: removing it changes .text. */
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

    func_8005B190(g_ReverbDepthL, g_ReverbDepthR);

    value = g_SeqVolume;
    delta = g_SeqVolumeFadeStep;
    value += delta;
    g_SeqVolume = value;
    if (value <= 0) {
        g_SeqVolume = 0;
        g_SeqVolumeFadeStep = 0;
        func_8005E8B8();
        func_8005E600(6);
        arg0 = 0x28;
        arg1 = 0x28;
        func_8005B190(arg0, arg1);
    }

    func_8005E7A0(g_SeqVolume);
}

void ApplyDuckedSequenceAudio(void) asm("func_8005EA14");
void ApplyDuckedSequenceAudio(void) {
    /* These pins are load-bearing: removing any one changes .text. */
    s32 value;
    register s32 scaled asm("$2");
    s32 seq;
    s32 volume;

    value = g_SeqVolume;
    seq = g_SeqHandle;
    scaled = value << 1;
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
    func_8005B190(0x3C, 0x3C);
}
