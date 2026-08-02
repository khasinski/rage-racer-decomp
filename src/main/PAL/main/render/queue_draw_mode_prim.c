#include "common.h"
#include "game/render.h"
#include "psyq/gpu.h"
#include "psyq/gte.h"

extern u8 D_8007BED0[];

/* DR_MODE, 12 bytes: sets the texture page (and the blend mode packed into it)
 * for the primitives that follow, links it into the ordering table and returns
 * the advanced packet cursor. */
u8 *GameQueueDrawModePrim(void *ot, u8 *prim, u16 tpage) {
    u8 *pkt;

    SetDrawMode((DrawPacket *)prim, 0, 1, tpage, D_8007BED0);
    pkt = prim;
    prim += 12;
    AddPrim(ot, pkt);
    return prim;
}

u8 *GameQueueShadedTexturedRect(
    void *ot,
    u8 *prim,
    s16 x,
    s16 y,
    s16 w,
    s16 h,
    u8 u,
    u8 v,
    u16 clutIndex,
    u16 tpage,
    u8 intensity) {
    POLY_FT4 *packet;
    s16 width = w;
    s16 height = h;
    u8 u0 = u;
    u8 v0 = v;

    SetPolyFT4((POLY_FT4 *)prim);
    if (width < 0) {
        width += 1;
        u0 -= width;
    }
    if (height < 0) {
        height += 1;
        v0 -= height;
    }

    (((POLY_FT4 *)prim)->x0 = x,
     ((POLY_FT4 *)prim)->y0 = y,
     ((POLY_FT4 *)prim)->x1 = x + (width < 0 ? -width : width),
     ((POLY_FT4 *)prim)->y1 = y,
     ((POLY_FT4 *)prim)->x2 = x,
     ((POLY_FT4 *)prim)->y2 = y + (height < 0 ? -height : height),
     ((POLY_FT4 *)prim)->x3 = x + (width < 0 ? -width : width),
     ((POLY_FT4 *)prim)->y3 = y + (height < 0 ? -height : height));
    ((POLY_FT4 *)prim)->u0 = u0;
    ((POLY_FT4 *)prim)->v0 = v0;
    ((POLY_FT4 *)prim)->u1 = u0 + width;
    ((POLY_FT4 *)prim)->v1 = v0;
    ((POLY_FT4 *)prim)->u2 = u0;
    ((POLY_FT4 *)prim)->v2 = v0 + height;
    ((POLY_FT4 *)prim)->u3 = u0 + width;
    ((POLY_FT4 *)prim)->v3 = v0 + height;
    ((POLY_FT4 *)prim)->t.r0 = intensity;
    ((POLY_FT4 *)prim)->t.g0 = intensity;
    ((POLY_FT4 *)prim)->t.b0 = intensity;
    ((POLY_FT4 *)prim)->clut = clutIndex;
    ((POLY_FT4 *)prim)->tpage = tpage;
    packet = (POLY_FT4 *)prim;
    prim += sizeof(POLY_FT4);
    AddPrim(ot, packet);
    return prim;
}

u8 *GameQueueTexturedRect(
    void *ot,
    u8 *prim,
    s16 x,
    s16 y,
    s16 w,
    s16 h,
    u8 u,
    u8 v,
    u8 uSpan,
    u8 vSpan,
    u16 clutIndex,
    u16 tpage) {
    POLY_FT4 *packet;
    s16 width = w;
    s16 height = h;
    u8 u0 = u;
    u8 v0 = v;

    SetPolyFT4((POLY_FT4 *)prim);
    SetShadeTex((POLY_FT4 *)prim, 1);

    if (width < 0) {
        u0 -= width + 1;
    }
    if (height < 0) {
        v0 -= height + 1;
    }

    (((POLY_FT4 *)prim)->x0 = x,
     ((POLY_FT4 *)prim)->y0 = y,
     ((POLY_FT4 *)prim)->x1 = x + (width < 0 ? -width : width),
     ((POLY_FT4 *)prim)->y1 = y,
     ((POLY_FT4 *)prim)->x2 = x,
     ((POLY_FT4 *)prim)->y2 = y + (height < 0 ? -height : height),
     ((POLY_FT4 *)prim)->x3 = x + (width < 0 ? -width : width),
     ((POLY_FT4 *)prim)->y3 = y + (height < 0 ? -height : height));
    ((POLY_FT4 *)prim)->u0 = u0;
    ((POLY_FT4 *)prim)->v0 = v0;
    ((POLY_FT4 *)prim)->u1 = u0 + uSpan;
    ((POLY_FT4 *)prim)->v1 = v0;
    ((POLY_FT4 *)prim)->u2 = u0;
    ((POLY_FT4 *)prim)->v2 = v0 + vSpan;
    ((POLY_FT4 *)prim)->u3 = u0 + uSpan;
    ((POLY_FT4 *)prim)->v3 = v0 + vSpan;
    ((POLY_FT4 *)prim)->clut = clutIndex;
    ((POLY_FT4 *)prim)->tpage = tpage;
    packet = (POLY_FT4 *)prim;
    prim += sizeof(POLY_FT4);
    AddPrim(ot, packet);
    return prim;
}

/* Local alias: retail passes the tpage through as a full word, so the narrow
 * parameter in the shared prototype would insert a truncation. */
void SetDrawModeWide(DrawPacket *pkt, s32 a, s32 b, s32 tpage, void *tw) asm("func_800666F4");

/* Fills a DR_MODE packet in place; unlike GameQueueDrawModePrim it neither
 * links the packet nor advances the scratchpad cursor. No callers in retail. */
void GameSetDrawModePacket(u8 *prim, s32 tpage) {
    SetDrawModeWide((DrawPacket *)prim, 0, 1, tpage, D_8007BED0);
}

/* World position in full-precision components; the camera keeps one of these
 * in the scratchpad at 0x1F800008. */
typedef struct WorldPos {
    s32 vx;
    s32 vy;
    s32 vz;
} WorldPos;

/* The 56-byte scratch block callers hand in: the object-relative offset as a
 * short vector, the rotated result, then the matrix that is handed to the
 * GTE. */
typedef struct ObjectMatrixWork {
    s16 relative[3];
    s16 pad06;
    WorldPos view;
    s32 pad14;
    Matrix mtx;
} ObjectMatrixWork;

/* The per-frame scratchpad block: camera position at +8, view matrix at +0x28. */
#define SCRATCH_CAMERA_POS ((WorldPos *)0x1F800008)
#define SCRATCH_VIEW_MATRIX ((void *)0x1F800028)

/*
 * Per-object GTE setup: takes the object's offset from the camera through the
 * scratchpad view matrix, scales the rotated offset by 4 into the work
 * matrix's translation, and programs the GTE with the caller's rotation and
 * that translation.
 */
void GameSetGteObjectMatrix(void *work, void *objectPos, Matrix *rot) {
    ObjectMatrixWork *w = (ObjectMatrixWork *)work;
    WorldPos *pos = (WorldPos *)objectPos;

    w->relative[0] = pos->vx - SCRATCH_CAMERA_POS->vx;
    w->relative[1] = pos->vy - SCRATCH_CAMERA_POS->vy;
    w->relative[2] = pos->vz - SCRATCH_CAMERA_POS->vz;
    ApplyMatrix(SCRATCH_VIEW_MATRIX, w->relative, &w->view);
    w->mtx.t[0] = w->view.vx * 4;
    w->mtx.t[1] = w->view.vy * 4;
    w->mtx.t[2] = w->view.vz * 4;
    SetRotMatrix(rot);
    SetTransMatrix(&w->mtx);
}
