#include <sys/types.h>
#include <string.h>

long strncmp(u_char *lhs, u_char *rhs, long len) {
    long left;
    long right;

    if ((lhs == 0) || (rhs == 0)) {
        if (lhs == rhs) {
            return 0;
        }
        if (lhs == 0) {
            return -1;
        }
        return 1;
    }

    len--;
    if (len < 0) {
        return 0;
    }

for (;;) {
    left = *lhs;
    right = *rhs++;
    if (left != right) {
    } else {
    lhs++;
    if (left == 0) {
        return 0;
    }
    len--;
    if (len >= 0) {
        continue;
    }

    }
break;
}
    if (len < 0) {
        return 0;
    }
    return *lhs - rhs[-1];
}
