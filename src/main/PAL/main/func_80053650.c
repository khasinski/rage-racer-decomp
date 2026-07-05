#include "common.h"
extern s16 D_801E4DAC;
extern s16 D_801E4034;
extern s32 D_801E428C;
s32 func_80053650(void) {
    s32 v1 = 0;
    if (D_801E4DAC != 0) {
        v1 = (D_801E4034 != 0) << 2;
    }
    return v1 < D_801E428C;
}
