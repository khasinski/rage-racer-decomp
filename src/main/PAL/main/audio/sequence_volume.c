#include "common.h"
#include "game/audio.h"
#include "game/sound.h"

void RefreshSequenceVolumeScale(void) {
    s32 temp = g_SeqVolumeSetting * 114;

    SetSequenceVolume(temp / 15);
}

void SetSequenceVolumeScale(s32 arg0) {
    s32 temp = arg0 * 114;

    g_SeqVolumeSetting = arg0;
    SetSequenceVolume(temp / 15);
}
