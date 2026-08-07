#include "common.h"
#include "game/vector.h"
#include "psyq/gte.h"
#include "game/state.h"
#include "game/render.h"
#include "game/track.h"

typedef struct Obj {
    s16 id;    /* 0x0 */
    s16 f2;    /* 0x2 */
    s32 f4;    /* 0x4 */
    s32 f8;    /* 0x8 */
    s32 fC;    /* 0xC */
    s32 flags; /* 0x10 */
} Obj;

extern Obj *g_CourseObjects;
extern s32 g_CourseObjectCount;
extern s32 *g_VisibleCellMask;

/*
 * Draw loop over the world-object array g_CourseObjects (g_CourseObjectCount entries). For
 * each visible object (id != -1, passing the per-sector visibility bitmask test
 * against g_VisibleCellMask) it builds a Z-rotation matrix in the scratchpad
 * (0x1F800028), transforms the object position through the GTE
 * (0x1F80011C -> 0x1F800124), sets the primitive shade/semi-trans mode word at
 * 0x1F800084, then dispatches a prim builder (SubmitCourseModel2 / SubmitCourseModel)
 * on the scratchpad OT at 0x1F800000.
 */
void DrawCourseObjects(void) {
    Matrix mtx;
    volatile s32 pad[10];
    Obj *obj;
    s32 i;
    s32 visShift;
    s32 vis;
    s32 flags;

    obj = g_CourseObjects;
    i = 0;
    if (g_CourseObjectCount <= 0) {
        return;
    }

    do {
        if (obj->id == -1) {
        } else {
        visShift = obj->f4 / 2048;  /* per-sector visibility bit index */
        vis = g_VisibleCellMask[obj->fC / 2048] & (1 << visShift);
        if (vis == 0) {
            continue;
        }

        BuildRotMatrixY(&mtx, obj->f2);
        MulMatrix2((void *)0x1F800028, &mtx);
        {
            s32 ov;
            s32 cv;
            ov = (u16)obj->f4;
            cv = *(u16 *)0x1F800008;
            ov -= cv;
            *(s16 *)0x1F80011C = ov;
            ov = (u16)obj->f8;
            cv = *(u16 *)0x1F80000C;
            ov -= cv;
            *(s16 *)0x1F80011E = ov;
            ov = (u16)obj->fC;
            cv = *(u16 *)0x1F800010;
            ov -= cv;
            *(s16 *)0x1F800120 = ov;
        }
        ApplyMatrix((void *)0x1F800028, (void *)0x1F80011C, (void *)0x1F800124);
        {
            s32 a;
            register s32 b asm("$3");
            a = *(s32 *)0x1F800124;
            b = *(s32 *)0x1F80012C;
            a <<= 2;
            *(s32 *)0x1F800148 = a;
            a = *(s32 *)0x1F800128;
            b <<= 2;
            *(s32 *)0x1F800150 = b;
            a <<= 2;
            *(s32 *)0x1F80014C = a;
        }
        SetRotMatrix(&mtx);
        SetTransMatrix((void *)0x1F800134);

        flags = obj->flags;
        if (flags & 8) {
            *(s32 *)0x1F800084 = ((g_AnimTimer & 0x10) == 0) << 16;
        } else if (flags & 4) {
            *(s32 *)0x1F800084 = 0x10000;
        } else {
            *(s32 *)0x1F800084 = 0;
        }

        if (g_IsEnvironmentMode4 ? (obj->flags & 2) : (obj->flags % 2)) {
            SubmitCourseModel2((void *)0x1F800000, obj->id);
        } else {
            SubmitCourseModel((void *)0x1F800000, obj->id);
        }

        }
    } while (i++, obj++, i < g_CourseObjectCount);
}


u32 GetCellRegion(s32 arg0, s32 arg1) {
    arg1 = (arg1 * 32) + arg0;
    return g_TerrainCellGrid[arg1] >> 10;
}


u32 IsCellVisibleFromRegion(s32 arg0, s32 arg1, s32 arg2) {
    s32 x = arg0 << 2;
    s32 y;
    u8 *base;
    u32 mask;

    y = arg1 << 7;
    base = g_CellVisibilityTable;
    mask = 1;
    y += (s32)base;
    x += y;
    return (mask << arg2) & *(u32 *)x;
}

typedef struct Scr {
    s32 f0;
    s32 f4;
    s32 f8;
    s32 fC;
    s32 f10;
    s32 f14;
} Scr;

extern Vec4 *g_VisibleCellList;
void *ApplyMatrixLV(void *mtx, void *vec, void *out);

void BuildVisibleCells(s32 arg0, s32 arg1) {
    Scr *s = (Scr *)0x1F800008;
    s32 i;
    s32 j;
    s32 oct;
    s32 cx, cy;
    u32 ret0;
    Vec4 *out;
    s32 sx;
    s32 sy;
    s32 center;
    s32 vec[3];
    s32 proj[3];

    for (i = 31; i >= 0; i--) {
        g_VisibleCellMask[i] = 0;
    }

    oct = (s->f14 / 128) & 0x1F;
    cx = s->f0 / 2048;
    cy = s->f8 / 2048;
    ret0 = GetCellRegion(cx, cy);

    i = 0;
    out = g_VisibleCellList;
    j = 0;
    for (; i < 64; j += 2, i++, out++) {
        s32 k;
        s32 dx;
        s32 dy;
        s32 invalid = -1;

        switch (oct / 8) {
        case 0:
            k = j + (oct << 7);
            dx = g_CellScanOffsetX[k];
            dy = g_CellScanOffsetY[k];
            sx = cx + dx;
            sy = cy + dy;
            break;
        case 1:
            k = j + ((8 - (oct % 8)) << 7);
            dx = g_CellScanOffsetX[k];
            dy = g_CellScanOffsetY[k];
            sx = cx + dx;
            sy = cy - dy;
            break;
        case 2:
            k = j + ((oct - 16) << 7);
            dx = g_CellScanOffsetX[k];
            dy = g_CellScanOffsetY[k];
            sx = cx - dx;
            sy = cy - dy;
            break;
        case 3:
            k = oct % 8;
            k = 8 - k;
            k = j + (k << 7);
            dx = g_CellScanOffsetX[k];
            dy = g_CellScanOffsetY[k];
            sx = cx - dx;
            sy = cy + dy;
            break;
        }

        if (sx < 32U && sy < 32U && IsCellVisibleFromRegion(sx, sy, ret0)) {
            s32 clut = g_TerrainCellGrid[((31 - sy) << 5) + sx] & 0x3FF;

            out->w = clut;
            g_VisibleCellMask[sy] |= 1 << sx;
            center = 1024;
            if (clut != 0x3FF) {
                vec[0] = ((sx << 11) - (s->f0 - center)) << 2;
                vec[1] = (-s->f4) << 2;
                vec[2] = ((sy << 11) - (s->f8 - center)) << 2;
                ApplyMatrixLV((void *)0x1F800028, vec, proj);
                if (proj[2] >= arg0 && arg1 >= proj[2]) {
                    out->x = proj[0];
                    out->y = proj[1];
                    out->z = proj[2];
                    continue;
                }
                out->w = invalid;
                continue;
            }
            out->w = invalid;
            continue;
        }
        out->w = invalid;
    }
}
