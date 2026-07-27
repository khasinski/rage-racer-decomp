#include "common.h"
#include "game/track.h"
#include "psyq/gte.h"
#include "game/render.h"

#define FIELD(base, type, offset) (*(type)((s32)(base) + (offset)))

void GameSetCarKnockback(void *, s32, s32, s32) asm("func_80038CE8");            /* extern */
s32 func_80068568(s32);                               /* extern */
s32 func_80068634(s32);                               /* extern */
extern u8 g_PlayerCar asm("D_8009E6D4");

/*
 * Track-segment / route-sprite geometry builder. Interpolates between the
 * GameTrackPoint at `trackPointIndex` (*(GameTrackPoint*)0x8009E688 + i*0x18)
 * and its successor: computes route angles/heights via atan2 (GameAtan2)
 * and rsin/rcos (func_80068568/func_80068634), builds the collision-boundary
 * offset, and writes the interpolated position/angle/height into the render
 * object `obj`. The scratchpad struct at 0x1F80011C ("spad") is the GTE
 * per-primitive transform scratch. `clampPair` supplies the s16 margin values
 * (offsets 0/2/4/6). Raw FIELD(base,type,offset) accesses preserve the match.
 * Returns the boundary/skid response code.
 */
s32 GameUpdateCarTrackState(void *obj, s32 trackPointIndex, void *clampPair) asm("func_80031298");
s32 GameUpdateCarTrackState(void *obj, s32 trackPointIndex, void *clampPair) {
    s32 temp_a0;
    s32 secondResult;
    s16 temp_a0_10;
    s16 temp_a0_11;
    s16 temp_a0_12;
    s16 temp_a0_5;
    s16 temp_a0_6;
    s16 temp_a0_7;
    s16 temp_a0_8;
    s16 temp_a0_9;
    s32 temp_a1_3;
    s16 temp_a1_4;
    s16 temp_v0;
    s32 temp_v0_4;
    s32 temp_v1_5;
    s32 var_a0_2;
    s32 var_a1_2;
    s32 temp_a0_2;
    s32 temp_a0_3;
    s32 temp_a0_4;
    s32 temp_a1;
    s32 temp_a1_2;
    s32 temp_a2;
    s32 temp_a2_2;
    s32 temp_a3;
    s32 temp_hi;
    s32 temp_hi_2;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_s0;
    s32 temp_s0_2;
    s32 temp_s0_3;
    s32 temp_s0_4;
    s32 temp_s0_5;
    s32 temp_s0_7;
    s32 temp_v0_3;
    s32 temp_v1_2;
    s32 temp_v1_3;
    s32 temp_v1_4;
    s32 temp_v1_6;
    s32 temp_v1_7;
    s32 var_a0_3;
    s32 var_a1;
    s32 var_a2;
    s32 var_s3;
    s32 var_v0;
    s32 var_v0_2;
    s32 var_v0_3;
    s32 var_v1;
    u16 temp_v1;
    void *temp_s0_6;
    void *temp_s4;
    void *temp_s6;
    void *temp_v0_2;
    void *spad;

    temp_hi_2 = (trackPointIndex + 1) % *(s32 *)0x8009E6A8;
    temp_a0_2 = *(s32 *)0x8009E688;
    spad = (void *)0x1F80011C;
    *(s32 *)0x1F800158 = 0;
    temp_s4 = (void *)((trackPointIndex * 0x18) + temp_a0_2);
    temp_v1 = FIELD(temp_s4, u16 *, 0x16);
    FIELD(spad, u16 *, 0x96) = temp_v1;
    temp_s6 = (void *)((temp_hi_2 * 0x18) + temp_a0_2);
    if ((s32) ((u32) temp_v1 << 0x10) <= 0)
    {
        FIELD(spad, u16 *, 0x96) = 1U;
    }
    FIELD(spad, u16 *, 0x90) = FIELD(temp_s4, u16 *, 0xA);
    temp_v1_2 = (s32) ((u32) FIELD(temp_s4, u16 *, 0x14) << 0x10) >> 0x14;
    FIELD(spad, s16 *, 0x7A) = (s16) temp_v1_2;
    temp_v0 = FIELD(temp_s4, u16 *, 0x14) & 3;
    FIELD(spad, s16 *, 0x78) = temp_v0;
    if (temp_v0 != 0)
    {
        temp_v0_2 = (void *)((temp_v1_2 * 0xC) + *(s32 *)0x8019C7D0);
        temp_v1_3 = FIELD(temp_v0_2, s32 *, 0);
        FIELD(spad, s32 *, 0) = temp_v1_3;
        temp_v0_3 = FIELD(temp_v0_2, s32 *, 4);
        FIELD(spad, s32 *, 0x04) = temp_v0_3;
        temp_a0_3 = FIELD(obj, s32 *, 0) - temp_v1_3;
        FIELD(spad, s32 *, 0x08) = temp_a0_3;
        temp_a1 = FIELD(obj, s32 *, 8) - temp_v0_3;
        FIELD(spad, s32 *, 0x0C) = temp_a1;
        FIELD(spad, s16 *, 0x7E) = GameAtan2(temp_a0_3, temp_a1) & 0xFFF;
        temp_a0_4 = FIELD(temp_s4, s32 *, 0);
        temp_v1_4 = FIELD(spad, s32 *, 0);
        temp_a2_2 = FIELD(spad, s32 *, 0x04);
        temp_a0_4 -= temp_v1_4;
        FIELD(spad, s32 *, 0x24) = temp_a0_4;
        temp_a1_2 = FIELD(temp_s4, s32 *, 4) - temp_a2_2;
        FIELD(spad, s32 *, 0x2C) = temp_a1_2;
        FIELD(spad, s32 *, 0x28) = FIELD(temp_s6, s32 *, 0) - temp_v1_4;
        FIELD(spad, s32 *, 0x30) = FIELD(temp_s6, s32 *, 4) - temp_a2_2;
        FIELD(spad, s16 *, 0x80) = GameAtan2(temp_a0_4, temp_a1_2) & 0xFFF;
        FIELD(spad, s16 *, 0x82) = GameAtan2(FIELD(spad, s32 *, 0x28), FIELD(spad, s32 *, 0x30)) & 0xFFF;
        temp_s0 = func_80068634(FIELD(spad, s16 *, 0x7E));
        var_v0 = (temp_s0 * FIELD(spad, s32 *, 0x08)) + (func_80068568(FIELD(spad, s16 *, 0x7E)) * FIELD(spad, s32 *, 0x0C));
        if (var_v0 < 0)
        {
            var_v0 += 0xFFF;
        }
        FIELD(spad, s32 *, 0x10) = var_v0 >> 0xC;
        temp_s0_2 = func_80068634(FIELD(spad, s16 *, 0x80));
        var_v0_2 = (temp_s0_2 * FIELD(spad, s32 *, 0x24)) + (func_80068568(FIELD(spad, s16 *, 0x80)) * FIELD(spad, s32 *, 0x2C));
        if (var_v0_2 < 0)
        {
            var_v0_2 += 0xFFF;
        }
        FIELD(spad, s32 *, 0x14) = var_v0_2 >> 0xC;
        temp_s0_3 = func_80068634(FIELD(spad, s16 *, 0x82));
        var_v0_3 = (temp_s0_3 * FIELD(spad, s32 *, 0x28)) + (func_80068568(FIELD(spad, s16 *, 0x82)) * FIELD(spad, s32 *, 0x30));
        if (var_v0_3 < 0)
        {
            var_v0_3 += 0xFFF;
        }
        FIELD(spad, s32 *, 0x18) = var_v0_3 >> 0xC;
        FIELD(spad, s16 *, 0x7C) = GameGetAngleDistance(FIELD(spad, s16 *, 0x80), FIELD(spad, s16 *, 0x82));
        temp_v0_4 = GameGetAngleDistance(FIELD(spad, s16 *, 0x80), FIELD(spad, s16 *, 0x7E));
        temp_a1_3 = FIELD(spad, s16 *, 0x7C);
        FIELD(spad, s16 *, 0x7E) = temp_v0_4;
        {
            s32 interpolated;

            if (temp_a1_3 <= 0)
            {
                interpolated = FIELD(spad, s32 *, 0x14);
                FIELD(spad, s16 *, 0x7C) = 1;
            }
            else
            {
                interpolated = (s32) ((s32) (((s16) temp_v0_4 * FIELD(spad, s32 *, 0x14)) + ((temp_a1_3 - (s16) temp_v0_4) * FIELD(spad, s32 *, 0x18))) / temp_a1_3);
            }
            *(s32 *)0x1F800130 = interpolated;
        }
        var_a0_2 = (s16) (FIELD(spad, u16 *, 0x10) - FIELD(spad, u16 *, 0x14));
        if (FIELD(spad, s16 *, 0x78) == 2)
        {
            var_a0_2 = 0 - var_a0_2;
        }
        FIELD(spad, s16 *, 0x84) = var_a0_2;
        {
            temp_a0 = FIELD(temp_s6, s16 *, 0xA);
            var_a1_2 = FIELD(temp_s4, s16 *, 0xA);
            if ((temp_a0 - var_a1_2) >= 0x801)
            {
                temp_v1_5 = FIELD(spad, s16 *, 0x7E);
                temp_a0_5 = FIELD(spad, s16 *, 0x7C);
                FIELD(spad, s16 *, 0x90) = (s16) ((s32) (((temp_a0 - 0x1000) * temp_v1_5) + (var_a1_2 * (temp_a0_5 - temp_v1_5))) / temp_a0_5);
            }
            else if ((var_a1_2 - temp_a0) >= 0x801)
            {
                temp_v1_5 = FIELD(spad, s16 *, 0x7E);
                temp_a0_5 = FIELD(spad, s16 *, 0x7C);
                FIELD(spad, s16 *, 0x90) = (s16) ((s32) ((temp_a0 * temp_v1_5) + ((var_a1_2 - 0x1000) * (temp_a0_5 - temp_v1_5))) / temp_a0_5);
            }
            else
            {
                temp_v1_5 = FIELD(spad, s16 *, 0x7E);
                temp_a0_5 = FIELD(spad, s16 *, 0x7C);
                FIELD(spad, s16 *, 0x90) = (s16) ((s32) ((temp_a0 * temp_v1_5) + (var_a1_2 * (temp_a0_5 - temp_v1_5))) / temp_a0_5);
            }
        }
        goto block_21;
    }
block_21:
    FIELD(spad, u16 *, 0x60) = (u16) (((u16) FIELD(obj, s32 *, 0) - (u16) FIELD(temp_s4, s32 *, 0)) * 4);
    temp_a0 = FIELD(spad, s16 *, 0x90);
    FIELD(spad, s16 *, 0x64) = (s16) (((u16) FIELD(obj, s32 *, 8) - (u16) FIELD(temp_s4, s32 *, 4)) * 4);
    FIELD(spad, s16 *, 0x62) = 0;
    temp_s0_4 = func_80068634(temp_a0);
    var_v1 = (temp_s0_4 * (s16) FIELD(spad, u16 *, 0x60)) + (func_80068568(FIELD(spad, s16 *, 0x90)) * FIELD(spad, s16 *, 0x64));
    if (var_v1 < 0)
    {
        var_v1 += 0xFFF;
    }
    var_s3 = var_v1 >> 0xE;
    temp_s0_5 = func_80068568(FIELD(spad, s16 *, 0x90));
    var_v1 = ((0 - temp_s0_5) * (s16) FIELD(spad, u16 *, 0x60)) + (func_80068634(FIELD(spad, s16 *, 0x90)) * FIELD(spad, s16 *, 0x64));
    if (var_v1 < 0)
    {
        var_v1 += 0xFFF;
    }
    var_a2 = var_v1 >> 0xE;
    if (FIELD(spad, s16 *, 0x78) != 0)
    {
        var_a2 = (s32) FIELD(spad, s16 *, 0x84);
    }
    temp_a0_6 = FIELD(spad, s16 *, 0x96);
    FIELD(spad, s16 *, 0x8A) = (s16) ((s32) ((FIELD(temp_s6, s16 *, 0x10) * var_s3) + (FIELD(temp_s4, s16 *, 0x10) * (temp_a0_6 - var_s3))) / temp_a0_6);
    temp_a0_7 = FIELD(spad, s16 *, 0x96);
    temp_lo = (s32) ((FIELD(temp_s6, s16 *, 0x12) * var_s3) + (FIELD(temp_s4, s16 *, 0x12) * (temp_a0_7 - var_s3))) / temp_a0_7;
    FIELD(spad, s16 *, 0x88) = (s16) temp_lo;
    temp_v1_6 = FIELD(spad, s16 *, 0x8A) + FIELD(clampPair, s16 *, 2);
    temp_s0_6 = spad + 0x40;
    if (var_a2 < (0 - temp_v1_6))
    {
        var_a2 += temp_v1_6;
        FIELD(spad, u16 *, 0x60) = 0U;
        FIELD(spad, s16 *, 0x62) = 0;
        FIELD(spad, s16 *, 0x64) = var_a2;
        GameBuildRotMatrixY(temp_s0_6, FIELD(spad, s16 *, 0x90));
        ApplyMatrix(temp_s0_6, (void *)((u8 *)spad + 0x60), (void *)((u8 *)spad + 0x68));
        if (obj == &g_PlayerCar)
        {
            GameSetCarKnockback(obj, FIELD(spad, s32 *, 0x68), FIELD(spad, s32 *, 0x70), FIELD(clampPair, s16 *, 6));
        }
        FIELD(obj, s32 *, 0) = (s32) (FIELD(obj, s32 *, 0) - FIELD(spad, s32 *, 0x68));
        FIELD(obj, s32 *, 8) = (s32) (FIELD(obj, s32 *, 8) - FIELD(spad, s32 *, 0x70));
        var_a2 = -FIELD(spad, s16 *, 0x8A) - FIELD(clampPair, s16 *, 2);
        FIELD(spad, s32 *, 0x3C) = FIELD(clampPair, s16 *, 6);
        goto boundary_done;
    }
    temp_v1_7 = (s16) temp_lo - FIELD(clampPair, s16 *, 0);
    if (temp_v1_7 < var_a2)
    {
        var_a2 -= temp_v1_7;
        FIELD(spad, u16 *, 0x60) = 0U;
        FIELD(spad, s16 *, 0x62) = 0;
        FIELD(spad, s16 *, 0x64) = var_a2;
        GameBuildRotMatrixY(temp_s0_6, FIELD(spad, s16 *, 0x90));
        ApplyMatrix(temp_s0_6, (void *)((u8 *)spad + 0x60), (void *)((u8 *)spad + 0x68));
        if (obj == &g_PlayerCar)
        {
            GameSetCarKnockback(obj, FIELD(spad, s32 *, 0x68), FIELD(spad, s32 *, 0x70), FIELD(clampPair, s16 *, 4));
        }
        FIELD(obj, s32 *, 0) = (s32) (FIELD(obj, s32 *, 0) - FIELD(spad, s32 *, 0x68));
        FIELD(obj, s32 *, 8) = (s32) (FIELD(obj, s32 *, 8) - FIELD(spad, s32 *, 0x70));
        var_a2 = FIELD(spad, s16 *, 0x88) - FIELD(clampPair, s16 *, 0);
        FIELD(spad, s32 *, 0x3C) = (s32) FIELD(clampPair, s16 *, 4);
    }
boundary_done:
    if (FIELD(spad, s16 *, 0x96) < var_s3)
    {
        var_s3 = FIELD(spad, s16 *, 0x96);
    }
    else if (var_s3 < 0)
    {
        var_s3 = 0;
    }
    FIELD(obj, s32 *, 0x38) = (s32) ((s32) (var_s3 << 0xA) / (s16) FIELD(spad, s16 *, 0x96));
    if (var_a2 < 0)
    {
        FIELD(obj, s32 *, 0x3C) = (var_a2 * 0x400) / FIELD(spad, s16 *, 0x8A);
    }
    else
    {
        FIELD(obj, s32 *, 0x3C) = (var_a2 * 0x400) / FIELD(spad, s16 *, 0x88);
    }
    {
        u32 outputProgress;
        s32 useProgress;

        useProgress = *(s32 *)0x801E408C;
        FIELD(obj, s32 *, 0x34) = var_a2;
        if (useProgress != 0)
        {
            outputProgress = var_s3;
        }
        else
        {
            outputProgress = FIELD(spad, s16 *, 0x96) - var_s3;
        }
        FIELD(obj, s32 *, 0x6C) = outputProgress;
    }
    temp_a0_8 = FIELD(spad, s16 *, 0x96);
    FIELD(spad, s16 *, 0x8E) = (s16) ((s32) ((FIELD(temp_s6, s16 *, 0xE) * var_s3) + (FIELD(temp_s4, s16 *, 0xE) * (temp_a0_8 - var_s3))) / temp_a0_8);
    temp_a0_9 = FIELD(spad, s16 *, 0x96);
    temp_lo_2 = (s32) ((FIELD(temp_s6, s16 *, 8) * var_s3) + (FIELD(temp_s4, s16 *, 8) * (temp_a0_9 - var_s3))) / temp_a0_9;
    FIELD(obj, s32 *, 4) = temp_lo_2;
    FIELD(obj, s32 *, 4) = (s32) (((s32) (FIELD(spad, s16 *, 0x8E) * var_a2) >> 7) + temp_lo_2);
    {
        s16 angle;

        angle = FIELD(obj, u16 *, 0x24);
        angle -= 0xC00;
        FIELD(spad, s16 *, 0x8C) = angle + FIELD(spad, u16 *, 0x90);
    }
    temp_a0_10 = FIELD(spad, s16 *, 0x96);
    FIELD(spad, s16 *, 0x92) = (s16) ((s32) ((FIELD(temp_s6, s16 *, 0xC) * var_s3) + (FIELD(temp_s4, s16 *, 0xC) * (temp_a0_10 - var_s3))) / temp_a0_10);
    temp_a0_11 = (u16) FIELD(spad, s16 *, 0x88) + (u16) FIELD(spad, s16 *, 0x8A);
    FIELD(spad, s16 *, 0x86) = temp_a0_11;
    temp_s0_7 = GameAtan2((s32) temp_a0_11, (s32) (FIELD(temp_s6, s16 *, 0xE) * temp_a0_11) >> 7);
    temp_a0_12 = FIELD(spad, s16 *, 0x86);
    secondResult = GameAtan2((s32) temp_a0_12, (s32) (FIELD(temp_s4, s16 *, 0xE) * temp_a0_12) >> 7);
    temp_a1_4 = FIELD(spad, s16 *, 0x96);
    FIELD(spad, s16 *, 0x94) = (s16) ((s32) ((temp_s0_7 * var_s3) + (secondResult * (temp_a1_4 - var_s3))) / temp_a1_4);
    FIELD(spad, s32 *, 0x38) = func_80068634(FIELD(spad, s16 *, 0x8C));
    {
        s32 firstProduct;
        s32 sinValue;
        s32 secondProduct;

        sinValue = func_80068568(FIELD(spad, s16 *, 0x8C));
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
    var_a0_3 = (0 - FIELD(spad, s32 *, 0x38)) * FIELD(spad, s16 *, 0x94);
    if (var_a0_3 < 0)
    {
        var_a0_3 += 0xFFF;
    }
    var_a1 = FIELD(spad, s16 *, 0x92) * FIELD(spad, s32 *, 0x34);
    temp_a3 = var_a0_3 >> 0xC;
    if (var_a1 < 0)
    {
        var_a1 += 0xFFF;
    }
    temp_a2 = g_TrackLength;
    temp_hi = (s32) (FIELD(obj, s32 *, 0x68) + FIELD(obj, s32 *, 0x6C)) % temp_a2;
    FIELD(obj, s32 *, 0x28) = (s32) (temp_a3 + (var_a1 >> 0xC));
    FIELD(obj, s32 *, 0xB4) = (s32) FIELD(spad, s16 *, 0x90);
    FIELD(obj, s32 *, 0x74) = (s32) FIELD(obj, s32 *, 0x70);
    FIELD(obj, s32 *, 0x70) = temp_hi;
    if (temp_hi < 0)
    {
        FIELD(obj, s32 *, 0x70) = (s32) (temp_hi + temp_a2);
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
