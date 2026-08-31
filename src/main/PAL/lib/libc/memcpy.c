#include <sys/types.h>
#include <string.h>

void memcpy(u8 *dest, u8 *src, long count) {
    long i;

    i = count;
    while (i--) {
        *dest = *src;
        src++;
        dest++;
    }
}
