#include "psyq/kernel.h"

void copyKernelWords(u_long *dst, u_long *src, u_long count, long arg3) {
    u_long i;

    for (i = 0; i < count; i++) {
        *dst++ = *src++;
    }
}
