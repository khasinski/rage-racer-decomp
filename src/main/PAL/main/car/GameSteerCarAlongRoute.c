#include "common.h"
#include "game/car.h"
#include "game/track.h"
#include "game/race.h"
#include "game/render.h"
#include "psyq/gte.h"

void func_8002FC84(s32 arg0, s32 *out, s32 weight);
s32 func_8002FD9C(s32 arg0, s32 arg1);
s32 func_80068568(s32 arg0);
s32 func_80068634(s32 arg0);

typedef struct GameCarVector4 {
    s32 x;
    s32 y;
    s32 z;
    s32 w;
} GameCarVector4;

typedef struct GameCarShortVector {
    s16 vx;
    s16 vy;
    s16 vz;
    s16 pad;
} GameCarShortVector;

extern s32 g_AnimTimer asm("D_8009E694");
extern s16 g_ClosestRivalRank asm("D_801E7740");

void GameRankContenders(void) asm("func_8003A728");
void GameUpdateCarTrafficAvoidance() asm("func_8003A280");
void GameUpdateCarAiTargetSpeed() asm("func_800396FC");
void GameApplyCarRacingLineHint() asm("func_800394DC");
void GameClampCarLateralOffset() asm("func_8003A148");
void GameUpdateRivalRubberBand(void) asm("func_8003A974");
void GameSlowRivalAhead() asm("func_8003A6A4");
void GameAccumulateLapProgress() asm("func_8002C168");
void GameApplyCarKnockback() asm("func_80038C4C");
s32 GameUpdateCarTrackState() asm("func_80031298");
void GameStartCarBodyKick() asm("func_80038F0C");
void GameUpdateCarBodyKick() asm("func_80038FF0");
void GameUpdateCarCrestHop() asm("func_80039280");

/*
 * Car route-steering update. Samples a look-ahead track point (two ahead or two
 * behind depending on the lap-direction flag g_RaceSeries), clamps the lateral
 * offset to the track half-width (field_10/field_12), projects the target point
 * off the centre-line along the inward normal (0x1000 - smoothed track angle),
 * then nudges the car's headingAngle toward that target (GameGetAngleDelta). Writes
 * the steer value into field_44 and the route sub-block (field_BC).
 * Register-pinned locals are match-load-bearing.
 */
void GameSteerCarAlongRoute(GameCarRuntime *car) asm("func_8003AEBC");
void GameSteerCarAlongRoute(GameCarRuntime *car) {
    GameCarRuntime *route;
    GameTrackPoint *point;
    s32 index;
    s32 offset;
    s32 lateral;
    s32 rem;
    s32 coords[3];
    s32 angle;
    /* This pin is load-bearing: removing it changes .text. */
    register s32 value asm("$2");
    s32 zValue;
    s32 lowerLimit;
    /* This pin is load-bearing: removing it changes .text. */
    register s32 callArg asm("$4");

    lateral = car->field_11C;
    offset = car->trackPointIndex;
    route = (GameCarRuntime *)&car->field_BC;
    car->field_DC = 0;

    if (g_RaceSeries != 0) {
        index = offset + 2;
    } else {
        index = offset - 2;
    }

    rem = index;
    if (index < 0) {
        rem = index + g_TrackPointCount;
    }
    index = rem % g_TrackPointCount;

    point = &g_TrackPoints[index];
    if (point->field_12 < lateral) {
        value = point->field_12 * car->field_3C;
        if (value < 0) {
            value += 0x7FF;
        }
        lateral = value >> 11;
    } else {
        value = point->field_10;
        lowerLimit = -value;
        if (lateral < lowerLimit) {
            value = lowerLimit * car->field_3C;
            if (value < 0) {
                value += 0x7FF;
            }
            lateral = value >> 11;
        }
    }

    func_8002FC84(index, coords, car->field_38);
    angle = 0x1000 - func_8002FD9C(index, car->field_38);

    value = func_80068568(angle) * lateral;
    if (value < 0) {
        value += 0xFFF;
    }
    coords[0] += value >> 12;

    zValue = func_80068634(angle) * lateral;
    if (zValue < 0) {
        zValue += 0xFFF;
    }
    coords[2] += zValue >> 12;

    angle = 0x400 - GameAtan2(coords[0] - car->x, coords[2] - car->z);

    callArg = g_RaceSeries;
    value = car->field_B4;
    callArg = (callArg << 11) + 0xC00;
    value = -GameGetAngleDelta(callArg - value, angle);
    car->field_44 = value * 3;

    if (car->field_98 == 0) {
        value = GameGetAngleDelta(car->headingAngle, angle);
        value += car->headingAngle;
        car->headingAngle = value;
        route->trackPointIndex = value;
        car->field_24 = value;
    }
}

/*
 * Runs the rival-car update passes used by an interactive race. Cars 4..10
 * split their traffic-avoidance work across alternating frames.
 */
