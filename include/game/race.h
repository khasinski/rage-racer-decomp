#ifndef GAME_RACE_H
#define GAME_RACE_H

#include "common.h"

/* Grand Prix class index, 0-based; displayed as CLASS(n+1). Also the track
 * tier: course asset index = 0x57 + (course << 1) + (class << 3). OVAL is
 * gated to class >= 2. See names.md 3. */
extern s32 g_GrandPrixClass asm("D_8009E6A4");

/* Course selector: low 2 bits are the course (0 BIG, 1 MID, 2 HI, 3 OVAL).
 * Bits 2+ transiently carry g_GrandPrixSeries, so mask with 3. */
extern s32 g_CourseIndex asm("D_801E428C");

/* Which Grand Prix series is played: 0 = first (6 classes), non-zero =
 * advanced (5 classes). Outer index of every per-series table. */
extern s16 g_GrandPrixSeries asm("D_8019CABC");

/* Display names: [0..5] first-series classes, [6..10] advanced-series classes,
 * [11..13] course names. */
extern char *g_GrandPrixNames[] asm("D_8007D3D8");

/* Race position, 1 = leading; recomputed each frame from how many cars are
 * further along. At the finish it indexes g_PrizeMoney. */
extern s16 g_RacePosition asm("D_8009E834");

/* Prize money per [course][class][place], place 0 = 1st. */
extern s32 g_PrizeMoney[][6][3] asm("D_8007BEEC");

/* Round number within the current class; drives the "R O U N D %d" overlay. */
extern s32 g_GrandPrixRound asm("D_8009EC90");

/* 1 = Grand Prix (championship), 0 = Time Attack. Picks the pre-race panel, the
 * innermost index of the record tables, and the in-race option count
 * (2 - mode). See names.md 3. */
extern s16 g_GrandPrixMode asm("D_801E4DAC");

/* In-race copy of g_GrandPrixSeries, latched when the grid is built. Outer
 * index of the per-series tables and, because the advanced series runs the
 * courses backwards, also the lap-direction flag. See names.md 3. */
extern s32 g_RaceSeries asm("D_801E408C");

/* Race phase: 0 pre-start (physics frozen), 1 countdown, 2 racing, 4/5
 * finished, 7 goal/retire, 8 aborted. */
extern s16 g_RacePhase asm("D_801E6E74");

/* Series / save file the title menu picked (0 first, 1 advanced); also indexes
 * g_MaxClassReached. Final class is 4 for the first series, 5 for advanced. */
extern s16 g_SeriesSelection asm("D_801E4034");

/* Non-zero once the advanced series is unlocked (first series' last class
 * cleared). Saved at save+0x4E; gates title-menu entry 1. */
extern s16 g_AdvancedSeriesUnlocked asm("D_8019CAC0");

/* Highest class reached per series/save file ([1] is the old D_801E7730).
 * Unlocks courses and bounds the attract-demo class roll. Saved at save+0x50. */
extern s32 g_MaxClassReached[2] asm("D_801E772C");

/* Mirror mode, armed by holding the 0x80C pad combination as the race starts:
 * swaps left/right in steering, body roll, stereo pan and the sound cue. */
extern s32 g_MirrorMode asm("D_8019CACC");

/* One save slot's Grand Prix / Time Attack progress; InitMenuMode copies it
 * straight into the live globals and func_80053730 writes it back. */
typedef struct GameRaceProgress {
    s32 course;
    s32 carIndex;
    s32 classIndex;
    s32 maxClassReached; /* highest class unlocked in this slot */
    s32 unk10;      /* GP: elapsed time, clamped to 999999999.
                       Time Attack: g_GrandPrixSeries, read back as u16. */
} GameRaceProgress;

/* The save slot the front end is editing; repointed at one of the three below,
 * matching the title-menu row that g_CarTable was repointed for. Declared s32
 * because most translation units only touch the first word. */
extern GameRaceProgress *g_RaceProgress asm("D_801E4FAC");
/* The three slots themselves. Their fields used to be spelled as separate
 * symbols per serialiser (g_GrandPrixSaveCar and friends); they are members. */
extern GameRaceProgress g_GrandPrixSave asm("D_801E4094");
extern GameRaceProgress g_ExtraGrandPrixSave asm("D_801E6E7C");
extern GameRaceProgress g_TimeAttackSave asm("D_8019C980");

typedef struct GameRaceRanking {
    s16 count;
    u8 pad2[0x1A];
    s32 values[1];
} GameRaceRanking;

/* Course-select gate: `g_CourseIndex < (class < 2 ? 2 : 3)`, or 6 : 7 for the
 * advanced series. This is the OVAL unlock; see names.md 5. */
s32 CanSelectNextCourse(void) asm("func_80053688");

