#include "common.h"
#include "game/waypoint.h"

extern s32 D_8009E6D4;
extern s32 D_8009E6DC;

s32 func_80037808(TrackWaypointRuntime *arg0) {
    s32 center_x = D_8009E6D4;
    s32 x = arg0->x;
    s32 ret = 0;

    if ((center_x - 0x40) < x) {
        s32 max_x = center_x + 0x40;

        if (x < max_x) {
            s32 center_y = D_8009E6DC;
            s32 y = arg0->y;

            if ((center_y - 0x40) < y) {
                s32 max_y = center_y + 0x40;

                ret = y < max_y;
            }
        }
    }

    return ret;
}
