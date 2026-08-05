#include <sys/types.h>

#include "common.h"

void Gpu_ArmTimeout(void);

extern volatile u_long *g_GpuGp1;
extern volatile u_long *g_GpuDmaChcr;
extern long g_GpuQueueWriteIdx;
extern long g_GpuQueueReadIdx;

void Gpu_ExecuteQueue(void);
long Gpu_CheckTimeout(void);

/* Driver-table slot +0x3C, the body of DrawSync: mode 0 blocks until the
 * queue is empty and the GPU idle, any other mode returns the number of
 * queue entries still outstanding. */
long Gpu_DrawSync(long mode);
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

extern long g_GpuTimeoutDeadline;
extern long g_GpuTimeoutPolls;
long VSync(long mode);
void Gpu_ArmTimeout(void) { g_GpuTimeoutDeadline = VSync(-1) + 240; g_GpuTimeoutPolls = 0; }
