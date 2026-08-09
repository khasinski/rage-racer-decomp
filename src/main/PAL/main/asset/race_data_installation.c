#include "common.h"
#include <stdio.h>
#include "game/track.h"
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
    TrackEventData *eventData;
    volatile TrackEventOffsets *offsets;

    eventData = resourceData;
    offsets = &eventData->offsets;
    offset0 = offsets->flybyScenery;
    offset1 = offsets->raceIntroCamera;
    base = (u8 *)offsets;
    g_TrackEventData = eventData;
    g_FlybySceneryData = (SceneryMotionData *)(base + offset0);
    offset0 = offsets->routeScenery;
    g_RaceIntroCameraScript = (RaceIntroCameraScript *)(base + offset1);
    offset1 = offsets->pathSceneryRotation;
    g_RouteSceneryData = (SceneryMotionData *)(base + offset0);
    offset0 = offsets->pathSceneryPosition;
    callArg = g_MsgEventOk;
    offset0 = (s32)(base + offset0);
    base += offset1;
    g_PathSceneryPosData = (PathSceneryPositionData *)offset0;
    g_PathSceneryRotData = (PathSceneryRotationData *)base;
    printf(callArg);
}
