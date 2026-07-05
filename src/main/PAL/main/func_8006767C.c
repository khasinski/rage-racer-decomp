#include "common.h"

extern s32 func_800676A0(void *callback, void *data, s32 size, s32 arg3);

s32 func_8006767C(void *callback, void *data, s32 arg2) {
    return func_800676A0(callback, data, 0, arg2);
}
