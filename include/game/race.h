#ifndef GAME_RACE_H
#define GAME_RACE_H

#include "common.h"
#include "game/waypoint.h"
#include "game/vector.h"
#include "game/replay.h"

struct PlayerCarRuntime;

/* Grand Prix class index, 0-based; displayed as CLASS(n+1). Also the track
 * tier: course asset index = 0x57 + (course << 1) + (class << 3). OVAL is
 * gated to class >= 2. See names.md 3. */
extern s32 g_GrandPrixClass;

/* Course selector: low 2 bits are the course (0 BIG, 1 MID, 2 HI, 3 OVAL).
 * Bits 2+ transiently carry g_GrandPrixSeries, so mask with 3. */
extern s32 g_CourseIndex;

/* Which Grand Prix series is played: 0 = first (6 classes), non-zero =
 * advanced (5 classes). Outer index of every per-series table. */
extern s16 g_GrandPrixSeries;

/* Display names: [0..5] first-series classes, [6..10] advanced-series classes,
 * [11..13] course names. */
extern char *g_GrandPrixNames[];

/* Race position, 1 = leading; recomputed each frame from how many cars are
 * further along. At the finish it indexes g_PrizeMoney. */
extern s16 g_RacePosition;

/* Prize money per [course][class][place], place 0 = 1st. */
extern s32 g_PrizeMoney[][6][3];

/* Round number within the current class; drives the "R O U N D %d" overlay. */
extern s32 g_GrandPrixRound;

/* 1 = Grand Prix (championship), 0 = Time Attack. Picks the pre-race panel, the
 * innermost index of the record tables, and the in-race option count
 * (2 - mode). See names.md 3. */
extern s16 g_GrandPrixMode;

/* In-race copy of g_GrandPrixSeries, latched when the grid is built. Outer
 * index of the per-series tables and, because the advanced series runs the
 * courses backwards, also the lap-direction flag. See names.md 3. */
#ifndef GAME_RACE_SERIES_QUALIFIER
#define GAME_RACE_SERIES_QUALIFIER
#endif
extern GAME_RACE_SERIES_QUALIFIER s32 g_RaceSeries;
#undef GAME_RACE_SERIES_QUALIFIER

/* Race phase: 0 pre-start (physics frozen), 1 countdown, 2 racing, 4/5
 * finished, 7 goal/retire, 8 aborted. */
extern s16 g_RacePhase;

/* Series / save file the title menu picked (0 first, 1 advanced); also indexes
 * g_MaxClassReached. Final class is 4 for the first series, 5 for advanced. */
extern s16 g_SeriesSelection;

/* Non-zero once the advanced series is unlocked (first series' last class
 * cleared). Saved at save+0x4E; gates title-menu entry 1. */
extern s16 g_AdvancedSeriesUnlocked;

/* Highest class reached per series/save file ([1] is the old g_MaxClassReachedAdvanced).
 * Unlocks courses and bounds the attract-demo class roll. Saved at save+0x50. */
extern s32 g_MaxClassReached[2];

/* Mirror mode, armed by holding the 0x80C pad combination as the race starts:
 * swaps left/right in steering, body roll, stereo pan and the sound cue. */
extern s32 g_MirrorMode;

/* One save slot's Grand Prix / Time Attack progress; InitMenuMode copies it
 * straight into the live globals and UpdateCourseSelectScreen writes it back. */
typedef struct GameRaceProgress {
    s32 course;
    s32 carIndex;
    s32 classIndex;
    s32 maxClassReached; /* highest class unlocked in this slot */
    s32 money;      /* GP and Extra GP: prize money, capped at 999999999,
                       confirmed against US and JP saves advertising that cap.
                       The Time Attack slot reuses the word for
                       g_GrandPrixSeries, read back as u16. */
} GameRaceProgress;

