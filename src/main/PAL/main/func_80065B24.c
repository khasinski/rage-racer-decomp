#include "common.h"

typedef struct GpuFuncs {
    u8 pad0[0x8];
    void (*send)(u32 cmd, void *rect, s32 size, u32 data);
    u8 padC[0x20 - 0xC];
    u32 loadImage;
} GpuFuncs;

extern GpuFuncs *g_GpuFuncs asm("D_800941E0");

void CheckPrim(char *arg0, void *arg1) asm("func_80065968");
extern char D_80013578[];

void LoadImage(void *arg0, void *arg1) asm("func_80065B24");

void LoadImage(void *arg0, void *arg1) {
    CheckPrim(D_80013578, arg0);
    g_GpuFuncs->send(g_GpuFuncs->loadImage, arg0, 8, (u32)arg1);
}
