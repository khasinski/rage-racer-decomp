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
 * Car route-steering update. Samples a look-ahead track point (two ahead or two
 * behind depending on the lap-direction flag g_RaceSeries), clamps the lateral
 * offset to the track half-width (`leftHalfWidth`/`rightHalfWidth`), projects the target point
 * off the centre-line along the inward normal (0x1000 - smoothed track angle),
 * then nudges the car's headingAngle toward that target (GetAngleDelta). Writes
 * the steer value into steeringAngle and the rival AI block at `aiEnabled`.
 * Register-pinned locals are match-load-bearing.
 */
void SteerCarAlongRoute(GameCarRuntime *car) {
    GameCarAiBlock *ai;
    GameTrackPoint *point;
    s32 index;
    s32 offset;
    s32 lateral;
    s32 rem;
    s32 coords[3];
    s32 angle;
    register s32 value asm("$2");
    s32 zValue;
    s32 lowerLimit;
    register s32 callArg asm("$4");

    lateral = car->aiLateralOffset;
    offset = car->trackPointIndex;
    ai = GetCarAiBlock(car);
    car->reservedDC = 0;

    if (ReadStableRaceSeries() != 0) {
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
    if (point->rightHalfWidth < lateral) {
        value = point->rightHalfWidth * car->normalizedLateralOffset;
        if (value < 0) {
            value += 0x7FF;
        }
        lateral = value >> 11;
    } else {
        value = point->leftHalfWidth;
        lowerLimit = -value;
        if (lateral < lowerLimit) {
            value = lowerLimit * car->normalizedLateralOffset;
            if (value < 0) {
                value += 0x7FF;
            }
            lateral = value >> 11;
        }
    }

    InterpolateTrackPoint(index, coords, car->segmentFraction);
    angle = 0x1000 - SmoothTrackAngle(index, car->segmentFraction);

    value = rsin(angle) * lateral;
    if (value < 0) {
        value += 0xFFF;
    }
    coords[0] += value >> 12;

    zValue = rcos(angle) * lateral;
    if (zValue < 0) {
        zValue += 0xFFF;
    }
    coords[2] += zValue >> 12;

    angle = 0x400 - Atan2(coords[0] - car->x, coords[2] - car->z);

    callArg = ReadStableRaceSeries();
    value = car->trackHeading.value;
    callArg = (callArg << 11) + 0xC00;
    value = -GetAngleDelta(callArg - value, angle);
    car->steeringAngle = value * 3;

    if (car->verticalMotionState == 0) {
        value = GetAngleDelta(car->headingAngle, angle);
        value += car->headingAngle;
        car->headingAngle = value;
        ai->targetYaw = value;
        car->bodyYaw = value;
    }
}


/*
 * Runs the rival-car update passes used by an interactive race. Cars 4..10
 * split their traffic-avoidance work across alternating frames.
 */
