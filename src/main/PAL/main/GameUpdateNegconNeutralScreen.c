#include "common.h"
#include "game/audio.h"
#include "game/menu.h"
#include "game/state.h"

extern u8 g_PadType asm("D_801E4369");

/* The raw NeGcon axes in the BIOS pad buffer (D_801E403C + 4): steering,
 * then the three analog buttons. */
extern u8 D_801E4040;
extern u8 D_801E4041;
extern u8 D_801E4042;
extern u8 D_801E4043;

/* Where the calibrated neutral readings live. */
extern u16 D_801E4BF0;
extern u16 D_8019CA08;
extern u16 D_8019CA0A;
extern u16 D_8019CA0C;

void func_80023750(s32 arg0);

/*
 * Game mode 9: hold the NeGcon still and press start. Start latches the four
 * axes as the neutral point and advances to mode 10 (the steering play
 * screen); unplugging the NeGcon drops straight back to mode 1.
 */
void GameUpdateNegconNeutralScreen(void) {
    g_AnimTimer++;
    if (g_PadEdge2 & 0x800) {
        GamePlaySoundCue(2);
        g_GameMode = 10;
        D_801E4BF0 = D_801E4040 - 128;
        D_8019CA08 = D_801E4041;
        D_8019CA0A = D_801E4042;
        D_8019CA0C = D_801E4043;
    }
    if (g_PadType != 0x23) {
        g_GameMode = 1;
    }
    GameDrawNegconNeutralScreen();
    func_80023750(4);
    GameDrawControllerSetupScene(0);
}
