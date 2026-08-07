#include "common.h"
#include "psyq/gpu.h"

extern GpuCallbacks *g_GpuFuncs;
extern GpuCallbacks *g_GpuFuncs;
extern GpuCallbacks *g_GpuFuncs;
/*
 * The five words at 0x80094290 are one GPU "move image" primitive: tag, the
 * 0xE1-class command word, then source xy, destination xy and the packed
 * width/height.  MoveImage fills the last three and sends the whole packet.
 */
typedef struct {
    u_long tag;
    u_long code;
    u_long src;
    u_long dst;
    u_long wh;
} GpuMovePacket;

extern GpuMovePacket g_MoveImagePacket;

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
    if (arg0->w == 0 || arg0->h == 0) {
        return -1;
    }

    g_MoveImagePacket.src = arg0->xy;
    g_MoveImagePacket.dst = (arg2 << 0x10) | (arg1 & 0xFFFF);
    g_MoveImagePacket.wh = *(u_long *)&arg0->w;
    return g_GpuFuncs->send(g_GpuFuncs->sendList, &g_MoveImagePacket,
                            sizeof(g_MoveImagePacket), 0);
}

extern u_char g_GraphDebug;
extern char D_8001359C[];
extern char D_800135B4[];
extern u_long g_OtagTerminator;

extern GpuCallbacks *g_GpuFuncs;

void *ClearOTag(u_long *arg0, long count) {
    long remaining = count;

    if (g_GraphDebug >= 2) {
        void (*debug)(char *, ...) = GPU_printf;

        debug(D_8001359C, arg0, remaining);
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
            next = arg0 + 1;
            ((u_char *)arg0)[3] = 0;
            tag = *arg0;
            low = (u_long)next & mask;
            tag &= hiMask;
            tag |= low;
            *arg0 = tag;
            arg0 = next;
        } while (remaining != 0);
    }

    *arg0 = (u_long)&g_OtagTerminator & 0xFFFFFF;
    return arg0;
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
