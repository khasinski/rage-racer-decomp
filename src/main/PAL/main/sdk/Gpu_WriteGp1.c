#include <sys/types.h>

#include "common.h"

extern u_long *g_GpuGp1 asm("D_800942BC");
extern volatile u_long *g_GpuGp0 asm("D_800942B8");
extern volatile u_long *g_GpuGp1Volatile asm("D_800942BC");
extern volatile u_long *D_800942C0;
extern volatile u_long *D_800942C4;
extern volatile u_long *D_800942C8;
extern u_char g_GpuGp1Mirror[] asm("D_8009B9F0");

void Gpu_WriteGp1(u_long arg0) asm("func_80067574");
long Gpu_WriteGp0Words(u_long *src, long count) asm("func_800675B0");
void Gpu_StartDmaTransfer(u_long arg0) asm("func_80067600");
u_long _param(u_long arg0) asm("func_8006764C");
extern long func_800676A0(void *callback, void *data, long size, long arg3);

void Gpu_WriteGp1(u_long arg0) {
    *g_GpuGp1 = arg0;
    g_GpuGp1Mirror[arg0 >> 24] = arg0;
}

u_char Gpu_GetControlMirrorByte(long arg0) asm("func_8006759C");

u_char Gpu_GetControlMirrorByte(long arg0) {
    return g_GpuGp1Mirror[arg0];
}

long Gpu_WriteGp0Words(u_long *src, long count) {
    long i;

    *g_GpuGp1Volatile = 0x04000000;
    for (i = count - 1; i != -1; i--) {
        *g_GpuGp0 = *src;
        src++;
    }
    return 0;
}

void Gpu_StartDmaTransfer(u_long arg0) {
    *g_GpuGp1Volatile = 0x4000002;
    *D_800942C0 = arg0;
    *D_800942C4 = 0;
    *D_800942C8 = 0x1000401;
}

u_long _param(u_long arg0) {
    *g_GpuGp1Volatile = arg0 | 0x10000000;
    return *g_GpuGp0 & 0xFFFFFF;
}

long func_8006767C(void *callback, void *data, long arg2) {
    return func_800676A0(callback, data, 0, arg2);
}
