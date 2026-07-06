#include "common.h"
#include "game/car.h"

extern s32 D_801E408C;

void func_800393AC(GameCarRuntime *arg0, s32 arg1) {
    register GameCarRuntime *obj asm("a2") = arg0;
    register s32 temp asm("v0");
    register s32 value asm("v1");
    register u8 *base asm("a0");
    register s32 scene asm("a0");

    base = (u8 *)&obj->field_BC;
    if (*(s32 *)&obj->field_F0 == 0) {
        if (obj->field_F4 != 0) {
            goto check_field_F0;
        }
        if (arg1 != 0) {
            value = obj->field_A4;
            if (value < 0x3C1) {
                return;
            }
            scene = D_801E408C;
            value = arg1 * value;
            temp = value / 0x320;
            *(s32 *)&obj->field_F0 = temp;
            if (scene != 0) {
                temp = -temp;
                *(s32 *)&obj->field_F0 = temp;
            }
            obj->field_F4 = 0;
            return;
        }
check_field_F0:
        temp = *(s32 *)(base + 0x34);
        if (temp == 0) {
            goto decay_field_F4;
        }
    }

decay_field_F0:
    value = *(s32 *)(base + 0x34);
    value = (value << 5) - value;
    if (value < 0) {
        value += 0x1F;
    }
    temp = value >> 5;
    value = (u32)value >> 31;
    *(s32 *)(base + 0x34) = temp;
    temp += value;
    value = *(s32 *)(base + 0x38);
    temp >>= 1;
    value -= temp;
    *(s32 *)(base + 0x38) = value;
    if (value >= 0x2BC) {
        *(s32 *)(base + 0x38) = 0x2BC;
        return;
    }
    temp = value < -0x2BB;
    if (temp != 0) {
        temp = -0x2BC;
        *(s32 *)(base + 0x38) = temp;
        return;
    }
    return;

decay_field_F4:
    value = *(s32 *)(base + 0x38);
    if (value != 0) {
        temp = (value << 4) - value;
        temp <<= 1;
        if (temp < 0) {
            temp += 0x1F;
        }
        temp >>= 5;
        *(s32 *)(base + 0x38) = temp;
        if (temp == 0) {
            *(s16 *)(base + 0x7E) = 0;
        }
    }
}
