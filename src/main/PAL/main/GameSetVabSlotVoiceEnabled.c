#include "common.h"
#include "game/audio.h"

extern s32 D_801E6CC8[];

void GameSetVabSlotVoiceEnabled(s32 arg0, s32 arg1, s32 arg2) {
    s32 *state;

    if (arg1 != 0) {
        s32 *base = D_801E6CC8;

        state = base + arg0;
        if (*state == 0) {
            GameStartVabSlotVoice(arg0, 0, (s16)arg2);
            *state = 1;
        }
    } else {
        s32 *base = D_801E6CC8;

        state = base + arg0;
        if (*state != 0) {
            GameStopDirectVoice(arg0);
            *state = 0;
        }
    }
}

extern s16 g_SeqHandle asm("D_801E6D90");
extern s32 D_801E6D94;
void func_80072698(s32 arg0, s32 arg1, s32 arg2);
void GameSetSequenceVolume(s32 volume) {
    D_801E6D94 = volume;
    func_80072698(g_SeqHandle, (s16)volume, (s16)volume);
}
