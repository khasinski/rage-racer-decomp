#include "common.h"

extern s32 D_801E8AF8;
extern s32 D_8019CA6C;

s32 func_80019D24(void);
void func_80019D7C(void);

void func_80019E84(void) {
    s32 temp;

    temp = func_80019D24();
    D_801E8AF8 = temp;
    D_8019CA6C = temp;
    func_80019D7C();
}
