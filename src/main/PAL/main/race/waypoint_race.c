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




void DrawRoundScreen(void);

void RequestTrackTexturePage(s32 arg0);

void DrawFullscreenFadeTile(s32 arg0, s32 arg1);

void DrawRaceOptionMenu(s32 arg0);

void GetTrackZoneBlend(s32 arg0);

void ExitRaceScene(s32 arg0);

void UpdateFreeLookCamera(u8 *arg0, s32 arg1);

void DrawCourseScenery(s32 arg0, s32 arg1, s32 arg2);

void UpdateFlybyScenery(void);

void DrawFlybyScenery(void);

void UpdateRouteScenery(void);

void DrawRouteScenery(void);

void UpdatePathScenerySound(void);

void DrawPathScenery(void);

void UpdateTrackEventSound(s32 arg0);

void UpdatePointAmbience(s32 arg0);

void UpdateZoneAmbience(s32 arg0);


void DrawTerrainCells(void);

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

void UpdateWaypointRaceScene(void);
void UpdateWaypointRaceScene(void) {
    s32 value;
    s32 option;

    option = 0;
    g_SceneTimer++;
    if ((u32)g_SceneTimer < 0x3D) {
        DrawRoundScreen();
        value = g_SceneTimer - 6;
        DrawFullscreenFadeTile(0xFF - (((value * 3) * 4) - value), 0x49);
    }

    if (g_PauseDebounce > 0) {
        g_PauseDebounce--;
    }

    if ((u32)((u16)g_RacePhase - 1) < 2 && (g_PadEdge2 & 0x800) && g_PauseDebounce <= 0) {
        g_PauseDebounce = 0;
        g_RacePaused = (u32)g_RacePaused < 1;

        if (g_RacePaused != 0) {
            PauseCdAudio();
            ForceAllEffectVoicesEnabled(0);
            g_RaceOptionCursor = 0;
            PlaySoundCue(2);
        } else if (g_RaceOptionCursor == 2) {
            g_RaceFadeTimer = 0;
            g_RacePhase = 7;
            StartCdVolumeFade(0x1E);
        } else if (g_RaceOptionCursor == 1) {
            ExitRaceScene(0xB);
        } else {
            ForceAllEffectVoicesEnabled(1);
            if (g_RacePhase >= 2) {
                ResumeCdAudio();
            }
        }
    }

    if (g_RacePhase == 7) {
        value = g_RaceFadeTimer;
        if (value > 0) {
            DrawFullscreenFadeTile(value * 3, 0x49);
            option = 6;
            value = g_RaceFadeTimer;
        }
        if (value >= 0x3D) {
            ExitRaceScene(option);
        }
        g_RaceFadeTimer++;
    }

    if (g_RacePaused != 0) {
        if ((g_PadEdge2 & 0x1000) && g_RaceOptionCursor > 0) {
            g_RaceOptionCursor--;
            PlaySoundCue(1);
        }

        if ((g_PadEdge2 & 0x4000) && g_RaceOptionCursor < (2 - g_GrandPrixMode)) {
            g_RaceOptionCursor++;
            PlaySoundCue(1);
        }

        g_SceneTimer--;
        DrawRaceOptionMenu(g_RaceOptionCursor);
        UpdateFreeLookCamera(g_PlayerCar, 0);
        GetTrackZoneBlend(g_PlayerTrackProgress);
        RequestTrackTexturePage(g_PlayerTrackSection);
        func_800418D4();
        *(s32 *)0x1F800084 = g_IsEnvironmentMode4;
        DrawTerrainCells();
        DrawCourseObjects();
        DrawFlybyScenery();
        DrawRouteScenery();
        DrawPathScenery();
        DrawCourseScenery(g_CourseIndex & 3, g_SceneTimer, 0);
        return;
    }

    g_AnimTimer++;

    if ((u32)g_SceneTimer >= 0x1F && g_RacePhase == 0) {
        StartCdAudio();
        g_RacePhase = 2;
        g_PauseDebounce = 0xA;
    }

    if (g_CameraCarProgressA >= g_TrackLength && g_RacePhase < 3) {
        g_RaceFadeTimer = 0;
        g_RacePhase = 7;
        StartCdVolumeFade(0x1E);
        ForceAllEffectVoicesEnabled(0);
    }

    UpdateLoadedAudioVoices(0, 1);
    UpdateFreeLookCamera(g_PlayerCar, 1);
    RequestTrackTexturePage(g_PlayerTrackSection);
    UpdateEnvironment();
    func_800418D4();
    *(s32 *)0x1F800084 = g_IsEnvironmentMode4;
    DrawTerrainCells();
    DrawCourseObjects();
    UpdateFlybyScenery();
    DrawFlybyScenery();
    UpdateRouteScenery();
    DrawRouteScenery();
    UpdatePathScenerySound();
    DrawPathScenery();
    DrawCourseScenery(g_CourseIndex & 3, g_SceneTimer, 1);
    GetTrackZoneBlend(g_PlayerTrackProgress);
    SetReverbDepth(g_ReverbZoneDepth, g_ReverbZoneDepth);
    UpdateZoneAmbience(g_PlayerTrackProgress);
    UpdatePointAmbience(g_PlayerTrackProgress);
    UpdateTrackEventSound(g_PlayerTrackSection);
}

void SeedWaypoints(void);
void SeedWaypoints(void) {
    TrackWaypointRuntime *waypoint;
    s32 i;
    TrackWaypointSeed *seed;
    s32 track;

    track = g_PlayerLap - 1;
    track = track % 10;
    if (track < 0) {
        track = 0;
    } else if (track >= 9) {
        track = 9;
    }

    waypoint = g_Waypoints;
    seed = &g_WaypointSeeds[track];

    for (i = 0; i < 6; i++) {
        waypoint->active = 0;
        waypoint->x = seed->x + seed->stepX * i;
        waypoint->y = seed->y + seed->stepY * i;
        waypoint->height = 0x1766;
        waypoint->field18 = 0x174;
        waypoint->scale = 0;
        waypoint->field1C = 0;
        waypoint++;
    }

    g_WaypointSpawnCooldown = 0;
}
