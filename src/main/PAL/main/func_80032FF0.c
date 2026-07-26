#include "common.h"
#include "psyq/gpu.h"
#include "game/render.h"

/* Expands a GameSpriteDesc into a scratchpad SPRT. */
void func_80032FF0(SPRT *prim, GameSpriteDesc *src) {
    SetSprt(prim);

    prim->x0 = src->x;
    prim->y0 = src->y;
    prim->w = src->w;
    prim->h = src->h;
    prim->u0 = src->u0;
    prim->v0 = src->v0;
    prim->clut = src->clut;
    SetSemiTrans(prim, src->semiTrans);
    SetShadeTex(prim, 1);
}
