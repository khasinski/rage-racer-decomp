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
