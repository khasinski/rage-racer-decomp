#include "common.h"

u32 RestoreKernelRegistersStub[17] asm("func_8006E6C0") __attribute__((section(".text"))) = {
    0x8C9F0000,
    0x8C9C002C,
    0x8C9D0004,
    0x8C9E0008,
    0x8C90000C,
    0x8C910010,
    0x8C920014,
    0x8C930018,
    0x8C94001C,
    0x8C950020,
    0x8C960024,
    0x8C970028,
    0x00A01021,
    0x03E00008,
    0,
    0,
    0,
};
