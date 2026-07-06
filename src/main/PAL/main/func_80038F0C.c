#include "common.h"
#include "game/car.h"

s32 func_8002FB60(s32 arg0, s32 arg1);
s32 func_8002A788(s32 arg0, s32 arg1);
s32 func_800632B0(void);

void func_80038F0C(s32 arg0, GameCarRuntime *arg1) {
    register GameCarRuntime *obj asm("$16");
    register s32 value asm("$2");
    register s32 temp asm("$3");
    register s32 distance asm("$4");

    obj = arg1;
    asm("" : "=r"(obj) : "0"(obj));
    value = 1;
    obj->motionMode = arg0;
    if (arg0 == value) {
        goto mode1;
    }

    value = 2;
    if (arg0 == value) {
        goto mode2;
    }

    return;

mode1:
    value = obj->field_9A;
    temp = 0x1E;
    obj->motionModeTimer = temp;
    value <<= 3;
    goto final_store;

mode2:
    value = func_8002FB60(obj->field_30, (s32)arg1);
    temp = func_8002A788(value, obj->field_24);
    if (temp >= 0x401) {
        temp = 0x800 - temp;
    }

    distance = obj->field_A4;
    if (distance < 0x140) {
        obj->motionValue = 0;
        goto after_store;
    }

    value = distance - 0x140;
    value *= temp;
    if (value < 0) {
        value += 0xFFF;
    }
    value >>= 12;

store:
    obj->motionValue = value;

after_store:
    value = 0x1E;
    obj->motionModeTimer = value;

    value = func_800632B0();
    if (value & 0x80) {
        value = *(u16 *)&obj->motionValue;
        value = -value;

final_store:
        obj->motionValue = value;
    }
}

asm(".globl func_80038FEC\nfunc_80038FEC = func_80038F0C + 0xE0");
