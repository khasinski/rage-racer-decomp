#include "common.h"

extern volatile s32 D_8007FB4C;

void func_80046E00();

void func_800509C4(s32 arg0) {
    void *scratch = *(void **)0x1F800004;
    register s32 step asm("s0") = arg0;
    register s32 zero asm("$0");
    register s32 value asm("v0");
    s32 renderValue;
    s32 y0;
    s32 y1;

    if (step == 0) {
        D_8007FB4C = zero;
        return;
    }

    if (step < 0) {
        value = D_8007FB4C + step;
        D_8007FB4C = value;
        if (value < 0) {
            D_8007FB4C = 0;
        }
    }

    renderValue = D_8007FB4C;
    y0 = 0xD7;
    if (renderValue != 0) {
        y0 = (s16)(y0 - renderValue);
        y1 = (s16)(renderValue + 0xD8);
        func_80046E00(
            scratch,
            0x4C,
            y0,
            0x7C,
            y0,
            0x4C,
            y1,
            0x7C,
            y1,
            0xCC,
            0x38,
            0xFC,
            0x38,
            0xCC,
            0x50,
            0xFC,
            0x50,
            0,
            0,
            0,
            0x20F,
            1,
            0,
            0x1C);
    }

    if (step > 0) {
        value = D_8007FB4C + step;
        D_8007FB4C = value;
        __asm__ volatile("" ::: "memory");
        if (value >= 0xD) {
            __asm__ volatile("" ::: "memory");
            D_8007FB4C = 0xC;
        }
    }
}
