#include "common.h"
#include <string.h>

u8 *memchr(u8 *buf, s32 ch, s32 len) {
    switch (0) { default:
    if (buf == 0) {
        return 0;
    }
    if (len <= 0) {
        return 0;
    }

    break;

found:
    return buf - 1;

    }

    len--;
    if (len < 0) {
        return 0;
    }

    ch &= 0xFF;
    do {
        if (*buf++ == ch) {
            goto found;
        }
        len--;
    } while (len >= 0);

    return 0;
}
