#include "common.h"
#include "psyq/gpu.h"

extern GpuCallbacks *D_800941E0;

void func_80065E00(u8 *arg0) {
    u32 mode = arg0[3];

    D_800941E0->drawSync(0);
    D_800941E0->putDispEnv(arg0 + 4, mode);
}
