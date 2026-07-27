#include "common.h"
#include "game/audio.h"

extern s16 g_SeqHandle asm("D_801E6D90");
void func_800725F0(s32 arg0, s32 arg1, s32 arg2);
void GamePlaySequence(void) asm("func_8005E88C");
void GamePlaySequence(void) { func_800725F0(g_SeqHandle, 1, 0); }

void func_800728A0(s32 arg0);
void GameStopSequence(void) asm("func_8005E8B8");
void GameStopSequence(void) { func_800728A0(g_SeqHandle); }

extern s32 g_ReverbFadeStep asm("D_801E6D8C");
extern s32 g_SeqVolumeFadeStep asm("D_801E6D9C");

void GameStartSequenceFadeOut(void) {
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
void func_80072698(s32 arg0, s32 arg1, s32 arg2);

void GameUpdateSequenceFadeOut(void) asm("func_8005E900");
void GameUpdateSequenceFadeOut(void) {
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

void GameApplyDuckedSequenceAudio(void) asm("func_8005EA14");
void GameApplyDuckedSequenceAudio(void) {
    /* These pins are load-bearing: removing any one changes .text. */
    register s32 value asm("$3");
    register s32 scaled asm("$2");
    s32 seq;
    s32 volume;

    value = g_SeqVolume;
    seq = g_SeqHandle;
    asm volatile("" : : "r"(seq));
    scaled = value << 1;
    value = scaled + value;
    scaled = value;
    if (value < 0) {
        scaled = value + 3;
    }
    scaled <<= 0xE;
    volume = scaled >> 0x10;
    func_80072698(seq, volume, volume);
    func_8005B190(0x3C, 0x3C);
}
