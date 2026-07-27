#include "common.h"
#include "game/race.h"
#include "game/state.h"
#include "game/menu.h"
#include "game/track.h"
#include "game/render.h"
#include "game/cd.h"
#include "game/car.h"


extern s32 g_SectorTime1 asm("D_8009AF84");

extern s32 D_8009AF88;


extern s32 g_RefSectorTime0 asm("D_8009AF90");
extern s32 g_RefSectorTime1 asm("D_8009AF94");

extern s32 g_RefSectorTime2 asm("D_8009AF98");



extern u8 g_PlayerCar[] asm("D_8009E6D4");

extern u8 *g_CourseProgress asm("D_8009E67C");

extern s32 g_PlayerProgressA asm("D_8009E73C");

extern s32 g_PlayerProgressB asm("D_8009E740");

extern s16 g_PlayerLap asm("D_8009E83C");

extern s32 g_BestTotalTimes[][4][2] asm("D_8019C70C");

extern s32 g_SeriesCleared asm("D_8019C8EC");

extern s16 D_8019CA10;




extern s32 g_LapCount asm("D_801E4364");


extern s32 g_BestLapTimes[][4][2] asm("D_801E4408");

extern s32 g_BestSectorTimes[][4][3] asm("D_801E41E8");

extern s32 D_801E41EC;

extern s32 D_801E41F0;

extern s32 g_RaceTotalTime asm("D_801E4BA8");

extern s32 g_BestLapThisRace asm("D_801E4BCC");



extern s16 g_RaceCueDelay asm("D_801E6C90");


extern s32 g_RankingTimes[][4][20] asm("D_801E774C");

extern s32 g_RaceCueFlags asm("D_801E7A50");


void GameBeginReplay(void) asm("func_8001FA70");

void GameCommitClassProgress(void) asm("func_800207E0");

void GameDrawFullscreenFadeTile(s32 arg0, s32 arg1) asm("func_80033AA0");

s32 GameFramesToMilliseconds(s32 arg0, s32 arg1) asm("func_80034F18");

void GameExitRaceScene(s32 arg0) asm("func_80035258");

void GameSeedFinishCamera(void *arg0) asm("func_8003CA14");

void GameUpdateRivalCueGate(void) asm("func_80041170");




void GamePlaySoundCue(s32 cue) asm("func_8005D6EC");

void func_8005E4A4(s32 arg0);

s32 GameRandom15(void) asm("func_800632B0");

extern s16 g_PlayerTrackSection asm("D_8009E74C");


extern s32 g_CameraViewMode asm("D_8009E870");



extern s16 g_PauseDebounce asm("D_8019C750");

extern s32 D_8019C768;

extern u8 *D_8019C9A8;

extern s16 D_8019C9AC;


extern s32 D_801E40CC;

extern s32 g_BgmTrack asm("D_801E40E0");



extern s32 D_801E4248;


extern s32 g_RivalCueFlags asm("D_801E4BB4");

extern s16 D_801E4CF8;


extern s32 g_SectorEndDistance0 asm("D_801E4D98");
extern s32 g_SectorEndDistance1 asm("D_801E4D9C");

extern s32 g_SectorEndDistance2 asm("D_801E4DA0");

extern s16 g_RivalCueCooldown0 asm("D_801E6F20");

extern s16 g_RivalCueCooldown1 asm("D_801E6F22");

extern s16 g_RivalCueCooldown2 asm("D_801E6F24");

extern s16 g_RivalCueCooldown3 asm("D_801E6F26");

extern u8 D_80011488[];


void GameInitRenderState(s32) asm("func_80017884");

void func_8001F100(void);

void GameLoadTrackTexturePageRange(void) asm("func_8001D30C");

void func_8001D210(void);

void func_8002C478(void *);

void GameSetTrackTexturePageNow(s32) asm("func_80019E84");

void GameBuildStartingGrid(void) asm("func_80038844");

void GameResetMirrorState(void) asm("func_8001A980");

void func_800458CC(s32);

void func_800340D8(void);

void GameBuildRaceHudPrims(s32) asm("func_80032D5C");

void func_8003D6E8(void);

void GameInitShuttleScenery(void) asm("func_8003F0F8");

