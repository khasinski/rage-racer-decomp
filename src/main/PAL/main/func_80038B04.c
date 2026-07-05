#include "common.h"

extern s16 D_801E6E74;
extern u8 *D_801E42D8;

void func_80038B04(u8 *arg0) {
    register u8 *obj asm("$5");
    register u8 *ptr asm("$4");
    register s32 value asm("$2");
    register s32 limit asm("$3");

    obj = arg0;
    __asm__ volatile("" : "=r"(obj) : "0"(obj));

    ptr = obj + 0xBC;
    if (D_801E6E74 >= 2) {
        goto main_body;
    }

    value = 8;
    goto store_value;

main_body:
    if (*(s16 *)(obj + 0x98) != 0) {
        goto dampen;
    }

    if (*(s32 *)(obj + 0x134) < *(s16 *)(D_801E42D8 + 0x106)) {
        goto side_check;
    }
    if (*(s16 *)(obj + 0x15C) < 0x81) {
        goto side_check;
    }
    if (*(s16 *)(obj + 0xF0) != 0) {
        goto side_check;
    }

    ptr = (u8 *)(s32)*(u16 *)(obj + 0x8C);
    value = *(s16 *)(obj + 0x132);
    ptr -= 4;
    *(s16 *)(obj + 0x8C) = (s32)ptr;
    ptr = (u8 *)((s32)ptr << 16 >> 16);
    limit = 9 - value;
    value = (limit << 2) + limit;
    value = -value;
    if ((s32)ptr < value) {
        *(s16 *)(obj + 0x8C) = value;
    }
    return;

side_check:
    if (*(s16 *)(ptr + 0xA2) >= 0x81 || *(s16 *)(ptr + 0x34) > 0) {
        if (*(s32 *)(obj + 0xA4) >= 0x51) {
            value = *(u16 *)(obj + 0x8C) + 2;
            *(s16 *)(obj + 0x8C) = value;
            value = (s16)value;
            if (value >= 9) {
                *(s16 *)(obj + 0x8C) = 8;
            }
            return;
        }
    }

dampen:
    limit = *(s16 *)(obj + 0x8C);
    value = limit * 3;
    if (value < 0) {
        value += 3;
    }
    value >>= 2;

store_value:
    *(s16 *)(obj + 0x8C) = value;
}
