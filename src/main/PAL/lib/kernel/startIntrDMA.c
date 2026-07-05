#include "psyq/kernel.h"

extern u32 D_8009A4F8[];
extern volatile u32 *D_8009A4F4;

void RegisterKernelCallback(s32 slot, void *callback) asm("func_8006DF64");
void *setIntrDMAAddress(void) asm("func_8006EA00");

void *startIntrDMA(void) {
    clearIntrDMACallbacks(D_8009A4F8, 8);
    *D_8009A4F4 = 0;
    RegisterKernelCallback(3, intrDMADispatcher);

    return setIntrDMAAddress;
}
