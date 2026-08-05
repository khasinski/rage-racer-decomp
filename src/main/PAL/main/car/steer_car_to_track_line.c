#include "common.h"
#include "game/vector.h"
#include "game/car.h"
#include "game/track.h"
#include "game/render.h"
#include "game/audio.h"
#include "game/random.h"
void AdvanceCarPosition();

void UpdateCarAirborne(GameCarRuntime *car);

s32 rsin(s32 arg0);
s32 rcos(s32 arg0);

/*
 * AI route steering: projects a target point ahead of (or behind, per the
 * direction flag at car+0x110) the car on the track centre-line, offset
 * laterally, and steers the car's heading toward it. The heading correction is
 * divided by the spec block's steerResponse. Register pins are
 * match-load-bearing.
 */
void SteerCarToTrackLine(GameCarRuntime *car) {
    GameCarSpec *spec;
    s32 timer;
    s32 index;
    register s32 lateral asm("$18");
    register s32 baseIndex asm("$4");
    s32 finalAngle;
    s32 coords[3];
    s32 angle;
    s32 value;
    s32 xValue;
    s32 headingDelta;
    s32 rawIndex;
    s32 trackCount;
    s32 directionFlag;
    s32 divisor;

    spec = g_CarSpec;
    lateral = car->field_34;
    timer = spec->steerResponse;
    directionFlag = *(s32 *)((u8 *)car + 0x110);

    asm volatile("" : "=r"(timer) : "0"(timer));
    baseIndex = car->trackPointIndex;
    index = baseIndex + 2;
    if (directionFlag == 0) {
        index = baseIndex - 2;
    }

    rawIndex = index;
    if (index < 0) {
        rawIndex = index + g_TrackPointCount;
    }
    trackCount = g_TrackPointCount;
    index = rawIndex % trackCount;

    InterpolateTrackPoint(index, coords, car->field_38);
    angle = 0x1000 - SmoothTrackAngle(index, car->field_38);

    xValue = rsin(angle) * lateral;
    if (xValue < 0) {
        xValue += 0xFFF;
    }
    coords[0] += xValue >> 12;

    value = rcos(angle) * lateral;
    if (value < 0) {
        value += 0xFFF;
    }
    coords[2] += value >> 12;

    finalAngle = 0x400 - Atan2(coords[0] - car->x, coords[2] - car->z);

    if (car->field_98 == 0) {
        xValue = timer << 16;
        divisor = xValue >> 16;
        if (divisor <= 0) {
            divisor = 1;
        }

        headingDelta = GetAngleDelta(car->headingAngle, finalAngle);
        headingDelta = ((headingDelta * 5) << 2) / divisor;
        car->headingAngle += headingDelta;
    }
}

/*
 * Car motion-state handler for state98 == 1: the one-frame takeoff of a jump.
 * Turns the launch spin UpdateCarDriving seeded into clamped yaw, recomputes revs /
 * tacho / world velocity, then sets route+0x38 = 0x14 and route+0x98 = 2 to hand
 * the car to the airborne handler UpdateCarAirborne. See docs/names.md 1.
 */

s32 GetAngleDistance(s32 arg0, s32 arg1);
s32 GetAngleDelta(s32 arg0, s32 arg1);
s32 rsin(s32 arg0);
s32 rcos(s32 arg0);

