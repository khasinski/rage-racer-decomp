#include "common.h"
#include <stdio.h>
#include "game/track.h"
#include "game/car.h"
#include "game/race.h"

extern u8 *volatile g_RouteSceneryData;

extern u8 *volatile g_PathSceneryRotData;

extern u8 *volatile g_FlybySceneryData;

extern u8 *volatile g_PathSceneryPosData;




void InstallResourceData(void) {
    printf((u8 *)g_MsgResOk);
}
void SetCarSpec(u32 spec) {
    g_CarSpec = (GameCarSpec *)spec;
}

void InstallTrackEventData(u8 *eventData) {
    register s32 offset0 asm("$2");
    s32 offset1;
    u8 *callArg;
    u8 *base;

    offset0 = *(s32 *)(eventData + 0xB78);
    offset1 = *(s32 *)(eventData + 0xB68);
    base = eventData + 0xB64;
    g_TrackEventData = eventData;
    g_FlybySceneryData = base + offset0;
    offset0 = *(s32 *)(eventData + 0xB64);
    g_RaceIntroCameraScript = base + offset1;
    offset1 = *(s32 *)(eventData + 0xB70);
    g_RouteSceneryData = base + offset0;
    offset0 = *(s32 *)(eventData + 0xB6C);
    callArg = g_MsgEventOk;
    offset0 = (s32)(base + offset0);
    base += offset1;
    g_PathSceneryPosData = (u8 *)offset0;
    g_PathSceneryRotData = base;
    printf((u8 *)callArg);
}
