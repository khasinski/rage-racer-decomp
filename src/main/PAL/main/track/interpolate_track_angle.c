#include "common.h"
#include "game/track.h"


s32 func_8002FAE8(s32 arg0, s32 arg1, s32 arg2);

/* Interpolates the track angle between point `pointIndex` and its successor by `weight`. */
s32 InterpolateTrackAngle(s32 pointIndex, s32 weight) asm("func_8002FB60");
s32 InterpolateTrackAngle(s32 pointIndex, s32 weight) {
    s32 next = (pointIndex + 1) % g_TrackPointCount;

    return func_8002FAE8(g_TrackPoints[pointIndex].angle, g_TrackPoints[next].angle, weight);
}
