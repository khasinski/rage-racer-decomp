#include "common.h"
#include "game/audio.h"
#include "game/sound.h"
#include "psyq/snd.h"

void SetVabSlotVoiceEnabled(s32 voice, s32 enabled, s32 vabSlot) {
    s32 *state;

    if (enabled != 0) {
        s32 *base = g_SoundSlotActive;

        state = base + voice;
        if (*state == 0) {
            StartVabSlotVoice(voice, 0, (s16)vabSlot);
            *state = 1;
        }
    } else {
        s32 *base = g_SoundSlotActive;

        state = base + voice;
        if (*state != 0) {
            StopDirectVoice(voice);
            *state = 0;
        }
    }
}

void SetSequenceVolume(s32 volume) {
    g_SeqVolume = volume;
    SsSeqSetVol(g_SeqHandle, (s16)volume, (s16)volume);
}
