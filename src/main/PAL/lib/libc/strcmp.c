#include <sys/types.h>
#include <string.h>

long strcmp(u8 *lhs, u8 *rhs) {
    long left;

    if ((lhs == 0) || (rhs == 0)) {
        if (lhs == rhs) {
            return 0;
        }
        if (lhs == 0) {
            return -1;
        }
        return 1;
    }

    while ((left = *lhs) == *rhs++) {
        if (left == 0) {
            return 0;
        }
        lhs++;
    }

    return *lhs - rhs[-1];
}
