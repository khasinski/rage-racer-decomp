#include "common.h"
#include "game/car.h"

extern s16 D_801E6E74;
extern u8 *D_801E42D8;

void func_80038B04(GameCarRuntime *arg0) {
    register GameCarRuntime *obj asm("$5");
    register u8 *ptr asm("$4");
    register s32 value asm("$2");
    register s32 limit asm("$3");

    obj = arg0;
    __asm__ volatile("" : "=r"(obj) : "0"(obj));

    ptr = (u8 *)&obj->field_BC;
    if (D_801E6E74 >= 2) {
        goto main_body;
    }

    value = 8;
    goto store_value;

main_body:
    if (obj->field_98 != 0) {
        goto dampen;
    }

    if (obj->field_134 < *(s16 *)(D_801E42D8 + 0x106)) {
        goto side_check;
    }
    if (obj->field_15C < 0x81) {
        goto side_check;
    }
    if (obj->field_F0 != 0) {
        goto side_check;
    }

    ptr = (u8 *)(s32)(u16)obj->field_8C;
    value = obj->field_132;
    ptr -= 4;
    obj->field_8C = (s32)ptr;
    ptr = (u8 *)((s32)ptr << 16 >> 16);
    limit = 9 - value;
    value = (limit << 2) + limit;
    value = -value;
    if ((s32)ptr < value) {
        obj->field_8C = value;
    }
    return;

side_check:
    if (*(s16 *)(ptr + 0xA2) >= 0x81 || *(s16 *)(ptr + 0x34) > 0) {
        if (obj->field_A4 >= 0x51) {
            value = (u16)obj->field_8C + 2;
            obj->field_8C = value;
            value = (s16)value;
            if (value >= 9) {
                obj->field_8C = 8;
            }
            return;
        }
    }

dampen:
    limit = obj->field_8C;
    value = limit * 3;
    if (value < 0) {
        value += 3;
    }
    value >>= 2;

store_value:
    obj->field_8C = value;
}
