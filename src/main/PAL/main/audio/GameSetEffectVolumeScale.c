#include "common.h"
#include "game/audio.h"

void GameSetEffectVolumeScale(s32 arg0) {
    if (arg0 >= 0) {
        if (arg0 < 0x81) {
            goto done;
        }
        arg0 = 0x80;
    } else {
        arg0 = 0;
    }

done:
    g_EffectVolumeScale = arg0;
}

extern s32 D_801E6CE0;

void GameSetLoadedTableVolumeScale(s32 arg0) {
    if (arg0 >= 0) {
        if (arg0 < 0x81) {
            goto done;
        }
        arg0 = 0x80;
    } else {
        arg0 = 0;
    }

done:
    D_801E6CE0 = arg0;
}

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

/* Set the effect master volume scale (g_EffectVolumeScale = SoundScale.scale) from a
 * 0..15 level, mapping it onto the 0..0x80 fixed-point scale used by the
 * effect-voice volume math. */
void GameSetEffectVolumeSetting(s32 level) asm("func_8005BDD4");
void GameSetEffectVolumeSetting(s32 level) {
    if (level >= 0) {
        if (level < 0x10) {
            goto done;
        }
        level = 0xF;
    } else {
        level = 0;
    }

done:
    g_EffectVolumeScale = (level << 7) / 15;
}

extern s32 D_80082F40;
void GameSetCdMixPreset(s32 arg0) asm("func_8004318C");
void func_80072AF4(void);
void GameSetStereoOutput(void) asm("func_8005BE24");
void GameSetStereoOutput(void) { D_80082F40 = 1; GameSetCdMixPreset(0); func_80072AF4(); }

void func_80072AE0(void);
void GameSetMonoOutput(void) asm("func_8005BE58");
void GameSetMonoOutput(void) { D_80082F40 = 0; GameSetCdMixPreset(1); func_80072AE0(); }

extern u32 D_801E6CC4;

u32 GameGetLoadedAudioStep(void) {
    return D_801E6CC4;
}

extern s32 g_AudioSlotMask asm("D_801E6C9C");

s32 GameGetActiveAudioSlots(void) {
    return g_AudioSlotMask;
}
