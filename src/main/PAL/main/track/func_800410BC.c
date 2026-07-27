#include "common.h"
#include "game/race.h"
#include "game/track.h"

extern s16 D_801E6C90;

void GamePlaySoundCue(s32 cue) asm("func_8005D6EC");

void func_800410BC(s32 arg0) {
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
            D_801E6C90 = 0x5A;
        }
    }
}

extern s32 g_PlayerTrackProgress asm("D_8009E744");
extern s16 D_8009E6A0;
extern s16 D_801E8A8C;

void func_80041170(void) {
    s32 value;

    value = g_PlayerTrackProgress;
    if ((value < 0x7001) || (value >= g_TrackLength - 0x3000)) {
        if (D_8009E6A0 == 1) {
            D_8009E6A0 = 0;
            goto check_count;
        }
    }

    if ((value >= 0x7001) && (value < g_TrackLength - 0x3000)) {
        D_8009E6A0 = 1;
    }

check_count:
    if (D_801E8A8C >= 0xA) {
        D_8009E6A0 = 0;
    }
}

extern u32 *D_801E4B2C;
extern u32 D_801E4BBC;

void func_8004121C(u32 *arg0) {
    u32 value;

    value = *arg0;
    D_801E4B2C = arg0 + 1;
    D_801E4BBC = value;
}
