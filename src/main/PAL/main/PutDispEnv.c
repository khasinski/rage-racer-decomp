#include "common.h"
#include "psyq/gpu.h"

typedef struct GpuFuncs {
    u8 pad0[0x14];
    void (*putDispEnv)(void *env, s32 mode);
    u8 pad18[0x3C - 0x18];
    void (*drawSync)(s32 mode);
} GpuFuncs;

extern GpuFuncs *D_800941E0;

void func_80065E00(u8 *arg0) {
    u32 mode = arg0[3];

    D_800941E0->drawSync(0);
    D_800941E0->putDispEnv(arg0 + 4, mode);
}
