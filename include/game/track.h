#ifndef GAME_TRACK_H
#define GAME_TRACK_H

#include "common.h"

#include "game/vector.h"

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
    u16 segmentLength;
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

typedef struct ShuttlePath {
    Vec4 endpoint[2];
} ShuttlePath;

extern ShuttlePath g_ShuttlePathPoints[];

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

/* The two shuttle instances. Instance 1 used to carry eight split symbols of
 * its own, g_Shuttle1DwellCounter..g_Shuttle1AngleZ; they were this array's
 * second element all along and are addressed as g_ShuttleScenery[1] now. */
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

extern s32 g_CameraCarSeedYaw;
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
extern s32 g_PlayerSpeed;
extern s16 g_RaceCueDelay;
extern s32 g_RaceCueFlags;
extern volatile s32 g_RouteSceneryFrame;
extern s32 g_RouteSceneryRotX;
extern s32 g_RouteSceneryRotZ;
/*
 * The route prop's position: the four words g_RouteSceneryX/Y/Z/W at
 * 0x801E4340.  Both seeders assign the whole thing at once out of the course
 * record -- `*(Vec4 *)&g_RouteSceneryX = *(Vec4 *)(src + 0x10)` -- which is
 * what puts the address in a register with the four stores hanging off it,
 * and DrawRouteScenery hands the same address to SetGteObjectMatrix.
 *
 * The four names stay.  Declared as one Vec4 the components would be marked
 * as living in an aggregate, and gcc 2.6.3 keeps a repeated in-aggregate
 * address in a register instead of re-materialising the symbol; retail's
 * UpdateRouteScenery re-materialises it for each of the three `+=` in its
 * tail, so its component accesses were not member accesses.
 */
extern s32 g_RouteSceneryX;
extern s16 g_ShuttlePathDwellMax[];

void InterpolateTrackPoint(s32 pointIndex, s32* out, s32 weight);

/* Declared identically by 113 translation units before this
 * header carried them. */

extern s32 g_ChaseYawPrev;
extern volatile s16 g_RouteSceneryArmed;
extern s16 g_AnimScenery2Frame;
extern s32 g_AnimScenery2Tint;
extern s16 g_AnimScenery2Variant;
extern s16 g_AnimSceneryFrame;
extern s16 g_AnimSceneryPitch[];
extern s16 g_AnimSceneryRacePosition;
extern s32 g_AnimSceneryTint;
extern s16 g_AnimSceneryVariant;
extern s32 g_CamPathAngle[4];
extern s32 g_CamPathAngleDelta[4];
extern s32 g_CamPathAngleStart[4];
extern s32 g_CamPathFrame;
extern s32 g_CamPathNode;
extern s32 g_CamPathOffset[3];
extern s32 g_CamPathOffsetDelta[3];
extern s32 g_CamPathOffsetStart[3];
extern s32 g_CameraCarSpeedRamp;
extern s32 g_CameraCarTrackPoint;
extern u8 g_CameraModePrev;
extern s32 g_CameraNodeIndex;
extern s8 g_CellScanOffsetX[];
extern s8 g_CellScanOffsetY[];
extern s32 g_ChaseCameraPreset;
extern s32 g_ChaseCarSpeed;
extern s32 g_ChaseTargetYaw;
extern s32 g_ChaseYaw;
extern s32 g_ChaseYawDamping;
extern s32 g_ChaseYawLag;
extern s32 g_ChaseYawRampNeg;
extern s32 g_ChaseYawRampPos;
extern s32 g_ChaseYawStep;
extern s32 g_ChaseYawStepLimit;
extern u8 g_EnvColor1B;
extern u8 g_EnvColor1G;
extern u8 g_EnvColor2B;
extern u8 g_EnvColor2G;
extern u8 g_EnvColor3B;
extern u8 g_EnvColor3G;
extern u8 g_EnvColor3R;
extern u8 g_EnvColor4B;
extern u8 g_EnvColor4G;
extern u8 g_EnvColor5B;
extern u8 g_EnvColor5G;
extern u8 g_EnvColor6B;
extern u8 g_EnvColor6G;
extern u8 g_EnvColor6R;
extern u8 g_EnvColor7B;
extern u8 g_EnvColor7G;
extern u8 g_EnvColor8B;
extern u8 g_EnvColor8G;
extern u8 g_EnvColor8R;
extern s16 g_EnvLerpFrame;
extern u8 g_EnvScriptEnabled;
typedef struct SceneryMotionKeyframe {
    s16 rotationX;
    s16 rotationY;
    s16 rotationZ;
    s16 duration;
    s16 speed;
    s16 reserved;
} SceneryMotionKeyframe;

typedef struct SceneryMotionStart {
    Vec4 position;
    s32 reserved[4];
} SceneryMotionStart;

typedef struct SceneryMotionData {
    s16 triggerSection[2][2];
    s16 firstKeyframe[2][2];
    SceneryMotionStart start[2];
    SceneryMotionKeyframe keyframes[1];
} SceneryMotionData;

