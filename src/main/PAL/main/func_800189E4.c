#include "common.h"

extern s32 D_8007BED8;
extern s32 D_8007C704;
extern s32 D_8009AEFC;

s32 func_800189E4(s32 arg0) {
    if (D_8007BED8 != 0) {
        return 1;
    }

    D_8007C704 = 6;
    D_8009AEFC = arg0;
    D_8007BED8 = 1;
    return 1;
}
