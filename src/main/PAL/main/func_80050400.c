#include "common.h"

extern s32 D_8007FB40;
extern s32 D_8007FB44;
extern s32 D_8019CB0C;

void func_80046E00();

void func_80050400(s32 arg0, s32 arg1) {
    register s32 step0 asm("$16");
    register s32 step1 asm("$17");
    register void *scratch asm("$18");
    register s32 zero asm("$0");
    register s32 value asm("$2");
    register s32 offset asm("$8");
    register s32 x0 asm("$7");
    register s32 y0 asm("$6");
    register s32 render1 asm("$3");
    register s32 y1 asm("$2");
    register void *callScratch asm("$4");
    register s32 callX asm("$5");

    scratch = *(void **)0x1F800004;
    step0 = arg0;
    step1 = arg1;

    if (step0 == 0 && step1 == 0) {
        D_8007FB40 = zero;
        D_8007FB44 = zero;
        return;
    }

    if (step0 < 0) {
        value = D_8007FB40 + step0;
        D_8007FB40 = value;
        if (value < 0) {
            D_8007FB40 = 0;
        }
    }

    if (step1 < 0) {
        value = D_8007FB44 + step1;
        D_8007FB44 = value;
        if (value < 0) {
            D_8007FB44 = 0;
        }
    }

    value = D_8007FB40;
    if (value != 0) {
        offset = (value - 1) * 2;
        x0 = (D_8019CB0C != 0) ? 0x69 : 0xA8;
        y0 = 0x9E;
        callScratch = scratch;
        callX = x0;
        y0 = (s16)(y0 - offset);
        x0 += 0x1C;
        y1 = (s16)(offset + 0x9F);
        func_80046E00(
            callScratch,
            callX,
            y0,
            x0,
            y0,
            callX,
            y1,
            x0,
            y1,
            0xB0,
            0x38,
            0xCC,
            0x38,
            0xB0,
            0x6C,
            0xCC,
            0x6C,
            0x7F,
            0x7F,
            0x7F,
            0x232,
            0,
            0,
            0x1C);
    }

    render1 = D_8007FB44;
    if (render1 != 0) {
        offset = render1 - 1;
        x0 = (D_8019CB0C != 0) ? 0x92 : 0xC0;
        y0 = 0x128;
        callScratch = scratch;
        callX = x0;
        y0 = (s16)(y0 - offset);
        x0 += 0x4E;
        y1 = (s16)(render1 + 0x128);
        func_80046E00(
            callScratch,
            callX,
            y0,
            x0,
            y0,
            callX,
            y1,
            x0,
            y1,
            0x61,
            0x38,
            0xAF,
            0x38,
            0x61,
            0x58,
            0xAF,
            0x58,
            0x7F,
            0x7F,
            0x7F,
            0x259,
            0,
            0,
            0x1C);
    }

    if (step0 > 0) {
        value = D_8007FB40 + step0;
        D_8007FB40 = value;
        if (value >= 0xF) {
            D_8007FB40 = 0xE;
        }
    }

    if (step1 > 0) {
        value = D_8007FB44 + step1;
        D_8007FB44 = value;
        __asm__ volatile("" ::: "memory");
        if (value >= 0x11) {
            __asm__ volatile("" ::: "memory");
            D_8007FB44 = 0x10;
        }
    }
}
