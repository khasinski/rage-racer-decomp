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
