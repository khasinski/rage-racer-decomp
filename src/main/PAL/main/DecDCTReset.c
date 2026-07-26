#include "common.h"
void func_8006DF34(void);
void func_800640D4(s32 arg0);

void DecDCTReset(s32 arg0) asm("func_80063E44");
void DecDCTReset(s32 arg0) {
    if (arg0 == 0) {
        func_8006DF34();
    }
    func_800640D4(arg0);
}
