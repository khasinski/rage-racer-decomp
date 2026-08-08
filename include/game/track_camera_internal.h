#ifndef GAME_TRACK_CAMERA_INTERNAL_H
#define GAME_TRACK_CAMERA_INTERNAL_H

#include "common.h"

typedef union GameTrackCameraData {
    struct {
        s32 x;
        s32 y;
        s32 z;
        s32 blend;
    } world;
    struct {
        s32 pitch;
        s32 yaw;
        s32 roll;
        s32 distance;
    } orientation;
    s32 value[4];
} GameTrackCameraData;

typedef struct GameTrackCameraNode {
    GameTrackCameraData data;
    s32 offset[3];
    s32 duration;
    s16 mode;
    s16 trackSection;
} GameTrackCameraNode;

#ifndef GAME_TRACK_CAMERA_TYPE
#define GAME_TRACK_CAMERA_TYPE GameTrackCameraNode
#endif
extern GAME_TRACK_CAMERA_TYPE *g_TrackCameras;
#undef GAME_TRACK_CAMERA_TYPE
#ifndef GAME_TRACK_SECTION_COUNT_TYPE
#define GAME_TRACK_SECTION_COUNT_TYPE s16
#endif
extern GAME_TRACK_SECTION_COUNT_TYPE g_TrackSectionCount;
#undef GAME_TRACK_SECTION_COUNT_TYPE

#endif
