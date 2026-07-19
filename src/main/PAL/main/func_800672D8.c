#include "common.h"

extern u16 D_800941EC[];
extern u16 D_800941EE[];
extern volatile u32 *g_GpuGp0 asm("D_800942B8");
extern volatile u32 *g_GpuGp1 asm("D_800942BC");
extern volatile u32 *D_800942C0;
extern volatile u32 *D_800942C4;
extern volatile u32 *D_800942C8;

typedef struct {
    u32 word0;
    s16 w;
    s16 h;
} GpuRect;

void func_80067F04(void);
s32 func_80067F38(void);

s32 func_800672D8(GpuRect *rect, u32 *dst) {
    s16 w;
    s16 h;
    s16 cw;
    s16 ch;
    s32 area;
    s32 blocks;
    s32 rem;

    func_80067F04();

    w = rect->w;
    if (w >= 0) {
        volatile u16 *p = D_800941EC;
        if ((s16)*p < w) {
            cw = *p;
        } else {
            cw = w;
        }
    } else {
        cw = 0;
    }
    rect->w = cw;

    h = rect->h;
    if (h >= 0) {
        volatile u16 *q = D_800941EE;
        if ((s16)*q < h) {
            ch = *q;
        } else {
            ch = h;
        }
    } else {
        ch = 0;
    }
    rect->h = ch;

    area = (rect->w * ch + 1) / 2;
    if (area <= 0) {
        return -1;
    }
    rem = area % 16;
    blocks = area / 16;

    if ((*g_GpuGp1 & 0x04000000) == 0) {
        do {
            if (func_80067F38() != 0) {
                return -1;
            }
        } while ((*g_GpuGp1 & 0x04000000) == 0);
    }

    *g_GpuGp1 = 0x04000000;
    *g_GpuGp0 = 0x01000000;
    *g_GpuGp0 = 0xC0000000;
    *g_GpuGp0 = rect->word0;
    *g_GpuGp0 = *(u32 *)&rect->w;

    if ((*g_GpuGp1 & 0x08000000) == 0) {
        do {
            if (func_80067F38() != 0) {
                return -1;
            }
        } while ((*g_GpuGp1 & 0x08000000) == 0);
    }

    rem--;
    if (rem != -1) {
        do {
            *dst = *g_GpuGp0;
            rem--;
            dst++;
        } while (rem != -1);
    }

    if (blocks != 0) {
        *g_GpuGp1 = 0x04000003;
        *D_800942C0 = (u32)dst;
        *D_800942C4 = (blocks << 16) | 0x10;
        *D_800942C8 = 0x01000200;
    }
    return 0;
}
