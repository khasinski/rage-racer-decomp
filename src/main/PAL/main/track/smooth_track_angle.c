#include "common.h"
#include "game/track.h"


s32 func_8002FAE8(s32 arg0, s32 arg1, s32 arg2);
s32 func_8002FB60(s32 arg0, s32 arg1);

/*
 * Smooths the track angle at `pointIndex` by blending it (half weight, 0x200)
 * with the angles two points behind and two points ahead (wrap-aware).
 */
s32 GameSmoothTrackAngle(s32 pointIndex, s32 weight) asm("func_8002FD9C");
s32 GameSmoothTrackAngle(s32 pointIndex, s32 weight) {
    s32 center;
    s32 prev_index;
    s32 prev;
    s32 left;
    s32 next_index;
    s32 next;
    s32 right;

    center = func_8002FB60(pointIndex, weight);

    prev_index = pointIndex - 2;
    if (prev_index < 0) {
        s32 tmp;
        tmp = g_TrackPointCount;
        tmp -= 2;
        prev_index = tmp + pointIndex;
    }

    prev = func_8002FB60(prev_index, weight);
    left = func_8002FAE8(center, prev, 0x200);

    next_index = (pointIndex + 2) % g_TrackPointCount;
    next = func_8002FB60(next_index, weight);
    right = func_8002FAE8(center, next, 0x200);

    return func_8002FAE8(left, right, 0x200);
}
