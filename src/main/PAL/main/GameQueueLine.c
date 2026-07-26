#include "common.h"
#include "psyq/gpu.h"

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
