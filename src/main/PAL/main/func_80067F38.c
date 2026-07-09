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

void func_8001674C(char *fmt, ...);

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
    func_8001674C(D_8001362C, (pending - gpuTail) & 0x3F, *gp1ForLog, *D_800942C8, *D_800942C0);
    dc = D_800942DC;
    asm("" : "=r"(dc) : "0"(dc));
    func_8001674C(D_80013660, *dc, D_800942E0, D_800942E4);

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
