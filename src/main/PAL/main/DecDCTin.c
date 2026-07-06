#include "common.h"

void func_800641D0(volatile u32 *arg0, s32 arg1);

void func_80063FB0(volatile u32 *arg0, s32 arg1) {
    u32 mask;

    if (arg1 & 1) {
        arg0[0] &= 0xF7FFFFFF;
    } else {
        arg0[0] |= 0x08000000;
    }

    mask = 0x02000000;
    if (arg1 & 2) {
        arg0[0] |= mask;
    } else {
        arg0[0] &= 0xFDFFFFFF;
    }

    func_800641D0(arg0, *(u16 *)arg0);
}
