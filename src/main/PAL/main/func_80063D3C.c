#include "common.h"

extern u8 D_80082FD9[];

s32 func_80063D3C(s32 arg0) {
    register s32 value asm("$3") = arg0;

    if (D_80082FD9[value & 0xFF] & 2) {
        value = arg0 - 0x20;
    }

    return value & 0xFF;
}
