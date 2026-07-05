#include "psyq/kernel.h"

extern volatile u16 *D_8009A574;

s32 ResetRCnt(s32 arg0) {
    s32 index;

    index = arg0 & 0xFFFF;
    if (index >= 3) {
        return 0;
    }
    D_8009A574[index * 8] = 0;
    return 1;
}
