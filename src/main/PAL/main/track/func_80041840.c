#include "common.h"

extern s32 g_VisibleCellList asm("D_801E4BC8");
void func_800414F0(s32 arg0, s32 arg1);
void func_80069858(s32 arg0);
void func_80027FF4(s32 arg0, s32 arg1, s32 arg2);
void func_80041840(void) {
    func_800414F0(-12288, 0x14000);
    func_80069858(0x1F800028);
    func_80027FF4(0x1F800000, g_VisibleCellList, 0x40);
}

void func_80041888(void) {
    func_800414F0(0xFFFF6000, 0x14000);
    func_80069858(0x1F800028);
    func_80027FF4(0x1F800000, g_VisibleCellList, 0x40);
}

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/track/func_80041840", func_800418D4);

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

void GameStartCdAudio(void) asm("func_80042BF0");
void GameStartCdAudio(void) {
    g_CdCommandPending = 1;
    g_CdCommandStep = 0;
}

void GamePauseCdAudio(void) asm("func_80042C0C");
void GamePauseCdAudio(void) {
    g_CdCommandPending = 2;
    g_CdCommandStep = 0;
}

extern s32 D_8007F5F8;
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

void GameResetCdAudioState(void) asm("func_80042C94");
void GameResetCdAudioState(void) {
    g_CdTrackPending = -1;
    g_CdCommandPending = -1;
    g_CdTrackStep = 0;
    g_CdCommandStep = 0;
    D_8009B1B0 = 2;
}
