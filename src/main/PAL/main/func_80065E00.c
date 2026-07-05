#include "common.h"

typedef struct GpuFuncs {
    u8 pad0[0x14];
    void (*putDispEnv)(void *env, s32 mode);
    u8 pad18[0x3C - 0x18];
    void (*drawSync)(s32 mode);
} GpuFuncs;

extern GpuFuncs *g_GpuFuncs asm("D_800941E0");

void PutDispEnv(u8 *arg0) asm("func_80065E00");

void PutDispEnv(u8 *arg0) {
    u32 mode = arg0[3];

    g_GpuFuncs->drawSync(0);
    g_GpuFuncs->putDispEnv(arg0 + 4, mode);
}
