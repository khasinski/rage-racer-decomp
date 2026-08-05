#include "common.h"
#include "game/cd.h"

extern volatile u8 g_CdVolume;


void SetCdVolumeSetting(s32 arg0);
void SetCdVolumeSetting(s32 arg0) {
    s32 product = (arg0 << 7) - arg0;

    g_CdVolume = product / 15;
    SetCdVolume(g_CdVolume);
}
