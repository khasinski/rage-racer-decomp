#include "common.h"
#include "game/cd.h"

extern u8 g_CdVolume;


void ApplyCdVolume(void);
void ApplyCdVolume(void) {
    SetCdVolume(g_CdVolume);
}
