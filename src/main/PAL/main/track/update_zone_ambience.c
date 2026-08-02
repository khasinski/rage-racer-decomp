#include "common.h"
#include "game/race.h"
#include "game/track.h"


typedef struct UnkFunc80040DB4Entry {
    s32 start;
    s32 end;
    u16 unk8;
    u16 flags;
} UnkFunc80040DB4Entry;

void func_8005C31C(s32 arg0, s32 arg1, s32 arg2);

void UpdateZoneAmbience(s32 arg0) asm("func_80040DB4");
void UpdateZoneAmbience(s32 arg0) {
    s32 position;
    s32 base;
    register s32 mode asm("$4");
    s32 value;
    s32 finalValue;
    s32 i;
    s32 sentinel;
    s32 entryBaseValue;
    UnkFunc80040DB4Entry *entryBase;
    register UnkFunc80040DB4Entry *entry asm("$3");
    s32 selector;

    position = arg0;
    selector = g_GrandPrixClass;
    entryBaseValue = (s32)g_TrackEventData;
    entryBase = (UnkFunc80040DB4Entry *)(entryBaseValue + 0x1C9C);
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
    func_8005C31C(mode, finalValue, finalValue);
}
