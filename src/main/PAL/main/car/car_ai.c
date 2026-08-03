#include "common.h"
#include "game/car.h"
#include "game/track.h"
#include "game/race.h"
#include "psyq/gte.h"

typedef struct GameCollisionPoint {
    s16 x;
    s16 z;
} GameCollisionPoint;

typedef struct GameCollisionPointBytes {
    u8 bytes[4];
} GameCollisionPointBytes;

extern GameCollisionPoint g_CarCollisionCorners[4] asm("D_8007E23C");

void TransformCollisionVector(s32 *input, s32 *output) asm("func_800690E0");
void SetCarKnockback(GameCarRuntime *car, s32 x, s32 z, s32 mode)
    asm("func_80038CE8");
s32 IsPointInQuad(s32 p0, s32 p1, s32 p2, s32 p3, s32 point)
    asm("func_8002D2E8");

s32 GetCarCrestTrigger(GameCarRuntime *arg0) asm("func_80039184");

s32 func_80068568(s32 arg0);

/*
 * Jump / launch setup: when GetCarCrestTrigger reports a marker crossing, seeds the
 * launch trajectory (field_90/94/98/9A/9C/9E) and snapshots the car's render
 * offsets (field_20/28 and y). field_98 holds the launch state (1 = jump). The
 * inline mult/mfhi block is the compiler's divide idiom; keep it verbatim.
 */

/*
 * Fields of g_Cars[0] that retail reaches through their own split symbols
 * rather than by indexing the array, so the 0x19C-stride walks below keep them
 * raw:  g_CarTrackProgress = .trackProgress  g_CarMarkerIndex = .field_138
 *       g_CarMarkerFlag = .field_13A
 */
extern u8 g_CarTrackProgress[] asm("D_801F18C4");

extern u8 g_CarMarkerIndex[] asm("D_801F198C");

extern u8 g_CarMarkerFlag[] asm("D_801F198E");

void UpdateCarBodyKick(GameCarRuntime *car) asm("func_80038FF0");
void UpdateCarBodyKick(GameCarRuntime *car) {
    s32 value;
    s32 wave;
    s32 amplitude;
    s32 product;

    if (car->motionMode == 0) {
        return;
    }

    car->motionModeTimer--;
    if ((s16)car->motionModeTimer == 0) {
        car->motionMode = 0;
        car->field_92 = 0;
    }

    {
        s32 timer;

        timer = car->motionModeTimer;
        product = timer * car->motionValue;
        if (product < 0) {
            product += 0x7F;
        }
        amplitude = product >> 7;

        wave = func_80068568(((timer * 3) << 12) / 30) * amplitude;
    }
    if (wave < 0) {
        wave += 0x7FF;
    }
    value = wave >> 11;

    switch (car->motionMode) {
    case 1:
    case 5:
        car->field_92 = value + amplitude;
        car->field_20 += car->field_92;
        car->field_92 = value + amplitude / 2;
        car->field_28 += car->field_92 / 2;
        break;

    case 2:
        if (car->field_98 != 0) {
            break;
        }
        car->field_28 += value;
        break;

    case 3:
        car->field_92 = value + amplitude;
        car->field_20 += car->field_92;
        break;

    case 4:
        car->field_28 += value;
        break;
    }
}

