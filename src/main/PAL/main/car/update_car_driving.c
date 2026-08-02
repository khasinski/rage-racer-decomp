#include "common.h"
#include "game/car.h"
#include "game/race.h"
#include "game/render.h"

typedef struct D8007Pair {
    s16 f0;
    s16 f2;
} D8007Pair;

extern s16 g_SteerHoldFrames asm("D_801F17A4");
extern D8007Pair g_LaunchSpeedThresholds[] asm("D_8007DAC0");

void func_8002F4E4(GameCarRuntime *car, s32 arg1);
s32 func_80068568(s32 arg0);
s32 func_80068634(s32 arg0);
void func_8005C104(s32 index, s32 phase, s32 volume);

/*
 * Car motion handler for state98 == 0 (normal driving): turns steering into a
 * world velocity, triggers over-rev / redline engine-audio cues (comparing
 * against the spec block's redline at +0x100 / +0x106), advances the car
 * (func_8002F4E4), and detects the jump/launch trigger. The drive sub-block is
 * the GameCarDrive view of car->field_BC.
 */
void UpdateCarDriving(GameCarRuntime *car) asm("func_8002F690");
void UpdateCarDriving(GameCarRuntime *car) {
    GameCarDrive *route = (GameCarDrive *)&car->field_BC;
    s32 sinA;
    s32 cosA;
    s32 base;
    s32 r;
    s32 coords[3];
    GameCarSpec *spec1;
    GameCarSpec *spec;
    s32 t;
    s32 idx;

    r = GetAngleDelta(car->field_24, *(s32 *)&car->field_14C);
    base = car->field_24;
    car->field_24 = r / 5 + base;
    func_8002F4E4(car, base);

    sinA = func_80068568(car->field_24);
    cosA = func_80068634(car->field_24);

    car->field_C4 = func_80068568(car->headingAngle) * car->field_A4 / 256;
    car->field_CC = func_80068634(car->headingAngle) * car->field_A4 / 256;

    coords[0] = (cosA * car->field_C4 - sinA * car->field_CC) / 4096;
    coords[2] = (sinA * car->field_C4 + cosA * car->field_CC) / 4096;
    car->field_C4 = sinA * coords[2] / 4096;
    car->field_CC = cosA * coords[2] / 4096;

    spec1 = g_CarSpec;
    if (spec1->revLimit + 2000 < car->field_134 && g_RacePhase >= 2) {
        func_8005C104(0, 0x1800,
                      (car->field_134 - spec1->revLimit) / 100 + 128);
    } else {
        func_8005C104(-1, 0, 0);
    }

    spec = g_CarSpec;
    if (spec->redline + 1000 < route->unk78) {
        s16 v = g_SteerHoldFrames;
        if (v >= 41 && route->gear == spec->topGear &&
            car->field_98 == 0) {
            idx = v + 24;
            if (idx >= 101) {
                idx = 100;
            }
            func_8005C104(2, 0x1500, idx);
        } else {
            func_8005C104(-1, 0, 0);
        }
    } else {
        func_8005C104(-1, 0, 0);
    }

    if (route->unk9C == 1) {
        route->unk48 = car->field_A4 * route->unk44;
        route->unk44 = 0;
        if ((s32) g_LaunchSpeedThresholds[route->unk28].f0 < car->field_A4 &&
            route->unk48 > route->unk84) {
            route->state98 = 1;
            *(s16 *)&route->unk3E = 0;
            func_8005C104(0, 0, 0);
            t = 1000 - (route->unk88 - 1000) * 8;
            if (t < 1000) {
                t = 1000;
            }
            route->unk50 = -coords[0] * t / 1000 * 2;
            route->unk54 = car->facingBackwards;
        }
    } else {
        if (route->accelBtn < 128) {
            s16 m9e = route->unk9E;
            if (m9e == 1) {
                s32 av = coords[0] < 0 ? -coords[0] : coords[0];
                s32 aval = av * car->field_A4 / 64;
                route->unk48 = aval;
                if ((s32) g_LaunchSpeedThresholds[route->unk28].f2 < car->field_A4 &&
                    route->unk84 < aval) {
                    route->state98 = m9e;
                    *(s16 *)&route->unk3E = 0;
                    func_8005C104(0, 0, 0);
                    route->unk50 = -coords[0];
                    route->unk54 = car->facingBackwards;
                }
            } else {
                route->unk44 = route->unk44 + 1;
                route->unk48 = 0;
            }
        } else {
            route->unk44 = 0;
            route->unk48 = 0;
        }
    }
}
