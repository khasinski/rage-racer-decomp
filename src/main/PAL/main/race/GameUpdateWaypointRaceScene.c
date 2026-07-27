#include "common.h"
#include "game/audio.h"
#include "game/waypoint.h"
#include "game/race.h"
#include "game/state.h"
#include "game/menu.h"
#include "game/track.h"
#include "game/render.h"
#include "game/cd.h"

extern s16 g_PauseDebounce asm("D_8019C750");

extern s16 g_ReverbZoneDepth asm("D_8019C78C");


extern u8 g_PlayerCar[] asm("D_8009E6D4");

extern s32 g_PlayerTrackProgress asm("D_8009E744");

extern s16 g_PlayerTrackSection asm("D_8009E74C");

extern s32 g_CameraCarProgressA asm("D_801E3E7C");

extern s32 g_IsEnvironmentMode4 asm("D_801E4030");






extern s32 g_RacePaused asm("D_801E4BAC");




void func_8001C974(void);

void GameRequestTrackTexturePage(s32 arg0) asm("func_80019EFC");

void GameDrawFullscreenFadeTile(s32 arg0, s32 arg1) asm("func_80033AA0");

void func_8003479C(s32 arg0);

void GameGetTrackZoneBlend(s32 arg0) asm("func_800350B4");

void GameExitRaceScene(s32 arg0) asm("func_80035258");

void GameUpdateFreeLookCamera(u8 *arg0, s32 arg1) asm("func_8003CF14");

void GameDrawCourseScenery(s32 arg0, s32 arg1, s32 arg2) asm("func_8003E1A4");

void func_8003E590(void);

void GameDrawFlybyScenery(void) asm("func_8003EAF4");

void GameUpdateRouteScenery(void) asm("func_8003EC98");

void GameDrawRouteScenery(void) asm("func_8003F02C");

void func_8003F9C4(void);

void GameDrawPathScenery(void) asm("func_80040730");

void func_8004087C(s32 arg0);

void GameUpdatePointAmbience(s32 arg0) asm("func_80040ADC");

void GameUpdateZoneAmbience(s32 arg0) asm("func_80040DB4");


void GameDrawTerrainCells(void) asm("func_80041840");

void func_800418D4(void);






extern s16 g_PlayerLap asm("D_8009E83C");

extern s32 g_WaypointSpawnCooldown asm("D_8019C700");

extern TrackWaypointSeed g_WaypointSeeds[] asm("D_8007DFD4");

extern TrackWaypointRuntime g_Waypoints[] asm("D_801E4DF4");

/*
 * Initializes the 6 TrackWaypointRuntime slots for the current course. The
 * `magic`/mult/mfhi block is the compiler's divide-by-10 idiom computing the
 * track index (g_PlayerLap-1)/10, clamped to 0..9, which selects a
 * TrackWaypointSeed row in g_WaypointSeeds. Each slot i is seeded at
 * origin + step*i (x,y), with the fixed constants 0x1766 and 0x174, and marked
 * inactive. Register pins and the raw tail-offset writes are match-load-bearing.
 */

