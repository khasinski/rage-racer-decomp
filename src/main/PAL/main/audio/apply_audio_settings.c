#include "common.h"
#include "game/audio.h"
#include "game/menu.h"
#include "game/race.h"
#include "game/sound.h"
#include "game/state.h"

void ApplyAudioSettings(void) {
    SetSequenceVolumeSetting(g_BgmVolumeSetting);
    SetEffectVolumeSetting(g_SfxVolumeSetting);
    if (g_MonoOutput == 0) {
        SetStereoOutput();
    } else {
        SetMonoOutput();
    }
}
