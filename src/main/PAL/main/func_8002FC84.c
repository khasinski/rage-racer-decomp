#include "common.h"
#include "game/track.h"

extern s32 D_8009E6A8;
extern GameTrackPoint *D_8009E688;

void func_8002FC84(s32 arg0, s32 *out, s32 weight) {
    s32 next = (arg0 + 1) % D_8009E6A8;
    s32 inv = 0x400 - weight;
    GameTrackPoint *cur = &D_8009E688[arg0];
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
