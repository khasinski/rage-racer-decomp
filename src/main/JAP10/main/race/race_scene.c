#include "common.h"
#include <stdio.h>
#include "game/audio.h"
#include "game/car.h"
#include "game/player_car_internal.h"
#include "game/cd.h"
#include "game/menu.h"
#include "game/race.h"
#include "game/random.h"
#include "game/records_internal.h"
#include "game/render.h"
#include "game/render_internal.h"
#include "game/save_internal.h"
#include "game/race_internal.h"
#include "game/scratchpad.h"
#include "game/screens.h"
#include "game/state.h"
#include "game/track.h"
#include "psyq/gte.h"

/* Japanese build: five minutes on the race clock is 0x4650 frames at
 * 60 fps, against PAL's 0x3A98. */

void EnterRaceScene(void) {
    s32 pad[2];
    LapTimeTableAddress lapTableRow;
    PlayerCarRuntime *player;
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
    SectorTimeTableAddress sectorAddress;
    SectorTimeTableAddress lastSectorAddress;
    LapTimeTableAddress lapAddress;

    SetupDisplay240(0, 0, 0);
    InitRenderState(5);
    ResetReplayWriteCursor();
    LoadTrackTexturePageRange();
    InitTrackLighting();
    g_TrackWalkStart = g_TrackEventData->trackWalkStart;
    if (g_CourseIndex == 3) {
        g_LapCount = 6;
    } else {
        g_LapCount = 3;
    }
    player = &g_PlayerCar;
    InitPlayerCar(player);
    SetTrackTexturePageNow(g_PlayerCar.trackSection);
    BuildStartingGrid();
    trackLength = g_TrackLength;
    mode = (count = g_CourseIndex);
    scene = ReadStableRaceSeries();
    g_LapTimeMs = 0;
    g_UnusedLapTimingWord = 0;
    g_LapTimeSaturated = 0;
    g_SectorEndDistance[2] = trackLength;
    g_SectorEndDistance[0] = trackLength / 3;
    g_SectorEndDistance[1] = g_SectorEndDistance[0] * 2;
    tableOffset = (mode * 12) + (scene * 48);
    sectorAddress.table = g_BestSectorTimes;
    sectorAddress.bytes += tableOffset;
    g_RefSectorTimes.fields.first = sectorAddress.pointer[0];
    scene *= 32;
    sectorAddress.table = g_BestSectorTimes;
    sectorAddress.bytes += tableOffset;
    g_RefSectorTime1 = sectorAddress.pointer[1];
    mode *= 8;
    do {
        do {
            g_SectorIndex = -2;
        } while (0);
        /* g_BestLapTimes[g_RaceSeries][g_CourseIndex][g_GrandPrixMode],
         * which is how the same table is spelled four times elsewhere in
         * this file. Written indexed here, or with the three offsets folded
         * together, the schedule around these address calculations changes. */
        lapAddress.table = g_BestLapTimes;
        lapAddress.bytes += scene;
        lapTableRow.bytes = lapAddress.bytes;
        count = mode + lapTableRow.value;
        scratch = g_GrandPrixMode * 4;
        scratch += count;
        lapAddress.value = scratch;
        entry = lapAddress.pointer;
        lastSectorAddress.table = g_BestSectorTimes;
        lastSectorAddress.bytes += tableOffset;
        g_RefSectorTime2 = lastSectorAddress.pointer[2];
    } while (0);
    g_RefLapTime = *entry;
    count = (scratch = g_LapCount);
    g_RaceTimeRemaining = 0x4650;
    g_BestLapThisRace = g_RefLapTime;
    if (count > 0) {
        i = 0;
        second = player->lapTimes.table.milliseconds;
        first = player->lapTimes.table.frameCounts;
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
    SeekEnvironmentScript(g_TrackRenderTable->environmentScriptOffset);
    BuildTileStrips();
    BuildRaceHudPrims(g_GrandPrixMode);
    g_AnimTimer = 0;
    g_SceneTimer = 0;
    g_CameraViewMode = CAMERA_VIEW_CAR;
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
    g_UnusedRaceInitHalfword = (g_PlayerAutoSteer = (g_RaceCueDelay = 0));
    do {
    } while (0);
    g_SceneId = 12;
    g_FrameSyncThreshold = 0x180;
    DrawRoundScreen();
    printf(g_MsgGame0Ok);

    (void)pad;
}

void UpdateRaceScene(void) {
    s32 option;
    s32 value;
    u32 timerValue;
    s16 selection;
    s32 next;
    u16 mode;
    u32 pausePhase;
    u32 paused;

    value = g_SceneTimer + 1;
    g_SceneTimer = value;
    option = 0;
    timerValue = value;
    if (timerValue < 0x3D) {
        DrawRoundScreen();
        DrawFullscreenFadeTile(0xFF - ((g_SceneTimer - 6) * 0xB), 0x49);
    }

    if (g_PauseDebounce > 0) {
        g_PauseDebounce--;
    }

    mode = g_RacePhase;
    pausePhase = mode - 1;
    if (pausePhase < 2 && (g_PadPressed & PAD_START) && g_PauseDebounce <= 0) {
        g_PauseDebounce = 5;
        paused = g_RacePaused;
        value = paused < 1;
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
                    g_BestLapTimes[ReadStableRaceSeries()][g_CourseIndex][0] =
                        g_RankingRecords[ReadStableRaceSeries()][g_CourseIndex][0].raceTime;
                }
            } else {
                value = g_CourseProgress->retriesRemaining;
                g_RacePhase = 5;
                if (value != 0) {
                    PlaySoundCue(0x3D);
                }
            }
            SeedFinishCamera(&g_PlayerCar);
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
        if (((g_GrandPrixMode == 1) && (g_CourseProgress->retriesRemaining == 0)) ||
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
        } else if ((g_GrandPrixMode == 1) && (g_CourseProgress->retriesRemaining > 0)) {
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
        if ((g_PadPressed & PAD_UP) && g_RaceOptionCursor > 0) {
            g_RaceOptionCursor--;
            PlaySoundCue(1);
        }
        if (g_PadPressed & PAD_DOWN) {
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
        GetTrackZoneBlend(g_PlayerCar.trackProgress);
        DrawPlayerTachometer();

        {
            s32 selectorMask;
            u16 inputMask;

            selectorMask = g_PadType;
            inputMask = g_PadHeld;
            selectorMask = selectorMask == 0x23;
            if ((inputMask & g_PadMirrorMasks[selectorMask * 8]) &&
                g_CameraViewMode == CAMERA_VIEW_CAR && g_RacePhase == 2) {
                if (g_PadPressed & 8) {
                    g_MirrorViewEnabled = 1;
                } else if (g_PadPressed & 4) {
                    g_MirrorViewEnabled = 0;
                }
            }
        }

        UpdateCamera(g_CameraViewMode, &g_PlayerCar);
        RequestTrackTexturePage(g_PlayerCar.trackSection);
        if (g_GrandPrixMode != 0) {
            DrawCars();
        }
        if ((g_PlayerFacingBackwards != ReadStableRaceSeries()) && (g_WrongWayTimer >= 0xA)) {
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
            SetLightMatrix(&g_SceneLightMatrix);
            DrawScriptedScenery(0);
            DrawRearViewMirror(g_SceneTimer);
        }
        DrawCourseScenery(g_CourseIndex & 3, g_SceneTimer, 0);
        if (BeginMirrorPass() != 0) {
            DrawCourseScenery(g_CourseIndex & 3, g_SceneTimer, 0);
            EndMirrorPass();
        }
    } else {
        u32 frameValue;

        g_AnimTimer++;
        if ((g_RacePhase >= 2) && (g_GrandPrixMode != 0)) {
            g_RaceTimeRemaining--;
        }

        frameValue = g_SceneTimer;
        if (frameValue >= 0x5A) {
            if (g_RacePhase == 0) {
                g_RacePhase = 1;
            } else {
                goto update_race;
            }
        } else if (g_RacePhase == 0) {
            RunRaceIntroCamera(&g_PlayerCar, frameValue);
        } else {
update_race:
            if ((g_RacePhase == 1) && (g_SceneTimer >= 0xD3U)) {
                BeginCarStandingStart(&g_PlayerCar, frameValue);
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
            option = UpdateLapAndFinish(&g_PlayerCar, g_GrandPrixMode);
            UpdateSplitTimes(&g_PlayerCar, g_GrandPrixMode, option);
            if (option < 2) {
                DrawLapTimes();
            }
        }

        if (g_RacePhase < 4) {
            if (g_GrandPrixMode != 0) {
                DrawTimeRemaining(g_RaceTimeRemaining);
            }
            if (g_RaceTimeRemaining <= 0) {
                if (g_CourseProgress->retriesRemaining != 0) {
                    PlaySoundCue(0x3D);
                }
                ForceAllEffectVoicesEnabled(0);
                g_RacePhase = 5;
                g_RaceFadeTimer = 0;
                SeedFinishCamera(&g_PlayerCar);
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
            UpdatePlayerCar(&g_PlayerCar);
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
            inputMask = g_PadPressed;
            selectorMask = selectorMask == 0x23;
            if ((inputMask & g_PadMirrorMasks[selectorMask * 8]) &&
            ((u16)g_RacePhase - 2) < 2U) {
                g_CameraViewMode ^= 1;
            }
        }

        if (g_RacePhase == 5) {
            UpdateFinishCamera(&g_PlayerCar);
        } else if (g_RacePhase > 0) {
            UpdateCamera(g_CameraViewMode, &g_PlayerCar);
        }

        if (g_RacePhase != 5) {
            next = g_PlayerCar.trackSection;
        } else {
            next = g_CameraCarTrackSection;
        }
        RequestTrackTexturePage(next);

        if (g_GrandPrixMode != 0) {
            DrawCars();
        }
        UpdateEnvironment();
        DrawSkyBackground();

        if ((g_PlayerFacingBackwards != ReadStableRaceSeries()) && (g_RacePhase < 4)) {
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
            SetLightMatrix(&g_SceneLightMatrix);
            DrawScriptedScenery(1);
            DrawRearViewMirror(g_SceneTimer);
        }
        DrawCourseScenery(g_CourseIndex & 3, g_SceneTimer, 1);
        if (BeginMirrorPass() != 0) {
            DrawCourseScenery(g_CourseIndex & 3, g_SceneTimer, 0);
            EndMirrorPass();
        }

        GetTrackZoneBlend(g_PlayerCar.trackProgress);
        if (g_RacePhase >= 4) {
            g_ReverbZoneDepth = 0;
        }
        SetReverbDepth(g_ReverbZoneDepth, g_ReverbZoneDepth);
        if ((g_RacePhase != 0) && (option < 2) && (g_RacePhase < 5)) {
            DrawPlayerTachometer();
        }

        if (g_RacePhase < 4) {
            s32 *valuePtr;

            valuePtr = &g_PlayerCar.trackProgress;
            UpdateZoneAmbience(*valuePtr);
            UpdatePointAmbience(*valuePtr);
            UpdateTrackEventSound(g_PlayerCar.trackSection);
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
