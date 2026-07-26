#include "common.h"
#include "psyq/gpu.h"

/* Local wide-parameter declaration; see GameQueueSprite.c. */
u8 *GameQueueSpriteTrans(
    void *ot,
    u8 *prim,
    s32 x,
    s32 y,
    s32 w,
    s32 h,
    s32 u,
    s32 v,
    s32 clutIndex) asm("func_80017138");

/* SPRT, 20 bytes: GameQueueSprite plus SetSemiTrans. */
u8 *GameQueueSpriteTrans(
    void *ot,
    u8 *prim,
    s32 x,
    s32 y,
    s32 w,
    s32 h,
    s32 u,
    s32 v,
    s32 clutIndex) {
    SPRT *sprt = (SPRT *)prim;

    SetSprt(sprt);
    SetSemiTrans(sprt, 1);
    SetShadeTex(sprt, 1);
    sprt->x0 = x;
    sprt->y0 = y;
    sprt->w = w;
    sprt->h = h;
    sprt->u0 = u;
    sprt->v0 = v;
    sprt->clut = clutIndex;
    prim += 20;
    AddPrim(ot, sprt);
    return prim;
}
