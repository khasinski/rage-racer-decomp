#include "common.h"

extern s32 D_8019C9A8;
extern s32 D_801E4284;
extern s32 D_801E4288;

void func_8001D30C(void) {
    s32 *ptr;

    ptr = (s32 *)D_8019C9A8;
    D_801E4284 = ptr[0];
    D_801E4288 = ptr[1];
}
