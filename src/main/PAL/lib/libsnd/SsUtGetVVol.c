#include "psyq/snd.h"

extern s16 *D_8009A588;

s16 SsUtGetVVol(s16 arg0, s16 *arg1, s16 *arg2) {
    s16 *ptr;
    s16 left;
    s16 right;

    if ((u16)arg0 < 24U) {
        ptr = &D_8009A588[arg0 * 8];
        left = ptr[0];
        right = ptr[1];
        *arg1 = left / 129;
        *arg2 = right / 129;
        return 0;
    }

    return -1;
}
