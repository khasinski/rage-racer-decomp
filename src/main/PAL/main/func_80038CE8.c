#include "common.h"
#include "game/car.h"

s32 func_8002A788(s32 arg0, s32 arg1);
s32 func_80068568(s32 angle);
s32 func_80068634(s32 angle);

void func_80038CE8(GameCarRuntime *car, s32 arg1, s32 arg2, s32 mode) {
    register GameCarRuntime *carReg asm("$17");
    register s32 x asm("$16");
    register s32 angle asm("$18");
    register s32 savedAngle asm("$19");
    register s32 z asm("$6") = arg2;
    register s32 adjustedReg asm("$2");
    register s32 raw asm("$3");
    register s32 rawArg asm("$4");
    register s32 field34 asm("$2");
    s32 adjusted;
    s32 fieldA4;
    register s32 tmp asm("$2");
    s32 speed;
    s32 trig;
    register s32 product asm("$3");

    carReg = car;
    asm("" : : "r"(carReg));
    asm("" : : : "$16");
    carReg->motionActive = 1;
    x = arg1;
    if (mode >= 2) {
        goto mode_ge_2;
    }

    adjustedReg = *(u16 *)&carReg->field_B4;
    raw = 0xC00;
    raw -= adjustedReg;
    rawArg = raw;
    raw <<= 16;
    field34 = carReg->field_34;
    raw >>= 16;
    if (field34 >= 0) {
        goto pos_angle_0;
    }
    adjusted = raw - 0x400;
    goto got_angle_0;

pos_angle_0:
    adjusted = raw + 0x400;

got_angle_0:
    adjustedReg = adjusted & 0xFFF;
    asm("" : "=r"(adjustedReg) : "0"(adjustedReg));
    fieldA4 = carReg->field_A4;
    angle = adjustedReg;
    if (fieldA4 < 0x321) {
        goto low_speed;
    }
    if (fieldA4 >= 0x709) {
        speed = 0x708;
    } else {
        speed = *(u16 *)&carReg->field_A4;
    }
    trig = func_80068568(func_8002A788((s16)rawArg, carReg->field_24));
    product = (s16)speed * trig;
    if (product >= 0) {
        goto nonneg_speed;
    }
    product += 0xFFFF;

nonneg_speed:
    tmp = product >> 16;
    goto speed_ready;

low_speed:
    trig = func_80068568(func_8002A788((s16)rawArg, carReg->field_24));
    product = trig << 1;
    product += trig;
    product <<= 3;
    product += trig;
    adjustedReg = product << 1;
    if (adjustedReg >= 0) {
        goto low_speed_nonneg;
    }
    adjustedReg += 0xFFF;

low_speed_nonneg:
    tmp = adjustedReg >> 12;

speed_ready:
    speed = tmp + 10;

make_vector:
    savedAngle = angle;
    asm("" : "=r"(savedAngle) : "0"(savedAngle));
    trig = func_80068568(savedAngle);
    product = speed << 16;
    angle = product >> 16;
    adjustedReg = trig * angle;
    if (adjustedReg >= 0) {
        goto x_nonneg;
    }
    adjustedReg += 0xFFF;

x_nonneg:
    x = adjustedReg >> 12;
    trig = func_80068634(savedAngle);
    adjustedReg = trig * angle;
    if (adjustedReg >= 0) {
        goto z_nonneg;
    }
    adjustedReg += 0xFFF;

z_nonneg:
    z = adjustedReg >> 12;
    asm("" : "=r"(z) : "0"(z));
    tmp = 0x1E;
    goto store_values;

mode_ge_2:
    if (mode >= 4) {
        goto mode_ge_4;
    }
    adjustedReg = *(u16 *)&carReg->field_B4;
    raw = 0xC00;
    raw -= adjustedReg;
    raw <<= 16;
    field34 = carReg->field_34;
    raw >>= 16;
    if (field34 >= 0) {
        goto pos_angle_2;
    }
    adjusted = raw - 0x400;
    goto got_angle_2;

pos_angle_2:
    adjusted = raw + 0x400;

got_angle_2:
    rawArg = adjusted & 0xFFF;
    angle = rawArg;
    trig = func_80068568(rawArg);
    tmp = trig * 20;
    if (tmp >= 0) {
        goto mode2_x_nonneg;
    }
    tmp += 0xFFF;

mode2_x_nonneg:
    x = tmp >> 12;
    trig = func_80068634(angle);
    tmp = trig * 20;
    if (tmp >= 0) {
        goto mode2_z_nonneg;
    }
    tmp += 0xFFF;

mode2_z_nonneg:
    z = tmp >> 12;
    tmp = 0xF;
    goto store_values;

mode_ge_4:
    adjustedReg = (u32)x >> 31;
    adjustedReg = x + adjustedReg;
    x = adjustedReg >> 1;
    adjustedReg = (u32)z >> 31;
    adjustedReg = z + adjustedReg;
    z = adjustedReg >> 1;
    tmp = 0xF;

store_values:
    carReg->motionTimer = tmp;
    carReg->velocityX = x;
    carReg->velocityZ = z;
}
