#include "common.h"
#include "game/audio.h"
#include "game/sound.h"

void ApplyCurrentSequenceAudio(void) {
    SsSeqSetVol(g_SeqHandle.value, *(s16 *)&g_SeqVolume, *(s16 *)&g_SeqVolume);
    SetReverbDepth(0x28, 0x28);
}

void SetMasterVolumeMono(s16 volume) { SsSetMVol(volume, volume); }
