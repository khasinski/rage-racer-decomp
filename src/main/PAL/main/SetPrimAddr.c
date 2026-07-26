#include "common.h"

void SetPrimAddr(u32 *arg0, u32 arg1) asm("func_80064E54");
void SetPrimAddr(u32 *arg0, u32 arg1) {
    *arg0 = (*arg0 & 0xFF000000) | (arg1 & 0x00FFFFFF);
}

void TermPrim(u32 *arg0) asm("func_80064E78");
void TermPrim(u32 *arg0) {
    *arg0 |= 0x00FFFFFF;
}
