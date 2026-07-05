#include "common.h"

extern volatile u32 *g_GpuGp0 asm("D_800942B8");
extern volatile u32 *g_GpuGp1 asm("D_800942BC");
extern volatile u32 *D_800942C0;
extern volatile u32 *D_800942C4;
extern volatile u32 *D_800942C8;

void Gpu_StartDmaTransfer(u32 arg0) asm("func_80067600");

void Gpu_StartDmaTransfer(u32 arg0) {
    *g_GpuGp1 = 0x4000002;
    *D_800942C0 = arg0;
    *D_800942C4 = 0;
    *D_800942C8 = 0x1000401;
}

u32 _param(u32 arg0) asm("func_8006764C");

u32 _param(u32 arg0) {
    *g_GpuGp1 = arg0 | 0x10000000;
    return *g_GpuGp0 & 0xFFFFFF;
}
