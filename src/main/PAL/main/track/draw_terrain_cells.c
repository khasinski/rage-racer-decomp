#include "common.h"

extern s32 g_VisibleCellList asm("D_801E4BC8");
void BuildVisibleCells(s32 arg0, s32 arg1) asm("func_800414F0");
void func_80069858(s32 arg0);
void SubmitTerrainCells(s32 arg0, s32 arg1, s32 arg2) asm("func_80027FF4");
void DrawTerrainCells(void) asm("func_80041840");
void DrawTerrainCells(void) {
    BuildVisibleCells(-12288, 0x14000);
    func_80069858(0x1F800028);
    SubmitTerrainCells(0x1F800000, g_VisibleCellList, 0x40);
}

void DrawTerrainCellsWide(void) asm("func_80041888");
void DrawTerrainCellsWide(void) {
    BuildVisibleCells(0xFFFF6000, 0x14000);
    func_80069858(0x1F800028);
    SubmitTerrainCells(0x1F800000, g_VisibleCellList, 0x40);
}

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/track/draw_terrain_cells", func_800418D4);

extern s32 g_CdTrackPending asm("D_8007F600");
extern s32 g_CdCommandPending asm("D_8007F604");
extern s32 g_CdTrackStep asm("D_8007F608");
extern s32 g_CdCommandStep asm("D_8007F60C");

void RequestCdTrack(s32 arg0) asm("func_80042BC0");
void RequestCdTrack(s32 arg0) {
    g_CdTrackPending = arg0 & 0xFF;
    g_CdTrackStep = 0;
    g_CdCommandPending = -1;
    g_CdCommandStep = 0;
}

void StartCdAudio(void) asm("func_80042BF0");
void StartCdAudio(void) {
    g_CdCommandPending = 1;
    g_CdCommandStep = 0;
}

void PauseCdAudio(void) asm("func_80042C0C");
void PauseCdAudio(void) {
    g_CdCommandPending = 2;
    g_CdCommandStep = 0;
}

/* Set when the CD was stopped rather than paused, so a resume has to reissue
 * a play of g_CdCurrentTrack from the top instead of an unpause. g_CdCurrentTrack is the
 * track the CD-DA driver last selected; ResetCdAudioState parks it at 2. */
extern s32 g_CdRestartOnResume asm("D_8007F5F8");
extern u8 g_CdCurrentTrack asm("D_8009B1B0");

void ResumeCdAudio(void) asm("func_80042C28");
void ResumeCdAudio(void) {
    if (g_CdRestartOnResume != 0) {
        u8 value;

        value = g_CdCurrentTrack;
        g_CdTrackStep = 4;
        g_CdRestartOnResume = 0;
        g_CdCommandPending = 1;
        g_CdCommandStep = 0;
        g_CdTrackPending = value;
    } else {
        g_CdCommandPending = 3;
        g_CdCommandStep = 0;
    }
}

void ResetCdAudioState(void) asm("func_80042C94");
void ResetCdAudioState(void) {
    g_CdTrackPending = -1;
    g_CdCommandPending = -1;
    g_CdTrackStep = 0;
    g_CdCommandStep = 0;
    g_CdCurrentTrack = 2;
}
