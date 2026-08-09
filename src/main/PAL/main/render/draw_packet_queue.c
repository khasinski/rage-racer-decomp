#include "common.h"
#include "game/render_internal.h"
#include "game/prim.h"
#include "game/render.h"
#include "game/scratchpad.h"
#include "game/vector.h"
#include "psyq/gpu.h"
#include "psyq/gte.h"


/* DR_MODE, 12 bytes: sets the texture page (and the blend mode packed into it)
 * for the primitives that follow, links it into the ordering table and returns
 * the advanced packet cursor. */
u8 *QueueDrawModePrim(void *ot, u8 *prim, s32 tpage) {
    u8 *pkt;

    SetDrawMode((DrawPacket *)prim, 0, 1, (u16)tpage, g_DrawModeEnv);
    pkt = prim;
    prim = (void *)((DrawPacket *)prim + 1);
    AddPrim(ot, pkt);
    return prim;
}

u8 *GameQueueShadedTexturedRect(ot, prim, x, y, w, h, u, v, clutIndex, tpage, intensity)
    void *ot;
    u8 *prim;
    s16 x;
    s16 y;
    s16 w;
    s16 h;
    u8 u;
    u8 v;
    u16 clutIndex;
    u16 tpage;
    u8 intensity;
{
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

u8 *GameQueueTexturedRect(ot, prim, x, y, w, h, u, v, uSpan, vSpan, clutIndex, tpage)
    void *ot;
    u8 *prim;
    s16 x;
    s16 y;
    s16 w;
    s16 h;
    u8 u;
    u8 v;
    u8 uSpan;
    u8 vSpan;
    u16 clutIndex;
    u16 tpage;
{
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

/* Fills a DR_MODE packet in place; unlike QueueDrawModePrim it neither
 * links the packet nor advances the scratchpad cursor. No callers in retail. */
void SetDrawModePacket(u8 *prim, s32 tpage) {
    SetDrawMode((DrawPacket *)prim, 0, 1, tpage, g_DrawModeEnv);
}

/* World position in full-precision components; the camera keeps one of these
 * in the scratchpad at 0x1F800008. */
/* The 56-byte scratch block callers hand in: the object-relative offset as a
 * short vector, the rotated result, then the matrix that is handed to the
 * GTE. */
typedef struct ObjectMatrixWork {
    s16 relative[3];
    s16 pad06;
    LVec view;
    s32 pad14;
    Matrix mtx;
} ObjectMatrixWork;

/* The per-frame scratchpad block: camera position at +8, view matrix at +0x28. */
#define SCRATCH_CAMERA_POS ((LVec *)&SCRATCH_VIEW_X)
#define SCRATCH_VIEW_MATRIX (SCRATCH_VIEW_MATRIX_GTE)

/*
 * Per-object GTE setup: takes the object's offset from the camera through the
 * scratchpad view matrix, scales the rotated offset by 4 into the work
 * matrix's translation, and programs the GTE with the caller's rotation and
 * that translation.
 */
void SetGteObjectMatrix(void *work, void *objectPos, Matrix *rot) {
    ObjectMatrixWork *w = (ObjectMatrixWork *)work;
    LVec *pos = (LVec *)objectPos;

    w->relative[0] = pos->x - SCRATCH_CAMERA_POS->x;
    w->relative[1] = pos->y - SCRATCH_CAMERA_POS->y;
    w->relative[2] = pos->z - SCRATCH_CAMERA_POS->z;
    ApplyMatrix(SCRATCH_VIEW_MATRIX, w->relative, &w->view);
    w->mtx.t[0] = w->view.x * 4;
    w->mtx.t[1] = w->view.y * 4;
    w->mtx.t[2] = w->view.z * 4;
    SetRotMatrix(rot);
    SetTransMatrix(&w->mtx);
}
