#include "common.h"
#include "game/race.h"


s32 GameCanSelectNextCourse(void) asm("func_80053688");

s32 GameCanSelectNextCourse(void) {
    s32 limit;

    if (g_GrandPrixMode != 0) {
        if (g_SeriesSelection != 0) {
            limit = (g_GrandPrixClass < 2) ? 6 : 7;
        } else {
            limit = (g_GrandPrixClass < 2) ? 2 : 3;
        }
    } else if (g_AdvancedSeriesUnlocked != 0) {
        limit = (g_MaxClassReached[1] < 2) ? 6 : 7;
    } else {
        limit = (g_MaxClassReached[0] < 2) ? 2 : 3;
    }

    return g_CourseIndex < limit;
}
