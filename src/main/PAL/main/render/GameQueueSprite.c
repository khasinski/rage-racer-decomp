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

/* Local wide-parameter declaration; see the definition below. */
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

/*
 * Local wide-parameter declaration. Retail passes every coordinate and colour
 * component as a full word - the stack arguments are read with `lw` - so the
 * s16 / u8 typing that game/render.h uses for documentation would make gcc
 * narrow the loads here. Only this TU needs the wide view, so it declares the
 * symbol itself rather than changing the shared header.
 */
u8 *GameQueueLine(
    void *ot,
    u8 *prim,
    s32 x0,
    s32 y0,
    s32 x1,
    s32 y1,
    s32 r,
    s32 g,
    s32 b) asm("func_800172D4");

/* LINE_F2, 16 bytes: one flat-shaded line, linked into `ot`. Returns the
 * advanced packet cursor. */
u8 *GameQueueLine(
    void *ot,
    u8 *prim,
    s32 x0,
    s32 y0,
    s32 x1,
    s32 y1,
    s32 r,
    s32 g,
    s32 b) {
    LINE_F2 *line = (LINE_F2 *)prim;

    SetLineF2(line);
    line->x0 = x0;
    line->y0 = y0;
    line->x1 = x1;
    line->y1 = y1;
    line->t.r0 = r;
    line->t.g0 = g;
    line->t.b0 = b;
    prim += 16;
    AddPrim(ot, line);
    return prim;
}
