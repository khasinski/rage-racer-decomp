#include "common.h"

void MemFill(u8 *dst, u8 value, s32 count) asm("func_80068180");
u8 *MemCopy(u8 *dst, u8 *src, s32 count) asm("func_800681BC");

void MemFill(u8 *dst, u8 value, s32 count) {
    volatile s32 unused;
    s32 i = count - 1;

    if (count != 0) {
        s32 end = -1;

        do {
            *dst = value;
            i--;
            dst++;
        } while (i != end);
    }
}

u32 func_800681AC[4] __attribute__((section(".text"))) = {
    0x240A00A0,
    0x01400008,
    0x24090049,
    0,
};

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
