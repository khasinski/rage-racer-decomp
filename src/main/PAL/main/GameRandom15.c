#include "common.h"
#include "game/random.h"

extern u32 D_8009B9A8;

s32 GameRandom15(void) {
    u32 value = D_8009B9A8;

    value *= 0x41C64E6D;
    value += 0x3039;
    D_8009B9A8 = value;
    return (value >> 0x10) & 0x7FFF;
}
