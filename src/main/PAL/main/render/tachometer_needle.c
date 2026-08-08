#include "common.h"
#include "game/car.h"
#include "game/render.h"
#include "game/track.h"
#include "psyq/gpu.h"

extern GameSpriteDesc g_TachoNeedleSprite;

void BuildTachoNeedleQuad(void) {
    u8 *data = (u8 *)g_CarSpec;
    u8 *prim0 = g_TachoNeedlePrim0;
    u8 *prim1 = g_TachoNeedlePrim1;
    GameSpriteDesc *src = &g_TachoNeedleSprite;

    g_TachoNeedleQuad[0][0] = -data[0x14F];
    g_TachoNeedleQuad[0][1] = data[0x14E];
    g_TachoNeedleQuad[1][0] = -data[0x14D];
    g_TachoNeedleQuad[1][1] = -data[0x14C];
    g_TachoNeedleQuad[2][0] = data[0x14F];
    g_TachoNeedleQuad[2][1] = data[0x14E];
    g_TachoNeedleQuad[3][0] = data[0x14D];
    g_TachoNeedleQuad[3][1] = -data[0x14C];

    src->x = *(u16 *)(data + 0x13C) + *(u16 *)(data + 0x138);
    src->y = *(u16 *)(data + 0x13E) + *(u16 *)(data + 0x13A);

    BuildSpriteFromDesc((SPRT *)prim0, src);
    BuildSpriteFromDesc((SPRT *)prim1, src);
    SetShadeTex(prim0, 0);
    SetShadeTex(prim1, 0);
    SetDrawMode((DrawPacket *)(prim0 - 0x18), 0, 1, 9, 0);
    SetDrawMode((DrawPacket *)(prim0 - 0x0C), 0, 1, 0xA, 0);
    SetDrawMode((DrawPacket *)g_TachoNeedlePrim1PageA, 0, 1, 9, 0);
    SetDrawMode((DrawPacket *)g_TachoNeedlePrim1PageB, 0, 1, 0xA, 0);
}
