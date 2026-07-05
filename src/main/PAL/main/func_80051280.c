#include "common.h"

extern u32 D_8009E87C;

void func_80051238(void);

void func_80051280(void) {
    D_8009E87C = D_8009E87C < 1;
    func_80051238();
}
