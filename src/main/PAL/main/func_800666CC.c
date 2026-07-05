#include "common.h"

void SetDrawMask(u8 *arg0, s32 arg1, u32 arg2) asm("func_800666CC");

void SetDrawMask(u8 *arg0, s32 arg1, u32 arg2) {
    u32 cmd;

    arg0[3] = 2;
    cmd = 0xE6000000;
    if (arg1 != 0) {
        cmd = 0xE6000002;
    }
    *(u32 *)(arg0 + 4) = cmd | (arg2 != 0);
    *(u32 *)(arg0 + 8) = 0;
}
