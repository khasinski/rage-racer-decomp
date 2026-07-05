#include "common.h"
#include "psyq/gpu.h"

typedef struct GpuFuncs {
    u8 pad0[0x8];
    void (*send)(u32 cmd, void *rect, s32 size, u32 data);
    u8 padC[0x18 - 0xC];
    u32 moveImage;
} GpuFuncs;

extern GpuFuncs *g_GpuFuncs asm("D_800941E0");
extern u8 g_GraphDebug asm("D_800941EA");
extern void (*D_800941E4)(char *, ...);
extern char D_800135CC[];

void DrawOTag(void *arg0) asm("func_80065E60");

void DrawOTag(void *arg0) {
    if (g_GraphDebug >= 2) {
        D_800941E4(D_800135CC, arg0);
    }
    g_GpuFuncs->send(g_GpuFuncs->moveImage, arg0, 0, 0);
}

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_80065E60", func_80065ED4);

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_80065E60", func_80065F98);

#if 0
void func_80065ED4(void) {
}

void func_80065F98(void) {
}
#endif
