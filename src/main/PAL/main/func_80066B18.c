#include "common.h"

extern u16 D_800941EC[];
extern u16 D_800941EE[];
extern u8 g_GraphType[] asm("D_800941E8");

u32 Gpu_BuildDrawAreaBottomRightCmd(s32 arg0, s32 arg1) asm("func_80066B18");

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
        register volatile u8 *modep asm("$2") = g_GraphType;

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
