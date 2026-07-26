#include "common.h"
#include "psyq/gpu.h"

/* Local wide-parameter declaration; see GameQueueSprite.c. */
u8 *GameQueueTileTrans(
    void *ot,
    u8 *prim,
    s32 x,
    s32 y,
    s32 w,
    s32 h,
    s32 r,
    s32 g,
    s32 b) asm("func_8001720C");

/* TILE, 16 bytes: a semi-transparent solid rectangle linked into `ot`.
 * Returns the advanced packet cursor. */
u8 *GameQueueTileTrans(
    void *ot,
    u8 *prim,
    s32 x,
    s32 y,
    s32 w,
    s32 h,
    s32 r,
    s32 g,
    s32 b) {
    TILE *tile = (TILE *)prim;

    SetTile(tile);
    SetSemiTrans(tile, 1);
    tile->x0 = x;
    tile->y0 = y;
    tile->w = w;
    tile->h = h;
    tile->t.r0 = r;
    tile->t.g0 = g;
    tile->t.b0 = b;
    prim += 16;
    AddPrim(ot, tile);
    return prim;
}
