#include "common.h"
#include <stdio.h>
#include "psyq/kernel.h"
#include "psyq/gpu_internal.h"

long Gpu_CheckTimeout(void) {
    long intrMask;
    long state;
    long result;
    long *dc;
    volatile u_long *gp1ForLog;
    long pending;
    long gpuTail;

    switch (0) { default:
    if (g_GpuTimeoutDeadline >= VSync(-1)) {
        state = g_GpuTimeoutPolls++;
        if (state <= 0xF0000) {
            break;
        }
    }

    gp1ForLog = g_GpuGp1;
    (void)*gp1ForLog;
    pending = g_GpuQueueWriteIdx;
    gpuTail = g_GpuQueueReadIdx;
    printf(g_MsgGpuTimeout, (pending - gpuTail) & 0x3F, *gp1ForLog, *g_GpuDmaChcr, *g_GpuDmaMadr);
    dc = (long *)&g_GpuLastCb;
    asm("" : "=r"(dc) : "0"(dc));
    printf(g_MsgGpuTimeoutCallback, *dc, g_GpuLastCbArg, g_GpuLastCbData);

    intrMask = SetIntrMask(0);
    g_GpuQueueReadIdx = 0;
    state = g_GpuQueueReadIdx;
    g_GpuResetIntrMask = intrMask;
    g_GpuQueueWriteIdx = state;

    *g_GpuDmaChcr = 0x401;
    *g_GpuDpcr |= 0x800;
    *g_GpuGp1 = 0x02000000;
    *g_GpuGp1 = 0x01000000;
    SetIntrMask(g_GpuResetIntrMask);
    result = -1;
    return result;

    }
    result = 0;

    return result;
}

/* GPU-type probe: GP1(10h) info word 7, then a texture-window write-back
 * test. Returns 0..4; ResetGraph stores it as the graph type. */
long Gpu_ProbeType(u_long probe) {
    volatile u_long *gp0;
    u_long status;

    *g_GpuGp1 = 0x10000007;
    gp0 = g_GpuGp0;
    status = *gp0 & 0xFFFFFF;

    if (status != 2) {
        *gp0 = (*g_GpuGp1 & 0x3FFF) | 0xE1001000;
        *g_GpuGp0;

        if ((*g_GpuGp1 & 0x1000) == 0) {
            return 0;
        }

        if ((probe & 8) == 0) {
            return 1;
        }

        *g_GpuGp1 = 0x20000504;
        return 2;
    }

    if ((probe & 8) == 0) {
        return 3;
    }

    *g_GpuGp1 = 0x9000001;
    return 4;
}

void MemFill(u_char *dst, long value, long count) {
    volatile long unused;
    long i = count - 1;

    if (count != 0) {
        long end = -1;

        do {
            *dst = value;
            i--;
            dst++;
        } while (i != end);
    }
}

u_long GPU_cw[4] __attribute__((section(".text"))) = {
    0x240A00A0,
    0x01400008,
    0x24090049,
    0,
};

u_char *MemCopy(u_char *dst, u_char *src, long count) {
    u_char *ret;

    if (dst == 0) {
        return 0;
    }

    ret = dst;
    if (count > 0) {
        do {
            *dst = *src;
            src++;
            count--;
            dst++;
        } while (count > 0);
    }

    return ret;
}
