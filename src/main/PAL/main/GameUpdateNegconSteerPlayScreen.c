#include "common.h"
#include "game/audio.h"
#include "game/menu.h"
#include "game/state.h"

extern u8 g_PadType asm("D_801E4369");

/* The 0..3 steering-play setting, signed here because the screen clamps it. */
extern s16 D_8019CAD0;
/* Its backup, taken by GameBeginNegconCalibration. */
extern u16 D_8019C75C;
/* The arrow pulse angle the setup screens advance every frame. */
extern s32 D_8007C13C;
/* One of the four controller-screen animation counters. */
extern s32 D_801E8A9C;

void func_80023750(s32 arg0);

/*
 * Game mode 10: pick the steering play with left/right, confirm with
 * start/cross (on to mode 11, the max-twist screen) or cancel with
 * circle/square. Cancelling - and unplugging the NeGcon - restores the
 * backed-up setting on the way back to mode 1.
 */
void GameUpdateNegconSteerPlayScreen(void) {
    g_AnimTimer++;
    D_8007C13C += 96;
    if (g_PadEdge2 & 0x90) {
        GamePlaySoundCue(3);
        g_GameMode = 1;
        D_8019CAD0 = D_8019C75C;
    } else if (g_PadEdge2 & 0x860) {
        GamePlaySoundCue(2);
        g_GameMode = 11;
    }
    if (g_PadEdge2 & 0x8000) {
        if (D_8019CAD0 > 0) {
            GamePlaySoundCue(8);
            D_8019CAD0 = D_8019CAD0 - 1;
        }
    }
    if (g_PadEdge2 & 0x2000) {
        if (D_8019CAD0 < 3) {
            GamePlaySoundCue(8);
            D_8019CAD0 = D_8019CAD0 + 1;
        }
    }
    if (g_PadType != 0x23) {
        g_GameMode = 1;
        D_8019CAD0 = D_8019C75C;
    }
    D_801E8A9C = -896;
    GameDrawNegconSteerPlayScreen();
    func_80023750(4);
    GameDrawControllerSetupScene(1);
}
