#include "common.h"

extern u32 *g_GpuGp1 asm("D_800942BC");
extern volatile u32 *g_GpuGp0 asm("D_800942B8");
extern volatile u32 *g_GpuGp1Volatile asm("D_800942BC");
extern volatile u32 *D_800942C0;
extern volatile u32 *D_800942C4;
extern volatile u32 *D_800942C8;
extern u8 g_GpuGp1Mirror[] asm("D_8009B9F0");

void Gpu_WriteGp1(u32 arg0) asm("func_80067574");
s32 Gpu_WriteGp0Words(u32 *src, s32 count) asm("func_800675B0");
void Gpu_StartDmaTransfer(u32 arg0) asm("func_80067600");
u32 _param(u32 arg0) asm("func_8006764C");
extern s32 func_800676A0(void *callback, void *data, s32 size, s32 arg3);

void Gpu_WriteGp1(u32 arg0) {
    *g_GpuGp1 = arg0;
    g_GpuGp1Mirror[arg0 >> 24] = arg0;
}

u8 Gpu_GetControlMirrorByte(s32 arg0) asm("func_8006759C");

u8 Gpu_GetControlMirrorByte(s32 arg0) {
    return g_GpuGp1Mirror[arg0];
}

s32 Gpu_WriteGp0Words(u32 *src, s32 count) {
    volatile s32 unused;
    s32 i = count - 1;

    *g_GpuGp1Volatile = 0x04000000;
    if (count != 0) {
        s32 end = -1;

        do {
            *g_GpuGp0 = *src;
            src++;
            i--;
        } while (i != end);
    }
    return 0;
}

void Gpu_StartDmaTransfer(u32 arg0) {
    *g_GpuGp1Volatile = 0x4000002;
    *D_800942C0 = arg0;
    *D_800942C4 = 0;
    *D_800942C8 = 0x1000401;
}

u32 _param(u32 arg0) {
    *g_GpuGp1Volatile = arg0 | 0x10000000;
    return *g_GpuGp0 & 0xFFFFFF;
}

s32 func_8006767C(void *callback, void *data, s32 arg2) {
    return func_800676A0(callback, data, 0, arg2);
}
