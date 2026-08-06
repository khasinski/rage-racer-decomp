#include "common.h"
#include "game/race.h"
#include "game/state.h"
#include "game/menu.h"
#include "game/track.h"
#include "game/render.h"
#include "game/cd.h"
#include "game/car.h"
#include "game/audio.h"
#include "game/random.h"
#include "psyq/gte.h"
#include "game/screens.h"

/* Elements 0, 1 and 2 of g_RefSectorTimes. They CANNOT be spelled
 * g_RefSectorTimes[k] here: with one array symbol GCC 2.6.3 CSEs the base
 * address into a and reschedules around it, which shifts the
 * allocation of the whole surrounding block. Three symbols keep three separate
 * %hi/%lo pairs, which is what the retail code has. */
extern s32 g_RefSectorTimes;

extern u8 g_PlayerCar[];

extern u8 *g_CourseProgress;

extern s16 g_PlayerLap;

extern s32 g_BestTotalTimes[][4][2];

extern s32 g_BestLapTimes[][4][2];

extern s32 g_BestSectorTimes[][4][3];

extern s32 g_RaceTotalTime;

extern s32 g_RankingTimes[][4][20];

void DrawFullscreenFadeTile(s32 color, s32 tpage);

void ExitRaceScene(s32 sceneId);

extern s32 g_CameraViewMode;

extern u8 *g_CamRow;

void InitRenderState(s32);

void SetTrackTexturePageNow(s32);

extern u8 g_SceneLightMatrix[];

extern s32 g_PlayerTrackProgress;

extern u8 g_PadType;

/*
 * Optional trace for the state returned by the lap/finish update. A null
 * format disables it; otherwise the six named values are the complete
 * DebugPrintf argument list.
 */
static __inline__ void GameDebugLapResult(
    char *format,
    s32 result,
    s32 progress,
    s32 mode,
    s32 lapCount,
    s32 racePhase,
    s32 fadeTimer)
{
    if (format != 0) {
        DebugPrintf(
            format, result, progress, mode, lapCount, racePhase, fadeTimer);
    }
}

void RequestTrackTexturePage(s32 trackSection);

void DrawRaceEndBanner(s32 fade);

void BeginCarStandingStart(void *car, s32 sceneTimer);

void UpdatePlayerCar(void *car);

void DrawPlayerTachometer(void);

void GetTrackZoneBlend(s32 position);

void RunRaceIntroCamera(void *car, s32 frame);

void UpdateTrackEventSound(s32 trackSection);

void PlayCountdownCues(s32 sceneTimer);

void UpdateCamera(s32 cameraModeSel, void *car);

