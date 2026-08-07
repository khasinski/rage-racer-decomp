#include "common.h"
#include "game/car.h"
#include "game/track.h"
#include "game/race.h"

void BeginCarStandingStart(u8 *arg0) {
    s32 value;
    s16 index;

    value = ((g_EngineRpm - g_PeakOutputRpm) * 10000) / g_CarSpec->revLimit;
    D_801E8A4C = 0;

    if (value < 0) {
        value = g_EngineRpm - 1000;
        if (g_EngineRpm < 2000) {
            value = 0;
        }
    } else {
        index = *(s16 *)(arg0 + 0x132);
        value *= g_PeakOutputValue / ((index * 200) + 300);
        *(s32 *)(arg0 + 0x150) = *(s32 *)(arg0 + 0x150) / index;
        if (*(s16 *)(arg0 + 0x132) >= 2) {
            g_GripLossTimer = 200;
        }
    }

    g_StandingStartSpin = value;
}

void SeedCarLapProgress(u8 *arg0, s32 arg1) {
    GameCarRuntime *obj = (GameCarRuntime *)arg0;
    s32 state = g_RaceSeries;
    s32 cur = obj->trackPointIndex;
    s32 total = 0;
    s32 index;

    obj->field_68 = 0;
    if (state != 0) {
        index = *(s32 *)g_TrackEventData;
        if (arg1 == 1) {
            s32 count;
            u8 *table;
            s32 wrapped;

            count = g_TrackPointCount;
            table = (u8 *)g_TrackPoints;
while (1) {
            index++;
            wrapped = index % count;
            if (cur == wrapped) {
                break;
            }
            total += ((GameTrackPoint *)table)[wrapped].segmentLength;
            }
        } else {
            s32 count;
            u8 *table;
            s32 wrapped;
            s32 mod;

            count = g_TrackPointCount;
            table = (u8 *)g_TrackPoints;
while (1) {
            if (index < 0) {
                wrapped = index + count;
            } else {
                wrapped = index;
            }
            mod = wrapped % count;
            total -= ((GameTrackPoint *)table)[mod].segmentLength;
            if (cur == wrapped) {
                break;
            }
            index--;
            }
        }
    } else {
        index = *(s32 *)g_TrackEventData;
        if (arg1 == 0) {
            s32 count;
            u8 *table;
            s32 wrapped;

            count = g_TrackPointCount;
            table = (u8 *)g_TrackPoints;
            do {
                index++;
                wrapped = index % count;
                total -= ((GameTrackPoint *)table)[wrapped].segmentLength;
            } while (cur != wrapped);

        } else {
            s32 count;
            u8 *table;
            s32 mod;

            count = g_TrackPointCount;
            table = (u8 *)g_TrackPoints;
            do {
                if (index < 0) {
                    arg1 = index + count;
                } else {
                    arg1 = index;
                }
                if (cur == arg1) {
                    break;
                }
                mod = arg1 % count;
                total += ((GameTrackPoint *)table)[mod].segmentLength;
                index--;
            } while (1);
        }
    }
    obj->field_68 = total;
}

s32 FindTrackSegment(GameCarRuntime * car,s32 idx);

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
