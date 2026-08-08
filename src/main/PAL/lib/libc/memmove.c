#include "common.h"
#include <string.h>

void *memmove(u8 *dest, u8 *src, s32 count) {
    if (dest >= src) {
        while (count-- > 0) {
            dest[count] = src[count];
        }
    } else {
        while (count-- > 0) {
            *dest++ = *src++;
        }
    }

    return dest;
}
