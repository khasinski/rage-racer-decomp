#include "common.h"

extern void *g_CarModelSlot asm("D_8009E87C");

/* Declared locally: this unit passes the slot as a pointer, which gcc 2.6.3
 * will not take against the s32 prototype in game/render.h. */
void GameSelectModelBank(void *index) asm("func_80017A10");
void GameSelectCarModelSlot(void *arg0) asm("func_80017BAC");
void GameUploadCarImage(void *arg0) asm("func_80017B5C");

void func_80051238(void) {
    GameSelectCarModelSlot(g_CarModelSlot);
    GameSelectModelBank(g_CarModelSlot);
    GameUploadCarImage(g_CarModelSlot);
}
