#include <sys/types.h>

#include "common.h"

typedef struct QEntry {
    void (*cb)(long, long);
    long arg;
    long tag;
    long params[21];
} QEntry;

extern QEntry g_GpuQueue[] asm("D_801E5024");
extern volatile long g_GpuQueueWriteIdx asm("D_800942EC");
extern long g_GpuQueueReadIdx asm("D_800942F0");
extern long g_AddQueueIntrMask asm("D_800942F4");
extern u_char g_GpuLastCb[] asm("D_800942DC");
extern long g_GpuLastCbArg asm("D_800942E0");
extern long g_GpuLastCbData asm("D_800942E4");
extern volatile u_long *g_GpuDmaChcr asm("D_800942C8");
extern volatile u_long *g_GpuGp1 asm("D_800942BC");
extern u_char g_DrawSyncCbPending[] asm("D_800941F0");
extern volatile u_char g_GraphQueue asm("D_800941E9");
extern long g_DrawSyncCallback asm("D_800941F4");

extern void Gpu_ArmTimeout(void) asm("func_80067F04");
extern long Gpu_CheckTimeout(void) asm("func_80067F38");
extern void func_80067984(void);
extern long SetIntrMask(long) asm("func_8006E0B0");
extern void DMACallback(long, void *) asm("func_8006DF94");

/* Driver-table slot +0x08, the `send` entry every libgpu call goes through:
 * runs the worker immediately when the queue is empty and the GPU idle,
 * otherwise copies `size` bytes of parameters into the 64-entry, 96-byte
 * queue at D_801E5024 and arms the DMA2 callback Gpu_ExecuteQueue. */
long Gpu_AddQueue(void (*worker)(long, long), long param, long size, long data) asm("func_800676A0");
long Gpu_AddQueue(void (*cb)(long, long), long arg, long size, long tag) {
    long i;
    long *src;
    long ret;

    Gpu_ArmTimeout();
    while ((((g_GpuQueueWriteIdx + 1) & 0x3f)) == g_GpuQueueReadIdx) {
        if (Gpu_CheckTimeout() != 0) {
            return -1;
        }
        func_80067984();
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
    DMACallback(2, (void *)func_80067984);

    if (size != 0) {
        u_char *pbase = (u_char *)g_GpuQueue + 12;
        i = 0;
        src = (long *)arg;
        while (i < size / 4) {
            *(long *)(pbase + g_GpuQueueWriteIdx * 96 + i * 4) = *src;
            src++;
            i++;
            /* This barrier is load-bearing: removing it changes .text. */
            asm("");
        }
        g_GpuQueue[g_GpuQueueWriteIdx].arg = (long)g_GpuQueue[g_GpuQueueWriteIdx].params;
    } else {
        g_GpuQueue[g_GpuQueueWriteIdx].arg = arg;
    }

    g_GpuQueue[g_GpuQueueWriteIdx].tag = tag;
    /* These barriers are load-bearing: removing any one changes .text. */
    asm("");
    g_GpuQueue[g_GpuQueueWriteIdx].cb = cb;
    asm("");

    g_GpuQueueWriteIdx = (g_GpuQueueWriteIdx + 1) & 0x3f;
    SetIntrMask(g_AddQueueIntrMask);
    func_80067984();
    return (g_GpuQueueWriteIdx - g_GpuQueueReadIdx) & 0x3f;
}
