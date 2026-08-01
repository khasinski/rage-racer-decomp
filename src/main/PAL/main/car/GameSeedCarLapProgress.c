#include "common.h"
#include "game/track.h"
#include "game/race.h"
#include "game/car.h"


void GameSeedCarLapProgress(u8 *arg0, s32 arg1) asm("func_8002BF68");
void GameSeedCarLapProgress(u8 *arg0, s32 arg1) {
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
                goto done;
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
                goto done;
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
advance_forward_sub:
            do {
                index++;
                wrapped = index % count;
                total -= ((GameTrackPoint *)table)[wrapped].segmentLength;
            } while (cur != wrapped);

        } else {
            /* These pins are load-bearing: removing any one changes .text. */
            register s32 count asm("$8");
            u8 *table;
            register s32 wrapped asm("$5");
            s32 mod;

            count = g_TrackPointCount;
            table = (u8 *)g_TrackPoints;
advance_backward_add:
            do {
                if (index < 0) {
                    wrapped = index + count;
                } else {
                    wrapped = index;
                }
                if (cur == wrapped) {
                    break;
                }
                mod = wrapped % count;
                total += ((GameTrackPoint *)table)[mod].segmentLength;
                index--;
            } while (1);
        }
    }
done:
    obj->field_68 = total;
}
