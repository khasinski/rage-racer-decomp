#include "common.h"

u32 _get_mode(s32 arg0, s32 arg1, u32 arg2) asm("func_800669F0");
u32 Gpu_BuildTexWindowCmd(void *arg0) asm("func_80066C2C");

void Gpu_InitDrawModeTexWindowPacket(u8 *arg0, s32 arg1, s32 arg2, u16 arg3, void *arg4) asm("func_800666F4");

void Gpu_InitDrawModeTexWindowPacket(u8 *arg0, s32 arg1, s32 arg2, u16 arg3, void *arg4) {
    arg0[3] = 2;
    *(u32 *)(arg0 + 4) = _get_mode(arg1, arg2, arg3);
    *(u32 *)(arg0 + 8) = Gpu_BuildTexWindowCmd(arg4);
}
