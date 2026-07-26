#include "common.h"
#include "psyq/gpu.h"

/* Local wide-parameter declaration; see GameQueueShadedSprite.c. */
u8 *GameQueueShadedSpriteTrans(
    void *ot,
    u8 *prim,
    s32 x,
    s32 y,
    s32 w,
    s32 h,
    s32 u,
    s32 v,
    s32 clutIndex,
    s32 intensity) asm("func_8001705C");

/* SPRT, 20 bytes: GameQueueShadedSprite plus SetSemiTrans. */
u8 *GameQueueShadedSpriteTrans(
    void *ot,
    u8 *prim,
    s32 x,
    s32 y,
    s32 w,
    s32 h,
    s32 u,
    s32 v,
    s32 clutIndex,
    s32 intensity) {
    SPRT *sprt = (SPRT *)prim;

    SetSprt(sprt);
    SetSemiTrans(sprt, 1);
    sprt->x0 = x;
    sprt->y0 = y;
    sprt->w = w;
    sprt->h = h;
    sprt->u0 = u;
    sprt->v0 = v;
    sprt->t.r0 = intensity;
    sprt->t.g0 = intensity;
    sprt->t.b0 = intensity;
    sprt->clut = clutIndex;
    prim += 20;
    AddPrim(ot, sprt);
    return prim;
}
