#include "common.h"
extern s32 D_80099048;
void func_8006BAF0(void);

void func_8006A494(void) {
    func_8006BAF0();
}

s32 func_8006A4B4(s32 arg0) {
    s32 old = D_80099048;
    D_80099048 = arg0;
    return old;
}