s32 GetCarCrestTrigger(GameCarRuntime *arg0) asm("func_80039184");
s32 GetCarCrestTrigger(GameCarRuntime *arg0) {
    u8 *base;
    s32 pos0;
    s32 pos1;
    s32 row;
    s32 temp;
    s32 crossed;
    register s32 i asm("a2");
    s32 offset;
    s32 sentinel;
    s32 cursor;
    s32 diff;
    s32 cmp;
    s32 threshold;
    register s32 resultOffset asm("v0");

    base = g_TrackEventData;
    if (arg0->field_A4 < 0x320) {
        return 0;
    }

    pos0 = arg0->trackProgress;
    pos1 = arg0->previousTrackProgress;
    row = arg0->facingBackwards;

    if (g_RaceSeries != 0) {
        pos0 = g_TrackLength - pos0;
        pos1 = g_TrackLength - pos1;
    }

    switch (0) { default:
    if (pos1 < pos0) {
        temp = pos0;
        diff = temp - pos1;
    } else {
    goto not_crossed;

crossed_label:
    crossed = 1;
    break;

not_crossed:
    temp = pos1;
    pos1 = pos0;
    diff = temp - pos1;

    }
    if (diff >= 0x1000) {
        temp = 0;
        pos1 = 0;
    }

    crossed = 0;
    i = 0;
    sentinel = -1;
    offset = row * 64;
    cursor = (s32)(base + offset);

for (;;) {
    if (!(*(s32 *)(cursor + 8) == sentinel)) {
    threshold = *(s32 *)(cursor + 4);
    cmp = temp < threshold;
    offset += 8;
    if (!(cmp != 0)) {
    cmp = pos1 < threshold;
    if (cmp != 0) {
        goto crossed_label;
    }

    }
    i++;
    if (i < 8) {
        cursor = (s32)(base + offset);
        continue;
    }

    }
break;
}
    }
    if (crossed != 0) {
        resultOffset = i * 8;
        resultOffset += row * 64;
        resultOffset = (s32)(base + resultOffset);
        return *(s32 *)(resultOffset + 8);
    }
    return 0;
}

void UpdateCarCrestHop(GameCarRuntime *arg0) asm("func_80039280");
void UpdateCarCrestHop(GameCarRuntime *arg0) {
    GameCarRuntime *obj;
    s32 value;
    /* These pins are load-bearing: removing either one changes .text. */
    register s32 temp asm("$3");
    s32 result;
    s32 one;
    volatile s32 stack[2];

    obj = arg0;
    (void)stack;

    if (obj->field_98 != 0) {
        result = obj->field_9A;
        value = result * result;
        temp = obj->field_90;
        /* /6 is the retail `mult` by 0x2AAAAAAB + `mfhi` - (x >> 31); gcc
         * generates that magic-number sequence for a signed divide by 6. */
        value = value / 6;
        /* These barriers are load-bearing: without them the copy to `result`
         * is scheduled ahead of the divide and the load delay needs a nop. */
        asm volatile("" : "=r"(value) : "0"(value));
        result = temp;
        asm volatile("" : "=r"(temp) : "0"(temp));
        if (temp >= 0x12C) {
            value >>= 8;
        }
        result += value;
        obj->field_90 = result;

        temp = value;
        if (value < 0) {
            temp = value + 3;
        }
        result = (u16)obj->field_94;
        temp >>= 2;
        result += temp;
        obj->field_94 = result;

        result = obj->field_90;
        temp = obj->field_94;
        obj->field_20 = result;
        obj->field_28 = temp;
        return;
    }

    value = GetCarCrestTrigger((GameCarRuntime *)obj);
    if (value == 0) {
        return;
    }

    one = 1;
    obj->field_98 = one;
    if (value > 0) {
        temp = value * obj->field_A4;
        temp = temp / -4800;
        obj->field_98 = one;
        obj->field_9C = temp;
    } else {
        result = 2;
        obj->field_98 = result;
        result = -value;
        obj->field_9C = result;
    }

    result = (u16)obj->field_20;
    temp = (u16)obj->field_28;
    value = (u16)obj->y;
    obj->field_9A = 0;
    obj->field_90 = result;
    obj->field_94 = temp;
    obj->field_9E = value;
}

