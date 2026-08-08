#include "common.h"
#include <stdio.h>
#include "psyq/gpu.h"
#include "psyq/kernel.h"
#include "game/render.h"
#include "psyq/gpu_internal.h"

void SetDrawTPage(u_char *prim, long dfe, long dtd, long tpage) {
s32 encoded;
    s32 mode;

    prim[3] = 1;
    mode = GetGraphType();
    if ((mode == 1) || (GetGraphType() == 2)) {
        s32 value = 0xE1000000;

        if (dtd != 0) {
            value = 0xE1000800;
        }
        encoded = tpage & 0x27FF;
        if (dfe != 0) {
            encoded |= 0x1000;
        }
        { s32 rel = encoded; encoded = value | rel; }
    } else {
        encoded = 0xE1000000 | (dtd ? 0x200 : 0) |
                  (dfe ? 0x400 : 0) | (tpage & 0x9FF);
    }
    *(u32 *)&prim[4] = encoded;
}

void SetDrawLoad(u8 *p, u8 *rect) {
    s32 sign;
    s32 value;
    s32 size;

    sign = *(s16 *)&rect[4];
    value = sign * *(s16 *)&rect[6];
    value += 1;
    sign = (u32)value >> 31;
    value += sign;
    value >>= 1;
    size = value + 4;
    if ((u32)(value - 1) >= 0xB) {
        size = 0;
    }

    *(u32 *)&p[4] = 0x01000000;
    p[3] = size;
    *(u32 *)&p[8] = 0xA0000000;
    *(u32 *)&p[0xC] = *(u32 *)&rect[0];
    *(u32 *)&p[0x10] = *(u32 *)&rect[4];
}

s32 MargePrim(u8 *p0, u8 *p1) {
    s32 value;

    value = p0[3] + p1[3] + 1;
    if (value >= 0x21) {
        return -1;
    }
    p0[3] = value;
    *(u32 *)p1 = 0;
    return 0;
}

void DumpDrawEnv(DrawEnv *env) {
    s32 mode;
    u32 value;

    GPU_printf(g_FmtGpuDrawClip, env->clip.x, env->clip.y, env->clip.w, env->clip.h);
    GPU_printf(g_FmtGpuDrawOffset, env->ofs[0], env->ofs[1]);
    GPU_printf(g_FmtGpuTextureWindow, env->tw.x, env->tw.y, env->tw.w, env->tw.h);
    GPU_printf(g_FmtGpuDither, env->dtd);
    GPU_printf(g_FmtGpuDrawOnDisplay, env->dfe);

    mode = GetGraphType();
    switch (0) { default:
    if (mode == 1) {
    } else {

    mode = GetGraphType();
    if (mode != 2) {
        break;
    }

    }
    value = env->tpage;
    GPU_printf(g_FmtGpuTPage, (value >> 9) & 3, (value >> 7) & 3, (value * 64) & 0x7C0, (value * 8) & 0x300);
    return;

    }
    value = env->tpage;
    GPU_printf(g_FmtGpuTPage, (value >> 7) & 3, (value >> 5) & 3, (value * 64) & 0x7C0, ((value * 16) & 0x100) + ((value >> 2) & 0x200));
}

void DumpDispEnv(DispEnv *env) {
    GPU_printf(g_FmtGpuDispArea, env->disp.x, env->disp.y, env->disp.w, env->disp.h);
    GPU_printf(g_FmtGpuScreenArea, env->screen.x, env->screen.y, env->screen.w, env->screen.h);
    GPU_printf(g_FmtGpuIsInterlace, env->isinter);
    GPU_printf(g_FmtGpuIsRgb24, env->isrgb24);
}


/* libgpu ResetGraph. Own trace strings g_FmtGpuResetGraphTrace "ResetGraph:jtb=%08x,env=%08x"
 * and g_FmtGpuResetGraph "ResetGraph(%d)..."; mode&7 of 0 or 3 does the full reset. */
void ResetGraph(s32 mode) {
    s32 maskedMode;
    u8 *graphState;
    s32 graphType;
    u8 *clearEnv;
    s32 fillValue;

    maskedMode = mode & 7;
    if ((maskedMode == 0) || (maskedMode == 3)) {
        graphState = &g_GraphType;
        printf((u8 *)g_FmtGpuResetGraphTrace, g_GpuJumpTable, graphState);
        MemFill(graphState, 0, 0x80);
        KernelCallbackSlot3();
        GPU_cw((void *)((u32)g_GpuFuncs & 0xFFFFFF));
        graphType = Gpu_Reset(maskedMode != 0);
        clearEnv = graphState + 0x10;
        *(volatile u8 *)graphState = graphType;
        {
            s32 st0 = *(volatile u8 *)graphState;
            u16 v;
            s32 st1;

            g_GraphQueue = 1;
            v = *(u16 *)&g_VramWidthTable[st0 * 4];
            st1 = *(volatile u8 *)graphState;
            fillValue = -1;
            g_VramWidth = v;
            g_VramHeight = *(u16 *)&g_VramHeightTable[st1 * 4];
        }
        MemFill(clearEnv, fillValue, 0x5C);
        MemFill(graphState + 0x6C, -1, 0x14);
        graphType = *(volatile u8 *)graphState;
    } else {
        if (g_GraphDebug >= 2) {
            GPU_printf(g_FmtGpuResetGraph, mode);
        }
        g_GpuFuncs->resetGraph(1);
    }
}


s32 SetGraphReverse(s32 mode) {
    u8 *state = &g_GraphReverse;
    s32 newValue = mode;
    s32 old = *state;
    GpuCallbacks *callbacks;
    GpuCallbacks *callbacks2;
    s32 value;
    s32 command;

    if (g_GraphDebug >= 2) {
        GPU_printf(g_FmtGpuSetGraphReverse, newValue);
    }

    callbacks = g_GpuFuncs;
    *state = newValue;
    value = callbacks->read(8);
    if (*state != 0) {
        command = value | 0x08000080;
    } else {
        command = value | 0x08000000;
    }
    g_GpuFuncs->submit(command);

    if (g_GraphType == 2) {
        callbacks2 = g_GpuFuncs;
        if (g_GraphReverse != 0) {
            command = 0x20000501;
        } else {
            command = 0x20000504;
        }
        callbacks2->submit(command);
    }

    return old;
}


s32 SetGraphDebug(u8 level) {
    u8 *ptr = &g_GraphDebug;
    u8 old = *ptr;

    *ptr = level;
    if (level != 0) {
        void (*func)(char *, ...) = GPU_printf;
        s32 a1;
        s32 a2;
        s32 a3;
        char *fmt;

        a1 = *ptr;
        a2 = g_GraphType;
        a3 = g_GraphReverse;
        fmt = g_FmtGpuSetGraphDebug;
        func(fmt, a1, a2, a3);
    }
    return old;
}
