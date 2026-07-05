#include "common.h"

typedef struct {
    s32 field0;
    s32 field4;
    s16 pad8;
    s16 angle;
    u8 pad_c[0xC];
} Entry8009E688;

extern s32 D_8009E6A8;
extern Entry8009E688 *D_8009E688;

void func_8002FC84(s32 arg0, s32 *out, s32 weight) {
    s32 next = (arg0 + 1) % D_8009E6A8;
    s32 inv = 0x400 - weight;
    Entry8009E688 *cur = &D_8009E688[arg0];
    Entry8009E688 *nxt = &D_8009E688[next];
    s32 sum;

    sum = (cur->field0 * inv) + (nxt->field0 * weight);
    if (sum < 0) {
        sum += 0x3FF;
    }
    out[0] = sum >> 10;

    sum = (inv * cur->pad8) + (weight * nxt->pad8);
    if (sum < 0) {
        sum += 0x7FF;
    }
    out[1] = sum >> 11;

    sum = (cur->field4 * inv) + (nxt->field4 * weight);
    if (sum < 0) {
        sum += 0x3FF;
    }
    out[2] = sum >> 10;
}
