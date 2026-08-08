#include "common.h"
#include <ctype.h>
#include "psyq/libc_internal.h"

s32 toupper(s32 ch) {
    u8 value = ch;

    if (g_LibcCtype[(u8)value] & 2) {
        value = ch - 0x20;
    }

    return (u8)value;
}
