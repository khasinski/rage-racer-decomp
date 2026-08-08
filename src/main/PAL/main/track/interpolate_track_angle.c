#include "common.h"
#include "game/track.h"

/* Interpolates the track angle between point `pointIndex` and its successor by `weight`. */
s32 InterpolateTrackAngle(s32 pointIndex, s32 weight) {
    s32 next = (pointIndex + 1) % g_TrackPointCount;

    return BlendAngle(g_TrackPoints[pointIndex].angle, g_TrackPoints[next].angle, weight);
}

