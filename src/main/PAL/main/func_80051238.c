#include "common.h"

extern void *g_CarModelSlot asm("D_8009E87C");

/* Declared locally: this unit passes the slot as a pointer, which gcc 2.6.3
 * will not take against the s32 prototype in game/render.h. */
void GameSelectModelBank(void *index) asm("func_80017A10");
void func_80017BAC(void *arg0);
void func_80017B5C(void *arg0);

void func_80051238(void) {
    func_80017BAC(g_CarModelSlot);
    GameSelectModelBank(g_CarModelSlot);
    func_80017B5C(g_CarModelSlot);
}
