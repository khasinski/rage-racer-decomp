#include "common.h"
#include "game/race.h"

extern s16 D_801E4DAC;
extern s16 D_801E4034;
extern s16 D_8019CAC0;
extern s32 D_801E7730;
extern s32 D_801E772C;

s32 GameCanSelectNextCourse(void) asm("func_80053688");

s32 GameCanSelectNextCourse(void) {
    s32 limit;

    if (D_801E4DAC != 0) {
        if (D_801E4034 != 0) {
            limit = (g_GrandPrixClass < 2) ? 6 : 7;
        } else {
            limit = (g_GrandPrixClass < 2) ? 2 : 3;
        }
    } else if (D_8019CAC0 != 0) {
        limit = (D_801E7730 < 2) ? 6 : 7;
    } else {
        limit = (D_801E772C < 2) ? 2 : 3;
    }

    return g_CourseIndex < limit;
}
