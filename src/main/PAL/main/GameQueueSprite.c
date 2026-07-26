#include "common.h"
#include "psyq/gpu.h"

/*
 * Local wide-parameter declaration: retail passes every coordinate, texel and
 * CLUT index as a full word (the stack arguments are read with `lw`), so the
 * narrow documentation types in game/render.h would make gcc shrink the loads.
 */
u8 *GameQueueSprite(
    void *ot,
    u8 *prim,
    s32 x,
    s32 y,
    s32 w,
    s32 h,
    s32 u,
    s32 v,
    s32 clutIndex) asm("func_80016EC4");

/* SPRT, 20 bytes: a raw (SetShadeTex) textured sprite linked into `ot`.
 * Returns the advanced packet cursor. */
u8 *GameQueueSprite(
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
