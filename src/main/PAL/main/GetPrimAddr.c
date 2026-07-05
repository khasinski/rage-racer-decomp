#include "common.h"

s32 GetPrimAddr(u32 *arg0) asm("func_80064DA4");
s32 GetPrimAddr(u32 *arg0) {
    return (*arg0 & 0x00FFFFFF) | 0x80000000;
}

s32 IsEndPrim(u32 *arg0) asm("func_80064DC0");
s32 IsEndPrim(u32 *arg0) {
    return ((*arg0 & 0x00FFFFFF) == 0x00FFFFFF);
}