void GameUpdateWaypointRaceScene(void) asm("func_80037200");
void GameUpdateWaypointRaceScene(void) {
    s32 value;
    s32 option;

    option = 0;
    g_SceneTimer++;
    if ((u32)g_SceneTimer < 0x3D) {
        func_8001C974();
        value = g_SceneTimer - 6;
        GameDrawFullscreenFadeTile(0xFF - (((value * 3) * 4) - value), 0x49);
    }

    if (g_PauseDebounce > 0) {
        g_PauseDebounce--;
    }

    if ((u32)((u16)g_RacePhase - 1) < 2 && (g_PadEdge2 & 0x800) && g_PauseDebounce <= 0) {
        g_PauseDebounce = 0;
        g_RacePaused = (u32)g_RacePaused < 1;

        if (g_RacePaused != 0) {
            GamePauseCdAudio();
            GameForceAllEffectVoicesEnabled(0);
            g_RaceOptionCursor = 0;
            GamePlaySoundCue(2);
        } else if (g_RaceOptionCursor == 2) {
            g_RaceFadeTimer = 0;
            g_RacePhase = 7;
            GameStartCdVolumeFade(0x1E);
        } else if (g_RaceOptionCursor == 1) {
            GameExitRaceScene(0xB);
        } else {
            GameForceAllEffectVoicesEnabled(1);
            if (g_RacePhase >= 2) {
                GameResumeCdAudio();
            }
        }
    }

    if (g_RacePhase == 7) {
        value = g_RaceFadeTimer;
        if (value > 0) {
            GameDrawFullscreenFadeTile(value * 3, 0x49);
            option = 6;
            value = g_RaceFadeTimer;
        }
        if (value >= 0x3D) {
            GameExitRaceScene(option);
        }
        g_RaceFadeTimer++;
    }

    if (g_RacePaused != 0) {
        if ((g_PadEdge2 & 0x1000) && g_RaceOptionCursor > 0) {
            g_RaceOptionCursor--;
            GamePlaySoundCue(1);
        }

        if ((g_PadEdge2 & 0x4000) && g_RaceOptionCursor < (2 - g_GrandPrixMode)) {
            g_RaceOptionCursor++;
            GamePlaySoundCue(1);
        }

        g_SceneTimer--;
        func_8003479C(g_RaceOptionCursor);
        GameUpdateFreeLookCamera(g_PlayerCar, 0);
        GameGetTrackZoneBlend(g_PlayerTrackProgress);
        GameRequestTrackTexturePage(g_PlayerTrackSection);
        func_800418D4();
        *(s32 *)0x1F800084 = g_IsEnvironmentMode4;
        GameDrawTerrainCells();
        GameDrawCourseObjects();
        GameDrawFlybyScenery();
        GameDrawRouteScenery();
        GameDrawPathScenery();
        GameDrawCourseScenery(g_CourseIndex & 3, g_SceneTimer, 0);
        return;
    }

    g_AnimTimer++;

    if ((u32)g_SceneTimer >= 0x1F && g_RacePhase == 0) {
        GameStartCdAudio();
        g_RacePhase = 2;
        g_PauseDebounce = 0xA;
    }

    if (g_CameraCarProgressA >= g_TrackLength && g_RacePhase < 3) {
        g_RaceFadeTimer = 0;
        g_RacePhase = 7;
        GameStartCdVolumeFade(0x1E);
        GameForceAllEffectVoicesEnabled(0);
    }

    GameUpdateLoadedAudioVoices(0, 1);
    GameUpdateFreeLookCamera(g_PlayerCar, 1);
    GameRequestTrackTexturePage(g_PlayerTrackSection);
    GameUpdateEnvironment();
    func_800418D4();
    *(s32 *)0x1F800084 = g_IsEnvironmentMode4;
    GameDrawTerrainCells();
    GameDrawCourseObjects();
    func_8003E590();
    GameDrawFlybyScenery();
    GameUpdateRouteScenery();
    GameDrawRouteScenery();
    func_8003F9C4();
    GameDrawPathScenery();
    GameDrawCourseScenery(g_CourseIndex & 3, g_SceneTimer, 1);
    GameGetTrackZoneBlend(g_PlayerTrackProgress);
    GameSetReverbDepth(g_ReverbZoneDepth, g_ReverbZoneDepth);
    GameUpdateZoneAmbience(g_PlayerTrackProgress);
    GameUpdatePointAmbience(g_PlayerTrackProgress);
    func_8004087C(g_PlayerTrackSection);
}

void GameSeedWaypoints(void) asm("func_80037714");
void GameSeedWaypoints(void) {
    TrackWaypointRuntime *waypoint;
    s32 i;
    TrackWaypointSeed *seed;
    s32 code;
    s32 field18;
    register char *tail asm("$4");
    s32 track;
    register s32 temp asm("$2");
    register s32 seedBase asm("$3");

    track = g_PlayerLap - 1;
    track = track % 10;
    if (track < 0) {
        track = 0;
    } else if (track >= 9) {
        track = 9;
    }

    waypoint = g_Waypoints;
    temp = (track << 1) + track;
    temp <<= 2;
    seedBase = (s32)g_WaypointSeeds;
    seed = (TrackWaypointSeed *)(temp + seedBase);
    i = 0;
    code = 0x1766;
    field18 = 0x174;
    tail = (char *)waypoint + 0x1C;

    do {
        waypoint->active = 0;
        *(s32 *)(tail - 0x18) = seed->x + (seed->stepX * i);
        *(s32 *)(tail - 0x10) = seed->y + (seed->stepY * i);
        *(s32 *)(tail - 0x14) = code;
        *(s32 *)(tail - 0x4) = field18;
        *(s32 *)(tail - 0x8) = 0;
        *(s32 *)tail = 0;

        i++;
        waypoint++;
        tail += sizeof(*waypoint);
    } while (i < 6);

    g_WaypointSpawnCooldown = 0;
}
