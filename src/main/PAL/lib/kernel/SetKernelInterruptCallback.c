#include <sys/types.h>

#include "common.h"

typedef void (*Callback)(void);

extern Callback g_IntrCallbacks[];
extern u_short g_IntrCallbackMask;
extern volatile u_short *g_IrqMask;

void ChangeClearRCnt(long clear);
void ChangeClearInterruptMask(long index, long clear);
Callback SetKernelInterruptCallback(long arg0, Callback arg1) {
    long index;
    Callback callback;
    register Callback *base asm("$5");
    long offset;
    Callback *slot;
    Callback oldCallback;
    register u_long pendingValue asm("$3");
    u_long pendingMask;
    long disabled;
    volatile u_short *maskPtr;

    index = arg0;
    __asm__("" : "=r"(index) : "0"(index));
    callback = arg1;
    base = g_IntrCallbacks;
    __asm__("" : "=r"(base) : "0"(base));
    offset = index << 2;
    slot = (Callback *)(offset + (long)base);
    oldCallback = *slot;

    if (callback == oldCallback) {
        return oldCallback;
    }

    if (*((u_short *)base - 2) == 0) {
    } else {

    maskPtr = g_IrqMask;
    pendingValue = *maskPtr;
    __asm__("" : "=r"(pendingValue) : "0"(pendingValue));
    *maskPtr = 0;
    pendingMask = pendingValue & 0xFFFF;

    if (callback != 0) {
        u_long bit;

        bit = 1 << index;
        __asm__("" : "=r"(bit) : "0"(bit));
        pendingMask |= bit;
        *slot = callback;
        {
            u_long value;

            value = *(u_short *)(base + 11);
            value |= bit;
            *(u_short *)(base + 11) = value;
        }
    } else {
        register Callback zero asm("$0");
        u_long bit;
        register u_long activeMask asm("$3");

        bit = 1 << index;
        bit = ~bit;
        *slot = zero;
        activeMask = g_IntrCallbackMask;
        pendingMask &= bit;
        activeMask &= bit;
        g_IntrCallbackMask = activeMask;
    }

    if (index == 0) {
        disabled = callback == 0;
        ChangeClearRCnt(disabled);
        ChangeClearInterruptMask(3, disabled);
    }
    if (index == 4) {
        ChangeClearInterruptMask(0, callback == 0);
    }
    if (index == 5) {
        ChangeClearInterruptMask(1, callback == 0);
    }
    if (index == 6) {
        ChangeClearInterruptMask(2, callback == 0);
    }

    *g_IrqMask = pendingMask;
    }
    return oldCallback;
}
