#include "common.h"
#include "game/track.h"
#include "game/race.h"
#include "game/car.h"


void GameSeedCarLapProgress(u8 *arg0, s32 arg1) asm("func_8002BF68");
void GameSeedCarLapProgress(u8 *arg0, s32 arg1) {
    register GameCarRuntime *obj asm("$4") = (GameCarRuntime *)arg0;
    register s32 state asm("$2") = g_RaceSeries;
    register s32 cur asm("$9") = obj->trackPointIndex;
    register s32 total asm("$7") = 0;
    register s32 index asm("$6");

    obj->field_68 = 0;
    if (state != 0) {
        index = *(s32 *)g_TrackEventData;
        if (arg1 == 1) {
            register s32 count asm("$5");
            register u8 *table asm("$8");
            s32 wrapped;

            count = g_TrackPointCount;
            table = (u8 *)g_TrackPoints;
advance_forward_add:
            index++;
            wrapped = index % count;
            if (cur == wrapped) {
                goto done;
            }
            total += ((GameTrackPoint *)table)[wrapped].segmentLength;
            goto advance_forward_add;
        } else {
            register s32 count asm("$8");
            register u8 *table asm("$10");
            register s32 wrapped asm("$5");
            s32 mod;

            count = g_TrackPointCount;
            table = (u8 *)g_TrackPoints;
advance_backward_sub:
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
            goto advance_backward_sub;
        }
    } else {
        index = *(s32 *)g_TrackEventData;
        if (arg1 == 0) {
            register s32 count asm("$5");
            register u8 *table asm("$8");
            s32 wrapped;

            count = g_TrackPointCount;
            table = (u8 *)g_TrackPoints;
advance_forward_sub:
            do {
                index++;
                wrapped = index % count;
                total -= ((GameTrackPoint *)table)[wrapped].segmentLength;
            } while (cur != wrapped);
            goto done;
        } else {
            register s32 count asm("$8");
            register u8 *table asm("$10");
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
