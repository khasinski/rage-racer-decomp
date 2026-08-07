#include <sys/types.h>

#include "common.h"
#include "psyq/gpu.h"

extern u_short g_VramWidth[];
extern u_short g_VramHeight[];
extern u_long g_ClearImagePacket[];

u_long _param(long arg0);
void Gpu_StartDmaTransfer(void *arg0);

long Gpu_ClearImage(short *env, u_long rgb) {
    {
        long x = env[2];
        long value;
        if (x >= 0) {
            volatile u_short *p = g_VramWidth;
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
            volatile u_short *p = g_VramHeight;
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
        u_long *p8 = &g_ClearImagePacket[8];
        u_long c3 = 0xE3000000;
        u_long c5 = 0xE5000000;
        g_ClearImagePacket[0] = ((u_long)p8 & 0xFFFFFF) | 0x07000000;
        g_ClearImagePacket[1] = c3;
        g_ClearImagePacket[2] = 0xE4FFFFFF;
        g_ClearImagePacket[3] = c5;
        g_ClearImagePacket[4] = 0xE6000000;
        g_ClearImagePacket[5] = (rgb & 0xFFFFFF) | 0x60000000;
        g_ClearImagePacket[6] = ((u_long *)env)[0];
        g_ClearImagePacket[7] = ((u_long *)env)[1];
        *p8 = 0x03FFFFFF;
        g_ClearImagePacket[9] = _param(3) | c3;
        g_ClearImagePacket[10] = _param(4) | 0xE4000000;
        g_ClearImagePacket[11] = _param(5) | c5;
    } else {
        g_ClearImagePacket[0] = 0x04FFFFFF;
        g_ClearImagePacket[1] = 0xE6000000;
        g_ClearImagePacket[2] = (rgb & 0xFFFFFF) | 0x02000000;
        g_ClearImagePacket[3] = ((u_long *)env)[0];
        g_ClearImagePacket[4] = ((u_long *)env)[1];
    }

    Gpu_StartDmaTransfer(&g_ClearImagePacket[0]);
    return 0;
}

extern volatile u_long *g_GpuGp1;

void Gpu_ArmTimeout(void);
long Gpu_CheckTimeout(void);

/* Driver-table slot +0x20: the worker LoadImage enqueues. Clips the rect,
 * issues GP0(A0h) and pushes the odd words by hand, the rest by DMA2. */
long Gpu_LoadImage(GpuRectPacked *rect, u_long *src) {
    GpuRectPacked *savedRect;
    u_long *current;
    long rem;
    long blocks;
    long mode;
    u_long readyMask;
    u_long dmaCommand;
    register long w asm("$4");
    register long adjustedWords asm("$2");
    long halfWords;
    register long quotient asm("$3");
    u_long status;
    u_long gpCommand;
    register volatile u_long *dmaReg asm("$2");
    volatile u_long *dmaSizeReg;
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
        volatile u_short *width = g_VramWidth;

        if ((short)*width < w) {
            clippedW = *width;
        }
    } else {
        clippedW = 0;
    }
    savedRect->w = clippedW;

    h = savedRect->h;
    if (h >= 0) {
        volatile u_short *height = g_VramHeight;

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
    *g_GpuGp0 = savedRect->xy;
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
        dmaReg = g_GpuDmaMadr;
        dmaCommand = 0x01000000;
        *dmaReg = (u_long)current;
        adjustedWords = blocks << 16;
        dmaSizeReg = g_GpuDmaBcr;
        adjustedWords |= 0x10;
        *dmaSizeReg = adjustedWords;
        dmaReg = g_GpuDmaChcr;
        dmaCommand |= 0x201;
        *dmaReg = dmaCommand;
    }
    return 0;
}

extern volatile u_long *g_GpuGp1;

/* Driver-table slot +0x1C: the worker StoreImage enqueues. The GP0(C0h)
 * mirror image of Gpu_LoadImage. */
long Gpu_StoreImage(GpuRectPacked *rect, u_long *dst) {
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
        volatile u_short *p = g_VramWidth;
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
        volatile u_short *q = g_VramHeight;
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

    if ((*g_GpuGp1 & 0x04000000) == 0) {
        do {
            if (Gpu_CheckTimeout() != 0) {
                return -1;
            }
        } while ((*g_GpuGp1 & 0x04000000) == 0);
    }

    *g_GpuGp1 = 0x04000000;
    *g_GpuGp0 = 0x01000000;
    *g_GpuGp0 = 0xC0000000;
    *g_GpuGp0 = rect->xy;
    *g_GpuGp0 = *(u_long *)&rect->w;

    if ((*g_GpuGp1 & 0x08000000) == 0) {
        do {
            if (Gpu_CheckTimeout() != 0) {
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
        *g_GpuDmaMadr = (u_long)dst;
        *g_GpuDmaBcr = (blocks << 16) | 0x10;
        *g_GpuDmaChcr = 0x01000200;
    }
    return 0;
}
