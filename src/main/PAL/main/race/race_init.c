#include "common.h"
#include "game/track.h"
#include "game/car.h"
s32 InitSoundWithVab(void);

extern u8 g_MsgResOk[];



extern u8 g_MsgEventOk[];

extern u8 *volatile g_RaceIntroCameraScript;

extern u8 *volatile g_RouteSceneryData;


extern u8 *volatile g_PathSceneryRotData;

extern u8 *volatile g_FlybySceneryData;

extern u8 *volatile g_PathSceneryPosData;

extern u8 g_MsgSoundError;

extern u8 g_MsgInitSoundOk;


extern u8 g_MsgInitEngineOk;

void LoadExtraVabSlotWithTable(void);

void SetEffectVoicesEnabled(s32 arg0);

void SetReverbPreset(s32 arg0, s32 arg1, s32 arg2);

void InstallResourceData(void);
void InstallResourceData(void) {
    DebugPrintf(g_MsgResOk);
}

void SetCarSpec(u32 arg0);
void SetCarSpec(u32 arg0) {
    g_CarSpec = (GameCarSpec *)arg0;
}

void InstallTrackEventData(u8 *arg0);
void InstallTrackEventData(u8 *arg0) {
    register s32 offset0 asm("$2");
    s32 offset1;
    u8 *callArg;
    u8 *base;

    offset0 = *(s32 *)(arg0 + 0xB78);
    offset1 = *(s32 *)(arg0 + 0xB68);
    base = arg0 + 0xB64;
    g_TrackEventData = arg0;
    g_FlybySceneryData = base + offset0;
    offset0 = *(s32 *)(arg0 + 0xB64);
    g_RaceIntroCameraScript = base + offset1;
    offset1 = *(s32 *)(arg0 + 0xB70);
    g_RouteSceneryData = base + offset0;
    offset0 = *(s32 *)(arg0 + 0xB6C);
    callArg = g_MsgEventOk;
    offset0 = (s32)(base + offset0);
    base += offset1;
    g_PathSceneryPosData = (u8 *)offset0;
    g_PathSceneryRotData = base;
    DebugPrintf(callArg);
}

void InitSoundSystem(void);
void InitSoundSystem(void) {
    if (InitSoundWithVab() != 0) {
        DebugPrintf(&g_MsgSoundError);
    }
    DebugPrintf(&g_MsgInitSoundOk);
}

void InitEngineSound(void);
void InitEngineSound(void) {
    LoadExtraVabSlotWithTable();
    SetEffectVoicesEnabled(1);
    SetReverbPreset(2, 0, 0);
    DebugPrintf(&g_MsgInitEngineOk);
}

s32 FramesToMilliseconds(s32 arg0, s32 arg1);
s32 FramesToMilliseconds(s32 arg0, s32 arg1) {
    s32 quotient;
    s32 quotientPart;

    quotient = arg0 / 25;
    arg0 -= quotient * 25;
    arg0 = (arg0 * 40) + arg1;
    quotientPart = quotient * 1000;
    return quotientPart + arg0;
}