s32 UpdateLapAndFinish(void *car, s32 grandPrixMode) {
    s32 value;
    s32 result;
    s16 recordIndex;
    s32 candidateTime;
    s32 count;
    u16 returnValue;
    s16 progress;
    s32 step;
    s32 tableOffset;
    s32 routeOffset;
    s32 routeCallOffset;
    s32 routeStoreOffset;
    s32 routeCompareOffset;
    s32 routeFinalOffset;
    s32 recordOffset;
    s32 resultOffset;
    s32 routeProgress;
    s32 oldTimer;
    s32 timer;
    u8 *route;

    /*
     * `route` is the car's drive block (GameCarDrive in game/car.h) but this
     * function reaches past the part that struct describes: `route + i*4 +
     * 0xAC / 0xC0 / 0xC4` are per-lap arrays whose element counts are not yet
     * known, and every access here is written as an explicit offset temporary
     * because that is what reproduces retail's address arithmetic. Left raw on
     * purpose; see the note in docs/names.md.
     */
    switch (0) { default:
    route = (u8 *)car + 0xBC;
    if (*(s16 *)((u8 *)car + 0x168) > 0) {
        if (g_LapCount >= *(s16 *)((u8 *)car + 0x168)) {
            routeOffset = *(s16 *)((u8 *)car + 0x168) * 4;
            *(s32 *)((u8 *)route + routeOffset + 0xAC) += 1;
            routeOffset = *(s16 *)((u8 *)car + 0x168) * 4;
            if (*(s32 *)((u8 *)route + routeOffset + 0xAC) > 0xFFFF) {
                *(s32 *)(route + 0xAC +
                         *(s16 *)((u8 *)car + 0x168) * 4) = 0x10000;
            }
            *(s32 *)((u8 *)route +
                     (routeStoreOffset =
                          *(s16 *)((u8 *)car + 0x168) * 4) +
                     0xC4) = FramesToMilliseconds(
                (routeCallOffset = *(s16 *)((u8 *)car + 0x168) * 4,
                 *(s32 *)((u8 *)route + routeCallOffset + 0xAC)),
                Random15() % 40);
            routeCompareOffset = *(s16 *)((u8 *)car + 0x168) * 4;
            if (*(s32 *)((u8 *)route + routeCompareOffset + 0xC4) > 0x927BE) {
                *(s32 *)(route + 0xC4 +
                         *(s16 *)((u8 *)car + 0x168) * 4) = 0x927BF;
                g_LapTimeSaturated = 1;
            }
            routeFinalOffset = *(s16 *)((u8 *)car + 0x168) * 4;
            g_LapTimeMs =
                *(s32 *)((u8 *)route + routeFinalOffset + 0xC4);
            break;
        }

    }
    if (g_LapCount < *(s16 *)((u8 *)car + 0x168)) {
        if (g_RaceTotalTime <
            g_BestTotalTimes[g_RaceSeries][g_CourseIndex][grandPrixMode]) {
            g_BestTotalTimes[g_RaceSeries][g_CourseIndex][grandPrixMode] = g_RaceTotalTime;
        }
    }

    }
    progress = *(s16 *)(route + 0xAC);
    if (progress * g_TrackLength <= g_PlayerProgressB + g_PlayerProgressA) {
        s32 progressLimit;

        progressLimit = g_LapCount;
        if (progress > progressLimit) {
            returnValue = 0;
        } else {
        returnValue = 1;
        *(s16 *)(route + 0xAC) = progress + 1;
        g_LapTimeSaturated = 0;
        g_RaceCueFlags &= 0xF;
        if (g_RaceCueDelay == 0) {
            g_RaceCueDelay = 2;
        }
        recordIndex = *(s16 *)(route + 0xAC);
        progressLimit = g_BestLapThisRace;
        recordOffset = recordIndex * 4;
        candidateTime =
            *(s32 *)((u8 *)route + recordOffset + 0xC0);
        tableOffset = progressLimit;
        step = candidateTime < tableOffset;
        if (step && (recordIndex != 1)) {
            routeProgress = *(u16 *)(route + 0xAC);
            *(s16 *)(route + 0xA6) = routeProgress - 2;
            resultOffset = *(s16 *)(route + 0xAC) * 4;
            result =
                *(s32 *)((u8 *)route + resultOffset + 0xC0);
            g_BestLapThisRace = candidateTime;
            g_SectorTimes[2] = result;
            if (grandPrixMode == 0) {
                g_RefSectorTime2 = result;
                g_RefSectorTimes = g_SectorTimes[0];
                g_RefSectorTime1 = g_SectorTimes[1];
            }

            if (!(g_LapCount < *(s16 *)(route + 0xAC))) {
                PlaySoundCue(0x26);
                g_RaceCueDelay = 0x96;
            }
        }

        count = g_LapCount;
        step = *(s16 *)(route + 0xAC);
        if (step == count + 1) {
                if (*(s16 *)(route + 0xA4) < 4) {
                    {
                        s32 *cursor;
                        s32 element;
                        s32 accumulated;

                        result = 0;
                        if (count > 0) {
                            cursor = (s32 *)route;
                            do {
                                element = cursor[0x32];
                                accumulated = g_RaceTotalTime;
                                accumulated += element;
                                g_RaceTotalTime = accumulated;
                                cursor++;
                            } while (++result < count);
                        }
                    }
                if (g_RaceTotalTime > 0x927BE) {
                    g_RaceTotalTime = 0x927BF;
                }
                if (g_BestLapTimes[g_RaceSeries][g_CourseIndex][grandPrixMode] >
                    g_BestLapThisRace) {
                    g_BestLapTimes[g_RaceSeries][g_CourseIndex][grandPrixMode] = g_BestLapThisRace;
                }
                if (grandPrixMode == 0) {
                    tableOffset = g_CourseIndex * 12 + g_RaceSeries * 48;
                    *(s32 *)((u8 *)g_BestSectorTimes + tableOffset) = g_RefSectorTimes;
                    *(s32 *)((u8 *)&g_BestSectorTimes[0][0][1] + tableOffset) =
                        g_RefSectorTime1;
                    *(s32 *)((u8 *)&g_BestSectorTimes[0][0][2] + tableOffset) =
                        g_RefSectorTime2;
                }
                g_RacePhase = 4;
                StartCdVolumeFade(8);
                PlaySoundCue(0x2B);
            } else {
            g_RacePhase = 5;
            SeedFinishCamera(&g_PlayerCar);
            StartCdVolumeFade(0x3C);
            if (*(s16 *)((u8 *)g_CourseProgress + 6) != 0) {
                PlaySoundCue(0x3D);
            }
            }
            ForceAllEffectVoicesEnabled(0);
            g_RaceFadeTimer = 0;
            g_MirrorViewEnabled = 0;

        }
        }

    } else {
        returnValue = 0;
    }

    if ((g_LapCount < *(s16 *)(route + 0xAC)) &&
        (g_RacePhase == 4)) {
        DrawFullscreenFadeTile(g_RaceFadeTimer * 2, 0x29);
        timer = g_RaceFadeTimer;
        oldTimer = timer;
        timer = timer < 2;
        if (!timer) {
            returnValue = 2;
        }
        timer = oldTimer + 1;
        g_RaceFadeTimer = timer;
        if ((s16)timer == 0x3F) {
            if (g_GrandPrixMode != 0) {
                CommitClassProgress();
                if (g_SeriesCleared == 1) {
                    RequestCdTrack(0x10);
                } else {
                    RequestCdTrack(0xC);
                }
            } else {
                g_SeriesCleared = 0;
                RequestCdTrack(0xD);
            }
        }
        if (g_RaceFadeTimer >= 0x83) {
            BeginReplay();
            ExitRaceScene(0x11);
            StartCdAudio();

        }
    } else if ((g_GrandPrixMode == 0) &&
               (((*(s32 *)((u8 *)car + 0x6C) +
                  *(s32 *)((u8 *)car + 0x68)) <= -g_TrackLength) ||
                ((g_PlayerLap == 0) && (g_WrongWayTimer >= 0x3C)))) {
        g_RacePhase = 5;
        g_BestLapTimes[g_RaceSeries][g_CourseIndex][0] =
            g_RankingTimes[g_RaceSeries][g_CourseIndex][0];
        StartCdVolumeFade(8);
        ForceAllEffectVoicesEnabled(0);
        g_RaceFadeTimer = 0;
        SeedFinishCamera(&g_PlayerCar);
    }

    if (g_RaceCueDelay == 2) {
        value = g_LapCount - *(s16 *)(route + 0xAC);
        switch (value) {
        case 2:
            PlaySoundCue(0x27);
            break;
        case 1:
            PlaySoundCue(0x28);
            break;
        case 0:
            PlaySoundCue(0x29);
            break;
        }
        g_RaceCueDelay--;
    } else if (g_RaceCueDelay == 1) {
        g_RaceCueDelay = 0;
        g_RivalCueEnabled = 2;
    } else if (g_RaceCueDelay > 0) {
        g_RaceCueDelay--;
    }

    UpdateRivalCueGate();
    GameDebugLapResult(
        0, returnValue, progress, grandPrixMode, g_LapCount,
        g_RacePhase, g_RaceFadeTimer);
    return returnValue;
}