void UpdateCarLaunch(GameCarRuntime *arg0) {
    register GameCarRuntime *car = arg0;
    register u8 *r;
    register s32 s4val;
    s32 res;
    register s32 v0 asm("$2");
    register s32 first24 asm("$4");
    register s32 firstHeading asm("$5");
    s32 sinF24;
    s32 cosF24;
    /* The two adjacent motion-state handlers use this same vector at sp+0x10. */
    s32 coords[3];

    first24 = car->field_24;
    v0 = *(s32 *)((u8 *)car + 0x10C);
    firstHeading = car->headingAngle;
    s4val = v0;
    if (v0 < 0) {
        s4val = -s4val;
    }

    res = GetAngleDistance(first24, firstHeading);
    r = (u8 *)car + 188;
    if (res >= 0x600) {
        car->field_A4 = car->field_A4 * 990 / 1000;
    }

    if (car->field_98 == 0) {
        s32 near;
        s32 phase;
        s32 volume;

        volume = 0x7F;
        near = res < 513;
        if (near) {
            volume = res / 8 + 0x40;
            asm("" : "=r"(res) : "0"(res));
            near = res < 513;
        }
        asm("" : "=r"(near) : "0"(near));
        if (near) {
            phase = res * 3 + 0x1800;
        } else {
            phase = 0x1E00;
        }
        SetIndexedEffectVoice(0, phase, volume);
    } else {
        SetIndexedEffectVoice(-1, 0, 0);
    }

    if (res < 0x80 && s4val < 0x800) {
        *(s32 *)(r + 0x48) -= (0x800 - s4val) * 4000 / 256;
    }
    if (car->field_A4 < 0x190) {
        *(s32 *)(r + 0x48) -= (0x190 - car->field_A4) * 100;
    }

    if (*(s32 *)(r + 0x48) > 0) {
        s32 s2;

        *(s16 *)(r + 0x3E) += 10;
        if (*(s16 *)(r + 0x3E) >= 100) {
            *(s16 *)(r + 0x3E) = 100;
        }

        res = GetAngleDelta(car->field_24, *(s32 *)(r + 0x90)) * 98 / 100;
        s2 = res * (*(s32 *)(r + 0x4C) + 0x800);
        res = s2 / 2048;
        *(s32 *)(r + 0x50) += res * 16;

        if ((u32)(*(s32 *)(r + 0x1C) + 127) < 255) {
            if (GetAngleDistance(car->field_24, car->headingAngle) < 0x200) {
                *(s32 *)(r + 0x50) = *(s32 *)(r + 0x50) * 31 / 32;
                *(s32 *)(r + 0x50) =
                    GetAngleDelta(car->field_24, car->headingAngle) + *(s32 *)(r + 0x50);
            } else if (s4val < 0x800) {
                *(s32 *)(r + 0x50) = res / 2 + *(s32 *)(r + 0x50);
            }
        }

        if (*(s32 *)(r + 0x50) > 0x3600) {
            *(s32 *)(r + 0x50) = 0x3600;
        }
        if (*(s32 *)(r + 0x50) < -0x3600) {
            *(s32 *)(r + 0x50) = -0x3600;
        }

        car->field_24 = *(s32 *)(r + 0x50) / 256 + car->field_24;
        *(s32 *)(r + 0x48) -= 64;

        res = GetAngleDistance(car->field_24, car->headingAngle);
        *(s32 *)(r + 0x48) -= res * res / 65536;
        *(s32 *)(r + 0x48) -= (0x3600 - s4val) / 64;

        {
            s32 a4 = car->field_A4;
            s32 half = *(s32 *)(r + 0x8C) / 2;
            if (a4 < half) {
                *(s32 *)(r + 0x48) -= (half - a4) / 8;
            }
        }

        *(s32 *)(r + 0x48) -= *(s16 *)(r + 0xA2) * 4;
        *(s32 *)(r + 0x48) -= (0x100 - *(s16 *)(r + 0xA0)) * 4;
        car->field_A4 -= *(s16 *)(r + 0xA2) * 10 / 256;
        car->field_A4 -= (0x100 - *(s16 *)(r + 0xA0)) * 10 / 256;
    } else {
        *(s32 *)(r + 0x50) = *(s32 *)(r + 0x50) * 15 / 16;
        if (s4val < 0x1000) {
            s32 lo;
            u8 *specBase;

            {
                s32 gain = (100 - (*(s16 *)(r + 0x76) - 1) * 4) * 10000;
                *(s32 *)(r + 0x94) = gain * car->field_A4 / 100;
            }
            *(s32 *)(r + 0x60) = GetAngleDelta(car->headingAngle, car->field_24);
            *(s32 *)(r + 0x58) = car->headingAngle;
            car->headingAngle = car->field_24;

            {
                register s32 t;
                register s32 sq asm("$3");

                t = *(s32 *)(r + 0x60);
                sq = t;
                if (t < 0) {
                    sq = -sq;
                }
                if (sq < 0x401) {
                    sq = sq * sq;
                } else {
                    sq = 0x800 - sq;
                    sq = sq * sq;
                }
                *(s32 *)(r + 0x5C) = sq * car->field_A4 / 0x100000;
            }

            *(s32 *)(r + 0x50) = 0;

            specBase = (u8 *)g_CarSpec;
            lo = car->field_A4 * 0xA0 / 1168 * 10000 /
                 *(s32 *)((u8 *)specBase +
                          (*(s16 *)(r + 0x76) << 2) + 0xE4);
            {
                register s32 phaseValue asm("$5");
                s32 offset;

                asm volatile("" : : : "memory");
                offset = *(s16 *)(r + 0x76);
                phaseValue = *(u16 *)(r + 0x78);
                offset <<= 2;
                asm volatile("" : : : "memory");
                *(s16 *)(r + 0x38) = 0x14;
                *(s32 *)(r + 0x98) = 2;
                g_ShiftTargetRpm = lo;
                *(s16 *)(r + 0x3C) =
                    *(u16 *)&g_ShiftTargetRpm - phaseValue;
                specBase += offset;
            }
            {
                *(s16 *)(r + 0x2C) =
                    lo * *(s32 *)(specBase + 0xCC) / 0x20000;
                if (*(s16 *)(r + 0x74) == 0) {
                    *(s16 *)(r + 0x2C) =
                        *(s16 *)(r + 0x2C) * 985 / 1000;
                }
            }

            if ((u32)((*(u16 *)(r + 0x3C) + 99) & 0xFFFF) < 199) {
                g_ShiftSoundLevel = 1;
            } else {
                g_ShiftSoundLevel = 0;
            }
        }
    }

    *(s32 *)(r + 0x90) = car->field_24;
    SteerCarToTrackLine(car);

    res = GetAngleDistance(car->field_24, car->headingAngle);
    if (res >= 0x401) {
        s32 factor;
        s32 a8;

        factor = (0x3600 - s4val) * 4;
        a8 = car->field_A8;
        factor = factor * a8;
        factor = factor * (res - 0x400);
        car->field_A8 = a8 / 2 + factor / 14155776;
    } else {
        s32 a8 = car->field_A8;

        car->field_A8 = (0x200 - res) * a8 / 512;
    }

    {
        s32 saved = car->headingAngle;
        AdvanceCarPosition(car);
        car->headingAngle = saved;
    }

    sinF24 = rsin(car->field_24);
    cosF24 = rcos(car->field_24);
    *(s32 *)(r + 0x08) = rsin(car->headingAngle) * car->field_A4 / 256;
    *(s32 *)(r + 0x10) = rcos(car->headingAngle) * car->field_A4 / 256;
}

