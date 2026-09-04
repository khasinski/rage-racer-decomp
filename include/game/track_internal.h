#ifndef GAME_TRACK_INTERNAL_H
#define GAME_TRACK_INTERNAL_H

#include "common.h"
#include "game/track.h"
#include "game/track_camera_internal.h"

typedef struct CourseObject {
    s16 modelId;
    s16 field2;
    s32 x;
    s32 y;
    s32 z;
    s32 flags;
} CourseObject;

typedef struct CourseObjectTable {
    u32 count;
    CourseObject objects[1];
} CourseObjectTable;

typedef struct StartGridSceneryStep {
    s16 x;
    s16 y;
} StartGridSceneryStep;

typedef struct PathSceneryClock {
    s16 posFrame;
    s16 rotFrame;
    s16 posRateNeg;
    s16 rotRateNeg;
} PathSceneryClock;

typedef char PathSceneryClockSizeCheck[
    sizeof(PathSceneryClock) == 0x8 ? 1 : -1];

typedef union PathSceneryClockAddress {
    PathSceneryClock *clock;
    u16 *halfwords;
} PathSceneryClockAddress;

typedef struct PathSceneryTransform {
    Block16 position;
    SVec rotation;
} PathSceneryTransform;

typedef char PathSceneryTransformSizeCheck[
    sizeof(PathSceneryTransform) == 0x18 ? 1 : -1];

extern GameTrackArcCenter *g_TrackArcCenters;
extern s32 g_EnvScriptClock;
extern CourseObject *g_CourseObjects;
extern s32 g_CourseObjectCount;
extern StartGridSceneryStep g_StartGridSceneryStep[];
extern PathSceneryClock g_PathSceneryClock;
extern PathSceneryTransform g_PathSceneryTransform;
extern s32 g_TrackLength;
extern TrackEventData *g_TrackEventData;

extern SceneryMotionData *volatile g_RouteSceneryData;
extern PathSceneryRotationData *volatile g_PathSceneryRotData;
extern SceneryMotionData *volatile g_FlybySceneryData;
extern PathSceneryPositionData *volatile g_PathSceneryPosData;

extern s32 g_RouteSceneryClock;
extern s16 g_RouteSceneryKeyIndex;
extern s32 g_RouteSceneryRotY;
extern SceneryMotionKeyframe *g_RouteSceneryKeyframe;

extern s32 g_EnvScriptLength;
extern GameEnvironmentCue *g_EnvScriptCursor;

extern s32 g_SkyRowBase;

extern FlybySceneryState g_FlybyScenery;

#endif