void EnterRaceScene(void) {
    s32 pad[2];
    u8 *lapTableRow;
    u8 *base;
    s32 mode;
    s32 scene;
    s32 tableOffset;
    s32 trackLength;
    s32 *entry;
    s32 count;
    s32 i;
    s32 *first;
    s32 scratch;
    s32 *second;

    SetupDisplay240(0, 0, 0);
    InitRenderState(5);
    ResetReplayWriteCursor();
    LoadTrackTexturePageRange();
    InitTrackLighting();
    g_TrackWalkStart = *(s32 *)g_TrackEventData;
    if (g_CourseIndex == 3) {
        g_LapCount = 6;
    } else {
        g_LapCount = 3;
    }
    base = g_PlayerCar;
    InitPlayerCar(base);
    SetTrackTexturePageNow(g_PlayerTrackSection);
    BuildStartingGrid();
    trackLength = g_TrackLength;
    mode = (count = g_CourseIndex);
    scene = g_RaceSeries;
    g_LapTimeMs = 0;
    D_801E4248 = 0;
    g_LapTimeSaturated = 0;
    g_SectorEndDistance[2] = trackLength;
    g_SectorEndDistance[0] = trackLength / 3;
    g_SectorEndDistance[1] = g_SectorEndDistance[0] * 2;
    tableOffset = (mode * 12) + (scene * 48);
    g_RefSectorTimes = *(s32 *)((u8 *)g_BestSectorTimes + tableOffset);
    scene *= 32;
    g_RefSectorTime1 = *(s32 *)((u8 *)g_BestSectorTimes + tableOffset + 4);
    mode *= 8;
    do {
        do {
            g_SectorIndex = -2;
        } while (0);
        /* g_BestLapTimes[g_RaceSeries][g_CourseIndex][g_GrandPrixMode],
         * which is how the same table is spelled four times elsewhere in
         * this file. Written indexed here, or with the three offsets folded
         * together, the schedule around these barriers changes. */
        lapTableRow = (u8 *)g_BestLapTimes + scene;
        count = mode + (s32)lapTableRow;
        scratch = g_GrandPrixMode * 4;
        scratch += count;
        entry = (s32 *)scratch;
        g_RefSectorTime2 = *(s32 *)((u8 *)g_BestSectorTimes + tableOffset + 8);
    } while (0);
    g_RefLapTime = *entry;
    count = (scratch = g_LapCount);
    g_RaceTimeRemaining = 0x3A98;
    g_BestLapThisRace = g_RefLapTime;
    if (count > 0) {
        i = 0;
        second = (s32 *)(base + 0x184);
        first = (s32 *)(base + 0x16C);
        do {
            *first = 0;
            *second = 0;
            second++;
            i++;
            first++;
        } while (i < count);
    }
    g_RaceTotalTime = 0;
    ResetMirrorState();
    SeekEnvironmentScript(*(s32 *)(g_CamRow + 8));
    BuildTileStrips();
    BuildRaceHudPrims(g_GrandPrixMode);
    g_AnimTimer = 0;
    g_SceneTimer = 0;
    g_CameraViewMode = 0;
    g_RacePhase = 0;
    g_RaceCueFlags = 0;
    g_RivalCueFlags = 0x1FE;
    g_RivalCueCooldown3 = 0;
    g_RivalCueCooldown2 = 0;
    g_RivalCueCooldown1 = 0;
    g_RivalCueCooldown0 = 0;
    ResetFreeLookCamera();
    InitShuttleScenery();
    SeedFlybyScenery();
    SeedRouteScenery();
    InitPathScenery();
    RequestCdTrack(g_BgmTrack + 3);
    g_PauseDebounce = 0;
    g_RaceFadeTimer = 0;
    InitEffectVoiceRuntime();
    g_RivalCueEnabled = 1;
    D_801E4CF8 = (D_8019C9AC = (g_RaceCueDelay = 0));
    do {
    } while (0);
    g_SceneId = 12;
    g_FrameSyncThreshold = 0x180;
    DrawRoundScreen();
    DebugPrintf(g_MsgGame0Ok);

    (void)pad;
}

