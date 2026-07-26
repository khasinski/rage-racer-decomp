#include "common.h"

extern void *g_CarModelSlot asm("D_8009E87C");

void func_80017BAC(void *arg0);
void func_80017A10(void *arg0);
void func_80017B5C(void *arg0);

void func_80051238(void) {
    func_80017BAC(g_CarModelSlot);
    func_80017A10(g_CarModelSlot);
    func_80017B5C(g_CarModelSlot);
}
