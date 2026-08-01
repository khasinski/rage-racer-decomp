#include "common.h"
#include "game/car.h"
#include "game/race.h"
#include "game/state.h"
#include "game/render.h"

void GameClearCarMotionState(GameCarRuntime *arg0) asm("func_80038AB8");

extern s32 g_ClosestRivalRank asm("D_801E7740");




extern s32 g_RaceGridSlots[] asm("D_8007E074");

extern s32 g_AttractGridSlots[] asm("D_8007E0A4");

void GameInitRivalCar(GameCarRuntime *arg0, s32 arg1, s32 *arg2) asm("func_800383A8");

void GameInitRivalCarAi(GameCarRuntime *arg0, s32 arg1, s32 *arg2) asm("func_800385FC");

void GameSeedCarRouteMarkers(void) asm("func_80039644");


void func_8001DFC0(GameCarRuntime *arg0);



/*
 * Updates the car's skid/tilt counter (field_8C / field_8E), clamping it
 * against the spec block's redline value (g_CarSpec + 0x106). Register-pinned,
 * goto-structured; the raw drive-block reads (+0xA2 / +0x34) preserve the match.
 */

/*
 * Applies and decays the collision shake: while motionActive/motionTimer are
 * set, subtracts field_7C / field_7E from the car position and damps the
 * velocity by 7/8 (*8-1 >> 3) each frame. field_7C / field_7E are read via
 * inline __asm__ lh loads and the registers are pinned, so the struct offsets
 * must not change.
 */

void GameBuildStartingGrid(void) asm("func_80038844");
void GameBuildStartingGrid(void) {
    GameCarRuntime *entryBase;
    /* These pins are load-bearing: removing any one changes .text. */
    register s32 *table asm("s3");
    s32 i;
    register s16 *flagPtr asm("s1");
    s32 one;
    s32 *cursor;
    s32 state;
    u16 track;

    entryBase = g_Cars;
    state = g_SceneId;
    g_ClosestRivalRank = 3;

    if (state == 0xB) {
        table = g_RaceGridSlots;
        i = 0;
        one = 1;
        flagPtr = &entryBase->activeFlag;
        g_RaceSeries = g_GrandPrixSeries;
        cursor = table;
        do {
            track = *(u16 *)&g_RaceSeries;
            *(s16 *)flagPtr = 0;
            *(s16 *)(flagPtr + 6) = track;
            if (*cursor >= 0) {
                GameClearCarMotionState(entryBase);
                *(s16 *)flagPtr = one;
                GameInitRivalCar(entryBase, i, table);
                GameInitRivalCarAi(entryBase, i, table);
            } else {
                *(s16 *)flagPtr = 0;
            }
            cursor++;
            i++;
            flagPtr = (s16 *)((u8 *)flagPtr + 0x19C);
            entryBase++;
        } while (i < 0xB);
    } else {
        table = g_AttractGridSlots;
        i = 0;
        one = 1;
        flagPtr = &entryBase->activeFlag;
        g_RaceSeries = g_GrandPrixSeries;
        cursor = table;
        do {
            track = *(u16 *)&g_RaceSeries;
            *(s16 *)flagPtr = 0;
            *(s16 *)(flagPtr + 6) = track;
            if (*cursor >= 0) {
                GameClearCarMotionState(entryBase);
                *(s16 *)flagPtr = one;
                GameInitRivalCar(entryBase, i, table);
                GameInitRivalCarAi(entryBase, i, table);
            } else {
                *(s16 *)flagPtr = 0;
            }
            cursor++;
            i++;
            flagPtr = (s16 *)((u8 *)flagPtr + 0x19C);
            entryBase++;
        } while (i < 0xB);
    }

    GameSeedCarRouteMarkers();
}

void GameDrawCars(void) asm("func_800389F0");
void GameDrawCars(void) {
    GameCarRuntime *base;
    s32 i;
    u8 *entry;
    s32 one;
    s32 minus_one;

    base = g_Cars;
    GameSelectModelBank(1);

    i = 0;
    minus_one = -1;
    one = 1;
    entry = (u8 *)&base->field_BC;
    do {
        if (*(s16 *)(entry - 0x10) != (i++, minus_one)) {
            if (*(s32 *)entry == one) {
                func_8001DFC0(base);
            }
        }
        entry += 0x19C;
        base++;
    } while (i < 11);
}

void GameDrawPlayerCarOnly(void) asm("func_80038A88");
void GameDrawPlayerCarOnly(void) {
    GameSelectModelBank(1);
    func_8001DFC0(g_Cars);
}

