#include "common.h"

extern u32 g_CarModelSlot asm("D_8009E87C");

void func_80051238(void);

void func_80051280(void) {
    g_CarModelSlot = g_CarModelSlot < 1;
    func_80051238();
}
