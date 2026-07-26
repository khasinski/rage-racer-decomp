#include "common.h"

extern s32 D_801E6CA4;

/* Set the effect master volume scale (D_801E6CA4 = SoundScale.scale) from a
 * 0..15 level, mapping it onto the 0..0x80 fixed-point scale used by the
 * effect-voice volume math. */
void func_8005BDD4(s32 level) {
    if (level >= 0) {
        if (level < 0x10) {
            goto done;
        }
        level = 0xF;
    } else {
        level = 0;
    }

done:
    D_801E6CA4 = (level << 7) / 15;
}
