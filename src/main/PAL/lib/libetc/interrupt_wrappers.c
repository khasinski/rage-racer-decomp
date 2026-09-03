#include "psyq/kernel.h"
#include <stdio.h>

typedef void (*Callback2)(long, long);

typedef struct CallbackTable {
    u_char pad0[0xC];
    void (*callback)(void);
} CallbackTable;

void KernelCallbackSlot3(void) {
    ((CallbackTable *)g_IntrRpNode)->callback();
}

long KernelCallbackSlot2(void) {
    return ((long (*)())g_IntrRpNode[2])();
}

void DMACallback(long spec, long callback) {
    g_IntrRpNode[1]();
}

void VSyncCallback(long count) {
    ((Callback2)g_IntrRpNode[5])(0, count);
}

void KernelCallbackSlot5(void) {
    g_IntrRpNode[5]();
}

void KernelCallbackSlot4(void) {
    g_IntrRpNode[4]();
}

void KernelCallbackSlot6(void) {
    g_IntrRpNode[6]();
}

long GetKernelStatus(void) {
    return g_IntrInDispatch;
}

long GetIntrMask(void) {
    return *g_IrqMask;
}

long SetIntrMask(long mask) {
    u_short value;
    volatile u_short *ptr;

    ptr = g_IrqMask;
    value = *ptr;
    *ptr = mask;
    return value;
}
