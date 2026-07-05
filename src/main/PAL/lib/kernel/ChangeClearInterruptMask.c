#include "common.h"

u32 ChangeClearInterruptMaskStub[4] asm("func_8006DF24") __attribute__((section(".text"))) = {
    0x240A00C0,
    0x01400008,
    0x2409000A,
    0,
};
