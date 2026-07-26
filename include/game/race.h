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

/* One save slot's Grand Prix / Time Attack progress; GameInitMenuMode copies it
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
extern s32 g_GrandPrixSave asm("D_801E4094");
extern s32 g_ExtraGrandPrixSave asm("D_801E6E7C");
extern s32 g_TimeAttackSave asm("D_8019C980");

typedef struct GameRaceRanking {
    s16 count;
    u8 pad2[0x1A];
    s32 values[1];
} GameRaceRanking;

/* Course-select gate: `g_CourseIndex < (class < 2 ? 2 : 3)`, or 6 : 7 for the
 * advanced series. This is the OVAL unlock; see names.md 5. */
s32 GameCanSelectNextCourse(void) asm("func_80053688");

/* The race-start signal gantry, live for 105 <= g_SceneTimer < 300: the "3" /
 * "2" / "1" / "GO" dot-matrix board from D_8007DDC0[1..4] plus the six start
 * lamps. See docs/names.md 1. */
void GameDrawStartCountdown(s32 sceneTimer) asm("func_8003425C");

/*
 * Per-course records, all in the memory-card save block. Per-file types.
 *   g_BestTotalTimes  D_8019C70C  [series][course][mode] ms
 *   g_BestLapTimes    D_801E4408  same shape, best single lap
 *   g_BestSectorTimes D_801E41E8  [series][course][3] sector splits
 *   g_CourseProgress  D_8009E67C  -> the running file's course-result record
 *   g_GrandPrixCourseProgress      D_801E42EC  row 0's record
 *   g_ExtraGrandPrixCourseProgress D_8009E874  row 1's record
 */


#endif
