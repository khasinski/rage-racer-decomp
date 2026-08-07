#include "common.h"
#include "game/menu.h"
#include "game/render.h"
#include "game/scratchpad.h"
#include "game/state.h"
#include "psyq/gpu.h"

void DrawRaceEndBanner(s32 arg0);
s32 GameQueueDrawModePrimWide(u8 *arg0, s32 arg1, s32 arg2) asm("QueueDrawModePrim");

/* Scene 34: the still shown after the ending FMV. Fades in, waits 300
 * frames or a confirm press, fades out and returns to scene 2. */
void UpdateEndingStill(void) {
    s32 v0, v1;
    if ((g_SceneTimer = g_SceneTimer + 1) == 2) {
        SetDispMask(1);
    }
    v0 = g_FadeLevel + g_FadeStep;
    g_FadeLevel = v0;
    v1 = g_FadeStep;
    if (v1 > 0) {
        if (v0 >= 257) {
            g_FadeLevel = 0x100;
            g_FadeStep = 0;
        }
    } else if (v1 == 0) {
        if (g_SceneTimer == 0x12C || (g_PadEdge2 & 0x860)) {
            g_FadeLevel = 0x100;
            g_FadeStep = -4;
        }
    } else if (v0 == 0) {
        g_SceneId = 2;
    }
    DrawRaceEndBanner(g_FadeLevel);
}

/* The still itself: a 0x100 + 0x40 wide pair of full-height sprites. */
void DrawEndingStill(void) {
    u8 *base;
    s32 clut;
    s32 height;
    volatile s32 *scratch;
    s32 next;

    base = g_DrawBuffer + 0xCC;
    height = 0xF0;
    clut = 0x3FDB;
    scratch = (volatile s32 *)SCRATCHPAD_ADDR;

    next = *scratch;
    next = (s32)GameQueueSprite(base, next, 0, 0, 0x100, height, 0, 0, clut);
    next = GameQueueDrawModePrimWide(base, next, 6);
    next = (s32)GameQueueSprite(base, next, 0x100, 0, 0x40, height, 0, 0, clut);
    *scratch = GameQueueDrawModePrimWide(base, next, 7);
}
