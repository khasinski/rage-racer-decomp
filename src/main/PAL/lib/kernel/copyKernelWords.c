#include "psyq/kernel.h"

void copyKernelWords(u32 *dst, u32 *src, u32 count, s32 arg3) {
    u32 i;

    for (i = 0; i < count; i++) {
        *dst++ = *src++;
    }
}
