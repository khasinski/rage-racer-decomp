#include <sys/types.h>

#include "common.h"
#include "psyq/gpu.h"

/* The libgpu copy of the hardware pointers, initialised in the data
 * segment: GP0 0x1F801810, GP1 0x1F801814, DMA channel 2 (GPU) MADR/BCR/CHCR
 * 0x1F8010A0/A4/A8. See docs/names.md "Hardware mirrors". */
extern volatile u_long *g_GpuGp1;

void Gpu_StartDmaTransfer(u_long arg0);
u_long _param(u_long arg0);
extern long Gpu_AddQueue(void *callback, void *data, long size, long arg3);

void Gpu_WriteGp1(u_long arg0) {
    *g_GpuGp1 = arg0;
    g_GpuGp1Mirror[arg0 / 16777216] = arg0;
}

u_char Gpu_GetControlMirrorByte(long arg0) {
    return g_GpuGp1Mirror[arg0];
}

long Gpu_WriteGp0Words(u_long *src, long count) {
    long i;

    *g_GpuGp1 = 0x04000000;
    for (i = count - 1; i != -1; i--) {
        *g_GpuGp0 = *src;
        src++;
    }
    return 0;
}

void Gpu_StartDmaTransfer(u_long arg0) {
    *g_GpuGp1 = 0x4000002;
    *g_GpuDmaMadr = arg0;
    *g_GpuDmaBcr = 0;
    *g_GpuDmaChcr = 0x1000401;
}

u_long _param(u_long arg0) {
    *g_GpuGp1 = arg0 | 0x10000000;
    return *g_GpuGp0 & 0xFFFFFF;
}

long _addque(void *callback, void *data, long arg2) {
    return Gpu_AddQueue(callback, data, 0, arg2);
}
