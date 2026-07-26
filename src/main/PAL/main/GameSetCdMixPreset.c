#include "common.h"
#include "game/cd.h"

extern s32 D_8007F5FC;
extern u8 g_CdVolume asm("D_8009B194");


void GameSetCdMixPreset(s32 arg0) asm("func_8004318C");
void GameSetCdMixPreset(s32 arg0) {
    D_8007F5FC = arg0;
    GameSetCdVolume(g_CdVolume);
}
