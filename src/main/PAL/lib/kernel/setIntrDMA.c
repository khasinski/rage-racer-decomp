#include "psyq/kernel.h"

extern u32 D_8009A4F8[];
extern volatile u32 *D_8009A4F4;

u32 setIntrDMA(s32 arg0, u32 arg1) {
    register s32 index asm("$6");
    register u32 callback asm("$4");
    register u32 *base asm("$3");
    register s32 offset asm("$2");
    register u32 *slot asm("$3");
    register u32 oldCallback asm("$7");

    index = arg0;
    asm("");
    base = D_8009A4F8;
    offset = index << 2;
    slot = (u32 *)((s32)base + offset);
    oldCallback = *slot;
    callback = arg1;

    if (callback != oldCallback) {
        if (callback != 0) {
            register volatile u32 *bits asm("$5") = D_8009A4F4;
            register u32 value asm("$4");
            register s32 shift asm("$3");
            register u32 mask asm("$2") = 0xFFFFFF;

            *slot = callback;
            value = *bits;
            shift = index + 0x10;
            value &= mask;
            mask = 1;
            mask <<= shift;
            shift = 0x800000;
            mask |= shift;
            value |= mask;
            *bits = value;
        } else {
            register volatile u32 *bits asm("$5") = D_8009A4F4;
            register u32 value asm("$3");
            register s32 shift asm("$4");
            register u32 mask asm("$2") = 0xFFFFFF;
            register u32 zero asm("$0");

            *slot = zero;
            value = *bits;
            shift = index + 0x10;
            value &= mask;
            mask = 0x800000;
            value |= mask;
            mask = 1;
            mask <<= shift;
            mask = ~mask;
            value &= mask;
            *bits = value;
        }
    }

    return oldCallback;
}