void GameSeedFlybyScenery(void) asm("func_8003E464");

void GameSeedRouteScenery(void) asm("func_8003EBCC");

void GameInitPathScenery(void) asm("func_8003F700");

void func_8005DC1C(void);

void func_8001C974(void);



extern u8 g_SceneLightMatrix[] asm("D_8009E6AC");

extern s32 g_PlayerTrackProgress asm("D_8009E744");


extern s16 g_ReverbZoneDepth asm("D_8019C78C");

extern s16 g_CameraCarTrackSection asm("D_801E3E8C");

extern s32 g_IsEnvironmentMode4 asm("D_801E4030");


extern u8 g_PadType asm("D_801E4369");



extern s16 D_801E4B6C[];

extern s32 g_RacePaused asm("D_801E4BAC");

s32 GameBeginMirrorPass(void) asm("func_8001A9A8");

s32 GameUpdateLapAndFinish(void *arg0, s32 arg1) asm("func_8003591C");

void GameRequestTrackTexturePage(s32 arg0) asm("func_80019EFC");

void GameEndMirrorPass(void) asm("func_8001ABD8");

void GameDrawRearViewMirror(s32 arg0) asm("func_8001ADF4");

void GameRecordReplayFrame(void) asm("func_8001F9D8");

void GameDrawLostRaceCaption(s32 arg0) asm("func_800215B8");

void func_800218A0(s32 arg0);

void GameBeginCarStandingStart(void *arg0, s32 arg1) asm("func_8002BE18");

void GameUpdatePlayerCar(void *arg0) asm("func_8002DEFC");

void GameDrawPlayerTachometer(void) asm("func_8002F458");

void GameDrawRaceHudLabels(s32 arg0) asm("func_80032E9C");

void func_80033090(void);

void GameDrawTimeRemaining(s32 arg0) asm("func_800331F8");

void func_80033230(void);

void GameDrawWrongWayWarning(void);

void func_8003425C(s32 arg0);

void func_8003479C(s32 arg0);

void GameGetTrackZoneBlend(s32 arg0) asm("func_800350B4");

void GameUpdateSplitTimes(void *arg0, s32 arg1, s32 arg2) asm("func_800352B8");

void GameDrawSplitTimes(void) asm("func_800357BC");


void func_8003AE2C(void);

void func_8003B0D4(void);

void GameRunRaceIntroCamera(void *arg0, s32 arg1) asm("func_8003C508");

void GameUpdateFinishCamera(void *arg0) asm("func_8003CB3C");

void GameDrawStartGridScenery(s32 arg0) asm("func_8003D458");

void GameDrawCourseScenery(s32 arg0, s32 arg1, s32 arg2) asm("func_8003E1A4");

void GameDrawScriptedScenery(s32 arg0) asm("func_8003F608");

void func_8004087C(s32 arg0);

void GameUpdatePointAmbience(s32 arg0) asm("func_80040ADC");

void GameUpdateZoneAmbience(s32 arg0) asm("func_80040DB4");

void GameTriggerRaceCues(void) asm("func_80040F24");

void GamePlayCountdownCues(s32 arg0) asm("func_800410BC");


void GameDrawTerrainCells(void) asm("func_80041840");

void func_800418D4(void);



void GameUpdateCamera(s32 arg0, void *arg1) asm("func_80043BCC");


void func_8005B190(s32 arg0, s32 arg1);

void func_8005BEA8(s32 arg0, s32 arg1);

void func_8005C31C(s32 arg0, s32 arg1, s32 arg2);

void func_8005D9F8(s32 arg0, s32 arg1);

void func_80069888(void *arg0);

