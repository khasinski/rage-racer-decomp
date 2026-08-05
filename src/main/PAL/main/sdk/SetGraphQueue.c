#include "common.h"
#include "psyq/gpu.h"
#include "psyq/kernel.h"

extern GpuCallbacks *g_GpuFuncs asm("D_800941E0");
extern void (*GPU_printf)(char *, ...) asm("D_800941E4");
extern u_char g_GraphQueue asm("D_800941E9");
extern u_char g_GraphDebug asm("D_800941EA");
extern char D_800134F0[];

/* GP1 queue mode; own trace string D_800134F0 is "SetGrapQue(%d)...". */
long SetGraphQueue(long mode);
long SetGraphQueue(long arg0) {
    u_char *queue = &g_GraphQueue;
    long newQueue = arg0;
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

extern u_char g_GraphType asm("D_800941E8");
extern u_char g_GraphDebug asm("D_800941EA");

long GetGraphType(void) asm("func_800657E4");
long GetGraphType(void) {
    return g_GraphType;
}

long GetGraphDebug(void);
long GetGraphDebug(void) {
    return g_GraphDebug;
}

extern char D_80013504[];
extern void (*D_800941E4)(char *, ...);
extern u_long g_DrawSyncCallback asm("D_800941F4");

u_long DrawSyncCallback(u_long arg0);
u_long DrawSyncCallback(u_long arg0) {
    u_long ret;

    if (g_GraphDebug >= 2) {
        D_800941E4(D_80013504, arg0);
    }

    ret = g_DrawSyncCallback;
    g_DrawSyncCallback = arg0;
    return ret;
}

extern char D_80013520[];

void func_80068180(u_char *dst, long value, long count);

void SetDispMask(long arg0);
void SetDispMask(long arg0) {
    u_char *debug = &g_GraphDebug;
    long enable = arg0;
    u_char *clearPtr;
    GpuCallbacks *gpu;
    long mask;

    if (*debug >= 2) {
        GPU_printf(D_80013520, enable);
    }

    clearPtr = debug + 0x6A;
    if (enable == 0) {
        func_80068180(clearPtr, -1, 0x14);
    }

    mask = 0x3000000;
    asm("" : "=r"(mask) : "0"(mask));
    gpu = g_GpuFuncs;
    if (enable == 0) {
        mask |= 1;
    } else {
        mask = 0x3000000;
    }

    gpu->submit(mask);
}

extern GpuCallbacks *g_GpuFuncs asm("D_800941E0");
extern char D_80013534[];

void DrawSync(long arg0) asm("func_800658FC");
void DrawSync(long arg0) {
    if (g_GraphDebug >= 2) {
        D_800941E4(D_80013534, arg0);
    }
    g_GpuFuncs->drawSync(arg0);
}

extern short g_VramWidth asm("D_800941EC");
extern short g_VramHeight asm("D_800941EE");
extern char D_80013548[];
extern char D_80013554[];
extern char D_80013568[];

void CheckPrim(char *arg0, Rect *rect) asm("func_80065968");
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

extern GpuCallbacks *g_GpuFuncs asm("D_800941E0");
extern char D_8001356C[];

void func_80065968(char *arg0, void *arg1);

void ClearImage(void *rect, u_char r, u_char g, u_char b);
void ClearImage(void *rect, u_char r, u_char g, u_char b) {
    func_80065968(D_8001356C, rect);
    g_GpuFuncs->send(g_GpuFuncs->cmd0C, rect, 8, (b << 16) | (g << 8) | r);
}
