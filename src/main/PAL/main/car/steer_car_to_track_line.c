#include "common.h"
#include "game/vector.h"
#include "game/car.h"
#include "game/track.h"
#include "game/render.h"
#include "game/audio.h"
#include "game/random.h"



/*
 * AI route steering: projects a target point ahead of (or behind, per the
 * direction flag at car+0x110) the car on the track centre-line, offset
 * laterally, and steers the car's heading toward it. The heading correction is
 * divided by the spec block's steerResponse. Register pins are
 * match-load-bearing.
 */
void SteerCarToTrackLine(PlayerCarRuntime *car) {
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
    lateral = car->trackLateralOffset;
    timer = spec->steerResponse;
    directionFlag = car->drive.unk54;

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

    InterpolateTrackPoint(index, coords, car->segmentFraction);
    angle = 0x1000 - SmoothTrackAngle(index, car->segmentFraction);

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

    if (car->shiftState == 0) {
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


void UpdateCarLaunch(PlayerCarRuntime *carArg, s32 unused) {
    register PlayerCarRuntime *car = carArg;
    register GameCarDrive *drive;
    register s32 s4val;
    s32 res;
    register s32 v0 asm("$2");
    register s32 first24 asm("$4");
    register s32 firstHeading asm("$5");
    s32 sinF24;
    s32 cosF24;
    /* The two adjacent motion-state handlers use this same vector at sp+0x10. */
    s32 coords[3];

    first24 = car->bodyYaw;
    v0 = car->drive.unk50;
    firstHeading = car->headingAngle;
    s4val = v0;
    if (v0 < 0) {
        s4val = -s4val;
    }

    res = GetAngleDistance(first24, firstHeading);
    drive = &car->drive;
    if (res >= 0x600) {
        car->speed = car->speed * 990 / 1000;
    }

    if (car->shiftState == 0) {
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
        drive->unk48 -= (0x800 - s4val) * 4000 / 256;
    }
    if (car->speed < 0x190) {
        drive->unk48 -= (0x190 - car->speed) * 100;
    }

    if (drive->unk48 > 0) {
        s32 s2;

        drive->unk3E += 10;
        if (drive->unk3E >= 100) {
            drive->unk3E = 100;
        }

        res = GetAngleDelta(car->bodyYaw, drive->unk90) * 98 / 100;
        s2 = res * (drive->unk4C + 0x800);
        res = s2 / 2048;
        drive->unk50 += res * 16;

        if ((u32)(drive->steerPos + 127) < 255) {
            if (GetAngleDistance(car->bodyYaw, car->headingAngle) < 0x200) {
                drive->unk50 = drive->unk50 * 31 / 32;
                drive->unk50 =
                    GetAngleDelta(car->bodyYaw, car->headingAngle) + drive->unk50;
            } else if (s4val < 0x800) {
                drive->unk50 = res / 2 + drive->unk50;
            }
        }

        if (drive->unk50 > 0x3600) {
            drive->unk50 = 0x3600;
        }
        if (drive->unk50 < -0x3600) {
            drive->unk50 = -0x3600;
        }

        car->bodyYaw = drive->unk50 / 256 + car->bodyYaw;
        drive->unk48 -= 64;

        res = GetAngleDistance(car->bodyYaw, car->headingAngle);
        drive->unk48 -= res * res / 65536;
        drive->unk48 -= (0x3600 - s4val) / 64;

        {
            s32 a4 = car->speed;
            s32 half = drive->unk8C / 2;
            if (a4 < half) {
                drive->unk48 -= (half - a4) / 8;
            }
        }

        drive->unk48 -= drive->brakeBtn * 4;
        drive->unk48 -= (0x100 - drive->accelBtn) * 4;
        car->speed -= drive->brakeBtn * 10 / 256;
        car->speed -= (0x100 - drive->accelBtn) * 10 / 256;
    } else {
        drive->unk50 = drive->unk50 * 15 / 16;
        if (s4val < 0x1000) {
            s32 lo;
            u8 *specBase;

            {
                s32 gain = (100 - (drive->gear - 1) * 4) * 10000;
                drive->unk94 = gain * car->speed / 100;
            }
            drive->unk60 = GetAngleDelta(car->headingAngle, car->bodyYaw);
            drive->unk58 = car->headingAngle;
            car->headingAngle = car->bodyYaw;

            {
                register s32 t;
                register s32 sq asm("$3");

                t = drive->unk60;
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
                drive->unk5C = sq * car->speed / 0x100000;
            }

            drive->unk50 = 0;

            specBase = (u8 *)g_CarSpec;
            lo = car->speed * 0xA0 / 1168 * 10000 /
                 *(s32 *)((u8 *)specBase +
                          (drive->gear << 2) + 0xE4);
            {
                s32 offset;

                asm volatile("" : : : "memory");
                offset = drive->gear;
                firstHeading = (u16)drive->unk78;
                offset <<= 2;
                asm volatile("" : :);
                RAW(drive->unk38) = 0x14;
                RAW(drive->state98) = 2;
                g_ShiftTargetRpm = lo;
                RAW(drive->unk3C) = *(u16 *)&g_ShiftTargetRpm - firstHeading;
                specBase += offset;
            }
            {
                drive->unk2C =
                    lo * ((GameCarSpec *)specBase)->gearLoad[0] / 0x20000;
                if (drive->manual == 0) {
                    drive->unk2C =
                        drive->unk2C * 985 / 1000;
                }
            }

            if ((u32)(((u16)drive->unk3C + 99) & 0xFFFF) < 199) {
                g_ShiftSoundLevel = 1;
            } else {
                g_ShiftSoundLevel = 0;
            }
        }
    }

    drive->unk90 = car->bodyYaw;
    SteerCarToTrackLine(car);

    res = GetAngleDistance(car->bodyYaw, car->headingAngle);
    if (res >= 0x401) {
        s32 factor;
        s32 a8;

        factor = (0x3600 - s4val) * 4;
        a8 = car->acceleration;
        factor = factor * a8;
        factor = factor * (res - 0x400);
        car->acceleration = a8 / 2 + factor / 14155776;
    } else {
        s32 a8 = car->acceleration;

        car->acceleration = (0x200 - res) * a8 / 512;
    }

    {
        s32 saved = car->headingAngle;
        AdvanceCarPosition(car);
        car->headingAngle = saved;
    }

    sinF24 = rsin(car->bodyYaw);
    cosF24 = rcos(car->bodyYaw);
    drive->accelPos = rsin(car->headingAngle) * car->speed / 256;
    drive->brakePos = rcos(car->headingAngle) * car->speed / 256;
}

/*
 * Car motion handler for state98 == 2 (airborne / jump): decays velocity and
 * spin, advances the car (AdvanceCarPosition), and lands it when it returns to the
 * ground. The drive sub-block is the GameCarDrive view of car->field_BC.
 */
void UpdateCarAirborne(PlayerCarRuntime *car, s32 unused) {
    GameCarDrive *r = &car->drive;
    s32 sinF24;
    s32 cosF24;
    volatile s32 coords[3];
    s32 flag = g_ShiftSoundLevel;

    if (flag == 0) {
        s32 phase;
        s32 f11c = car->drive.unk60;
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
        s32 rr = GetAngleDelta(car->bodyYaw, r->unk90);
        s32 base = car->bodyYaw;
        car->bodyYaw = rr / 5 + base;
        AdvanceCarPosition(car, base);
    }

    sinF24 = rsin(car->bodyYaw);
    cosF24 = rcos(car->bodyYaw);

    r->accelPos = rsin(car->headingAngle + r->unk60) * car->speed / 256;
    r->brakePos = rcos(car->headingAngle + r->unk60) * car->speed / 256;

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

    r->unk3E = r->unk3E * 2 / 3;
    if (r->unk60 >= 1537) {
        car->speed = car->speed * 4 / 5;
    }

    if (r->unk38 <= 0) {
        SetIndexedEffectVoice(-1, 0, 0);
        car->bodyYaw -= r->unk50;
        g_ShiftSoundLevel = 0;
        r->unk3C = 0;
        r->unk60 = 0;
        r->unk5C = 0;
        r->state98 = 0;
        r->unk3E = 0;
    }
}

/*
 * Car motion handler for state98 == 3 (crash / tumble): applies a random shake
 * (Random15) scaled by the remaining shake budget g_StandingStartSpin, advances the
 * car (AdvanceCarPosition), and resets the car once the budget expires. field_15C /
 * field_15E hold the shake magnitude.
 */
void UpdateCarStandingStart(PlayerCarRuntime *car, s32 unused) {
    GameCarDrive *route = &car->drive;
    s32 sinA;
    s32 cosA;
    s32 base;
    s32 r;
    s32 coords[3];

    r = GetAngleDelta(car->bodyYaw, route->unk90);
    base = car->bodyYaw;
    car->bodyYaw = r / 5 + base;
    AdvanceCarPosition(car, base);

    sinA = rsin(car->bodyYaw);
    cosA = rcos(car->bodyYaw);

    route->accelPos = rsin(car->headingAngle) * car->speed / 256;
    route->brakePos = rcos(car->headingAngle) * car->speed / 256;

    coords[0] = (cosA * route->accelPos - sinA * route->brakePos) / 4096;
    coords[2] = (sinA * route->accelPos + cosA * route->brakePos) / 4096;
    route->accelPos = sinA * coords[2] / 16384;
    route->brakePos = cosA * coords[2] / 16384;

    SetIndexedEffectVoice(0, 0x1A80,
                          (0x60 - (g_StandingStartSpin & 0x1F) * 2) *
                              route->accelBtn / 256);

    car->speed = car->speed / 10;

    if (g_StandingStartSpin >= 11) {
        s32 f15c = route->accelBtn;
        s32 f134 = route->unk78;

        sinA = f15c + 32;
        g_StandingStartSpin -= route->brakeBtn * 2;
        if (f134 < 2000) {
            sinA = f15c + 1032;
        }
        if (f15c < 127 && f134 >= 2001) {
            sinA += 127;
        }
        route->unk68 = (Random15() & 3) * sinA / 256;
        route->unk6C = (Random15() & 7) * sinA / 256;
        g_StandingStartSpin -= sinA;
        if (g_StandingStartSpin <= 0) {
            route->unk68 = 0;
            route->unk6C = 0;
            route->state98 = 0;
            SetIndexedEffectVoice(-1, 0, 0);
        }
    } else {
        SetIndexedEffectVoice(-1, 0, 0);
        route->state98 = 0;
        route->unk68 = 0;
        route->unk6C = 0;
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
