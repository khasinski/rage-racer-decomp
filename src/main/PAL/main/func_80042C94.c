#include "common.h"

extern s32 g_CdTrackPending asm("D_8007F600");
extern s32 g_CdCommandPending asm("D_8007F604");
extern s32 g_CdTrackStep asm("D_8007F608");
extern s32 g_CdCommandStep asm("D_8007F60C");
extern u8 D_8009B1B0;

void func_80042C94(void) {
    g_CdTrackPending = -1;
    g_CdCommandPending = -1;
    g_CdTrackStep = 0;
    g_CdCommandStep = 0;
    D_8009B1B0 = 2;
}
