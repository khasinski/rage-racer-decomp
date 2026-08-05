#include <sys/types.h>

#include "common.h"
#include "psyq/gpu.h"

typedef struct LastCb {
    void (*cb)(long, long);
    long arg;
    long tag;
} LastCb;

extern volatile QEntry g_GpuQueue[];
extern volatile long g_GpuQueueWriteIdx;
extern volatile long g_GpuQueueReadIdx;
extern long g_ExecQueueIntrMask;
extern volatile LastCb g_GpuLastCb;
extern volatile u_long *g_GpuGp1;
extern volatile long g_DrawSyncCallback;

extern long SetIntrMask(long);
extern void DMACallback(long, void *);

/* Driver-table slot +0x24, and the DMA2 completion callback: drains the
 * Gpu_AddQueue ring, then fires the DrawSyncCallback when it empties.
 * Returns the number of entries still queued. */
long Gpu_ExecuteQueue(void) {
    if (*g_GpuDmaChcr & 0x01000000) {
        return 1;
    }

    g_ExecQueueIntrMask = SetIntrMask(0);

    if (g_GpuQueueWriteIdx != g_GpuQueueReadIdx) {
        while ((*g_GpuDmaChcr & 0x01000000) == 0) {
            if ((((g_GpuQueueReadIdx + 1) & 0x3f) == g_GpuQueueWriteIdx) &&
                (*(volatile long *)(u_char *)&g_DrawSyncCallback == 0)) {
                DMACallback(2, 0);
            }

            while ((*g_GpuGp1 & 0x04000000) == 0) {
                ;
            }

            g_GpuQueue[g_GpuQueueReadIdx].cb(g_GpuQueue[g_GpuQueueReadIdx].arg,
                                      g_GpuQueue[g_GpuQueueReadIdx].tag);
            g_GpuLastCb.cb = g_GpuQueue[g_GpuQueueReadIdx].cb;
            g_GpuLastCb.arg = g_GpuQueue[g_GpuQueueReadIdx].arg;
            g_GpuLastCb.tag = g_GpuQueue[g_GpuQueueReadIdx].tag;

            g_GpuQueueReadIdx = (g_GpuQueueReadIdx + 1) & 0x3f;
            if (g_GpuQueueWriteIdx == g_GpuQueueReadIdx) {
                break;
            }
        }
    }

    SetIntrMask(g_ExecQueueIntrMask);

    if ((g_GpuQueueWriteIdx == g_GpuQueueReadIdx) && ((*g_GpuDmaChcr & 0x01000000) == 0) &&
        (*(volatile long *)g_DrawSyncCbPending != 0) && (g_DrawSyncCallback != 0)) {
        *(volatile long *)g_DrawSyncCbPending = 0;
        ((void (*)(void))g_DrawSyncCallback)();
    }

    return (g_GpuQueueWriteIdx - g_GpuQueueReadIdx) & 0x3f;
}
