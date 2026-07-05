#include "common.h"

extern s16 D_801E4DAC;
extern s16 D_801E4034;
extern s16 D_8019CAC0;
extern s32 D_8009E6A4;
extern s32 D_801E7730;
extern s32 D_801E772C;
extern s32 D_801E428C;

s32 func_80053688(void) {
    s32 limit;

    if (D_801E4DAC != 0) {
        if (D_801E4034 != 0) {
            limit = (D_8009E6A4 < 2) ? 6 : 7;
        } else {
            limit = (D_8009E6A4 < 2) ? 2 : 3;
        }
    } else if (D_8019CAC0 != 0) {
        limit = (D_801E7730 < 2) ? 6 : 7;
    } else {
        limit = (D_801E772C < 2) ? 2 : 3;
    }

    return D_801E428C < limit;
}
