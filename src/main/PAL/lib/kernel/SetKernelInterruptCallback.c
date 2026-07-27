#include <sys/types.h>

#include "common.h"

typedef void (*Callback)(void);

extern Callback D_80099434[];
extern u_short D_80099460;
extern volatile u_short *D_8009A4C0;

void ChangeClearRCnt(long clear) asm("func_8006DF14");
void ChangeClearInterruptMask(long index, long clear) asm("func_8006DF24");
Callback SetKernelInterruptCallback(long arg0, Callback arg1) asm("func_8006E390");
Callback SetKernelInterruptCallback(long arg0, Callback arg1) {
    register long index asm("$17");
    register Callback callback asm("$18");
    register Callback *base asm("$5");
    register long offset asm("$2");
    register Callback *slot asm("$4");
    register Callback oldCallback asm("$20");
    register u_long pendingValue asm("$3");
    register u_long pendingMask asm("$19");
    register long disabled asm("$16");
    volatile u_short *maskPtr;

    index = arg0;
    __asm__("" : "=r"(index) : "0"(index));
    callback = arg1;
    base = D_80099434;
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

    maskPtr = D_8009A4C0;
    pendingValue = *maskPtr;
    __asm__("" : "=r"(pendingValue) : "0"(pendingValue));
    *maskPtr = 0;
    pendingMask = pendingValue & 0xFFFF;

    if (callback != 0) {
        register u_long bit asm("$3");

        bit = 1 << index;
        __asm__("" : "=r"(bit) : "0"(bit));
        pendingMask |= bit;
        *slot = callback;
        {
            register u_long value asm("$2");

            value = *(u_short *)(base + 11);
            value |= bit;
            *(u_short *)(base + 11) = value;
        }
    } else {
        register Callback zero asm("$0");
        register u_long bit asm("$2");
        register u_long activeMask asm("$3");

        bit = 1 << index;
        bit = ~bit;
        *slot = zero;
        activeMask = D_80099460;
        pendingMask &= bit;
        activeMask &= bit;
        D_80099460 = activeMask;
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

    *D_8009A4C0 = pendingMask;
done:
    return oldCallback;
}
