#ifndef GAME_TRACK_H
#define GAME_TRACK_H

#include "common.h"

typedef struct GameTrackPoint {
    s32 x;
    s32 z;
    s16 y;
    s16 angle;
    u8 padC[4];
    s16 field_10;
    s16 field_12;
    u8 pad14[2];
    s16 segmentLength;
} GameTrackPoint;

/*
 * Track centreline points of the loaded course, g_TrackPointCount of them.
 * Walked cyclically by the car physics/AI and by the track renderer.
 */
extern GameTrackPoint *g_TrackPoints asm("D_8009E688");

/*
 * Number of valid entries in the track centreline point array g_TrackPoints.
 * Set by func_8002A6B0 when the track is built; every walker of the array
 * wraps its index with `% g_TrackPointCount`.
 */
extern s32 g_TrackPointCount asm("D_8009E6A8");

/*
 * ---------------------------------------------------------------------------
 * Animated course scenery (the func_8003Dxxx / func_8003Fxxx family)
 * ---------------------------------------------------------------------------
 *
 * Landmark set pieces of the shared world: a windmill-style spinner, a vehicle
 * that shuttles between two points, and a few animated and static props. All
 * four courses live in one coordinate space (they are different routes through
 * the same landscape, with THE EXTREME OVAL shifted +0x5000 in z), so the
 * positions are one static table in .rodata at 0x8007E2C0..0x8007E3E8 and every
 * prop decides for itself whether it is on screen.
 *
 * Each leaf follows the same four steps:
 *   1. take its fixed world position and heading from the static table;
 *   2. cull itself against the visible-terrain bitmask D_801E6828 - a 32x32 bit
 *      grid of 2048-unit cells that func_800414F0 rebuilds every frame from the
 *      camera, so a prop is skipped unless the cell it stands in is visible;
 *   3. build its rotation matrix (func_8001A4C0/530/5A0 = Z/Y/X) and hand it to
 *      GameSetGteObjectMatrix;
 *   4. submit a model by index into the loaded course object bank through
 *      func_800296B4 / func_80029E50, clamping the index against the bank size
 *      D_801E40E4 and falling back to model 1 when the bank is too small.
 *
 * The recurring `g_GrandPrixClass == 5` test is not an ending sequence: class 5
 * (the last class) forces the animate flag to 0, and the animated props skip
 * drawing altogether. `D_801E4DAC != 0` means a Grand Prix is running, so the
 * animated props do not appear in Time Attack at all.
 */

/*
 * Per-frame update+draw of every animated prop belonging to the current course,
 * dispatched on the course index (0 = MYTHICAL COAST, 1 = OVER PASS CITY,
 * 2 = LAKESIDE GATE, 3 = THE EXTREME OVAL):
 *
 *   0: spinners (+ the class-4 prop)   1: spinner (class 2+) and one shuttle
 *   2: two shuttles                    3: a second animated prop instance
 *
 * and the static prop on every course. GameDrawCourseScenery is the copy the
 * race itself uses (scene 0xC, func_800363D4 / func_80037200 / func_80037D90)
 * and takes the course explicitly; GameDrawCourseScenery2 is the copy the other
 * 3D scenes use (the replay scene 0x11 func_8001FD3C, the attract/free-run
 * scene 0x1E via func_80026920, func_80025ED8, func_80026F9C) and reads
 * g_CourseIndex itself. The two keep separate animation state
 * (D_8007E2E4/E8/EA vs D_8007E2EC/F0/F2) so neither disturbs the other.
 *
 * Arguments are (course, timer, animate) and (timer, animate); `animate` gates
 * every state advance, so passing 0 draws a frozen frame.
 */
void GameDrawCourseScenery(s32 course, s32 timer, s32 animate) asm("func_8003E1A4");
void GameDrawCourseScenery2(s32 timer, s32 animate) asm("func_8003E2E8");

/*
 * The two-part animated prop, at D_8007E2C0[0..1]. Plays a 16-phase model-swap
 * animation - one phase every 4 frames, model index = phase + 0xA (holding one
 * of four end poses for the last three phases) or phase + 0x18 - and draws a
 * companion part on top whose model is re-rolled from 3 variants every 64
 * frames and whose 0x1F800084 draw-mode word cycles through 4 steps every 8
 * frames. Grand Prix only, and nothing is drawn in class 5.
 * ...Scenery is the race copy (it also freezes on the race's D_801E4BAC pause
 * flag); ...Scenery2 is the copy for the replay/attract scenes, which is driven
 * purely by its arguments and picks the animation by `g_SceneId == 0x11`.
 */
