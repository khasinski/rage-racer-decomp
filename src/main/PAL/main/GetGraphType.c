#include "common.h"

extern u8 g_GraphType asm("D_800941E8");
extern u8 g_GraphDebug asm("D_800941EA");

s32 GetGraphType(void) asm("func_800657E4");
s32 GetGraphDebug(void) asm("func_800657F4");

s32 GetGraphType(void) {
    return g_GraphType;
}

s32 GetGraphDebug(void) {
    return g_GraphDebug;
}
