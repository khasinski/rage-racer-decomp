#include "common.h"
#include "psyq/gte.h"

typedef struct {
    s16 x;
    s16 y;
    s16 z;
    s16 pad;
} SVec;

typedef struct {
    u16 x;
    u16 y;
    u16 z;
    u16 pad;
} UVec;

extern SVec D_80011A48[];
extern s32 D_8007FB48;

void func_8001A530(Matrix *mtx, s32 angle);
s16 *func_800696C8(s32 *matrix, void *vec, s16 *out);
void func_80046E00();

void func_800506BC(s32 *p0, s32 *p1, s32 *p2) {
    SVec verts[4];
    UVec out[4];
    Matrix mtx;
    s32 scratch2;
    s32 n;
    s32 v;
    s32 depth;

    verts[0] = D_80011A48[0];
    verts[1] = D_80011A48[1];
    verts[2] = D_80011A48[2];
    verts[3] = D_80011A48[3];

    scratch2 = *(s32 *)0x1F800004 + 4;

    n = *p0 - *p1;
    if (n != 0) {
        if (n > 0)
            n = (n + 12) / 12;
        else
            n = (n - 12) / 12;
    }
    *p1 = *p1 + n;
    n = *p1 / 1000;
    if (n < 11) {
        n = 11;
    }
    if (n < 1024) {
        v = *p2;
        if (v >= 0) {
            D_8007FB48 = v;
            *p2 = -1;
        }
    }

    switch (D_8007FB48) {
    case 1:
        depth = 0x1F8;
        break;
    case 2:
        depth = 0x20B;
        break;
    case 3:
        depth = 0x1F9;
        break;
    default:
        return;
    }

    func_8001A530(&mtx, n);
    func_800696C8((s32 *)&mtx, &verts[0], (s16 *)&out[0]);
    func_800696C8((s32 *)&mtx, &verts[1], (s16 *)&out[1]);
    func_800696C8((s32 *)&mtx, &verts[2], (s16 *)&out[2]);
    func_800696C8((s32 *)&mtx, &verts[3], (s16 *)&out[3]);

    {
        register s32 x0 asm("$5");
        register s32 y0 asm("$6");
        register s32 x1 asm("$7");
        register s32 y1 asm("$3");
        register s32 x2 asm("$8");
        register s32 y2 asm("$9");
        register s32 x3 asm("$10");
        register s32 y3 asm("$11");

        x0 = out[0].x;
        x1 = out[1].x;
        x2 = out[2].x;
        x3 = out[3].x;
        y0 = out[0].y;
        y1 = out[1].y;
        y2 = out[2].y;
        y3 = out[3].y;

        x0 = (s16)(x0 + 0xE4);
        y0 = (s16)(y0 + 0x58);
        y1 = (s16)(y1 + 0x58);
        x2 = (s16)(x2 + 0xE4);
        y2 = (s16)(y2 + 0x58);
        x3 = (s16)(x3 + 0xE4);
        y3 = (s16)(y3 + 0x58);
        x1 = (s16)(x1 + 0xE4);

        func_80046E00(scratch2,
            x0, y0, x1, y1, x2, y2, x3, y3,
            0xA0, 0x70, 0xDF, 0x70, 0xA0, 0xBF, 0xDF, 0xBF,
            0x7F, 0x7F, 0x7F,
            depth,
            0, 0,
            0x1C);
    }
}
