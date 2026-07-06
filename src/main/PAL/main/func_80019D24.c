#include "common.h"

extern s32 D_801E4284;
extern s32 D_801E4288;
extern s32 D_801E6F10;

s32 func_80019D24(s32 arg0) {
    s32 ret;
    s32 one;

    ret = 0;
    if (arg0 < D_801E4284) {
        goto low;
    }

    ret = 0x100;
    if (arg0 >= D_801E4288) {
        goto high;
    }

    one = 1;
    D_801E6F10 = one;
    goto done;

high:
    ret = 0;
low:
    D_801E6F10 = 0;

done:
    return ret;
}
