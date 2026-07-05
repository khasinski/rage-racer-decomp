#include "common.h"
#include "game/audio.h"

extern s32 D_801E6CC8[];

void GameSetSoundSlotVoiceEnabled(s32 arg0, s32 arg1) {
    s32 *entry;
    register s32 *base asm("$2");
    register s32 offset asm("$3");

    if (arg1 != 0) {
        base = D_801E6CC8;
        offset = arg0 << 2;
        asm("" : "=r"(base), "=r"(offset) : "0"(base), "1"(offset));
        entry = (s32 *)(offset + (s32)base);
        if (*entry == 0) {
            GamePlaySoundSlotVoice(arg0, 0, 3);
            *entry = 1;
        }
    } else {
        base = D_801E6CC8;
        offset = arg0 << 2;
        asm("" : "=r"(base), "=r"(offset) : "0"(base), "1"(offset));
        entry = (s32 *)(offset + (s32)base);
        if (*entry != 0) {
            GameStopSoundSlotVoice(arg0);
            *entry = 0;
        }
    }
}
