#include "common.h"
#include "game/prim.h"

u8 *DrawShadowedTile(void *ot, u8 *prim, s32 x, s32 y) {
    u8 *next;

    next = AddTilePrim(ot, prim, x + 1, y + 2, 0xC2, 0x1C, 0, 0, 0);
    return AddTilePrim(ot, next, x, y, 0xC4, 0x20, 0xFF, 0xFF, 0xFF);
}
