#include "common.h"

extern s32 D_8007F948;
extern s32 D_8009B298;
extern s32 D_8009B29C;

void func_8004B8B4(s32 arg0, s32 arg1) {
    s32 temp;
    s32 y;

    if (arg0 > 0) {
        temp = arg0 + D_8009B298;
        D_8009B298 = temp;
        if (temp >= 0x101) {
            D_8009B298 = 0x100;
        }
    } else {
        temp = arg0 + D_8009B298;
        D_8009B298 = temp;
        if (temp < 0x40) {
            D_8009B298 = 0x40;
        }
    }

    if (arg1 > 0) {
        temp = arg1 + D_8009B29C;
        D_8009B29C = temp;
        if (temp >= 0x101) {
            D_8009B29C = 0x100;
        }
    } else {
        temp = arg1 + D_8009B29C;
        D_8009B29C = temp;
        if (temp < 0) {
            D_8009B29C = 0;
        }
    }

    y = D_8009B29C;
    temp = ((y << 4) + y) << 4;
    if (temp < 0) {
        temp += 0xFF;
    }
    D_8007F948 = 0x220 - (temp >> 8);
}