void GameUpdateRaceCars(void) {
    GameCarVector4 vpos;
    /*
     * GCC 2.6.3 keeps these two Matrix-sized source workspaces in the debug
     * frame even though their high-level scratch values are optimized away.
     */
    Matrix matrixScratch0;
    s16 pair[4];
    Matrix m1;
    Matrix m2;
    GameCarShortVector sv;
    Matrix matrixScratch3;
    /* These pins reproduce the retail induction and matrix registers. */
    register GameCarRuntime *base asm("$18");
    register GameCarAiBlock *drive;
    register Matrix *pm1;
    register Matrix *pm2;
    s16 i;
    register u8 *q asm("$5");
    s32 t;
    i = 0;
    q = (u8 *)g_Cars + 0x8A;
    do {
        (*(s32 *)((u8 *)(q) + (0x6E))) = 0;
        (*(s32 *)((u8 *)(q) + (-0x66))) = (*(s32 *)((u8 *)(q) + (0x7E)));
        (*(s16 *)((u8 *)(q) + (0))) = (*(u16 *)((u8 *)(q) + (0))) & 1;
        i++;
        q += 0x19C;
    } while ((s16)i < 11);
    GameRankContenders();
    for (i = 0; i < 11; i++) {
        s32 j = (s16)i;
        if (j >= 4 && (i & 1) != (g_AnimTimer & 1)) {
            continue;
        }
        if (g_Cars[j].activeFlag != -1) {
            GameUpdateCarTrafficAvoidance(&g_Cars[j], j);
        }
    }
    i = 0;
    base = g_Cars;
    do {
        GameCollideRivalCars(base, (s16)i);
        i++;
        base++;
    } while ((s16)i < 10);
    i = 0;
    base = g_Cars;
    do {
        s32 j = (s16)i;
        GameUpdateCarAiTargetSpeed(base, j);
        GameApplyCarRacingLineHint(base, j);
        GameClampCarLateralOffset(base, j);
        GameSteerCarAlongRoute(base);
        i++;
        base++;
    } while ((s16)i < 11);
    GameUpdateRivalRubberBand();
    i = g_ClosestRivalRank;
    if (i > 0) {
        do {
            s32 j = (s16)i;
            GameSlowRivalAhead(g_RankedCars[j], j);
            i--;
        } while ((s16)i > 0);
    }
    {
    GameCarRuntime *walk;
    i = 0;
    base = g_Cars;
    walk = g_Cars;
    do {
        if ((*(s16 *)((u8 *)(((u8 *)walk + 0x24)) + (0x88))) != -1) {
            drive = (GameCarAiBlock *)&base->field_BC;
            if ((*(s16 *)((u8 *)(((u8 *)walk + 0x24)) + (0x10A))) > 0) {
                if ((*(s16 *)((u8 *)(((u8 *)walk + 0x24)) + (0x104))) < (*(s16 *)((u8 *)(((u8 *)walk + 0x24)) + (0x10A))) && (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x80))) >= 0x321) {
                    (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x84))) = 0;
                } else if (drive->field_130 >= (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x84)))) {
                    (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x84))) = drive->field_12C + (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x84)));
                } else {
                    (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x84))) = drive->field_130;
                }
                drive->field_12E = drive->field_12E - 1;
            } else if ((*(s16 *)((u8 *)(((u8 *)walk + 0x24)) + (0x10C))) >= (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x84)))) {
                (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x84))) = (*(s16 *)((u8 *)(((u8 *)walk + 0x24)) + (0x102))) + (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x84)));
            } else {
                (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x84))) = (*(s16 *)((u8 *)(((u8 *)walk + 0x24)) + (0x10C)));
            }
            (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x80))) = (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x80))) * 0x5E / 100;
            (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x80))) = (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x80))) + (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x84)));
            (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x00))) =
                GameGetAngleDelta((*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x00))), drive->field_EC) / 5 + (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x00)));
        }
        i++;
        walk++;
        base++;
    } while ((s16)i < 11);
    }
    {
    GameCarRuntime *walk;
    i = 0;
    base = g_Cars;
    pm1 = &m1;
    pm2 = &m2;
    walk = g_Cars;
    do {
        drive = (GameCarAiBlock *)&base->field_BC;
        if ((*(s16 *)((u8 *)(((u8 *)walk + 0x24)) + (0x88))) != -1) {
            (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0xE4))) = (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x00)));
            t = func_80068568((*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x7C)))) * (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x80)));
            if (t < 0) {
                t += 0xFF;
            }
            (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0xA4))) = t >> 8;
            t = func_80068634((*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x7C)))) * (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x80)));
            if (t < 0) {
                t += 0xFF;
            }
            (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0xAC))) = t >> 8;
            if ((s16)i < 4) {
                s32 sixth;
                s32 f4;
                (*(s32 *)((u8 *)(base) + (0x00))) = (*(s32 *)((u8 *)(base) + (0x00))) - (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (-0x14)));
                f4 = (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0xD0)));
                (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (-0x1C))) = (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (-0x1C))) - (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (-0x0C)));
                if (f4 < 0) {
                    sixth = -f4 / 6;
                } else {
                    sixth = f4 / 6;
                }
                GameBuildRotMatrixY(pm1, (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x00))));
                GameBuildRotMatrixX(pm2, (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (-0x04))));
                MulMatrix2(pm2, pm1);
                GameBuildRotMatrixZ(pm2, (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x04))));
                MulMatrix2(pm2, pm1);
                sv.vx = 0;
                sv.vy = 0;
                sv.vz = -sixth - 0x32;
                m2.m[0][0] = m1.m[0][0];
                m2.m[0][1] = m1.m[1][0];
                m2.m[0][2] = m1.m[2][0];
                m2.m[1][0] = m1.m[0][1];
                m2.m[1][1] = m1.m[1][1];
                m2.m[1][2] = m1.m[2][1];
                m2.m[2][0] = m1.m[0][2];
                m2.m[2][1] = m1.m[1][2];
                m2.m[2][2] = m1.m[2][2];
                ApplyMatrix(pm2, &sv, (u8 *)base + 0x10);
                (*(s32 *)((u8 *)(base) + (0x00))) = (*(s32 *)((u8 *)(base) + (0x00))) + (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (-0x14)));
                (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (-0x1C))) = (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (-0x1C))) + (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (-0x0C)));
            }
            vpos.x = drive->field_C8 * 6 / 1280 + (*(s32 *)((u8 *)(base) + (0x00)));
            vpos.z = drive->field_D0 * 6 / 1280 + (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (-0x1C)));
            /*
             * Retail only initializes x and z before copying all four words.
             * Its uninitialized y and w stores are intentionally preserved.
             */
            *(GameCarVector4 *)base = vpos;
            if ((*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x20))) >= 0x41) {
                (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x40))) = (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x40))) - 6;
            } else if ((*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x20))) < -0x40) {
                (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x40))) = (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x40))) + 6;
            }
            if ((*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x40))) != 0) {
                (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x40))) = (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x40))) * 7 / 8;
            }
            (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x20))) = (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x20))) + drive->field_F4;
            if ((*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x20))) >= 0x12C) {
                (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x20))) = 0x12C;
            } else if ((*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x20))) < -0x12B) {
                (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x20))) = -0x12C;
            }
            (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x00))) = (*(s32 *)((u8 *)(((u8 *)walk + 0x24)) + (0x00))) + drive->field_F4;
        }
        i++;
        walk++;
        base++;
    } while ((s16)i < 11);
    }
    pair[0] = 0x3C;
    pair[1] = -0x3C;
    for (i = 0; i < 11; i++) {
        if (g_Cars[(s16)i].activeFlag != -1) {
            GameAccumulateLapProgress(&g_Cars[(s16)i]);
        }
    }
    for (i = 0; i < 11; i++) {
        if (g_Cars[(s16)i].activeFlag != -1) {
            if ((s16)g_Cars[(s16)i].motionTimer > 0) {
                GameApplyCarKnockback(&g_Cars[(s16)i]);
            }
            GameUpdateCarTrackState(
                &g_Cars[(s16)i],
                g_Cars[(s16)i].trackPointIndex,
                pair);
        }
    }
    {
        GameCarRuntime *lastBase;
        i = 0;
        base = g_Cars;
        lastBase = g_Cars;
        do {
        if ((*(s16 *)((u8 *)(lastBase) + (0xAC))) != -1) {
            register s16 step asm("$17");
            register s32 spin asm("$2");
            s32 scaled;
            s32 limit;
            scaled = (*(s32 *)((u8 *)(lastBase) + (0xA4))) * 3;
            step = scaled;
            if ((s16)scaled >= 0x1001) {
                step = 0x249;
            }
            spin = (step + (*(s32 *)((u8 *)(lastBase) + (0x48)))) & 0xFFF;
            (*(s32 *)((u8 *)(lastBase) + (0x48))) = spin;
            if ((*(s32 *)((u8 *)(lastBase) + (0xA4))) >= 0x321) {
                (*(s32 *)((u8 *)(lastBase) + (0x48))) = spin | 0x1000;
            }
            limit = (*(s32 *)((u8 *)(lastBase) + (0x04))) - 8;
            *(GameCarVector4 *)((u8 *)lastBase + 0x50) =
                *(GameCarVector4 *)((u8 *)lastBase + 0x20);
            (*(s32 *)((u8 *)(lastBase) + (0x28))) =
                (*(s32 *)((u8 *)(lastBase) + (0x28))) + (*(s32 *)((u8 *)(lastBase) + (0x64)));
            (*(s32 *)((u8 *)(lastBase) + (0x60))) = (*(s32 *)((u8 *)(lastBase) + (0x04)));
            if ((*(s16 *)((u8 *)(lastBase) + (0x98))) != 0) {
                register s32 state asm("$4");
                s32 tick;
                tick = (u16)(*(s16 *)((u8 *)(lastBase) + (0x9A))) + 1;
                (*(s16 *)((u8 *)(lastBase) + (0x9A))) = tick;
                state = (*(s16 *)((u8 *)(lastBase) + (0x98)));
                if (state == 1) {
                    s32 n = (s16)tick;
                    (*(s32 *)((u8 *)(lastBase) + (0x04))) =
                        (*(s16 *)((u8 *)(lastBase) + (0x9C))) * n + n * n * 72 / 100
                        + (*(s32 *)((u8 *)(lastBase) + (0x04)));
                    if ((*(s32 *)((u8 *)(lastBase) + (0x04))) >= limit) {
                        (*(s16 *)((u8 *)(lastBase) + (0x98))) = 0;
                    }
                } else if (state == 2) {
                    if ((*(s16 *)((u8 *)(lastBase) + (0x9E))) >=
                        limit - (*(s16 *)((u8 *)(lastBase) + (0x9C)))) {
                        (*(s32 *)((u8 *)(lastBase) + (0x04))) = (*(s16 *)((u8 *)(lastBase) + (0x9E)));
                    } else {
                        (*(s16 *)((u8 *)(lastBase) + (0x98))) = 3;
                        (*(s16 *)((u8 *)(lastBase) + (0x9C))) = (*(s16 *)((u8 *)(lastBase) + (0x9A)));
                        (*(s32 *)((u8 *)(lastBase) + (0x04))) = (*(s16 *)((u8 *)(lastBase) + (0x9E)));
                    }
                } else {
                    s16 n = tick - (u16)(*(s16 *)((u8 *)(lastBase) + (0x9C)));
                    (*(s32 *)((u8 *)(lastBase) + (0x04))) =
                        (*(s16 *)((u8 *)(lastBase) + (0x9E))) + n * n * 216 / 100;
                    if ((*(s32 *)((u8 *)(lastBase) + (0x04))) >= limit) {
                        (*(s16 *)((u8 *)(lastBase) + (0x98))) = 0;
                    }
                }
                if ((*(s16 *)((u8 *)(lastBase) + (0x98))) == 0) {
                    (*(s32 *)((u8 *)(lastBase) + (0x04))) = limit + 8;
                    (*(s16 *)((u8 *)(lastBase) + (0x90))) = 0;
                    (*(s16 *)((u8 *)(lastBase) + (0x94))) = 0;
                    (*(s16 *)((u8 *)(lastBase) + (0x98))) = 0;
                    GameStartCarBodyKick(1, base);
                }
            }
            if ((*(s16 *)((u8 *)(lastBase) + (0x8A))) == 0) {
                GameUpdateCarBodyKick(base);
                GameUpdateCarCrestHop(base);
            } else {
                (*(s32 *)((u8 *)(lastBase) + (0xA4))) =
                    (*(s32 *)((u8 *)(lastBase) + (0xA4))) * 97 / 100 * 97 / 100;
            }
        }
        i++;
        lastBase++;
        base++;
        } while (i < 11);
    }
}


