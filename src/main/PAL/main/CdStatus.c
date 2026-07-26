#include "common.h"

extern u8 D_8009904C;

/* CdStatus: returns the last cached CD drive status byte. */
s32 func_8006A3E8(void) {
    return D_8009904C;
}
