#include "common.h"
#include "game/cd.h"

extern s32 D_8007F5FC;
extern u8 g_CdVolume asm("D_8009B194");


void func_8004318C(s32 arg0) {
    D_8007F5FC = arg0;
    GameSetCdVolume(g_CdVolume);
}
