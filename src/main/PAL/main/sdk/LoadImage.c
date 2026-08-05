#include "common.h"
#include "psyq/gpu.h"

extern GpuCallbacks *g_GpuFuncs;
extern GpuCallbacks *g_GpuFuncs;
extern GpuCallbacks *g_GpuFuncs;
extern u_long g_MoveImageSrc;
extern u_long g_MoveImageDst;
extern u_long g_MoveImageSize;

void CheckPrim(char *arg0, void *arg1);
extern char D_80013578[];
extern char D_80013584[];
extern char D_80013590[];

void LoadImage(Rect *arg0, void *arg1) {
    CheckPrim(D_80013578, arg0);
    g_GpuFuncs->send(g_GpuFuncs->loadImage, arg0, 8, (u_long)arg1);
}

void StoreImage(Rect *arg0, void *arg1) {
    CheckPrim(D_80013584, arg0);
    g_GpuFuncs->send(g_GpuFuncs->storeImage, arg0, 8, (u_long)arg1);
}

long MoveImage(GpuRectPacked *arg0, u_long arg1, u_long arg2) {
    CheckPrim(D_80013590, arg0);
    if (arg0->w == 0) {
        return -1;
    }
    if (arg0->h != 0) {
        u_long *buf;
        register GpuCallbacks *gpu asm("$3");
        register u_long xy asm("$4");
        long size;
        long data;
        u_long packed;
        register u_long low asm("$3");
        u_long wh;

        packed = arg2 << 0x10;
        low = arg1 & 0xFFFF;
        packed |= low;
        buf = &g_MoveImageSrc;
        xy = arg0->xy;
        gpu = g_GpuFuncs;
        size = 0x14;
        g_MoveImageDst = packed;
        *buf = xy;
        wh = *(u_long *)&arg0->w;
        data = 0;
        g_MoveImageSize = wh;
        asm("" : : "r"(wh) : "memory");
        return gpu->send(gpu->sendList, buf - 2, size, data);
    }
    return -1;
}

extern u_char g_GraphDebug;
extern char D_8001359C[];
extern char D_800135B4[];
extern u_long g_OtagTerminator;

extern GpuCallbacks *g_GpuFuncs;

void *ClearOTag(u_long *arg0, long count) {
    register u_long *ptr asm("$16") = arg0;
    long remaining = count;

    if (g_GraphDebug >= 2) {
        void (*debug)(char *, ...) = GPU_printf;

        debug(D_8001359C, ptr, remaining);
    }

    remaining--;
    if (remaining != 0) {
        u_long mask = 0xFFFFFF;
        u_long hiMask = 0xFF000000;

        do {
            u_long *next;
            u_long tag;
            u_long low;

            remaining--;
            next = ptr + 1;
            ((u_char *)ptr)[3] = 0;
            tag = *ptr;
            low = (u_long)next & mask;
            tag &= hiMask;
            tag |= low;
            *ptr = tag;
            ptr = next;
        } while (remaining != 0);
    }

    *ptr = (u_long)&g_OtagTerminator & 0xFFFFFF;
    return ptr;
}

void *ClearOTagR(u_long *arg0, long arg1) {
    if (g_GraphDebug >= 2) {
        GPU_printf(D_800135B4, arg0, arg1);
    }

    g_GpuFuncs->clearOTag(arg0, arg1);

    {
        u_long mask = 0xFFFFFF;
        register u_long *ret asm("$2") = arg0;
        u_long next;

        asm("" : "=r"(ret), "=r"(mask) : "0"(ret), "1"(mask));
        next = (u_long)&g_OtagTerminator;
        asm("" : "=r"(next) : "0"(next));
        next &= mask;
        *ret = next;
        return ret;
    }
}

extern GpuCallbacks *g_GpuFuncs;

void DrawPrim(u_char *arg0) {
    u_long mode = arg0[3];

    g_GpuFuncs->drawSync(0);
    g_GpuFuncs->writeGp0Words(arg0 + 4, mode);
}
