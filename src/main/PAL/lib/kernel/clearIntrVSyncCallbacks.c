#include "psyq/kernel.h"

void clearIntrVSyncCallbacks(u32 *dst, s32 count) {
    volatile s32 unused;
    s32 i = count - 1;

    if (count != 0) {
        do {
            *dst = 0;
            i--;
            dst++;
        } while (i != -1);
    }
}
