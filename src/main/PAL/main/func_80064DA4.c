#include "common.h"

s32 func_80064DA4(u32 *arg0) {
    return (*arg0 & 0x00FFFFFF) | 0x80000000;
}

s32 func_80064DC0(u32 *arg0) {
    return ((*arg0 & 0x00FFFFFF) == 0x00FFFFFF);
}
