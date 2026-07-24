#include "common.h"

extern s32 D_801E6CA4;

void func_8005BDD4(s32 arg0) {
    if (arg0 >= 0) {
        if (arg0 < 0x10) {
            goto done;
        }
        arg0 = 0xF;
    } else {
        arg0 = 0;
    }

done:
    D_801E6CA4 = (arg0 << 7) / 15;
}
