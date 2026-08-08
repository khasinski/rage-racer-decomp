#include "common.h"
#include <stdio.h>
#include "game/state.h"

void puts(u8 *str) {
    u8 *ptr;
    s32 value;

    ptr = str;
    if (ptr == 0) {
        ptr = g_LibcNullText;
    }

    while (value = *ptr++, value != 0) {
        putchar(value);
    }
}