void GameClearCarMotionState(GameCarRuntime *arg0) asm("func_80038AB8");
void GameClearCarMotionState(GameCarRuntime *arg0) {
    arg0->field_8A = 0;
    arg0->motionMode = 0;
    arg0->motionModeTimer = 0;
    arg0->motionValue = 0;
    arg0->motionActive = 0;
    arg0->motionTimer = 0;
    arg0->velocityX = 0;
    arg0->velocityZ = 0;
    arg0->field_8C = 0;
    arg0->field_8E = 0;
    arg0->field_90 = 0;
    arg0->field_92 = 0;
    arg0->field_94 = 0;
    arg0->field_96 = 0;
    arg0->field_98 = 0;
    arg0->field_9A = 0;
    arg0->field_9C = 0;
    arg0->field_9E = 0;
}

void GameUpdateCarTiltCounter(GameCarRuntime *arg0) asm("func_80038B04");
void GameUpdateCarTiltCounter(GameCarRuntime *arg0) {
    GameCarRuntime *obj;
    /* These pins are load-bearing: removing any one changes .text. */
    register u8 *ptr asm("$4");
    s32 value;
    register s32 limit asm("$3");

    obj = arg0;

    ptr = (u8 *)&obj->field_BC;
    if (g_RacePhase < 2) {
        value = 8;
    } else {
    if (obj->field_98 == 0) {
        if (obj->field_134 >= g_CarSpec->redline &&
            obj->field_15C >= 0x81 &&
            obj->field_F0 == 0) {
            ptr = (u8 *)(s32)(u16)obj->field_8C;
            value = obj->field_132;
            ptr -= 4;
            obj->field_8C = (s32)ptr;
            ptr = (u8 *)((s32)ptr << 16 >> 16);
            limit = 9 - value;
            value = (limit << 2) + limit;
            value = -value;
            if ((s32)ptr < value) {
                obj->field_8C = value;
            }
            return;
        }

        if (*(s16 *)(ptr + 0xA2) >= 0x81 || *(s16 *)(ptr + 0x34) > 0) {
            if (obj->field_A4 >= 0x51) {
                value = (u16)obj->field_8C + 2;
                obj->field_8C = value;
                value = (s16)value;
                if (value >= 9) {
                    obj->field_8C = 8;
                }
                return;
            }
        }
    }

    limit = obj->field_8C;
    value = limit * 3;
    if (value < 0) {
        value += 3;
    }
    value >>= 2;
    }
    obj->field_8C = value;
}

void GameApplyCarKnockback(GameCarRuntime *arg0) asm("func_80038C4C");
void GameApplyCarKnockback(GameCarRuntime *arg0) {
    u32 timer;

    if (arg0->motionActive != 0) {
        timer = arg0->motionTimer - 1;
        arg0->motionTimer = timer;
        if ((s32)(timer << 16) <= 0) {
            arg0->motionActive = 0;
            arg0->motionTimer = 0;
        }

        arg0->x -= arg0->velocityX;
        arg0->z -= arg0->velocityZ;
        arg0->velocityX = arg0->velocityX * 7 / 8;
        arg0->velocityZ = arg0->velocityZ * 7 / 8;
    }
}

s32 func_80068568(s32 angle);

s32 func_80068634(s32 angle);

/*
 * Collision / boundary response: sets the car's knock-back motion vector
 * (velocityX / velocityZ, motionTimer, motionActive) from a push direction and
 * speed derived per `mode` (0 / 2 / 4 select different angle+speed math from
 * arg1/arg2). Register-pinned, goto-structured decompilation; do not restructure.
 */

s32 func_8002FB60(s32 arg0);

s32 GameRandom15(void) asm("func_800632B0");

