#include "psyq/kernel.h"

extern u16 D_80099432;
extern volatile u16 *D_8009A4C0;

s32 GetKernelStatus(void) {
    return D_80099432;
}

s32 GetIntrMask(void) {
    return *D_8009A4C0;
}

s32 SetIntrMask(s32 arg0) {
    u16 value;
    volatile u16 *ptr;

    ptr = D_8009A4C0;
    value = *ptr;
    *ptr = arg0;
    return value;
}
