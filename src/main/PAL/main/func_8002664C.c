#include "common.h"

extern s32 D_801E682C;
extern s32 D_801E40B8;
extern s32 D_801E42E0;
extern s16 D_8007D6CC[];

s32 func_8002664C(s32 arg0) {
    s32 value;

    if (D_801E682C == 0) {
        value = (D_801E40B8 * 4) - D_8007D6CC[arg0];
    } else {
        if (D_801E42E0 > 0) {
            D_801E42E0--;
        }
        value = D_801E42E0;
    }

    return value < 0 ? 0 : (value < 0x80 ? value : 0x7F);
}