s32 GameUpdateLapAndFinish(void *arg0, s32 arg1) asm("func_8003591C");
s32 GameUpdateLapAndFinish(void *arg0, s32 arg1) {
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

    /* Match note: GCC reserves the original 0x48 frame only when it sees
       this unevaluated seven-argument call shape. It emits no instructions. */
    if (0) GameDebugPrintf((char *)0, 0, 0, 0, 0, 0, 0);

    /*
     * `route` is the car's drive block (GameCarDrive in game/car.h) but this
     * function reaches past the part that struct describes: `route + i*4 +
     * 0xAC / 0xC0 / 0xC4` are per-lap arrays whose element counts are not yet
     * known, and every access here is written as an explicit offset temporary
     * because that is what reproduces retail's address arithmetic. Left raw on
     * purpose; see the note in docs/names.md.
     */
    route = (u8 *)arg0 + 0xBC;
    if (*(s16 *)((u8 *)arg0 + 0x168) > 0) {
        if (g_LapCount >= *(s16 *)((u8 *)arg0 + 0x168)) {
            routeOffset = *(s16 *)((u8 *)arg0 + 0x168) * 4;
            *(s32 *)((s32)route + routeOffset + 0xAC) += 1;
            routeOffset = *(s16 *)((u8 *)arg0 + 0x168) * 4;
            if (*(s32 *)((s32)route + routeOffset + 0xAC) > 0xFFFF) {
                *(s32 *)(route + 0xAC +
                         *(s16 *)((u8 *)arg0 + 0x168) * 4) = 0x10000;
            }
            *(s32 *)((s32)route +
                     (routeStoreOffset =
                          *(s16 *)((u8 *)arg0 + 0x168) * 4) +
                     0xC4) = GameFramesToMilliseconds(
                (routeCallOffset = *(s16 *)((u8 *)arg0 + 0x168) * 4,
                 *(s32 *)((s32)route + routeCallOffset + 0xAC)),
                GameRandom15() % 40);
            routeCompareOffset = *(s16 *)((u8 *)arg0 + 0x168) * 4;
            if (*(s32 *)((s32)route + routeCompareOffset + 0xC4) > 0x927BE) {
                *(s32 *)(route + 0xC4 +
                         *(s16 *)((u8 *)arg0 + 0x168) * 4) = 0x927BF;
                g_LapTimeSaturated = 1;
            }
            routeFinalOffset = *(s16 *)((u8 *)arg0 + 0x168) * 4;
            g_LapTimeMs =
                *(s32 *)((s32)route + routeFinalOffset + 0xC4);
            goto update_progress;
        }
        goto update_best;
    }
    if (g_LapCount < *(s16 *)((u8 *)arg0 + 0x168)) {
update_best:
        if (g_RaceTotalTime <
            g_BestTotalTimes[g_RaceSeries][g_CourseIndex][arg1]) {
            g_BestTotalTimes[g_RaceSeries][g_CourseIndex][arg1] = g_RaceTotalTime;
        }
    }

update_progress:
    progress = *(s16 *)(route + 0xAC);
    if (progress * g_TrackLength <= g_PlayerProgressB + g_PlayerProgressA) {
        s32 progressLimit;

        progressLimit = g_LapCount;
        if (progress > progressLimit) {
            goto progress_failed;
        }
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
            *(s32 *)((s32)route + recordOffset + 0xC0);
        tableOffset = progressLimit;
        step = candidateTime < tableOffset;
        if (step && (recordIndex != 1)) {
            routeProgress = *(u16 *)(route + 0xAC);
            *(s16 *)(route + 0xA6) = routeProgress - 2;
            resultOffset = *(s16 *)(route + 0xAC) * 4;
            result =
                *(s32 *)((s32)route + resultOffset + 0xC0);
            g_BestLapThisRace = candidateTime;
            D_8009AF88 = result;
            if (arg1 == 0) {
                g_RefSectorTime2 = result;
                g_RefSectorTime0 = g_SectorTimes[0];
                g_RefSectorTime1 = g_SectorTime1;
            }

            if (g_LapCount < *(s16 *)(route + 0xAC)) {
                goto record_done;
            }
            GamePlaySoundCue(0x26);
            g_RaceCueDelay = 0x96;
        }

record_done:
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
                if (g_BestLapTimes[g_RaceSeries][g_CourseIndex][arg1] >
                    g_BestLapThisRace) {
                    g_BestLapTimes[g_RaceSeries][g_CourseIndex][arg1] = g_BestLapThisRace;
                }
                if (arg1 == 0) {
                    tableOffset = g_CourseIndex * 12 + g_RaceSeries * 48;
                    *(s32 *)((u8 *)g_BestSectorTimes + tableOffset) = g_RefSectorTime0;
                    *(s32 *)((u8 *)&D_801E41EC + tableOffset) =
                        g_RefSectorTime1;
                    *(s32 *)((u8 *)&D_801E41F0 + tableOffset) =
                        g_RefSectorTime2;
                }
                g_RacePhase = 4;
                GameStartCdVolumeFade(8);
                GamePlaySoundCue(0x2B);
                goto reset_transition;
            }

            g_RacePhase = 5;
            GameSeedFinishCamera(&g_PlayerCar);
            GameStartCdVolumeFade(0x3C);
            if (*(s16 *)((u8 *)g_CourseProgress + 6) != 0) {
                GamePlaySoundCue(0x3D);
            }
reset_transition:
            func_8005E4A4(0);
            g_RaceFadeTimer = 0;
            D_8019CA10 = 0;
            goto after_progress;
        }
        goto check_finish_transition;
    } else {
progress_failed:
        returnValue = 0;
    }

