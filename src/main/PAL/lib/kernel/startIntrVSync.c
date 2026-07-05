#include "psyq/kernel.h"

extern u32 D_8009A4CC[];
extern u32 *D_8009A4F0;
extern s32 D_8009A4EC;

void RegisterKernelCallback(s32 slot, void *callback) asm("func_8006DF64");
void *setIntrVSyncAddress(void) asm("func_8006E7D4");

void *startIntrVSync(void) {
    *D_8009A4F0 = 0x107;
    D_8009A4EC = 0;
    clearIntrVSyncCallbacks(D_8009A4CC, 8);
    RegisterKernelCallback(0, intrVSyncDispatcher);

    return setIntrVSyncAddress;
}
