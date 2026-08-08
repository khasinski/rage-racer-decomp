#include "common.h"

typedef struct Func80043B18Entry {
    u8 pad[0x22];
    s16 value;
} Func80043B18Entry;

extern u16 g_TrackSectionCount;
extern Func80043B18Entry *g_TrackCameras;

s32 FindNearestTrackCamera(u8 *car, u16 rawValue) {
    s32 best;
    Func80043B18Entry *entry;
    s32 index;
    s32 selected;
    s32 span;
    s32 halfSpan;
    s32 target;
    s32 dist;
    s32 tmp;
    u32 candidate;

    entry = g_TrackCameras;
    best = 0x7FFFFFFF;
    rawValue = *(u16 *)&entry[0].value;
    dist = entry[0].value;
    index = 0;

    if (dist != -1) {
        tmp = g_TrackSectionCount;
        target = *(s16 *)(car + 0x78);
        tmp <<= 16;
        span = tmp >> 16;
        tmp = (u32)tmp >> 31;
        halfSpan = (span + tmp) >> 1;

        do {
            dist = (s16)rawValue;
            tmp = dist < target;
            if (tmp) {
                dist = target - dist;
            } else {
                dist = dist - target;
            }

            candidate = halfSpan < dist;
            if (candidate) {
                candidate = (u32)(span - dist);
            } else {
                candidate = (u32)dist;
            }

            dist = (s32)candidate;
            tmp = dist < best;
            if (tmp) {
                selected = index;
                best = dist;
            }

            tmp = -1;
            index++;
            rawValue = *(u16 *)&entry[index].value;
            dist = entry[index].value;
        } while (dist != tmp);
    }

    return selected;
}
