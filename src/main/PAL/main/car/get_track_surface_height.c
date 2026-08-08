#include "common.h"
#include "game/track.h"
#include "psyq/gte.h"
#include "game/render.h"
#include "game/car.h"
#include "psyq/gpu.h"

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

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
    void *point;
    void *nextPoint;
    void *arcCenter;
    void *spad;

    *(s32 *)0x1F800158 = 0;
    trackPointIndex = FIELD(car, s32 *, 0x30);
    nextPointIndex = (trackPointIndex + 1) % *(s32 *)0x8009E6A8;
    pointsBase = *(s32 *)0x8009E688;
    spad = (void *)0x1F80011C;
    point = (void *)((trackPointIndex * 0x18) + pointsBase);
    segmentLength = FIELD(point, u16 *, 0x16);
    FIELD(spad, u16 *, 0x96) = segmentLength;
    nextPoint = (void *)((nextPointIndex * 0x18) + pointsBase);
    if ((s32)((u32)segmentLength << 0x10) <= 0) {
        FIELD(spad, u16 *, 0x96) = 1U;
    }
    FIELD(spad, u16 *, 0x90) = FIELD(point, u16 *, 0xA);
    arcIndex = (s32)((u32)FIELD(point, u16 *, 0x14) << 0x10) >> 0x14;
    FIELD(spad, s16 *, 0x7A) = (s16)arcIndex;
    curveMode = FIELD(point, u16 *, 0x14) & 3;
    FIELD(spad, s16 *, 0x78) = curveMode;
    if (curveMode != 0) {
        arcCenter = (void *)((arcIndex * 0xC) + *(s32 *)0x8019C7D0);
        arcCenterX = FIELD(arcCenter, s32 *, 0);
        FIELD(spad, s32 *, 0) = arcCenterX;
        arcCenterZ = FIELD(arcCenter, s32 *, 4);
        FIELD(spad, s32 *, 0x04) = arcCenterZ;
        carToCenterX = FIELD(car, s32 *, 0) - arcCenterX;
        FIELD(spad, s32 *, 0x08) = carToCenterX;
        carToCenterZ = FIELD(car, s32 *, 8) - arcCenterZ;
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
        FIELD(spad, s16 *, 0x82) =
            Atan2(FIELD(spad, s32 *, 0x28), FIELD(spad, s32 *, 0x30)) & 0xFFF;
        cosCarAngle = rcos(FIELD(spad, s16 *, 0x7E));
        carRadius = (cosCarAngle * FIELD(spad, s32 *, 0x08)) +
                 (rsin(FIELD(spad, s16 *, 0x7E)) * FIELD(spad, s32 *, 0x0C));
        if (carRadius < 0) {
            carRadius += 0xFFF;
        }
        FIELD(spad, s32 *, 0x10) = carRadius >> 0xC;
        cosPointAngle = rcos(FIELD(spad, s16 *, 0x80));
        pointRadius = (cosPointAngle * FIELD(spad, s32 *, 0x24)) +
                   (rsin(FIELD(spad, s16 *, 0x80)) * FIELD(spad, s32 *, 0x2C));
        if (pointRadius < 0) {
            pointRadius += 0xFFF;
        }
        FIELD(spad, s32 *, 0x14) = pointRadius >> 0xC;
        cosNextAngle = rcos(FIELD(spad, s16 *, 0x82));
        nextRadius = (cosNextAngle * FIELD(spad, s32 *, 0x28)) +
                   (rsin(FIELD(spad, s16 *, 0x82)) * FIELD(spad, s32 *, 0x30));
        if (nextRadius < 0) {
            nextRadius += 0xFFF;
        }
        FIELD(spad, s32 *, 0x18) = nextRadius >> 0xC;
        FIELD(spad, s16 *, 0x7C) =
            GetAngleDistance(FIELD(spad, s16 *, 0x80), FIELD(spad, s16 *, 0x82));
        if (FIELD(spad, s16 *, 0x7C) <= 0) {
            FIELD(spad, s16 *, 0x7C) = 1;
        }
        sweptAngle =
            GetAngleDistance(FIELD(spad, s16 *, 0x80), FIELD(spad, s16 *, 0x7E));
        arcAngle = FIELD(spad, s16 *, 0x7C);
        FIELD(spad, s16 *, 0x7E) = sweptAngle;
        FIELD(spad, s32 *, 0x14) =
            (s32)(((s16)sweptAngle * FIELD(spad, s32 *, 0x14)) +
                  ((arcAngle - (s16)sweptAngle) * FIELD(spad, s32 *, 0x18))) /
            arcAngle;
        arcLateral = (s16)(FIELD(spad, u16 *, 0x10) - FIELD(spad, u16 *, 0x14));
        if (FIELD(spad, s16 *, 0x78) == 2) {
            arcLateral = 0 - arcLateral;
        }
        FIELD(spad, s16 *, 0x84) = arcLateral;
        {
            headingAngle = FIELD(nextPoint, s16 *, 0xA);
            pointHeading = FIELD(point, s16 *, 0xA);
            if ((headingAngle - pointHeading) >= 0x801) {
                swept = FIELD(spad, s16 *, 0x7E);
                arcSpan = FIELD(spad, s16 *, 0x7C);
                FIELD(spad, s16 *, 0x90) =
                    (s16)((((headingAngle - 0x1000) * swept) +
                           (pointHeading * (arcSpan - swept))) /
                          arcSpan);
            } else if ((pointHeading - headingAngle) >= 0x801) {
                swept = FIELD(spad, s16 *, 0x7E);
                arcSpan = FIELD(spad, s16 *, 0x7C);
                FIELD(spad, s16 *, 0x90) =
                    (s16)(((headingAngle * swept) +
                           ((pointHeading - 0x1000) * (arcSpan - swept))) /
                          arcSpan);
            } else {
                swept = FIELD(spad, s16 *, 0x7E);
                arcSpan = FIELD(spad, s16 *, 0x7C);
                FIELD(spad, s16 *, 0x90) =
                    (s16)(((headingAngle * swept) +
                           (pointHeading * (arcSpan - swept))) /
                          arcSpan);
            }
        }
    }

    FIELD(spad, u16 *, 0x60) =
        (u16)(((u16)FIELD(car, s32 *, 0) - (u16)FIELD(point, s32 *, 0)) * 4);
    headingAngle = FIELD(spad, s16 *, 0x90);
    FIELD(spad, s16 *, 0x64) =
        (s16)(((u16)FIELD(car, s32 *, 8) - (u16)FIELD(point, s32 *, 4)) * 4);
    FIELD(spad, s16 *, 0x62) = 0;
    cosHeading = rcos(headingAngle);
    rotated = (cosHeading * (s16)FIELD(spad, u16 *, 0x60)) +
             (rsin(FIELD(spad, s16 *, 0x90)) * FIELD(spad, s16 *, 0x64));
    if (rotated < 0) {
        rotated += 0xFFF;
    }
    alongSegment = rotated >> 0xE;

    /*
     * The non-clamping path does not consume the lateral component. GCC 2.6.3
     * removes its mflo/add/round/shift but leaves the two HI/LO-setting mults.
     */
    sinHeading = rsin(FIELD(spad, s16 *, 0x90));
    rotated = ((0 - sinHeading) * (s16)FIELD(spad, u16 *, 0x60)) +
             (rcos(FIELD(spad, s16 *, 0x90)) * FIELD(spad, s16 *, 0x64));
    if (rotated < 0) {
        rotated += 0xFFF;
    }
    lateralOffset = rotated >> 0xE;

    if (FIELD(spad, s16 *, 0x96) < alongSegment) {
        alongSegment = FIELD(spad, s16 *, 0x96);
    } else if (alongSegment < 0) {
        alongSegment = 0;
    }
    segLenA = FIELD(spad, s16 *, 0x96);
    edgeHeight = ((FIELD(nextPoint, s16 *, 0x12) * alongSegment) +
               (FIELD(point, s16 *, 0x12) * (segLenA - alongSegment))) /
              segLenA;
    FIELD(spad, s16 *, 0x88) = (s16)edgeHeight;
    useProgress = *(s32 *)0x801E408C;
    segLenB = FIELD(spad, s16 *, 0x96);
    {
        s32 widthSum;
        s32 remainingLength;

        widthSum = FIELD(nextPoint, s16 *, 0x10) * alongSegment;
        remainingLength = segLenB - alongSegment;
        widthSum += FIELD(point, s16 *, 0x10) * remainingLength;
        FIELD(spad, s16 *, 0x8A) = (s16)(widthSum / segLenB);
    }
    {
        u32 outputProgress;

        if (useProgress != 0) {
            outputProgress = alongSegment;
        } else {
            outputProgress = FIELD(spad, s16 *, 0x96) - alongSegment;
        }
        FIELD(car, s32 *, 0x6C) = outputProgress;
    }
    segLenC = FIELD(spad, s16 *, 0x96);
    FIELD(spad, s16 *, 0x8E) =
        (s16)(((FIELD(nextPoint, s16 *, 0xE) * alongSegment) +
               (FIELD(point, s16 *, 0xE) * (segLenC - alongSegment))) /
              segLenC);
    {
        s16 angle;

        angle = (u16)FIELD(car, s32 *, 0x24);
        angle -= 0xC00;
        FIELD(spad, s16 *, 0x8C) = angle + FIELD(spad, u16 *, 0x90);
    }
    segLenD = FIELD(spad, s16 *, 0x96);
    FIELD(spad, s16 *, 0x92) =
        (s16)(((FIELD(nextPoint, s16 *, 0xC) * alongSegment) +
               (FIELD(point, s16 *, 0xC) * (segLenD - alongSegment))) /
              segLenD);
    trackWidth = (u16)FIELD(spad, s16 *, 0x8A) + (u16)FIELD(spad, s16 *, 0x88);
    FIELD(spad, s16 *, 0x86) = trackWidth;
    nextCamber = Atan2(
        (s32)trackWidth,
        (s32)(FIELD(nextPoint, s16 *, 0xE) * trackWidth) >> 7);
    trackWidthCopy = FIELD(spad, s16 *, 0x86);
    secondResult = Atan2(
        (s32)trackWidthCopy,
        (s32)(FIELD(point, s16 *, 0xE) * trackWidthCopy) >> 7);
    segLenE = FIELD(spad, s16 *, 0x96);
    FIELD(spad, s16 *, 0x94) =
        (s16)(((nextCamber * alongSegment) +
               (secondResult * (segLenE - alongSegment))) /
              segLenE);
    FIELD(spad, s32 *, 0x38) = rcos(FIELD(spad, s16 *, 0x8C));
    {
        s32 firstProduct;
        s32 sinValue;
        s32 secondProduct;

        sinValue = rsin(FIELD(spad, s16 *, 0x8C));
        FIELD(spad, s32 *, 0x34) = sinValue;
        firstProduct = FIELD(spad, s16 *, 0x92) * FIELD(spad, s32 *, 0x38);
        if (firstProduct < 0) {
            firstProduct += 0xFFF;
        }
        firstProduct >>= 0xC;
        secondProduct = FIELD(spad, s16 *, 0x94) * sinValue;
        if (secondProduct < 0) {
            secondProduct += 0xFFF;
        }
        FIELD(car, s32 *, 0x50) = firstProduct + (secondProduct >> 0xC);
    }
    {
        s32 firstComponent;

        {
            s32 firstProduct;

            firstProduct =
                (0 - FIELD(spad, s32 *, 0x38)) * FIELD(spad, s16 *, 0x94);
            if (firstProduct < 0) {
                firstProduct += 0xFFF;
            }
            lateralProduct = FIELD(spad, s16 *, 0x92) * FIELD(spad, s32 *, 0x34);
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
                (FIELD(car, s32 *, 0x68) + FIELD(car, s32 *, 0x6C)) % trackLength;
            combinedComponent = firstComponent + (lateralProduct >> 0xC);
            FIELD(car, s32 *, 0x58) = combinedComponent;
            FIELD(car, s32 *, 0x54) = FIELD(car, s32 *, 0x24);
            FIELD(car, s32 *, 0xB4) = FIELD(spad, s16 *, 0x90);
            FIELD(car, s32 *, 0x74) = FIELD(car, s32 *, 0x70);
            FIELD(car, s32 *, 0x70) = progress;
            if (progress < 0) {
                s32 adjustedProgress;

                adjustedProgress = progress + trackLength;
                FIELD(car, s32 *, 0x70) = adjustedProgress;
            }
        }
    }
    {
        s32 finalAngle;

        if (*(s32 *)0x801E408C != 0) {
            finalAngle = g_TrackLength - FIELD(car, s32 *, 0x70);
            FIELD(car, s16 *, 0x78) = (s16)(finalAngle >> 8);
        } else {
            finalAngle = FIELD(car, s32 *, 0x70);
            FIELD(car, s16 *, 0x78) = (s16)(finalAngle >> 8);
        }
    }
}

