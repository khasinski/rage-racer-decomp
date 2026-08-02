#include "common.h"
#include "game/cd.h"

extern u8 g_CdVolume asm("D_8009B194");


void ApplyCdVolume(void) asm("func_8004310C");
void ApplyCdVolume(void) {
    SetCdVolume(g_CdVolume);
}