void UpdateCarSlideAngle(GameCarRuntime *arg0, s32 arg1) asm("func_800393AC");
void UpdateCarSlideAngle(GameCarRuntime *arg0, s32 arg1) {
    GameCarRuntime *obj = arg0;
    s32 temp;
    s32 value;
    u8 *base;
    s32 scene;

    base = (u8 *)&obj->field_BC;
    switch (0) { default:
    if (*(s32 *)&obj->field_F0 == 0) {
        if (!(obj->field_F4 != 0)) {
        if (arg1 != 0) {
            value = obj->field_A4;
            if (value < 0x3C1) {
                return;
            }
            scene = g_RaceSeries;
            arg1 *= value;
            value = arg1;
            temp = value / 0x320;
            *(s32 *)&obj->field_F0 = temp;
            if (scene != 0) {
                temp = -temp;
                *(s32 *)&obj->field_F0 = temp;
            }
            obj->field_F4 = 0;
            return;
        }
        }
        temp = *(s32 *)(base + 0x34);
        if (temp == 0) {
            break;
        }
    }

    value = *(s32 *)(base + 0x34);
    value = (value * 32) - value;
    if (value < 0) {
        value += 0x1F;
    }
    temp = value >> 5;
    value = (u32)value >> 31;
    *(s32 *)(base + 0x34) = temp;
    temp += value;
    value = *(s32 *)(base + 0x38);
    temp >>= 1;
    value -= temp;
    *(s32 *)(base + 0x38) = value;
    if (value >= 0x2BC) {
        *(s32 *)(base + 0x38) = 0x2BC;
        return;
    }
    temp = value < -0x2BB;
    if (temp != 0) {
        temp = -0x2BC;
        *(s32 *)(base + 0x38) = temp;
        return;
    }
    return;

    }
    value = *(s32 *)(base + 0x38);
    if (value != 0) {
        temp = (value * 16) - value;
        temp <<= 1;
        if (temp < 0) {
            temp += 0x1F;
        }
        temp >>= 5;
        *(s32 *)(base + 0x38) = temp;
        if (temp == 0) {
            *(s16 *)(base + 0x7E) = 0;
        }
    }
}

void ApplyCarRacingLineHint(GameCarRuntime *obj, s32 arg1) asm("func_800394DC");
void ApplyCarRacingLineHint(GameCarRuntime *obj, s32 arg1) {
    GameCarRuntime *objReg = obj;
    s32 target;
    u8 *state;
    s32 index;
    s32 advanceOffset;
    s32 scene;
    register s32 offset asm("$3");
    register u8 *base asm("$2");
    s16 *entry;
    s32 value;
    s32 valueRaw;
    register s32 raw asm("$2");
    s32 stack[2];

    raw = objReg->trackProgress;
    asm volatile("" : : "r"(stack));
    scene = g_RaceSeries;
    target = raw >> 4;
    index = objReg->routeIndex;
    raw = (scene * 2) + scene;
    offset = (raw << 4) - raw;
    offset <<= 3;
    base = g_TrackEventData;
    offset += 0x84;
    entry = (s16 *)(offset + (s32)base);
    raw = (index * 2) + index;
    raw <<= 2;
    entry = (s16 *)((s32)entry + raw);

    if (target < 0x20) {
        state = (u8 *)&objReg->field_BC;
        objReg->routeIndex = 0;
        target = 0;
    } else {
        state = (u8 *)&objReg->field_BC;
    }

    if (target < entry[0]) {
    } else {
    if (entry[1] < target) {
        goto advance;
    }
    if (arg1 < 4 && objReg->field_10C == 0) {
        valueRaw = objReg->field_11C;
        if (entry[2] < valueRaw) {
            value = valueRaw;
            asm volatile("" : "=r"(valueRaw) : "0"(valueRaw));
            raw = entry[3];
            raw = valueRaw < raw;
            if (raw != 0) {
                raw = value + *(u16 *)(entry + 4);
                objReg->field_11C = raw;
            }
        }
    }
    return;

    }
    if (!(entry[1] < target)) {

    } else {
advance:
    {
        register s32 next asm("$2");

        next = *(s32 *)(state + 0x44);
        scene = g_RaceSeries;
        next++;
        advanceOffset = (next * 2) + next;
        advanceOffset <<= 2;
        *(s32 *)(state + 0x44) = next;
    }
    raw = (scene * 2) + scene;
    offset = (raw << 4) - raw;
    offset <<= 3;
    base = g_TrackEventData;
    advanceOffset += offset;
    raw = (s32)(base + advanceOffset);
    if (*(s16 *)(raw + 0x84) == -1) {
        *(s32 *)(state + 0x44) = 0;
    }
    *(s16 *)(state + 0x52) = 0;
    return;

    }
    objReg->field_10E = 0;
}