/* Runs the corresponding all-cars pass for attract and replay scenes. */
void GameUpdateAttractCars(void) {
    GameCarVector4 vTmp;
    /* See GameUpdateRaceCars: these two Matrix workspaces shape retail's frame. */
    Matrix matrixScratch0;
    s16 svAng[4];
    Matrix m1;
    Matrix m2;
    GameCarShortVector sv1;
    Matrix matrixScratch3;
    /* These pins reproduce the retail induction registers. */
    register GameCarRuntime *car;
    register GameCarRuntime *sub asm("$16");
    s16 i;
    GameCarRuntime *c0;
    c0 = g_Cars;
    for (i = 0; i < 11; i++) {
        c0->field_F8 = 0;
        c0->field_8A = 0;
        c0->field_24 = c0->field_108;
        c0->field_68 = ((c0->field_68) % (g_TrackLength));
        c0++;
    }
    for (i = 0; i < 11; i++) {
        if (((g_Cars[(s16)i]).activeFlag != -1)) {
            GameUpdateCarTrafficAvoidance(&g_Cars[(s16)i], (s16)i);
        }
    }
    i = 0;
    car = g_Cars;
    do {
        GameCollideRivalCars(car, (s16)i);
        i++;
        car++;
    } while (i < 10);
    i = 0;
    car = g_Cars;
    do {
        GameUpdateCarAiTargetSpeed(car, (s16)i);
        GameApplyCarRacingLineHint(car, (s16)i);
        GameClampCarLateralOffset(car, (s16)i);
        GameSteerCarAlongRoute(car);
        i++;
        car++;
    } while (i < 11);
    {
        register GameCarAiBlock *drive;
        i = 0;
        car = g_Cars;
        sub = (GameCarRuntime *)((u8 *)g_Cars + 0x24);
        do {
        if ((*(s16 *)((u8 *)(sub) + (0x88))) != -1) {
            drive = (GameCarAiBlock *)&car->field_BC;

            if ((*(s32 *)((u8 *)(sub) + (0x84))) < (*(s16 *)((u8 *)(sub) + (0x10C)))) {
                (*(s32 *)((u8 *)(sub) + (0x84))) = (*(s16 *)((u8 *)(sub) + (0x102))) + (*(s32 *)((u8 *)(sub) + (0x84)));
            } else {
                (*(s32 *)((u8 *)(sub) + (0x84))) = (*(s16 *)((u8 *)(sub) + (0x10C)));
            }
            (*(s32 *)((u8 *)(sub) + (0x80))) = (((*(s32 *)((u8 *)(sub) + (0x80)))) * 94 / 100);
            (*(s32 *)((u8 *)(sub) + (0x80))) = (*(s32 *)((u8 *)(sub) + (0x80))) + (*(s32 *)((u8 *)(sub) + (0x84)));
            (*(s32 *)((u8 *)(sub) + (0x00))) =
                GameGetAngleDelta((*(s32 *)((u8 *)(sub) + (0x00))), drive->field_EC) / 5 + (*(s32 *)((u8 *)(sub) + (0x00)));
        }
        i++;
        sub = (GameCarRuntime *)((u8 *)sub + 0x19C);
        car++;
        } while (i < 11);
        {
        GameCarRuntime *base;
        i = 0;
        car = g_Cars;
        base = g_Cars;
        do {
        drive = (GameCarAiBlock *)&car->field_BC;

        if ((*(s16 *)((u8 *)(base) + (0xAC))) != -1) {
            s32 t;
            (*(s32 *)((u8 *)(base) + (0x108))) = (*(s32 *)((u8 *)(base) + (0x24)));
            t = func_80068568((*(s32 *)((u8 *)(base) + (0xA0)))) * (*(s32 *)((u8 *)(base) + (0xA4)));
            if (t < 0) {
                t += 0xFF;
            }
            (*(s32 *)((u8 *)(base) + (0xC8))) = t >> 8;
            (*(s32 *)((u8 *)(base) + (0xD0))) = func_80068634((*(s32 *)((u8 *)(base) + (0xA0)))) * (*(s32 *)((u8 *)(base) + (0xA4))) / 256;
            if ((s16)i < 4) {
                s32 sixth;
                s32 f4;
                (*(s32 *)((u8 *)(car) + (0x00))) = (*(s32 *)((u8 *)(car) + (0x00))) - (*(s32 *)((u8 *)(base) + (0x10)));
                f4 = (*(s32 *)((u8 *)(base) + (0xF4)));
                (*(s32 *)((u8 *)(base) + (0x08))) = (*(s32 *)((u8 *)(base) + (0x08))) - (*(s32 *)((u8 *)(base) + (0x18)));
                if (f4 < 0) {
                    sixth = -f4 / 6;
                } else {
                    sixth = f4 / 6;
                }
                GameBuildRotMatrixY(&m1, (*(s32 *)((u8 *)(base) + (0x24))));
                GameBuildRotMatrixX(&m2, (*(s32 *)((u8 *)(base) + (0x20))));
                MulMatrix2(&m2, &m1);
                GameBuildRotMatrixZ(&m2, (*(s32 *)((u8 *)(base) + (0x28))));
                MulMatrix2(&m2, &m1);
                sv1.vx = 0;
                sv1.vy = 0;
                sv1.vz = -sixth - 0x32;
                m2.m[0][0] = m1.m[0][0];
                m2.m[0][1] = m1.m[1][0];
                m2.m[0][2] = m1.m[2][0];
                m2.m[1][0] = m1.m[0][1];
                m2.m[1][1] = m1.m[1][1];
                m2.m[1][2] = m1.m[2][1];
                m2.m[2][0] = m1.m[0][2];
                m2.m[2][1] = m1.m[1][2];
                m2.m[2][2] = m1.m[2][2];
                ApplyMatrix(&m2, &sv1, (u8 *)car + 0x10);
                (*(s32 *)((u8 *)(car) + (0x00))) = (*(s32 *)((u8 *)(car) + (0x00))) + (*(s32 *)((u8 *)(base) + (0x10)));
                (*(s32 *)((u8 *)(base) + (0x08))) = (*(s32 *)((u8 *)(base) + (0x08))) + (*(s32 *)((u8 *)(base) + (0x18)));
            }
            vTmp.x = drive->field_C8 * 6 / 1280 + (*(s32 *)((u8 *)(car) + (0x00)));
            vTmp.z = drive->field_D0 * 6 / 1280 + (*(s32 *)((u8 *)(base) + (0x08)));
            /*
             * Retail only initializes x and z before copying all four words.
             * Its uninitialized y and w stores are intentionally preserved.
             */
            *(GameCarVector4 *)car = vTmp;
            if ((*(s32 *)((u8 *)(base) + (0x44))) >= 0x41) {
                (*(s32 *)((u8 *)(base) + (0x64))) = (*(s32 *)((u8 *)(base) + (0x64))) - 6;
            } else if ((*(s32 *)((u8 *)(base) + (0x44))) < -0x40) {
                (*(s32 *)((u8 *)(base) + (0x64))) = (*(s32 *)((u8 *)(base) + (0x64))) + 6;
            }
            if ((*(s32 *)((u8 *)(base) + (0x64))) != 0) {
                (*(s32 *)((u8 *)(base) + (0x64))) = (*(s32 *)((u8 *)(base) + (0x64))) * 7 / 8;
            }
            (*(s32 *)((u8 *)(base) + (0x44))) = (*(s32 *)((u8 *)(base) + (0x44))) + drive->field_F4;
            if ((*(s32 *)((u8 *)(base) + (0x44))) >= 0x12C) {
                (*(s32 *)((u8 *)(base) + (0x44))) = 0x12C;
            } else if ((*(s32 *)((u8 *)(base) + (0x44))) < -0x12B) {
                (*(s32 *)((u8 *)(base) + (0x44))) = -0x12C;
            }
            (*(s32 *)((u8 *)(base) + (0x24))) = (*(s32 *)((u8 *)(base) + (0x24))) + drive->field_F4;
        }
        i++;
        base++;
        car++;
        } while (i < 11);
        }
    }
    svAng[0] = 0x3C;
    svAng[1] = -0x3C;
    for (i = 0; i < 11; i++) {
        if (((g_Cars[(s16)i]).activeFlag != -1)) {
            GameAccumulateLapProgress(&g_Cars[(s16)i]);
        }
    }
    for (i = 0; i < 11; i++) {
        if (((g_Cars[(s16)i]).activeFlag != -1)) {
            if ((s16)g_Cars[(s16)i].motionTimer > 0) {
                GameApplyCarKnockback(&g_Cars[(s16)i]);
            }
            GameUpdateCarTrackState(
                &g_Cars[(s16)i],
                g_Cars[(s16)i].trackPointIndex,
                svAng);
        }
    }
    {
    GameCarRuntime *base;
    i = 0;
    car = g_Cars;
    base = g_Cars;
    do {
        if ((*(s16 *)((u8 *)(base) + (0xAC))) != -1) {
            register s16 step asm("$17");
            register s32 spin asm("$2");
            s32 scaled;
            s32 limit;
            scaled = (*(s32 *)((u8 *)(base) + (0xA4))) * 3;
            step = scaled;
            if ((s16)scaled >= 0x1001) {
                step = 0x249;
            }
            spin = (step + (*(s32 *)((u8 *)(base) + (0x48)))) & 0xFFF;
            (*(s32 *)((u8 *)(base) + (0x48))) = spin;
            if ((*(s32 *)((u8 *)(base) + (0xA4))) >= 0x321) {
                (*(s32 *)((u8 *)(base) + (0x48))) = spin | 0x1000;
            }
            limit = (*(s32 *)((u8 *)(base) + (0x04))) - 8;
            *(GameCarVector4 *)((u8 *)base + 0x50) =
                *(GameCarVector4 *)((u8 *)base + 0x20);
            (*(s32 *)((u8 *)(base) + (0x28))) = (*(s32 *)((u8 *)(base) + (0x28))) + (*(s32 *)((u8 *)(base) + (0x64)));
            (*(s32 *)((u8 *)(base) + (0x60))) = (*(s32 *)((u8 *)(base) + (0x04)));
            if ((*(s16 *)((u8 *)(base) + (0x98))) != 0) {
                register s32 state asm("$4");
                s32 tick;
                tick = (u16)(*(s16 *)((u8 *)(base) + (0x9A))) + 1;
                (*(s16 *)((u8 *)(base) + (0x9A))) = tick;
                state = (*(s16 *)((u8 *)(base) + (0x98)));
                if (state == 1) {
                    s32 t = (s16)tick;
                    (*(s32 *)((u8 *)(base) + (0x04))) =
                        (*(s16 *)((u8 *)(base) + (0x9C))) * t + t * t * 72 / 100 + (*(s32 *)((u8 *)(base) + (0x04)));
                    if ((*(s32 *)((u8 *)(base) + (0x04))) >= limit) {
                        (*(s16 *)((u8 *)(base) + (0x98))) = 0;
                    }
                } else if (state == 2) {
                    if ((*(s16 *)((u8 *)(base) + (0x9E))) >= limit - (*(s16 *)((u8 *)(base) + (0x9C)))) {
                        (*(s32 *)((u8 *)(base) + (0x04))) = (*(s16 *)((u8 *)(base) + (0x9E)));
                    } else {
                        (*(s16 *)((u8 *)(base) + (0x98))) = 3;
                        (*(s16 *)((u8 *)(base) + (0x9C))) = (*(s16 *)((u8 *)(base) + (0x9A)));
                        (*(s32 *)((u8 *)(base) + (0x04))) = (*(s16 *)((u8 *)(base) + (0x9E)));
                    }
                } else {
                    s16 n = tick - (u16)(*(s16 *)((u8 *)(base) + (0x9C)));
                    (*(s32 *)((u8 *)(base) + (0x04))) = (*(s16 *)((u8 *)(base) + (0x9E))) + n * n * 216 / 100;
                    if ((*(s32 *)((u8 *)(base) + (0x04))) >= limit) {
                        (*(s16 *)((u8 *)(base) + (0x98))) = 0;
                    }
                }
                if ((*(s16 *)((u8 *)(base) + (0x98))) == 0) {
                    (*(s32 *)((u8 *)(base) + (0x04))) = limit + 8;
                    (*(s16 *)((u8 *)(base) + (0x90))) = 0;
                    (*(s16 *)((u8 *)(base) + (0x94))) = 0;
                    (*(s16 *)((u8 *)(base) + (0x98))) = 0;
                    GameStartCarBodyKick(1, car);
                }
            }
            if ((*(s16 *)((u8 *)(base) + (0x8A))) == 0) {
                GameUpdateCarBodyKick(car);
                GameUpdateCarCrestHop(car);
            } else {
                (*(s32 *)((u8 *)(base) + (0xA4))) = (*(s32 *)((u8 *)(base) + (0xA4))) * 97 / 100 * 97 / 100;
            }
        }
        i++;
        base++;
        car++;
    } while (i < 11);
    }
}



