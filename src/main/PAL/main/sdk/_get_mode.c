#include "common.h"
#include "psyq/gpu.h"

typedef unsigned int u_long;

extern u8 g_GraphType asm("D_800941E8");
extern u8 g_GraphTypeArray[] asm("D_800941E8");
extern u16 D_800941EC[];
extern u16 D_800941EE[];

u32 _get_mode(s32 arg0, s32 arg1, u32 arg2) asm("func_800669F0");
u32 Gpu_BuildDrawAreaTopLeftCmd(s32 arg0, s32 arg1) asm("func_80066A4C");
u32 Gpu_BuildDrawAreaBottomRightCmd(s32 arg0, s32 arg1) asm("func_80066B18");
u32 Gpu_BuildDrawOffsetCmd(s32 arg0, s32 arg1) asm("func_80066BE4");
u_long Gpu_BuildTexWindowCmd(GpuTexWindow *tw) asm("func_80066C2C");

u32 _get_mode(s32 arg0, s32 arg1, u32 arg2) {
    register volatile u8 *modep asm("$2") = &g_GraphType;
    register u32 value asm("$2");
    register u32 cmd asm("$3");

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

u32 Gpu_BuildDrawAreaTopLeftCmd(s32 arg0, s32 arg1) {
    register s32 x asm("$4") = arg0;
    register s32 y asm("$3");
    register s32 outY asm("$5");
    register u32 value asm("$2");
    register u32 shiftedY asm("$3");

    x = (s16)x;
    if (x < 0) {
        value = 0;
    } else {
        register volatile u16 *widthp asm("$2") = D_800941EC;
        register s32 maxX asm("$6");

        value = *widthp;
        value = (s16)value;
        maxX = value - 1;
        value = maxX < x;
        if (value != 0) {
            x = maxX;
        }
        value = x;
    }
    x = value;

    value = arg1 << 16;
    y = (s32)value >> 16;
    outY = 0;
    if (y >= 0) {
        register volatile u16 *heightp asm("$2") = D_800941EE;
        register s32 maxY asm("$5");

        value = *heightp;
        value = (s16)value;
        maxY = value - 1;
        value = maxY < y;
        if (value != 0) {
            y = maxY;
        }
        outY = y;
    }

    {
        register volatile u8 *modep asm("$2") = g_GraphTypeArray;

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

u32 Gpu_BuildDrawAreaBottomRightCmd(s32 arg0, s32 arg1) {
    register s32 x asm("$4") = arg0;
    register s32 y asm("$3");
    register s32 outY asm("$5");
    register u32 value asm("$2");
    register u32 shiftedY asm("$3");

    x = (s16)x;
    if (x < 0) {
        value = 0;
    } else {
        register volatile u16 *widthp asm("$2") = D_800941EC;
        register s32 maxX asm("$6");

        value = *widthp;
        value = (s16)value;
        maxX = value - 1;
        value = maxX < x;
        if (value != 0) {
            x = maxX;
        }
        value = x;
    }
    x = value;

    value = arg1 << 16;
    y = (s32)value >> 16;
    outY = 0;
    if (y >= 0) {
        register volatile u16 *heightp asm("$2") = D_800941EE;
        register s32 maxY asm("$5");

        value = *heightp;
        value = (s16)value;
        maxY = value - 1;
        value = maxY < y;
        if (value != 0) {
            y = maxY;
        }
        outY = y;
    }

    {
        register volatile u8 *modep asm("$2") = g_GraphTypeArray;

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

u32 Gpu_BuildDrawOffsetCmd(s32 arg0, s32 arg1) {
    register u32 x asm("$2");
    register u32 y asm("$3");
    register volatile u8 *modep asm("$2") = g_GraphTypeArray;

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

extern u8 g_GraphReverse asm("D_800941EB");

u_long get_dx(DispEnv *env) asm("func_80066CB0");

u_long get_dx(DispEnv *env) {
    register volatile u8 *modep asm("$2") = &g_GraphType;
    register s32 value asm("$2");
    register s32 mode asm("$3");

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

extern u32 *D_800942BC;

u32 _status(void) asm("func_80066D6C");
u32 _status(void) {
    return *D_800942BC;
}

extern volatile u32 *D_800942CC;
extern volatile u32 *D_800942D0;
extern volatile u32 *D_800942D4;
extern volatile u32 *D_800942D8;

void func_80067F04(void);
s32 func_80067F38(void);

/* Driver-table slot +0x2C: clears an ordering table through the hardware
 * OTC DMA channel (chcr 0x11000002, walking backwards from the end).
 * ClearOTagR is its only caller. */
s32 Gpu_ClearOTagDma(u32 *ot, s32 count) asm("func_80066D84");
s32 Gpu_ClearOTagDma(u32 *arg0, s32 arg1) {
    s32 size;
    u32 mask;
    volatile u32 *status;
    s32 offset;

    size = arg1;
    status = D_800942D8;
    *status |= 0x08000000;
    *D_800942D4 = 0;
    offset = (size << 2) - 4;
    arg0 = (u32 *)((u8 *)arg0 + offset);
    *D_800942CC = (u32)arg0;
    *D_800942D0 = size;
    *D_800942D4 = 0x11000002;
    func_80067F04();

    if ((*D_800942D4 & 0x01000000) != 0) {
        mask = 0x01000000;
        while (1) {
            u32 statusValue;

            if (func_80067F38() != 0) {
                return -1;
            }
            statusValue = *D_800942D4;
            statusValue &= mask;
            if (statusValue == 0) {
                break;
            }
        }
    }
    return size;
}
