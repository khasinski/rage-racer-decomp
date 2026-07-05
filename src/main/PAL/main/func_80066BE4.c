#include "common.h"

extern u8 g_GraphType[] asm("D_800941E8");

u32 Gpu_BuildDrawOffsetCmd(s32 arg0, s32 arg1) asm("func_80066BE4");

u32 Gpu_BuildDrawOffsetCmd(s32 arg0, s32 arg1) {
    register u32 x asm("$2");
    register u32 y asm("$3");
    register volatile u8 *modep asm("$2") = g_GraphType;

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
