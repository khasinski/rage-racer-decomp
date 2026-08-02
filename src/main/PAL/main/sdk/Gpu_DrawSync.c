#include <sys/types.h>

#include "common.h"

void Gpu_ArmTimeout(void) asm("func_80067F04");

extern volatile u_long *g_GpuGp1 asm("D_800942BC");
extern volatile u_long *g_GpuDmaChcr asm("D_800942C8");
extern long g_GpuQueueWriteIdx asm("D_800942EC");
extern long g_GpuQueueReadIdx asm("D_800942F0");

void func_80067984(void);
long Gpu_CheckTimeout(void) asm("func_80067F38");

/* Driver-table slot +0x3C, the body of DrawSync: mode 0 blocks until the
 * queue is empty and the GPU idle, any other mode returns the number of
 * queue entries still outstanding. */
long Gpu_DrawSync(long mode) asm("func_80067DBC");
long Gpu_DrawSync(long arg0) {
    long pending;

    if (arg0 == 0) {
        Gpu_ArmTimeout();
        switch (0) { default:
        break;

retry:
        func_80067984();
        if (Gpu_CheckTimeout() != 0) {
            return -1;
        }

        }
        if (!(g_GpuQueueWriteIdx == g_GpuQueueReadIdx)) {
        goto retry;

waitReady:
        if (Gpu_CheckTimeout() != 0) {
            return -1;
        }

        }
        if (!(*g_GpuDmaChcr & 0x01000000)) {

        if (*g_GpuGp1 & 0x04000000) {
            return 0;
        }
        }
        goto waitReady;
    }

    pending = (g_GpuQueueWriteIdx - g_GpuQueueReadIdx) & 0x3F;
    switch (0) { default:
    if (pending != 0) {
        func_80067984();
    }

    if (*g_GpuDmaChcr & 0x01000000) {
    } else {

    if (*g_GpuGp1 & 0x04000000) {
        break;
    }

    }
    if (pending == 0) {
        return 1;
    }
    return pending;

    }
    return pending;
}

extern long g_GpuTimeoutDeadline asm("D_80094300");
extern long g_GpuTimeoutPolls asm("D_80094304");
long VSync(long mode) asm("func_8006DD30");
void Gpu_ArmTimeout(void) { g_GpuTimeoutDeadline = VSync(-1) + 240; g_GpuTimeoutPolls = 0; }