void UpdateRaceScene(void) {
    s32 option;
    s32 value;
    s16 selection;
    s32 next;
    u16 mode;

    value = g_SceneTimer + 1;
    g_SceneTimer = value;
    option = 0;
    if ((u32)value < 0x3D) {
        DrawRoundScreen();
        DrawFullscreenFadeTile(0xFF - ((g_SceneTimer - 6) * 0xB), 0x49);
    }

    if (g_PauseDebounce > 0) {
        g_PauseDebounce--;
    }

    mode = g_RacePhase;
    if ((u32)(mode - 1) < 2 && (g_PadEdge2 & 0x800) && g_PauseDebounce <= 0) {
        g_PauseDebounce = 5;
        value = (u32)g_RacePaused < 1;
        g_RacePaused = value;

        if (value != 0) {
            PauseCdAudio();
            ForceAllEffectVoicesEnabled(0);
            g_RaceOptionCursor = 0;
            PlaySoundCue(2);
        } else if (g_RaceOptionCursor == (2 - g_GrandPrixMode)) {
            g_RaceFadeTimer = 0;
            if (g_GrandPrixMode == 0 || (s16)mode < 2) {
                g_RacePhase = 7;
                if (g_GrandPrixMode == 0) {
                    g_BestLapTimes[g_RaceSeries][g_CourseIndex][0] =
                        g_RankingTimes[g_RaceSeries][g_CourseIndex][0];
                }
            } else {
                value = *(s16 *)(g_CourseProgress + 6);
                g_RacePhase = 5;
                if (value != 0) {
                    PlaySoundCue(0x3D);
                }
            }
            SeedFinishCamera(g_PlayerCar);
            StartCdVolumeFade(8);
        } else if (g_RaceOptionCursor == 1 && g_GrandPrixMode == 0) {
            ExitRaceScene(0xB);
            g_RacePhase = 8;
        } else {
            g_PauseDebounce = 0x1E;
            ForceAllEffectVoicesEnabled(1);
            if (g_RacePhase >= 2) {
                ResumeCdAudio();
            }
        }
    }

    if (g_RacePhase == 5) {
        if (((g_GrandPrixMode == 1) && (*(s16 *)(g_CourseProgress + 6) == 0)) ||
            (g_GrandPrixMode == 0)) {
            if (g_RaceFadeTimer >= 0x15) {
                DrawRaceEndBanner((g_RaceFadeTimer - 0x14) * 3);
                DrawFullscreenFadeTile((g_RaceFadeTimer - 0x14) * 3, 0x49);
                option = 0xF;
            }
            if (g_RaceFadeTimer == 0xA) {
                RequestCdTrack(0xF);
                StartCdAudio();
            }
            if (g_RaceFadeTimer >= 0x65) {
                ExitRaceScene(option);
            }
        } else if ((g_GrandPrixMode == 1) && (*(s16 *)(g_CourseProgress + 6) > 0)) {
            DrawLostRaceCaption(g_RaceFadeTimer * 2);
            DrawFullscreenFadeTile(g_RaceFadeTimer * 2, 0x49);
            option = 0xD;
            if (g_RaceFadeTimer >= 0x7E) {
                ExitRaceScene(0xD);
            }
        }
        g_MirrorViewEnabled = 0;
        g_RaceFadeTimer++;
    } else if (g_RacePhase == 7) {
        ExitRaceScene(6);
    }

    if (g_RacePaused != 0) {
        SetReverbDepth(0x28, 0x28);
        if ((g_PadEdge2 & 0x1000) && g_RaceOptionCursor > 0) {
            g_RaceOptionCursor--;
            PlaySoundCue(1);
        }
        if (g_PadEdge2 & 0x4000) {
            selection = g_RaceOptionCursor;
            if (selection < (2 - g_GrandPrixMode)) {
                g_RaceOptionCursor = selection + 1;
                PlaySoundCue(1);
            }
        }

        g_SceneTimer--;
        DrawRaceOptionMenu(g_RaceOptionCursor);
        if (g_GrandPrixMode == 0) {
            DrawSplitTimes();
        }
        DrawRaceHudLabels(g_GrandPrixMode);
        if (g_GrandPrixMode != 0) {
            DrawTimeRemaining(g_RaceTimeRemaining);
            DrawRacePosition();
        }
        DrawLapTimes();
        DrawStartCountdown(g_SceneTimer);
        GetTrackZoneBlend(g_PlayerTrackProgress);
        DrawPlayerTachometer();

        {
            s32 selectorMask;
            u16 inputMask;

            selectorMask = g_PadType;
            inputMask = g_PadHeld;
            selectorMask = (u32)(selectorMask ^ 0x23) < 1;
            if ((inputMask & g_PadMirrorMasks[selectorMask * 8]) &&
                g_CameraViewMode == 0 && g_RacePhase == 2) {
                if (g_PadEdge2 & 8) {
                    g_MirrorViewEnabled = 1;
                } else if (g_PadEdge2 & 4) {
                    g_MirrorViewEnabled = 0;
                }
            }
        }

        UpdateCamera(g_CameraViewMode, g_PlayerCar);
        RequestTrackTexturePage(g_PlayerTrackSection);
        if (g_GrandPrixMode != 0) {
            DrawCars();
        }
        if ((g_PlayerFacingBackwards != g_RaceSeries) && (g_WrongWayTimer >= 0xA)) {
            DrawWrongWayWarning();
        }
        DrawSkyBackground();
        SCRATCH_ENV_MODE4 = g_IsEnvironmentMode4;
        DrawTerrainCells();
        DrawCourseObjects();
        if (g_GrandPrixMode != 0) {
            if (g_GrandPrixClass != 5) {
                DrawStartGridScenery(g_SceneTimer);
            }
            SetLightMatrix(g_SceneLightMatrix);
            DrawScriptedScenery(0);
            DrawRearViewMirror(g_SceneTimer);
        }
        DrawCourseScenery(g_CourseIndex & 3, g_SceneTimer, 0);
        if (BeginMirrorPass() != 0) {
            DrawCourseScenery(g_CourseIndex & 3, g_SceneTimer, 0);
            EndMirrorPass();
        }
    } else {
        s32 frameValue;

        g_AnimTimer++;
        if ((g_RacePhase >= 2) && (g_GrandPrixMode != 0)) {
            g_RaceTimeRemaining--;
        }

        frameValue = g_SceneTimer;
        if ((u32)frameValue >= 0x5A) {
            if (g_RacePhase == 0) {
                g_RacePhase = 1;
            } else {
                goto update_race;
            }
        } else if (g_RacePhase == 0) {
            RunRaceIntroCamera(g_PlayerCar, frameValue);
        } else {
update_race:
            if ((g_RacePhase == 1) && ((u32)g_SceneTimer >= 0xD3)) {
                BeginCarStandingStart(g_PlayerCar, frameValue);
                StartCdAudio();
                g_RacePhase = 2;
                g_PauseDebounce = 0x1E;
            }
        }

        if (g_RacePhase < 4) {
            DrawStartCountdown(g_SceneTimer);
            PlayCountdownCues(g_SceneTimer);
        }

        if (g_RacePhase < 5) {
            option = UpdateLapAndFinish((void *)g_PlayerCar, g_GrandPrixMode);
            UpdateSplitTimes(g_PlayerCar, g_GrandPrixMode, option);
            if (option < 2) {
                DrawLapTimes();
            }
        }

        if (g_RacePhase < 4) {
            if (g_GrandPrixMode != 0) {
                DrawTimeRemaining(g_RaceTimeRemaining);
            }
            if (g_RaceTimeRemaining <= 0) {
                if (*(s16 *)(g_CourseProgress + 6) != 0) {
                    PlaySoundCue(0x3D);
                }
                ForceAllEffectVoicesEnabled(0);
                g_RacePhase = 5;
                g_RaceFadeTimer = 0;
                SeedFinishCamera(g_PlayerCar);
                StartCdVolumeFade(8);
            }
        }

        if (g_GrandPrixMode != 0) {
            if (g_RacePhase < 4) {
                UpdateRacePosition();
                DrawRacePosition();
            }
        }
        if (option < 2 && g_RacePhase < 5) {
            DrawRaceHudLabels(g_GrandPrixMode);
        }

        if (g_RacePhase > 0) {
            UpdatePlayerCar(g_PlayerCar);
        } else if (g_RacePhase == 0) {
            UpdateLoadedAudioVoices(0, 0);
        }

        if ((g_RacePhase >= 2) && (g_GrandPrixMode != 0)) {
            UpdateRaceCars();
        }

        {
            s32 selectorMask;
            u16 inputMask;

            selectorMask = g_PadType;
            inputMask = g_PadEdge2;
            selectorMask = (u32)(selectorMask ^ 0x23) < 1;
            if ((inputMask & g_PadMirrorMasks[selectorMask * 8]) &&
                (u32)((u16)g_RacePhase - 2) < 2) {
                g_CameraViewMode ^= 1;
            }
        }

        if (g_RacePhase == 5) {
            UpdateFinishCamera(g_PlayerCar);
        } else if (g_RacePhase > 0) {
            UpdateCamera(g_CameraViewMode, g_PlayerCar);
        }

        if (g_RacePhase != 5) {
            next = g_PlayerTrackSection;
        } else {
            next = g_CameraCarTrackSection;
        }
        RequestTrackTexturePage(next);

        if (g_GrandPrixMode != 0) {
            DrawCars();
        }
        UpdateEnvironment();
        DrawSkyBackground();

        if ((g_PlayerFacingBackwards != g_RaceSeries) && (g_RacePhase < 4)) {
            s16 counter;

            counter = g_WrongWayTimer + 1;
            g_WrongWayTimer = counter;
            if (counter >= 0xA) {
                DrawWrongWayWarning();
                if (g_WrongWayTimer >= 0x51) {
                    g_WrongWayTimer = 0xA;
                }
                if ((u8)g_SceneTimer == 0) {
                    PlaySoundCue(0x2C);
                }
            }
        } else {
            g_WrongWayTimer = 0;
        }

        SCRATCH_ENV_MODE4 = g_IsEnvironmentMode4;
        DrawTerrainCells();
        DrawCourseObjects();
        if (g_GrandPrixMode != 0) {
            if (g_GrandPrixClass != 5) {
                DrawStartGridScenery(g_SceneTimer);
            }
            SetLightMatrix(g_SceneLightMatrix);
            DrawScriptedScenery(1);
            DrawRearViewMirror(g_SceneTimer);
        }
        DrawCourseScenery(g_CourseIndex & 3, g_SceneTimer, 1);
        if (BeginMirrorPass() != 0) {
            DrawCourseScenery(g_CourseIndex & 3, g_SceneTimer, 0);
            EndMirrorPass();
        }

        GetTrackZoneBlend(g_PlayerTrackProgress);
        if (g_RacePhase >= 4) {
            g_ReverbZoneDepth = 0;
        }
        SetReverbDepth(g_ReverbZoneDepth, g_ReverbZoneDepth);
        if ((g_RacePhase != 0) && (option < 2) && (g_RacePhase < 5)) {
            DrawPlayerTachometer();
        }

        if (g_RacePhase < 4) {
            s32 *valuePtr;

            valuePtr = &g_PlayerTrackProgress;
            UpdateZoneAmbience(*valuePtr);
            UpdatePointAmbience(*valuePtr);
            UpdateTrackEventSound(g_PlayerTrackSection);
            TriggerRaceCues();
        } else {
            SetPanVoiceTargetVolume(0, 0);
            SetStereoSoundCue(2, 0, 0);
            SetStereoSoundCue(3, 0, 0);
            SetStereoSoundCue(0, 0, 0);
            SetStereoSoundCue(1, 0, 0);
        }
        RecordReplayFrame();
    }

}
