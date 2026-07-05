#include "common.h"
#include "game/audio.h"

extern s32 D_80082F40;
extern s32 D_801E6CE4;
extern s32 D_801E6CE8;

void GameSetPanVoiceTargetVolume(s32 arg0, s32 arg1) {
    if (arg0 >= 0) {
        if (arg0 < 0x81) {
            goto arg0_done;
        }
        arg0 = 0x80;
    } else {
        arg0 = 0;
    }

arg0_done:
    if (arg1 >= 0) {
        if (arg1 < 0x81) {
            goto arg1_done;
        }
        arg1 = 0x80;
    } else {
        arg1 = 0;
    }

arg1_done:
    if (D_80082F40 != 0) {
        D_801E6CE4 = arg0;
        D_801E6CE8 = arg1;
    } else {
        register s32 temp asm("$2") = (arg0 + arg1) / 2;

        D_801E6CE4 = temp;
        D_801E6CE8 = temp;
    }
}
