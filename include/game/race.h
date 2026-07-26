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
 * (offset +6), the class count D_801E772C[], and the outer dimension of the
 * per-series record tables (D_801E7744, D_8019CB78, D_8019C70C, D_801E4408).
 * func_80038844 copies it to D_801E408C for the in-race code.
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

#endif
