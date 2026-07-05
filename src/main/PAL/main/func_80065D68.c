#include "common.h"

typedef struct GpuFuncs {
    u8 pad0[0x2C];
    void (*clearImage)(void *rect, s32 rgb);
} GpuFuncs;

extern GpuFuncs *g_GpuFuncs asm("D_800941E0");
extern u8 g_GraphDebug asm("D_800941EA");
extern void (*D_800941E4)(char *, ...);
extern char D_800135B4[];
extern u32 D_800942A4;

void *ClearOTagR(u32 *arg0, s32 arg1) asm("func_80065D68");

void *ClearOTagR(u32 *arg0, s32 arg1) {
    if (g_GraphDebug >= 2) {
        D_800941E4(D_800135B4, arg0, arg1);
    }

    g_GpuFuncs->clearImage(arg0, arg1);

    {
        register u32 mask asm("$4") = 0xFFFFFF;
        register u32 *ret asm("$2") = arg0;
        register u32 next asm("$3");

        asm("" : "=r"(ret), "=r"(mask) : "0"(ret), "1"(mask));
        next = (u32)&D_800942A4;
        asm volatile("and $3,$3,$4 # maspsx-keep" : "=r"(next) : "0"(next), "r"(mask));
        *ret = next;
        return ret;
    }
}
