#ifndef GAME_TRACK_H
#define GAME_TRACK_H

#include "common.h"

/*
 * One centreline point, 0x18 bytes. `field_10` / `field_12` are the left and
 * right half-widths (SteerCarAlongRoute clamps the lateral offset to
 * [-field_10, field_12]); everything from `field_C` up is interpolated between
 * a segment's two endpoints by UpdateCarTrackState and its non-clamping
 * twin ResetCarTrackState. See docs/names.md 31.
 */
typedef struct GameTrackPoint {
    s32 x;
    s32 z;
    s16 y;
    s16 angle;
    /* +0x0C the pitch component of the surface tilt: interpolated, then paired
     * with the cross-slope angle derived from field_E and rotated by the car's
     * track-relative heading to give the two tilt words at obj +0x20 / +0x28. */
    s16 field_C;
    /* +0x0E cross-slope gradient in 1/128 of a unit per unit of lateral
     * offset. `surfaceY = interp(y) + (interp(field_E) * lateral >> 7)` in
     * GetTrackSurfaceHeight, SampleTrackSurfaceHeight and
     * UpdateCarTrackState alike. */
    s16 field_E;
    s16 field_10;
    s16 field_12;
    /* +0x14 arc reference, read as one u16 and split: bits 0..1 select the
     * cornering model (0 = straight, the arc block is skipped entirely; 2
     * negates the lateral offset, so it is the mirrored hand), and bits 4..15
     * are a signed index into g_TrackArcCenters (`(s16)arcRef >> 4`). Bits 2..3
     * are never read. */
    u16 arcRef;
    s16 segmentLength;
} GameTrackPoint;

/*
 * One corner's centre of curvature. `GameTrackPoint.arcRef >> 4` indexes this
 * array, which InstallTrackPoints publishes at `g_TrackArcCenters`
 * (g_TrackArcCenters) immediately after the point table. The stride is 12, proven by
 * three independent `* 0xC` sites (UpdateCarDrivetrain, and 8003237C /
 * UpdateCarTrackState); the third word is never read anywhere in the image.
 *
 * g_TrackArcCenters is declared per-file, and the two existing declarations disagree:
 * `u8 *` in car/UpdateCarDrivetrain.c (which does its own `* 0xC`) and
 * `GameTrackPoint *` in track/InstallTrackPoints.c, where the element type
 * is wrong but harmless because that unit only computes the base address. No
 * extern is declared here on purpose: adding one would collide with those two.
 */
typedef struct GameTrackArcCenter {
    s32 x;      /* +0x00 */
    s32 z;      /* +0x04 */
    s32 unk08;  /* +0x08 never read */
} GameTrackArcCenter;

/* Track centreline points of the loaded course, g_TrackPointCount of them;
 * walked cyclically. */
extern GameTrackPoint *g_TrackPoints;

/* Valid entries in g_TrackPoints; every walker wraps with `% this`. */
extern s32 g_TrackPointCount;

/*
 * Animated course scenery (func_8003Dxxx / func_8003Fxxx). All four courses
 * share one coordinate space, so prop positions are one static table at
 * 0x8007E2C0 and each prop culls itself against the visible-terrain bitmask
 * g_VisibleCellMask. Full write-up in docs/names.md section 5b.
 */

/* Per-frame update+draw of the current course's props, dispatched on the course
 * index. ...Scenery is the race copy (course passed in), ...Scenery2 the copy
 * for the replay/attract scenes (reads g_CourseIndex); they keep separate
 * animation state. `animate` == 0 draws a frozen frame. */
void DrawCourseScenery(s32 course, s32 timer, s32 animate);
void DrawCourseScenery2(s32 timer, s32 animate);

/* The two-part animated prop at g_AnimSceneryPos[0..1]: a 16-phase model swap plus a
 * companion part. Grand Prix only, nothing drawn in class 5. */
void DrawAnimatedScenery(s32 timer, s32 instance);
void DrawAnimatedScenery2(s32 timer, s32 instance, s32 isReplay, s32 animate);

/* The spinners: 1 on MYTHICAL COAST, 3 on OVER PASS CITY from class 2 up. A
 * 12-bit angle in g_SpinningSceneryAngle[] spins about Z at a rate re-randomised every
 * 512 frames. */
