#include "common.h"
#include "game/car.h"

s32 func_80039184(GameCarRuntime *arg0);

void func_80039280(GameCarRuntime *arg0) {
    register GameCarRuntime *obj asm("$16");
    register s32 value asm("$4");
    register s32 temp asm("$3");
    register s32 result asm("$2");
    register s32 one asm("$5");
    volatile s32 stack[2];

    obj = arg0;
    (void)stack;

    if (obj->field_98 != 0) {
        result = obj->field_9A;
        value = result * result;
        asm volatile("" : "=r"(value) : "0"(value));
        result = 0x2AAAAAAB;
        asm volatile("mult $4,$2");
        temp = obj->field_90;
        asm volatile("sra $4,$4,31\nmfhi $2\nsubu $4,$2,$4" : "=r"(value), "=r"(result) : "0"(value), "1"(result));
        asm volatile("" : "=r"(value) : "0"(value));
        result = temp;
        asm volatile("" : "=r"(temp) : "0"(temp));
        if (temp >= 0x12C) {
            value >>= 8;
        }
        result += value;
        obj->field_90 = result;

        temp = value;
        if (value < 0) {
            temp = value + 3;
        }
        result = (u16)obj->field_94;
        temp >>= 2;
        result += temp;
        obj->field_94 = result;

        result = obj->field_90;
        temp = obj->field_94;
        obj->field_20 = result;
        obj->field_28 = temp;
        return;
    }

    asm("" : "=r"(obj) : "0"(obj));
    value = func_80039184(obj);
    if (value == 0) {
        return;
    }

    one = 1;
    obj->field_98 = one;
    if (value > 0) {
        temp = value * obj->field_A4;
        temp = temp / -4800;
        obj->field_98 = one;
        obj->field_9C = temp;
        goto finish_setup;
    } else {
        result = 2;
        obj->field_98 = result;
        result = -value;
        obj->field_9C = result;
    }

finish_setup:
    result = (u16)obj->field_20;
    temp = (u16)obj->field_28;
    value = (u16)obj->y;
    obj->field_9A = 0;
    obj->field_90 = result;
    obj->field_94 = temp;
    obj->field_9E = value;
}
