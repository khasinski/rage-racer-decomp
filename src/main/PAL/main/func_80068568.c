#include "common.h"

s32 func_800685A4(s32 arg0);

s32 rsin(s32 arg0) asm("func_80068568");

s32 rsin(s32 arg0) {
    s32 ret;

    if (arg0 < 0) {
        goto negative;
    }

    ret = func_800685A4(arg0 & 0xFFF);
    goto done;

negative:
    ret = -func_800685A4(-arg0 & 0xFFF);

done:
    return ret;
}
