#include "common.h"

extern s32 D_8009B2C4;

s32 func_80054C84(s32 arg0) {
    s32 value;

    if (arg0 == 0) {
        D_8009B2C4 = 0;
        return;
    }

    if (arg0 > 0) {
        value = arg0 + D_8009B2C4;
        D_8009B2C4 = value;
        if (value >= 0x1FD) {
            D_8009B2C4 = 0x1FC;
        }
    } else {
        value = arg0 + D_8009B2C4;
        D_8009B2C4 = value;
        if (value < 0) {
            D_8009B2C4 = 0;
        }
    }

    return D_8009B2C4;
}
