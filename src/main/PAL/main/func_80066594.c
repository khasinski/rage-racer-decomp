#include "common.h"
#include "psyq/gpu.h"

typedef struct GpuFuncs {
    u8 pad[0x38];
    s32 (*drawSync)(void);
} GpuFuncs;

extern GpuFuncs *D_800941E0;

u32 func_80066594(void) {
    u32 ret;

    ret = D_800941E0->drawSync();
    return ret >> 31;
}