void SeedCarRouteMarkers(void) asm("func_80039644");
void SeedCarRouteMarkers(void) {
    s32 one = 1;
    register s32 offset asm("a2") = 0;
    register u8 *base asm("t0");
    s32 scene;
    s32 product;
    s32 baseOffset;
    s32 index;
    s32 tableOffset;
    s32 raw;
    s32 target;
    s32 value;

    scene = g_RaceSeries;
    base = g_TrackEventData;
    product = (scene * 8) + scene;
    baseOffset = product * 64;

outer:
    __asm__ volatile("" ::: "memory");
    index = 0;
    raw = *(s32 *)(g_CarTrackProgress + offset);
    tableOffset = baseOffset;
    *(s16 *)(g_CarMarkerFlag + offset) = one;
    target = raw >> 4;

inner:
    value = *(s16 *)(base + tableOffset + 0x474);
    if (target >= value) {
        *(s16 *)(g_CarMarkerIndex + offset) = index;
        offset += 0x19C;
    } else {
    if (value == -1) {
        *(s16 *)(g_CarMarkerIndex + offset) = 0;
        offset += 0x19C;
    } else {
    __asm__ volatile("" ::: "memory");
    index++;
    if (index < 0x30) {
        tableOffset += 0xC;
        goto inner;
    }
    offset += 0x19C;

    }
    }
    if (offset < 0x11B4) {
        goto outer;
    }
}

void UpdateCarAiTargetSpeed(u8 *car, s32 gear) asm("func_800396FC");
void UpdateCarAiTargetSpeed(u8 *car, s32 gear) {
  u8 *p[2];
  u16 lim[4];
  u16 val[2];
  register u8 *sub_R9 asm("$9");
  s32 rpm;
  s32 g0;
  s32 raw;
  u8 *tbl;
  s32 f;
  s32 lo_R7;
  s32 hi;
  s32 range;
  register s32 d_R3 asm("$3");
  s32 pitch;
  s32 q;
  s32 v20_R4;
  s32 cnt;
  s32 one;
  int new_var;
  raw = *((s32 *) (car + 0x70));
  rpm = raw >> 4;
  g0 = *((s16 *) (car + 0x138));
  sub_R9 = car + 0xBC;
  if (rpm < 0x20)
  {
    *((s16 *) (car + 0x138)) = 0;
  }
  if (g0 < 0)
  {
    *((s16 *) (car + 0x138)) = 0;
  }
  tbl = g_TrackEventData + ((g_RaceSeries * 576) + 0x474);
  p[0] = tbl + (g0 * 12);
  p[1] = tbl + ((g0 * 12) + 12);
  lim[0] = *((u16 *) p[0]);
  lim[1] = *((u16 *) p[1]);
  if (gear < 4)
  {
    val[0] = *((u16 *) ((p[0] + (gear * 2)) + 4));
    val[1] = *((u16 *) ((p[1] + (gear * 2)) + 4));
  }
  else
  {
    f = 0x55 - gear;
    val[0] = ((*((s16 *) (p[0] + 0xA))) * f) / 100;
    val[1] = ((*((s16 *) (p[1] + 0xA))) * f) / 100;
  }
  pitch = 0;
  lo_R7 = *((s16 *) (&lim[0]));
  switch (0) { default:
  if (rpm < lo_R7)
  {
  } else {
  hi = *((s16 *) (&lim[1]));
  one = hi;
  if (one < rpm)
  {
    goto L2_inc;
  }
  range = hi - lo_R7;
  pitch = *((u16 *) (p[0] + 2));
  if (!(range > 0))
  {
    range = 1;
  }
  d_R3 = rpm - lo_R7;
  v20_R4 = (new_var = *((s16 *) (&val[0])));
  q = (((*((s16 *) (&val[1]))) - new_var) * d_R3) / range;
  *((s16 *) (sub_R9 + 0x74)) = ((((new_var + q) * 1168) / 160) * 6) / 100;
  break;
  }
  if ((*((s16 *) (&lim[1]))) < rpm)
  {
    L2_inc:
    cnt = *((u16 *) (sub_R9 + 0x7C));
    d_R3 = 1;
    q = d_R3;
    g0 = q;
    *((s16 *) (sub_R9 + 0x7E)) = g0;
    cnt = cnt + 1;
  }
  else
  {
    cnt = *((u16 *) (sub_R9 + 0x7C));
    one = 1;
    *((s16 *) (sub_R9 + 0x7E)) = one;
    cnt = cnt - 1;
  }

  *((u16 *) (sub_R9 + 0x7C)) = cnt;
  if (rpm < 0x20)
  {
    *((u16 *) (sub_R9 + 0x7C)) = 0;
  }
  }
  if ((*((s16 *) (sub_R9 + 0x7E))) != 0)
  {
    UpdateCarSlideAngle((GameCarRuntime *)car, (s16) pitch);
  }

}

