#include "common.h"
#include "game/track.h"
#include "psyq/gte.h"
#include "game/render.h"
#include "game/car.h"
#include "game/scratchpad.h"
#include "psyq/gpu.h"

typedef struct TrackPointWindow {
    s32 x;
    s32 z;
    s16 y;
    s16 angle;
    u8 padC[2];
    s16 field_E;
    s16 field_10;
    s16 field_12;
    u8 pad14[2];
    u16 segmentLength;
} TrackPointWindow;

typedef struct TrackSurfaceCell {
    u16 field_0;
    u8 pad2[6];
    u16 field_8;
    u8 padA[0x26];
    s32 field_30;
} TrackSurfaceCell;


s32 GetTrackSurfaceHeight(TrackSurfaceCell *cell) {
    s32 index;
    s32 nextIndex;
    TrackPointWindow *base;
    TrackPointWindow *cur;
    TrackPointWindow *next;
    u32 segmentLengthRaw;
    s32 curOff;
    s32 nextOff;
    Matrix mtx;
    s16 vec[4];
    s32 out[3];
    s32 distance;
    s32 segmentLength;
    s32 segmentLengthCompare;
    s32 invDistance;
    s32 outZ;
    s32 fieldE;
    s32 y;
    s32 argX;
    s32 curX;
    s32 argZ;
    s32 curZ;
    s32 angle;

    index = FindTrackSegment(cell, cell->field_30);
    nextIndex = (index + 1) % g_TrackPointCount;

    base = (TrackPointWindow *)g_TrackPoints;

    curOff = (index * 3) << 3;
    cur = (TrackPointWindow *)(curOff + (s32)base);

    argX = cell->field_0;
    curX = (u16)cur->x;
    segmentLengthRaw = cur->segmentLength;
    vec[0] = argX - curX;

    argZ = cell->field_8;
    curZ = (u16)cur->z;
    vec[1] = 0;
    vec[2] = argZ - curZ;

    angle = cur->angle;
    nextOff = (nextIndex * 3) << 3;
    next = (TrackPointWindow *)(nextOff + (s32)base);
    BuildRotMatrixY(&mtx, (0x1000 - angle) & 0xFFF);

    ApplyMatrix((s32 *)&mtx, (s32 *)vec, out);

    segmentLengthCompare = (s16)segmentLengthRaw;
    distance = out[0];
    outZ = out[2];
    if (segmentLengthCompare < distance) {
        distance = segmentLengthCompare;
    } else if (distance < 0) {
        distance = 0;
    }

    segmentLength = (s16)segmentLengthRaw;
    invDistance = segmentLength - distance;
    fieldE = ((next->field_E * distance) + (cur->field_E * invDistance)) / segmentLength;
    y = ((next->y * distance) + (cur->y * invDistance)) / segmentLength;

    return y + (((s16)fieldE * outZ) >> 7);
}

/*
 * Rebuilds a car's position and orientation relative to its current track
 * segment, including the curved-segment path kept in the PS1 scratchpad.
 */