void GameDrawAnimatedScenery(s32 timer, s32 instance) asm("func_8003D6F0");
void GameDrawAnimatedScenery2(s32 timer, s32 instance, s32 isReplay, s32 animate) asm("func_8003DA90");

/*
 * The wind-driven spinners: one instance on MYTHICAL COAST, a row of three on
 * OVER PASS CITY (only from class 2 up). Each holds a 12-bit angle in
 * D_8007E334[] that accumulates D_8007E33C[course != 0] every frame and spins
 * the model about Z (a horizontal axis, applied after the heading rotation).
 * The two speeds are re-randomised to rand & 0x1F / rand & 0x3F every 512
 * frames, i.e. the rotation rate drifts like wind - which together with the
 * three-in-a-row layout is why these read as windmills, though the model itself
 * is in the course object bank and has not been confirmed.
 */
void GameDrawSpinningScenery(s32 timer, s32 animate) asm("func_8003DDAC");

/*
 * The static landmark at D_8007E340 (40594, 6002, 11940), drawn on all four
 * courses; pass 1 for THE EXTREME OVAL to apply the +0x5000 z shift. Sits right
 * beside GameDrawAnimatedScenery's first instance. Model 0x3A or 0x3B depending
 * on D_801E4030.
 */
void GameDrawStaticScenery(s32 shifted) asm("func_8003DF68");

/*
 * A second static landmark, at (29266, 6039, 45612). Reached only from the
 * MYTHICAL COAST branch and only once g_GrandPrixClass >= 4, and it is the one
 * prop with no visibility cull.
 */
void GameDrawHighClassScenery(void) asm("func_8003E0D0");

/*
 * The shuttling prop: a vehicle that runs between two fixed endpoints, waits,
 * and reverses. State lives in D_801E4FB8[instance] (0x34 bytes: dwell counter,
 * travel step, which endpoint it started from, path index, then the interpolated
 * position at +0x10 and the two heading angles at +0x24/+0x28).
 *
 * GameUpdateShuttleScenery advances it: the dwell counter climbs to
 * D_8007E3E0[path] at the endpoint, then `step` climbs to D_8007E3D8[path] while
 * the position is lerped between D_8007E360's two endpoints for that path, and
 * on arrival the start endpoint flips so the next leg runs backwards.
 * GameDrawShuttleScenery culls and draws it (model 0x3F, or 0x3C from course 2
 * up; on LAKESIDE GATE it also ignores the cull). GameInitShuttleScenery seeds
 * both instances when the course loads.
 *
 * Paths: OVER PASS CITY gets one instance on path 0, which descends 2938 units
 * over 628 frames and waits 300; LAKESIDE GATE gets two, on paths 1 and 2,
 * which are the same near-level 3400-unit run traversed in opposite directions
 * so the pair passes mid-way (512-frame traversal, 128-frame wait).
 */
void GameUpdateShuttleScenery(s32 instance) asm("func_8003F2A4");
void GameDrawShuttleScenery(s32 instance) asm("func_8003F4BC");
void GameInitShuttleScenery(void) asm("func_8003F0F8");

/*
 * Total length of the loaded course: func_8002A6B0 sums every point's
 * segmentLength into it while it builds g_TrackPoints. Cars' along-track
 * progress is measured in the same units, so this is the lap distance -
 * func_80037200 ends the lap at `>= g_TrackLength`, func_80037808 /
 * func_8003591C multiply it by the lap count to get the race distance, and the
 * reverse-direction code mirrors a position with `g_TrackLength - position`.
 */
extern s32 g_TrackLength asm("D_801E40D8");

/*
 * Base of the loaded course's event/marker data block. func_80034E04 installs it
 * (`g_TrackEventData = arg0`, then it derives D_801E4448 / D_8019CAF8 /
 * D_801E4128 / D_801E4BA4 / D_801E4174 from the offset words at +0xB64..+0xB78)
 * and logs "event ok". The block starts with an s32 - `*(s32 *)g_TrackEventData`
 * is the track-walk start index used by func_8002BF68 / func_8003591C /
 * func_800250BC - and func_80038FF0 reads 8-entry marker rows of 0x40 bytes out
 * of it, per series: `g_TrackEventData + g_RaceSeries * 576 + 0x474`.
 */
extern u8 *g_TrackEventData asm("D_801E4150");

#endif
