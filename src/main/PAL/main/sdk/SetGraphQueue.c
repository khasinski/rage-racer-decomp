#include "common.h"
#include "psyq/gpu.h"
#include "psyq/kernel.h"

extern GpuCallbacks *g_GpuFuncs;
extern u_char g_GraphQueue;
extern u_char g_GraphDebug;
extern char D_800134F0[];

long SetGraphQueue(long mode) {
    u_char *queue = &g_GraphQueue;
    long newQueue = mode;
    long oldQueue;

    oldQueue = *queue;
    if (g_GraphDebug >= 2) {
        GPU_printf(D_800134F0, newQueue);
    }

    if (newQueue != *queue) {
        g_GpuFuncs->resetGraph(1);
        *queue = newQueue;
        DMACallback(2, 0);
    }

    return oldQueue;
}

extern u_char g_GraphType;

long GetGraphType(void) {
    return g_GraphType;
}

long GetGraphDebug(void) {
    return g_GraphDebug;
}

extern char D_80013504[];
extern u_long g_DrawSyncCallback;

u_long DrawSyncCallback(u_long callback) {
    u_long ret;

    if (g_GraphDebug >= 2) {
        GPU_printf(D_80013504, callback);
    }

    ret = g_DrawSyncCallback;
    g_DrawSyncCallback = callback;
    return ret;
}

extern char D_80013520[];

void SetDispMask(long mask) {
    u_char *debug = &g_GraphDebug;
    long enable = mask;
    u_char *clearPtr;
    GpuCallbacks *gpu;

    if (*debug >= 2) {
        GPU_printf(D_80013520, enable);
    }

    clearPtr = debug + 0x6A;
    if (enable == 0) {
        MemFill(clearPtr, -1, 0x14);
    }

    gpu = g_GpuFuncs;
    gpu->submit(enable ? 0x3000000 : 0x3000001);
}

extern char D_80013534[];

void DrawSync(long mode) {
    if (g_GraphDebug >= 2) {
        GPU_printf(D_80013534, mode);
    }
    g_GpuFuncs->drawSync(mode);
}

extern short g_VramWidth;
extern short g_VramHeight;
extern char D_80013548[];
extern char D_80013554[];
extern char D_80013568[];

void CheckPrim(char *arg0, Rect *rect) {
    switch (g_GraphDebug) {
    case 1: {
        long w = rect->w;
        long maxX = g_VramWidth;
        long x;
        long y;
        long h;
        long maxY;

        if (maxX < w) {
        } else {
        x = rect->x;
        if (maxX < w + x) {
        } else {
        y = rect->y;
        maxY = g_VramHeight;
        if (maxY < y) {
        } else {
        h = rect->h;
        if (maxY < y + h) {
        } else {
        if (w <= 0) {
        } else {
        if (x < 0) {
        } else {
        if (y < 0) {
        } else {
        if (h > 0) {
            return;
        }
        }
        }
        }
        }
        }
        }
        }
        GPU_printf(D_80013548, arg0);
        GPU_printf(D_80013554, rect->x, rect->y, rect->w, rect->h);
        return;
    }
    case 2:
        GPU_printf(D_80013568, arg0);
        GPU_printf(D_80013554, rect->x, rect->y, rect->w, rect->h);
        return;
    }
}

extern char D_8001356C[];

void ClearImage(void *rect, u_char r, u_char g, u_char b) {
    CheckPrim(D_8001356C, rect);
    g_GpuFuncs->send(g_GpuFuncs->cmd0C, rect, 8, (b << 16) | (g << 8) | r);
}
