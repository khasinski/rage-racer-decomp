#include "common.h"

s32 LibcStrlen(u8 *arg0) asm("func_80063C08");
s32 LibcStrlen(u8 *arg0) {
    s32 count = 0;

    if (arg0 == 0) {
        return 0;
    }

    while (*arg0++ != 0) {
        count++;
    }

    return count;
}
