#include <sys/types.h>

#include "common.h"

typedef void (*Callback)(void);

extern Callback g_IntrCallbacks[] asm("D_80099434");
extern u_short g_IntrCallbackMask asm("D_80099460");
extern volatile u_short *g_IrqMask asm("D_8009A4C0");

void ChangeClearRCnt(long clear) asm("func_8006DF14");
void ChangeClearInterruptMask(long index, long clear) asm("func_8006DF24");
Callback SetKernelInterruptCallback(long arg0, Callback arg1) asm("func_8006E390");
Callback SetKernelInterruptCallback(long arg0, Callback arg1) {
    long index;
    Callback callback;
    /* This pin is load-bearing: removing it changes .text. */
    register Callback *base asm("$5");
    long offset;
    Callback *slot;
    Callback oldCallback;
    /* This pin is load-bearing: removing it changes .text. */
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
    __asm__("" : "=r"(slot) : "0"(slot));
    oldCallback = *slot;

    if (callback == oldCallback) {
        goto done;
    }

    if (*((u_short *)base - 2) == 0) {
        goto done;
    }

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
        /* These pins are load-bearing: removing any one changes .text. */
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
done:
    return oldCallback;
}
