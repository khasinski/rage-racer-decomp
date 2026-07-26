#include "common.h"
#include "psyq/gpu.h"

/* Local wide-parameter declaration; see GameQueueSprite.c. `clutIndex` really
 * is the narrow one here - retail reads it back out of its argument slot with
 * `lhu` at the point of use. */
u8 *GameQueueShadedSprite(
    void *ot,
    u8 *prim,
    s32 x,
    s32 y,
    s32 w,
    s32 h,
    s32 u,
    s32 v,
    s32 clutIndex,
    s32 intensity) asm("func_80016F8C");

/* SPRT, 20 bytes: a textured sprite modulated by `intensity` on all three
 * channels (no SetShadeTex, so the texel is shaded). */
u8 *GameQueueShadedSprite(
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