after_progress:
check_finish_transition:
    if ((g_LapCount < *(s16 *)(route + 0xAC)) &&
        (g_RacePhase == 4)) {
        GameDrawFullscreenFadeTile(g_RaceFadeTimer * 2, 0x29);
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
                GameCommitClassProgress();
                if (g_SeriesCleared == 1) {
                    GameRequestCdTrack(0x10);
                } else {
                    GameRequestCdTrack(0xC);
                }
            } else {
                g_SeriesCleared = 0;
                GameRequestCdTrack(0xD);
            }
        }
        if (g_RaceFadeTimer >= 0x83) {
            GameBeginReplay();
            GameExitRaceScene(0x11);
            GameStartCdAudio();
            goto update_countdown;
        }
    } else if ((g_GrandPrixMode == 0) &&
               (((*(s32 *)((u8 *)arg0 + 0x6C) +
                  *(s32 *)((u8 *)arg0 + 0x68)) <= -g_TrackLength) ||
                ((g_PlayerLap == 0) && (g_WrongWayTimer >= 0x3C)))) {
        g_RacePhase = 5;
        g_BestLapTimes[g_RaceSeries][g_CourseIndex][0] =
            g_RankingTimes[g_RaceSeries][g_CourseIndex][0];
        GameStartCdVolumeFade(8);
        func_8005E4A4(0);
        g_RaceFadeTimer = 0;
        GameSeedFinishCamera(&g_PlayerCar);
    }

update_countdown:
    if (g_RaceCueDelay == 2) {
        value = g_LapCount - *(s16 *)(route + 0xAC);
        switch (value) {
        case 2:
            GamePlaySoundCue(0x27);
            break;
        case 1:
            GamePlaySoundCue(0x28);
            break;
        case 0:
            GamePlaySoundCue(0x29);
            break;
        }
        g_RaceCueDelay--;
    } else if (g_RaceCueDelay == 1) {
        g_RaceCueDelay = 0;
        g_RivalCueEnabled = 2;
    } else if (g_RaceCueDelay > 0) {
        g_RaceCueDelay--;
    }

    GameUpdateRivalCueGate();
    return returnValue;
}

