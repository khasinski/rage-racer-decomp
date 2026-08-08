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

