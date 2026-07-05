#include "common.h"
#include "game/car.h"

extern u8 D_8007C474[];
extern GameCarEntry *D_8019C7C8;

s32 func_8001785C(s32 car_index) {
    return D_8019C7C8[car_index].modelVariant + D_8007C474[car_index];
}
