#include "common.h"
#include "psyq/gpu.h"

extern u_char g_GraphType[] asm("D_800941E8");
extern u_short g_VramWidth[] asm("D_800941EC");
extern u_short g_VramHeight[] asm("D_800941EE");

u_long _get_mode(long arg0, long arg1, u_long arg2) asm("func_800669F0");
u_long Gpu_BuildDrawAreaTopLeftCmd(long arg0, long arg1) asm("func_80066A4C");
u_long Gpu_BuildDrawAreaBottomRightCmd(long arg0, long arg1) asm("func_80066B18");
u_long Gpu_BuildDrawOffsetCmd(long arg0, long arg1) asm("func_80066BE4");
u_long Gpu_BuildTexWindowCmd(GpuTexWindow *tw) asm("func_80066C2C");

u_long _get_mode(long arg0, long arg1, u_long arg2) {
    volatile u_char *modep = g_GraphType;
    u_long value;
    u_long cmd;

    value = *modep;
    value = value - 1;
    value = value < 2U;

    if (value != 0) {
        cmd = 0xE1000000;
        if (arg1 != 0) {
            cmd = 0xE1000800;
        }
        value = arg2 & 0x27FF;
        if (arg0 != 0) {
            value |= 0x1000;
        }
    } else {
        cmd = 0xE1000000;
        if (arg1 != 0) {
            cmd = 0xE1000200;
        }
        value = arg2 & 0x9FF;
        if (arg0 != 0) {
            value |= 0x400;
        }
    }

    return cmd | value;
}

u_long Gpu_BuildDrawAreaTopLeftCmd(long arg0, long arg1) {
    long x = arg0;
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

    value = arg1 << 16;
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

u_long Gpu_BuildDrawAreaBottomRightCmd(long arg0, long arg1) {
    long x = arg0;
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

    value = arg1 << 16;
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

u_long Gpu_BuildDrawOffsetCmd(long arg0, long arg1) {
    register u_long x asm("$2");
    register u_long y asm("$3");
    volatile u_char *modep = g_GraphType;

    x = *modep;
    x = x - 1;
    x = x < 2U;
    y = arg1 & 0xFFF;

    if (x == 0) {
        y = arg1 & 0x7FF;
        y <<= 11;
        x = arg0 & 0x7FF;
    } else {
        y <<= 12;
        x = arg0 & 0xFFF;
    }

    x |= 0xE5000000;
    return y | x;
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

extern u_char g_GraphReverse asm("D_800941EB");

u_long get_dx(DispEnv *env) asm("func_80066CB0");

u_long get_dx(DispEnv *env) {
    volatile u_char *modep = g_GraphType;
    register long value asm("$2");
    long mode;

    value = *modep;
    asm("" : "=r"(value) : "0"(value));
    mode = value & 0xFF;

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

extern u_long *g_GpuGp1 asm("D_800942BC");

u_long _status(void) asm("func_80066D6C");
u_long _status(void) {
    return *g_GpuGp1;
}

extern volatile u_long *g_OtcDmaMadr asm("D_800942CC");
extern volatile u_long *g_OtcDmaBcr asm("D_800942D0");
extern volatile u_long *g_OtcDmaChcr asm("D_800942D4");
extern volatile u_long *g_GpuDpcr asm("D_800942D8");

void Gpu_ArmTimeout(void) asm("func_80067F04");
long Gpu_CheckTimeout(void) asm("func_80067F38");

/* Driver-table slot +0x2C: clears an ordering table through the hardware
 * OTC DMA channel (chcr 0x11000002, walking backwards from the end).
 * ClearOTagR is its only caller. */
long Gpu_ClearOTagDma(u_long *ot, long count) asm("func_80066D84");
long Gpu_ClearOTagDma(u_long *arg0, long arg1) {
    long size;
    u_long mask;
    volatile u_long *status;
    long offset;

    size = arg1;
    status = g_GpuDpcr;
    *status |= 0x08000000;
    *g_OtcDmaChcr = 0;
    offset = (size * 4) - 4;
    arg0 = (u_long *)((u_char *)arg0 + offset);
    *g_OtcDmaMadr = (u_long)arg0;
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
