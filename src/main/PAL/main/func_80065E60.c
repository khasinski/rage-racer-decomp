#include "common.h"
#include "psyq/gpu.h"

extern GpuCallbacks *g_GpuFuncs asm("D_800941E0");
extern u8 g_GraphDebug asm("D_800941EA");
extern void (*D_800941E4)(char *, ...);
extern char D_800135CC[];
extern char D_800135E0[];
extern char D_800135F8[];

void func_8006674C(void *arg0, void *arg1);
void *func_800681BC(void *dst, void *src, s32 count);

void DrawOTag(void *arg0) asm("func_80065E60");

void DrawOTag(void *arg0) {
    if (g_GraphDebug >= 2) {
        D_800941E4(D_800135CC, arg0);
    }
    g_GpuFuncs->send(g_GpuFuncs->moveImage, arg0, 0, 0);
}

void *func_80065ED4(void *arg0) {
    register u8 *debug asm("$18") = &g_GraphDebug;
    register void *prim asm("$17") = arg0;
    register void *tag asm("$16");

    if (*debug >= 2) {
        D_800941E4(D_800135E0, prim);
    }

    tag = (u8 *)prim + 0x1C;
    func_8006674C(tag, prim);
    {
        register u32 mask asm("$4") = 0xFFFFFF;
        register void *sendTag asm("$5") = tag;
        register s32 size asm("$6") = 0x40;
        register u32 word asm("$2") = *(u32 *)tag;
        register GpuCallbacks *gpu asm("$3") = g_GpuFuncs;

        word |= mask;
        *(u32 *)tag = word;
        gpu->send(gpu->moveImage, sendTag, size, 0);
    }
    func_800681BC(debug + 0xE, prim, 0x5C);
    return prim;
}

void *func_80065F98(void *arg0, void *arg1) {
    register void *src asm("$18") = arg0;
    register u8 *debug asm("$19") = &g_GraphDebug;
    register void *prim asm("$17") = arg1;
    register void *tag asm("$16");

    if (*debug >= 2) {
        D_800941E4(D_800135F8, src, prim);
    }

    tag = (u8 *)prim + 0x1C;
    func_8006674C(tag, prim);
    {
        register u32 mask asm("$4") = 0xFFFFFF;
        register void *sendTag asm("$5") = tag;
        register s32 size asm("$6") = 0x40;
        register u32 highMask asm("$3") = 0xFF000000;
        register u32 word asm("$2") = *(u32 *)tag;
        register GpuCallbacks *gpu asm("$3");

        asm("" : "=r"(size), "=r"(highMask), "=r"(word) : "0"(size), "1"(highMask), "2"(word));
        mask = (u32)src & mask;
        word &= highMask;
        gpu = g_GpuFuncs;
        word |= mask;
        *(u32 *)tag = word;
        gpu->send(gpu->moveImage, sendTag, size, 0);
    }
    return func_800681BC(debug + 0xE, prim, 0x5C);
}
