#include "common.h"
#include "psyq/gpu.h"

extern GpuCallbacks *g_GpuCallbacks asm("D_800941E0");
extern void (*GPU_printf)(char *, ...) asm("D_800941E4");
extern u8 g_GraphDebug asm("D_800941EA");
extern char D_80013520[];

void func_80068180(u8 *dst, s32 value, s32 count);

void func_80065860(s32 arg0) {
    register s32 enable asm("$16") = arg0;
    register u8 *debug asm("$17") = &g_GraphDebug;
    register u8 *clearPtr asm("$4");
    register GpuCallbacks *gpu asm("$2");
    register s32 mask asm("$4");

    if (*debug >= 2) {
        GPU_printf(D_80013520, enable);
    }

    clearPtr = debug + 0x6A;
    if (enable == 0) {
        func_80068180(clearPtr, -1, 0x14);
    }

    mask = 0x3000000;
    asm("" : "=r"(mask) : "0"(mask));
    gpu = g_GpuCallbacks;
    if (enable == 0) {
        mask |= 1;
    } else {
        mask = 0x3000000;
    }

    gpu->submit(mask);
}
