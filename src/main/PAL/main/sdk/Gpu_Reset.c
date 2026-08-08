#include "common.h"
#include "psyq/kernel.h"
#include "psyq/gpu.h"

extern volatile u_long *g_GpuGp1;
extern long g_GpuQueueWriteIdx;
extern volatile long g_GpuQueueReadIdx;
extern u_char g_GpuQueue[];

long Gpu_ProbeType(u_long probe);

/* Driver-table slot +0x34, the body of ResetGraph: empties the queue,
 * re-arms DMA2, and issues GP1(00h) (mode 0) or GP1(02h)+GP1(01h)
 * (mode 1). Mode 0 also probes the GPU type through Gpu_ProbeType. */
long Gpu_Reset(u_long mode) {
    long intrMask;
    long state;

    intrMask = SetIntrMask(0);
    g_GpuQueueReadIdx = 0;
    state = g_GpuQueueReadIdx;
    g_GpuResetIntrMask = intrMask;
    g_GpuQueueWriteIdx = state;

    switch (mode % 8) {
    case 0:
        *g_GpuDmaChcr = 0x401;
        *g_GpuDpcr |= 0x800;
        *g_GpuGp1 = 0;
        MemFill(g_GpuGp1Mirror, 0, 0x100);
        MemFill(g_GpuQueue, 0, 0x1800);
        break;
    case 1:
        *g_GpuDmaChcr = 0x401;
        *g_GpuDpcr |= 0x800;
        *g_GpuGp1 = 0x02000000;
        *g_GpuGp1 = 0x01000000;
        break;
    }

    SetIntrMask(g_GpuResetIntrMask);

    if ((mode % 8) == 0) {
        return Gpu_ProbeType(mode);
    }

    return 0;
}
