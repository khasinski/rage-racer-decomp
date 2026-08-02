#include "common.h"
#include "game/track.h"

extern s32 g_PlayerSegmentWeight asm("D_8009E70C");

s32 func_8002FAE8(s32 arg0, s32 arg1, s32 arg2);

/*
 * Track angle between point `pointIndex` and its successor (blend weight from
 * the current-point index g_PlayerSegmentWeight), returned as the opposite direction
 * (0x800 - angle).
 */
s32 GetReverseTrackAngle(s32 pointIndex) asm("func_8002FBEC");
s32 GetReverseTrackAngle(s32 pointIndex) {
    s32 next = (pointIndex + 1) % g_TrackPointCount;

    return 0x800 - func_8002FAE8(g_TrackPoints[pointIndex].angle, g_TrackPoints[next].angle, g_PlayerSegmentWeight);
}
