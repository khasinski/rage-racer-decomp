#include "common.h"
#include "game/car.h"
#include "game/track.h"
#include "game/race.h"


s32 FindTrackSegment(GameCarRuntime *car, s32 idx);

/*
 * Lap-progress accumulator. Relocates the car's trackPointIndex to the segment
 * that now contains it (FindTrackSegment), then walks the intervening points and
 * adds (forward) or subtracts (backward) their segmentLength into
 * car->field_68 (progress). The two mirror-image branches select forward vs
 * reverse lap direction from the direction flag g_RaceSeries. Register-pinned
 * locals (bv/ir) are load-bearing for the match.
 */
void AccumulateLapProgress(GameCarRuntime *car) {
    s32 r;
    s32 n;
    s32 i;
    s32 j;
    s32 idx;
    s32 fwd;
    s32 back;
    register s32 bv asm("$2");
    s32 count;
    GameTrackPoint *array;

    n = 1;
    r = FindTrackSegment(car, car->trackPointIndex);
    if (r < 0) {
        car->activeFlag = -1;
        return;
    }

    if (g_RaceSeries == 0) {
        if (r != car->trackPointIndex) {
            count = g_TrackPointCount;
            array = g_TrackPoints;
            do {
                j = car->trackPointIndex - n;
                back = j;
                if (j < 0) {
                    back = j + count;
                }
                fwd = (car->trackPointIndex + n) % count;
                if (r == back) {
                    s32 ir;
                    for (i = 0; i < n; i++) {
                        j = car->trackPointIndex - i;
                        ir = j;
                        if (j < 0) {
                            ir = j + count;
                        }
                        car->field_68 += array[ir].segmentLength;
                    }
                    break;
                }
                if (r == fwd) {
                    for (i = 1; i <= n; i++) {
                        car->field_68 -= array[(car->trackPointIndex + i) % count].segmentLength;
                    }
                    break;
                }
                n++;
            } while (r != car->trackPointIndex);
        }
    } else {
        if (r != car->trackPointIndex) {
            count = g_TrackPointCount;
            array = g_TrackPoints;
            do {
                j = car->trackPointIndex - n;
                back = j;
                if (j < 0) {
                    back = j + count;
                }
                fwd = (car->trackPointIndex + n) % count;
                bv = back;
                if (r == fwd) {
                    for (i = 0; i < n; i++) {
                        car->field_68 += array[(car->trackPointIndex + i) % count].segmentLength;
                    }
                    break;
                }
                if (r == bv) {
                    register s32 ir asm("$3");
                    for (i = 1; i <= n; i++) {
                        j = car->trackPointIndex - i;
                        ir = j;
                        if (j < 0) {
                            ir = j + count;
                        }
                        car->field_68 -= array[ir].segmentLength;
                    }
                    break;
                }
                n++;
            } while (r != car->trackPointIndex);
        }
    }
    car->trackPointIndex = r;
}
