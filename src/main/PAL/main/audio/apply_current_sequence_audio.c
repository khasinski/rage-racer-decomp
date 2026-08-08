#include "common.h"
#include "game/audio.h"
#include "game/cd.h"
#include "game/memcard.h"

extern s16 g_SeqHandle;
extern s16 g_SeqVolume;
void SsSeqSetVol(s32 seq, s32 volLeft, s32 volRight);
void ApplyCurrentSequenceAudio(void) {
    SsSeqSetVol(g_SeqHandle, g_SeqVolume, g_SeqVolume);
    SetReverbDepth(0x28, 0x28);
}

void SsSetMVol(s32 volLeft, s32 volRight);
void SetMasterVolumeMono(s16 volume) { SsSetMVol(volume, volume); }
