#include "common.h"

extern s16 D_8007B664[];

s32 func_8001A6AC(s32 arg0, s32 arg1) {
    s32 temp_v0;
    s32 temp_v1;

    if (arg0 == 0) {
        if (arg1 == 0) {
            return 0;
        }
        if (arg1 > 0) {
            return 0x400;
        }
        return -0x400;
    }

    if (arg0 > 0) {
        if (arg1 >= 0) {
            if (arg0 < arg1) {
                temp_v1 = D_8007B664[(arg0 << 10) / arg1];
                return 0x400 - temp_v1;
            }
            return D_8007B664[(arg1 << 10) / arg0];
        }

        arg1 = -arg1;
        if (arg0 < arg1) {
            return D_8007B664[(arg0 << 10) / arg1] - 0x400;
        }
        return -D_8007B664[(arg1 << 10) / arg0];
    }

    arg0 = -arg0;
    if (arg1 >= 0) {
        if (arg0 < arg1) {
            return D_8007B664[(arg0 << 10) / arg1] + 0x400;
        }
        temp_v1 = D_8007B664[(arg1 << 10) / arg0];
        return 0x800 - temp_v1;
    }

    arg1 = -arg1;
    if (arg0 < arg1) {
        temp_v1 = D_8007B664[(arg0 << 10) / arg1];
        temp_v0 = 0xC00;
        return temp_v0 - temp_v1;
    }
    return D_8007B664[(arg1 << 10) / arg0] + 0x800;
}
