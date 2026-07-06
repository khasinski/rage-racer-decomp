#include "common.h"

s32 func_800657E4(void);

s32 func_80064BB4(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 mode;
    s32 value;

    mode = func_800657E4();
    if (mode == 1) {
        goto high_mode;
    }

    mode = func_800657E4();
    if (mode != 2) {
        goto low_mode;
    }

high_mode:
    value = ((arg0 & 3) << 9) | ((arg1 & 3) << 7) | ((arg3 & 0x300) >> 3);
    return value | ((arg2 & 0x3FF) >> 6);

low_mode:
    value = ((arg0 & 3) << 7) | ((arg1 & 3) << 5) | ((arg3 & 0x100) >> 4) | ((arg2 & 0x3FF) >> 6);
    return value | ((arg3 & 0x200) << 2);
}
