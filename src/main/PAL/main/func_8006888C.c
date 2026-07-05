#include "common.h"

s32 func_80068738(s32 arg0);
s32 Lzc(s32 arg0) asm("func_80069C7C");

s32 func_8006888C(s32 arg0) {
    s32 bits;
    s32 shift;
    s32 value;
    s32 ret;

    if (arg0 == 0) {
        return 0;
    }

    bits = 8 - Lzc(arg0);
    if (bits >= 0) {
        shift = bits >> 1;
        value = arg0 >> (shift * 2);
    } else {
        shift = (bits >> 1) + 1;
        value = arg0 << -(((bits >> 1) + 1) * 2);
    }

    shift -= 6;
    if (shift < 0) {
        ret = func_80068738(value) >> -shift;
        goto done;
    }

    ret = func_80068738(value) << shift;

done:
    return ret;
}
