#include "common.h"

u32 WaitEvent[4] asm("func_8007B004") __attribute__((section(".text"))) = {
    0x240A00B0,
    0x01400008,
    0x2409000A,
    0x00000000,
};
