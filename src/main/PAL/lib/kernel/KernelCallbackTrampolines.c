#include "psyq/kernel.h"

typedef void (*Callback2)(s32, s32);

extern KernelCallback *D_8009A4B8;

void ResetCallback(s32 arg0, s32 arg1) {
    D_8009A4B8[1]();
}

void VSyncCallback(s32 arg0) {
    ((Callback2)D_8009A4B8[5])(0, arg0);
}

void KernelCallbackSlot5(void) {
    D_8009A4B8[5]();
}

void KernelCallbackSlot4(void) {
    D_8009A4B8[4]();
}

void KernelCallbackSlot6(void) {
    D_8009A4B8[6]();
}
