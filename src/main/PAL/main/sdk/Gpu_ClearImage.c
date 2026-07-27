#include <sys/types.h>

#include "common.h"

extern u_short D_800941EC[];
extern u_short D_800941EE[];
extern u_long D_8009B9B0[];

u_long func_8006764C(long arg0);
void func_80067600(void *arg0);

/* Driver-table slot +0x0C: the worker ClearImage enqueues. Fills the rect
 * with GP0(02h) when both x and w are 64-aligned, otherwise draws a flat
 * rectangle behind a saved draw-mode/area/offset block. */
long Gpu_ClearImage(short *rect, u_long rgb) asm("func_80066E6C");
long Gpu_ClearImage(short *env, u_long rgb) {
    {
        long x = env[2];
        long value;
        if (x >= 0) {
            volatile u_short *p = D_800941EC;
            long mm;
            value = *p;
            value = (short)value;
            mm = value - 1;
            value = mm < x;
            if (value != 0) {
                x = mm;
            }
            value = x;
        } else {
            value = 0;
        }
        env[2] = value;
    }

    {
        long x = env[3];
        if (x >= 0) {
            volatile u_short *p = D_800941EE;
            long mm;
            long c;
            c = *p;
            c = (short)c;
            mm = c - 1;
            c = mm < x;
            if (c != 0) {
                x = mm;
            }
        } else {
            x = 0;
        }
        env[3] = x;
    }

    if ((((u_short *)env)[0] & 0x3F) != 0 || (((u_short *)env)[2] & 0x3F) != 0) {
        u_long *p8 = &D_8009B9B0[8];
        u_long c3 = 0xE3000000;
        u_long c5 = 0xE5000000;
        D_8009B9B0[0] = ((u_long)p8 & 0xFFFFFF) | 0x07000000;
        D_8009B9B0[1] = c3;
        D_8009B9B0[2] = 0xE4FFFFFF;
        D_8009B9B0[3] = c5;
        D_8009B9B0[4] = 0xE6000000;
        D_8009B9B0[5] = (rgb & 0xFFFFFF) | 0x60000000;
        D_8009B9B0[6] = ((u_long *)env)[0];
        D_8009B9B0[7] = ((u_long *)env)[1];
        *p8 = 0x03FFFFFF;
        D_8009B9B0[9] = func_8006764C(3) | c3;
        D_8009B9B0[10] = func_8006764C(4) | 0xE4000000;
        D_8009B9B0[11] = func_8006764C(5) | c5;
    } else {
        D_8009B9B0[0] = 0x04FFFFFF;
        D_8009B9B0[1] = 0xE6000000;
        D_8009B9B0[2] = (rgb & 0xFFFFFF) | 0x02000000;
        D_8009B9B0[3] = ((u_long *)env)[0];
        D_8009B9B0[4] = ((u_long *)env)[1];
    }

    func_80067600(&D_8009B9B0[0]);
    return 0;
}

extern volatile u_long *g_GpuGp0 asm("D_800942B8");
extern volatile u_long *g_GpuGp1 asm("D_800942BC");
extern volatile u_long *D_800942C0;
extern volatile u_long *D_800942C4;
extern volatile u_long *D_800942C8;

typedef struct {
    u_long word0;
    short w;
    short h;
} GpuRect;

void Gpu_ArmTimeout(void) asm("func_80067F04");
long Gpu_CheckTimeout(void) asm("func_80067F38");

/* Driver-table slot +0x20: the worker LoadImage enqueues. Clips the rect,
 * issues GP0(A0h) and pushes the odd words by hand, the rest by DMA2. */
