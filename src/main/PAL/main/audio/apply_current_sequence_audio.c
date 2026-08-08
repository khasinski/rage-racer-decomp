#include "common.h"
#include "game/audio.h"

extern s16 g_SeqHandle;
extern s16 g_SeqVolume;
void ApplyCurrentSequenceAudio(void) {
    SsSeqSetVol(g_SeqHandle, g_SeqVolume, g_SeqVolume);
    SetReverbDepth(0x28, 0x28);
}

void SetMasterVolumeMono(s16 volume) { SsSetMVol(volume, volume); }
