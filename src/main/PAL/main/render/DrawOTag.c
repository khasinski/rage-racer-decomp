#include "common.h"
#include "psyq/gpu.h"
#include "game/render.h"

extern GpuCallbacks *g_GpuFuncs;
extern u8 g_GraphDebug;



void DrawOTag(void *ot) {
    if (g_GraphDebug >= 2) {
        GPU_printf(D_800135CC, ot);
    }
    g_GpuFuncs->send(g_GpuFuncs->sendList, ot, 0, 0);
}

/* Named from its own trace string D_800135E0, "PutDrawEnv(%08x)...". */
void *PutDrawEnv(void *env) {
    u8 *debug = &g_GraphDebug;
    void *prim = env;
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

void *DrawOTagEnv(void *ot, void *env) {
    void *src = ot;
    u8 *debug = &g_GraphDebug;
    void *prim = env;
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
