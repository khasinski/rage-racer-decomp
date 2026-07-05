#include "common.h"
#include "game/audio.h"

extern s32 D_801E6CF0;
extern s32 D_801E6CF8;
extern s32 D_801E6CFC;

void GameSetIndexedEffectVoice(s32 arg0, s32 arg1, s32 arg2) {
    if (arg0 >= -1) {
        if (arg0 >= 3) {
            arg0 = 2;
        }
    } else {
        arg0 = -1;
    }

    if (arg2 >= 0) {
        if (arg2 >= 0x80) {
            arg2 = 0x7F;
        }
    } else {
        arg2 = 0;
    }

    D_801E6CF0 = arg0;
    if (arg0 >= 0) {
        D_801E6CFC = arg2;
        D_801E6CF8 = arg1;
    }
}
