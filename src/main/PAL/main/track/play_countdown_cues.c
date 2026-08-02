#include "common.h"
#include "game/race.h"
#include "game/track.h"

extern s16 g_RaceCueDelay asm("D_801E6C90");

void GamePlaySoundCue(s32 cue) asm("func_8005D6EC");

void GamePlayCountdownCues(s32 arg0) asm("func_800410BC");
void GamePlayCountdownCues(s32 arg0) {
    if (arg0 < 0x110) {
        if (arg0 == 0x79) {
            GamePlaySoundCue(0x1E);
        } else if (arg0 == 0x97) {
            GamePlaySoundCue(0x1F);
        } else if (arg0 == 0xB5) {
            GamePlaySoundCue(0x20);
        } else if (arg0 == 0xD3) {
            GamePlaySoundCue(0x21);
        } else if ((arg0 == 0x10F) && (g_GrandPrixMode == 1)) {
            GamePlaySoundCue(0x22);
            g_RaceCueDelay = 0x5A;
        }
    }
}

extern s32 g_PlayerTrackProgress asm("D_8009E744");

void GameUpdateRivalCueGate(void) asm("func_80041170");
void GameUpdateRivalCueGate(void) {
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

extern u32 *g_CourseObjects asm("D_801E4B2C");
extern u32 g_CourseObjectCount asm("D_801E4BBC");

void GameSetCourseObjects(u32 *arg0) asm("func_8004121C");
void GameSetCourseObjects(u32 *arg0) {
    u32 value;

    value = *arg0;
    g_CourseObjects = arg0 + 1;
    g_CourseObjectCount = value;
}
