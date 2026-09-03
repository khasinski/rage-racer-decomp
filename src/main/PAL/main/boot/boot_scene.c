#include "common.h"
#include "game/prim.h"
#include "game/asset.h"
#include "game/menu.h"
#include "game/race.h"
#include "game/render.h"
#include "game/render_internal.h"
#include "game/scratchpad.h"
#include "game/state.h"
#include "psyq/gpu.h"
#include "psyq/gte.h"


void DrawBootLogo(void) {
    u8 *base;
    s32 height;
    s32 clut;
    void **scratch;
    s32 fade;
    s32 value;
    void *next;

    fade = g_SceneTimer;
    if (fade >= 0) {
        value = fade;
        if (value >= 0x100) {
            value = 0xFF;
        }
    } else {
        value = 0;
    }
    fade = value;

    base = g_DrawBuffer + 0xCC;
    scratch = &SCRATCH_PRIM_CURSOR_AS(void);

    next = *scratch;
    next = GameQueueShadedSprite(base, next, 0x64, 0xEC, 0x7C, 0x18, 0x80, 0, 0x3F97, fade);

    height = 0x20;
    clut = 0x3FD7;
    next = GameQueueShadedSprite(base, next, 0xDC, 0xC4, 8, 0x10, 0, height, clut, fade);
    next = GameQueueShadedSprite(base, next, 0x64, 0xC4, 0x78, height, 0, 0, clut, fade);
    *scratch = QueueDrawModePrim(base, next, 5);
}
