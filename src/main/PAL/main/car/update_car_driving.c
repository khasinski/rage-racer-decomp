#include "common.h"
#include "game/car.h"
#include "game/car_internal.h"
#include "game/race.h"
#include "game/render.h"
#include "game/audio.h"

/*
 * Car motion handler for state98 == 0 (normal driving): turns steering into a
 * world velocity, triggers over-rev / redline engine-audio cues (comparing
 * against the spec block's redline at +0x100 / +0x106), advances the car
 * (AdvanceCarPosition), and detects the jump/launch trigger. The drive sub-block is
 * the GameCarDrive view of car->field_BC.
 */
void UpdateCarDriving(PlayerCarRuntime *car, s32 unused) {
    GameCarDrive *route = &car->drive;
    s32 sinA;
    s32 cosA;
    s32 base;
    s32 r;
    s32 coords[3];
    GameCarSpec *spec1;
    GameCarSpec *spec;
    s32 t;
    s32 idx;

    r = GetAngleDelta(car->field_24, route->unk90);
    base = car->field_24;
    car->field_24 = r / 5 + base;
    AdvanceCarPosition(car, base);

    sinA = rsin(car->field_24);
    cosA = rcos(car->field_24);

    route->accelPos = rsin(car->headingAngle) * car->speed / 256;
    route->brakePos = rcos(car->headingAngle) * car->speed / 256;

    coords[0] = (cosA * route->accelPos - sinA * route->brakePos) / 4096;
    coords[2] = (sinA * route->accelPos + cosA * route->brakePos) / 4096;
    route->accelPos = sinA * coords[2] / 4096;
    route->brakePos = cosA * coords[2] / 4096;

    spec1 = g_CarSpec;
    if (spec1->revLimit + 2000 < route->unk78 && g_RacePhase >= 2) {
        SetIndexedEffectVoice(0, 0x1800,
                      (route->unk78 - spec1->revLimit) / 100 + 128);
    } else {
        SetIndexedEffectVoice(-1, 0, 0);
    }

    spec = g_CarSpec;
    if (spec->redline + 1000 < route->unk78) {
        s16 v = g_SteerHoldFrames;
        if (v >= 41 && route->gear == spec->topGear &&
            car->shiftState == 0) {
            idx = v + 24;
            if (idx >= 101) {
                idx = 100;
            }
            SetIndexedEffectVoice(2, 0x1500, idx);
        } else {
            SetIndexedEffectVoice(-1, 0, 0);
        }
    } else {
        SetIndexedEffectVoice(-1, 0, 0);
    }

    if (route->unk9C == 1) {
        route->unk48 = car->speed * route->unk44;
        route->unk44 = 0;
        if ((s32) g_LaunchSpeedThresholds[route->unk28].initial < car->speed &&
            route->unk48 > route->unk84) {
            route->state98 = 1;
            route->unk3E = 0;
            SetIndexedEffectVoice(0, 0, 0);
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
                s32 aval = av * car->speed / 64;
                route->unk48 = aval;
                if ((s32) g_LaunchSpeedThresholds[route->unk28].sustain < car->speed &&
                    route->unk84 < aval) {
                    route->state98 = m9e;
                    route->unk3E = 0;
                    SetIndexedEffectVoice(0, 0, 0);
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
