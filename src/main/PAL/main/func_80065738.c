#include "common.h"
#include "psyq/gpu.h"
#include "psyq/kernel.h"

extern GpuCallbacks *g_GpuCallbacks asm("D_800941E0");
extern void (*GPU_printf)(char *, ...) asm("D_800941E4");
extern u8 g_GraphQueue asm("D_800941E9");
extern u8 g_GraphDebug asm("D_800941EA");
extern char D_800134F0[];

s32 func_80065738(s32 arg0) {
    register s32 newQueue asm("$16") = arg0;
    register u8 *queue asm("$17") = &g_GraphQueue;
    register s32 oldQueue asm("$18");

    oldQueue = *queue;
    if (g_GraphDebug >= 2) {
        GPU_printf(D_800134F0, newQueue);
    }

    if (newQueue != *queue) {
        g_GpuCallbacks->resetGraph(1);
        *queue = newQueue;
        ResetCallback(2, 0);
    }

    return oldQueue;
}
