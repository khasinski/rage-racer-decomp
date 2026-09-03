#include "common.h"
#include "game/asset.h"
#include "game/car.h"
#include "game/car_internal.h"
#include "game/race.h"
#include "game/render.h"
#include "game/render_internal.h"
#include "game/scratchpad.h"
#include "game/state.h"
#include "game/track.h"
#include "game/vector.h"
#include "psyq/gte.h"

/*
 * Runs the rival-car update passes used by an interactive race. Cars 4..10
 * split their traffic-avoidance work across alternating frames.
 */
void UpdateRaceCars(void) {
    Vec4 vpos;
    /*
     * GCC 2.6.3 keeps these two Matrix-sized source workspaces in the debug
     * frame even though their high-level scratch values are optimized away.
     */
    Matrix matrixScratch0;
    CarTrackLimits limits;
    Matrix m1;
    Matrix m2;
    SVec sv;
    Matrix matrixScratch3;
    /* These pins reproduce the retail induction and matrix registers. */
    register GameCarRuntime *base asm("$18");
    register GameCarAiBlock *drive;
    register Matrix *pm1;
    register Matrix *pm2;
    s16 i;
    GameCarRuntime *q;
    s32 t;
    i = 0;
    q = g_Cars;
    do {
        q->reservedF8 = 0;
        q->bodyYaw = q->baseBodyYaw;
        q->collisionFlag = (u16)q->collisionFlag & 1;
        i++;
        q++;
    } while ((s16)i < 11);
    RankContenders();
    for (i = 0; i < 11; i++) {
        s32 j = (s16)i;
        if (j >= 4 && (i & 1) != (g_AnimTimer & 1)) {
            continue;
        }
        if (g_Cars[j].activeFlag != -1) {
            UpdateCarTrafficAvoidance(&g_Cars[j], j);
        }
    }
    i = 0;
    base = g_Cars;
    do {
        CollideRivalCars(base, (s16)i);
        i++;
        base++;
    } while ((s16)i < 10);
    i = 0;
    base = g_Cars;
    do {
        s32 j = (s16)i;
        UpdateCarAiTargetSpeed(base, j);
        ApplyCarRacingLineHint(base, j);
        ClampCarLateralOffset(base, j);
        SteerCarAlongRoute(base);
        i++;
        base++;
    } while ((s16)i < 11);
    UpdateRivalRubberBand();
    i = (s16)g_ClosestRivalRank;
    if (i > 0) {
        do {
            s32 j = (s16)i;
            SlowRivalAhead(g_RankedCars[j], j);
            i--;
        } while ((s16)i > 0);
    }
    {
    GameCarRuntime *walk;
    i = 0;
    base = g_Cars;
    walk = g_Cars;
    do {
        if (walk->activeFlag != -1) {
            drive = GetCarAiBlock(base);
            if (walk->boostTimer > 0) {
                if (walk->boostAccelerationThreshold < walk->boostTimer && walk->speed >= 0x321) {
                    walk->acceleration = 0;
                } else if (drive->accelerationLimit >= walk->acceleration) {
                    walk->acceleration = drive->boostAcceleration + walk->acceleration;
                } else {
                    walk->acceleration = drive->accelerationLimit;
                }
                drive->boostTimer = drive->boostTimer - 1;
            } else if (walk->accelerationLimit >= walk->acceleration) {
                walk->acceleration = walk->accelerationStep + walk->acceleration;
            } else {
                walk->acceleration = walk->accelerationLimit;
            }
            walk->speed = walk->speed * 0x5E / 100;
            walk->speed = walk->speed + walk->acceleration;
            walk->bodyYaw =
                GetAngleDelta(walk->bodyYaw, drive->targetYaw) / 5 + walk->bodyYaw;
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
        drive = GetCarAiBlock(base);
        if (walk->activeFlag != -1) {
            walk->baseBodyYaw = walk->bodyYaw;
            t = rsin(walk->headingAngle) * walk->speed;
            if (t < 0) {
                t += 0xFF;
            }
            walk->worldVelocityX = t >> 8;
            t = rcos(walk->headingAngle) * walk->speed;
            if (t < 0) {
                t += 0xFF;
            }
            walk->worldVelocityZ = t >> 8;
            if ((s16)i < 4) {
                s32 sixth;
                s32 yawStep;
                base->x = base->x - walk->motionX;
                yawStep = walk->yawRate;
                walk->z = walk->z - walk->motionZ;
                if (yawStep < 0) {
                    sixth = -yawStep / 6;
                } else {
                    sixth = yawStep / 6;
                }
                BuildRotMatrixY(pm1, walk->bodyYaw);
                BuildRotMatrixX(pm2, walk->bodyPitch);
                MulMatrix2(pm2, pm1);
                BuildRotMatrixZ(pm2, walk->bodyRoll);
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
                ApplyMatrix(pm2, &sv, &base->motionX);
                base->x = base->x + walk->motionX;
                walk->z = walk->z + walk->motionZ;
            }
            vpos.x = (drive->worldVelocityX / 256) + base->x;
            vpos.z = (drive->worldVelocityZ / 256) + walk->z;
            /*
             * Retail only initializes x and z before copying all four words.
             * Its uninitialized y and w stores are intentionally preserved.
             */
            *GetCarVector4(base) = vpos;
            if (walk->steeringAngle >= 0x41) {
                walk->bodyRollVelocity = walk->bodyRollVelocity - 6;
            } else if (walk->steeringAngle < -0x40) {
                walk->bodyRollVelocity = walk->bodyRollVelocity + 6;
            }
            if (walk->bodyRollVelocity != 0) {
                walk->bodyRollVelocity = walk->bodyRollVelocity * 7 / 8;
            }
            walk->steeringAngle = walk->steeringAngle + drive->yawRate;
            if (walk->steeringAngle >= 0x12C) {
                walk->steeringAngle = 0x12C;
            } else if (walk->steeringAngle < -0x12B) {
                walk->steeringAngle = -0x12C;
            }
            walk->bodyYaw = walk->bodyYaw + drive->yawRate;
        }
        i++;
        walk++;
        base++;
    } while ((s16)i < 11);
    }
    limits.rightInset = 0x3C;
    limits.leftInset = -0x3C;
    for (i = 0; i < 11; i++) {
        if (g_Cars[(s16)i].activeFlag != -1) {
            AccumulateLapProgress(&g_Cars[(s16)i]);
        }
    }
    for (i = 0; i < 11; i++) {
        if (g_Cars[(s16)i].activeFlag != -1) {
            if ((s16)g_Cars[(s16)i].motionTimer > 0) {
                ApplyCarKnockback(&g_Cars[(s16)i]);
            }
            UpdateCarTrackState(
                &g_Cars[(s16)i],
                g_Cars[(s16)i].trackPointIndex,
                &limits);
        }
    }
    {
        GameCarRuntime *lastBase;
        i = 0;
        base = g_Cars;
        lastBase = g_Cars;
        do {
        if (lastBase->activeFlag != -1) {
            register s16 step asm("$17");
            register s32 spin asm("$2");
            s32 scaled;
            s32 limit;
            scaled = lastBase->speed * 3;
            step = scaled;
            if ((s16)scaled >= 0x1001) {
                step = 0x249;
            }
            spin = (step + lastBase->wheelRotation) & 0xFFF;
            lastBase->wheelRotation = spin;
            if (lastBase->speed >= 0x321) {
                lastBase->wheelRotation = spin | 0x1000;
            }
            limit = lastBase->y - 8;
            CopyCarBodyRotationToModel(lastBase);
            lastBase->bodyRoll = lastBase->bodyRoll + lastBase->bodyRollVelocity;
            lastBase->modelY = lastBase->y;
            if (lastBase->verticalMotionState != 0) {
                s32 tick;
                s32 state;
                tick = (u16)lastBase->verticalMotionTimer + 1;
                lastBase->verticalMotionTimer = tick;
                state = lastBase->verticalMotionState;
                if (state == 1) {
                    s32 n = (s16)tick;
                    lastBase->y =
                        lastBase->verticalMotionRate * n + n * n * 60 / 100
                        + lastBase->y;
                    if (lastBase->y >= limit) {
                        lastBase->verticalMotionState = 0;
                    }
                } else if (state == 2) {
                    if (lastBase->verticalTargetY >= limit - lastBase->verticalMotionRate) {
                        lastBase->y = lastBase->verticalTargetY;
                    } else {
                        lastBase->verticalMotionState = 3;
                        lastBase->verticalMotionRate = lastBase->verticalMotionTimer;
                        lastBase->y = lastBase->verticalTargetY;
                    }
                } else {
                    s16 n = tick - (u16)lastBase->verticalMotionRate;
                    lastBase->y = lastBase->verticalTargetY + n * n * 180 / 100;
                    if (lastBase->y >= limit) {
                        lastBase->verticalMotionState = 0;
                    }
                }
                if (lastBase->verticalMotionState == 0) {
                    lastBase->y = limit + 8;
                    lastBase->verticalPitch = 0;
                    lastBase->verticalRoll = 0;
                    lastBase->verticalMotionState = 0;
                    StartCarBodyKick(1, base);
                }
            }
            if (lastBase->collisionFlag == 0) {
                UpdateCarBodyKick(base);
                UpdateCarCrestHop(base);
            } else {
                lastBase->speed = lastBase->speed * 97 / 100 * 97 / 100;
            }
        }
        i++;
        lastBase++;
        base++;
        } while (i < 11);
    }
}

/* Runs the corresponding all-cars pass for attract and replay scenes. */
void UpdateAttractCars(void) {
    Vec4 vTmp;
    /* See UpdateRaceCars: these two Matrix workspaces shape retail's frame. */
    Matrix matrixScratch0;
    CarTrackLimits limits;
    Matrix m1;
    Matrix m2;
    SVec sv1;
    Matrix matrixScratch3;
    /* These pins reproduce the retail induction registers. */
    register GameCarRuntime *car;
    GameCarRuntime *sub;
    s16 i;
    GameCarRuntime *c0;
    c0 = g_Cars;
    for (i = 0; i < 11; i++) {
        c0->reservedF8 = 0;
        c0->collisionFlag = 0;
        c0->bodyYaw = c0->baseBodyYaw;
        c0->progressA = ((c0->progressA) % (g_TrackLength));
        c0++;
    }
    for (i = 0; i < 11; i++) {
        if (((g_Cars[(s16)i]).activeFlag != -1)) {
            UpdateCarTrafficAvoidance(&g_Cars[(s16)i], (s16)i);
        }
    }
    i = 0;
    car = g_Cars;
    do {
        CollideRivalCars(car, (s16)i);
        i++;
        car++;
    } while (i < 10);
    i = 0;
    car = g_Cars;
    do {
        UpdateCarAiTargetSpeed(car, (s16)i);
        ApplyCarRacingLineHint(car, (s16)i);
        ClampCarLateralOffset(car, (s16)i);
        SteerCarAlongRoute(car);
        i++;
        car++;
    } while (i < 11);
    {
        register GameCarAiBlock *drive;
        i = 0;
        car = g_Cars;
        sub = g_Cars;
        do {
        if (sub->activeFlag != -1) {
            drive = GetCarAiBlock(car);

            if (sub->acceleration < sub->accelerationLimit) {
                sub->acceleration = sub->accelerationStep + sub->acceleration;
            } else {
                sub->acceleration = sub->accelerationLimit;
            }
            sub->speed = sub->speed * 94 / 100;
            sub->speed = sub->speed + sub->acceleration;
            sub->bodyYaw =
                GetAngleDelta(sub->bodyYaw, drive->targetYaw) / 5 + sub->bodyYaw;
        }
        i++;
        sub++;
        car++;
        } while (i < 11);
        {
        GameCarRuntime *base;
        i = 0;
        car = g_Cars;
        base = g_Cars;
        do {
        drive = GetCarAiBlock(car);

        if (base->activeFlag != -1) {
            s32 t;
            base->baseBodyYaw = base->bodyYaw;
            t = rsin(base->headingAngle) * base->speed;
            if (t < 0) {
                t += 0xFF;
            }
            base->worldVelocityX = t >> 8;
            base->worldVelocityZ = rcos(base->headingAngle) * base->speed / 256;
            if ((s16)i < 4) {
                s32 sixth;
                s32 yawStep;
                car->x = car->x - base->motionX;
                yawStep = base->yawRate;
                base->z = base->z - base->motionZ;
                if (yawStep < 0) {
                    sixth = -yawStep / 6;
                } else {
                    sixth = yawStep / 6;
                }
                BuildRotMatrixY(&m1, base->bodyYaw);
                BuildRotMatrixX(&m2, base->bodyPitch);
                MulMatrix2(&m2, &m1);
                BuildRotMatrixZ(&m2, base->bodyRoll);
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
                ApplyMatrix(&m2, &sv1, &car->motionX);
                car->x = car->x + base->motionX;
                base->z = base->z + base->motionZ;
            }
            vTmp.x = (drive->worldVelocityX / 256) + car->x;
            vTmp.z = (drive->worldVelocityZ / 256) + base->z;
            /*
             * Retail only initializes x and z before copying all four words.
             * Its uninitialized y and w stores are intentionally preserved.
             */
            *GetCarVector4(car) = vTmp;
            if (base->steeringAngle >= 0x41) {
                base->bodyRollVelocity = base->bodyRollVelocity - 6;
            } else if (base->steeringAngle < -0x40) {
                base->bodyRollVelocity = base->bodyRollVelocity + 6;
            }
            if (base->bodyRollVelocity != 0) {
                base->bodyRollVelocity = base->bodyRollVelocity * 7 / 8;
            }
            base->steeringAngle = base->steeringAngle + drive->yawRate;
            if (base->steeringAngle >= 0x12C) {
                base->steeringAngle = 0x12C;
            } else if (base->steeringAngle < -0x12B) {
                base->steeringAngle = -0x12C;
            }
            base->bodyYaw = base->bodyYaw + drive->yawRate;
        }
        i++;
        base++;
        car++;
        } while (i < 11);
        }
    }
    limits.rightInset = 0x3C;
    limits.leftInset = -0x3C;
    for (i = 0; i < 11; i++) {
        if (((g_Cars[(s16)i]).activeFlag != -1)) {
            AccumulateLapProgress(&g_Cars[(s16)i]);
        }
    }
    for (i = 0; i < 11; i++) {
        if (((g_Cars[(s16)i]).activeFlag != -1)) {
            if ((s16)g_Cars[(s16)i].motionTimer > 0) {
                ApplyCarKnockback(&g_Cars[(s16)i]);
            }
            UpdateCarTrackState(
                &g_Cars[(s16)i],
                g_Cars[(s16)i].trackPointIndex,
                &limits);
        }
    }
    {
    GameCarRuntime *base;
    i = 0;
    car = g_Cars;
    base = g_Cars;
    do {
        if (base->activeFlag != -1) {
            register s16 step asm("$17");
            register s32 spin asm("$2");
            s32 scaled;
            s32 limit;
            scaled = base->speed * 3;
            step = scaled;
            if ((s16)scaled >= 0x1001) {
                step = 0x249;
            }
            spin = (step + base->wheelRotation) & 0xFFF;
            base->wheelRotation = spin;
            if (base->speed >= 0x321) {
                base->wheelRotation = spin | 0x1000;
            }
            limit = base->y - 8;
            CopyCarBodyRotationToModel(base);
            base->bodyRoll = base->bodyRoll + base->bodyRollVelocity;
            base->modelY = base->y;
            if (base->verticalMotionState != 0) {
                s32 tick;
                s32 state;
                tick = (u16)base->verticalMotionTimer + 1;
                base->verticalMotionTimer = tick;
                state = base->verticalMotionState;
                if (state == 1) {
                    s32 t = (s16)tick;
                    base->y =
                        base->verticalMotionRate * t + t * t * 60 / 100 + base->y;
                    if (base->y >= limit) {
                        base->verticalMotionState = 0;
                    }
                } else if (state == 2) {
                    if (base->verticalTargetY >= limit - base->verticalMotionRate) {
                        base->y = base->verticalTargetY;
                    } else {
                        base->verticalMotionState = 3;
                        base->verticalMotionRate = base->verticalMotionTimer;
                        base->y = base->verticalTargetY;
                    }
                } else {
                    s16 n = tick - (u16)base->verticalMotionRate;
                    base->y = base->verticalTargetY + n * n * 180 / 100;
                    if (base->y >= limit) {
                        base->verticalMotionState = 0;
                    }
                }
                if (base->verticalMotionState == 0) {
                    base->y = limit + 8;
                    base->verticalPitch = 0;
                    base->verticalRoll = 0;
                    base->verticalMotionState = 0;
                    StartCarBodyKick(1, car);
                }
            }
            if (base->collisionFlag == 0) {
                UpdateCarBodyKick(car);
                UpdateCarCrestHop(car);
            } else {
                base->speed = base->speed * 97 / 100 * 97 / 100;
            }
        }
        i++;
        base++;
        car++;
    } while (i < 11);
    }
}
