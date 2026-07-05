#include "common.h"

extern s16 D_8019CAB0;
extern s32 D_8019C8FC;
extern s32 D_8019CAB4;
extern s32 D_801E4170;
extern s32 D_801E40B0;

s32 func_8003351C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

s32 func_8002F458(void) {
    s32 value;
    s32 arg2;
    s32 arg3;

    if (D_8019CAB0 != 3) {
        value = D_8019C8FC;
        arg3 = value - 0x1154;
        if ((u32)arg3 < 0x434C) {
            if ((u32)arg3 < 0x80) {
                arg2 = 3;
            } else {
                arg3 = value - 0x5420;
                if ((u32)arg3 < 0x80) {
                    arg2 = 1;
                } else {
                    arg2 = 0;
                    goto clear_arg3;
                }
            }
        } else {
            arg2 = 2;
clear_arg3:
            arg3 = 0;
        }
    } else {
        arg2 = 2;
        arg3 = 0;
    }

    return func_8003351C(D_8019CAB4 + D_801E4170, D_801E40B0, arg2, arg3);
}
