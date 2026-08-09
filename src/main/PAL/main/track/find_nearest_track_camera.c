#include "common.h"
#include "game/render.h"

#define GAME_TRACK_SECTION_COUNT_TYPE u16
#include "game/track_camera_internal.h"

s32 FindNearestTrackCamera(GameRenderObject *car) {
    s32 best;
    GameTrackCameraNode *entry;
    s32 index;
    s32 selected;
    s32 span;
    s32 halfSpan;
    s32 target;
    s32 dist;
    s32 tmp;
    u32 candidate;
    u16 rawValue;

    entry = g_TrackCameras;
    best = 0x7FFFFFFF;
    rawValue = *(u16 *)&entry[0].trackSection;
    dist = entry[0].trackSection;
    index = 0;

    if (dist != -1) {
        tmp = g_TrackSectionCount;
        target = *(s16 *)&car->trackSection;
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
            rawValue = *(u16 *)&entry[index].trackSection;
            dist = entry[index].trackSection;
        } while (dist != tmp);
    }

    return selected;
}
