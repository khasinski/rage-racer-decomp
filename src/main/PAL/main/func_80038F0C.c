#include "common.h"

s32 func_8002FB60(s32 arg0, s32 arg1);
s32 func_8002A788(s32 arg0, s32 arg1);
s32 func_800632B0(void);

void func_80038F0C(s32 arg0, u8 *arg1) {
    register u8 *obj asm("$16");
    register s32 value asm("$2");
    register s32 temp asm("$3");
    register s32 distance asm("$4");

    obj = arg1;
    asm("" : "=r"(obj) : "0"(obj));
    value = 1;
    *(s16 *)(obj + 0x84) = arg0;
    if (arg0 == value) {
        goto mode1;
    }

    value = 2;
    if (arg0 == value) {
        goto mode2;
    }

    return;

mode1:
    value = *(s16 *)(obj + 0x9A);
    temp = 0x1E;
    *(s16 *)(obj + 0x86) = temp;
    value <<= 3;
    goto final_store;

mode2:
    value = func_8002FB60(*(s32 *)(obj + 0x30), (s32)arg1);
    temp = func_8002A788(value, *(s32 *)(obj + 0x24));
    if (temp >= 0x401) {
        temp = 0x800 - temp;
    }

    distance = *(s32 *)(obj + 0xA4);
    if (distance < 0x140) {
        *(s16 *)(obj + 0x88) = 0;
        goto after_store;
    }

    value = distance - 0x140;
    value *= temp;
    if (value < 0) {
        value += 0xFFF;
    }
    value >>= 12;

store:
    *(s16 *)(obj + 0x88) = value;

after_store:
    value = 0x1E;
    *(s16 *)(obj + 0x86) = value;

    value = func_800632B0();
    if (value & 0x80) {
        value = *(u16 *)(obj + 0x88);
        value = -value;

final_store:
        *(s16 *)(obj + 0x88) = value;
    }
}

asm(".globl func_80038FEC\nfunc_80038FEC = func_80038F0C + 0xE0");