/*
 * Car motion handler for state98 == 2 (airborne / jump): decays velocity and
 * spin, advances the car (AdvanceCarPosition), and lands it when it returns to the
 * ground. The drive sub-block is the GameCarDrive view of car->field_BC.
 */
void UpdateCarAirborne(GameCarRuntime *car) {
    GameCarDrive *r = (GameCarDrive *)&car->field_BC;
    s32 sinF24;
    s32 cosF24;
    volatile s32 coords[3];
    s32 flag = g_ShiftSoundLevel;

    if (flag == 0) {
        s32 phase;
        s32 f11c = *(s32 *)((u8 *)car + 0x11C);
        if (f11c < 513) {
            phase = f11c * 3 + 6144;
        } else {
            phase = 0x1e00;
        }
        SetIndexedEffectVoice(0, phase, r->unk38 * 2 + 80);
    } else {
        SetIndexedEffectVoice(0, 0x1800, flag + 25);
    }

    {
        s32 rr = GetAngleDelta(car->field_24, r->unk90);
        s32 base = car->field_24;
        car->field_24 = rr / 5 + base;
        AdvanceCarPosition(car, base);
    }

    sinF24 = rsin(car->field_24);
    cosF24 = rcos(car->field_24);

    r->accelPos = rsin(car->headingAngle + r->unk60) * car->field_A4 / 256;
    r->brakePos = rcos(car->headingAngle + r->unk60) * car->field_A4 / 256;

    coords[0] = (cosF24 * r->accelPos - sinF24 * r->brakePos) / 4096;
    coords[2] = (sinF24 * r->accelPos + cosF24 * r->brakePos) / 4096;

    r->accelPos =
        rsin(r->unk58) * r->unk5C / 256 + sinF24 * coords[2] / 4096;
    r->brakePos =
        rcos(r->unk58) * r->unk5C / 256 + cosF24 * coords[2] / 4096;

    if (r->unk9C != 1 && r->unk9E != 1 && r->accelBtn < 128) {
        r->unk44 += 1;
    } else {
        r->unk44 = 0;
    }

    r->unk50 = r->unk50 * 31 / 32;
    r->unk5C = r->unk5C * 31 / 32;
    r->unk60 = r->unk60 * 31 / 32;

    *(s16 *)&r->unk3E = *(s16 *)&r->unk3E * 2 / 3;
    if (r->unk60 >= 1537) {
        car->field_A4 = car->field_A4 * 4 / 5;
    }

    if (r->unk38 <= 0) {
        SetIndexedEffectVoice(-1, 0, 0);
        car->field_24 -= r->unk50;
        g_ShiftSoundLevel = 0;
        r->unk3C = 0;
        r->unk60 = 0;
        r->unk5C = 0;
        r->state98 = 0;
        *(s16 *)&r->unk3E = 0;
    }
}