long Gpu_LoadImage(GpuRect *rect, u_long *src) asm("func_80067084");
long Gpu_LoadImage(GpuRect *rect, u_long *src) {
    register GpuRect *savedRect asm("$17");
    register u_long *current asm("$18");
    register long rem asm("$16");
    register long blocks asm("$20");
    register long mode asm("$21");
    register u_long readyMask asm("$19");
    register u_long dmaCommand asm("$4");
    register long w asm("$4");
    register long adjustedWords asm("$2");
    register long halfWords asm("$4");
    register long quotient asm("$3");
    register u_long status asm("$2");
    register u_long gpCommand asm("$3");
    register u_long dmaSize asm("$2");
    register volatile u_long *dmaReg asm("$2");
    register volatile u_long *dmaSizeReg asm("$3");
    short h;
    short clippedW;
    short clippedH;
    volatile long framePadding[2];

    savedRect = rect;
    asm("" : "=r"(savedRect) : "0"(savedRect));
    current = src;
    Gpu_ArmTimeout();

    w = savedRect->w;
    mode = 0;
    clippedW = w;
    if (w >= 0) {
        volatile u_short *width = D_800941EC;

        if ((short)*width < w) {
            clippedW = *width;
        }
    } else {
        clippedW = 0;
    }
    savedRect->w = clippedW;

    h = savedRect->h;
    if (h >= 0) {
        volatile u_short *height = D_800941EE;

        if ((short)*height < h) {
            clippedH = *height;
        } else {
            clippedH = h;
        }
    } else {
        clippedH = 0;
    }
    adjustedWords = savedRect->w * clippedH;
    savedRect->h = clippedH;
    adjustedWords++;
    adjustedWords += (u_long)adjustedWords >> 31;
    halfWords = adjustedWords >> 1;
    if (halfWords <= 0) {
        return -1;
    }
    rem = adjustedWords >> 5;
    asm("" : "=r"(rem) : "0"(rem));
    quotient = rem;
    adjustedWords = quotient << 4;
    rem = halfWords - adjustedWords;
    asm("" : "=r"(rem) : "0"(rem));
    blocks = quotient;

    if ((*g_GpuGp1 & 0x04000000) == 0) {
        readyMask = 0x04000000;
        do {
            if (Gpu_CheckTimeout() != 0) {
                return -1;
            }
            status = *g_GpuGp1;
            status &= readyMask;
        } while (status == 0);
    }

    *g_GpuGp1 = 0x04000000;
    *g_GpuGp0 = 0x01000000;
    *g_GpuGp0 = mode ? 0xB0000000 : 0xA0000000;
    *g_GpuGp0 = savedRect->word0;
    *g_GpuGp0 = *(u_long *)&savedRect->w;

    rem--;
    if (rem != -1) {
        do {
            *g_GpuGp0 = *current;
            current++;
            rem--;
        } while (rem != -1);
    }

    if (blocks != 0) {
        gpCommand = 0x04000000;
        dmaReg = g_GpuGp1;
        gpCommand |= 2;
        *dmaReg = gpCommand;
        dmaReg = D_800942C0;
        dmaCommand = 0x01000000;
        *dmaReg = (u_long)current;
        dmaSize = blocks << 16;
        dmaSizeReg = D_800942C4;
        dmaSize |= 0x10;
        *dmaSizeReg = dmaSize;
        dmaReg = D_800942C8;
        dmaCommand |= 0x201;
        *dmaReg = dmaCommand;
    }
    return 0;
}

extern volatile u_long *D_800942B8;
extern volatile u_long *D_800942BC;

/* Driver-table slot +0x1C: the worker StoreImage enqueues. The GP0(C0h)
 * mirror image of Gpu_LoadImage. */
long Gpu_StoreImage(GpuRect *rect, u_long *dst) asm("func_800672D8");
long Gpu_StoreImage(GpuRect *rect, u_long *dst) {
    short w;
    short h;
    short cw;
    short ch;
    long area;
    long blocks;
    long rem;

    Gpu_ArmTimeout();

    w = rect->w;
    if (w >= 0) {
        volatile u_short *p = D_800941EC;
        if ((short)*p < w) {
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
        volatile u_short *q = D_800941EE;
        if ((short)*q < h) {
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

    if ((*D_800942BC & 0x04000000) == 0) {
        do {
            if (Gpu_CheckTimeout() != 0) {
                return -1;
            }
        } while ((*D_800942BC & 0x04000000) == 0);
    }

    *D_800942BC = 0x04000000;
    *D_800942B8 = 0x01000000;
    *D_800942B8 = 0xC0000000;
    *D_800942B8 = rect->word0;
    *D_800942B8 = *(u_long *)&rect->w;

    if ((*D_800942BC & 0x08000000) == 0) {
        do {
            if (Gpu_CheckTimeout() != 0) {
                return -1;
            }
        } while ((*D_800942BC & 0x08000000) == 0);
    }

    rem--;
    if (rem != -1) {
        do {
            *dst = *D_800942B8;
            rem--;
            dst++;
        } while (rem != -1);
    }

    if (blocks != 0) {
        *D_800942BC = 0x04000003;
        *D_800942C0 = (u_long)dst;
        *D_800942C4 = (blocks << 16) | 0x10;
        *D_800942C8 = 0x01000200;
    }
    return 0;
}
