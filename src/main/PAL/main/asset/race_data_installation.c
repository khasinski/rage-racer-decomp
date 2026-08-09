#include "common.h"
#include <stdio.h>
#include "game/track.h"
#define GAME_SCENERY_DATA_QUALIFIER volatile
#include "game/track_internal.h"
#include "game/car.h"
#include "game/race.h"




void InstallResourceData(void) {
    printf(g_MsgResOk);
}
void SetCarSpec(GameCarSpec *spec) {
    g_CarSpec = spec;
}

void InstallTrackEventData(void *resourceData) {
    register s32 offset0 asm("$2");
    s32 offset1;
    u8 *callArg;
    u8 *base;
    u8 *rawEventData;
    TrackEventData *eventData;

    rawEventData = resourceData;
    eventData = resourceData;
    offset0 = eventData->offsets.flybyScenery;
    offset1 = eventData->offsets.raceIntroCamera;
    base = (u8 *)&eventData->offsets;
    g_TrackEventData = eventData;
    g_FlybySceneryData = (SceneryMotionData *)(base + offset0);
    offset0 = *(s32 *)(rawEventData + 0xB64);
    g_RaceIntroCameraScript = base + offset1;
    offset1 = *(s32 *)(rawEventData + 0xB70);
    g_RouteSceneryData = (SceneryMotionData *)(base + offset0);
    offset0 = *(s32 *)(rawEventData + 0xB6C);
    callArg = g_MsgEventOk;
    offset0 = (s32)(base + offset0);
    base += offset1;
    g_PathSceneryPosData = (PathSceneryPositionData *)offset0;
    g_PathSceneryRotData = (PathSceneryRotationData *)base;
    printf(callArg);
}
