#include "common.h"
#include "game/cd.h"

extern u8 g_CdVolume asm("D_8009B194");


void GameApplyCdVolume(void) asm("func_8004310C");
void GameApplyCdVolume(void) {
    GameSetCdVolume(g_CdVolume);
}
