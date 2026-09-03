#include "common.h"
#include "game/prim.h"
#include "game/render.h"
#include "game/scratchpad.h"
#include "game/state.h"
#include "psyq/gpu.h"

/* The still itself: a 0x100 + 0x40 wide pair of full-height sprites. */
void DrawEndingStill(void) {
    u8 *base;
    s32 clut;
    s32 height;
    u8 *volatile *scratch;
    u8 *next;

    base = g_DrawBuffer + 0xCC;
    height = 0xF0;
    clut = 0x3FDB;
    scratch = SCRATCH_PRIM_CURSOR_SLOT;

    next = *scratch;
    next = GameQueueSprite(base, next, 0, 0, 0x100, height, 0, 0, clut);
    next = QueueDrawModePrim(base, next, 6);
    next = GameQueueSprite(base, next, 0x100, 0, 0x40, height, 0, 0, clut);
    *scratch = QueueDrawModePrim(base, next, 7);
}
