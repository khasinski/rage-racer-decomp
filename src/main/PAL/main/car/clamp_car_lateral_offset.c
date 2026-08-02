#include "common.h"
#include "game/car.h"
#include "game/track.h"


/*
 * Clamps the car's lateral offset (field_11C) to a fraction of the track
 * half-width at its current point: the left half-width (field_10) when offset
 * is negative, the right half-width (field_12) otherwise. `arg1` selects the
 * scaling: <4 uses 5/8 of the half-width, else 4/7. Writes the clamped value
 * back into the route sub-block (field_BC + 0x60) only if it would exceed the
 * limit.
 */
void GameClampCarLateralOffset(GameCarRuntime *car, s32 arg1) asm("func_8003A148");
void GameClampCarLateralOffset(GameCarRuntime *car, s32 arg1) {
    GameCarRuntime *carReg = car;
    u8 *state;
    s32 current;
    s32 magnitude;
    s32 limit;
    s32 trackIndex;

    current = carReg->field_11C;
    state = (u8 *)&carReg->field_BC;
    magnitude = current;
    if (current < 0) {
        magnitude = -current;
    }

    if (arg1 < 4) {
        GameTrackPoint *point;
        s32 scaled;

        if (current < 0) {
            trackIndex = carReg->trackPointIndex;
            point = &g_TrackPoints[trackIndex];
            limit = point->field_10;
        } else {
            trackIndex = carReg->trackPointIndex;
            point = &g_TrackPoints[trackIndex];
            limit = point->field_12;
        }
        scaled = limit << 2;
        scaled += limit;
        if (scaled < 0) {
            scaled += 7;
        }
        limit = scaled >> 3;
    } else {
        GameTrackPoint *point;

        if (current < 0) {
            trackIndex = carReg->trackPointIndex;
            point = &g_TrackPoints[trackIndex];
            limit = (point->field_10 << 2) / 7;
        } else {
            trackIndex = carReg->trackPointIndex;
            point = &g_TrackPoints[trackIndex];
            limit = (point->field_12 << 2) / 7;
        }
    }

    if (limit < magnitude) {
        if (current > 0) {
            *(s16 *)(state + 0x60) = limit;
        } else {
            *(s16 *)(state + 0x60) = -limit;
        }
    }
}
