#include "common.h"

typedef struct GpuFuncs {
    u8 pad0[0x8];
    void (*send)(u32 cmd, void *rect, s32 size, u32 data);
    u8 padC[0x1C - 0xC];
    u32 storeImage;
} GpuFuncs;

extern GpuFuncs *g_GpuFuncs asm("D_800941E0");

void CheckPrim(char *arg0, void *arg1) asm("func_80065968");
extern char D_80013584[];

void StoreImage(void *arg0, void *arg1) asm("func_80065B88");

void StoreImage(void *arg0, void *arg1) {
    CheckPrim(D_80013584, arg0);
    g_GpuFuncs->send(g_GpuFuncs->storeImage, arg0, 8, (u32)arg1);
}
