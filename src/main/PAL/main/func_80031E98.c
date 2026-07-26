#include "common.h"
#include "psyq/gte.h"
#include "game/render.h"

typedef struct {
    s16 x;
    s16 y;
    s16 z;
    s16 pad;
} SVec;

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} LVec;

typedef struct {
    u16 x;         /* 0x00 */
    u16 pad2;      /* 0x02 */
    s32 out4;      /* 0x04 */
    u16 z;         /* 0x08 */
    u16 padA;      /* 0x0A */
    u8 padC[0x24]; /* 0x0C */
    s32 f30;       /* 0x30 */
    u8 pad34[0x2C];/* 0x34 */
    s32 f60;       /* 0x60 */
    u8 pad64[0x34];/* 0x64 */
    s16 f98;       /* 0x98 */
} Car;

typedef struct {
    u16 x;             /* 0x00 */
    u16 pad2;          /* 0x02 */
    u16 z;             /* 0x04 */
    u16 pad6;          /* 0x06 */
    s16 y;             /* 0x08 */
    s16 angle;         /* 0x0A */
    u8 padC[2];        /* 0x0C */
    s16 field_E;       /* 0x0E */
    u8 pad10[6];       /* 0x10 */
    u16 segmentLength; /* 0x16 */
} TP;

/*
 * Same objects as game/track.h's g_TrackPoints / g_TrackPointCount; this TU
 * views the point record through the local TP typedef (it needs 0x0E, which
 * GameTrackPoint leaves unnamed, and an unsigned segmentLength), so it carries
 * its own aliased declarations instead of including the header.
 */
extern TP *g_TrackPoints asm("D_8009E688");
extern s32 g_TrackPointCount asm("D_8009E6A8");

s32 GameFindTrackSegment(Car *car, s32 idx) asm("func_80030EB4");

/*
 * Samples the track surface height under the car. Locates the containing
 * segment (GameFindTrackSegment), rotates the car position into segment-local space,
 * clamps the along-segment distance `t` to [0, segmentLength], and linearly
 * interpolates the point height `y` and slope `field_E` between the two segment
 * endpoints. Writes the resulting surface height into car->out4 (and out4 into
 * f60 while f98 is idle). The local TP/Car/SVec/LVec structs mirror
 * GameTrackPoint / the render object by raw offset to stay byte-exact.
 */
void func_80031E98(Car *car) {
    Matrix mtx;
    SVec v;
    LVec out;
    TP *p1;
    TP *p2;
    s32 idx;
    s32 seg;
    s32 t;
    s32 oz;
    s32 diff;
    s32 e;
    s32 v8;

    idx = GameFindTrackSegment(car, car->f30);
    p2 = &g_TrackPoints[(idx + 1) % g_TrackPointCount];
    p1 = &g_TrackPoints[idx];

    seg = p1->segmentLength;
    v.x = car->x - p1->x;
    v.z = car->z - p1->z;
    v.y = 0;
    GameBuildRotMatrixY(&mtx, (0x1000 - p1->angle) & 0xFFF);
    ApplyMatrix((s32 *)&mtx, (s32 *)&v, (s32 *)&out);

    t = out.x;
    oz = out.z;
    if ((s16)seg < t) {
        t = (s16)seg;
    } else if (t < 0) {
        t = 0;
    }

    diff = (s16)seg - t;
    e = (p2->field_E * t + p1->field_E * diff) / (s16)seg;
    v8 = (p2->y * t + p1->y * diff) / (s16)seg;

    car->out4 = ((s16)e * oz >> 7) + v8;
    if (car->f98 == 0) {
        car->f60 = car->out4;
    }
}
