#include "common.h"

extern u16 D_8019C9A4;
extern u8 *D_8019C7CC;

s32 func_80043B18(u8 *arg0) {
    register s32 best asm("$9");
    register u8 *entry asm("$6");
    register s32 index asm("$7");
    register s32 selected asm("$11");
    register s32 span asm("$8");
    register s32 halfSpan asm("$10");
    register s32 target asm("$4");
    register s32 dist asm("$3");
    register u32 rawValue asm("$5");
    register s32 tmp asm("$2");
    s8 framePad[16];

    asm volatile("" : "=m"(framePad));

    best = 0x7FFFFFFF;
    asm volatile("" : "=r"(best) : "0"(best));
    entry = D_8019C7CC;
    tmp = -1;
    dist = *(s16 *)(entry + 0x22);
    asm volatile("" : "=r"(dist) : "0"(dist));
    rawValue = *(volatile u16 *)(entry + 0x22);
    index = 0;

    if (dist != tmp) {
        tmp = D_8019C9A4;
        target = *(s16 *)(arg0 + 0x78);
        tmp <<= 16;
        span = tmp >> 16;
        tmp = (u32)tmp >> 31;
        tmp = span + tmp;
        halfSpan = tmp >> 1;

        do {
            tmp = rawValue << 16;
            dist = tmp >> 16;
            tmp = dist < target;
            if (tmp) {
                dist = target - dist;
            } else {
                dist = dist - target;
            }

            tmp = halfSpan < dist;
            if (tmp) {
                tmp = span - dist;
            } else {
                tmp = dist;
            }

            dist = tmp;
            tmp = dist < best;
            entry += 0x24;
            if (tmp) {
                selected = index;
                best = dist;
            }

            tmp = -1;
            dist = *(s16 *)(entry + 0x22);
            asm volatile("" : "=r"(dist) : "0"(dist));
            rawValue = *(volatile u16 *)(entry + 0x22);
        } while (dist != tmp, index++, dist != tmp);
    }

    return selected;
}
