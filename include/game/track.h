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

/* Track centreline points of the loaded course, g_TrackPointCount of them;
 * walked cyclically. */
extern GameTrackPoint *g_TrackPoints asm("D_8009E688");

/* Valid entries in g_TrackPoints; every walker wraps with `% this`. */
extern s32 g_TrackPointCount asm("D_8009E6A8");

/*
 * Animated course scenery (func_8003Dxxx / func_8003Fxxx). All four courses
 * share one coordinate space, so prop positions are one static table at
 * 0x8007E2C0 and each prop culls itself against the visible-terrain bitmask
 * D_801E6828. Full write-up in docs/names.md section 5b.
 */

/* Per-frame update+draw of the current course's props, dispatched on the course
 * index. ...Scenery is the race copy (course passed in), ...Scenery2 the copy
 * for the replay/attract scenes (reads g_CourseIndex); they keep separate
 * animation state. `animate` == 0 draws a frozen frame. */
void GameDrawCourseScenery(s32 course, s32 timer, s32 animate) asm("func_8003E1A4");
void GameDrawCourseScenery2(s32 timer, s32 animate) asm("func_8003E2E8");

/* The two-part animated prop at D_8007E2C0[0..1]: a 16-phase model swap plus a
 * companion part. Grand Prix only, nothing drawn in class 5. */
void GameDrawAnimatedScenery(s32 timer, s32 instance) asm("func_8003D6F0");
void GameDrawAnimatedScenery2(s32 timer, s32 instance, s32 isReplay, s32 animate) asm("func_8003DA90");

/* The spinners: 1 on MYTHICAL COAST, 3 on OVER PASS CITY from class 2 up. A
 * 12-bit angle in D_8007E334[] spins about Z at a rate re-randomised every
 * 512 frames. */
void GameDrawSpinningScenery(s32 timer, s32 animate) asm("func_8003DDAC");

/* The static landmark at D_8007E340 (40594, 6002, 11940), on all four courses;
 * pass 1 for THE EXTREME OVAL's +0x5000 z shift. Model 0x3A or 0x3B depending
 * on D_801E4030. */
void GameDrawStaticScenery(s32 shifted) asm("func_8003DF68");

/* A second static landmark at (29266, 6039, 45612): MYTHICAL COAST only, from
 * g_GrandPrixClass >= 4, and the one prop with no visibility cull. */
void GameDrawHighClassScenery(void) asm("func_8003E0D0");

/* State of a shuttling prop: it runs between the two endpoints of its path in
 * D_8007E360, dwells, then reverses. Paths and timings in names.md 5b. */
typedef struct GameShuttleScenery {
    s32 dwellCounter;  /* +0x00 frames waited at the endpoint, capped at D_8007E3E0[path] */
    s32 unk04;
    s32 travelStep;    /* +0x08 progress along the leg, capped at D_8007E3D8[path] */
    s16 startEndpoint; /* +0x0C which of the path's two endpoints this leg started from */
    s16 pathIndex;     /* +0x0E path: 0 OVER PASS CITY, 1 and 2 LAKESIDE GATE */
    s32 x;             /* +0x10 interpolated world position; also the cull key */
    s32 y;             /* +0x14 */
    s32 z;             /* +0x18 */
    s32 unk1C;         /* +0x1C fourth word copied out of the endpoint record */
    s32 angleX;        /* +0x20 seeded from D_8007E3C0, never read by the drawer */
    s32 angleY;        /* +0x24 Y rotation (func_8001A530) */
    s32 angleZ;        /* +0x28 Z rotation (func_8001A4C0) */
    u8 pad2C[8];
} GameShuttleScenery;

/* The two shuttle instances. Instance 1's fields also carry their own split
 * symbols D_801E4FEC..D_801E5014. */
extern GameShuttleScenery g_ShuttleScenery[2] asm("D_801E4FB8");

void GameUpdateShuttleScenery(s32 instance) asm("func_8003F2A4");
void GameDrawShuttleScenery(s32 instance) asm("func_8003F4BC");
void GameInitShuttleScenery(void) asm("func_8003F0F8");

/* Lap distance: the sum of every g_TrackPoints[].segmentLength. Cars' along-
 * track progress uses the same units. */
extern s32 g_TrackLength asm("D_801E40D8");

/* Base of the course's event/marker block (func_80034E04 installs it). Starts
 * with the s32 track-walk start index; sub-table offsets are at +0xB64..+0xB78
 * and the per-series marker rows at + g_RaceSeries * 576 + 0x474. */
extern u8 *g_TrackEventData asm("D_801E4150");

#endif
