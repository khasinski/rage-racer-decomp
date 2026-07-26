#ifndef GAME_RACE_H
#define GAME_RACE_H

#include "common.h"

/*
 * Grand Prix class index, 0-based. Displayed as `CLASS%d` = g_GrandPrixClass+1
 * by func_800201D4, which also picks the class name
 * g_GrandPrixNames[g_GrandPrixSeries ? class+6 : class]
 * ("CALME".."DIABLE" / "AISANCE".."RAGE").
 *
 * Also selects the per-class track variant: the asset index of a course pack is
 *     0x57 + (g_CourseIndex << 1) + (g_GrandPrixClass << 3)
 * which walks g_AssetPaths[87..134] = "BIG1".."OVAL6" (6 classes x 4 courses x
 * 2 halves), i.e. class N loads the "<course><N+1>" track files.
 *
 * The OVAL course is gated on this: func_80018C88 re-rolls the class into
 * [2, ...] whenever the course is OVAL, so OVAL only ever appears from
 * CLASS 3 upward.
 */
extern s32 g_GrandPrixClass asm("D_8009E6A4");

/*
 * Course selector. The low 2 bits are the course: 0 = BIG, 1 = MID, 2 = HI,
 * 3 = OVAL (the order of the track packs in g_AssetPaths). It is used directly
 * as a 0..3 index into the per-course tables (prize money D_8007BEEC, records
 * D_801E7744 / D_8019CB78, D_8019C70C, D_801E4408).
 *
 * Bits 2+ additionally carry g_GrandPrixSeries across a screen transition:
 * func_80050C18 packs it in (`= (series << 2) | course`), func_80053730
 * unpacks it (`series = course >> 2`) and clears the course bits (`& ~3`).
 * So a reader must mask with 3 when only the course is wanted.
 */
extern s32 g_CourseIndex asm("D_801E428C");

/*
 * Which Grand Prix series is being played: 0 = the first series (class names
 * "CALME".."DIABLE", 6 classes), non-zero = the advanced series
 * ("AISANCE".."RAGE", 5 classes). Selects the name set in g_GrandPrixNames
 * (offset +6), the per-series progression g_MaxClassReached[], and the outer
 * dimension of the per-series record tables (D_801E7744, D_8019CB78,
 * D_8019C70C, D_801E4408). func_80038844 copies it to g_RaceSeries for the
 * in-race code.
 */
extern s16 g_GrandPrixSeries asm("D_8019CABC");

/*
 * Grand Prix / course display names. [0..5] are the first-series class names
 * CALME, BRISE, RAFALE, MISTRAL, TEMPETE, DIABLE; [6..10] the advanced-series
 * names AISANCE, AGITATION, IRRITATION, COLERE, RAGE; [11..13] the course
 * names MYTHICAL COAST, OVER PASS CITY, LAKESIDE GATE.
 */
extern char *g_GrandPrixNames[] asm("D_8007D3D8");

/*
 * The player's race position, 1 = leading. func_8003AE2C recomputes it each
 * frame by counting how many of the 11 cars are further along the track.
 * At the end of a race it is the finishing place: func_800206B8 awards
 * g_PrizeMoney[course][class][g_RacePosition - 1] and records the best
 * (lowest) value reached per course, and a 1 selects the victory sound cue.
 */
extern s16 g_RacePosition asm("D_8009E834");

/*
 * Prize money per [course][class][place], place 0 = 1st. Displayed by
 * func_8001C974 through the "1ST/%7dv" / "2ND/%7dv" / "3RD/%7dv" formats and
 * paid out by func_800206B8.
 */
extern s32 g_PrizeMoney[][6][3] asm("D_8007BEEC");

/*
 * Grand Prix round number within the current class, displayed by the
 * "R O U N D %d" / "ROUND%d IN" overlays (func_8001C974 / func_8001FC30).
 * Reset to 0 and incremented by func_8001C7BC.
 */
extern s32 g_GrandPrixRound asm("D_8009EC90");

/*
 * Non-zero while a Grand Prix (championship) race is being played, 0 for Time
 * Attack. func_8001B884 sets it from the title-menu selection: entries 0 and 1
 * (the two Grand Prix save files, g_TitleMenuSelection) set it to 1, entry 2
 * (Time Attack) sets it to 0; the attract demo (func_80023BB4 case 3) clears it.
 *
 * It selects the pre-race info panel (func_8001C974 shows "R O U N D %d" plus
 * g_PrizeMoney in Grand Prix, best-lap / best-total times in Time Attack), the
 * innermost index of the record tables D_8019C70C / D_801E4408
 * ([series][course][g_GrandPrixMode]), and the number of selectable entries in
 * the in-race option list (`2 - g_GrandPrixMode`, func_80037200 / func_8003591C).
 */
extern s16 g_GrandPrixMode asm("D_801E4DAC");

