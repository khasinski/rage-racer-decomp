#include "common.h"
#include "game/car.h"
#include "game/track.h"
#include "game/race.h"
#include "game/render.h"


void func_8002FC84(s32 arg0, s32 *out, s32 weight);
s32 func_8002FD9C(s32 arg0, s32 arg1);
s32 func_80068568(s32 arg0);
s32 func_80068634(s32 arg0);

/*
 * Car route-steering update. Samples a look-ahead track point (two ahead or two
 * behind depending on the lap-direction flag g_RaceSeries), clamps the lateral
 * offset to the track half-width (field_10/field_12), projects the target point
 * off the centre-line along the inward normal (0x1000 - smoothed track angle),
 * then nudges the car's headingAngle toward that target (GameGetAngleDelta). Writes
 * the steer value into field_44 and the route sub-block (field_BC).
 * Register-pinned locals are match-load-bearing.
 */
void func_8003AEBC(GameCarRuntime *car) {
    GameCarRuntime *route;
    GameTrackPoint *point;
    s32 index;
    s32 offset;
    s32 lateral;
    s32 rem;
    s32 coords[3];
    s32 angle;
    register s32 value asm("$2");
    register s32 zValue asm("$5");
    register s32 lowerLimit asm("$3");
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
