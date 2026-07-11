#include "common.h"
#include "game/car.h"

s32 func_80068568(s32 arg0);

void func_80038FF0(GameCarRuntime *car) {
    s32 value;
    s32 wave;
    s32 amplitude;
    register s32 product asm("$4");

    if (car->motionMode == 0) {
        return;
    }

    car->motionModeTimer--;
    if ((s16)car->motionModeTimer == 0) {
        car->motionMode = 0;
        car->field_92 = 0;
    }

    {
        register s32 timer asm("$5");

        timer = car->motionModeTimer;
        asm("" : "=r"(timer) : "0"(timer));
        product = timer * car->motionValue;
        if (product < 0) {
            product += 0x7F;
        }
        amplitude = product >> 7;

        wave = func_80068568(((timer * 3) << 12) / 30) * amplitude;
    }
    if (wave < 0) {
        wave += 0x7FF;
    }
    value = wave >> 11;

    switch (car->motionMode) {
    case 1:
    case 5:
        car->field_92 = value + amplitude;
        car->field_20 += car->field_92;
        car->field_92 = value + ((s32)(amplitude + ((u32)amplitude >> 31)) >> 1);
        car->field_28 += car->field_92 / 2;
        break;

    case 2:
        if (car->field_98 != 0) {
            break;
        }
        car->field_28 += value;
        break;

    case 3:
        car->field_92 = value + amplitude;
        car->field_20 += car->field_92;
        break;

    case 4:
        car->field_28 += value;
        break;
    }
}
