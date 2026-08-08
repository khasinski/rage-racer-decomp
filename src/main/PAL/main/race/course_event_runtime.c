#include "common.h"
#include "game/race.h"
#include "game/track.h"
#include "game/audio.h"

void PlayCountdownCues(s32 timer) {
    if (timer < 0x110) {
        if (timer == 0x79) {
            PlaySoundCue(0x1E);
        } else if (timer == 0x97) {
            PlaySoundCue(0x1F);
        } else if (timer == 0xB5) {
            PlaySoundCue(0x20);
        } else if (timer == 0xD3) {
            PlaySoundCue(0x21);
        } else if ((timer == 0x10F) && (g_GrandPrixMode == 1)) {
            PlaySoundCue(0x22);
            g_RaceCueDelay = 0x5A;
        }
    }
}

extern s32 g_PlayerTrackProgress;

void UpdateRivalCueGate(void) {
    s32 value;

    value = g_PlayerTrackProgress;
    if (((value < 0x7001) || (value >= g_TrackLength - 0x3000)) && (g_RivalCueEnabled == 1)) {
        g_RivalCueEnabled = 0;
    } else if ((value >= 0x7001) && (value < g_TrackLength - 0x3000)) {
        g_RivalCueEnabled = 1;
    }

    if (g_WrongWayTimer >= 0xA) {
        g_RivalCueEnabled = 0;
    }
}

extern u32 *g_CourseObjects;
extern u32 g_CourseObjectCount;

void SetCourseObjects(u32 *table) {
    u32 value;

    value = *table;
    g_CourseObjects = table + 1;
    g_CourseObjectCount = value;
}
