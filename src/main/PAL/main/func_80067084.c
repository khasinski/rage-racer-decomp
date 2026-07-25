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

s32 func_80067084(GpuRect *rect, u32 *src) {
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
    func_80067F04();

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
            if (func_80067F38() != 0) {
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
