#include "common.h"
#include "game/car.h"
#include "game/track.h"
#include "game/render.h"

void GameUpdateCarAirborne(GameCarRuntime *car) asm("func_80030814");

void func_8002FC84(s32 arg0, s32 *out, s32 weight);
s32 func_8002FD9C(s32 arg0, s32 arg1);
s32 func_80068568(s32 arg0);
s32 func_80068634(s32 arg0);

/*
 * AI route steering: projects a target point ahead of (or behind, per the
 * direction flag at car+0x110) the car on the track centre-line, offset
 * laterally, and steers the car's heading toward it. The heading correction is
 * divided by the spec block's steerResponse. Register pins are
 * match-load-bearing.
 */
void GameSteerCarToTrackLine(GameCarRuntime *car) asm("func_8002FE74");
void GameSteerCarToTrackLine(GameCarRuntime *car) {
    register GameCarSpec *spec asm("$2");
    register s32 timer asm("$19");
    s32 index;
    register s32 lateral asm("$18");
    register s32 baseIndex asm("$4");
    s32 finalAngle;
    s32 coords[3];
    s32 angle;
    s32 value;
    register s32 xValue asm("$2");
    register s32 headingDelta asm("$3");
    s32 rawIndex;
    s32 trackCount;
    s32 directionFlag;
    register s32 divisor asm("$16");

    spec = g_CarSpec;
    lateral = car->field_34;
    timer = spec->steerResponse;
    asm volatile("" : "=r"(timer) : "0"(timer));

    directionFlag = *(s32 *)((u8 *)car + 0x110);
    asm volatile("" : "=r"(directionFlag) : "0"(directionFlag));
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

    func_8002FC84(index, coords, car->field_38);
    angle = 0x1000 - func_8002FD9C(index, car->field_38);

    xValue = func_80068568(angle) * lateral;
    if (xValue < 0) {
        xValue += 0xFFF;
    }
    coords[0] += xValue >> 12;

    value = func_80068634(angle) * lateral;
    if (value < 0) {
        value += 0xFFF;
    }
    coords[2] += value >> 12;

    finalAngle = 0x400 - GameAtan2(coords[0] - car->x, coords[2] - car->z);

    if (car->field_98 == 0) {
        xValue = timer << 16;
        divisor = xValue >> 16;
        if (divisor <= 0) {
            divisor = 1;
        }

        headingDelta = GameGetAngleDelta(car->headingAngle, finalAngle);
        headingDelta = ((headingDelta * 5) << 2) / divisor;
        car->headingAngle += headingDelta;
    }
}

/*
 * Car motion-state handler for state98 == 1: the one-frame takeoff of a jump.
 * Turns the launch spin GameUpdateCarDriving seeded into clamped yaw, recomputes revs /
 * tacho / world velocity, then sets route+0x38 = 0x14 and route+0x98 = 2 to hand
 * the car to the airborne handler GameUpdateCarAirborne. See docs/names.md 1.
 *
 * PERMUTER-CANDIDATE: real C exists (scratch/permuter-seeds/func_80030030.wip.c,
 * DIFFS=68), 505/505 words, structure correct. Residual is s0/s2 CSE +
 * scheduling. Byte-exact INCLUDE_ASM until permuter.
 */

INCLUDE_ASM("asm/nonmatchings/PAL/main", func_80030030);

extern s32 D_801E8AA0;

void func_8002F4E4(GameCarRuntime *car, s32 arg1);
void func_8005C104(s32 index, s32 phase, s32 volume);

/*
 * Car motion handler for state98 == 2 (airborne / jump): decays velocity and
 * spin, advances the car (func_8002F4E4), and lands it when it returns to the
 * ground. The drive sub-block is the GameCarDrive view of car->field_BC.
 */
void GameUpdateCarAirborne(GameCarRuntime *car) asm("func_80030814");
void GameUpdateCarAirborne(GameCarRuntime *car) {
    GameCarDrive *r = (GameCarDrive *)&car->field_BC;
    s32 sinF24;
    s32 cosF24;
    volatile s32 coords[3];
    s32 flag = D_801E8AA0;

    if (flag == 0) {
        s32 phase;
        s32 f11c = *(s32 *)((u8 *)car + 0x11C);
        if (f11c < 513) {
            phase = f11c * 3 + 6144;
        } else {
            phase = 0x1e00;
        }
        func_8005C104(0, phase, r->unk38 * 2 + 80);
    } else {
        func_8005C104(0, 0x1800, flag + 25);
    }

    {
        s32 rr = GameGetAngleDelta(car->field_24, r->unk90);
        s32 base = car->field_24;
        car->field_24 = rr / 5 + base;
        func_8002F4E4(car, base);
    }

    sinF24 = func_80068568(car->field_24);
    cosF24 = func_80068634(car->field_24);

    r->accelPos = func_80068568(car->headingAngle + r->unk60) * car->field_A4 / 256;
    r->brakePos = func_80068634(car->headingAngle + r->unk60) * car->field_A4 / 256;

    coords[0] = (cosF24 * r->accelPos - sinF24 * r->brakePos) / 4096;
    coords[2] = (sinF24 * r->accelPos + cosF24 * r->brakePos) / 4096;

    r->accelPos =
        func_80068568(r->unk58) * r->unk5C / 256 + sinF24 * coords[2] / 4096;
    r->brakePos =
        func_80068634(r->unk58) * r->unk5C / 256 + cosF24 * coords[2] / 4096;

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
        func_8005C104(-1, 0, 0);
        car->field_24 -= r->unk50;
        D_801E8AA0 = 0;
        r->unk3C = 0;
        r->unk60 = 0;
        r->unk5C = 0;
        r->state98 = 0;
        *(s16 *)&r->unk3E = 0;
    }
}