s32 CollideRivalCars(GameCarRuntime *car, s32 index) {
    s16 rotation[4];
    s32 transformed[3];
    Matrix matrix;
    s16 velocityDelta[2];
    GameCollisionPointBytes quads[4][4];
    GameCollisionPoint samples[5];
    GameCollisionPoint carCorners[4];
    GameCollisionPoint otherCorners[4];
    GameCarRuntime *other;
    s32 nextIndex;
    s32 carProgress;
    s32 carField34;
    s32 hit;
    s32 corner;
    s32 offset;
    s32 quadIndex;
    s32 distance;
    s32 progressDelta;
    s32 average01X;
    s32 average01Z;
    s32 average23X;
    s32 average23Z;
    u32 average02X;
    u32 average02Z;
    u32 average13X;
    u32 average13Z;
    u32 centerX;
    u32 centerZ;

    other = (GameCarRuntime *)((index * sizeof(GameCarRuntime)) + (s32)&g_Cars[1]);
    nextIndex = index + 1;
    hit = 0;
    carProgress = car->trackProgress;
    carField34 = car->field_34;

    while (nextIndex < 11) {
        if (other->activeFlag != -1 && other->field_98 == car->field_98) {
            progressDelta =
                (other->trackProgress + g_TrackLength - carProgress) % g_TrackLength;
            distance = other->field_34 - carField34;
            if (distance < 0) {
                distance = -distance;
            }
            if (distance < 100 &&
                (progressDelta < 200 || g_TrackLength - 200 < progressDelta)) {
                velocityDelta[0] = (u16)other->x - (u16)car->x;
                velocityDelta[1] = (u16)other->z - (u16)car->z;

                rotation[0] = (u16)car->field_20;
                rotation[2] = (u16)car->field_28;
                rotation[1] = (u16)car->field_24;
                RotMatrix(rotation, &matrix);
                SetRotMatrix(&matrix);

                for (corner = 0, offset = 0; corner < 4; corner++, offset++) {
                    rotation[0] = g_CarCollisionCorners[offset].x;
                    rotation[2] = g_CarCollisionCorners[offset].z;
                    rotation[1] = 0;
                    TransformCollisionVector((s32 *)rotation, transformed);
                    carCorners[offset].x = transformed[0] >> 2;
                    carCorners[offset].z = transformed[2] >> 2;
                    quads[corner][offset] =
                        *(GameCollisionPointBytes *)&carCorners[offset];
                }

                average02X = carCorners[0].x + carCorners[2].x;
                average02X += average02X >> 31;
                average02X >>= 1;
                average02Z = carCorners[0].z + carCorners[2].z;
                average02Z += average02Z >> 31;
                average02Z >>= 1;
                average13X = carCorners[1].x + carCorners[3].x;
                average13X += average13X >> 31;
                average13X >>= 1;
                average13Z = carCorners[1].z + carCorners[3].z;
                average13Z += average13Z >> 31;
                average13Z >>= 1;

                average01X = carCorners[0].x + carCorners[1].x;
                average01X += (u32)average01X >> 31;
                average01X >>= 1;
                average23X = carCorners[2].x + carCorners[3].x;
                average23X /= 2;
                centerX = (s16)average01X + (s16)average23X;
                centerX += centerX >> 31;
                centerX >>= 1;

                average01Z = carCorners[0].z + carCorners[1].z;
                average01Z += (u32)average01Z >> 31;
                average01Z >>= 1;
                average23Z = carCorners[2].z + carCorners[3].z;
                average23Z /= 2;
                centerZ = (s16)average01Z + (s16)average23Z;
                centerZ += centerZ >> 31;
                centerZ >>= 1;

                ((GameCollisionPoint *)&quads[1][0])->x = average01X;
                ((GameCollisionPoint *)&quads[0][1])->x = average01X;
                ((GameCollisionPoint *)&quads[1][0])->z = average01Z;
                ((GameCollisionPoint *)&quads[0][1])->z = average01Z;
                ((GameCollisionPoint *)&quads[2][0])->x = average02X;
                ((GameCollisionPoint *)&quads[0][2])->x = average02X;
                ((GameCollisionPoint *)&quads[2][0])->z = average02Z;
                ((GameCollisionPoint *)&quads[0][2])->z = average02Z;
                ((GameCollisionPoint *)&quads[3][1])->x = average13X;
                ((GameCollisionPoint *)&quads[1][3])->x = average13X;
                ((GameCollisionPoint *)&quads[3][1])->z = average13Z;
                ((GameCollisionPoint *)&quads[1][3])->z = average13Z;
                ((GameCollisionPoint *)&quads[3][2])->x = average23X;
                ((GameCollisionPoint *)&quads[2][3])->x = average23X;
                ((GameCollisionPoint *)&quads[3][2])->z = average23Z;
                ((GameCollisionPoint *)&quads[2][3])->z = average23Z;
                ((GameCollisionPoint *)&quads[3][0])->x = centerX;
                ((GameCollisionPoint *)&quads[2][1])->x = centerX;
                ((GameCollisionPoint *)&quads[1][2])->x = centerX;
                ((GameCollisionPoint *)&quads[0][3])->x = centerX;
                ((GameCollisionPoint *)&quads[3][0])->z = centerZ;
                ((GameCollisionPoint *)&quads[2][1])->z = centerZ;
                ((GameCollisionPoint *)&quads[1][2])->z = centerZ;
                ((GameCollisionPoint *)&quads[0][3])->z = centerZ;

                rotation[0] = (u16)other->field_20;
                rotation[2] = (u16)other->field_28;
                rotation[1] = (u16)other->field_24;
                RotMatrix(rotation, &matrix);
                SetRotMatrix(&matrix);

                for (corner = 0, offset = 0; corner < 4; corner++, offset += 4) {
                    rotation[0] = g_CarCollisionCorners[corner].x;
                    rotation[2] = g_CarCollisionCorners[corner].z;
                    rotation[1] = 0;
                    TransformCollisionVector((s32 *)rotation, transformed);
                    otherCorners[corner].x =
                        (transformed[0] >> 2) + velocityDelta[0];
                    otherCorners[corner].z =
                        (transformed[2] >> 2) + velocityDelta[1];
                }

                samples[0].x =
                    (otherCorners[0].x + otherCorners[1].x) / 2;
                samples[0].z =
                    (otherCorners[0].z + otherCorners[1].z) / 2;
                samples[1].x =
                    (otherCorners[0].x + otherCorners[2].x) / 2;
                samples[1].z =
                    (otherCorners[0].z + otherCorners[2].z) / 2;
                samples[2].x =
                    (otherCorners[1].x + otherCorners[3].x) / 2;
                samples[2].z =
                    (otherCorners[1].z + otherCorners[3].z) / 2;
                samples[3].x =
                    (otherCorners[2].x + otherCorners[3].x) / 2;
                samples[3].z =
                    (otherCorners[2].z + otherCorners[3].z) / 2;
                samples[4].x =
                    (samples[0].x + samples[2].x) / 2;
                samples[4].z =
                    (samples[0].z + samples[2].z) / 2;

                corner = 0;
                do {
                    quadIndex = 0;
                    do {
                        hit = IsPointInQuad(
                            *(s32 *)&quads[quadIndex][2],
                            *(s32 *)&quads[quadIndex][3],
                            *(s32 *)&quads[quadIndex][0],
                            *(s32 *)&quads[quadIndex][1],
                            *(s32 *)&otherCorners[corner]);
                        if (hit > 0) {
                            hit = quadIndex + 1;
                            break;
                        }
                        quadIndex++;
                    } while (quadIndex < 4);
                    if (hit > 0) {
                        break;
                    }
                    corner++;
                } while (corner < 4);

                if (hit <= 0) {
                    corner = 0;
                    do {
                        quadIndex = 0;
                        do {
                            hit = IsPointInQuad(
                                *(s32 *)&quads[quadIndex][2],
                                *(s32 *)&quads[quadIndex][3],
                                *(s32 *)&quads[quadIndex][0],
                                *(s32 *)&quads[quadIndex][1],
                                *(s32 *)&samples[corner]);
                            if (hit > 0) {
                                hit = quadIndex + 1;
                                break;
                            }
                            quadIndex++;
                        } while (quadIndex < 4);
                        if (hit > 0) {
                            break;
                        }
                        corner++;
                    } while (corner < 5);
                }
                if (hit > 0) {
                    break;
                }
            }
        }
        other++;
        nextIndex++;
    }

    if (hit > 0) {
        if (hit < 3) {
            s32 deltaX;
            s32 deltaZ;

            deltaX =
                (s16)((u16)other->field_C8 - (u16)car->field_C8);
            if (deltaX < 0) {
                deltaX += 31;
            }
            velocityDelta[0] = deltaX >> 5;
            deltaZ =
                (s16)((u16)other->field_D0 - (u16)car->field_D0);
            if (deltaZ < 0) {
                deltaZ += 31;
            }
            velocityDelta[1] = deltaZ >> 5;
            SetCarKnockback(car, 0, 0, 4);
            SetCarKnockback(
                other, velocityDelta[0], velocityDelta[1], 4);
            car->field_8A = 1;
            car->field_A8 = (car->field_A8 * 90) / 100;
            other->field_8A = 1;
        } else {
            s32 deltaX;
            s32 deltaZ;

            deltaX =
                (s16)((u16)other->field_C8 - (u16)car->field_C8);
            if (deltaX < 0) {
                deltaX += 31;
            }
            velocityDelta[0] = deltaX >> 5;
            deltaZ =
                (s16)((u16)other->field_D0 - (u16)car->field_D0);
            if (deltaZ < 0) {
                deltaZ += 31;
            }
            velocityDelta[1] = deltaZ >> 5;
            SetCarKnockback(
                car, -velocityDelta[0], -velocityDelta[1], 4);
            SetCarKnockback(other, 0, 0, 4);
            other->field_A8 = (other->field_A8 * 90) / 100;
            car->field_8A = 1;
            other->field_8A = 1;
        }
    }
    return hit;
}
