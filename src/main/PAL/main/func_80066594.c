#include "common.h"
#include "psyq/gpu.h"

typedef struct GpuFuncs {
    u8 pad[0x38];
    s32 (*drawSync)(void);
} GpuFuncs;

extern GpuFuncs *g_GpuFuncs asm("D_800941E0");

u32 func_80066594(void) {
    register u32 ret asm("$2");

    ret = g_GpuFuncs->drawSync();
    asm("srl %0,%0,31" : "=r"(ret) : "0"(ret));
    return ret;
}
