#include <sys/types.h>

#include "common.h"
#include "psyq/gpu.h"

extern QEntry g_GpuQueue[];
extern volatile long g_GpuQueueWriteIdx;
extern long g_GpuQueueReadIdx;
extern long g_AddQueueIntrMask;
extern u_char g_GpuLastCb[];
extern volatile u_long *g_GpuGp1;
extern volatile u_char g_GraphQueue;
extern long g_DrawSyncCallback;

extern void Gpu_ArmTimeout(void);
extern long Gpu_CheckTimeout(void);
extern void Gpu_ExecuteQueue(void);
extern long SetIntrMask(long);
extern void DMACallback(long, void *);

/* Driver-table slot +0x08, the `send` entry every libgpu call goes through:
 * runs the worker immediately when the queue is empty and the GPU idle,
 * otherwise copies `size` bytes of parameters into the 64-entry, 96-byte
 * queue at g_GpuQueue and arms the DMA2 callback Gpu_ExecuteQueue. */
long Gpu_AddQueue(void (*worker)(long, long), long param, long size, long data);
long Gpu_AddQueue(void (*cb)(long, long), long arg, long size, long tag) {
    long i;
    long *src;
    long ret;

    Gpu_ArmTimeout();
    while ((((g_GpuQueueWriteIdx + 1) & 0x3f)) == g_GpuQueueReadIdx) {
        if (Gpu_CheckTimeout() != 0) {
            return -1;
        }
        Gpu_ExecuteQueue();
    }

    ret = SetIntrMask(0);
    *(volatile long *)g_DrawSyncCbPending = 1;
    g_AddQueueIntrMask = ret;

    switch (0) { default:
    if (g_GraphQueue != 0) {
        if (g_GpuQueueWriteIdx != g_GpuQueueReadIdx || (*g_GpuDmaChcr & 0x01000000) != 0 || g_DrawSyncCallback != 0) break;
    }

    do {
    } while ((*g_GpuGp1 & 0x04000000) == 0);

    cb(arg, tag);
    *(void (*volatile *)(long, long))g_GpuLastCb = cb;
    g_GpuLastCbArg = arg;
    g_GpuLastCbData = tag;
    SetIntrMask(g_AddQueueIntrMask);
    return 0;

    }
    DMACallback(2, (void *)Gpu_ExecuteQueue);

    if (size != 0) {
        u_char *pbase = (u_char *)g_GpuQueue + 12;
        i = 0;
        src = (long *)arg;
        while (i < size / 4) {
            *(long *)(pbase + g_GpuQueueWriteIdx * 96 + i * 4) = *src;
            src++;
            i++;
            asm("");
        }
        g_GpuQueue[g_GpuQueueWriteIdx].arg = (long)g_GpuQueue[g_GpuQueueWriteIdx].params;
    } else {
        g_GpuQueue[g_GpuQueueWriteIdx].arg = arg;
    }

    g_GpuQueue[g_GpuQueueWriteIdx].tag = tag;
    asm("");
    g_GpuQueue[g_GpuQueueWriteIdx].cb = cb;
    asm("");

    g_GpuQueueWriteIdx = (g_GpuQueueWriteIdx + 1) & 0x3f;
    SetIntrMask(g_AddQueueIntrMask);
    Gpu_ExecuteQueue();
    return (g_GpuQueueWriteIdx - g_GpuQueueReadIdx) & 0x3f;
}
