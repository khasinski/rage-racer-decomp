#include "common.h"
#include "psyq/kernel.h"

extern volatile u32 *g_GpuGp1 asm("D_800942BC");
extern volatile u32 *D_800942C8;
extern volatile u32 *D_800942D8;
extern s32 D_800942EC;
extern volatile s32 D_800942F0;
extern s32 D_800942FC;
extern u8 g_GpuGp1Mirror[] asm("D_8009B9F0");
extern u8 D_801E5024[];

void func_80068180(u8 *dst, u8 value, s32 count);
s32 func_800680A4(u32 arg0);

s32 func_80067C80(u32 mode) {
    s32 intrMask;
    s32 state;

    intrMask = SetIntrMask(0);
    D_800942F0 = 0;
    state = D_800942F0;
    D_800942FC = intrMask;
    D_800942EC = state;
    asm("" ::: "memory");

    switch (mode & 7) {
    case 0:
        *D_800942C8 = 0x401;
        *D_800942D8 |= 0x800;
        *g_GpuGp1 = 0;
        func_80068180(g_GpuGp1Mirror, 0, 0x100);
        func_80068180(D_801E5024, 0, 0x1800);
        break;
    case 1:
        *D_800942C8 = 0x401;
        *D_800942D8 |= 0x800;
        *g_GpuGp1 = 0x02000000;
        *g_GpuGp1 = 0x01000000;
        break;
    }

    SetIntrMask(D_800942FC);

    if ((mode & 7) == 0) {
        return func_800680A4(mode);
    }

    return 0;
}

asm(".globl func_80067DB4\nfunc_80067DB4 = func_80067C80 + 0x134");