typedef struct KE {
    u16 f0;
    u16 f2;
    u16 f4;
    u16 f6;
    u16 f8;
    u16 fA;
    s32 fC;
    s16 f10;
    s16 f12;
} KE;

typedef struct B16 {
    s32 a;
    s32 b;
    s32 c;
    s32 d;
} B16;

typedef struct Obj {
    s32 x;
    s32 y;
    s32 z;
    s32 f0C;
    s32 f10;
    s32 f14;
    s32 f18;
    s32 f1C;
    s32 f20;
    s32 f24;
    s32 f28;
    s32 f2C;
} Obj;

extern u8 *volatile g_RaceIntroCameraScript asm("D_8019CAF8");
extern KE *g_RaceIntroCameraCursor asm("D_801E4038");
/* The pre-race fly-in: the timer counts down to 0 while the eye is eased
 * along the delta by cos(timer / keyframe->f12), and it also drives the
 * fade (timer * 26). */
extern s32 g_RaceIntroCameraTimer asm("D_8009AFB4");
extern s16 g_RaceIntroCameraDeltaX asm("D_8009AFBC");
extern s16 g_RaceIntroCameraDeltaY asm("D_8009AFBE");
extern s16 g_RaceIntroCameraDeltaZ asm("D_8009AFC0");

