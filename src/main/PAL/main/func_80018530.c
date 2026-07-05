#include "common.h"

extern s32 D_8007BED8;
extern s32 D_8007C704;

s32 func_80018530(void) {
    if (D_8007BED8 != 0) {
        return 1;
    }

    if (D_8007C704 == 4) {
        D_8007C704 = 0;
        return 0;
    }

    D_8007C704 = 4;
    D_8007BED8 = 1;
    return 1;
}
