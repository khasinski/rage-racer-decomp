#include "common.h"
#include "game/save_internal.h"


s32 ComputeClassGrade(void) {
    u8 *ptr;
    s32 value;
    u8 *end;
    u8 extra;

    ptr = g_CourseProgress->bestPlace;
    value = 0;
    if (g_CourseProgress->unlockPending != 0) {
        return 0;
    }

    end = ptr + 3;
    do {
        value += *ptr++;
    } while ((s32)ptr < (s32)end);

    extra = g_CourseProgress->bestPlace[3];
    if (extra == 0xFF) {
        value++;
    } else {
        value += extra;
    }

    value -= 3;
    if (value >= 4) {
        value = 0;
    }
    return value;
}
