#include "common.h"

extern u8 D_8009904C;

/* CdStatus: returns the last cached CD drive status byte. */
s32 CdStatus(void) asm("func_8006A3E8");
s32 CdStatus(void) {
    return D_8009904C;
}
