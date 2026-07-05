#include "common.h"

extern s32 D_8007D78C;

s32 func_8002785C(s32 arg0) {
    s32 old = D_8007D78C;

    D_8007D78C = arg0;
    return old;
}
