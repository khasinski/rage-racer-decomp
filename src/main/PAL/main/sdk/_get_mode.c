#include "common.h"
#include "psyq/gpu.h"

extern u_char g_GraphType[];
extern u_short g_VramWidth[];
extern u_short g_VramHeight[];

u_long _get_mode(long arg0, long arg1, u_long arg2);
u_long Gpu_BuildDrawAreaTopLeftCmd(long x, long y);
u_long Gpu_BuildDrawAreaBottomRightCmd(long arg0, long arg1);
u_long Gpu_BuildDrawOffsetCmd(long x, long y);
u_long Gpu_BuildTexWindowCmd(GpuTexWindow *tw);

u_long _get_mode(long dfe, long dtd, u_long tpage) {
    volatile u_char *modep = g_GraphType;
    u_long value;
    u_long cmd;

    value = *modep;
    value = value - 1;
    value = value < 2U;

    if (value != 0) {
        cmd = 0xE1000000;
        if (dtd != 0) {
            cmd = 0xE1000800;
        }
        value = tpage & 0x27FF;
        if (dfe != 0) {
            value |= 0x1000;
        }
    } else {
        cmd = 0xE1000000;
        if (dtd != 0) {
            cmd = 0xE1000200;
        }
        value = tpage & 0x9FF;
        if (dfe != 0) {
            value |= 0x400;
        }
    }

    return cmd | value;
}

u_long Gpu_BuildDrawAreaTopLeftCmd(long left, long top) {
    long x = left;
    long y;
    long outY;
    register u_long value asm("$2");
    register u_long shiftedY asm("$3");

    x = (short)x;
    if (x < 0) {
        value = 0;
    } else {
        volatile u_short *widthp = g_VramWidth;
        register long maxX asm("$6");

        value = *widthp;
        value = (short)value;
        maxX = value - 1;
        value = maxX < x;
        if (value != 0) {
            x = maxX;
        }
        value = x;
    }
    x = value;

    value = top << 16;
    y = (long)value >> 16;
    outY = 0;
    if (y >= 0) {
        volatile u_short *heightp = g_VramHeight;
        long maxY;

        value = *heightp;
        value = (short)value;
        maxY = value - 1;
        value = maxY < y;
        if (value != 0) {
            y = maxY;
        }
        outY = y;
    }

    {
        volatile u_char *modep = g_GraphType;

        value = *modep;
    }
    value = value - 1;
    value = value < 2U;
    shiftedY = outY & 0xFFF;

    if (value == 0) {
        shiftedY = outY & 0x3FF;
        shiftedY <<= 10;
        value = x & 0x3FF;
    } else {
        shiftedY <<= 12;
        value = x & 0xFFF;
    }

    value |= 0xE3000000;
    return shiftedY | value;
}

u_long Gpu_BuildDrawAreaBottomRightCmd(long right, long bottom) {
    long x = right;
    long y;
    long outY;
    register u_long value asm("$2");
    register u_long shiftedY asm("$3");

    x = (short)x;
    if (x < 0) {
        value = 0;
    } else {
        volatile u_short *widthp = g_VramWidth;
        register long maxX asm("$6");

        value = *widthp;
        value = (short)value;
        maxX = value - 1;
        value = maxX < x;
        if (value != 0) {
            x = maxX;
        }
        value = x;
    }
    x = value;

    value = bottom << 16;
    y = (long)value >> 16;
    outY = 0;
    if (y >= 0) {
        volatile u_short *heightp = g_VramHeight;
        long maxY;

        value = *heightp;
        value = (short)value;
        maxY = value - 1;
        value = maxY < y;
        if (value != 0) {
            y = maxY;
        }
        outY = y;
    }

    {
        volatile u_char *modep = g_GraphType;

        value = *modep;
    }
    value = value - 1;
    value = value < 2U;
    shiftedY = outY & 0xFFF;

    if (value == 0) {
        shiftedY = outY & 0x3FF;
        shiftedY <<= 10;
        value = x & 0x3FF;
    } else {
        shiftedY <<= 12;
        value = x & 0xFFF;
    }

    value |= 0xE4000000;
    return shiftedY | value;
}

u_long Gpu_BuildDrawOffsetCmd(long x, long y) {
    volatile u_char *modep = g_GraphType;
    u_long mode;

    mode = *modep;
    mode = mode - 1;
    mode = mode < 2U;
    if (mode != 0) {
        return 0xE5000000 | ((y & 0xFFF) << 12) | (x & 0xFFF);
    }
    return 0xE5000000 | ((y & 0x7FF) << 11) | (x & 0x7FF);
}

u_long Gpu_BuildTexWindowCmd(GpuTexWindow *tw) {
    u_long pad[4];

    if (tw != 0) {
        pad[0] = (tw->x & 0xFF) >> 3;
        pad[2] = (-tw->w & 0xFF) >> 3;
        pad[1] = (tw->y & 0xFF) >> 3;
        pad[3] = (-tw->h & 0xFF) >> 3;
        return 0xE2000000 | (pad[1] << 0xF) | (pad[0] << 0xA) |
               (pad[3] << 5) | pad[2];
    }

    return 0;
}

extern u_char g_GraphReverse;

u_long get_dx(DispEnv *env);

u_long get_dx(DispEnv *env) {
    volatile u_char *modep = g_GraphType;
    long mode;

    mode = *modep & 0xFF;

    switch (mode) {
    case 1:
        return g_GraphReverse ? 0x400 - env->disp.x - env->disp.w : env->disp.x;
    case 2:
        return g_GraphReverse ? 0x400 - env->disp.x - (env->disp.w / 2)
                          : env->disp.x / 2;
    default:
        return env->disp.x;
    }
}

extern u_long *g_GpuGp1;

u_long _status(void) {
    return *g_GpuGp1;
}

extern volatile u_long *g_OtcDmaMadr;
extern volatile u_long *g_OtcDmaBcr;
extern volatile u_long *g_OtcDmaChcr;

void Gpu_ArmTimeout(void);
long Gpu_CheckTimeout(void);

long Gpu_ClearOTagDma(u_long *ot, long count) {
    long size;
    u_long mask;
    volatile u_long *status;
    long offset;

    size = count;
    status = g_GpuDpcr;
    *status |= 0x08000000;
    *g_OtcDmaChcr = 0;
    offset = (size * 4) - 4;
    ot = (u_long *)((u_char *)ot + offset);
    *g_OtcDmaMadr = (u_long)ot;
    *g_OtcDmaBcr = size;
    *g_OtcDmaChcr = 0x11000002;
    Gpu_ArmTimeout();

    if ((*g_OtcDmaChcr & 0x01000000) != 0) {
        mask = 0x01000000;
        while (1) {
            u_long statusValue;

            if (Gpu_CheckTimeout() != 0) {
                return -1;
            }
            statusValue = *g_OtcDmaChcr;
            statusValue &= mask;
            if (statusValue == 0) {
                break;
            }
        }
    }
    return size;
}
