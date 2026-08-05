#include "common.h"
#include "game/render.h"
#include "game/audio.h"
#include "game/state.h"
#include "psyq/gpu.h"
#include "psyq/kernel.h"
#include "psyq/snd.h"
#include "game/memcard.h"
#include "psyq/cd.h"
void ServiceAssetLoad(void);
void TickCdAudio(void);
void RequestBootAssets(void);
void InitCdAudio(void);
void InitAssetSystem(void);
void InitSaveDefaults(void);

void MainLoop(void);

void ResetGraph(s32 arg0);
void SetGraphDebug(s32 arg0);
void InitGeom(void);
void RestartMemoryCard(void);
void ResetReplayFrameCounts(void);
void InitRecordTables(void);
void InitRenderState(s32 arg0) asm("func_80017884");

extern s32 g_ScreenOffsetY;
extern s32 g_ScreenOffsetX;
extern s32 g_PadErrorState;
extern s32 D_801E4D14;
extern s32 D_8019CB10;
extern s32 g_MirrorMode;

extern u16 g_NegconSteerPlay;
extern u16 g_PadMappingIndex;
extern u16 g_NegconMappingIndex;
extern u16 g_NegconSteerNeutral;
extern u16 g_NegconNeutralI;
extern u16 g_NegconNeutralII;
extern u16 g_NegconNeutralL;
extern u16 g_NegconMaxTwist;
extern u16 g_AdvancedSeriesUnlocked;

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
    *(s32 *)0x1F80000C = -64;
    *(s32 *)0x1F800010 = -256;
    g_AdvancedSeriesUnlocked = 0;
    *(s32 *)0x1F800008 = 0;
    *(s32 *)0x1F800018 = 0x100;
    *(s32 *)0x1F80001C = 0;
    *(s32 *)0x1F800020 = 0;
    SetCameraRotMatrix();
}

void __main(void);
void BiosSetMemSize(s32 arg0);
void TickSequenceAudio(void);
void StepTrackTextureSwap(void);
void PutDrawEnv(u8 *env);
void PutDispEnv(u8 *arg0);
void DrawOTag(u8 *ot);

/* The two 0x237E8-byte frame contexts the loop ping-pongs between. */
extern u8 g_FrameContexts[];
/* Which of them is current, mirrored for everyone else to read. */
extern s32 g_FrameParity;
/* Minimum VSync count per frame; 0x80 means "never wait", i.e. free-running. */
extern s32 g_FrameSyncThreshold;
/* Running VSync-derived clock and the raw frame counter. */
extern s32 g_GameClock;
extern s32 g_FrameCounter;
/* Scene handlers, indexed by g_SceneId. */
extern void (*g_SceneHandlers[])(void) asm("D_8007C268");

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
        *(s32 *)0x1F800004 = (s32)(frame + 0xCC);
        *(s32 *)0x1F800000 = (s32)(frame + 0x16CC);
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
