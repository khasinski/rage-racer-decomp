#include "common.h"

void AddPrim(u32 *arg0, u32 *arg1) asm("func_80064DDC");
void AddPrim(u32 *arg0, u32 *arg1) {
    register u32 mask asm("$6") = 0x00FFFFFF;
    register u32 tag asm("$7") = 0xFF000000;

    *arg1 = (*arg1 & tag) | (*arg0 & mask);
    *arg0 = (*arg0 & tag) | ((u32)arg1 & mask);
}

void AddPrims(u32 *arg0, u32 arg1, u32 *arg2) asm("func_80064E18");
void AddPrims(u32 *arg0, u32 arg1, u32 *arg2) {
    register u32 mask asm("$7") = 0x00FFFFFF;
    register u32 tag asm("$8") = 0xFF000000;

    *arg2 = (*arg2 & tag) | (*arg0 & mask);
    *arg0 = (*arg0 & tag) | (arg1 & mask);
}