void ResetCarTrackState(GameCarRuntime *car) {
    s32 headingAngle;
    s32 secondResult;
    s16 trackWidth;
    s16 trackWidthCopy;
    s32 pointsBase;
    s32 carToCenterX;
    s32 pointToCenterX;
    s16 arcSpan;
    s16 segLenA;
    s16 segLenB;
    s16 segLenC;
    s16 segLenD;
    s32 carToCenterZ;
    s32 pointToCenterZ;
    s32 arcAngle;
    s16 segLenE;
    s32 centerZ;
    s32 nextPointIndex;
    s32 edgeHeight;
    s32 cosCarAngle;
    s32 cosPointAngle;
    s32 cosNextAngle;
    s32 cosHeading;
    s32 sinHeading;
    s32 nextCamber;
    s32 arcCenterZ;
    s32 sweptAngle;
    s32 arcIndex;
    s32 arcCenterX;
    s32 centerX;
    s32 swept;
    s32 trackPointIndex;
    s32 useProgress;
    s32 arcLateral;
    s32 lateralProduct;
    s32 pointHeading;
    s32 lateralOffset;
    s32 alongSegment;
    s32 carRadius;
    s32 pointRadius;
    s32 nextRadius;
    s32 rotated;
    s16 curveMode;
    u16 segmentLength;
    GameTrackPoint *point;
    GameTrackPoint *nextPoint;
    GameTrackArcCenter *arcCenter;
    CarTrackScratch *spad;

    spad = CAR_TRACK_SCRATCH;
    spad->field_3C = 0;
    trackPointIndex = car->trackPointIndex;
    nextPointIndex = (trackPointIndex + 1) % *(s32 *)0x8009E6A8;
    pointsBase = *(s32 *)0x8009E688;
    point = (GameTrackPoint *)((trackPointIndex * 0x18) + pointsBase);
    segmentLength = point->segmentLength;
    spad->segmentLength = segmentLength;
    nextPoint = (GameTrackPoint *)((nextPointIndex * 0x18) + pointsBase);
    if ((s32)((u32)segmentLength << 0x10) <= 0) {
        spad->segmentLength = 1U;
    }
    spad->heading = (u16)point->angle;
    arcIndex = (s32)((u32)point->arcRef << 0x10) >> 0x14;
    spad->arcIndex = (s16)arcIndex;
    curveMode = point->arcRef & 3;
    spad->curveMode = curveMode;
    if (curveMode != 0) {
        arcCenter = (GameTrackArcCenter *)((arcIndex * 0xC) + *(s32 *)0x8019C7D0);
        arcCenterX = arcCenter->x;
        spad->arcCenterX = arcCenterX;
        arcCenterZ = arcCenter->z;
        spad->arcCenterZ = arcCenterZ;
        carToCenterX = car->x - arcCenterX;
        spad->carToCenterX = carToCenterX;
        carToCenterZ = car->z - arcCenterZ;
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
        carRadius = (cosCarAngle * spad->carToCenterX) +
                 (rsin(spad->sweptAngle) * spad->carToCenterZ);
        if (carRadius < 0) {
            carRadius += 0xFFF;
        }
        spad->carRadius.value = carRadius >> 0xC;
        cosPointAngle = rcos(spad->pointAngle);
        pointRadius = (cosPointAngle * spad->pointToCenterX) +
                   (rsin(spad->pointAngle) * spad->pointToCenterZ);
        if (pointRadius < 0) {
            pointRadius += 0xFFF;
        }
        spad->pointRadius.value = pointRadius >> 0xC;
        cosNextAngle = rcos(spad->nextPointAngle);
        nextRadius = (cosNextAngle * spad->nextPointToCenterX) +
                   (rsin(spad->nextPointAngle) * spad->nextPointToCenterZ);
        if (nextRadius < 0) {
            nextRadius += 0xFFF;
        }
        spad->nextPointRadius.value = nextRadius >> 0xC;
        spad->arcSpan = GetAngleDistance(spad->pointAngle, spad->nextPointAngle);
        if (spad->arcSpan <= 0) {
            spad->arcSpan = 1;
        }
        sweptAngle =
            GetAngleDistance(spad->pointAngle, spad->sweptAngle);
        arcAngle = spad->arcSpan;
        spad->sweptAngle = sweptAngle;
        spad->pointRadius.value =
            (s32)(((s16)sweptAngle * spad->pointRadius.value) +
                  ((arcAngle - (s16)sweptAngle) * spad->nextPointRadius.value)) /
            arcAngle;
        arcLateral = (s16)(spad->carRadius.half.low - spad->pointRadius.half.low);
        if (spad->curveMode == 2) {
            arcLateral = 0 - arcLateral;
        }
        spad->arcLateral = arcLateral;
        {
            headingAngle = nextPoint->angle;
            pointHeading = point->angle;
            if ((headingAngle - pointHeading) >= 0x801) {
                swept = spad->sweptAngle;
                arcSpan = spad->arcSpan;
                spad->heading =
                    (s16)((((headingAngle - 0x1000) * swept) +
                           (pointHeading * (arcSpan - swept))) /
                          arcSpan);
            } else if ((pointHeading - headingAngle) >= 0x801) {
                swept = spad->sweptAngle;
                arcSpan = spad->arcSpan;
                spad->heading =
                    (s16)(((headingAngle * swept) +
                           ((pointHeading - 0x1000) * (arcSpan - swept))) /
                          arcSpan);
            } else {
                swept = spad->sweptAngle;
                arcSpan = spad->arcSpan;
                spad->heading =
                    (s16)(((headingAngle * swept) +
                           (pointHeading * (arcSpan - swept))) /
                          arcSpan);
            }
        }
    }

    spad->offsetX = (u16)(((u16)car->x - (u16)point->x) * 4);
    headingAngle = spad->heading;
    spad->offsetZ = (s16)(((u16)car->z - (u16)point->z) * 4);
    spad->field_62 = 0;
    cosHeading = rcos(headingAngle);
    rotated = (cosHeading * (s16)spad->offsetX) +
             (rsin(spad->heading) * spad->offsetZ);
    if (rotated < 0) {
        rotated += 0xFFF;
    }
    alongSegment = rotated >> 0xE;

    /*
     * The non-clamping path does not consume the lateral component. GCC 2.6.3
     * removes its mflo/add/round/shift but leaves the two HI/LO-setting mults.
     */
    sinHeading = rsin(spad->heading);
    rotated = ((0 - sinHeading) * (s16)spad->offsetX) +
             (rcos(spad->heading) * spad->offsetZ);
    if (rotated < 0) {
        rotated += 0xFFF;
    }
    lateralOffset = rotated >> 0xE;

    if ((s16)spad->segmentLength < alongSegment) {
        alongSegment = (s16)spad->segmentLength;
    } else if (alongSegment < 0) {
        alongSegment = 0;
    }
    segLenA = (s16)spad->segmentLength;
    edgeHeight = ((nextPoint->field_12 * alongSegment) +
               (point->field_12 * (segLenA - alongSegment))) /
              segLenA;
    spad->field_88 = (s16)edgeHeight;
    useProgress = *(s32 *)0x801E408C;
    segLenB = (s16)spad->segmentLength;
    {
        s32 widthSum;
        s32 remainingLength;

        widthSum = nextPoint->field_10 * alongSegment;
        remainingLength = segLenB - alongSegment;
        widthSum += point->field_10 * remainingLength;
        spad->field_8A = (s16)(widthSum / segLenB);
    }
    {
        u32 outputProgress;

        if (useProgress != 0) {
            outputProgress = alongSegment;
        } else {
            outputProgress = (s16)spad->segmentLength - alongSegment;
        }
        car->field_6C = outputProgress;
    }
    segLenC = (s16)spad->segmentLength;
    spad->field_8E =
        (s16)(((nextPoint->field_E * alongSegment) +
               (point->field_E * (segLenC - alongSegment))) /
              segLenC);
    {
        s16 angle;

        angle = (u16)car->field_24;
        angle -= 0xC00;
        spad->field_8C = angle + (u16)spad->heading;
    }
    segLenD = (s16)spad->segmentLength;
    spad->field_92 =
        (s16)(((nextPoint->field_C * alongSegment) +
               (point->field_C * (segLenD - alongSegment))) /
              segLenD);
    trackWidth = (u16)spad->field_8A + (u16)spad->field_88;
    spad->field_86 = trackWidth;
    nextCamber = Atan2(
        (s32)trackWidth,
        (s32)(nextPoint->field_E * trackWidth) >> 7);
    trackWidthCopy = spad->field_86;
    secondResult = Atan2(
        (s32)trackWidthCopy,
        (s32)(point->field_E * trackWidthCopy) >> 7);
    segLenE = (s16)spad->segmentLength;
    spad->field_94 =
        (s16)(((nextCamber * alongSegment) +
               (secondResult * (segLenE - alongSegment))) /
              segLenE);
    spad->field_38 = rcos(spad->field_8C);
    {
        s32 firstProduct;
        s32 sinValue;
        s32 secondProduct;

        sinValue = rsin(spad->field_8C);
        spad->field_34 = sinValue;
        firstProduct = spad->field_92 * spad->field_38;
        if (firstProduct < 0) {
            firstProduct += 0xFFF;
        }
        firstProduct >>= 0xC;
        secondProduct = spad->field_94 * sinValue;
        if (secondProduct < 0) {
            secondProduct += 0xFFF;
        }
        car->field_50 = firstProduct + (secondProduct >> 0xC);
    }
    {
        s32 firstComponent;

        {
            s32 firstProduct;

            firstProduct =
                (0 - spad->field_38) * spad->field_94;
            if (firstProduct < 0) {
                firstProduct += 0xFFF;
            }
            lateralProduct = spad->field_92 * spad->field_34;
            firstComponent = firstProduct >> 0xC;
        }
        if (lateralProduct < 0) {
            lateralProduct += 0xFFF;
        }
        {
            s32 trackLength;
            s32 progress;
            s32 combinedComponent;

            trackLength = g_TrackLength;
            progress =
                (car->field_68 + car->field_6C) % trackLength;
            combinedComponent = firstComponent + (lateralProduct >> 0xC);
            car->field_58 = combinedComponent;
            car->field_54 = car->field_24;
            car->field_B4 = spad->heading;
            car->previousTrackProgress = car->trackProgress;
            car->trackProgress = progress;
            if (progress < 0) {
                s32 adjustedProgress;

                adjustedProgress = progress + trackLength;
                car->trackProgress = adjustedProgress;
            }
        }
    }
    {
        s32 finalAngle;

        if (*(s32 *)0x801E408C != 0) {
            finalAngle = g_TrackLength - car->trackProgress;
            car->field_78 = (s16)(finalAngle >> 8);
        } else {
            finalAngle = car->trackProgress;
            car->field_78 = (s16)(finalAngle >> 8);
        }
    }
}
