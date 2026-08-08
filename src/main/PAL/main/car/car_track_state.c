#include "common.h"
#include "game/car.h"
#include "game/player_car_internal.h"
#include "game/track.h"
#include "psyq/gte.h"
#include "game/render.h"
#include "game/vector.h"

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

/*
 * Track-segment / route-sprite geometry builder. Interpolates between the
 * GameTrackPoint at `trackPointIndex` (*(GameTrackPoint*)0x8009E688 + i*0x18)
 * and its successor: computes route angles/heights via atan2 (Atan2)
 * and rsin/rcos, builds the collision-boundary
 * offset, and writes the interpolated position/angle/height into the render
 * object `obj`. The scratchpad struct at 0x1F80011C ("spad") is the GTE
 * per-primitive transform scratch. `clampPair` supplies the s16 margin values
 * (offsets 0/2/4/6). Raw FIELD(base,type,offset) accesses preserve the match.
 * Returns the boundary/skid response code.
 */
s32 UpdateCarTrackState(void *obj, s32 trackPointIndex, void *clampPair) {
    s32 headingAngle;
    s32 secondResult;
    s16 segLenE;
    s16 trackWidth;
    s16 trackWidthCopy;
    s16 arcSpan;
    s16 segLenA;
    s16 segLenB;
    s16 segLenC;
    s16 segLenD;
    s32 arcAngle;
    s16 segLenF;
    s16 curveMode;
    s32 sweptAngle;
    s32 swept;
    s32 arcLateral;
    s32 pointHeading;
    s32 pointsBase;
    s32 carToCenterX;
    s32 pointToCenterX;
    s32 carToCenterZ;
    s32 pointToCenterZ;
    s32 trackLength;
    s32 centerZ;
    s32 forwardComponent;
    s32 lapProgress;
    s32 nextPointIndex;
    s32 edgeHeight;
    s32 surfaceHeight;
    s32 cosCarAngle;
    s32 cosPointAngle;
    s32 cosNextAngle;
    s32 cosHeading;
    s32 sinHeading;
    s32 nextCamber;
    s32 arcCenterZ;
    s32 arcIndex;
    s32 arcCenterX;
    s32 centerX;
    s32 leftLimit;
    s32 rightLimit;
    s32 forwardProduct;
    s32 lateralProduct;
    s32 lateralOffset;
    s32 alongSegment;
    s32 carRadius;
    s32 pointRadius;
    s32 nextRadius;
    s32 rotated;
    u16 segmentLength;
    void *clampSource;
    void *point;
    void *nextPoint;
    void *arcCenter;
    void *spad;

    nextPointIndex = (trackPointIndex + 1) % *(s32 *)0x8009E6A8;
    pointsBase = *(s32 *)0x8009E688;
    spad = (void *)0x1F80011C;
    *(s32 *)0x1F800158 = 0;
    point = (void *)((trackPointIndex * 0x18) + pointsBase);
    segmentLength = FIELD(point, u16 *, 0x16);
    FIELD(spad, u16 *, 0x96) = segmentLength;
    nextPoint = (void *)((nextPointIndex * 0x18) + pointsBase);
    if ((s32) ((u32) segmentLength << 0x10) <= 0)
    {
        FIELD(spad, u16 *, 0x96) = 1U;
    }
    FIELD(spad, u16 *, 0x90) = FIELD(point, u16 *, 0xA);
    arcIndex = (s32) ((u32) FIELD(point, u16 *, 0x14) << 0x10) >> 0x14;
    FIELD(spad, s16 *, 0x7A) = (s16) arcIndex;
    curveMode = FIELD(point, u16 *, 0x14) & 3;
    FIELD(spad, s16 *, 0x78) = curveMode;
    if (curveMode != 0)
    {
        arcCenter = (void *)((arcIndex * 0xC) + *(s32 *)0x8019C7D0);
        arcCenterX = FIELD(arcCenter, s32 *, 0);
        FIELD(spad, s32 *, 0) = arcCenterX;
        arcCenterZ = FIELD(arcCenter, s32 *, 4);
        FIELD(spad, s32 *, 0x04) = arcCenterZ;
        carToCenterX = FIELD(obj, s32 *, 0) - arcCenterX;
        FIELD(spad, s32 *, 0x08) = carToCenterX;
        carToCenterZ = FIELD(obj, s32 *, 8) - arcCenterZ;
        FIELD(spad, s32 *, 0x0C) = carToCenterZ;
        FIELD(spad, s16 *, 0x7E) = Atan2(carToCenterX, carToCenterZ) & 0xFFF;
        pointToCenterX = FIELD(point, s32 *, 0);
        centerX = FIELD(spad, s32 *, 0);
        centerZ = FIELD(spad, s32 *, 0x04);
        pointToCenterX -= centerX;
        FIELD(spad, s32 *, 0x24) = pointToCenterX;
        pointToCenterZ = FIELD(point, s32 *, 4) - centerZ;
        FIELD(spad, s32 *, 0x2C) = pointToCenterZ;
        FIELD(spad, s32 *, 0x28) = FIELD(nextPoint, s32 *, 0) - centerX;
        FIELD(spad, s32 *, 0x30) = FIELD(nextPoint, s32 *, 4) - centerZ;
        FIELD(spad, s16 *, 0x80) = Atan2(pointToCenterX, pointToCenterZ) & 0xFFF;
        FIELD(spad, s16 *, 0x82) = Atan2(FIELD(spad, s32 *, 0x28), FIELD(spad, s32 *, 0x30)) & 0xFFF;
        cosCarAngle = rcos(FIELD(spad, s16 *, 0x7E));
        carRadius = (cosCarAngle * FIELD(spad, s32 *, 0x08)) + (rsin(FIELD(spad, s16 *, 0x7E)) * FIELD(spad, s32 *, 0x0C));
        if (carRadius < 0)
        {
            carRadius += 0xFFF;
        }
        FIELD(spad, s32 *, 0x10) = carRadius >> 0xC;
        cosPointAngle = rcos(FIELD(spad, s16 *, 0x80));
        pointRadius = (cosPointAngle * FIELD(spad, s32 *, 0x24)) + (rsin(FIELD(spad, s16 *, 0x80)) * FIELD(spad, s32 *, 0x2C));
        if (pointRadius < 0)
        {
            pointRadius += 0xFFF;
        }
        FIELD(spad, s32 *, 0x14) = pointRadius >> 0xC;
        cosNextAngle = rcos(FIELD(spad, s16 *, 0x82));
        nextRadius = (cosNextAngle * FIELD(spad, s32 *, 0x28)) + (rsin(FIELD(spad, s16 *, 0x82)) * FIELD(spad, s32 *, 0x30));
        if (nextRadius < 0)
        {
            nextRadius += 0xFFF;
        }
        FIELD(spad, s32 *, 0x18) = nextRadius >> 0xC;
        FIELD(spad, s16 *, 0x7C) = GetAngleDistance(FIELD(spad, s16 *, 0x80), FIELD(spad, s16 *, 0x82));
        sweptAngle = GetAngleDistance(FIELD(spad, s16 *, 0x80), FIELD(spad, s16 *, 0x7E));
        arcAngle = FIELD(spad, s16 *, 0x7C);
        FIELD(spad, s16 *, 0x7E) = sweptAngle;
        {
            s32 interpolated;

            if (arcAngle <= 0)
            {
                interpolated = FIELD(spad, s32 *, 0x14);
                FIELD(spad, s16 *, 0x7C) = 1;
            }
            else
            {
                interpolated = (s32) ((s32) (((s16) sweptAngle * FIELD(spad, s32 *, 0x14)) + ((arcAngle - (s16) sweptAngle) * FIELD(spad, s32 *, 0x18))) / arcAngle);
            }
            *(s32 *)0x1F800130 = interpolated;
        }
        arcLateral = (s16) (FIELD(spad, u16 *, 0x10) - FIELD(spad, u16 *, 0x14));
        if (FIELD(spad, s16 *, 0x78) == 2)
        {
            arcLateral = 0 - arcLateral;
        }
        FIELD(spad, s16 *, 0x84) = arcLateral;
        {
            headingAngle = FIELD(nextPoint, s16 *, 0xA);
            pointHeading = FIELD(point, s16 *, 0xA);
            if ((headingAngle - pointHeading) >= 0x801)
            {
                swept = FIELD(spad, s16 *, 0x7E);
                arcSpan = FIELD(spad, s16 *, 0x7C);
                FIELD(spad, s16 *, 0x90) = (s16) ((s32) (((headingAngle - 0x1000) * swept) + (pointHeading * (arcSpan - swept))) / arcSpan);
            }
            else if ((pointHeading - headingAngle) >= 0x801)
            {
                swept = FIELD(spad, s16 *, 0x7E);
                arcSpan = FIELD(spad, s16 *, 0x7C);
                FIELD(spad, s16 *, 0x90) = (s16) ((s32) ((headingAngle * swept) + ((pointHeading - 0x1000) * (arcSpan - swept))) / arcSpan);
            }
            else
            {
                swept = FIELD(spad, s16 *, 0x7E);
                arcSpan = FIELD(spad, s16 *, 0x7C);
                FIELD(spad, s16 *, 0x90) = (s16) ((s32) ((headingAngle * swept) + (pointHeading * (arcSpan - swept))) / arcSpan);
            }
        }
    }

    FIELD(spad, u16 *, 0x60) = (u16) (((u16) FIELD(obj, s32 *, 0) - (u16) FIELD(point, s32 *, 0)) * 4);
    headingAngle = FIELD(spad, s16 *, 0x90);
    FIELD(spad, s16 *, 0x64) = (s16) (((u16) FIELD(obj, s32 *, 8) - (u16) FIELD(point, s32 *, 4)) * 4);
    FIELD(spad, s16 *, 0x62) = 0;
    cosHeading = rcos(headingAngle);
    rotated = (cosHeading * (s16) FIELD(spad, u16 *, 0x60)) + (rsin(FIELD(spad, s16 *, 0x90)) * FIELD(spad, s16 *, 0x64));
    if (rotated < 0)
    {
        rotated += 0xFFF;
    }
    alongSegment = rotated >> 0xE;
    sinHeading = rsin(FIELD(spad, s16 *, 0x90));
    rotated = ((0 - sinHeading) * (s16) FIELD(spad, u16 *, 0x60)) + (rcos(FIELD(spad, s16 *, 0x90)) * FIELD(spad, s16 *, 0x64));
    if (rotated < 0)
    {
        rotated += 0xFFF;
    }
    lateralOffset = rotated >> 0xE;
    if (FIELD(spad, s16 *, 0x78) != 0)
    {
        lateralOffset = (s32) FIELD(spad, s16 *, 0x84);
    }
    segLenA = FIELD(spad, s16 *, 0x96);
    FIELD(spad, s16 *, 0x8A) = (s16) ((s32) ((FIELD(nextPoint, s16 *, 0x10) * alongSegment) + (FIELD(point, s16 *, 0x10) * (segLenA - alongSegment))) / segLenA);
    segLenB = FIELD(spad, s16 *, 0x96);
    edgeHeight = (s32) ((FIELD(nextPoint, s16 *, 0x12) * alongSegment) + (FIELD(point, s16 *, 0x12) * (segLenB - alongSegment))) / segLenB;
    FIELD(spad, s16 *, 0x88) = (s16) edgeHeight;
    leftLimit = FIELD(spad, s16 *, 0x8A) + FIELD(clampPair, s16 *, 2);
    clampSource = spad + 0x40;
    if (lateralOffset < (0 - leftLimit))
    {
        lateralOffset += leftLimit;
        FIELD(spad, u16 *, 0x60) = 0U;
        FIELD(spad, s16 *, 0x62) = 0;
        FIELD(spad, s16 *, 0x64) = lateralOffset;
        BuildRotMatrixY(clampSource, FIELD(spad, s16 *, 0x90));
        ApplyMatrix(clampSource, (void *)((u8 *)spad + 0x60), (void *)((u8 *)spad + 0x68));
        if (obj == &g_PlayerCar)
        {
            SetCarKnockback(obj, FIELD(spad, s32 *, 0x68), FIELD(spad, s32 *, 0x70), FIELD(clampPair, s16 *, 6));
        }
        FIELD(obj, s32 *, 0) = (s32) (FIELD(obj, s32 *, 0) - FIELD(spad, s32 *, 0x68));
        FIELD(obj, s32 *, 8) = (s32) (FIELD(obj, s32 *, 8) - FIELD(spad, s32 *, 0x70));
        lateralOffset = -FIELD(spad, s16 *, 0x8A) - FIELD(clampPair, s16 *, 2);
        FIELD(spad, s32 *, 0x3C) = FIELD(clampPair, s16 *, 6);
    }
    else
    {
    rightLimit = (s16) edgeHeight - FIELD(clampPair, s16 *, 0);
    if (rightLimit < lateralOffset)
    {
        lateralOffset -= rightLimit;
        FIELD(spad, u16 *, 0x60) = 0U;
        FIELD(spad, s16 *, 0x62) = 0;
        FIELD(spad, s16 *, 0x64) = lateralOffset;
        BuildRotMatrixY(clampSource, FIELD(spad, s16 *, 0x90));
        ApplyMatrix(clampSource, (void *)((u8 *)spad + 0x60), (void *)((u8 *)spad + 0x68));
        if (obj == &g_PlayerCar)
        {
            SetCarKnockback(obj, FIELD(spad, s32 *, 0x68), FIELD(spad, s32 *, 0x70), FIELD(clampPair, s16 *, 4));
        }
        FIELD(obj, s32 *, 0) = (s32) (FIELD(obj, s32 *, 0) - FIELD(spad, s32 *, 0x68));
        FIELD(obj, s32 *, 8) = (s32) (FIELD(obj, s32 *, 8) - FIELD(spad, s32 *, 0x70));
        lateralOffset = FIELD(spad, s16 *, 0x88) - FIELD(clampPair, s16 *, 0);
        FIELD(spad, s32 *, 0x3C) = (s32) FIELD(clampPair, s16 *, 4);
    }
    }
    if (FIELD(spad, s16 *, 0x96) < alongSegment)
    {
        alongSegment = FIELD(spad, s16 *, 0x96);
    }
    else if (alongSegment < 0)
    {
        alongSegment = 0;
    }
    FIELD(obj, s32 *, 0x38) = (s32) ((s32) (alongSegment << 0xA) / (s16) FIELD(spad, s16 *, 0x96));
    if (lateralOffset < 0)
    {
        FIELD(obj, s32 *, 0x3C) = (lateralOffset * 0x400) / FIELD(spad, s16 *, 0x8A);
    }
    else
    {
        FIELD(obj, s32 *, 0x3C) = (lateralOffset * 0x400) / FIELD(spad, s16 *, 0x88);
    }
    {
        u32 outputProgress;
        s32 useProgress;

        useProgress = *(s32 *)0x801E408C;
        FIELD(obj, s32 *, 0x34) = lateralOffset;
        if (useProgress != 0)
        {
            outputProgress = alongSegment;
        }
        else
        {
            outputProgress = FIELD(spad, s16 *, 0x96) - alongSegment;
        }
        FIELD(obj, s32 *, 0x6C) = outputProgress;
    }
    segLenC = FIELD(spad, s16 *, 0x96);
    FIELD(spad, s16 *, 0x8E) = (s16) ((s32) ((FIELD(nextPoint, s16 *, 0xE) * alongSegment) + (FIELD(point, s16 *, 0xE) * (segLenC - alongSegment))) / segLenC);
    segLenD = FIELD(spad, s16 *, 0x96);
    surfaceHeight = (s32) ((FIELD(nextPoint, s16 *, 8) * alongSegment) + (FIELD(point, s16 *, 8) * (segLenD - alongSegment))) / segLenD;
    FIELD(obj, s32 *, 4) = surfaceHeight;
    FIELD(obj, s32 *, 4) = (s32) (((s32) (FIELD(spad, s16 *, 0x8E) * lateralOffset) >> 7) + surfaceHeight);
    {
        s16 angle;

        angle = FIELD(obj, u16 *, 0x24);
        angle -= 0xC00;
        FIELD(spad, s16 *, 0x8C) = angle + FIELD(spad, u16 *, 0x90);
    }
    segLenE = FIELD(spad, s16 *, 0x96);
    FIELD(spad, s16 *, 0x92) = (s16) ((s32) ((FIELD(nextPoint, s16 *, 0xC) * alongSegment) + (FIELD(point, s16 *, 0xC) * (segLenE - alongSegment))) / segLenE);
    trackWidth = (u16) FIELD(spad, s16 *, 0x88) + (u16) FIELD(spad, s16 *, 0x8A);
    FIELD(spad, s16 *, 0x86) = trackWidth;
    nextCamber = Atan2((s32) trackWidth, (s32) (FIELD(nextPoint, s16 *, 0xE) * trackWidth) >> 7);
    trackWidthCopy = FIELD(spad, s16 *, 0x86);
    secondResult = Atan2((s32) trackWidthCopy, (s32) (FIELD(point, s16 *, 0xE) * trackWidthCopy) >> 7);
    segLenF = FIELD(spad, s16 *, 0x96);
    FIELD(spad, s16 *, 0x94) = (s16) ((s32) ((nextCamber * alongSegment) + (secondResult * (segLenF - alongSegment))) / segLenF);
    FIELD(spad, s32 *, 0x38) = rcos(FIELD(spad, s16 *, 0x8C));
    {
        s32 firstProduct;
        s32 sinValue;
        s32 secondProduct;

        sinValue = rsin(FIELD(spad, s16 *, 0x8C));
        FIELD(spad, s32 *, 0x34) = sinValue;
        firstProduct = FIELD(spad, s16 *, 0x92) * FIELD(spad, s32 *, 0x38);
        if (firstProduct < 0)
        {
            firstProduct += 0xFFF;
        }
        firstProduct >>= 0xC;
        secondProduct = FIELD(spad, s16 *, 0x94) * sinValue;
        if (secondProduct < 0)
        {
            secondProduct += 0xFFF;
        }
        FIELD(obj, s32 *, 0x20) = firstProduct + (secondProduct >> 0xC);
    }
    forwardProduct = (0 - FIELD(spad, s32 *, 0x38)) * FIELD(spad, s16 *, 0x94);
    if (forwardProduct < 0)
    {
        forwardProduct += 0xFFF;
    }
    lateralProduct = FIELD(spad, s16 *, 0x92) * FIELD(spad, s32 *, 0x34);
    forwardComponent = forwardProduct >> 0xC;
    if (lateralProduct < 0)
    {
        lateralProduct += 0xFFF;
    }
    trackLength = g_TrackLength;
    lapProgress = (s32) (FIELD(obj, s32 *, 0x68) + FIELD(obj, s32 *, 0x6C)) % trackLength;
    FIELD(obj, s32 *, 0x28) = (s32) (forwardComponent + (lateralProduct >> 0xC));
    FIELD(obj, s32 *, 0xB4) = (s32) FIELD(spad, s16 *, 0x90);
    FIELD(obj, s32 *, 0x74) = (s32) FIELD(obj, s32 *, 0x70);
    FIELD(obj, s32 *, 0x70) = lapProgress;
    if (lapProgress < 0)
    {
        FIELD(obj, s32 *, 0x70) = (s32) (lapProgress + trackLength);
    }
    {
        s32 finalAngle;

        if (*(s32 *)0x801E408C != 0)
        {
            finalAngle = g_TrackLength - FIELD(obj, s32 *, 0x70);
            FIELD(obj, s16 *, 0x78) = (s16) (finalAngle >> 8);
        }
        else
        {
            finalAngle = FIELD(obj, s32 *, 0x70);
            FIELD(obj, s16 *, 0x78) = (s16) (finalAngle >> 8);
        }
    }
    return FIELD(spad, s32 *, 0x3C);
}

typedef struct Car {
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

typedef struct TP {
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
 * Samples the track surface height under the car. Locates the containing
 * segment (FindTrackSegment), rotates the car position into segment-local space,
 * clamps the along-segment distance `t` to [0, segmentLength], and linearly
 * interpolates the point height `y` and slope `field_E` between the two segment
 * endpoints. Writes the resulting surface height into car->out4 (and out4 into
 * f60 while f98 is idle). The local TP/Car/SVec/LVec structs mirror
 * GameTrackPoint / the render object by raw offset to stay byte-exact.
 */
void SampleTrackSurfaceHeight(Car *car) {
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

    idx = FindTrackSegment(car, car->f30);
    p2 = (TP *)&g_TrackPoints[(idx + 1) % g_TrackPointCount];
    p1 = (TP *)&g_TrackPoints[idx];

    seg = p1->segmentLength;
    v.vx = car->x - p1->x;
    v.vz = car->z - p1->z;
    v.vy = 0;
    BuildRotMatrixY(&mtx, (0x1000 - p1->angle) & 0xFFF);
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
