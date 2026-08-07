#include "common.h"
#include "game/asset.h"
#include "game/audio.h"
#include "game/cd.h"
#include "game/memcard.h"
#include "game/race.h"
#include "game/render.h"
#include "game/scratchpad.h"
#include "game/state.h"
#include "psyq/cd.h"
#include "psyq/gpu.h"
#include "psyq/kernel.h"
#include "psyq/snd.h"

void SetGraphDebug(s32 arg0);
void InitRenderState(s32 otShift);

extern s32 g_ScreenOffsetY;
extern s32 g_ScreenOffsetX;

extern u16 g_NegconSteerPlay;
extern u16 g_PadMappingIndex;
extern u16 g_NegconMappingIndex;
extern u16 g_NegconSteerNeutral;
extern u16 g_NegconNeutralI;
extern u16 g_NegconNeutralII;
extern u16 g_NegconNeutralL;
extern u16 g_NegconMaxTwist;

/*
 * One-shot boot chain called from MainLoop: sequencer, sound runtime, GPU
 * and DMA, the pad, then the persistent settings block reset to its defaults
 * (NeGcon uncalibrated, both button mappings on preset 0) and the scratchpad
 * camera block primed before the first frame.
 */
void InitSubsystems(void) {
    ssinit();
    InitSoundRuntime();
    ResetGraph(0);
    SetGraphDebug(0);
    SetDispMask(0);
    g_ScreenOffsetY = 0;
    g_ScreenOffsetX = 0;
    SetDMAInterruptState(1);
    InitGeom();
    GameInitPad();
    RestartMemoryCard();
    g_NegconSteerPlay = 1;
    g_PadMappingIndex = 0;
    g_NegconMappingIndex = 0;
    g_NegconSteerNeutral = 0;
    g_NegconNeutralI = 0;
    g_NegconNeutralII = 0;
    g_NegconNeutralL = 0;
    g_NegconMaxTwist = 0;
    g_PadErrorState = 0;
    D_801E4D14 = 0x21;
    D_8019CB10 = 0;
    g_MirrorMode = 0;
    ResetReplayFrameCounts();
    ApplyPadButtonMapping();
    InitRecordTables();
    InitRenderState(5);
    InitSaveDefaults();
    SCRATCH_VIEW_Y = -64;
    SCRATCH_VIEW_Z = -256;
    g_AdvancedSeriesUnlocked = 0;
    SCRATCH_VIEW_X = 0;
    SCRATCH_VIEW_ANGLE_X = 0x100;
    SCRATCH_VIEW_ANGLE_Y = 0;
    SCRATCH_VIEW_ANGLE_Z = 0;
    SetCameraRotMatrix();
}

void PutDrawEnv(u8 *env);
void PutDispEnv(u8 *arg0);
void DrawOTag(u8 *ot);

/* The two 0x237E8-byte frame contexts the loop ping-pongs between. */
extern u8 g_FrameContexts[];
/* Which of them is current, mirrored for everyone else to read. */
extern s32 g_FrameParity;
/* Scene handlers, indexed by g_SceneId. */
extern void (*g_SceneHandlers[])(void);

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
        u8 *frame = g_FrameContexts + parity * 0x237E8;

        g_DrawBuffer = frame;
        g_FrameParity = parity;
        SCRATCH_OT_BASE_WORD = (s32)(frame + 0xCC);
        SCRATCH_PRIM_CURSOR_WORD = (s32)(frame + 0x16CC);
        ClearOTagR((u_long *)(frame + 0xCC), 0x2C0);
        ClearOTagR((u_long *)(g_DrawBuffer + 0xBCC), 0x2C0);
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
        PutDrawEnv(g_DrawBuffer);
        PutDispEnv(g_DrawBuffer + 0x5C);
        DrawOTag(g_DrawBuffer + 0xBC8);
        DrawOTag(g_DrawBuffer + 0x16C8);
        UpdatePadState();
        g_FrameCounter = g_FrameCounter + 1;
    }
}

/*
 * Stubbed-out debug printf: returns 1 and does nothing. Left un-prototyped
 * because callers pass anything from one to seven arguments. The format
 * strings they pass survive in the binary and were the key to naming much of
 * the game (see docs/names.md).
 */
s32 DebugPrintf() {
    return 1;
}
