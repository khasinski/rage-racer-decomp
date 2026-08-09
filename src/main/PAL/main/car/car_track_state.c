#include "common.h"
#include "game/car.h"
#include "game/player_car_internal.h"
#include "game/track.h"
#include "psyq/gte.h"
#include "game/render.h"
#include "game/scratchpad.h"
#include "game/vector.h"

/*
 * Track-segment / route-sprite geometry builder. Interpolates between the
 * GameTrackPoint at `trackPointIndex` (*(GameTrackPoint*)0x8009E688 + i*0x18)
 * and its successor: computes route angles/heights via atan2 (Atan2)
 * and rsin/rcos, builds the collision-boundary
 * offset, and writes the interpolated position/angle/height into the render
 * object `obj`. The scratchpad struct at 0x1F80011C ("spad") is the GTE
 * per-primitive transform scratch. `limits` supplies the boundary margins and
 * knockback modes.
 * Returns the boundary/skid response code.
 */
s32 UpdateCarTrackState(GameCarRuntime *obj, s32 trackPointIndex, CarTrackLimits *limits) {
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
    GameTrackPoint *point;
    GameTrackPoint *nextPoint;
    GameTrackArcCenter *arcCenter;
    CarTrackScratch *spad;

    nextPointIndex = (trackPointIndex + 1) % *(s32 *)0x8009E6A8;
    pointsBase = *(s32 *)0x8009E688;
    spad = CAR_TRACK_SCRATCH;
    spad->field_3C = 0;
    point = (GameTrackPoint *)((trackPointIndex * 0x18) + pointsBase);
    segmentLength = point->segmentLength;
    spad->segmentLength = segmentLength;
    nextPoint = (GameTrackPoint *)((nextPointIndex * 0x18) + pointsBase);
    if ((s32) ((u32) segmentLength << 0x10) <= 0)
    {
        spad->segmentLength = 1U;
    }
    spad->heading = (u16)point->angle;
    arcIndex = (s32)((u32)point->arcRef << 0x10) >> 0x14;
    spad->arcIndex = (s16)arcIndex;
    curveMode = point->arcRef & 3;
    spad->curveMode = curveMode;
    if (curveMode != 0)
    {
        arcCenter = (GameTrackArcCenter *)((arcIndex * 0xC) + *(s32 *)0x8019C7D0);
        arcCenterX = arcCenter->x;
        spad->arcCenterX = arcCenterX;
        arcCenterZ = arcCenter->z;
        spad->arcCenterZ = arcCenterZ;
        carToCenterX = obj->x - arcCenterX;
        spad->carToCenterX = carToCenterX;
        carToCenterZ = obj->z - arcCenterZ;
        spad->carToCenterZ = carToCenterZ;
        spad->sweptAngle = Atan2(carToCenterX, carToCenterZ) & 0xFFF;
        pointToCenterX = point->x;
        centerX = spad->arcCenterX;
        centerZ = spad->arcCenterZ;
        pointToCenterX -= centerX;
        spad->pointToCenterX = pointToCenterX;
        pointToCenterZ = point->z - centerZ;
        spad->pointToCenterZ = pointToCenterZ;
        spad->nextPointToCenterX = nextPoint->x - centerX;
        spad->nextPointToCenterZ = nextPoint->z - centerZ;
        spad->pointAngle = Atan2(pointToCenterX, pointToCenterZ) & 0xFFF;
        spad->nextPointAngle = Atan2(spad->nextPointToCenterX, spad->nextPointToCenterZ) & 0xFFF;
        cosCarAngle = rcos(spad->sweptAngle);
        carRadius = cosCarAngle * spad->carToCenterX + rsin(spad->sweptAngle) * spad->carToCenterZ;
        if (carRadius < 0)
        {
            carRadius += 0xFFF;
        }
        spad->carRadius.value = carRadius >> 0xC;
        cosPointAngle = rcos(spad->pointAngle);
        pointRadius = cosPointAngle * spad->pointToCenterX + rsin(spad->pointAngle) * spad->pointToCenterZ;
        if (pointRadius < 0)
        {
            pointRadius += 0xFFF;
        }
        spad->pointRadius.value = pointRadius >> 0xC;
        cosNextAngle = rcos(spad->nextPointAngle);
        nextRadius = cosNextAngle * spad->nextPointToCenterX + rsin(spad->nextPointAngle) * spad->nextPointToCenterZ;
        if (nextRadius < 0)
        {
            nextRadius += 0xFFF;
        }
        spad->nextPointRadius.value = nextRadius >> 0xC;
        spad->arcSpan = GetAngleDistance(spad->pointAngle, spad->nextPointAngle);
        sweptAngle = GetAngleDistance(spad->pointAngle, spad->sweptAngle);
        arcAngle = spad->arcSpan;
        spad->sweptAngle = sweptAngle;
        {
            s32 interpolated;

            if (arcAngle <= 0)
            {
                interpolated = spad->pointRadius.value;
                spad->arcSpan = 1;
            }
            else
            {
                interpolated = (s32) ((s32) (((s16) sweptAngle * spad->pointRadius.value) + ((arcAngle - (s16) sweptAngle) * spad->nextPointRadius.value)) / arcAngle);
            }
            *(s32 *)0x1F800130 = interpolated;
        }
        arcLateral =
            (s16)(RAW(spad->carRadius.half.low) - RAW(spad->pointRadius.half.low));
        if (spad->curveMode == 2)
        {
            arcLateral = 0 - arcLateral;
        }
        spad->arcLateral = arcLateral;
        {
            headingAngle = nextPoint->angle;
            pointHeading = point->angle;
            if ((headingAngle - pointHeading) >= 0x801)
            {
                swept = spad->sweptAngle;
                arcSpan = spad->arcSpan;
                spad->heading = (s16) ((s32) (((headingAngle - 0x1000) * swept) + (pointHeading * (arcSpan - swept))) / arcSpan);
            }
            else if ((pointHeading - headingAngle) >= 0x801)
            {
                swept = spad->sweptAngle;
                arcSpan = spad->arcSpan;
                spad->heading = (s16) ((s32) ((headingAngle * swept) + ((pointHeading - 0x1000) * (arcSpan - swept))) / arcSpan);
            }
            else
            {
                swept = spad->sweptAngle;
                arcSpan = spad->arcSpan;
                spad->heading = (s16) ((s32) ((headingAngle * swept) + (pointHeading * (arcSpan - swept))) / arcSpan);
            }
        }
    }

    spad->offsetX = (u16)(((u16)obj->x - (u16)point->x) * 4);
    headingAngle = spad->heading;
    spad->offsetZ = (s16)(((u16)obj->z - (u16)point->z) * 4);
    spad->field_62 = 0;
    cosHeading = rcos(headingAngle);
    rotated = (cosHeading * (s16) spad->offsetX) + (rsin(spad->heading) * spad->offsetZ);
    if (rotated < 0)
    {
        rotated += 0xFFF;
    }
    alongSegment = rotated >> 0xE;
    sinHeading = rsin(spad->heading);
    rotated = ((0 - sinHeading) * (s16) spad->offsetX) + (rcos(spad->heading) * spad->offsetZ);
    if (rotated < 0)
    {
        rotated += 0xFFF;
    }
    lateralOffset = rotated >> 0xE;
    if (spad->curveMode != 0)
    {
        lateralOffset = (s32) spad->arcLateral;
    }
    segLenA = (s16)spad->segmentLength;
    spad->field_8A = (s16) ((s32) ((nextPoint->field_10 * alongSegment) + (point->field_10 * (segLenA - alongSegment))) / segLenA);
    segLenB = (s16)spad->segmentLength;
    edgeHeight = (s32) ((nextPoint->field_12 * alongSegment) + (point->field_12 * (segLenB - alongSegment))) / segLenB;
    spad->field_88 = (s16) edgeHeight;
    leftLimit = spad->field_8A + limits->leftInset;
    clampSource = &spad->pad40[0];
    if (lateralOffset < (0 - leftLimit))
    {
        lateralOffset += leftLimit;
        spad->offsetX = 0U;
        spad->field_62 = 0;
        spad->offsetZ = lateralOffset;
        BuildRotMatrixY(clampSource, spad->heading);
        ApplyMatrix(clampSource, &spad->offsetX, &spad->field_68);
        if (obj == (GameCarRuntime *)&g_PlayerCar)
        {
            SetCarKnockback(obj, spad->field_68, spad->field_70, limits->leftKnockbackMode);
        }
        obj->x = obj->x - spad->field_68;
        obj->z = obj->z - spad->field_70;
        lateralOffset = -spad->field_8A - limits->leftInset;
        spad->field_3C = limits->leftKnockbackMode;
    }
    else
    {
    rightLimit = (s16)edgeHeight - limits->rightInset;
    if (rightLimit < lateralOffset)
    {
        lateralOffset -= rightLimit;
        spad->offsetX = 0U;
        spad->field_62 = 0;
        spad->offsetZ = lateralOffset;
        BuildRotMatrixY(clampSource, spad->heading);
        ApplyMatrix(clampSource, &spad->offsetX, &spad->field_68);
        if (obj == (GameCarRuntime *)&g_PlayerCar)
        {
            SetCarKnockback(obj, spad->field_68, spad->field_70, limits->rightKnockbackMode);
        }
        obj->x = obj->x - spad->field_68;
        obj->z = obj->z - spad->field_70;
        lateralOffset = spad->field_88 - limits->rightInset;
        spad->field_3C = limits->rightKnockbackMode;
    }
    }
    if ((s16)spad->segmentLength < alongSegment)
    {
        alongSegment = (s16)spad->segmentLength;
    }
    else if (alongSegment < 0)
    {
        alongSegment = 0;
    }
    obj->field_38 = (s32)((s32)(alongSegment << 0xA) / (s16)spad->segmentLength);
    if (lateralOffset < 0)
    {
        obj->field_3C = (lateralOffset * 0x400) / spad->field_8A;
    }
    else
    {
        obj->field_3C = (lateralOffset * 0x400) / spad->field_88;
    }
    {
        u32 outputProgress;
        s32 useProgress;

        useProgress = *(s32 *)0x801E408C;
        obj->field_34 = lateralOffset;
        if (useProgress != 0)
        {
            outputProgress = alongSegment;
        }
        else
        {
            outputProgress = (s16)spad->segmentLength - alongSegment;
        }
        obj->field_6C = outputProgress;
    }
    segLenC = (s16)spad->segmentLength;
    spad->field_8E = (s16) ((s32) ((nextPoint->field_E * alongSegment) + (point->field_E * (segLenC - alongSegment))) / segLenC);
    segLenD = (s16)spad->segmentLength;
    surfaceHeight = (s32) ((nextPoint->y * alongSegment) + (point->y * (segLenD - alongSegment))) / segLenD;
    obj->y = surfaceHeight;
    obj->y = (s32)(((s32)(spad->field_8E * lateralOffset) >> 7) + surfaceHeight);
    {
        s16 angle;

        angle = (u16)obj->field_24;
        angle -= 0xC00;
        spad->field_8C = angle + (u16)spad->heading;
    }
    segLenE = (s16)spad->segmentLength;
    spad->field_92 = (s16) ((s32) ((nextPoint->field_C * alongSegment) + (point->field_C * (segLenE - alongSegment))) / segLenE);
    trackWidth = (u16) spad->field_88 + (u16) spad->field_8A;
    spad->field_86 = trackWidth;
    nextCamber = Atan2((s32) trackWidth, (s32) (nextPoint->field_E * trackWidth) >> 7);
    trackWidthCopy = spad->field_86;
    secondResult = Atan2((s32) trackWidthCopy, (s32) (point->field_E * trackWidthCopy) >> 7);
    segLenF = (s16)spad->segmentLength;
    spad->field_94 = (s16) ((s32) ((nextCamber * alongSegment) + (secondResult * (segLenF - alongSegment))) / segLenF);
    spad->field_38 = rcos(spad->field_8C);
    {
        s32 firstProduct;
        s32 sinValue;
        s32 secondProduct;

        sinValue = rsin(spad->field_8C);
        spad->field_34 = sinValue;
        firstProduct = spad->field_92 * spad->field_38;
        if (firstProduct < 0)
        {
            firstProduct += 0xFFF;
        }
        firstProduct >>= 0xC;
        secondProduct = spad->field_94 * sinValue;
        if (secondProduct < 0)
        {
            secondProduct += 0xFFF;
        }
        obj->field_20 = firstProduct + (secondProduct >> 0xC);
    }
    forwardProduct = (0 - spad->field_38) * spad->field_94;
    if (forwardProduct < 0)
    {
        forwardProduct += 0xFFF;
    }
    lateralProduct = spad->field_92 * spad->field_34;
    forwardComponent = forwardProduct >> 0xC;
    if (lateralProduct < 0)
    {
        lateralProduct += 0xFFF;
    }
    trackLength = g_TrackLength;
    lapProgress = (s32)(obj->field_68 + obj->field_6C) % trackLength;
    obj->field_28 = (s32)(forwardComponent + (lateralProduct >> 0xC));
    obj->field_B4 = (s32)spad->heading;
    obj->previousTrackProgress = obj->trackProgress;
    obj->trackProgress = lapProgress;
    if (lapProgress < 0)
    {
        obj->trackProgress = (s32)(lapProgress + trackLength);
    }
    {
        s32 finalAngle;

        if (*(s32 *)0x801E408C != 0)
        {
            finalAngle = g_TrackLength - obj->trackProgress;
            obj->trackSection = (s16)(finalAngle >> 8);
        }
        else
        {
            finalAngle = obj->trackProgress;
            obj->trackSection = (s16)(finalAngle >> 8);
        }
    }
    return spad->field_3C;
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
        ApplyMatrix(&mtx, &v, &out);

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
