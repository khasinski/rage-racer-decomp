#include "common.h"
#include "psyq/gpu.h"

extern GpuCallbacks *D_800941E0;

u32 func_80066594(void) {
    u32 ret;

    ret = D_800941E0->drawSyncStatus();
    return ret >> 31;
}
