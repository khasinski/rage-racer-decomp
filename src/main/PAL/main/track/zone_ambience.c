#include "common.h"
#include "game/race.h"
#include "game/player_car_internal.h"
#include "game/track.h"
#include "game/audio.h"

void UpdateZoneAmbience(s32 zone) {
    s32 position;
    s32 base;
    register s32 mode asm("$4");
    s32 value;
    s32 finalValue;
    s32 i;
    s32 sentinel;
    TrackAmbienceZone *entryBase;
    register TrackAmbienceZone *entry asm("$3");
    s32 selector;

    position = zone;
    selector = g_GrandPrixClass;
    entryBase = g_TrackEventData->ambienceZones;
    selector = selector % 5;

    switch (selector) {
    default:
    case 0:
    case 5:
        base = 0;
        mode = 0;
        break;
    case 1:
    case 2:
        base = 0x60;
        mode = 0;
        break;
    case 3:
    case 4:
        base = 0x60;
        mode = 1;
        break;
    }

    if (g_RaceSeries != 0) {
        position = g_TrackLength - position;
    }

    value = 0;
    i = 0;
    sentinel = -1;
    entry = entryBase;
    while (i < 4) {
        s32 start;
        s32 end;

        start = entry->start;
        end = entry->end;
        if (start == sentinel) {
            break;
        }
        if (!(position < start) && !(end < position)) {
            s32 flags;
            s32 delta;
            s32 product;
            s32 flagBit;

            flags = entry->flags;
            if (position < start + 0x320 && (flags & 1) > 0) {
                delta = position - start;
                product = base * delta;
                value = product / 800;
            } else if (end - 0x320 < position && ((flagBit = flags & 2), flagBit > 0)) {
                delta = end - position;
                product = base * delta;
                value = product / 800;
            } else {
                value = base;
            }
            break;
        }
        i++;
        entry++;
    }

    finalValue = value;
    finalValue = (s16)finalValue;
    SetStereoSoundCue(mode, finalValue, finalValue);
}


void TriggerRaceCues(void) {
    u8 *rawBase;
    u8 *base;
    s32 i;
    s32 mask;
    u8 *stateBase;
    register s32 temp asm("v0");
    register s32 entry asm("v1");
    register s32 loopFlags asm("t0");
    register s32 current asm("a0");
    s32 product;

    rawBase = (u8 *)g_TrackEventData;
    current = g_RaceCueFlags;
    base = rawBase + 0x1CCC;

    if (!(current & 8)) {
        if (g_PlayerCar.trackSection == *(s16 *)((g_RaceSeries * 4) + (s32)base)) {
            entry = g_PlayerCar.lap;
            if (entry == g_LapCount) {
                entry = current | 8;
                g_RaceCueFlags = entry;
                if (g_WrongWayTimer < 10) {
                    PlaySoundCue(0x2A);
                }
            }
        }
    }

    if (g_WrongWayTimer != 0) {
        return;
    }

    stateBase = (u8 *)&g_PlayerCar.trackSection;
    i = 0;
    temp = 0x10;
    do {
        loopFlags = g_RaceCueFlags;
        mask = temp << i;
        temp = mask & loopFlags;
        if (temp == 0) {
            temp = g_RaceSeries;
            entry = ((temp * 3) + i) << 2;
            entry += (s32)base;
            current = *(s16 *)(entry + 0x10);
            temp = -1;
            if (current == temp) {
                return;
            }

            temp = *(s16 *)stateBase;
            if (temp == current) {
                entry = *(s16 *)(entry + 0x12);
                temp = *(s32 *)(stateBase + 0xD0);
                product = entry * temp;
                temp = product / 100;
                entry = *(s32 *)(stateBase + 0x2C);
                if (temp < entry) {
                    temp = *(s16 *)(stateBase + 0xA);
                    if (temp <= 0) {
                        temp = mask | loopFlags;
                        g_RaceCueFlags = temp;
                        PlaySoundCue(0x23);
                    }
                }
                return;
            }
        }
        i++;
        temp = i < 3;
        if (temp == 0) {
            break;
        }
        temp = 0x10;
    } while (1);
}
