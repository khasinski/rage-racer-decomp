#include "common.h"
#include "game/prim.h"
#include "game/asset.h"
#include "game/audio.h"
#include "game/menu.h"
#include "game/menu_internal.h"
#include "game/frontend_internal.h"
#include "game/race.h"
#include "game/random.h"
#include "game/render.h"
#include "game/render_internal.h"
#include "game/scratchpad.h"
#include "game/screens.h"
#include "game/state.h"
#include "psyq/cd.h"
#include "psyq/gpu.h"

#define FRAME_CONTEXT_ENVIRONMENT(offset)                                      \
    (*(volatile GameFrameEnvironmentHeader *)(g_FrameContexts[0].bytes + (offset)))



/*
 * Empty stub; SetupDisplay240 and SetupDisplay480 both call it with one argument,
 * so the parameter is declared and ignored.
 */
void ResetFrameContext(int buffer) {
}


void SetupDisplay240(s32 r, s32 g, s32 b) {
    GameFrameContext *context;
    u8 *base;
    s32 height;
    u16 *src0;
    u16 *src1;
    s32 i;
    s32 offset;
    s32 one;
    s32 stride;
    u16 value;
    u16 value2;

    SetGeomOffset(0xA0, 0x78);
    SetGeomScreen(0x140);

    context = g_FrameContexts;
    base = context->bytes;
    height = 0xF0;
    SetDefDrawEnv(&context->environment.draw, 0, 0, 0x140, height);
    SetDefDrawEnv(&g_DrawEnv1, 0, 0xF0, 0x140, height);
    SetDefDispEnv(&context->environment.display, 0, 0xF0, 0x140, height);
    SetDefDispEnv(&g_FrameContexts[1].environment.display, 0, 0, 0x140, height);

    {
        register DrawEnv *ptr;
        register s32 g;
        register s32 b;
        register s32 smallWidth;
        s32 small_height;

        ptr = &context->environment.mirrorDraw;
        g = 0x56;
        b = 0x12;
        smallWidth = 0x94;
        small_height = 0x24;
        SetDefDrawEnv(ptr, g, b, smallWidth, small_height);
        SetDefDrawEnv(&g_MirrorDrawEnv1, 0x56, 0x102, 0x94, small_height);
    }

    i = 0;
    one = 1;
    src0 = &g_ScreenOffsetX.displayValue;
    src1 = &g_ScreenOffsetY.displayValue;
    offset = 0;
    do {
        stride = 0x20000;
        FRAME_CONTEXT_ENVIRONMENT(offset).draw.dtd = one;
        FRAME_CONTEXT_ENVIRONMENT(offset).draw.isbg = one;
        FRAME_CONTEXT_ENVIRONMENT(offset).draw.r0 = r;
        FRAME_CONTEXT_ENVIRONMENT(offset).draw.g0 = g;
        FRAME_CONTEXT_ENVIRONMENT(offset).draw.b0 = b;
        value = *src0;
        stride |= 0x37E8;
        FRAME_CONTEXT_ENVIRONMENT(offset).display.screen.x = value;
        value2 = *src1;
        i++;
        FRAME_CONTEXT_ENVIRONMENT(offset).mirrorDraw.dtd = one;
        FRAME_CONTEXT_ENVIRONMENT(offset).mirrorDraw.isbg = 0;
        FRAME_CONTEXT_ENVIRONMENT(offset).mirrorDraw.r0 = r;
        FRAME_CONTEXT_ENVIRONMENT(offset).mirrorDraw.g0 = g;
        FRAME_CONTEXT_ENVIRONMENT(offset).mirrorDraw.b0 = b;
        FRAME_CONTEXT_ENVIRONMENT(offset).display.screen.y = value2 + 29;
        offset += stride;
    } while (i < 2);

    ResetFrameContext(0);
    ResetFrameContext(1);

}

void SetupDisplay480(s32 mode, s32 x, s32 y) {
    GameFrameContext *context = g_FrameContexts;
    u8 *base = context->bytes;
    s32 height;
    u16 *src0;
    u16 *src1;
    s32 i;
    s32 offset;
    s32 one;
    s32 stride;
    u16 value;
    u16 value2;

    SetGeomOffset(0xA0, 0xF0);
    SetGeomScreen(0x140);

    height = 0x1E0;
    SetDefDrawEnv(&context->environment.draw, 0, 0, 0x140, height);
    SetDefDrawEnv(&g_DrawEnv1, 0, 0, 0x140, height);
    SetDefDispEnv(&context->environment.display, 0, 0, 0x140, height);
    SetDefDispEnv(&g_FrameContexts[1].environment.display, 0, 0, 0x140, height);

    i = 0;
    one = 1;
    src0 = &g_ScreenOffsetX.displayValue;
    src1 = &g_ScreenOffsetY.displayValue;
    offset = 0;
    do {
        stride = 0x20000;
        FRAME_CONTEXT_ENVIRONMENT(offset).draw.dtd = one;
        FRAME_CONTEXT_ENVIRONMENT(offset).draw.isbg = one;
        FRAME_CONTEXT_ENVIRONMENT(offset).draw.r0 = mode;
        FRAME_CONTEXT_ENVIRONMENT(offset).draw.g0 = x;
        FRAME_CONTEXT_ENVIRONMENT(offset).draw.b0 = y;
        value = *src0;
        stride |= 0x37E8;
        FRAME_CONTEXT_ENVIRONMENT(offset).display.screen.x = value;
        value2 = *src1;
        i++;
        FRAME_CONTEXT_ENVIRONMENT(offset).mirrorDraw.dtd = one;
        FRAME_CONTEXT_ENVIRONMENT(offset).mirrorDraw.isbg = 0;
        FRAME_CONTEXT_ENVIRONMENT(offset).mirrorDraw.r0 = mode;
        FRAME_CONTEXT_ENVIRONMENT(offset).mirrorDraw.g0 = x;
        FRAME_CONTEXT_ENVIRONMENT(offset).mirrorDraw.b0 = y;
        FRAME_CONTEXT_ENVIRONMENT(offset).display.screen.y = value2 + 29;
        offset += stride;
    } while (i < 2);

    ResetFrameContext(0);
    ResetFrameContext(1);

    SCRATCH_CLIP_Y1 = 0x1E0;
}


#undef FRAME_CONTEXT_ENVIRONMENT
