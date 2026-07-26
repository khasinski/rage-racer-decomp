#include "common.h"
#include "game/race.h"

extern s16 D_801E6C90;

void GamePlaySoundCue(s32 cue) asm("func_8005D6EC");

void func_800410BC(s32 arg0) {
    if (arg0 < 0x110) {
        if (arg0 == 0x79) {
            GamePlaySoundCue(0x1E);
        } else if (arg0 == 0x97) {
            GamePlaySoundCue(0x1F);
        } else if (arg0 == 0xB5) {
            GamePlaySoundCue(0x20);
        } else if (arg0 == 0xD3) {
            GamePlaySoundCue(0x21);
        } else if ((arg0 == 0x10F) && (g_GrandPrixMode == 1)) {
            GamePlaySoundCue(0x22);
            D_801E6C90 = 0x5A;
        }
    }
}
