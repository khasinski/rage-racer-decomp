#include "psyq/kernel.h"

extern volatile u32 *D_8009A570;
extern u32 D_8009A578[];

s32 StartRCnt(s32 arg0) {
    s32 index;

    index = arg0 & 0xFFFF;
    D_8009A570[1] |= D_8009A578[index];
    return index < 3;
}
