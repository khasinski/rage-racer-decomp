#include "common.h"
#include "psyq/gpu.h"
#include "psyq/kernel.h"

extern GpuCallbacks *g_GpuCallbacks asm("D_800941E0");
extern void (*GPU_printf)(char *, ...) asm("D_800941E4");
extern u_char g_GraphQueue asm("D_800941E9");
extern u_char g_GraphDebug asm("D_800941EA");
extern char D_800134F0[];

/* GP1 queue mode; own trace string D_800134F0 is "SetGrapQue(%d)...". */
long SetGraphQueue(long mode) asm("func_80065738");
long SetGraphQueue(long arg0) {
    register long newQueue asm("$16") = arg0;
    register u_char *queue asm("$17") = &g_GraphQueue;
    register long oldQueue asm("$18");

    oldQueue = *queue;
    if (g_GraphDebug >= 2) {
        GPU_printf(D_800134F0, newQueue);
    }

    if (newQueue != *queue) {
        g_GpuCallbacks->resetGraph(1);
        *queue = newQueue;
        DMACallback(2, 0);
    }

    return oldQueue;
}

extern u_char D_800941E8;
extern u_char D_800941EA;

long GetGraphType(void) asm("func_800657E4");
long GetGraphType(void) {
    return D_800941E8;
}

long GetGraphDebug(void) asm("func_800657F4");
long GetGraphDebug(void) {
    return D_800941EA;
}

extern char D_80013504[];
extern void (*D_800941E4)(char *, ...);
extern u_long D_800941F4;

u_long DrawSyncCallback(u_long arg0) asm("func_80065804");
u_long DrawSyncCallback(u_long arg0) {
    u_long ret;

    if (D_800941EA >= 2) {
        D_800941E4(D_80013504, arg0);
    }

    ret = D_800941F4;
    D_800941F4 = arg0;
    return ret;
}

extern char D_80013520[];

void func_80068180(u_char *dst, long value, long count);

void SetDispMask(long arg0) asm("func_80065860");
void SetDispMask(long arg0) {
    register long enable asm("$16") = arg0;
    register u_char *debug asm("$17") = &g_GraphDebug;
    register u_char *clearPtr asm("$4");
    register GpuCallbacks *gpu asm("$2");
    register long mask asm("$4");

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

extern GpuCallbacks *D_800941E0;
extern char D_80013534[];

void DrawSync(long arg0) asm("func_800658FC");
void DrawSync(long arg0) {
    if (D_800941EA >= 2) {
        D_800941E4(D_80013534, arg0);
    }
    D_800941E0->drawSync(arg0);
}

extern short D_800941EC;
extern short D_800941EE;
extern char D_80013548[];
extern char D_80013554[];
extern char D_80013568[];

void CheckPrim(char *arg0, Rect *rect) asm("func_80065968");
void CheckPrim(char *arg0, Rect *rect) {
    switch (g_GraphDebug) {
    case 1: {
        register long w asm("$5") = rect->w;
        long maxX = D_800941EC;
        register long x asm("$7");
        register long y asm("$3");
        register long h asm("$6");
        long maxY;

        if (maxX < w) {
            goto bad;
        }
        x = rect->x;
        if (maxX < w + x) {
            goto bad;
        }
        y = rect->y;
        maxY = D_800941EE;
        if (maxY < y) {
            goto bad;
        }
        h = rect->h;
        if (maxY < y + h) {
            goto bad;
        }
        if (w <= 0) {
            goto bad;
        }
        if (x < 0) {
            goto bad;
        }
        if (y < 0) {
            goto bad;
        }
        if (h > 0) {
            return;
        }
bad:
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

void ClearImage(void *arg0, u_long arg1, u_long arg2, u_long arg3) asm("func_80065A90");
void ClearImage(void *arg0, u_long arg1, u_long arg2, u_long arg3) {
    register void *rect asm("s3") = arg0;
    register u_long b asm("s2") = arg1;
    register u_long g asm("s1") = arg2;
    register u_long r asm("s0") = arg3;

    func_80065968(D_8001356C, rect);

    {
        register void *rectArg asm("$5") = rect;

        asm volatile(
            "andi $16,$16,0x00ff # maspsx-keep\n\t"
            "sll $16,$16,16 # maspsx-keep\n\t"
            "andi $17,$17,0x00ff # maspsx-keep\n\t"
            "sll $17,$17,8 # maspsx-keep\n\t"
            "or $16,$16,$17 # maspsx-keep\n\t"
            "andi $18,$18,0x00ff # maspsx-keep"
            :
            : "r"(g), "r"(rectArg));

        g_GpuFuncs->send(g_GpuFuncs->cmd0C, rectArg, 8, r | b);
    }
}
