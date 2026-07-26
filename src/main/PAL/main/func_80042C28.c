#include "common.h"

extern s32 D_8007F5F8;
extern s32 g_CdTrackPending asm("D_8007F600");
extern s32 g_CdCommandPending asm("D_8007F604");
extern s32 g_CdTrackStep asm("D_8007F608");
extern s32 g_CdCommandStep asm("D_8007F60C");
extern u8 D_8009B1B0;

void GameResumeCdAudio(void) asm("func_80042C28");
void GameResumeCdAudio(void) {
    if (D_8007F5F8 != 0) {
        u8 value;

        value = D_8009B1B0;
        g_CdTrackStep = 4;
        D_8007F5F8 = 0;
        g_CdCommandPending = 1;
        g_CdCommandStep = 0;
        g_CdTrackPending = value;
    } else {
        g_CdCommandPending = 3;
        g_CdCommandStep = 0;
    }
}
