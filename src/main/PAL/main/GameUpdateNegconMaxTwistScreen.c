#include "common.h"
#include "game/audio.h"
#include "game/menu.h"
#include "game/state.h"

extern u8 g_PadType asm("D_801E4369");

/* The 0..3 twist range setting, signed here because the screen clamps it. */
extern s16 D_801E418C;
/* Its backup, taken by GameBeginNegconCalibration. */
extern u16 D_8019CB04;
/* One of the four controller-screen animation counters. */
extern s32 D_801E8A9C;

void func_80023750(s32 arg0);

/*
 * Game mode 11: pick the maximum twist range with left/right, confirm with
 * start/cross or cancel with circle/square. Cancelling - and unplugging the
 * NeGcon - restores the backed-up setting on the way back to mode 1.
 */
void GameUpdateNegconMaxTwistScreen(void) {
    g_AnimTimer++;
    if (g_PadEdge2 & 0x90) {
        GamePlaySoundCue(3);
        g_GameMode = 1;
        D_801E418C = D_8019CB04;
    } else if (g_PadEdge2 & 0x860) {
        GamePlaySoundCue(2);
        g_GameMode = 1;
    }
    if (g_PadEdge2 & 0x8000) {
        if (D_801E418C > 0) {
            GamePlaySoundCue(8);
            D_801E418C = D_801E418C - 1;
        }
    }
    if (g_PadEdge2 & 0x2000) {
        if (D_801E418C < 3) {
            GamePlaySoundCue(8);
            D_801E418C = D_801E418C + 1;
        }
    }
    if (g_PadType != 0x23) {
        g_GameMode = 1;
        D_801E418C = D_8019CB04;
    }
    D_801E8A9C = -896;
    GameDrawNegconMaxTwistScreen();
    func_80023750(4);
    GameDrawControllerSetupScene(1);
}
