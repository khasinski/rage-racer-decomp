#include "common.h"

extern volatile u8 D_8009E0E0[];
extern volatile u8 D_8009E0E2[];
extern volatile u8 D_8009E0E4[];
extern volatile u8 D_8009E0E6[];
extern volatile u8 D_8009E0E8[];
extern volatile u8 D_8009E0EA[];

void func_8007521C(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    register s32 voice asm("$11");
    register s32 start asm("$8");
    register s32 target asm("$9");
    register s32 step asm("$10");
    register s32 offset asm("$2");
    s32 delta;
    register s32 smallDenom asm("$3");
    s32 quotient;
    register s32 start16 asm("$5");
    register s32 target16 asm("$6");
    register s32 stepForSmallDiv asm("$4");
    s32 step16;

    voice = arg0;
    __asm__ volatile("" : "=r"(voice) : "0"(voice));
    start = arg1;
    target = arg2;
    __asm__ volatile("" : "=r"(start) : "0"(start));
    __asm__ volatile("" : "=r"(target) : "0"(target));
    step = arg3;
    start16 = (s16)arg1;
    target16 = (s16)arg2;

    if (start16 == target16) {
        return;
    }

    offset = (((((s16)arg0 * 2) + (s16)arg0) * 4) + (s16)arg0) * 4;
    *(volatile s16 *)(D_8009E0E0 + offset) = 1;
    *(volatile s16 *)(D_8009E0E8 + offset) = start;
    *(volatile s16 *)(D_8009E0EA + offset) = target;

    step16 = (s16)arg3;
    delta = start16 - target16;
    if (delta < 0) {
        offset = target16 - start16;
        if (offset >= step16) {
            goto large;
        }
    } else if (delta >= step16) {
        do {
        } while (0);
        goto large;
    }

small:
    stepForSmallDiv = (s16)step;
    smallDenom = (s16)start;
    offset = (s16)target;
    smallDenom -= offset;
    quotient = stepForSmallDiv / smallDenom;
    smallDenom = (s16)voice;
    offset = (((smallDenom * 2) + smallDenom) * 4 + smallDenom) * 4;
    *(volatile s16 *)(D_8009E0E2 + offset) = 1;
    *(volatile s16 *)(D_8009E0E4 + offset) = quotient;
    *(volatile s16 *)(D_8009E0E6 + offset) = quotient;
    return;

large:
    stepForSmallDiv = (s16)start;
    offset = (s16)target;
    stepForSmallDiv -= offset;
    offset = (s16)step;
    quotient = stepForSmallDiv / offset;
    smallDenom = (s16)voice;
    offset = (((smallDenom * 2) + smallDenom) * 4 + smallDenom) * 4;
    *(volatile s16 *)(D_8009E0E4 + offset) = 0;
    *(volatile s16 *)(D_8009E0E2 + offset) = quotient;
}
