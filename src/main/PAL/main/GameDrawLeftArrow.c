#include "common.h"
#include "game/render.h"

/* Free-running angle the controller-setup screens pulse their arrows with. */
extern s32 D_8007C13C;

s32 func_80068568(s32 angle);

/* Wide-parameter view of the packet builders; see GameQueueSprite.c. */
u8 *QueueSpriteWide(
    void *ot,
    u8 *prim,
    s32 x,
    s32 y,
    s32 w,
    s32 h,
    s32 u,
    s32 v,
    s32 clutIndex) asm("func_80016EC4");
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
u8 *GameDrawLeftArrow(void *ot, u8 *prim, s32 x, s32 y, s32 pulse) asm("func_80014A60");

/* The 16x32 left arrow, plus - while `pulse` is set - a tile over it whose
 * green channel breathes with rsin of the shared arrow angle. */
u8 *GameDrawLeftArrow(void *ot, u8 *prim, s32 x, s32 y, s32 pulse) {
    prim = QueueSpriteWide(ot, prim, x, y, 0x10, 0x20, 0x48, 0xB8, 0x7F82);
    prim = GameQueueDrawModePrim(ot, prim, 0x39);
    if (pulse != 0) {
        u8 glow = func_80068568(D_8007C13C % 0x1000) / 64 - 65;

        prim = (u8 *)GameAddTilePrim(
            (s32)ot, (s32)prim, x, y, 0x10, 0x20, 0, glow, 0);
    }
    return prim;
}