/*
 * Car motion handler for state98 == 3 (crash / tumble): applies a random shake
 * (Random15) scaled by the remaining shake budget g_StandingStartSpin, advances the
 * car (AdvanceCarPosition), and resets the car once the budget expires. field_15C /
 * field_15E hold the shake magnitude.
 */
void UpdateCarStandingStart(GameCarRuntime *car) {
    GameCarRuntime *route = (GameCarRuntime *)&car->field_BC;
    s32 sinA;
    s32 cosA;
    s32 base;
    s32 r;
    s32 coords[3];

    r = GetAngleDelta(car->field_24, *(s32 *)&car->field_14C);
    base = car->field_24;
    car->field_24 = r / 5 + base;
    AdvanceCarPosition(car, base);

    sinA = rsin(car->field_24);
    cosA = rcos(car->field_24);

    car->field_C4 = rsin(car->headingAngle) * car->field_A4 / 256;
    car->field_CC = rcos(car->headingAngle) * car->field_A4 / 256;

    coords[0] = (cosA * car->field_C4 - sinA * car->field_CC) / 4096;
    coords[2] = (sinA * car->field_C4 + cosA * car->field_CC) / 4096;
    car->field_C4 = sinA * coords[2] / 16384;
    car->field_CC = cosA * coords[2] / 16384;

    SetIndexedEffectVoice(0, 0x1A80, (0x60 - (g_StandingStartSpin & 0x1F) * 2) * car->field_15C / 256);

    car->field_A4 = car->field_A4 / 10;

    if (g_StandingStartSpin >= 11) {
        s32 f15c = car->field_15C;
        s32 f134 = car->field_134;

        sinA = f15c + 32;
        g_StandingStartSpin -= car->field_15E * 2;
        if (f134 < 2000) {
            sinA = f15c + 1032;
        }
        if (f15c < 127 && f134 >= 2001) {
            sinA += 127;
        }
        route->field_68 = (Random15() & 3) * sinA / 256;
        route->field_6C = (Random15() & 7) * sinA / 256;
        g_StandingStartSpin -= sinA;
        if (g_StandingStartSpin <= 0) {
            route->field_68 = 0;
            route->field_6C = 0;
            *(s32 *)&route->field_98 = 0;
            SetIndexedEffectVoice(-1, 0, 0);
        }
    } else {
        SetIndexedEffectVoice(-1, 0, 0);
        *(s32 *)((u8 *)car + 0x154) = 0;
        *(s32 *)&car->field_124 = 0;
        *(s32 *)&car->field_128 = 0;
    }
}