void GameEnterRaceScene(void) asm("func_8003609C");
void GameEnterRaceScene(void) {
    s32 pad[2];
    u8 *new_var;
    u8 *base;
    s32 mode;
    s32 scene;
    s32 tableOffset;
    s32 trackLength;
    s32 *entry;
    s32 count;
    s32 i;
    s32 *first;
    s32 new_var2;
    s32 *second;

    GameSetupDisplay240(0, 0, 0);
    GameInitRenderState(5);
    func_8001F100();
    GameLoadTrackTexturePageRange();
    func_8001D210();
    D_801E40CC = *(s32 *)g_TrackEventData;
    if (g_CourseIndex == 3) {
        g_LapCount = 6;
    } else {
        g_LapCount = 3;
    }
    base = g_PlayerCar;
    func_8002C478(base);
    GameSetTrackTexturePageNow(g_PlayerTrackSection);
    GameBuildStartingGrid();
    trackLength = g_TrackLength;
    mode = (count = g_CourseIndex);
    scene = g_RaceSeries;
    g_LapTimeMs = 0;
    D_801E4248 = 0;
    g_LapTimeSaturated = 0;
    g_SectorEndDistance2 = trackLength;
    g_SectorEndDistance0 = trackLength / 3;
    g_SectorEndDistance1 = g_SectorEndDistance0 * 2;
    tableOffset = (mode * 12) + (scene * 48);
    g_RefSectorTime0 = *(s32 *)((u8 *)g_BestSectorTimes + tableOffset);
    scene *= 32;
    g_RefSectorTime1 = *(s32 *)((u8 *)g_BestSectorTimes + tableOffset + 4);
    mode *= 8;
    do {
    } while (0);
    do {
        do {
            g_SectorIndex = -2;
        } while (0);
        new_var = (u8 *)g_BestLapTimes + scene;
        count = mode + (s32)new_var;
        new_var2 = g_GrandPrixMode << 2;
        new_var2 += count;
        entry = (s32 *)new_var2;
        g_RefSectorTime2 = *(s32 *)((u8 *)g_BestSectorTimes + tableOffset + 8);
    } while (0);
    g_RefLapTime = *entry;
    count = (new_var2 = g_LapCount);
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
    GameResetMirrorState();
    func_800458CC(*(s32 *)(D_8019C9A8 + 8));
    func_800340D8();
    GameBuildRaceHudPrims(g_GrandPrixMode);
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
    func_8003D6E8();
    GameInitShuttleScenery();
    GameSeedFlybyScenery();
    GameSeedRouteScenery();
    GameInitPathScenery();
    GameRequestCdTrack(g_BgmTrack + 3);
    g_PauseDebounce = 0;
    g_RaceFadeTimer = 0;
    func_8005DC1C();
    g_RivalCueEnabled = 1;
    D_801E4CF8 = (D_8019C9AC = (g_RaceCueDelay = 0));
    do {
    } while (0);
    g_SceneId = 12;
    D_8019C768 = 0x180;
    func_8001C974();
    GameDebugPrintf(D_80011488);

    (void)pad;
}

