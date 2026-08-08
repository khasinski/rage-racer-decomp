#include <sys/types.h>
#include <string.h>

void memcpy(u_char *dst, u_char *src, long count) {
    long i;

    i = count;
    while (i--) {
        *dst = *src;
        src++;
        dst++;
    }
}
