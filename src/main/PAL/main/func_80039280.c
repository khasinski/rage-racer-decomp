#include "common.h"

s32 func_80039184(u8 *arg0);

void func_80039280(u8 *arg0) {
    register u8 *obj asm("$16");
    register s32 value asm("$4");
    register s32 temp asm("$3");
    register s32 result asm("$2");
    register s32 one asm("$5");
    volatile s32 stack[2];

    obj = arg0;
    (void)stack;

    if (*(s16 *)(obj + 0x98) != 0) {
        result = *(s16 *)(obj + 0x9A);
        value = result * result;
        asm volatile("" : "=r"(value) : "0"(value));
        result = 0x2AAAAAAB;
        asm volatile("mult $4,$2");
        temp = *(s16 *)(obj + 0x90);
        asm volatile("sra $4,$4,31\nmfhi $2\nsubu $4,$2,$4" : "=r"(value), "=r"(result) : "0"(value), "1"(result));
        asm volatile("" : "=r"(value) : "0"(value));
        result = temp;
        asm volatile("" : "=r"(temp) : "0"(temp));
        if (temp >= 0x12C) {
            value >>= 8;
        }
        result += value;
        *(s16 *)(obj + 0x90) = result;

        temp = value;
        if (value < 0) {
            temp = value + 3;
        }
        result = *(u16 *)(obj + 0x94);
        temp >>= 2;
        result += temp;
        *(s16 *)(obj + 0x94) = result;

        result = *(s16 *)(obj + 0x90);
        temp = *(s16 *)(obj + 0x94);
        *(s32 *)(obj + 0x20) = result;
        *(s32 *)(obj + 0x28) = temp;
        return;
    }

    asm("" : "=r"(obj) : "0"(obj));
    value = func_80039184((u8 *)obj);
    if (value == 0) {
        return;
    }

    one = 1;
    *(s16 *)(obj + 0x98) = one;
    if (value > 0) {
        temp = value * *(s32 *)(obj + 0xA4);
        temp = temp / -4800;
        *(s16 *)(obj + 0x98) = one;
        *(s16 *)(obj + 0x9C) = temp;
        goto finish_setup;
    } else {
        result = 2;
        *(s16 *)(obj + 0x98) = result;
        result = -value;
        *(s16 *)(obj + 0x9C) = result;
    }

finish_setup:
    result = *(u16 *)(obj + 0x20);
    temp = *(u16 *)(obj + 0x28);
    value = *(u16 *)(obj + 4);
    *(s16 *)(obj + 0x9A) = 0;
    *(s16 *)(obj + 0x90) = result;
    *(s16 *)(obj + 0x94) = temp;
    *(s16 *)(obj + 0x9E) = value;
}
