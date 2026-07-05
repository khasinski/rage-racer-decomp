#include "common.h"

u8 *MemCopy(u8 *dst, u8 *src, s32 count) asm("func_800681BC");

u8 *MemCopy(u8 *dst, u8 *src, s32 count) {
    u8 *ret;

    if (dst == 0) {
        return 0;
    }

    ret = dst;
    if (count > 0) {
        do {
            *dst = *src;
            src++;
            count--;
            dst++;
        } while (count > 0);
    }

    return ret;
}
