#include "common.h"

extern u8 *D_8019C900;

void func_800332E0(s32 arg0) {
    *(u16 *)(D_8019C900 + 0x237A6) = arg0 ? 0x7811 : 0x7800;
}
