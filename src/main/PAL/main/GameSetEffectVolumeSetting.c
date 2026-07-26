#include "common.h"
#include "game/audio.h"


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
