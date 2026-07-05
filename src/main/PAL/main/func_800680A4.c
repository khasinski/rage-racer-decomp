#include "common.h"

extern volatile u32 *g_GpuGp0 asm("D_800942B8");
extern volatile u32 *g_GpuGp1 asm("D_800942BC");

s32 func_800680A4(u32 arg0) {
    volatile u32 *gp0;
    u32 status;

    *g_GpuGp1 = 0x10000007;
    gp0 = g_GpuGp0;
    status = *gp0 & 0xFFFFFF;

    if (status != 2) {
        *gp0 = (*g_GpuGp1 & 0x3FFF) | 0xE1001000;
        *g_GpuGp0;

        if ((*g_GpuGp1 & 0x1000) == 0) {
            return 0;
        }

        if ((arg0 & 8) == 0) {
            return 1;
        }

        *g_GpuGp1 = 0x20000504;
        return 2;
    }

    if ((arg0 & 8) == 0) {
        return 3;
    }

    *g_GpuGp1 = 0x9000001;
    return 4;
}
