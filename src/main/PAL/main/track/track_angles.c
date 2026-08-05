#include "common.h"

/*
 * Wrap-aware angle blend in the 12-bit (0..0xFFF) angle space: blends angleA
 * toward angleB by `weight` (0..0x400) taking the shorter way around the
 * 0x1000 circle (the 0x801 test unwraps one operand by +0x1000). Returns the
 * blended angle masked to 12 bits.
 */
s32 BlendAngle(s32 angleA, s32 angleB, s32 weight) {
    s32 lhs = angleA & 0xFFF;
    s32 rhs = angleB & 0xFFF;
    s32 inv = 0x400 - weight;
    s32 sum;

    if (rhs < lhs) {
        if (lhs - rhs >= 0x801) {
            rhs += 0x1000;
        }
    } else if (rhs - lhs >= 0x801) {
        lhs += 0x1000;
    }

    sum = lhs * inv + rhs * weight;
    if (sum < 0) {
        sum += 0x3FF;
    }

    return (sum >> 10) & 0xFFF;
}

#include "common.h"
#include "game/track.h"

/* Interpolates the track angle between point `pointIndex` and its successor by `weight`. */
s32 InterpolateTrackAngle(s32 pointIndex, s32 weight) {
    s32 next = (pointIndex + 1) % g_TrackPointCount;

    return BlendAngle(g_TrackPoints[pointIndex].angle, g_TrackPoints[next].angle, weight);
}

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

#include "common.h"
#include "game/track.h"

/*
 * Linearly interpolates the centre-line XYZ between GameTrackPoint[pointIndex]
 * and its successor by `weight` (0..0x400), writing the result to out[0..2].
 * The +0x3FF/+0x7FF bias before the >>10 / >>11 shifts rounds toward zero.
 */
void InterpolateTrackPoint(s32 pointIndex, s32 *out, s32 weight) {
    s32 next = (pointIndex + 1) % g_TrackPointCount;
    s32 inv = 0x400 - weight;
    GameTrackPoint *cur = &g_TrackPoints[pointIndex];
    GameTrackPoint *nxt = &g_TrackPoints[next];
    s32 sum;

    sum = (cur->x * inv) + (nxt->x * weight);
    if (sum < 0) {
        sum += 0x3FF;
    }
    out[0] = sum >> 10;

    sum = (inv * cur->y) + (weight * nxt->y);
    if (sum < 0) {
        sum += 0x7FF;
    }
    out[1] = sum >> 11;

    sum = (cur->z * inv) + (nxt->z * weight);
    if (sum < 0) {
        sum += 0x3FF;
    }
    out[2] = sum >> 10;
}

#include "common.h"
#include "game/track.h"

s32 InterpolateTrackAngle(s32 arg0, s32 arg1);

/*
 * Smooths the track angle at `pointIndex` by blending it (half weight, 0x200)
 * with the angles two points behind and two points ahead (wrap-aware).
 */
s32 SmoothTrackAngle(s32 pointIndex, s32 weight) {
    s32 center;
    s32 prev_index;
    s32 prev;
    s32 left;
    s32 next_index;
    s32 next;
    s32 right;

    center = InterpolateTrackAngle(pointIndex, weight);

    prev_index = pointIndex - 2;
    if (prev_index < 0) {
        s32 tmp;
        tmp = g_TrackPointCount;
        tmp -= 2;
        prev_index = tmp + pointIndex;
    }

    prev = InterpolateTrackAngle(prev_index, weight);
    left = BlendAngle(center, prev, 0x200);

    next_index = (pointIndex + 2) % g_TrackPointCount;
    next = InterpolateTrackAngle(next_index, weight);
    right = BlendAngle(center, next, 0x200);

    return BlendAngle(left, right, 0x200);
}
