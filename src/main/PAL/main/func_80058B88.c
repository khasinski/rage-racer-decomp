#include "common.h"

extern s32 D_8009B2E4;

s32 func_80058B88(s32 arg0) {
    s32 value;

    if (arg0 == 0) {
        D_8009B2E4 = 0;
        return;
    }

    if (arg0 > 0) {
        value = arg0 + D_8009B2E4;
        D_8009B2E4 = value;
        if (value >= 0x1FD) {
            D_8009B2E4 = 0x1FC;
        }
    } else {
        value = arg0 + D_8009B2E4;
        D_8009B2E4 = value;
        if (value < 0) {
            D_8009B2E4 = 0;
        }
    }

    return D_8009B2E4;
}
