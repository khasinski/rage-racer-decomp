#include "common.h"

extern s32 D_8019C9A8;
extern s32 g_TrackTextureSectionLo asm("D_801E4284");
extern s32 g_TrackTextureSectionHi asm("D_801E4288");
/*
 * Copies the first two words of the active camera row (D_8019C9A8 holds a
 * pointer to the current CamRow) into the active view-param globals.
 */
void GameLoadTrackTexturePageRange(void) asm("func_8001D30C");
void GameLoadTrackTexturePageRange(void) {
    s32 *camRow;

    camRow = (s32 *)D_8019C9A8;
    g_TrackTextureSectionLo = camRow[0];
    g_TrackTextureSectionHi = camRow[1];
}
