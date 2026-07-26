#include "common.h"
#include "game/track.h"

extern s32 D_8009E6A8;
extern GameTrackPoint *D_8009E688;

/*
 * Linearly interpolates the centre-line XYZ between GameTrackPoint[pointIndex]
 * and its successor by `weight` (0..0x400), writing the result to out[0..2].
 * The +0x3FF/+0x7FF bias before the >>10 / >>11 shifts rounds toward zero.
 */
void func_8002FC84(s32 pointIndex, s32 *out, s32 weight) {
    s32 next = (pointIndex + 1) % D_8009E6A8;
    s32 inv = 0x400 - weight;
    GameTrackPoint *cur = &D_8009E688[pointIndex];
    GameTrackPoint *nxt = &D_8009E688[next];
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
