#include "common.h"
#include "game/audio.h"

void GameSetCdVolumeSetting(s32 arg0) asm("func_80043134");

void GameSetSequenceVolumeSetting(s32 setting) {
    u32 adjusted;
    s32 value;

    value = setting;
    if (value < 0) {
        goto negative;
    }

    adjusted = setting;
    adjusted++;
    adjusted--;
    setting = adjusted;
    if (setting < 0x10) {
        goto call;
    }

    setting = 0xF;
    goto call;

negative:
    setting = 0;

call:
    value = setting;
    GameSetCdVolumeSetting(setting);
    GameSetSequenceVolumeScale(value);
}