/* The save slot the front end is editing; repointed at one of the three below,
 * matching the title-menu row that g_CarTable was repointed for. Declared s32
 * because most translation units only touch the first word. */
extern GameRaceProgress *g_RaceProgress;
/* The three slots themselves. Their fields used to be spelled as separate
 * symbols per serialiser (g_GrandPrixSaveCar and friends); they are members. */
extern GameRaceProgress g_GrandPrixSave;
extern GameRaceProgress g_ExtraGrandPrixSave;
extern GameRaceProgress g_TimeAttackSave;

typedef struct GameRaceRanking {
    s16 count;
    u8 pad2[0x1A];
    s32 values[1];
} GameRaceRanking;

extern s16 g_PlayerLap;
extern s32 g_ClosestRivalRank;

/* Course-select gate: `g_CourseIndex < (class < 2 ? 2 : 3)`, or 6 : 7 for the
 * advanced series. This is the OVAL unlock; see names.md 5. */
s32 CanSelectNextCourse(void);

/* The race-start signal gantry, live for 105 <= g_SceneTimer < 300: the "3" /
 * "2" / "1" / "GO" dot-matrix board from g_CountdownGlyphTable[1..4] plus the six start
 * lamps. See docs/names.md 1. */
void DrawStartCountdown(s32 sceneTimer);
void DrawRaceEndBanner(s32 level);

/*
 * Per-course records, all in the memory-card save block. Per-file types.
 *   g_BestTotalTimes  g_BestTotalTimes  [series][course][mode] ms
 *   g_BestLapTimes    g_BestLapTimes  same shape, best single lap
 *   g_BestSectorTimes g_BestSectorTimes  [series][course][3] sector splits
 *   g_CourseProgress  g_CourseProgress  -> the running file's course-result record
 *   g_GrandPrixCourseProgress      g_GrandPrixCourseProgress  row 0's record
 *   g_ExtraGrandPrixCourseProgress g_ExtraGrandPrixCourseProgress  row 1's record
 */

/*
 * Live race timing. Times are milliseconds; 0x927BF is the saturation value
 * for anything over 9'59"998, which the HUD prints as dashes. See names.md 15.
 */

/* Elapsed time of the lap in progress. */
extern s32 g_LapTimeMs;

/* Grand Prix time limit, in frames; counts down while g_RacePhase >= 2 and
 * forces g_RacePhase = 5 when it reaches 0. Seeded to 15000. */
extern s32 g_RaceTimeRemaining;

/* Sector being timed, 0..2; -2 before the first start-line crossing. */
extern s32 g_SectorIndex;

/* This lap's three sector times, filled in as each boundary is crossed. */
extern s32 g_SectorTimes[3];

/* Total of the best lap the split is measured against; seeded from the save
 * records and written back when the race completes. */
extern s32 g_RefLapTime;

/* Two 3-element arrays:
 *   g_SectorEndDistance[3]  g_SectorEndDistance/9C/A0  lap distance ending each sector
 *                                             (L/3, 2L/3, L)
 *   g_RefSectorTimes[3]     g_RefSectorTimes/94/98  the best lap's sector times
 * g_RefSectorTimes is the one exception to one-address-one-name in this header:
 * race/UpdateLapAndFinish.c must spell its three elements as the separate
 * scalars g_RefSectorTimes/1/2 or gcc 2.6.3 CSEs the array base into a register
 * and reschedules the surrounding block. g_SectorEndDistance was checked the
 * same way and does not need it, so it is indexed everywhere.
 */

/* Split readout: the sector time just recorded, the unsigned difference from
 * the reference, and its sign (+1 ahead, -1 behind, 0 no split). */
extern s32 g_LastSectorTime;
extern s32 g_SplitDelta;
extern s16 g_SplitSign;

/* Which sector's reference is on screen, the reference time itself, and the
 * 0..0x3C frame counter that ends the split display. */
extern s16 g_SplitSector;
extern s32 g_SplitTargetTime;
extern s16 g_SplitTimer;

