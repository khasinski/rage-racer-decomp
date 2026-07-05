#include "common.h"

extern u8 * volatile D_8009E67C;

s32 func_800214B8(void) {
    u8 *ptr;
    s32 value;
    u8 *end;
    u8 extra;

    ptr = D_8009E67C;
    value = 0;
    if (*(s16 *)(ptr + 4) != 0) {
        return 0;
    }

    end = ptr + 3;
    do {
        value += *ptr++;
    } while ((s32)ptr < (s32)end);

    extra = D_8009E67C[3];
    if (extra == 0xFF) {
        value++;
    } else {
        value += extra;
    }

    value -= 3;
    if (value >= 4) {
        value = 0;
    }
    return value;
}
