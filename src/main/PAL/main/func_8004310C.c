#include "common.h"

extern u8 g_CdVolume asm("D_8009B194");

void func_80042FA0(u8 arg0);

void func_8004310C(void) {
    func_80042FA0(g_CdVolume);
}
