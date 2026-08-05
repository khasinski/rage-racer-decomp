#include "common.h"

#include "game/audio.h"
#include "game/race.h"
#include "game/track.h"

extern s32 g_RaceCueFlags;
extern s16 g_PlayerTrackSection asm("D_8009E74C");
extern s16 g_PlayerLap asm("D_8009E83C");
extern s32 g_LapCount;

void TriggerRaceCues(void);
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

    rawBase = g_TrackEventData;
    current = g_RaceCueFlags;
    base = rawBase + 0x1CCC;

    if (!(current & 8)) {
        if (g_PlayerTrackSection == *(s16 *)((g_RaceSeries * 4) + (s32)base)) {
            entry = g_PlayerLap;
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

    stateBase = (u8 *)&g_PlayerTrackSection;
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
