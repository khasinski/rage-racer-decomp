#include "common.h"
void func_8006DF34(void);
void MDEC_reset(s32 arg0) asm("func_800640D4");

void DecDCTReset(s32 arg0) asm("func_80063E44");
void DecDCTReset(s32 arg0) {
    if (arg0 == 0) {
        func_8006DF34();
    }
    MDEC_reset(arg0);
}
