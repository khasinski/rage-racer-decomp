#include "common.h"
#include "game/audio.h"
extern s16 g_SeqHandle asm("D_801E6D90");
extern s32 D_801E6D94;
void func_80072698(s32 arg0, s32 arg1, s32 arg2);
void GameSetSequenceVolume(s32 volume) {
    D_801E6D94 = volume;
    func_80072698(g_SeqHandle, (s16)volume, (s16)volume);
}
