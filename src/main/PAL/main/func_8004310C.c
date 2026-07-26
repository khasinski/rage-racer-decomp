#include "common.h"
#include "game/cd.h"

extern u8 g_CdVolume asm("D_8009B194");


void func_8004310C(void) {
    GameSetCdVolume(g_CdVolume);
}