/*
 * In-race copy of g_GrandPrixSeries, latched by func_80038844 when the starting
 * grid is built (`D_801E408C = g_GrandPrixSeries`). Used exactly like
 * g_GrandPrixSeries as the outer index of the per-series tables (D_8019C70C,
 * D_801E4408, D_801E774C, D_801E41E8) and of the marker-table block
 * (`D_801E4150 + g_RaceSeries * 576`).
 *
 * The advanced series runs the same courses in the opposite direction, so the
 * physics and AI also use it as the lap-direction flag: func_8003AEBC looks two
 * points ahead instead of two behind, func_8002BF68 / func_8002C168 accumulate
 * segmentLength forwards instead of backwards, and func_80038FF0 /
 * func_80040DB4 / func_80034F74 / func_80040ADC mirror the track progress with
 * `g_TrackLength - progress`.
 */
extern s32 g_RaceSeries asm("D_801E408C");

/*
 * Race phase, driven by the race main loops func_80037200 / func_80037808 /
 * func_8003591C. 0 = pre-start (physics frozen: `if (phase > 0)` gates
 * func_8002DEFC), 1 = countdown, 2 = racing, 4 / 5 = finished (the HUD and the
 * lean/roll update stop at `>= 4`), 7 = goal / retire, 8 = aborted. Reset to 0
 * by func_8003591C when a race is armed.
 */
extern s16 g_RacePhase asm("D_801E6E74");

/*
 * Which series the title menu picked, 0 = first series, 1 = advanced series;
 * func_8001B5DC sets it from g_TitleMenuSelection (entry 0 -> 0, entry 1 -> 1,
 * Time Attack -> 0) and GameInitMenuMode copies it into g_GrandPrixSeries.
 * It also selects the save file whose progression is being played
 * (g_MaxClassReached[g_SeriesSelection], func_800206B8) and the final class of
 * that series (4 for the first, 5 for the advanced one).
 */
extern s16 g_SeriesSelection asm("D_801E4034");

/*
 * Non-zero once the advanced series has been unlocked - func_800206B8 sets it
 * after the last class of the first series is cleared
 * (`g_SeriesSelection == 0 && g_GrandPrixClass == 4`). It is stored in the save
 * block at +0x4E, gates title-menu entry 1 (func_8001B5DC skips over it and
 * func_8001B2D4 skips drawing it while this is 0), and switches
 * func_80053688's course limit from 2-3 to 6-7.
 */
extern s16 g_AdvancedSeriesUnlocked asm("D_8019CAC0");

/*
 * Highest Grand Prix class reached, one entry per series / save file: index 0 is
 * the first series (D_801E772C), index 1 the advanced one (D_801E7730).
 * func_800206B8 raises `g_MaxClassReached[g_SeriesSelection]` when a class is
 * cleared, func_80053688 unlocks courses from it, func_80018A70 rolls the
 * attract-demo class inside `% (g_MaxClassReached[series] + 1)`, and the memory
 * card block keeps it at +0x50 (GameLoadSaveStateBlock / func_8005F88C).
 */
extern s32 g_MaxClassReached[2] asm("D_801E772C");

/*
 * Mirror mode: func_8001C974 sets it when the pad holds the 0x80C combination
 * as the race scene starts, and every screen entry (GameInitMenuMode,
 * func_8002317C, func_8001FC30) clears it. When set, left and right are
 * exchanged everywhere - func_8002CD4C swaps the two steering button masks and
 * negates the body roll, func_80040ADC and func_8004087C swap the two stereo
 * arguments of the pan calls, and func_8002DEFC picks the mirrored sound cue
 * (0xC instead of 0xB).
 */
extern s32 g_MirrorMode asm("D_8019CACC");

typedef struct GameRaceProgress {
    s32 state;
    s32 pad4;
    s32 lap;
    s32 progression;
    s32 elapsedTime;
} GameRaceProgress;

typedef struct GameRaceRanking {
    s16 count;
    u8 pad2[0x1A];
    s32 values[1];
} GameRaceRanking;

/*
 * Course-select gate: may the cursor still step up to the next course?
 * Returns g_CourseIndex < limit, where the limit accounts for both the series
 * packed into bits 2+ of g_CourseIndex and the OVAL unlock:
 *
 *     series 0 (courses 0..3):  limit = class < 2 ? 2 : 3
 *     series 1 (courses 4..7):  limit = class < 2 ? 6 : 7
 *
 * i.e. only CLASS 3 and up may reach course 3 (OVAL), matching func_80018C88.
 * The class read is g_GrandPrixClass while a Grand Prix is live
 * (D_801E4DAC != 0) and the saved per-series class D_801E772C / D_801E7730
 * otherwise. func_80053730 uses it both to gate a Right press and to tell
 * func_80049418 whether to show the "next course" arrow.
 */
s32 GameCanSelectNextCourse(void) asm("func_80053688");

#endif
