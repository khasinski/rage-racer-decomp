#include "common.h"
#include "game/audio.h"
#include "game/sound.h"

extern s32 g_SoundSlotActive[];

void SetVabSlotVoiceEnabled(s32 arg0, s32 arg1, s32 arg2) {
    s32 *state;

    if (arg1 != 0) {
        s32 *base = g_SoundSlotActive;

        state = base + arg0;
        if (*state == 0) {
            StartVabSlotVoice(arg0, 0, (s16)arg2);
            *state = 1;
        }
    } else {
        s32 *base = g_SoundSlotActive;

        state = base + arg0;
        if (*state != 0) {
            StopDirectVoice(arg0);
            *state = 0;
        }
    }
}

void SsSeqSetVol(s32 arg0, s32 arg1, s32 arg2);
void SetSequenceVolume(s32 volume) {
    g_SeqVolume = volume;
    SsSeqSetVol(g_SeqHandle, (s16)volume, (s16)volume);
}
