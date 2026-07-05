#include "psyq/kernel.h"

extern s32 D_8009A51C;

s32 SetDMAInterruptState(s32 arg0) {
    s32 value;

    value = D_8009A51C;
    D_8009A51C = arg0;
    return value;
}

s32 GetDMAInterruptState(void) {
    return D_8009A51C;
}
