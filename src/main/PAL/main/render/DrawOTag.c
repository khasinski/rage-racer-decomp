#include "common.h"
#include "psyq/gpu.h"

extern GpuCallbacks *g_GpuFuncs;
extern u8 g_GraphDebug;
/* libgpu's printf hook; every GPU trace string goes through it. */
extern void (*GPU_printf)(char *, ...);
extern char D_800135CC[];
extern char D_800135E0[];
extern char D_800135F8[];

void Gpu_BuildDrawEnvCmds(void *arg0, void *arg1);
void *MemCopy(void *dst, void *src, s32 count);

void DrawOTag(void *arg0);

void DrawOTag(void *arg0) {
    if (g_GraphDebug >= 2) {
        GPU_printf(D_800135CC, arg0);
    }
    g_GpuFuncs->send(g_GpuFuncs->sendList, arg0, 0, 0);
}

/* Named from its own trace string D_800135E0, "PutDrawEnv(%08x)...". */
void *PutDrawEnv(void *env);
void *PutDrawEnv(void *arg0) {
    u8 *debug = &g_GraphDebug;
    void *prim = arg0;
    void *tag;

    if (*debug >= 2) {
        GPU_printf(D_800135E0, prim);
    }

    tag = (u8 *)prim + 0x1C;
    Gpu_BuildDrawEnvCmds(tag, prim);
    {
        u32 mask = 0xFFFFFF;
        void *sendTag = tag;
        s32 size = 0x40;
        u32 word = *(u32 *)tag;
        GpuCallbacks *gpu = g_GpuFuncs;

        word |= mask;
        *(u32 *)tag = word;
        gpu->send(gpu->sendList, sendTag, size, 0);
    }
    MemCopy(debug + 0xE, prim, 0x5C);
    return prim;
}

/* Named from its own trace string D_800135F8, "DrawOTagEnv(%08x,&08x)...". */
void *DrawOTagEnv(void *ot, void *env);
void *DrawOTagEnv(void *arg0, void *arg1) {
    void *src = arg0;
    u8 *debug = &g_GraphDebug;
    void *prim = arg1;
    void *tag;

    if (*debug >= 2) {
        GPU_printf(D_800135F8, src, prim);
    }

    tag = (u8 *)prim + 0x1C;
    Gpu_BuildDrawEnvCmds(tag, prim);
    {
        u32 word = (*(u32 *)tag & 0xFF000000) |
                   ((u32)src & 0xFFFFFF);
        GpuCallbacks *gpu = g_GpuFuncs;

        *(u32 *)tag = word;
        gpu->send(gpu->sendList, tag, 0x40, 0);
    }
    return MemCopy(debug + 0xE, prim, 0x5C);
}
