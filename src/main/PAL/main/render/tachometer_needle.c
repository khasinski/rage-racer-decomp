#include "common.h"
#include "game/car.h"
#include "game/render.h"
#include "game/track.h"
#include "psyq/gpu.h"

void BuildTachoNeedleQuad(void) {
    CarTachometerSpec *spec = &g_CarSpec->tachometer;
    SPRT *prim0 = g_TachoNeedlePrim0;
    SPRT *prim1 = g_TachoNeedlePrim1;
    GameSpriteDesc *src = &g_TachoNeedleSprite;
    RenderBufferAddress prim0Address;

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

    BuildSpriteFromDesc(prim0, src);
    BuildSpriteFromDesc(prim1, src);
    SetShadeTex(prim0, 0);
    SetShadeTex(prim1, 0);
    prim0Address.sprite = prim0;
    prim0Address.drawPacket -= 2;
    SetDrawMode(&prim0Address.drawPacket[0], 0, 1, 9, 0);
    SetDrawMode(&prim0Address.drawPacket[1], 0, 1, 0xA, 0);
    SetDrawMode(&g_TachoNeedlePrim1PageA, 0, 1, 9, 0);
    SetDrawMode(&g_TachoNeedlePrim1PageB, 0, 1, 0xA, 0);
}