s32 func_80068634(s32 angle);
s32 func_80068568(s32 angle);
void GameDrawPlayerCarModel(void *arg0) asm("func_8001DAB0");
void GameDrawFullscreenFadeTile(s32 arg0, s32 arg1) asm("func_80033AA0");
void GameUpdateCamera(void *arg0, s32 arg1) asm("func_80043BCC");

void GameRunRaceIntroCamera(Obj *obj, s32 mode) asm("func_8003C508");
void GameRunRaceIntroCamera(Obj *obj, s32 mode) {
    s32 *spad = (s32 *) 0x1F800000;
    /* This pin is load-bearing: removing it changes .text. */
    register s32 s0v asm("$16");
    s32 delta[3];

    __asm__("" : "=r"(s0v) : "0"(28), "r"(spad));
    if (mode < 90) {
        if (mode < 2) {
            u8 *base = g_RaceIntroCameraScript;
            s16 n = *(s16 *) (base + 2 * g_RaceSeries);
            s32 off = n * 20 + 4;
            KE *p = (KE *) (off + (s32) base);
            KE *q;
            g_RaceIntroCameraCursor = p;
            *(B16 *) 0x1F800008 = *(B16 *) p;
            q = g_RaceIntroCameraCursor;
            g_RaceIntroCameraDeltaX = -q[0].f0 + q[1].f0;
            g_RaceIntroCameraDeltaY = -q[0].f4 + q[1].f4;
            g_RaceIntroCameraDeltaZ = -q[0].f8 + q[1].f8;
            __asm__ volatile("");
            g_RaceIntroCameraTimer = q[0].f12;
        } else {
            KE *a = g_RaceIntroCameraCursor;
            if (mode == a->f10) {
                g_RaceIntroCameraCursor = &a[1];
                g_RaceIntroCameraTimer = a[1].f12;
                if (a[1].fC == 1) {
                    g_RaceIntroCameraDeltaX = -a[1].f0 + ((u16 *) obj)[0];
                    g_RaceIntroCameraDeltaY = -a[1].f4 - 28 + ((u16 *) obj)[2];
                    g_RaceIntroCameraDeltaZ = -a[1].f8 + ((u16 *) obj)[4];
                } else {
                    g_RaceIntroCameraDeltaX = -a[1].f0 + a[2].f0;
                    g_RaceIntroCameraDeltaY = -a[1].f4 + a[2].f4;
                    g_RaceIntroCameraDeltaZ = -a[1].f8 + a[2].f8;
                }
            }
        }

        g_RaceIntroCameraTimer = g_RaceIntroCameraTimer - 1;
        if (g_RaceIntroCameraTimer <= 0) {
            g_RaceIntroCameraTimer = 0;
        }

        if (g_RaceIntroCameraCursor->fC == 0) {
            spad[2] = ((s32 *) g_RaceIntroCameraCursor)[0]
                      + ((s32) g_RaceIntroCameraDeltaX * func_80068634((g_RaceIntroCameraTimer << 10) / g_RaceIntroCameraCursor->f12)) / 4096;
            spad[3] = ((s32 *) g_RaceIntroCameraCursor)[1]
                      + ((s32) g_RaceIntroCameraDeltaY * func_80068634((g_RaceIntroCameraTimer << 10) / g_RaceIntroCameraCursor->f12)) / 4096;
            spad[4] = ((s32 *) g_RaceIntroCameraCursor)[2]
                      + ((s32) g_RaceIntroCameraDeltaZ * func_80068634((g_RaceIntroCameraTimer << 10) / g_RaceIntroCameraCursor->f12)) / 4096;

            delta[0] = func_80068568(obj->f24) / 128 + obj->x - spad[2];
            delta[1] = obj->y - s0v - spad[3];
            delta[2] = func_80068634(obj->f24) / 128 + obj->z - spad[4];
            s0v = 0x400;
            spad[7] = s0v - GameAtan2(delta[0], delta[2]);
            s0v = s0v - GameAtan2(delta[1], SquareRoot12(delta[0] * delta[0] + delta[2] * delta[2]) >> 6);
            spad[6] = s0v;
            spad[8] = 0;
            GameSetCameraRotMatrix();
            GameSelectModelBank(0);
            GameDrawPlayerCarModel(obj);
        } else {
            GameDrawFullscreenFadeTile(g_RaceIntroCameraTimer * 26, 0x29);
            {
                s32 c0 = obj->x;
                s32 c1 = obj->y;
                s32 c2 = obj->z;
                s32 c3 = obj->f0C;
                spad[2] = c0;
                spad[3] = c1;
                spad[4] = c2;
                spad[5] = c3;
            }
            __asm__ volatile("");
            spad[3] -= s0v;
            {
                s32 c0 = obj->f20;
                s32 c1 = obj->f24;
                s32 c2 = obj->f28;
                s32 c3 = obj->f2C;
                spad[6] = c0;
                spad[7] = c1;
                spad[8] = c2;
                spad[9] = c3;
            }
            __asm__ volatile("");
            GameSetCameraRotMatrix();
        }
    } else {
        GameUpdateCamera(obj, 0);
    }
}

