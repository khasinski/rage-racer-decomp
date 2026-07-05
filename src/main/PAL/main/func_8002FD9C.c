#include "common.h"

extern s32 D_8009E6A8;

s32 func_8002FAE8(s32 arg0, s32 arg1, s32 arg2);
s32 func_8002FB60(s32 arg0, s32 arg1);

s32 func_8002FD9C(s32 arg0, s32 arg1) {
    s32 center;
    s32 prev_index;
    s32 prev;
    s32 left;
    s32 next_index;
    s32 next;
    s32 right;

    center = func_8002FB60(arg0, arg1);

    prev_index = arg0 - 2;
    if (prev_index < 0) {
        register s32 tmp asm("$2");
        tmp = D_8009E6A8;
        tmp -= 2;
        prev_index = tmp + arg0;
    }

    prev = func_8002FB60(prev_index, arg1);
    left = func_8002FAE8(center, prev, 0x200);

    next_index = (arg0 + 2) % D_8009E6A8;
    next = func_8002FB60(next_index, arg1);
    right = func_8002FAE8(center, next, 0x200);

    return func_8002FAE8(left, right, 0x200);
}
