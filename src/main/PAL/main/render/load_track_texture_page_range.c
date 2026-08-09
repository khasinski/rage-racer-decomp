#include "common.h"
#include "game/render.h"
#include "game/render_internal.h"

/*
 * Copies the first two words of the active camera row (g_CamRow holds a
 * pointer to the current CamRow) into the active view-param globals.
 */
void LoadTrackTexturePageRange(void) {
    CamRow *camRow = (CamRow *)g_CamRow;

    g_TrackTextureSectionLo = camRow->textureSectionLo;
    g_TrackTextureSectionHi = camRow->textureSectionHi;
}
