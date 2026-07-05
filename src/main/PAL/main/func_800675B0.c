#include "common.h"

extern volatile u32 *g_GpuGp0 asm("D_800942B8");
extern volatile u32 *g_GpuGp1 asm("D_800942BC");

s32 Gpu_WriteGp0Words(u32 *src, s32 count) asm("func_800675B0");

s32 Gpu_WriteGp0Words(u32 *src, s32 count) {
    volatile s32 unused;
    s32 i = count - 1;

    *g_GpuGp1 = 0x04000000;
    if (count != 0) {
        s32 end = -1;

        do {
            *g_GpuGp0 = *src;
            src++;
            i--;
        } while (i != end);
    }
    return 0;
}
