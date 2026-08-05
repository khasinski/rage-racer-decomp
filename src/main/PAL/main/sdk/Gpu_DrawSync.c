#include <sys/types.h>

#include "common.h"
#include "psyq/kernel.h"
#include "psyq/gpu.h"

void Gpu_ArmTimeout(void);

extern volatile u_long *g_GpuGp1;
extern long g_GpuQueueWriteIdx;
extern long g_GpuQueueReadIdx;

void Gpu_ExecuteQueue(void);
long Gpu_CheckTimeout(void);

long Gpu_DrawSync(long arg0) {
    long pending;

    if (arg0 == 0) {
        Gpu_ArmTimeout();
        switch (0) { default:
        break;

retry:
        Gpu_ExecuteQueue();
        if (Gpu_CheckTimeout() != 0) {
            return -1;
        }

        }
        if (!(g_GpuQueueWriteIdx == g_GpuQueueReadIdx)) {
        goto retry;

waitReady:
        if (Gpu_CheckTimeout() != 0) {
            return -1;
        }

        }
        if (!(*g_GpuDmaChcr & 0x01000000)) {

        if (*g_GpuGp1 & 0x04000000) {
            return 0;
        }
        }
        goto waitReady;
    }

    pending = (g_GpuQueueWriteIdx - g_GpuQueueReadIdx) & 0x3F;
    switch (0) { default:
    if (pending != 0) {
        Gpu_ExecuteQueue();
    }

    if (*g_GpuDmaChcr & 0x01000000) {
    } else {

    if (*g_GpuGp1 & 0x04000000) {
        break;
    }

    }
    if (pending == 0) {
        return 1;
    }
    return pending;

    }
    return pending;
}

void Gpu_ArmTimeout(void) { g_GpuTimeoutDeadline = VSync(-1) + 240; g_GpuTimeoutPolls = 0; }
