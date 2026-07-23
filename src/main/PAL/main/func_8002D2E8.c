#include "common.h"

s32 func_80069C98(s32 arg0, s32 arg1, s32 arg2);

s32 func_8002D2E8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 result;
    s32 ret = 0;

    if (func_80069C98(arg0, arg1, arg4) >= 0) {
        if (func_80069C98(arg1, arg3, arg4) >= 0) {
            if (func_80069C98(arg3, arg2, arg4) >= 0) {
                result = func_80069C98(arg2, arg0, arg4) >= 0;
                ret = result;
            }
        }
    }

    return ret;
}
