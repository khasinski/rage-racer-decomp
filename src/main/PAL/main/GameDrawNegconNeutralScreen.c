#include "common.h"
#include "game/render.h"
#include "game/state.h"

/* "Hold the "NeGcon" in an untwisted" / "position and press start button." */
extern char D_80010028[];
extern char D_8001004C[];

void func_80027874(s32 x, s32 y, char *str, s32 clutIndex);
s32 GameAddTilePrim(
    s32 ot,
    s32 prim,
    s32 x,
    s32 y,
    s32 w,
    s32 h,
    s32 r,
    s32 g,
    s32 b) asm("func_80032F34");

/* Game mode 9's own overlay: the two lines of instructions over a black panel
 * inside a white border, both drawn into the 0xD0 sub-buffer of the current
 * draw buffer from the shared scratchpad packet cursor. */
void GameDrawNegconNeutralScreen(void) {
    s32 *cursor = (s32 *)0x1F800000;
    s32 ot;
    s32 prim;

    func_80027874(0x18, 0x30, D_80010028, 0x7F81);
    func_80027874(0x18, 0x48, D_8001004C, 0x7F81);
    ot = (s32)(g_DrawBuffer + 0xD0);
    prim = *cursor;
    prim = GameAddTilePrim(ot, prim, 0, 0x28, 0x124, 0x40, 0, 0, 0);
    *cursor = GameAddTilePrim(ot, prim, 0, 0x26, 0x125, 0x44, 0xFF, 0xFF, 0xFF);
}
