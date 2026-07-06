#include "common.h"

extern s32 D_801E42E4;
extern s32 D_801E40B8;
extern s16 D_8007C798[];
extern s32 D_801E42E0;

s32 func_8001C8F0(s32 arg0) {
    s32 value;
    s32 ret;

    if (D_801E42E4 == 10) {
        s32 offset;
        s32 counter;

        offset = arg0 << 1;
        counter = D_801E40B8;
        value = (counter << 2) - *(s16 *)((u8 *)D_8007C798 + offset);
    } else {
        value = D_801E42E0;
        if (value > 0) {
            value--;
            D_801E42E0 = value;
        }
        value = D_801E42E0;
    }

    if (value >= 0) {
        ret = value;
        if (ret < 0x80) {
            return ret;
        }
        ret = 0x7F;
    } else {
        ret = 0;
    }
    return ret;
}
