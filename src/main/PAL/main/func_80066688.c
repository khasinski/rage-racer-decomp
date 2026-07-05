#include "common.h"

u32 Gpu_BuildDrawOffsetCmd(s16 arg0, s16 arg1) asm("func_80066BE4");

void SetDrawOffset(u8 *arg0, s16 *arg1) asm("func_80066688");

void SetDrawOffset(u8 *arg0, s16 *arg1) {
    arg0[3] = 2;
    *(u32 *)(arg0 + 4) = Gpu_BuildDrawOffsetCmd(arg1[0], arg1[1]);
    *(u32 *)(arg0 + 8) = 0;
}
