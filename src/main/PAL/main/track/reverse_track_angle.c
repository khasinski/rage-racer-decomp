#include "common.h"
#include "game/track.h"

/*
 * Track angle between point `pointIndex` and its successor (blend weight from
 * the current-point index g_PlayerSegmentWeight), returned as the opposite direction
 * (0x800 - angle).
 */
s32 GetReverseTrackAngle(s32 pointIndex) {
    s32 next = (pointIndex + 1) % g_TrackPointCount;

    return 0x800 - BlendAngle(g_TrackPoints[pointIndex].angle, g_TrackPoints[next].angle, g_PlayerSegmentWeight);
}

