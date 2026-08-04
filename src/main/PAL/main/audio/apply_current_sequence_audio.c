#include "common.h"
#include "game/audio.h"
#include "game/cd.h"
#include "game/memcard.h"

extern s16 g_SeqHandle asm("D_801E6D90");
extern s16 g_SeqVolume asm("D_801E6D94");
void SsSeqSetVol(s32 arg0, s32 arg1, s32 arg2) asm("func_80072698");
void func_8005B190(s32 arg0, s32 arg1);
void ApplyCurrentSequenceAudio(void) asm("func_8005EA6C");
void ApplyCurrentSequenceAudio(void) {
    SsSeqSetVol(g_SeqHandle, g_SeqVolume, g_SeqVolume);
    func_8005B190(0x28, 0x28);
}

void SsSetMVol(s32 arg0, s32 arg1) asm("func_8006EAFC");
void SetMasterVolumeMono(s16 arg0) asm("func_8005EAA8");
void SetMasterVolumeMono(s16 arg0) { SsSetMVol(arg0, arg0); }
