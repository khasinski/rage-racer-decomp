#include "common.h"
#include "psyq/kernel.h"

extern volatile u32 *g_GpuGp1 asm("D_800942BC");
extern volatile u32 *D_800942C0;
extern volatile u32 *D_800942C8;
extern volatile u32 *D_800942D8;
extern s32 D_800942DC[];
extern s32 D_800942E0;
extern s32 D_800942E4;
extern s32 D_800942EC;
extern volatile s32 D_800942F0;
extern s32 D_800942FC;
extern s32 D_80094300;
extern s32 D_80094304;
extern char D_8001362C[];
extern char D_80013660[];


s32 func_80067F38(void) {
    s32 intrMask;
    register s32 state asm("$3");
    register s32 result asm("$2");
    register s32 *dc asm("$2");
    register volatile u32 *gp1ForLog asm("$3");
    register s32 pending asm("$5");
    register s32 gpuTail asm("$8");

    if (D_80094300 >= VSync(-1)) {
        state = D_80094304++;
        if (state <= 0xF0000) {
            goto return_zero;
        }
    }

    gp1ForLog = g_GpuGp1;
    asm("" : "=r"(gp1ForLog) : "0"(gp1ForLog));
    (void)*gp1ForLog;
    pending = D_800942EC;
    gpuTail = D_800942F0;
    GameDebugPrintf(D_8001362C, (pending - gpuTail) & 0x3F, *gp1ForLog, *D_800942C8, *D_800942C0);
    dc = D_800942DC;
    asm("" : "=r"(dc) : "0"(dc));
    GameDebugPrintf(D_80013660, *dc, D_800942E0, D_800942E4);

    intrMask = SetIntrMask(0);
    D_800942F0 = 0;
    state = D_800942F0;
    D_800942FC = intrMask;
    D_800942EC = state;

    *D_800942C8 = 0x401;
    *D_800942D8 |= 0x800;
    *g_GpuGp1 = 0x02000000;
    *g_GpuGp1 = 0x01000000;
    SetIntrMask(D_800942FC);
    result = -1;
    goto done;

return_zero:
    result = 0;

done:
    return result;
}

extern volatile u32 *D_800942B8;
extern volatile u32 *D_800942BC;

s32 func_800680A4(u32 arg0) {
    volatile u32 *gp0;
    u32 status;

    *D_800942BC = 0x10000007;
    gp0 = D_800942B8;
    status = *gp0 & 0xFFFFFF;

    if (status != 2) {
        *gp0 = (*D_800942BC & 0x3FFF) | 0xE1001000;
        *D_800942B8;

        if ((*D_800942BC & 0x1000) == 0) {
            return 0;
        }

        if ((arg0 & 8) == 0) {
            return 1;
        }

        *D_800942BC = 0x20000504;
        return 2;
    }

    if ((arg0 & 8) == 0) {
        return 3;
    }

    *D_800942BC = 0x9000001;
    return 4;
}

void MemFill(u8 *dst, u8 value, s32 count) asm("func_80068180");
void MemFill(u8 *dst, u8 value, s32 count) {
    volatile s32 unused;
    s32 i = count - 1;

    if (count != 0) {
        s32 end = -1;

        do {
            *dst = value;
            i--;
            dst++;
        } while (i != end);
    }
}

u32 func_800681AC[4] __attribute__((section(".text"))) = {
    0x240A00A0,
    0x01400008,
    0x24090049,
    0,
};

u8 *MemCopy(u8 *dst, u8 *src, s32 count) asm("func_800681BC");
u8 *MemCopy(u8 *dst, u8 *src, s32 count) {
    u8 *ret;

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