/* Set when a lap time saturates at 0x927BF; write-only in retail. */
extern s32 g_LapTimeSaturated;

/* Frames the player has been driving the wrong way. Past 10 the warning shows
 * and rival cues are muted; in Time Attack 60 on lap 0 aborts the run. */
extern s16 g_WrongWayTimer;

/* g_PlayerCar.facingBackwards. Wrong way is `!= g_RaceSeries`, because the
 * advanced series drives the course in the other direction. */
extern s16 g_PlayerFacingBackwards;

/* Non-zero while rival proximity / position sound cues may play: set only in
 * the middle of a lap and cleared by the wrong-way warning. */
extern s16 g_RivalCueEnabled;

/* Frame counter of the in-race fade transitions; every use is the brightness
 * argument of DrawFullscreenFadeTile plus a frame threshold. */
extern s16 g_RaceFadeTimer;

/* Cursor of the in-race option overlay, clamped to 2 - g_GrandPrixMode. */
extern s16 g_RaceOptionCursor;

/* Best lap of this race so far (g_BestLapThisRace), seeded from g_BestLapTimes at the
 * grid, and DrawTimeValue, which prints one millisecond
 * time as m'ss"fff. Both are also referenced from render/, so they are
 * declared per file rather than here. */

/* The wrong-way warning: three sprites over a backing panel, drawn once
 * g_WrongWayTimer passes 10. */
void DrawWrongWayWarning(void);

/* Declared identically by 126 translation units before this
 * header carried them. */

extern s16 g_PlayerAutoSteer;
extern s32 g_AttractDemoStep;
extern s32 g_BestLapThisRace;
extern s32 g_BgmTrack;
extern s32 g_BonusCountStep;
extern s32 g_ClassClearFanfareTimer;
extern s32 g_ClassCompleted;
extern s32 g_ClassResultPlace;
extern s32 g_LapCount;
extern s16 g_PauseDebounce;
extern s32 g_PlayerCarZ;
extern s32 g_PlayerLapTimes[];
extern s32 g_PlayerProgressA;
extern s32 g_PlayerProgressB;
extern s16 g_PlayerTrackSection;
extern s32 g_PrizeAmount;
extern s32 g_PrizeScreenState;
extern s32 g_PromotionBonus;
extern s32 g_RacePaused;
extern s32 g_ReplayReadCursor;
extern s16 g_ReverbZoneDepth;
extern s16 g_RivalCueCooldown3;
extern s32 g_RivalCueFlags;
extern s32 g_SectorEndDistance[];
extern s32 g_SeriesCleared;
extern s32 g_TimeRecordTimes;
extern s16 g_TrackZoneCode;
extern s32 g_WaypointSpawnCooldown;

/*
 * None of the Draw* functions below draw. Each one packs primitives at the
 * scratchpad cursor and links them into an ordering table; the GPU is not
 * touched until boot/main_loop.c calls DrawOTag once per frame. That holds
 * for the whole family, render.h's DrawSprite / DrawLine / DrawSolidRect
 * included, which is why they are not spelled Queue* - the queueing is the
 * convention, not the exception.
 */
void DrawFlybyScenery(void);
u8 *DrawHudDigit(u8 *prim, s32 x, s32 y, s32 digit, u16 clut);
void DrawLostRaceCaption(s32 level);
void DrawPathScenery(void);
void DrawRoundScreen(void);
void DrawRouteScenery(void);
void DrawSplitDelta(s32 delta, s32 y);
void DrawTerrainCells(void);
void InitTrackScene(void);
s32 RequestSelectBgmAssets(void);
void ReturnToTitleScene(void);
void UpdateBgmTrackCount(void);
void UpdateFlybyScenery(void);
void UpdatePathScenerySound(void);
void UpdatePointAmbience(s32 arg);
void UpdateRouteScenery(void);
void UpdateZoneAmbience(s32 zone);