void GameUpdateRaceScene(void) asm("func_800363D4");
void GameUpdateRaceScene(void) {
    s32 option;
    s32 value;
    s16 selection;
    s32 next;
    u16 mode;

    value = g_SceneTimer + 1;
    g_SceneTimer = value;
    option = 0;
    if ((u32)value < 0x3D) {
        func_8001C974();
        GameDrawFullscreenFadeTile(0xFF - ((g_SceneTimer - 6) * 0xB), 0x49);
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
            GamePauseCdAudio();
            func_8005E4A4(0);
            g_RaceOptionCursor = 0;
            GamePlaySoundCue(2);
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
                    GamePlaySoundCue(0x3D);
                }
            }
            GameSeedFinishCamera(g_PlayerCar);
            GameStartCdVolumeFade(8);
        } else if (g_RaceOptionCursor == 1) {
            if (g_GrandPrixMode == 0) {
                GameExitRaceScene(0xB);
                g_RacePhase = 8;
            } else {
                goto set_countdown;
            }
        } else {
set_countdown:
            g_PauseDebounce = 0x1E;
            func_8005E4A4(1);
            if (g_RacePhase >= 2) {
                GameResumeCdAudio();
            }
        }
    }

    if (g_RacePhase == 5) {
        if (((g_GrandPrixMode == 1) && (*(s16 *)(g_CourseProgress + 6) == 0)) ||
            (g_GrandPrixMode == 0)) {
            if (g_RaceFadeTimer >= 0x15) {
                func_800218A0((g_RaceFadeTimer - 0x14) * 3);
                GameDrawFullscreenFadeTile((g_RaceFadeTimer - 0x14) * 3, 0x49);
                option = 0xF;
            }
            if (g_RaceFadeTimer == 0xA) {
                GameRequestCdTrack(0xF);
                GameStartCdAudio();
            }
            if (g_RaceFadeTimer >= 0x65) {
                GameExitRaceScene(option);
            }
        } else if ((g_GrandPrixMode == 1) && (*(s16 *)(g_CourseProgress + 6) > 0)) {
            GameDrawLostRaceCaption(g_RaceFadeTimer * 2);
            GameDrawFullscreenFadeTile(g_RaceFadeTimer * 2, 0x49);
            option = 0xD;
            if (g_RaceFadeTimer >= 0x7E) {
                GameExitRaceScene(0xD);
            }
        }
        D_8019CA10 = 0;
        g_RaceFadeTimer++;
    } else if (g_RacePhase == 7) {
        GameExitRaceScene(6);
    }

    if (g_RacePaused != 0) {
        func_8005B190(0x28, 0x28);
        if ((g_PadEdge2 & 0x1000) && g_RaceOptionCursor > 0) {
            g_RaceOptionCursor--;
            GamePlaySoundCue(1);
        }
        if (g_PadEdge2 & 0x4000) {
            selection = g_RaceOptionCursor;
            if (selection < (2 - g_GrandPrixMode)) {
                g_RaceOptionCursor = selection + 1;
                GamePlaySoundCue(1);
            }
        }

        g_SceneTimer--;
        func_8003479C(g_RaceOptionCursor);
        if (g_GrandPrixMode == 0) {
            GameDrawSplitTimes();
        }
        GameDrawRaceHudLabels(g_GrandPrixMode);
        if (g_GrandPrixMode != 0) {
            GameDrawTimeRemaining(g_RaceTimeRemaining);
            func_80033230();
        }
        func_80033090();
        func_8003425C(g_SceneTimer);
        GameGetTrackZoneBlend(g_PlayerTrackProgress);
        GameDrawPlayerTachometer();

        {
            s32 selectorMask;
            u16 inputMask;

            selectorMask = g_PadType;
            inputMask = g_PadHeld;
            selectorMask = (u32)(selectorMask ^ 0x23) < 1;
            if ((inputMask & D_801E4B6C[selectorMask * 8]) &&
                g_CameraViewMode == 0 && g_RacePhase == 2) {
                if (g_PadEdge2 & 8) {
                    D_8019CA10 = 1;
                } else if (g_PadEdge2 & 4) {
                    D_8019CA10 = 0;
                }
            }
        }

        GameUpdateCamera(g_CameraViewMode, g_PlayerCar);
        GameRequestTrackTexturePage(g_PlayerTrackSection);
        if (g_GrandPrixMode != 0) {
            GameDrawCars();
        }
        if ((g_PlayerFacingBackwards != g_RaceSeries) && (g_WrongWayTimer >= 0xA)) {
            GameDrawWrongWayWarning();
        }
        func_800418D4();
        *(s32 *)0x1F800084 = g_IsEnvironmentMode4;
        GameDrawTerrainCells();
        GameDrawCourseObjects();
        if (g_GrandPrixMode != 0) {
            if (g_GrandPrixClass != 5) {
                GameDrawStartGridScenery(g_SceneTimer);
            }
            func_80069888(g_SceneLightMatrix);
            GameDrawScriptedScenery(0);
            GameDrawRearViewMirror(g_SceneTimer);
        }
        GameDrawCourseScenery(g_CourseIndex & 3, g_SceneTimer, 0);
        if (GameBeginMirrorPass() != 0) {
            GameDrawCourseScenery(g_CourseIndex & 3, g_SceneTimer, 0);
            GameEndMirrorPass();
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
            GameRunRaceIntroCamera(g_PlayerCar, frameValue);
        } else {
update_race:
            if ((g_RacePhase == 1) && ((u32)g_SceneTimer >= 0xD3)) {
                GameBeginCarStandingStart(g_PlayerCar, frameValue);
                GameStartCdAudio();
                g_RacePhase = 2;
                g_PauseDebounce = 0x1E;
            }
        }

        if (g_RacePhase < 4) {
            func_8003425C(g_SceneTimer);
            GamePlayCountdownCues(g_SceneTimer);
        }

        if (g_RacePhase < 5) {
            option = GameUpdateLapAndFinish((void *)g_PlayerCar, g_GrandPrixMode);
            GameUpdateSplitTimes(g_PlayerCar, g_GrandPrixMode, option);
            if (option < 2) {
                func_80033090();
            }
        }

        if (g_RacePhase < 4) {
            if (g_GrandPrixMode != 0) {
                GameDrawTimeRemaining(g_RaceTimeRemaining);
            }
            if (g_RaceTimeRemaining <= 0) {
                if (*(s16 *)(g_CourseProgress + 6) != 0) {
                    GamePlaySoundCue(0x3D);
                }
                func_8005E4A4(0);
                g_RacePhase = 5;
                g_RaceFadeTimer = 0;
                GameSeedFinishCamera(g_PlayerCar);
                GameStartCdVolumeFade(8);
            }
        }

        if (g_GrandPrixMode != 0) {
            if (g_RacePhase < 4) {
                func_8003AE2C();
                func_80033230();
            }
        }
        if (option < 2 && g_RacePhase < 5) {
            GameDrawRaceHudLabels(g_GrandPrixMode);
        }

        if (g_RacePhase > 0) {
            GameUpdatePlayerCar(g_PlayerCar);
        } else if (g_RacePhase == 0) {
            func_8005D9F8(0, 0);
        }

        if ((g_RacePhase >= 2) && (g_GrandPrixMode != 0)) {
            func_8003B0D4();
        }

        {
            s32 selectorMask;
            u16 inputMask;

            selectorMask = g_PadType;
            inputMask = g_PadEdge2;
            selectorMask = (u32)(selectorMask ^ 0x23) < 1;
            if ((inputMask & D_801E4B6C[selectorMask * 8]) &&
                (u32)((u16)g_RacePhase - 2) < 2) {
                g_CameraViewMode ^= 1;
            }
        }

        if (g_RacePhase == 5) {
            GameUpdateFinishCamera(g_PlayerCar);
        } else if (g_RacePhase > 0) {
            GameUpdateCamera(g_CameraViewMode, g_PlayerCar);
        }

        if (g_RacePhase != 5) {
            next = g_PlayerTrackSection;
        } else {
            next = g_CameraCarTrackSection;
        }
        GameRequestTrackTexturePage(next);

        if (g_GrandPrixMode != 0) {
            GameDrawCars();
        }
        GameUpdateEnvironment();
        func_800418D4();

        if ((g_PlayerFacingBackwards != g_RaceSeries) && (g_RacePhase < 4)) {
            s16 counter;

            counter = g_WrongWayTimer + 1;
            g_WrongWayTimer = counter;
            if (counter >= 0xA) {
                GameDrawWrongWayWarning();
                if (g_WrongWayTimer >= 0x51) {
                    g_WrongWayTimer = 0xA;
                }
                if ((u8)g_SceneTimer == 0) {
                    GamePlaySoundCue(0x2C);
                }
            }
        } else {
            g_WrongWayTimer = 0;
        }

        *(s32 *)0x1F800084 = g_IsEnvironmentMode4;
        GameDrawTerrainCells();
        GameDrawCourseObjects();
        if (g_GrandPrixMode != 0) {
            if (g_GrandPrixClass != 5) {
                GameDrawStartGridScenery(g_SceneTimer);
            }
            func_80069888(g_SceneLightMatrix);
            GameDrawScriptedScenery(1);
            GameDrawRearViewMirror(g_SceneTimer);
        }
        GameDrawCourseScenery(g_CourseIndex & 3, g_SceneTimer, 1);
        if (GameBeginMirrorPass() != 0) {
            GameDrawCourseScenery(g_CourseIndex & 3, g_SceneTimer, 0);
            GameEndMirrorPass();
        }

        GameGetTrackZoneBlend(g_PlayerTrackProgress);
        if (g_RacePhase >= 4) {
            g_ReverbZoneDepth = 0;
        }
        func_8005B190(g_ReverbZoneDepth, g_ReverbZoneDepth);
        if ((g_RacePhase != 0) && (option < 2) && (g_RacePhase < 5)) {
            GameDrawPlayerTachometer();
        }

        if (g_RacePhase < 4) {
            s32 *valuePtr;

            valuePtr = &g_PlayerTrackProgress;
            GameUpdateZoneAmbience(*valuePtr);
            GameUpdatePointAmbience(*valuePtr);
            func_8004087C(g_PlayerTrackSection);
            GameTriggerRaceCues();
        } else {
            func_8005BEA8(0, 0);
            func_8005C31C(2, 0, 0);
            func_8005C31C(3, 0, 0);
            func_8005C31C(0, 0, 0);
            func_8005C31C(1, 0, 0);
        }
        GameRecordReplayFrame();
    }

}
