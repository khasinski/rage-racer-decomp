#include "common.h"

s32 func_8006CC28(u8 *arg0, u8 *arg1) {
    s32 left;

    if ((arg0 == 0) || (arg1 == 0)) {
        if (arg0 == arg1) {
            return 0;
        }
        if (arg0 == 0) {
            return -1;
        }
        return 1;
    }

    while ((left = *arg0) == *arg1++) {
        if (left == 0) {
            return 0;
        }
        arg0++;
    }

    return *arg0 - arg1[-1];
}