/* The race-start signal gantry, live for 105 <= g_SceneTimer < 300: the "3" /
 * "2" / "1" / "GO" dot-matrix board from D_8007DDC0[1..4] plus the six start
 * lamps. See docs/names.md 1. */
void DrawStartCountdown(s32 sceneTimer) asm("func_8003425C");

/*
 * Per-course records, all in the memory-card save block. Per-file types.
 *   g_BestTotalTimes  D_8019C70C  [series][course][mode] ms
 *   g_BestLapTimes    D_801E4408  same shape, best single lap
 *   g_BestSectorTimes D_801E41E8  [series][course][3] sector splits
 *   g_CourseProgress  D_8009E67C  -> the running file's course-result record
 *   g_GrandPrixCourseProgress      D_801E42EC  row 0's record
 *   g_ExtraGrandPrixCourseProgress D_8009E874  row 1's record
 */

/*
 * Live race timing. Times are milliseconds; 0x927BF is the saturation value
 * for anything over 9'59"998, which the HUD prints as dashes. See names.md 15.
 */

/* Elapsed time of the lap in progress. */
extern s32 g_LapTimeMs asm("D_801E4D64");

/* Grand Prix time limit, in frames; counts down while g_RacePhase >= 2 and
 * forces g_RacePhase = 5 when it reaches 0. Seeded to 15000. */
extern s32 g_RaceTimeRemaining asm("D_8009AF9C");

/* Sector being timed, 0..2; -2 before the first start-line crossing. */
extern s32 g_SectorIndex asm("D_801E4148");

/* This lap's three sector times, filled in as each boundary is crossed. */
extern s32 g_SectorTimes[3] asm("D_8009AF80");

/* Total of the best lap the split is measured against; seeded from the save
 * records and written back when the race completes. */
extern s32 g_RefLapTime asm("D_8009AF8C");

/* Two 3-element arrays:
 *   g_SectorEndDistance[3]  D_801E4D98/9C/A0  lap distance ending each sector
 *                                             (L/3, 2L/3, L)
 *   g_RefSectorTimes[3]     D_8009AF90/94/98  the best lap's sector times
 * g_RefSectorTimes is the one exception to one-address-one-name in this header:
 * race/UpdateLapAndFinish.c must spell its three elements as the separate
 * scalars g_RefSectorTime0/1/2 or gcc 2.6.3 CSEs the array base into a register
 * and reschedules the surrounding block. g_SectorEndDistance was checked the
 * same way and does not need it, so it is indexed everywhere.
 */

/* Split readout: the sector time just recorded, the unsigned difference from
 * the reference, and its sign (+1 ahead, -1 behind, 0 no split). */
extern s32 g_LastSectorTime asm("D_8009AF78");
extern s32 g_SplitDelta asm("D_8009AF7C");
extern s16 g_SplitSign asm("D_8009AFAC");

/* Which sector's reference is on screen, the reference time itself, and the
 * 0..0x3C frame counter that ends the split display. */
extern s16 g_SplitSector asm("D_8009AFA4");
extern s32 g_SplitTargetTime asm("D_8009AFB0");
extern s16 g_SplitTimer asm("D_8009AFA8");

/* Set when a lap time saturates at 0x927BF; write-only in retail. */
extern s32 g_LapTimeSaturated asm("D_8009AFA0");

/* Frames the player has been driving the wrong way. Past 10 the warning shows
 * and rival cues are muted; in Time Attack 60 on lap 0 aborts the run. */
extern s16 g_WrongWayTimer asm("D_801E8A8C");

/* g_PlayerCar.facingBackwards. Wrong way is `!= g_RaceSeries`, because the
 * advanced series drives the course in the other direction. */
extern s16 g_PlayerFacingBackwards asm("D_8009E78C");

/* Non-zero while rival proximity / position sound cues may play: set only in
 * the middle of a lap and cleared by the wrong-way warning. */
extern s16 g_RivalCueEnabled asm("D_8009E6A0");

/* Frame counter of the in-race fade transitions; every use is the brightness
 * argument of DrawFullscreenFadeTile plus a frame threshold. */
extern s16 g_RaceFadeTimer asm("D_801E43FC");

/* Cursor of the in-race option overlay, clamped to 2 - g_GrandPrixMode. */
extern s16 g_RaceOptionCursor asm("D_801E414C");

/* Best lap of this race so far (D_801E4BCC), seeded from g_BestLapTimes at the
 * grid, and DrawTimeValue (func_80033D50), which prints one millisecond
 * time as m'ss"fff. Both are also referenced from render/, so they are
 * declared per file rather than here. */

/* The wrong-way warning: three sprites over a backing panel, drawn once
 * g_WrongWayTimer passes 10. */
void DrawWrongWayWarning(void) asm("func_800333DC");

#endif
