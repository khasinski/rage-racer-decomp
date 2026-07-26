#include "common.h"

extern s32 g_CdTrackPending asm("D_8007F600");
extern s32 g_CdCommandPending asm("D_8007F604");
extern s32 g_CdTrackStep asm("D_8007F608");
extern s32 g_CdCommandStep asm("D_8007F60C");

void GameRequestCdTrack(s32 arg0) asm("func_80042BC0");
void GameRequestCdTrack(s32 arg0) {
    g_CdTrackPending = arg0 & 0xFF;
    g_CdTrackStep = 0;
    g_CdCommandPending = -1;
    g_CdCommandStep = 0;
}