extern GameSpriteDesc g_TachoNeedleSprite;

/*
 * Rebuilds the tachometer needle quad from the car spec block. The spec is
 * read through a plain `u8 *` on purpose: as GameCarSpec member reads, gcc
 * 2.6.3 stops treating the loads as aliasing the D_8019C7Dx globals this
 * function stores to and reorders the two against each other.
 *   +0x14C..0x14F  spec->needleQuad
 *   +0x138/+0x13A  spec->tachoNeedleX / Y
 *   +0x13C/+0x13E  spec->tachoFaceDX / DY
 */
void BuildTachoNeedleQuad(void) {
    u8 *data = (u8 *)g_CarSpec;
    u8 *prim0 = g_TachoNeedlePrim0;
    u8 *prim1 = g_TachoNeedlePrim1;
    GameSpriteDesc *src = &g_TachoNeedleSprite;

    g_TachoNeedleQuad[0][0] = -data[0x14F];
    g_TachoNeedleQuad[0][1] = data[0x14E];
    g_TachoNeedleQuad[1][0] = -data[0x14D];
    g_TachoNeedleQuad[1][1] = -data[0x14C];
    g_TachoNeedleQuad[2][0] = data[0x14F];
    g_TachoNeedleQuad[2][1] = data[0x14E];
    g_TachoNeedleQuad[3][0] = data[0x14D];
    g_TachoNeedleQuad[3][1] = -data[0x14C];

    src->x = *(u16 *)(data + 0x13C) + *(u16 *)(data + 0x138);
    src->y = *(u16 *)(data + 0x13E) + *(u16 *)(data + 0x13A);

    BuildSpriteFromDesc((SPRT *)prim0, src);
    BuildSpriteFromDesc((SPRT *)prim1, src);
    SetShadeTex(prim0, 0);
    SetShadeTex(prim1, 0);
    SetDrawMode((DrawPacket *)(prim0 - 0x18), 0, 1, 9, 0);
    SetDrawMode((DrawPacket *)(prim0 - 0x0C), 0, 1, 0xA, 0);
    SetDrawMode((DrawPacket *)g_TachoNeedlePrim1PageA, 0, 1, 9, 0);
    SetDrawMode((DrawPacket *)g_TachoNeedlePrim1PageB, 0, 1, 0xA, 0);
}
