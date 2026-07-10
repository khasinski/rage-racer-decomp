#include "common.h"

extern s32 D_801F18BC;
extern s32 D_801F18C0;
extern u8 D_801F1854[];
extern u8 *D_801E40BC;
extern u8 *D_801E40C0;
extern u8 *D_801E40C4;
extern u8 *D_801E40C8;

#define GET_BLOCK(index) (D_801F1854 + ((index) * 0x19C))

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
        offset += 0x19C;
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

    D_801E40BC = GET_BLOCK(indices[0]);
    D_801E40C8 = GET_BLOCK(indices[3]);

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
        D_801E40C0 = GET_BLOCK(indices[1]);
        D_801E40C4 = GET_BLOCK(indices[2]);
    } else {
        D_801E40C0 = GET_BLOCK(indices[2]);
        D_801E40C4 = GET_BLOCK(indices[1]);
    }
}

#undef GET_BLOCK