/*
 * Finds the track segment whose (rotated, half-width) quad currently contains
 * the car. Starting at `idx` it spirals outward over neighbouring segments
 * (k alternately added/subtracted), and for each builds the segment quad from
 * the two endpoints' angle + left/right half-widths (field_10/field_12) and
 * runs four half-plane cross-product tests (NormalClip). Returns the
 * containing segment index, or -1 (snapping the car onto the track) if none.
 * pts[0] is the car-relative point; pts[1..4] are the quad corners.
 */
s32 FindTrackSegment(GameCarRuntime *car, s32 idx) {
    DVec pts[5];
    s32 i;
    s32 k;
    s32 nxt;
    s32 ni;
    s32 carx;
    s32 carz;
    s32 sx;
    s32 sz;
    s32 cos_c;
    s32 sin_c;
    s32 cos_n;
    s32 sin_n;
    s32 pax;
    s32 paz;
    s32 f10a;
    s32 f12a;
    s32 f10b;
    s32 f12b;
    GameTrackPoint *pa;
    GameTrackPoint *pb;

    k = 0;
    carx = car->x;
    carz = car->z;
    i = idx;

    do {
        nxt = (i + 1) % g_TrackPointCount;
        pa = &g_TrackPoints[i];
        pb = &g_TrackPoints[nxt];

        pax = pa->x;
        paz = pa->z;
        sx = pb->x - pax;
        sz = pb->z - paz;
        pts[0].vx = carx - pax;
        pts[0].vy = carz - paz;

        cos_c = rcos(0xC00 - pa->angle);
        sin_c = rsin(0xC00 - pa->angle);
        cos_n = rcos(0xC00 - pb->angle);
        sin_n = rsin(0xC00 - pb->angle);

        f10a = pa->field_10;
        f12a = pa->field_12;
        f12b = pb->field_12;
        f10b = pb->field_10;

        pts[1].vx =  (s16)(f10a * 2) * (s16)cos_c / 4096;
        pts[1].vy = -(s16)(f10a * 2) * (s16)sin_c / 4096;
        pts[2].vx = -(s16)(f12a * 2) * (s16)cos_c / 4096;
        pts[2].vy =  (s16)(f12a * 2) * (s16)sin_c / 4096;
        pts[3].vx = sx + (s16)(f10b * 2) * (s16)cos_n / 4096;
        pts[3].vy = sz - (s16)(f10b * 2) * (s16)sin_n / 4096;
        pts[4].vx = sx - (s16)(f12b * 2) * (s16)cos_n / 4096;
        pts[4].vy = sz + (s16)(f12b * 2) * (s16)sin_n / 4096;

        if (NormalClip(*(s32 *)&pts[1], *(s32 *)&pts[2], *(s32 *)&pts[0]) >= 0 &&
            NormalClip(*(s32 *)&pts[2], *(s32 *)&pts[4], *(s32 *)&pts[0]) >= 0 &&
            NormalClip(*(s32 *)&pts[4], *(s32 *)&pts[3], *(s32 *)&pts[0]) > 0 &&
            NormalClip(*(s32 *)&pts[3], *(s32 *)&pts[1], *(s32 *)&pts[0]) >= 0) {
            return i;
        }

        k++;
        if (k % 2) {
            i += k;
        } else {
            i -= k;
        }
        if (i >= 0) {
            ni = i % g_TrackPointCount;
        } else {
            ni = (i + g_TrackPointCount) % g_TrackPointCount;
        }
        i = ni;
    } while (i != idx);

    car->x = g_TrackPoints[i].x;
    car->z = g_TrackPoints[i].z;
    i = -1;
    asm volatile("" : "=r"(i) : "0"(i));
    return i;
}
