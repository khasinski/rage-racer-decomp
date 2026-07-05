#include "common.h"

extern s32 D_8009E744;
extern s16 D_8009E6A0;
extern s32 D_801E40D8;
extern s16 D_801E8A8C;

void func_80041170(void) {
    s32 value;

    value = D_8009E744;
    if ((value < 0x7001) || (value >= D_801E40D8 - 0x3000)) {
        if (D_8009E6A0 == 1) {
            D_8009E6A0 = 0;
            goto check_count;
        }
    }

    if ((value >= 0x7001) && (value < D_801E40D8 - 0x3000)) {
        D_8009E6A0 = 1;
    }

check_count:
    if (D_801E8A8C >= 0xA) {
        D_8009E6A0 = 0;
    }
}
