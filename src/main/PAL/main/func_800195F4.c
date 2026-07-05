#include "common.h"

extern s32 D_8007BED8;
extern s32 D_8009E6A4;
extern s16 D_8019CABC;
extern s32 D_801E4B30;

s32 func_80017C78(s32 arg0, s32 arg1);

void func_800195F4(void) {
    s32 base;
    s32 offset;
    s32 value;

    if (D_8007BED8 == 1) {
        offset = D_8019CABC * 6;
        base = D_8009E6A4 + 0x4A;
        value = func_80017C78(offset + base, D_801E4B30);
        if (value != 0) {
            D_8007BED8 = 0;
        }
    }
}
