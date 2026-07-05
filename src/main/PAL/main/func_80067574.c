#include "common.h"

extern u32 *g_GpuGp1 asm("D_800942BC");
extern u8 g_GpuGp1Mirror[] asm("D_8009B9F0");

void Gpu_WriteGp1(u32 arg0) asm("func_80067574");

void Gpu_WriteGp1(u32 arg0) {
    *g_GpuGp1 = arg0;
    g_GpuGp1Mirror[arg0 >> 24] = arg0;
}

u8 Gpu_GetControlMirrorByte(s32 arg0) asm("func_8006759C");

u8 Gpu_GetControlMirrorByte(s32 arg0) {
    return g_GpuGp1Mirror[arg0];
}
