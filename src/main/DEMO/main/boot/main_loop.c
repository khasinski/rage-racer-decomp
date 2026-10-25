#include "common.h"
#include <stdio.h>
#include "game/asset.h"
#include "game/audio.h"
#include "game/cd.h"
#include "game/memcard.h"
#include "game/race.h"
#include "game/render.h"
#include "game/render_internal.h"
#include "game/scratchpad.h"
#include "game/state.h"
#include "game/input_internal.h"
#include "psyq/cd.h"
#include "psyq/gpu.h"
#include "psyq/kernel.h"
#include "psyq/snd.h"

/* A pre-release test-harness entry point, still reached directly from
 * _start's tail call with the BIOS-supplied argc/argv-style registers: the
 * second argument is a small parameter block rather than nothing. Retail's
 * MainLoop (src/main/PAL/main/boot/main_loop.c) replaced this with a plain
 * void(void) infinite loop; this earlier build still has the bounded
 * test-mode frame budget and shutdown path a PSYQ sample program would use. */
typedef struct TestHarnessParams {
    s32 mode;
    s32 frameLimitParam;
    s32 param;
} TestHarnessParams;

extern s32 g_TestMode;
extern s32 g_TestFrameLimit;
extern s32 g_TestParam;
extern s32 g_TestFrameCount;
extern u16 g_TestExitFlags;

s32 main_loop(s32 argc, TestHarnessParams *params) {
    s32 frameLimit;
    s32 elapsed;
    s32 ticks;
    s32 value;

    __main();
    value = params->mode;
    g_TestMode = value;
    if (value != 0) {
        value = params->frameLimitParam * 50;
    } else {
        value = 0xBB8;
    }
    g_TestFrameLimit = value;
    value = params->param;
    g_TestFrameCount = 0;
    g_TestParam = value;
    KernelCallbackSlot3();
    CdInit();
    InitSubsystems();
    InitAssetSystem();
    ResetGraph(3);
    InitCdAudio();
    g_FrameSyncThreshold = 0x80;
    SetDispMask(0);
    SetupDisplay480(0, 0, 0);
    g_SceneTimer = 0;
    g_SceneId = 1;
    RequestBootAssets();
    g_GameClock = 0;
    g_FrameCounter = 0;
    for (;;) {
        s32 parity = g_FrameCounter & 1;
        u8 *frame = g_FrameContexts[0].bytes + parity * GAME_FRAME_CONTEXT_SIZE;

        g_DrawBuffer = frame;
        g_FrameParity = parity;
        SCRATCH_OT_BASE_AS(u8) = frame + 0xCC;
        {
            GameFrameContextAddress frameAddress;
            frameAddress.bytes = frame;
            SCRATCH_PRIM_CURSOR_AS(u8) = frameAddress.context->layout.primitiveBuffer;
            ClearOTagR(frameAddress.context->layout.orderingTables[0], GAME_FRAME_OT_LENGTH);
        }
        {
            GameFrameContextAddress drawBuffer;
            drawBuffer.bytes = g_DrawBuffer;
            ClearOTagR(drawBuffer.context->layout.orderingTables[1], GAME_FRAME_OT_LENGTH);
        }
        TickCdAudio();
        TickSequenceAudio();
        ServiceAssetLoad();
        AdvanceSaveHeaderCounter();
        g_SceneHandlers[g_SceneId]();
        DrawSync(0);
        StepTrackTextureSwap();

        if (g_TestFrameCount == -1) {
            goto shutdown;
        }
        if (g_FrameSyncThreshold == 0x80) {
            g_TestFrameCount = g_TestFrameCount + 1;
        } else {
            g_TestFrameCount = g_TestFrameCount + 2;
        }
        if (g_TestMode != 0) {
            if (g_TestExitFlags & 0x800) {
                goto shutdown;
            }
        } else {
            if (g_TestExitFlags != 0) {
                g_TestFrameCount = 0;
            }
        }
        if (g_TestFrameCount >= g_TestFrameLimit) {
            goto shutdown;
        }
        if (g_TestExitFlags & 0x100) {
            goto shutdown;
        }

        frameLimit = g_FrameSyncThreshold;
        while (VSync(1) < frameLimit) {
        }
        elapsed = VSync(1);
        ticks = g_GameClock + 1;
        g_GameClock = ticks + elapsed / 256;
        VSync(0);
        {
            GameFrameContextAddress drawBuffer;
            drawBuffer.bytes = g_DrawBuffer;
            PutDrawEnv(&drawBuffer.context->environment.draw);
        }
        {
            GameFrameContextAddress drawBuffer;
            drawBuffer.bytes = g_DrawBuffer;
            PutDispEnv(&drawBuffer.context->environment.display);
        }
        {
            GameFrameContextAddress drawBuffer;
            drawBuffer.bytes = g_DrawBuffer;
            DrawOTag(&drawBuffer.context->layout.orderingTables[0][GAME_FRAME_OT_LENGTH - 1]);
        }
        {
            GameFrameContextAddress drawBuffer;
            drawBuffer.bytes = g_DrawBuffer;
            DrawOTag(&drawBuffer.context->layout.orderingTables[1][GAME_FRAME_OT_LENGTH - 1]);
        }
        UpdatePadState();
        g_FrameCounter = g_FrameCounter + 1;
    }

shutdown:
    ShutdownSoundSystem();
    if (g_AssetLoadState != 0) {
        ResetAssetLoader();
    }
    VSync(0xA);
    CdSync(0, 0);
    CdControlB(9, 0, 0);
    VSync(0xA);
    CdFlush();
    func_80074B58();
    ResetGraph(3);
    KernelCallbackSlot4();
    VSync(0xA);
    return 0;
}
