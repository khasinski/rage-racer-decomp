#include "common.h"
#include "game/cd.h"

extern volatile u8 g_CdVolume asm("D_8009B194");


void func_80043134(s32 arg0) {
    s32 product = (arg0 << 7) - arg0;

    g_CdVolume = product / 15;
    GameSetCdVolume(g_CdVolume);
}
