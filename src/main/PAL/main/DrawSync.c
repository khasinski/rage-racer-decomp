#include "common.h"
#include "psyq/gpu.h"

typedef struct GpuFuncs {
    u8 pad0[0x3C];
    void (*drawSync)(s32 mode);
} GpuFuncs;

extern GpuFuncs *g_GpuFuncs asm("D_800941E0");
extern u8 g_GraphDebug asm("D_800941EA");
extern void (*GPU_printf)(char *, ...) asm("D_800941E4");
extern char D_80013534[];

void DrawSync(s32 arg0) asm("func_800658FC");

void DrawSync(s32 arg0) {
    if (g_GraphDebug >= 2) {
        GPU_printf(D_80013534, arg0);
    }
    g_GpuFuncs->drawSync(arg0);
}
