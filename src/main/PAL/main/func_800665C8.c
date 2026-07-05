#include "common.h"

u32 Gpu_BuildTexWindowCmd(void *arg0) asm("func_80066C2C");

void SetTexWindow(u8 *arg0, void *arg1) asm("func_800665C8");

void SetTexWindow(u8 *arg0, void *arg1) {
    arg0[3] = 2;
    *(u32 *)(arg0 + 4) = Gpu_BuildTexWindowCmd(arg1);
    *(u32 *)(arg0 + 8) = 0;
}
