#include <sys/types.h>
#include <ctype.h>

#include "common.h"

extern u_char g_LibcCtype[];

long tolower(long ch) {
    u_char value = ch;

    if (g_LibcCtype[(u8)value] & 1) {
        value = ch + 0x20;
    }

    return (u8)value;
}
