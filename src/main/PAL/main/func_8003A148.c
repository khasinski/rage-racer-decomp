#include "common.h"
#include "game/car.h"
#include "game/track.h"

extern GameTrackPoint *D_8009E688;

void func_8003A148(GameCarRuntime *car, s32 arg1) {
    register GameCarRuntime *carReg asm("$4") = car;
    register u8 *state asm("$8");
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
        register GameTrackPoint *point asm("$2");
        register s32 scaled asm("$2");

        if (current < 0) {
            trackIndex = carReg->trackPointIndex;
            point = &D_8009E688[trackIndex];
            limit = point->field_10;
        } else {
            trackIndex = carReg->trackPointIndex;
            point = &D_8009E688[trackIndex];
            limit = point->field_12;
        }
        scaled = limit << 2;
        scaled += limit;
        if (scaled < 0) {
            scaled += 7;
        }
        limit = scaled >> 3;
    } else {
        register GameTrackPoint *point asm("$2");

        if (current < 0) {
            trackIndex = carReg->trackPointIndex;
            point = &D_8009E688[trackIndex];
            limit = (point->field_10 << 2) / 7;
        } else {
            trackIndex = carReg->trackPointIndex;
            point = &D_8009E688[trackIndex];
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