/* Declared identically by 153 translation units before this
 * header carried them. */

extern s16 g_EndingSceneLatch;
extern s32 D_801E4248;
extern s16 D_801E4CF8;
extern s16 g_AttractTitleDelays[];
extern s32 g_BestLapIndex;
extern s32 g_BgmRandomLabelTimer;
extern s32 g_BgmRandomPlay;
extern s32 g_CameraCarProgressA;
extern s16 g_CameraCarTrackSection;
extern char g_CaptionLapTime[];
extern char g_CaptionLapTime2[];
extern u8 g_CaptionLostRace;
extern u8 g_CaptionPrizeMoney[];
extern u8 g_CaptionPromotionBonus[];
extern char g_CaptionRanking[];
extern char g_CaptionRanking2[];
extern u8 g_CaptionTotalMoney[];
extern char g_CaptionTotalTime[];
extern char g_CaptionTotalTime2[];
extern s32 g_CarClassNames[];
extern s32 g_CarNames[];
extern s16 g_ChanceDigits[];
extern s32 g_ClassPromoted;
extern s16 g_ClassRecord5;
extern s16 g_ClassRecord6;
extern u8 g_ClockTextBuffer;
extern u8 g_ClockTextMinUnits[];
extern u8 g_ClockTextSecTens;
extern u8 g_ClockTextSecUnits;
extern s32 g_CountdownBoardOffset;
extern u32 g_CountdownDigitPatterns[16];
extern char *g_CourseNames[];
extern s32 g_DefaultLapTimes;
extern u16 g_DefaultRecordCars;
extern s32 g_DefaultRecordRows;
extern s32 g_DefaultRecordTimes;
extern s32 g_DefaultTotalTimes;
extern s32 g_EndingWashLevel;
extern char g_FmtCarName[];
extern char g_FmtClassGrandPrix[];
extern char g_FmtLapTime[];
extern u8 g_FmtMoney[];
extern char g_FmtRecordName[];
extern char g_FmtRoundIn[];
extern s32 g_LostRaceChoice;
extern u8 g_MsgEventOk[];
extern u8 g_MsgGame0Ok[];
extern u8 g_MsgGameExit;
extern u8 g_MsgInitEngineOk;
extern u8 g_MsgInitSoundOk;
extern u8 g_MsgResOk[];
extern u8 g_MsgSoundError;
extern s32 g_NameEntryChar;
extern u8 g_NameEntryCharset[];
extern s32 g_NameEntryCursor;
extern s16 g_PadMirrorMasks[];
extern u8 *g_PlaceSuffixNames[];
extern s16 g_PlayerGear;
extern s32 g_PlayerVelocity[];
extern s32 g_PrizeMoney3rd[][6][3];
extern s32 g_PrologueCutIndex;
extern s32 g_PrologueLineText[];
extern s16 g_PrologueLineX[];
extern s16 g_PrologueLineY[];
extern s32 g_PromotionBonusTable[];
extern char g_RaceOptionMarquee[4][40];
extern s32 g_RaceOptionPulseAngle;
extern s16 g_RaceOptionScroll0;
extern s16 g_RaceOptionScroll1;
extern s32 g_RankingInsertRow;
extern u8 g_RankingNameCodes[];
extern s32 g_RecordEntryState;
extern s32 g_RecordPanelSlide;
extern s32 g_RefSectorTime1;
extern s32 g_RefSectorTime2;
extern u16 g_ResultPanelCluts[];
extern u16 g_ResultPlaceCluts[];
extern s32 g_ReverbZoneEnd;
extern s32 g_ReverbZoneStart;
extern s16 g_RivalCueCooldown0;
extern s16 g_RivalCueCooldown1;
extern s16 g_RivalCueCooldown2;
extern u8 g_TachoFaceB;
extern u8 g_TachoFaceG;
extern u8 g_TachoFaceR;
extern char g_TextChance[];
extern u8 g_TextCongratulations;
extern char g_TextCourseIn[];
extern char g_TextEndRace[];
extern char g_TextPressStart[];
extern char g_TextResult[];
extern char g_TextTimeAttack[];
extern char g_TextTryAgain[];
extern u8 *g_TileStripBuffers[2];
extern u8 g_TileStripStorage[];
extern s32 g_TimeRecordInsertRow;
extern u8 g_TimeRecordNameCodes[];
extern u8 g_TimeTextBuffer[];
extern s32 g_WaypointsCollected;

