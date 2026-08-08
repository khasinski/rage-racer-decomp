#include "common.h"
#include <stdio.h>
#include "game/state.h"
#include "psyq/kernel.h"
#include "psyq/libc_internal.h"

void putchar(s32 ch) {
    u8 c;
    s32 value = ch;

    c = value;
    switch (0) { default:
    value = c;
    if (!(value == 9)) {
    if (value != 10) {
    } else {

    putchar(13);
    g_LibcOutColumn = 0;
    break;

    }
    } else {
    do {
        putchar(0x20);
    } while ((g_LibcOutColumn % 8) != 0);
    return;
    }

    if (g_LibcCtype[value] & 0x97) {
        g_LibcOutColumn++;
    }

    }
    BiosFileWrite(1, &c, 1);
}
