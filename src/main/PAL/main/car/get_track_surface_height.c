#include "common.h"
#include "game/track.h"
#include "psyq/gte.h"
#include "game/render.h"
#include "game/car.h"
#include "psyq/gpu.h"
s32 rcos();
s32 rsin();

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

s32 FindTrackSegment(TrackSurfaceCell *arg0, s32 arg1);

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
    s32 temp_a0;
    s32 secondResult;
    s16 temp_a0_10;
    s16 temp_a0_11;
    s32 temp_a0_2;
    s32 temp_a0_3;
    s32 temp_a0_4;
    s16 temp_a0_5;
    s16 temp_a0_6;
    s16 temp_a0_7;
    s16 temp_a0_8;
    s16 temp_a0_9;
    s32 temp_a1;
    s32 temp_a1_2;
    s32 temp_a1_3;
    s16 temp_a1_4;
    s32 temp_a2_2;
    s32 temp_hi_2;
    s32 temp_lo;
    s32 temp_s0;
    s32 temp_s0_2;
    s32 temp_s0_3;
    s32 temp_s0_4;
    s32 temp_s0_5;
    s32 temp_s0_7;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v1_2;
    s32 temp_v1_3;
    s32 temp_v1_4;
    s32 temp_v1_5;
    s32 temp_v1_6;
    s32 trackPointIndex;
    s32 useProgress;
    s32 var_a0_2;
    s32 var_a1;
    s32 var_a1_2;
    s32 var_a2;
    s32 var_s3;
    s32 var_v0;
    s32 var_v0_2;
    s32 var_v0_3;
    s32 var_v1;
    s16 temp_v0;
    u16 temp_v1;
    void *temp_s4;
    void *temp_s6;
    void *temp_v0_2;
    void *spad;

    *(s32 *)0x1F800158 = 0;
    trackPointIndex = FIELD(car, s32 *, 0x30);
    temp_hi_2 = (trackPointIndex + 1) % *(s32 *)0x8009E6A8;
    temp_a0_2 = *(s32 *)0x8009E688;
    spad = (void *)0x1F80011C;
    temp_s4 = (void *)((trackPointIndex * 0x18) + temp_a0_2);
    temp_v1 = FIELD(temp_s4, u16 *, 0x16);
    FIELD(spad, u16 *, 0x96) = temp_v1;
    temp_s6 = (void *)((temp_hi_2 * 0x18) + temp_a0_2);
    if ((s32)((u32)temp_v1 << 0x10) <= 0) {
        FIELD(spad, u16 *, 0x96) = 1U;
    }
    FIELD(spad, u16 *, 0x90) = FIELD(temp_s4, u16 *, 0xA);
    temp_v1_2 = (s32)((u32)FIELD(temp_s4, u16 *, 0x14) << 0x10) >> 0x14;
    FIELD(spad, s16 *, 0x7A) = (s16)temp_v1_2;
    temp_v0 = FIELD(temp_s4, u16 *, 0x14) & 3;
    FIELD(spad, s16 *, 0x78) = temp_v0;
    if (temp_v0 != 0) {
        temp_v0_2 = (void *)((temp_v1_2 * 0xC) + *(s32 *)0x8019C7D0);
        temp_v1_3 = FIELD(temp_v0_2, s32 *, 0);
        FIELD(spad, s32 *, 0) = temp_v1_3;
        temp_v0_3 = FIELD(temp_v0_2, s32 *, 4);
        FIELD(spad, s32 *, 0x04) = temp_v0_3;
        temp_a0_3 = FIELD(car, s32 *, 0) - temp_v1_3;
        FIELD(spad, s32 *, 0x08) = temp_a0_3;
        temp_a1 = FIELD(car, s32 *, 8) - temp_v0_3;
        FIELD(spad, s32 *, 0x0C) = temp_a1;
        FIELD(spad, s16 *, 0x7E) = Atan2(temp_a0_3, temp_a1) & 0xFFF;
        temp_a0_4 = FIELD(temp_s4, s32 *, 0);
        temp_v1_4 = FIELD(spad, s32 *, 0);
        temp_a2_2 = FIELD(spad, s32 *, 0x04);
        temp_a0_4 -= temp_v1_4;
        FIELD(spad, s32 *, 0x24) = temp_a0_4;
        temp_a1_2 = FIELD(temp_s4, s32 *, 4) - temp_a2_2;
        FIELD(spad, s32 *, 0x2C) = temp_a1_2;
        FIELD(spad, s32 *, 0x28) = FIELD(temp_s6, s32 *, 0) - temp_v1_4;
        FIELD(spad, s32 *, 0x30) = FIELD(temp_s6, s32 *, 4) - temp_a2_2;
        FIELD(spad, s16 *, 0x80) = Atan2(temp_a0_4, temp_a1_2) & 0xFFF;
        FIELD(spad, s16 *, 0x82) =
            Atan2(FIELD(spad, s32 *, 0x28), FIELD(spad, s32 *, 0x30)) & 0xFFF;
        temp_s0 = rcos(FIELD(spad, s16 *, 0x7E));
        var_v0 = (temp_s0 * FIELD(spad, s32 *, 0x08)) +
                 (rsin(FIELD(spad, s16 *, 0x7E)) * FIELD(spad, s32 *, 0x0C));
        if (var_v0 < 0) {
            var_v0 += 0xFFF;
        }
        FIELD(spad, s32 *, 0x10) = var_v0 >> 0xC;
        temp_s0_2 = rcos(FIELD(spad, s16 *, 0x80));
        var_v0_2 = (temp_s0_2 * FIELD(spad, s32 *, 0x24)) +
                   (rsin(FIELD(spad, s16 *, 0x80)) * FIELD(spad, s32 *, 0x2C));
        if (var_v0_2 < 0) {
            var_v0_2 += 0xFFF;
        }
        FIELD(spad, s32 *, 0x14) = var_v0_2 >> 0xC;
        temp_s0_3 = rcos(FIELD(spad, s16 *, 0x82));
        var_v0_3 = (temp_s0_3 * FIELD(spad, s32 *, 0x28)) +
                   (rsin(FIELD(spad, s16 *, 0x82)) * FIELD(spad, s32 *, 0x30));
        if (var_v0_3 < 0) {
            var_v0_3 += 0xFFF;
        }
        FIELD(spad, s32 *, 0x18) = var_v0_3 >> 0xC;
        FIELD(spad, s16 *, 0x7C) =
            GetAngleDistance(FIELD(spad, s16 *, 0x80), FIELD(spad, s16 *, 0x82));
        if (FIELD(spad, s16 *, 0x7C) <= 0) {
            FIELD(spad, s16 *, 0x7C) = 1;
        }
        temp_v0_4 =
            GetAngleDistance(FIELD(spad, s16 *, 0x80), FIELD(spad, s16 *, 0x7E));
        temp_a1_3 = FIELD(spad, s16 *, 0x7C);
        FIELD(spad, s16 *, 0x7E) = temp_v0_4;
        FIELD(spad, s32 *, 0x14) =
            (s32)(((s16)temp_v0_4 * FIELD(spad, s32 *, 0x14)) +
                  ((temp_a1_3 - (s16)temp_v0_4) * FIELD(spad, s32 *, 0x18))) /
            temp_a1_3;
        var_a0_2 = (s16)(FIELD(spad, u16 *, 0x10) - FIELD(spad, u16 *, 0x14));
        if (FIELD(spad, s16 *, 0x78) == 2) {
            var_a0_2 = 0 - var_a0_2;
        }
        FIELD(spad, s16 *, 0x84) = var_a0_2;
        {
            temp_a0 = FIELD(temp_s6, s16 *, 0xA);
            var_a1_2 = FIELD(temp_s4, s16 *, 0xA);
            if ((temp_a0 - var_a1_2) >= 0x801) {
                temp_v1_5 = FIELD(spad, s16 *, 0x7E);
                temp_a0_5 = FIELD(spad, s16 *, 0x7C);
                FIELD(spad, s16 *, 0x90) =
                    (s16)((((temp_a0 - 0x1000) * temp_v1_5) +
                           (var_a1_2 * (temp_a0_5 - temp_v1_5))) /
                          temp_a0_5);
            } else if ((var_a1_2 - temp_a0) >= 0x801) {
                temp_v1_5 = FIELD(spad, s16 *, 0x7E);
                temp_a0_5 = FIELD(spad, s16 *, 0x7C);
                FIELD(spad, s16 *, 0x90) =
                    (s16)(((temp_a0 * temp_v1_5) +
                           ((var_a1_2 - 0x1000) * (temp_a0_5 - temp_v1_5))) /
                          temp_a0_5);
            } else {
                temp_v1_5 = FIELD(spad, s16 *, 0x7E);
                temp_a0_5 = FIELD(spad, s16 *, 0x7C);
                FIELD(spad, s16 *, 0x90) =
                    (s16)(((temp_a0 * temp_v1_5) +
                           (var_a1_2 * (temp_a0_5 - temp_v1_5))) /
                          temp_a0_5);
            }
        }
    }

    FIELD(spad, u16 *, 0x60) =
        (u16)(((u16)FIELD(car, s32 *, 0) - (u16)FIELD(temp_s4, s32 *, 0)) * 4);
    temp_a0 = FIELD(spad, s16 *, 0x90);
    FIELD(spad, s16 *, 0x64) =
        (s16)(((u16)FIELD(car, s32 *, 8) - (u16)FIELD(temp_s4, s32 *, 4)) * 4);
    FIELD(spad, s16 *, 0x62) = 0;
    temp_s0_4 = rcos(temp_a0);
    var_v1 = (temp_s0_4 * (s16)FIELD(spad, u16 *, 0x60)) +
             (rsin(FIELD(spad, s16 *, 0x90)) * FIELD(spad, s16 *, 0x64));
    if (var_v1 < 0) {
        var_v1 += 0xFFF;
    }
    var_s3 = var_v1 >> 0xE;

    /*
     * The non-clamping path does not consume the lateral component. GCC 2.6.3
     * removes its mflo/add/round/shift but leaves the two HI/LO-setting mults.
     */
    temp_s0_5 = rsin(FIELD(spad, s16 *, 0x90));
    var_v1 = ((0 - temp_s0_5) * (s16)FIELD(spad, u16 *, 0x60)) +
             (rcos(FIELD(spad, s16 *, 0x90)) * FIELD(spad, s16 *, 0x64));
    if (var_v1 < 0) {
        var_v1 += 0xFFF;
    }
    var_a2 = var_v1 >> 0xE;

    if (FIELD(spad, s16 *, 0x96) < var_s3) {
        var_s3 = FIELD(spad, s16 *, 0x96);
    } else if (var_s3 < 0) {
        var_s3 = 0;
    }
    temp_a0_6 = FIELD(spad, s16 *, 0x96);
    temp_lo = ((FIELD(temp_s6, s16 *, 0x12) * var_s3) +
               (FIELD(temp_s4, s16 *, 0x12) * (temp_a0_6 - var_s3))) /
              temp_a0_6;
    FIELD(spad, s16 *, 0x88) = (s16)temp_lo;
    useProgress = *(s32 *)0x801E408C;
    temp_a0_7 = FIELD(spad, s16 *, 0x96);
    {
        s32 widthSum;
        s32 remainingLength;

        widthSum = FIELD(temp_s6, s16 *, 0x10) * var_s3;
        remainingLength = temp_a0_7 - var_s3;
        widthSum += FIELD(temp_s4, s16 *, 0x10) * remainingLength;
        FIELD(spad, s16 *, 0x8A) = (s16)(widthSum / temp_a0_7);
    }
    {
        u32 outputProgress;

        if (useProgress != 0) {
            outputProgress = var_s3;
        } else {
            outputProgress = FIELD(spad, s16 *, 0x96) - var_s3;
        }
        FIELD(car, s32 *, 0x6C) = outputProgress;
    }
    temp_a0_8 = FIELD(spad, s16 *, 0x96);
    FIELD(spad, s16 *, 0x8E) =
        (s16)(((FIELD(temp_s6, s16 *, 0xE) * var_s3) +
               (FIELD(temp_s4, s16 *, 0xE) * (temp_a0_8 - var_s3))) /
              temp_a0_8);
    {
        s16 angle;

        angle = (u16)FIELD(car, s32 *, 0x24);
        angle -= 0xC00;
        FIELD(spad, s16 *, 0x8C) = angle + FIELD(spad, u16 *, 0x90);
    }
    temp_a0_9 = FIELD(spad, s16 *, 0x96);
    FIELD(spad, s16 *, 0x92) =
        (s16)(((FIELD(temp_s6, s16 *, 0xC) * var_s3) +
               (FIELD(temp_s4, s16 *, 0xC) * (temp_a0_9 - var_s3))) /
              temp_a0_9);
    temp_a0_10 = (u16)FIELD(spad, s16 *, 0x8A) + (u16)FIELD(spad, s16 *, 0x88);
    FIELD(spad, s16 *, 0x86) = temp_a0_10;
    temp_s0_7 = Atan2(
        (s32)temp_a0_10,
        (s32)(FIELD(temp_s6, s16 *, 0xE) * temp_a0_10) >> 7);
    temp_a0_11 = FIELD(spad, s16 *, 0x86);
    secondResult = Atan2(
        (s32)temp_a0_11,
        (s32)(FIELD(temp_s4, s16 *, 0xE) * temp_a0_11) >> 7);
    temp_a1_4 = FIELD(spad, s16 *, 0x96);
    FIELD(spad, s16 *, 0x94) =
        (s16)(((temp_s0_7 * var_s3) +
               (secondResult * (temp_a1_4 - var_s3))) /
              temp_a1_4);
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
            var_a1 = FIELD(spad, s16 *, 0x92) * FIELD(spad, s32 *, 0x34);
            firstComponent = firstProduct >> 0xC;
        }
        if (var_a1 < 0) {
            var_a1 += 0xFFF;
        }
        {
            s32 trackLength;
            s32 progress;
            s32 combinedComponent;

            trackLength = g_TrackLength;
            progress =
                (FIELD(car, s32 *, 0x68) + FIELD(car, s32 *, 0x6C)) % trackLength;
            combinedComponent = firstComponent + (var_a1 >> 0xC);
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

void BuildSpriteFromDesc(SPRT *prim, GameSpriteDesc *src);
void SetDrawModeWide(u8 *arg0, s32 arg1, s32 arg2, u16 arg3, void *arg4) asm("SetDrawMode");

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
    SetDrawModeWide(prim0 - 0x18, 0, 1, 9, 0);
    SetDrawModeWide(prim0 - 0x0C, 0, 1, 0xA, 0);
    SetDrawModeWide(g_TachoNeedlePrim1PageA, 0, 1, 9, 0);
    SetDrawModeWide(g_TachoNeedlePrim1PageB, 0, 1, 0xA, 0);
}
