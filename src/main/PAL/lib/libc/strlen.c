#include "common.h"
#include <string.h>

s32 strlen(u8 *str) {
    s32 count = 0;

    if (str == 0) {
        return 0;
    }

    while (*str++ != 0) {
        count++;
    }

    return count;
}