extern u32 g_CameraCar[] asm("D_801E3E14");
extern s32 g_CameraCarY asm("D_801E3E18");
extern s32 g_CameraCarZ asm("D_801E3E1C");
extern s32 g_CameraCarAngleY asm("D_801E3E38");
extern s32 g_CameraCarHeading asm("D_801E3EB4");
extern s32 g_CameraCarSpeed asm("D_801E3EB8");
/* Deliberately raw: written here and in one other camera seeder, read
 * nowhere in the image. */
extern s32 D_801E3F60;

typedef struct CopyBlock8003CA14 {
    u32 word0;
    u32 word1;
    u32 word2;
    u32 word3;
} CopyBlock8003CA14;

void GameSeedFinishCamera(void *arg0) asm("func_8003CA14");
void GameSeedFinishCamera(void *arg0) {
    /* This pin is load-bearing: removing it changes .text. */
    register u32 word0 asm("$2");
    u32 word1;
    u32 word2;
    CopyBlock8003CA14 *src;
    CopyBlock8003CA14 *dst;
    CopyBlock8003CA14 *end;
    u32 *base;
    GameTrackPoint *track;
    GameTrackPoint *point;
    /* These pins are load-bearing: removing any one changes .text. */
    register s32 index asm("$3");
    s32 lastIndex;
    register s32 value asm("$3");

    base = arg0;
    /* This barrier is load-bearing: removing it changes .text. */
    asm("" : "=r"(base) : "0"(base));
    dst = (CopyBlock8003CA14 *)g_CameraCar;
    src = (CopyBlock8003CA14 *)base;
    end = (CopyBlock8003CA14 *)((s32)base + 0x190);
    do {
        *dst = *src;
        src++;
        dst++;
    } while (src != end);

    word0 = ((u32 *)src)[0];
    word1 = ((u32 *)src)[1];
    word2 = ((u32 *)src)[2];
    ((u32 *)dst)[0] = word0;
    ((u32 *)dst)[1] = word1;
    ((u32 *)dst)[2] = word2;

    index = *(s32 *)((s32)base + 0x30);
    track = g_TrackPoints;
    point = (GameTrackPoint *)((index * 3) << 3);
    point = (GameTrackPoint *)((s32)point + (s32)track);
    g_CameraCar[0] = point->x;

    index = *(s32 *)((s32)base + 0x30);
    point = (GameTrackPoint *)((index * 3) << 3);
    point = (GameTrackPoint *)((s32)point + (s32)track);
    g_CameraCarZ = point->z;

    index = *(s32 *)((s32)base + 0x30);
    point = (GameTrackPoint *)((index * 3) << 3);
    point = (GameTrackPoint *)((s32)point + (s32)track);
    value = g_CameraCarSpeed;
    word0 = point->y;
    value += 0x40;
    word0 -= 0x40;
    g_CameraCarSpeed = value;
    g_CameraCarY = word0;

    value = *(s16 *)((s32)base + 0xB8);
    lastIndex = *(s32 *)((s32)base + 0x30);
    value <<= 11;
    point = (GameTrackPoint *)((lastIndex * 3) << 3);
    point = (GameTrackPoint *)((s32)point + (s32)track);
    value += 0xC00;
    value -= point->angle;
    g_CameraCarHeading = value;
    D_801E3F60 = value;
    g_CameraCarAngleY = value;
}