s32 BeginMirrorPass(void);
void BeginReplay(void);
void BuildRaceHudPrims(s32);
void BuildTileStrips(void);
void CommitClassProgress(void);
s32 ComputeClassGrade(void);
s32 CycleAttractCameraCar(u32 mask, s32 current);
int CycleBgmSelectCameraCar();
void DrawAttractTitle(void);
void DrawBgmSelectBar();
void DrawLapNumber(void);
void DrawLapTimes(void);
void DrawNameEntryCursor(s32 charIndex, s32 row);
void DrawPlayerCarOnly(void);
void DrawPrologueText(void);
void DrawRaceHudLabels(s32 mode);
void DrawRacePosition(void);
void DrawRankingPanel(u8 *slideX);
void DrawRearViewMirror(s32 mode);
void DrawReplayBadge(void);
void DrawScriptedScenery(s32 flags);
void DrawSeriesClearedWash(s32 x, s32 y);
void DrawSpeedDigits(s32 x, s32 y, s32 speed);
void DrawSplitTimes(void);
void DrawStartGridScenery(s32 flags);
void DrawTimeRecordPanel(u8* s5);
void DrawTimeRemaining(s32 time);
void DrawWaypoints(void);
void ExitPrologue(void);
s32 FramesToMilliseconds(s32 frames, s32 millis);
s32 GetAttractTitleFade(s32 element);
void InitPathScenery(void);
void InsertRaceRecords(void);
void RecordReplayFrame(void);
void ResetFreeLookCamera(void);
void ResetMirrorState(void);
void ResetReplayWriteCursor(void);
void SeedFinishCamera(struct PlayerCarRuntime *car);
void SeedFlybyScenery(void);
void SeedReplayCars(void);
void SeedRouteScenery(void);
void SeedWaypoints(void);
void StoreReplayCarFrame(s32 pairIndex, u8* srcA, u8* srcB);
void StoreReplayTimeAttackFrame(s32 pointIndex, u8* srcPtr);
void TriggerRaceCues(void);
void UpdateFinishCamera(void* obj);
s32 UpdateLapAndFinish(struct PlayerCarRuntime *car, s32 grandPrixMode);
void UpdatePrologueLoad(void);
void UpdateRacePosition(void);
void UpdateReplayCars(void);
void UpdateRivalCueGate(void);
void UpdateSplitTimes(void *car, s32 grandPrixMode, s32 lapEvent);
void UpdateWaypoints(void);
void ExitRaceScene(s32 sceneId);
s32 GetTrackZoneBlend(s32 position);
void PlayCountdownCues(s32 timer);
void AdvanceBgmShuffleBag(u32 track);
void ApplyReplayFrame(s32 subframe, ReplayCarState *playerObj, ReplayCarState *rivalObj);
void ApplyReplayFrameAndTilt(s32 subframe, u8 *playerObj, u8 *rivalObj);
extern void (*g_AttractDemoSteps[])(void);
extern void (*g_BgmSelectSteps[])(void);

/* Declared identically by 5 translation units before this
 * header carried them. */

extern CVec g_CountdownCellColors[];
extern TrackWaypointSeed g_WaypointSeeds[];
extern TrackWaypointRuntime g_Waypoints[];

s32 IsCarNearWaypoint(TrackWaypointRuntime *waypoint);

#endif
