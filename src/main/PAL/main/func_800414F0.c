#include "common.h"

typedef struct {
    s32 f0;
    s32 f4;
    s32 f8;
    s32 fC;
    s32 f10;
    s32 f14;
} Scr;

typedef struct {
    s32 f0;
    s32 f4;
    s32 f8;
    s32 f12;
} Out;

extern s32 *g_VisibleCellMask asm("D_801E6828");
extern u16 *D_801E5020;
extern Out *g_VisibleCellList asm("D_801E4BC8");
extern s8 D_8007E45C[];
extern s8 D_8007E45D[];

u32 func_800414A0(s32 arg0, s32 arg1);
u32 func_800414C4(s32 arg0, s32 arg1, s32 arg2);
void *func_80068F80(void *mtx, void *vec, void *out);

void func_800414F0(s32 arg0, s32 arg1) {
    Scr *s = (Scr *)0x1F800008;
    s32 i;
    s32 j;
    s32 oct;
    s32 cx, cy;
    u32 ret0;
    Out *out;
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
    ret0 = func_800414A0(cx, cy);

    i = 0;
    out = g_VisibleCellList;
    j = 0;
    while (i < 64) {
        s32 k;
        s32 dx;
        s32 dy;
        s32 invalid = -1;

        switch (oct / 8) {
        case 0:
            k = j + (oct << 7);
            dx = D_8007E45C[k];
            dy = D_8007E45D[k];
            sx = cx + dx;
            sy = cy + dy;
            break;
        case 1:
            k = j + ((8 - (oct % 8)) << 7);
            dx = D_8007E45C[k];
            dy = D_8007E45D[k];
            sx = cx + dx;
            sy = cy - dy;
            break;
        case 2:
            k = j + ((oct - 16) << 7);
            dx = D_8007E45C[k];
            dy = D_8007E45D[k];
            sx = cx - dx;
            sy = cy - dy;
            break;
        case 3:
            k = oct % 8;
            k = 8 - k;
            k = j + (k << 7);
            dx = D_8007E45C[k];
            dy = D_8007E45D[k];
            sx = cx - dx;
            sy = cy + dy;
            break;
        }

        if (sx < 32U && sy < 32U && func_800414C4(sx, sy, ret0)) {
            s32 clut = D_801E5020[((31 - sy) << 5) + sx] & 0x3FF;

            out->f12 = clut;
            g_VisibleCellMask[sy] |= 1 << sx;
            center = 1024;
            if (clut != 0x3FF) {
                vec[0] = ((sx << 11) - (s->f0 - center)) << 2;
                vec[1] = (-s->f4) << 2;
                vec[2] = ((sy << 11) - (s->f8 - center)) << 2;
                func_80068F80((void *)0x1F800028, vec, proj);
                if (proj[2] >= arg0 && arg1 >= proj[2]) {
                    out->f0 = proj[0];
                    do {
                        do {
                            do {
                                do {
                                    out->f4 = proj[1];
                                } while (0);
                            } while (0);
                        } while (0);
                    } while (0);
                    out->f8 = proj[2];
                    goto cont;
                }
            }
        }
        out->f12 = invalid;
cont:
        j += 2;
        i++;
        out++;
    }
}