extern s32 g_StandingStartSpin asm("D_8019CA04");
s32 GameRandom15(void) asm("func_800632B0");

/*
 * Car motion handler for state98 == 3 (crash / tumble): applies a random shake
 * (GameRandom15) scaled by the remaining shake budget g_StandingStartSpin, advances the
 * car (func_8002F4E4), and resets the car once the budget expires. field_15C /
 * field_15E hold the shake magnitude.
 */
void GameUpdateCarStandingStart(GameCarRuntime *car) asm("func_80030BC4");
void GameUpdateCarStandingStart(GameCarRuntime *car) {
    GameCarRuntime *route = (GameCarRuntime *)&car->field_BC;
    s32 sinA;
    s32 cosA;
    s32 base;
    s32 r;
    s32 coords[3];

    r = GameGetAngleDelta(car->field_24, *(s32 *)&car->field_14C);
    base = car->field_24;
    car->field_24 = r / 5 + base;
    func_8002F4E4(car, base);

    sinA = func_80068568(car->field_24);
    cosA = func_80068634(car->field_24);

    car->field_C4 = func_80068568(car->headingAngle) * car->field_A4 / 256;
    car->field_CC = func_80068634(car->headingAngle) * car->field_A4 / 256;

    coords[0] = (cosA * car->field_C4 - sinA * car->field_CC) / 4096;
    coords[2] = (sinA * car->field_C4 + cosA * car->field_CC) / 4096;
    car->field_C4 = sinA * coords[2] / 16384;
    car->field_CC = cosA * coords[2] / 16384;

    func_8005C104(0, 0x1A80, (0x60 - (g_StandingStartSpin & 0x1F) * 2) * car->field_15C / 256);

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
        route->field_68 = (GameRandom15() & 3) * sinA / 256;
        route->field_6C = (GameRandom15() & 7) * sinA / 256;
        g_StandingStartSpin -= sinA;
        if (g_StandingStartSpin <= 0) {
            route->field_68 = 0;
            route->field_6C = 0;
            *(s32 *)&route->field_98 = 0;
            func_8005C104(-1, 0, 0);
        }
    } else {
        func_8005C104(-1, 0, 0);
        *(s32 *)((u8 *)car + 0x154) = 0;
        *(s32 *)&car->field_124 = 0;
        *(s32 *)&car->field_128 = 0;
    }
}

typedef struct { s16 vx; s16 vy; } DVEC;
s32 func_80069C98(s32 arg0, s32 arg1, s32 arg2);

/*
 * Finds the track segment whose (rotated, half-width) quad currently contains
 * the car. Starting at `idx` it spirals outward over neighbouring segments
 * (k alternately added/subtracted), and for each builds the segment quad from
 * the two endpoints' angle + left/right half-widths (field_10/field_12) and
 * runs four half-plane cross-product tests (func_80069C98). Returns the
 * containing segment index, or -1 (snapping the car onto the track) if none.
 * pts[0] is the car-relative point; pts[1..4] are the quad corners.
 */
s32 GameFindTrackSegment(GameCarRuntime *car, s32 idx) asm("func_80030EB4");
s32 GameFindTrackSegment(GameCarRuntime *car, s32 idx) {
    DVEC pts[5];
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

        cos_c = func_80068634(0xC00 - pa->angle);
        sin_c = func_80068568(0xC00 - pa->angle);
        cos_n = func_80068634(0xC00 - pb->angle);
        sin_n = func_80068568(0xC00 - pb->angle);

        f10a = pa->field_10;
        f12a = pa->field_12;
        f12b = pb->field_12;
        f10b = pb->field_10;

        pts[1].vx =  (s16)(f10a << 1) * (s16)cos_c / 4096;
        pts[1].vy = -(s16)(f10a << 1) * (s16)sin_c / 4096;
        pts[2].vx = -(s16)(f12a << 1) * (s16)cos_c / 4096;
        pts[2].vy =  (s16)(f12a << 1) * (s16)sin_c / 4096;
        pts[3].vx = sx + (s16)(f10b << 1) * (s16)cos_n / 4096;
        pts[3].vy = sz - (s16)(f10b << 1) * (s16)sin_n / 4096;
        pts[4].vx = sx - (s16)(f12b << 1) * (s16)cos_n / 4096;
        pts[4].vy = sz + (s16)(f12b << 1) * (s16)sin_n / 4096;

        if (func_80069C98(*(s32 *)&pts[1], *(s32 *)&pts[2], *(s32 *)&pts[0]) >= 0 &&
            func_80069C98(*(s32 *)&pts[2], *(s32 *)&pts[4], *(s32 *)&pts[0]) >= 0 &&
            func_80069C98(*(s32 *)&pts[4], *(s32 *)&pts[3], *(s32 *)&pts[0]) > 0 &&
            func_80069C98(*(s32 *)&pts[3], *(s32 *)&pts[1], *(s32 *)&pts[0]) >= 0) {
            return i;
        }

        k++;
        if (k & 1) {
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
