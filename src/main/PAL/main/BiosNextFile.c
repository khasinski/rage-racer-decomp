#include "common.h"
#include "game/random.h"

u32 func_800632A0[4] __attribute__((section(".text"))) = {
    0x240A00B0,
    0x01400008,
    0x24090043,
    0,
};

extern u32 D_8009B9A8;

s32 GameRandom15(void) {
    u32 value = D_8009B9A8;

    value *= 0x41C64E6D;
    value += 0x3039;
    D_8009B9A8 = value;
    return (value >> 0x10) & 0x7FFF;
}
