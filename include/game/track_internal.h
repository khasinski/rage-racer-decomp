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

extern GameTrackArcCenter *g_TrackArcCenters;
extern s32 g_EnvScriptClock;
extern CourseObject *g_CourseObjects;
extern s32 g_CourseObjectCount;
extern StartGridSceneryStep g_StartGridSceneryStep[];
extern PathSceneryClock g_PathSceneryClock;
extern Block16 g_PathSceneryX;
#ifndef GAME_PATH_SCENERY_ROT_DECL
#define GAME_PATH_SCENERY_ROT_DECL extern SVec g_PathSceneryRot
#endif
GAME_PATH_SCENERY_ROT_DECL;
#undef GAME_PATH_SCENERY_ROT_DECL
extern PathSceneryCursor g_PathSceneryPosCursor asm("g_PathSceneryPosPhase");
extern PathSceneryCursor g_PathSceneryRotCursor asm("g_PathSceneryRotPhase");

extern s32 g_TrackLength;
extern u8 *g_TrackEventData;

#ifndef GAME_SCENERY_DATA_QUALIFIER
#define GAME_SCENERY_DATA_QUALIFIER
#endif
extern u8 *GAME_SCENERY_DATA_QUALIFIER g_RouteSceneryData;
extern u8 *GAME_SCENERY_DATA_QUALIFIER g_PathSceneryRotData;
extern u8 *GAME_SCENERY_DATA_QUALIFIER g_FlybySceneryData;
extern u8 *GAME_SCENERY_DATA_QUALIFIER g_PathSceneryPosData;
#undef GAME_SCENERY_DATA_QUALIFIER

#ifndef GAME_ROUTE_SCENERY_QUALIFIER
#define GAME_ROUTE_SCENERY_QUALIFIER
#endif
extern GAME_ROUTE_SCENERY_QUALIFIER s32 g_RouteSceneryClock;
extern GAME_ROUTE_SCENERY_QUALIFIER s16 g_RouteSceneryKeyIndex;
extern GAME_ROUTE_SCENERY_QUALIFIER s32 g_RouteSceneryRotY;
#undef GAME_ROUTE_SCENERY_QUALIFIER
#ifndef GAME_ROUTE_KEYFRAME_TYPE
#define GAME_ROUTE_KEYFRAME_TYPE void
#endif
extern GAME_ROUTE_KEYFRAME_TYPE *g_RouteSceneryKeyframe;
#undef GAME_ROUTE_KEYFRAME_TYPE

#ifndef GAME_ENV_SCRIPT_LENGTH_TYPE
#define GAME_ENV_SCRIPT_LENGTH_TYPE s32
#endif
extern GAME_ENV_SCRIPT_LENGTH_TYPE g_EnvScriptLength;
#undef GAME_ENV_SCRIPT_LENGTH_TYPE
#ifndef GAME_ENV_SCRIPT_CURSOR_TYPE
#define GAME_ENV_SCRIPT_CURSOR_TYPE void
#endif
extern GAME_ENV_SCRIPT_CURSOR_TYPE *g_EnvScriptCursor;
#undef GAME_ENV_SCRIPT_CURSOR_TYPE

#ifndef GAME_SKY_ROW_BASE_TYPE
#define GAME_SKY_ROW_BASE_TYPE s32
#endif
extern GAME_SKY_ROW_BASE_TYPE g_SkyRowBase;
#undef GAME_SKY_ROW_BASE_TYPE

#ifndef GAME_FLYBY_SCENERY_DECL
#define GAME_FLYBY_SCENERY_DECL extern u8 g_FlybyScenery[]
#endif
GAME_FLYBY_SCENERY_DECL;
#undef GAME_FLYBY_SCENERY_DECL
#ifndef GAME_ENV_PALETTE_DECL
#define GAME_ENV_PALETTE_DECL extern u8 *g_EnvPaletteTable
#endif
GAME_ENV_PALETTE_DECL;
#undef GAME_ENV_PALETTE_DECL

#endif