typedef struct FlybySceneryState {
    s32 timer;
    s32 soundEnabled;
    s32 keyframeTime;
    s16 lap;
    s16 keyframeIndex;
    Vec4 position;
    s32 rotationX;
    s32 rotationY;
    s32 rotationZ;
    s32 reserved2C;
    s32 volume;
} FlybySceneryState;

extern SceneryMotionKeyframe *g_FlybySceneryKeyframe;
extern s32 g_FogNear;
extern s16 g_FreeCameraAngleOffset[];
extern s32 g_HighClassSceneryYaw;
extern s32 g_OrbitCameraDistance;
extern s32 g_OrbitCameraYaw;
extern s16 g_PathSceneryHalfDelta[3];
extern s16 g_PathSceneryPosIndex;
typedef struct PathSceneryPositionKey {
    s32 x;
    s32 y;
    s32 z;
    u16 loopIndex;
    u16 reserved;
    s16 span;
    s16 rate;
} PathSceneryPositionKey;

typedef struct PathSceneryRotationKey {
    s16 x;
    s16 y;
    s16 z;
    u16 loopIndex;
    s16 span;
    s16 rate;
} PathSceneryRotationKey;

typedef union PathSceneryKey {
    PathSceneryPositionKey position;
    PathSceneryRotationKey rotation;
} PathSceneryKey;

extern u8 *g_PathSceneryPosKeys;
typedef struct PathSceneryCursor {
    s16 phase;
    s16 otherPhase;
    u16 span;
    u16 otherSpan;
    u16 rate;
    u16 otherRate;
    s16 index;
    s16 otherIndex;
} PathSceneryCursor;
extern u16 g_PathSceneryPosPhase;
extern u16 g_PathSceneryPosRate;
extern s16 g_PathSceneryPosSpan;
extern u16 g_PathSceneryRotFrameU;
extern s16 g_PathSceneryRotHalfDelta[3];
extern s16 g_PathSceneryRotIndex;
extern u8 *g_PathSceneryRotKeys;
extern u16 g_PathSceneryRotPhase;
extern u16 g_PathSceneryRotRate;
extern s16 g_PathSceneryRotSpan;
extern s16 g_PathSceneryRotY;
extern s16 g_PathSceneryRotZ;
extern s32 g_PathSceneryVolume;
extern s32 g_PathSceneryY;
extern s32 g_PathSceneryZ;
extern s32 g_PlayerField3C;
extern s32 g_PlayerSegmentWeight;
extern s32 g_RouteSceneryY;
extern s32 g_RouteSceneryZ;
extern s32 g_ShuttlePath2Points[];
extern s16 g_ShuttlePathTravelMax[];
extern s16 g_SkyTileMap[][16];
extern u16 g_SpinningSceneryAngle[];
extern u8 g_SpinningSceneryPos[];
extern u16 g_SpinningSceneryRate[];
extern u8 g_SpinningSceneryYaw[];
extern s32 g_StartGridSceneryAngle[];
extern s32 g_StaticSceneryYaw;

s32 BlendAngle(s32 angleA, s32 angleB, s32 weight);
extern s32 FindNearestTrackCamera();
void LerpEnvColor(u8* from, u8* to, u8* out, s32 blend);
void LoadEnvironmentCue();
void UpdateTrackEventSound(s16 trackSection);
void UpdateFreeLookCamera(void *car, s32 updateMotion);

/* Declared identically by 5 translation units before this
 * header carried them. */

extern Vec4 g_AnimSceneryPos[];
extern SVec g_ShuttlePathAngles[];
extern Vec4 g_StartGridSceneryPos[];
extern Vec4 g_StaticSceneryPos;

/* The two tables InstallTerrainCellData splits out of sub-block 7: the
 * 32x32 cell grid (clut index in the low 10 bits) and the per-cell
 * visibility rows read by GetCellVisibility. */
extern u16 *g_TerrainCellGrid;
extern u8 *g_CellVisibilityTable;

/*
 * Their byte sizes, which is all InstallTerrainCellData needs them for - it
 * steps the sub-block pointer past each in turn. Both fall straight out of how
 * track/visible_cells.c indexes them, and both are fixed, not per-course:
 *
 *   grid       g_TerrainCellGrid[y * 32 + x], a u16 per cell
 *              -> 32 * 32 * 2 = 0x800
 *   visibility *(u32 *)(base + y * 0x80 + x * 4), bit = region id
 *              -> a u32 per cell, 32 * 32 * 4 = 0x1000
 *
 * The row stride the code uses is the proof of the second: `cellZ << 7` is
 * 32 u32 entries per row, and 32 such rows are 0x1000. It also caps region ids
 * at 32, even though the grid word has room for 64 in its top six bits.
 */
#define TERRAIN_CELL_GRID_SIZE      0x800
#define CELL_VISIBILITY_TABLE_SIZE  0x1000

#endif
