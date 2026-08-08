#include <sys/types.h>
#include <ctype.h>

#include "common.h"
#include "psyq/libc_internal.h"

long tolower(long ch) {
    u_char value = ch;

    if (g_LibcCtype[(u8)value] & 1) {
        value = ch + 0x20;
    }

    return (u8)value;
}
