#include "common.h"

extern u8 g_GraphDebug asm("D_800941EA");
extern void (*D_800941E4)(char *, ...);
extern char D_8001359C[];
extern char D_800135B4[];
extern u32 D_800942A4;

typedef struct ClearOTagRGpuFuncs {
    u8 pad0[0x2C];
    void (*clearImage)(void *rect, s32 rgb);
} ClearOTagRGpuFuncs;

extern ClearOTagRGpuFuncs *g_GpuFuncsClearOTagR asm("D_800941E0");

void *ClearOTag(u32 *arg0, s32 count) asm("func_80065CB0");
void *ClearOTagR(u32 *arg0, s32 arg1) asm("func_80065D68");

void *ClearOTag(u32 *arg0, s32 count) {
    register u32 *ptr asm("$16") = arg0;
    register s32 remaining asm("$17") = count;

    if (g_GraphDebug >= 2) {
        register void (*debug)(char *, ...) asm("$2") = D_800941E4;

        asm("" : "=r"(debug) : "0"(debug));
        debug(D_8001359C, ptr, remaining);
    }

    remaining--;
    if (remaining != 0) {
        register u32 mask asm("$5") = 0xFFFFFF;
        register u32 hiMask asm("$6") = 0xFF000000;

        do {
            register u32 *next asm("$4");
            register u32 tag asm("$2");
            register u32 low asm("$3");

            remaining--;
            next = ptr + 1;
            ((u8 *)ptr)[3] = 0;
            tag = *ptr;
            low = (u32)next & mask;
            tag &= hiMask;
            tag |= low;
            *ptr = tag;
            ptr = next;
        } while (remaining != 0);
    }

    *ptr = (u32)&D_800942A4 & 0xFFFFFF;
    return ptr;
}

void *ClearOTagR(u32 *arg0, s32 arg1) {
    if (g_GraphDebug >= 2) {
        D_800941E4(D_800135B4, arg0, arg1);
    }

    g_GpuFuncsClearOTagR->clearImage(arg0, arg1);

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
