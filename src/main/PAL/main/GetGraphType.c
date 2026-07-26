#include "common.h"

extern u8 D_800941E8;
extern u8 D_800941EA;

s32 GetGraphType(void) asm("func_800657E4");
s32 GetGraphType(void) {
    return D_800941E8;
}

s32 GetGraphDebug(void) asm("func_800657F4");
s32 GetGraphDebug(void) {
    return D_800941EA;
}