void GameSetCarKnockback(GameCarRuntime *car, s32 arg1, s32 arg2, s32 mode) asm("func_80038CE8");
void GameSetCarKnockback(GameCarRuntime *car, s32 arg1, s32 arg2, s32 mode) {
    GameCarRuntime *carReg;
    s32 x;
    s32 angle;
    /* These pins are load-bearing: removing any one changes .text. */
    register s32 savedAngle asm("$19");
    s32 z = arg2;
    register s32 adjustedReg asm("$2");
    s32 raw;
    register s32 rawArg asm("$4");
    register s32 field34 asm("$2");
    s32 adjusted;
    s32 fieldA4;
    s32 tmp;
    s32 speed;
    s32 trig;
    s32 product;

    carReg = car;
    /* This barrier is load-bearing: removing it changes .text. */
    asm("" : : "r"(carReg));
    x = arg1;
    carReg->motionActive = 1;
    if (mode < 2) {

    adjustedReg = *(u16 *)&carReg->field_B4;
    raw = 0xC00;
    raw -= adjustedReg;
    rawArg = raw;
    raw <<= 16;
    field34 = carReg->field_34;
    raw >>= 16;
    if (field34 < 0) {
        adjusted = raw - 0x400;
    } else {
        adjusted = raw + 0x400;
    }
    adjustedReg = adjusted & 0xFFF;
    /* This barrier is load-bearing: removing it changes .text. */
    asm("" : "=r"(adjustedReg) : "0"(adjustedReg));
    fieldA4 = carReg->field_A4;
    angle = adjustedReg;
    if (fieldA4 >= 0x321) {
        if (fieldA4 >= 0x709) {
            speed = 0x708;
        } else {
            speed = *(u16 *)&carReg->field_A4;
        }
        trig = func_80068568(GameGetAngleDistance((s16)rawArg, carReg->field_24));
        product = (s16)speed * trig;
        if (product < 0) {
            product += 0xFFFF;
        }
        tmp = product >> 16;
    } else {
        trig = func_80068568(GameGetAngleDistance((s16)rawArg, carReg->field_24));
        product = trig << 1;
        product += trig;
        product <<= 3;
        product += trig;
        adjustedReg = product << 1;
        if (adjustedReg < 0) {
            adjustedReg += 0xFFF;
        }
        tmp = adjustedReg >> 12;
    }

speed_ready:
    speed = tmp + 10;

make_vector:
    savedAngle = angle;
    trig = func_80068568(savedAngle);
    product = speed << 16;
    angle = product >> 16;
    adjustedReg = trig * angle;
    if (adjustedReg < 0) {
        adjustedReg += 0xFFF;
    }
    x = adjustedReg >> 12;
    trig = func_80068634(savedAngle);
    adjustedReg = trig * angle;
    if (adjustedReg < 0) {
        adjustedReg += 0xFFF;
    }
    z = adjustedReg >> 12;
    tmp = 0x1E;
    } else if (mode < 4) {
    adjustedReg = *(u16 *)&carReg->field_B4;
    raw = 0xC00;
    raw -= adjustedReg;
    raw <<= 16;
    field34 = carReg->field_34;
    raw >>= 16;
    if (field34 < 0) {
        adjusted = raw - 0x400;
    } else {
        adjusted = raw + 0x400;
    }
    rawArg = adjusted & 0xFFF;
    angle = rawArg;
    trig = func_80068568(rawArg);
    tmp = trig * 20;
    if (tmp < 0) {
        tmp += 0xFFF;
    }
    x = tmp >> 12;
    trig = func_80068634(angle);
    tmp = trig * 20;
    if (tmp < 0) {
        tmp += 0xFFF;
    }
    z = tmp >> 12;
    tmp = 0xF;
    } else {
    adjustedReg = (u32)x >> 31;
    adjustedReg = x + adjustedReg;
    x = adjustedReg >> 1;
    adjustedReg = (u32)z >> 31;
    adjustedReg = z + adjustedReg;
    z = adjustedReg >> 1;
    tmp = 0xF;
    }

store_values:
    carReg->motionTimer = tmp;
    carReg->velocityX = x;
    carReg->velocityZ = z;
}

void GameStartCarBodyKick(s32 arg0, GameCarRuntime *arg1) asm("func_80038F0C");
void GameStartCarBodyKick(s32 arg0, GameCarRuntime *arg1) {
    GameCarRuntime *obj;
    s32 value;
    s32 temp;
    s32 distance;

    obj = arg1;
    value = 1;
    obj->motionMode = arg0;
    if (arg0 == value) {
        goto mode1;
    }

    value = 2;
    if (arg0 == value) {
        goto mode2;
    }

    return;

mode1:
    value = obj->field_9A;
    temp = 0x1E;
    obj->motionModeTimer = temp;
    value <<= 3;
    goto final_store;

mode2:
    value = func_8002FB60(obj->trackPointIndex);
    temp = GameGetAngleDistance(value, obj->field_24);
    if (temp >= 0x401) {
        temp = 0x800 - temp;
    }

    distance = obj->field_A4;
    if (distance < 0x140) {
        obj->motionValue = 0;
        goto after_store;
    }

    value = distance - 0x140;
    value *= temp;
    if (value < 0) {
        value += 0xFFF;
    }
    value >>= 12;

store:
    obj->motionValue = value;

after_store:
    value = 0x1E;
    obj->motionModeTimer = value;

    value = GameRandom15();
    if (value & 0x80) {
        value = *(u16 *)&obj->motionValue;
        value = -value;

final_store:
        obj->motionValue = value;
    }
}
