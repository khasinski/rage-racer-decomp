#include "common.h"

extern u8 g_GraphType asm("D_800941E8");

u32 _get_mode(s32 arg0, s32 arg1, u32 arg2) asm("func_800669F0");

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
