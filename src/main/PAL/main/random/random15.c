#include "common.h"
#include "game/random.h"

extern u32 g_RandomSeed;

s32 Random15(void) {
    u32 value = g_RandomSeed;

    value *= 0x41C64E6D;
    value += 0x3039;
    g_RandomSeed = value;
    return (value >> 0x10) & 0x7FFF;
}
