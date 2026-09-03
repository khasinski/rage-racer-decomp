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

/* The first union field and the two trailing split symbols keep separate
 * %hi/%lo accesses. Indexing the union here makes GCC 2.6.3 CSE its base and
 * shifts the allocation of the surrounding block. */

/*
 * Optional trace for the state returned by the lap/finish update. A null
 * format disables it; otherwise the six named values are the complete
 * printf argument list.
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
        printf(
            format, result, progress, mode, lapCount, racePhase, fadeTimer);
    }
}












s32 UpdateLapAndFinish(PlayerCarRuntime *car, s32 grandPrixMode) {
    s32 value;
    s32 result;
    s16 recordIndex;
    s32 candidateTime;
    s32 count;
    u16 returnValue;
    s16 progress;
    s32 step;
    s32 tableOffset;
    s32 routeProgress;
    s32 oldTimer;
    s32 timer;
    PlayerCarRaceState *route;
    SectorTimeTableAddress sectorAddress;

    route = GetPlayerCarRaceState(car);
    if (route->timing.fields.lap > 0) {
        if (g_LapCount >= route->timing.fields.lap) {
            route->timing.fields.lapTimes.table
                .frameCounts[route->timing.fields.lap - 1] += 1;
            if (route->timing.fields.lapTimes.table
                    .frameCounts[route->timing.fields.lap - 1] > 0xFFFF) {
                route->timing.fields.lapTimes.table
                    .frameCounts[route->timing.fields.lap - 1] = 0x10000;
            }
            route->timing.fields.lapTimes.table
                .milliseconds[route->timing.fields.lap - 1] = FramesToMilliseconds(
                route->timing.fields.lapTimes.table
                    .frameCounts[route->timing.fields.lap - 1],
                Random15() % 40);
            if (route->timing.fields.lapTimes.table
                    .milliseconds[route->timing.fields.lap - 1] > 0x927BE) {
                route->timing.fields.lapTimes.table
                    .milliseconds[route->timing.fields.lap - 1] = 0x927BF;
                g_LapTimeSaturated = 1;
            }
            g_LapTimeMs = route->timing.fields.lapTimes.table
                              .milliseconds[route->timing.fields.lap - 1];
            goto timing_done;
        }
    }
    if (g_LapCount < route->timing.fields.lap) {
        if (g_RaceTotalTime <
            g_BestTotalTimes[ReadStableRaceSeries()][g_CourseIndex][grandPrixMode]) {
            g_BestTotalTimes[ReadStableRaceSeries()][g_CourseIndex][grandPrixMode] = g_RaceTotalTime;
        }
    }

timing_done:
    progress = route->timing.fields.lap;
    if (progress * g_TrackLength <= g_PlayerCar.progressB + g_PlayerCar.progressA) {
        s32 progressLimit;

        progressLimit = g_LapCount;
        if (progress > progressLimit) {
            returnValue = 0;
        } else {
        returnValue = 1;
        route->timing.fields.lap = progress + 1;
        g_LapTimeSaturated = 0;
        g_RaceCueFlags &= 0xF;
        if (g_RaceCueDelay == 0) {
            g_RaceCueDelay = 2;
        }
        recordIndex = route->timing.fields.lap;
        progressLimit = g_BestLapThisRace;
        candidateTime = route->timing.fields.lapTimes.table
                            .milliseconds[recordIndex - 2];
        tableOffset = progressLimit;
        step = candidateTime < tableOffset;
        if (step && (recordIndex != 1)) {
            routeProgress = (u16)route->timing.fields.lap;
            route->drive.hudLapHighlightRow = routeProgress - 2;
            result = route->timing.fields.lapTimes.table
                         .milliseconds[route->timing.fields.lap - 2];
            g_BestLapThisRace = candidateTime;
            g_SectorTimes[2] = result;
            if (grandPrixMode == 0) {
                g_RefSectorTime2 = result;
                g_RefSectorTimes.fields.first = g_SectorTimes[0];
                g_RefSectorTime1 = g_SectorTimes[1];
            }

            if (!(g_LapCount < route->timing.fields.lap)) {
                PlaySoundCue(0x26);
                g_RaceCueDelay = 0x96;
            }
        }

        count = g_LapCount;
        step = route->timing.fields.lap;
        if (step == count + 1) {
                if (route->drive.racePosition < 4) {
                    {
                        s32 *cursor;
                        s32 element;
                        s32 accumulated;
                        PlayerCarRaceStateAddress routeAddress;

                        result = 0;
                        if (count > 0) {
                            routeAddress.state = route;
                            cursor = routeAddress.words;
                            do {
                                routeAddress.words = cursor;
                                element = routeAddress.state->timing.fields
                                              .lapTimes.table.milliseconds[0];
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
                if (g_BestLapTimes[ReadStableRaceSeries()][g_CourseIndex][grandPrixMode] >
                    g_BestLapThisRace) {
                    g_BestLapTimes[ReadStableRaceSeries()][g_CourseIndex][grandPrixMode] = g_BestLapThisRace;
                }
                if (grandPrixMode == 0) {
                    tableOffset = g_CourseIndex * 12 + ReadStableRaceSeries() * 48;
                    sectorAddress.table = g_BestSectorTimes;
                    sectorAddress.bytes += tableOffset;
                    sectorAddress.pointer[0] = g_RefSectorTimes.fields.first;
                    sectorAddress.pointer = &g_BestSectorTimes[0][0][1];
                    sectorAddress.bytes += tableOffset;
                    sectorAddress.pointer[0] = g_RefSectorTime1;
                    sectorAddress.pointer = &g_BestSectorTimes[0][0][2];
                    sectorAddress.bytes += tableOffset;
                    sectorAddress.pointer[0] = g_RefSectorTime2;
                }
                g_RacePhase = 4;
                StartCdVolumeFade(8);
                PlaySoundCue(0x2B);
            } else {
            g_RacePhase = 5;
            SeedFinishCamera(&g_PlayerCar);
            StartCdVolumeFade(0x3C);
            if (g_CourseProgress->retriesRemaining != 0) {
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

    if ((g_LapCount < route->timing.fields.lap) &&
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
               (((car->progressB + car->progressA) <= -g_TrackLength) ||
                ((g_PlayerCar.lap == 0) && (g_WrongWayTimer >= 0x3C)))) {
        g_RacePhase = 5;
        g_BestLapTimes[ReadStableRaceSeries()][g_CourseIndex][0] =
            g_RankingRecords[ReadStableRaceSeries()][g_CourseIndex][0].raceTime;
        StartCdVolumeFade(8);
        ForceAllEffectVoicesEnabled(0);
        g_RaceFadeTimer = 0;
        SeedFinishCamera(&g_PlayerCar);
    }

    if (g_RaceCueDelay == 2) {
        value = g_LapCount - route->timing.fields.lap;
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
