#include "common.h"

extern u16 D_800941EC[];
extern u16 D_800941EE[];
extern u32 D_8009B9B0[];

u32 func_8006764C(s32 arg0);
void func_80067600(void *arg0);

/* Driver-table slot +0x0C: the worker ClearImage enqueues. Fills the rect
 * with GP0(02h) when both x and w are 64-aligned, otherwise draws a flat
 * rectangle behind a saved draw-mode/area/offset block. */
s32 Gpu_ClearImage(s16 *rect, u32 rgb) asm("func_80066E6C");
s32 Gpu_ClearImage(s16 *env, u32 rgb) {
    {
        s32 x = env[2];
        s32 value;
        if (x >= 0) {
            volatile u16 *p = D_800941EC;
            s32 mm;
            value = *p;
            value = (s16)value;
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
        s32 x = env[3];
        if (x >= 0) {
            volatile u16 *p = D_800941EE;
            s32 mm;
            s32 c;
            c = *p;
            c = (s16)c;
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

    if ((((u16 *)env)[0] & 0x3F) != 0 || (((u16 *)env)[2] & 0x3F) != 0) {
        u32 *p8 = &D_8009B9B0[8];
        u32 c3 = 0xE3000000;
        u32 c5 = 0xE5000000;
        D_8009B9B0[0] = ((u32)p8 & 0xFFFFFF) | 0x07000000;
        D_8009B9B0[1] = c3;
        D_8009B9B0[2] = 0xE4FFFFFF;
        D_8009B9B0[3] = c5;
        D_8009B9B0[4] = 0xE6000000;
        D_8009B9B0[5] = (rgb & 0xFFFFFF) | 0x60000000;
        D_8009B9B0[6] = ((u32 *)env)[0];
        D_8009B9B0[7] = ((u32 *)env)[1];
        *p8 = 0x03FFFFFF;
        D_8009B9B0[9] = func_8006764C(3) | c3;
        D_8009B9B0[10] = func_8006764C(4) | 0xE4000000;
        D_8009B9B0[11] = func_8006764C(5) | c5;
    } else {
        D_8009B9B0[0] = 0x04FFFFFF;
        D_8009B9B0[1] = 0xE6000000;
        D_8009B9B0[2] = (rgb & 0xFFFFFF) | 0x02000000;
        D_8009B9B0[3] = ((u32 *)env)[0];
        D_8009B9B0[4] = ((u32 *)env)[1];
    }

    func_80067600(&D_8009B9B0[0]);
    return 0;
}

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

void Gpu_ArmTimeout(void) asm("func_80067F04");
s32 Gpu_CheckTimeout(void) asm("func_80067F38");

/* Driver-table slot +0x20: the worker LoadImage enqueues. Clips the rect,
 * issues GP0(A0h) and pushes the odd words by hand, the rest by DMA2. */
s32 Gpu_LoadImage(GpuRect *rect, u32 *src) asm("func_80067084");
s32 Gpu_LoadImage(GpuRect *rect, u32 *src) {
    register GpuRect *savedRect asm("$17");
    register u32 *current asm("$18");
    register s32 rem asm("$16");
    register s32 blocks asm("$20");
    register s32 mode asm("$21");
    register u32 readyMask asm("$19");
    register u32 dmaCommand asm("$4");
    register s32 w asm("$4");
    register s32 adjustedWords asm("$2");
    register s32 halfWords asm("$4");
    register s32 quotient asm("$3");
    register u32 status asm("$2");
    register u32 gpCommand asm("$3");
    register u32 dmaSize asm("$2");
    register volatile u32 *dmaReg asm("$2");
    register volatile u32 *dmaSizeReg asm("$3");
    s16 h;
    s16 clippedW;
    s16 clippedH;
    volatile s32 framePadding[2];

    savedRect = rect;
    asm("" : "=r"(savedRect) : "0"(savedRect));
    current = src;
    Gpu_ArmTimeout();

    w = savedRect->w;
    mode = 0;
    clippedW = w;
    if (w >= 0) {
        volatile u16 *width = D_800941EC;

        if ((s16)*width < w) {
            clippedW = *width;
        }
    } else {
        clippedW = 0;
    }
    savedRect->w = clippedW;

    h = savedRect->h;
    if (h >= 0) {
        volatile u16 *height = D_800941EE;

        if ((s16)*height < h) {
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
    adjustedWords += (u32)adjustedWords >> 31;
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
    *g_GpuGp0 = *(u32 *)&savedRect->w;

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
        *dmaReg = (u32)current;
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

extern volatile u32 *D_800942B8;
extern volatile u32 *D_800942BC;

/* Driver-table slot +0x1C: the worker StoreImage enqueues. The GP0(C0h)
 * mirror image of Gpu_LoadImage. */
s32 Gpu_StoreImage(GpuRect *rect, u32 *dst) asm("func_800672D8");
s32 Gpu_StoreImage(GpuRect *rect, u32 *dst) {
    s16 w;
    s16 h;
    s16 cw;
    s16 ch;
    s32 area;
    s32 blocks;
    s32 rem;

    Gpu_ArmTimeout();

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
    *D_800942B8 = *(u32 *)&rect->w;

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
        *D_800942C0 = (u32)dst;
        *D_800942C4 = (blocks << 16) | 0x10;
        *D_800942C8 = 0x01000200;
    }
    return 0;
}
