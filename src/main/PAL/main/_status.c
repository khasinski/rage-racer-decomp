#include "common.h"

extern u32 *g_GpuGp1 asm("D_800942BC");

u32 _status(void) asm("func_80066D6C");

u32 _status(void) {
    return *g_GpuGp1;
}
