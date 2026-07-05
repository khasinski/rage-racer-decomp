#include "common.h"
#include "psyq/gpu.h"

typedef struct LoadImageGpuFuncs {
    u8 pad0[0x8];
    void (*send)(u32 cmd, void *rect, s32 size, u32 data);
    u8 padC[0x20 - 0xC];
    u32 loadImage;
} LoadImageGpuFuncs;

typedef struct StoreImageGpuFuncs {
    u8 pad0[0x8];
    void (*send)(u32 cmd, void *rect, s32 size, u32 data);
    u8 padC[0x1C - 0xC];
    u32 storeImage;
} StoreImageGpuFuncs;

typedef struct MoveImageGpuFuncs {
    u8 pad0[0x8];
    s32 (*send)(u32 cmd, void *buf, s32 size, u32 data);
    u8 padC[0x18 - 0xC];
    u32 moveImage;
} MoveImageGpuFuncs;

extern LoadImageGpuFuncs *g_GpuFuncs asm("D_800941E0");
extern StoreImageGpuFuncs *g_GpuFuncsStore asm("D_800941E0");
extern MoveImageGpuFuncs *g_GpuFuncsMove asm("D_800941E0");
extern u32 D_80094298;
extern u32 D_8009429C;
extern u32 D_800942A0;

void CheckPrim(char *arg0, void *arg1) asm("func_80065968");
extern char D_80013578[];
extern char D_80013584[];
extern char D_80013590[];

void LoadImage(Rect *arg0, void *arg1) asm("func_80065B24");
void StoreImage(Rect *arg0, void *arg1) asm("func_80065B88");
s32 MoveImage(GpuRectPacked *arg0, u32 arg1, u32 arg2) asm("func_80065BEC");

void LoadImage(Rect *arg0, void *arg1) {
    CheckPrim(D_80013578, arg0);
    g_GpuFuncs->send(g_GpuFuncs->loadImage, arg0, 8, (u32)arg1);
}

void StoreImage(Rect *arg0, void *arg1) {
    CheckPrim(D_80013584, arg0);
    g_GpuFuncsStore->send(g_GpuFuncsStore->storeImage, arg0, 8, (u32)arg1);
}

s32 MoveImage(GpuRectPacked *arg0, u32 arg1, u32 arg2) {
    CheckPrim(D_80013590, arg0);
    if (arg0->w == 0) {
        return -1;
    }
    if (arg0->h != 0) {
        register u32 *buf asm("$5");
        register MoveImageGpuFuncs *gpu asm("$3");
        register u32 xy asm("$4");
        register s32 size asm("$6");
        register s32 data asm("$7");
        register u32 packed asm("$2");
        register u32 low asm("$3");
        u32 wh;

        packed = arg2 << 0x10;
        low = arg1 & 0xFFFF;
        packed |= low;
        asm("" : "=r"(packed) : "0"(packed));
        buf = &D_80094298;
        xy = arg0->xy;
        gpu = g_GpuFuncsMove;
        size = 0x14;
        D_8009429C = packed;
        *buf = xy;
        wh = *(u32 *)&arg0->w;
        data = 0;
        D_800942A0 = wh;
        asm("" : : "r"(wh) : "memory");
        return gpu->send(gpu->moveImage, buf - 2, size, data);
    }
    return -1;
}
