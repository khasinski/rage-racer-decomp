#include "common.h"
#include "game/audio.h"
#include "game/waypoint.h"
#include "game/race.h"
#include "game/state.h"
#include "game/menu.h"
#include "game/track.h"
#include "game/render.h"
#include "game/cd.h"
#include "game/screens.h"

extern u8 g_PlayerCar[];

extern s32 g_PlayerTrackProgress;

void RequestTrackTexturePage(s32 trackSection);

void DrawFullscreenFadeTile(s32 color, s32 tpage);

void GetTrackZoneBlend(s32 trackProgress);

void ExitRaceScene(s32 sceneId);

void UpdateFreeLookCamera(u8 *car, s32 updateMotion);

void UpdateTrackEventSound(s32 trackSection);

extern s16 g_PlayerLap;

/*
 * Initializes the 6 TrackWaypointRuntime slots for the current course. The
 * `magic`/mult/mfhi block is the compiler's divide-by-10 idiom computing the
 * track index (g_PlayerLap-1)/10, clamped to 0..9, which selects a
 * TrackWaypointSeed row in g_WaypointSeeds. Each slot i is seeded at
 * origin + step*i (x,y), with the fixed constants 0x1766 and 0x174, and marked
 * inactive. Register pins and the raw tail-offset writes are match-load-bearing.
 */

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
        DrawSkyBackground();
        SCRATCH_ENV_MODE4 = g_IsEnvironmentMode4;
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
    DrawSkyBackground();
    SCRATCH_ENV_MODE4 = g_IsEnvironmentMode4;
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
