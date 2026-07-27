#include "common.h"
#include "psyq/gpu.h"
#include "psyq/kernel.h"

extern GpuCallbacks *g_GpuCallbacks asm("D_800941E0");
extern void (*GPU_printf)(char *, ...) asm("D_800941E4");
extern u8 g_GraphQueue asm("D_800941E9");
extern u8 g_GraphDebug asm("D_800941EA");
extern char D_800134F0[];

/* GP1 queue mode; own trace string D_800134F0 is "SetGrapQue(%d)...". */
s32 SetGraphQueue(s32 mode) asm("func_80065738");
s32 SetGraphQueue(s32 arg0) {
    register s32 newQueue asm("$16") = arg0;
    register u8 *queue asm("$17") = &g_GraphQueue;
    register s32 oldQueue asm("$18");

    oldQueue = *queue;
    if (g_GraphDebug >= 2) {
        GPU_printf(D_800134F0, newQueue);
    }

    if (newQueue != *queue) {
        g_GpuCallbacks->resetGraph(1);
        *queue = newQueue;
        ResetCallback(2, 0);
    }

    return oldQueue;
}

extern u8 D_800941E8;
extern u8 D_800941EA;

s32 GetGraphType(void) asm("func_800657E4");
s32 GetGraphType(void) {
    return D_800941E8;
}

s32 GetGraphDebug(void) asm("func_800657F4");
s32 GetGraphDebug(void) {
    return D_800941EA;
}

extern char D_80013504[];
extern void (*D_800941E4)(char *, ...);
extern u32 D_800941F4;

u32 DrawSyncCallback(u32 arg0) asm("func_80065804");
u32 DrawSyncCallback(u32 arg0) {
    u32 ret;

    if (D_800941EA >= 2) {
        D_800941E4(D_80013504, arg0);
    }

    ret = D_800941F4;
    D_800941F4 = arg0;
    return ret;
}

extern char D_80013520[];

void func_80068180(u8 *dst, s32 value, s32 count);

void SetDispMask(s32 arg0) asm("func_80065860");
void SetDispMask(s32 arg0) {
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

extern GpuCallbacks *D_800941E0;
extern char D_80013534[];

void DrawSync(s32 arg0) asm("func_800658FC");
void DrawSync(s32 arg0) {
    if (D_800941EA >= 2) {
        D_800941E4(D_80013534, arg0);
    }
    D_800941E0->drawSync(arg0);
}

extern s16 D_800941EC;
extern s16 D_800941EE;
extern char D_80013548[];
extern char D_80013554[];
extern char D_80013568[];

void CheckPrim(char *arg0, Rect *rect) asm("func_80065968");
void CheckPrim(char *arg0, Rect *rect) {
    switch (g_GraphDebug) {
    case 1: {
        register s32 w asm("$5") = rect->w;
        s32 maxX = D_800941EC;
        register s32 x asm("$7");
        register s32 y asm("$3");
        register s32 h asm("$6");
        s32 maxY;

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

void ClearImage(void *arg0, u32 arg1, u32 arg2, u32 arg3) asm("func_80065A90");
void ClearImage(void *arg0, u32 arg1, u32 arg2, u32 arg3) {
    register void *rect asm("s3") = arg0;
    register u32 b asm("s2") = arg1;
    register u32 g asm("s1") = arg2;
    register u32 r asm("s0") = arg3;

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
