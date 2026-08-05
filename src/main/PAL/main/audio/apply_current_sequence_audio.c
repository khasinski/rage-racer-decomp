#include "common.h"
#include "game/audio.h"
#include "game/cd.h"
#include "game/memcard.h"

extern s16 g_SeqHandle;
extern s16 g_SeqVolume;
void SsSeqSetVol(s32 arg0, s32 arg1, s32 arg2);
void SetReverbDepth(s32 arg0, s32 arg1);
void ApplyCurrentSequenceAudio(void) {
    SsSeqSetVol(g_SeqHandle, g_SeqVolume, g_SeqVolume);
    SetReverbDepth(0x28, 0x28);
}

void SsSetMVol(s32 arg0, s32 arg1);
void SetMasterVolumeMono(s16 arg0);
void SetMasterVolumeMono(s16 arg0) { SsSetMVol(arg0, arg0); }
