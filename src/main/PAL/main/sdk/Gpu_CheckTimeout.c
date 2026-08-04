#include "common.h"
#include "psyq/kernel.h"

extern volatile u_long *g_GpuGp1 asm("D_800942BC");
extern volatile u_long *g_GpuDmaMadr asm("D_800942C0");
extern volatile u_long *g_GpuDmaChcr asm("D_800942C8");
extern volatile u_long *g_GpuDpcr asm("D_800942D8");
extern long g_GpuLastCb[] asm("D_800942DC");
extern long g_GpuLastCbArg asm("D_800942E0");
extern long g_GpuLastCbData asm("D_800942E4");
extern long g_GpuQueueWriteIdx asm("D_800942EC");
extern volatile long g_GpuQueueReadIdx asm("D_800942F0");
extern long g_GpuResetIntrMask asm("D_800942FC");
extern long g_GpuTimeoutDeadline asm("D_80094300");
extern long g_GpuTimeoutPolls asm("D_80094304");
extern char D_8001362C[];
extern char D_80013660[];


long Gpu_CheckTimeout(void) asm("func_80067F38");
long Gpu_CheckTimeout(void) {
    long intrMask;
    long state;
    long result;
    long *dc;
    register volatile u_long *gp1ForLog asm("$3");
    register long pending asm("$5");
    long gpuTail;

    switch (0) { default:
    if (g_GpuTimeoutDeadline >= VSync(-1)) {
        state = g_GpuTimeoutPolls++;
        if (state <= 0xF0000) {
            break;
        }
    }

    gp1ForLog = g_GpuGp1;
    asm("" : "=r"(gp1ForLog) : "0"(gp1ForLog));
    (void)*gp1ForLog;
    pending = g_GpuQueueWriteIdx;
    gpuTail = g_GpuQueueReadIdx;
    DebugPrintf(D_8001362C, (pending - gpuTail) & 0x3F, *gp1ForLog, *g_GpuDmaChcr, *g_GpuDmaMadr);
    dc = g_GpuLastCb;
    asm("" : "=r"(dc) : "0"(dc));
    DebugPrintf(D_80013660, *dc, g_GpuLastCbArg, g_GpuLastCbData);

    intrMask = SetIntrMask(0);
    g_GpuQueueReadIdx = 0;
    state = g_GpuQueueReadIdx;
    g_GpuResetIntrMask = intrMask;
    g_GpuQueueWriteIdx = state;

    *g_GpuDmaChcr = 0x401;
    *g_GpuDpcr |= 0x800;
    *g_GpuGp1 = 0x02000000;
    *g_GpuGp1 = 0x01000000;
    SetIntrMask(g_GpuResetIntrMask);
    result = -1;
    return result;

    }
    result = 0;

    return result;
}

extern volatile u_long *g_GpuGp0 asm("D_800942B8");
extern volatile u_long *g_GpuGp1 asm("D_800942BC");

/* GPU-type probe: GP1(10h) info word 7, then a texture-window write-back
 * test. Returns 0..4; ResetGraph stores it as the graph type. */
long Gpu_ProbeType(u_long mode);
long Gpu_ProbeType(u_long arg0) {
    volatile u_long *gp0;
    u_long status;

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

void MemFill(u_char *dst, u_char value, long count) asm("func_80068180");
void MemFill(u_char *dst, u_char value, long count) {
    volatile long unused;
    long i = count - 1;

    if (count != 0) {
        long end = -1;

        do {
            *dst = value;
            i--;
            dst++;
        } while (i != end);
    }
}

u_long GPU_cw[4] asm("func_800681AC") __attribute__((section(".text"))) = {
    0x240A00A0,
    0x01400008,
    0x24090049,
    0,
};

u_char *MemCopy(u_char *dst, u_char *src, long count) asm("func_800681BC");
u_char *MemCopy(u_char *dst, u_char *src, long count) {
    u_char *ret;

    if (dst == 0) {
        return 0;
    }

    ret = dst;
    if (count > 0) {
        do {
            *dst = *src;
            src++;
            count--;
            dst++;
        } while (count > 0);
    }

    return ret;
}
