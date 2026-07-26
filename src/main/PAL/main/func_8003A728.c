#include "common.h"
#include "game/car.h"

/* g_Cars[0].field_68 / .field_6C - retail references both split symbols. */
extern s32 D_801F18BC;
extern s32 D_801F18C0;

/*
 * Ranks the first four cars by race progress (`field_68 + field_6C`) and
 * publishes the ordering into g_RankedCars: slot 0 the leader, slot 3 the
 * last of the four, slots 1/2 the middle pair in order. func_8003A974 reads
 * the result to rubber-band the AI.
 */
void func_8003A728(void) {
    s32 i;
    s32 offset;
    s32 maxValue;
    s32 minValue;
    s32 value;
    s32 sums[4];
    s32 *sumPtr;
    s16 indices[4];

    i = 0;
    sumPtr = sums;
    offset = 0;
    do {
        *sumPtr = *(s32 *)((u8 *)&D_801F18BC + offset) + *(s32 *)((u8 *)&D_801F18C0 + offset);
        offset += sizeof(GameCarRuntime);
        i++;
        sumPtr++;
    } while (i < 4);

    indices[0] = 0;
    indices[3] = 0;
    maxValue = sums[0];
    minValue = sums[0];
    for (i = 1; i < 4; i++) {
        value = sums[i];
        if (maxValue < value) {
            maxValue = value;
            indices[0] = i;
        } else if (value < minValue) {
            minValue = value;
            indices[3] = i;
        }
    }

    g_RankedCars[0] = &g_Cars[indices[0]];
    g_RankedCars[3] = &g_Cars[indices[3]];

    for (i = 0; i < 4; i++) {
        if ((i != indices[0]) && (i != indices[3])) {
            indices[1] = i;
            break;
        }
    }

    for (i = 0; i < 4; i++) {
        if ((i != indices[0]) && (i != indices[3]) && (i != indices[1])) {
            indices[2] = i;
            break;
        }
    }

    if (sums[indices[1]] > sums[indices[2]]) {
        g_RankedCars[1] = &g_Cars[indices[1]];
        g_RankedCars[2] = &g_Cars[indices[2]];
    } else {
        g_RankedCars[1] = &g_Cars[indices[2]];
        g_RankedCars[2] = &g_Cars[indices[1]];
    }
}
