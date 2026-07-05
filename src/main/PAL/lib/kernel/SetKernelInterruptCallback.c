#include "common.h"

typedef void (*Callback)(void);

extern Callback D_80099434[];
extern u16 D_80099460;
extern volatile u16 *D_8009A4C0;

void ChangeClearRCnt(s32 clear) asm("func_8006DF14");
void ChangeClearInterruptMask(s32 index, s32 clear) asm("func_8006DF24");
Callback func_8006E390(s32 arg0, Callback arg1) {
    register s32 index asm("$17");
    register Callback callback asm("$18");
    register Callback *base asm("$5");
    register s32 offset asm("$2");
    register Callback *slot asm("$4");
    register Callback oldCallback asm("$20");
    register u32 pendingValue asm("$3");
    register u32 pendingMask asm("$19");
    register s32 disabled asm("$16");
    volatile u16 *maskPtr;

    index = arg0;
    __asm__("" : "=r"(index) : "0"(index));
    callback = arg1;
    base = D_80099434;
    __asm__("" : "=r"(base) : "0"(base));
    offset = index << 2;
    slot = (Callback *)(offset + (s32)base);
    __asm__("" : "=r"(slot) : "0"(slot));
    oldCallback = *slot;

    if (callback == oldCallback) {
        goto done;
    }

    if (*((u16 *)base - 2) == 0) {
        goto done;
    }

    maskPtr = D_8009A4C0;
    pendingValue = *maskPtr;
    __asm__("" : "=r"(pendingValue) : "0"(pendingValue));
    *maskPtr = 0;
    pendingMask = pendingValue & 0xFFFF;

    if (callback != 0) {
        register u32 bit asm("$3");

        bit = 1 << index;
        __asm__("" : "=r"(bit) : "0"(bit));
        pendingMask |= bit;
        *slot = callback;
        {
            register u32 value asm("$2");

            value = *(u16 *)(base + 11);
            value |= bit;
            *(u16 *)(base + 11) = value;
        }
    } else {
        register Callback zero asm("$0");
        register u32 bit asm("$2");
        register u32 activeMask asm("$3");

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