void DrawSpinningScenery(s32 timer, s32 animate);

/* The static landmark at g_StaticSceneryPos (40594, 6002, 11940), on all four courses;
 * pass 1 for THE EXTREME OVAL's +0x5000 z shift. Model 0x3A or 0x3B depending
 * on g_IsEnvironmentMode4. */
void DrawStaticScenery(s32 shifted);

/* A second static landmark at (29266, 6039, 45612): MYTHICAL COAST only, from
 * g_GrandPrixClass >= 4, and the one prop with no visibility cull. */
void DrawHighClassScenery(void);

/* State of a shuttling prop: it runs between the two endpoints of its path in
 * g_ShuttlePathPoints, dwells, then reverses. Paths and timings in names.md 5b. */
typedef struct GameShuttleScenery {
    s32 dwellCounter;  /* +0x00 frames waited at the endpoint, capped at g_ShuttlePathDwellMax[path] */
    s32 unk04;
    s32 travelStep;    /* +0x08 progress along the leg, capped at g_ShuttlePathTravelMax[path] */
    s16 startEndpoint; /* +0x0C which of the path's two endpoints this leg started from */
    s16 pathIndex;     /* +0x0E path: 0 OVER PASS CITY, 1 and 2 LAKESIDE GATE */
    s32 x;             /* +0x10 interpolated world position; also the cull key */
    s32 y;             /* +0x14 */
    s32 z;             /* +0x18 */
    s32 unk1C;         /* +0x1C fourth word copied out of the endpoint record */
    s32 angleX;        /* +0x20 seeded from g_ShuttlePathAngles, never read by the drawer */
    s32 angleY;        /* +0x24 Y rotation (BuildRotMatrixY) */
    s32 angleZ;        /* +0x28 Z rotation (BuildRotMatrixZ) */
    u8 pad2C[8];
} GameShuttleScenery;

/* The two shuttle instances. Instance 1's fields also carry their own split
 * symbols g_Shuttle1DwellCounter..g_Shuttle1AngleZ. */
extern GameShuttleScenery g_ShuttleScenery[2];

void UpdateShuttleScenery(s32 instance);
void DrawShuttleScenery(s32 instance);
void InitShuttleScenery(void);

/* Lap distance: the sum of every g_TrackPoints[].segmentLength. Cars' along-
 * track progress uses the same units. */
extern s32 g_TrackLength;

/* Base of the course's event/marker block (InstallTrackEventData installs it). Starts
 * with the s32 track-walk start index; sub-table offsets are at +0xB64..+0xB78
 * and the per-series marker rows at + g_RaceSeries * 576 + 0x474. */
extern u8 *g_TrackEventData;

/* Declared identically by 73 translation units before this
 * header carried them. */

extern s32 D_801E3F60;
extern s32 g_CameraCarAngleY;
extern s32 g_CameraCarHeading;
extern s32 g_CameraCarSpeed;
extern s32 g_CameraCarStepX;
extern s32 g_CameraCarStepZ;
extern s32 g_CameraCarY;
extern s32 g_CameraCarZ;
extern s32 g_CourseModelCount;
extern s16 g_EnvFogEnabled;
extern s16 g_EnvLerpDuration;
extern u32 *g_EnvScriptCues;
extern u8 g_EnvSpare;
extern s16 g_EnvSpareFrom;
extern s16 g_EnvSpareLerp;
extern s16 g_EnvSpareTo;
extern s32 g_FlybySceneryRotX;
extern s32 g_FlybySceneryRotY;
extern s32 g_FlybySceneryRotZ;
extern s32 g_PlayerSpeed;
extern s32 g_PlayerTrackPoint;
extern s16 g_RaceCueDelay;
extern s32 g_RaceCueFlags;
extern volatile s32 g_RouteSceneryFrame;
extern s32 g_RouteSceneryRotX;
extern s32 g_RouteSceneryRotZ;
extern s32 g_RouteSceneryX;
extern s16 g_ShuttlePathDwellMax[];

void InterpolateTrackPoint(s32 arg0, s32 *out, s32 weight);

#endif
