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

/* Scene handlers, indexed by g_SceneId. */

/*
 * The PS-EXE `main`. Boots the subsystems, then never returns: each pass picks
 * the frame context, resets its two ordering tables and the scratchpad packet
 * cursor, runs the CD / sequencer / loader services and the current scene
 * handler, waits for the frame deadline, swaps the display and refreshes the
 * pad.
 */
void MainLoop(void) {
    s32 frameLimit;
    s32 elapsed;
    s32 ticks;

    __main();
    KernelCallbackSlot3();
    BiosSetMemSize(2);
    CdInit();
    InitSubsystems();
    InitAssetSystem();
    ResetGraph(3);
    InitCdAudio();
    g_FrameSyncThreshold = 0x80;
    SetDispMask(0);
    SetupDisplay240(0, 0, 0);
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
}
