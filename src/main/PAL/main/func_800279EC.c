#include "common.h"

s32 func_80032F34(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);

void func_800279EC(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp;

    temp = func_80032F34(arg0, arg1, arg2 + 1, arg3 + 2, 0xC2, 0x1C, 0, 0, 0);
    func_80032F34(arg0, temp, arg2, arg3, 0xC4, 0x20, 0xFF, 0xFF, 0xFF);
}
