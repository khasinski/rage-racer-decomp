#include "common.h"
#include "game/car.h"
#include "game/render.h"
#include "game/track.h"
#include "psyq/gpu.h"

void BuildTachoNeedleQuad(void) {
    CarTachometerSpec *spec = &g_CarSpec->tachometer;
    u8 *prim0 = g_TachoNeedlePrim0;
    u8 *prim1 = g_TachoNeedlePrim1;
    GameSpriteDesc *src = &g_TachoNeedleSprite;

    g_TachoNeedleQuad[0][0] = -spec->needleQuad[3];
    g_TachoNeedleQuad[0][1] = spec->needleQuad[2];
    g_TachoNeedleQuad[1][0] = -spec->needleQuad[1];
    g_TachoNeedleQuad[1][1] = -spec->needleQuad[0];
    g_TachoNeedleQuad[2][0] = spec->needleQuad[3];
    g_TachoNeedleQuad[2][1] = spec->needleQuad[2];
    g_TachoNeedleQuad[3][0] = spec->needleQuad[1];
    g_TachoNeedleQuad[3][1] = -spec->needleQuad[0];

    src->x = spec->faceDX + spec->needleX;
    src->y = spec->faceDY + spec->needleY;

    BuildSpriteFromDesc((SPRT *)prim0, src);
    BuildSpriteFromDesc((SPRT *)prim1, src);
    SetShadeTex(prim0, 0);
    SetShadeTex(prim1, 0);
    SetDrawMode((DrawPacket *)(prim0 - 0x18), 0, 1, 9, 0);
    SetDrawMode((DrawPacket *)(prim0 - 0x0C), 0, 1, 0xA, 0);
    SetDrawMode((DrawPacket *)g_TachoNeedlePrim1PageA, 0, 1, 9, 0);
    SetDrawMode((DrawPacket *)g_TachoNeedlePrim1PageB, 0, 1, 0xA, 0);
}
