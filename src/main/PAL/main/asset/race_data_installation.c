#include "common.h"
#include <stdio.h>
#include "game/asset.h"
#include "game/track.h"
#include "game/track_internal.h"
#include "game/car.h"
#include "game/race.h"




void InstallResourceData(void *data) {
    printf(g_MsgResOk);
}
void SetCarSpec(GameCarSpec *spec) {
    g_CarSpec = spec;
}

void InstallTrackEventData(void *resourceData) {
    register AssetAddress cursor asm("$2");
    s32 offset1;
    u8 *callArg;
    TrackEventOffsetBase base;
    TrackEventData *eventData;
    volatile TrackEventOffsets *offsets;

    eventData = resourceData;
    offsets = &eventData->offsets;
    cursor.offset = offsets->flybyScenery;
    offset1 = offsets->raceIntroCamera;
    base.offsets = offsets;
    g_TrackEventData = eventData;
    g_FlybySceneryData = (SceneryMotionData *)(base.bytes + cursor.offset);
    cursor.offset = offsets->routeScenery;
    g_RaceIntroCameraScript = (RaceIntroCameraScript *)(base.bytes + offset1);
    offset1 = offsets->pathSceneryRotation;
    g_RouteSceneryData = (SceneryMotionData *)(base.bytes + cursor.offset);
    cursor.offset = offsets->pathSceneryPosition;
    callArg = g_MsgEventOk;
    cursor.pointer = base.bytes + cursor.offset;
    base.bytes += offset1;
    g_PathSceneryPosData = cursor.pointer;
    g_PathSceneryRotData = (PathSceneryRotationData *)base.bytes;
    printf(callArg);
}
