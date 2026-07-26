#include "common.h"
#include "game/waypoint.h"

extern TrackWaypointRuntime D_801E4DF4[];

/* Counts how many of the 6 waypoint slots are active (active != 0). */
s32 func_80037BD4(void) {
    TrackWaypointRuntime *ptr = D_801E4DF4;
    s32 count = 0;
    s32 i = 5;

    do {
        i--;
        count += ptr->active != 0;
        ptr++;
    } while (i >= 0);

    return count;
}
