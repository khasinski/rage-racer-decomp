#include "common.h"
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
#include "psyq/gpu.h"
#include "psyq/kernel.h"
#include "psyq/snd.h"

#ifndef INIT_SUBSYSTEMS_ENABLE_DMA_INTERRUPTS
#define INIT_SUBSYSTEMS_ENABLE_DMA_INTERRUPTS 1
#endif

/* One-shot boot chain run before the first frame. */
void InitSubsystems(void) {
    ssinit();
    InitSoundRuntime();
    ResetGraph(0);
    SetGraphDebug(0);
    SetDispMask(0);
    g_ScreenOffsetY.value = 0;
    g_ScreenOffsetX.value = 0;
#if INIT_SUBSYSTEMS_ENABLE_DMA_INTERRUPTS
    SetDMAInterruptState(1);
#endif
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
    g_PadErrorState = PAD_ERROR_STATE_NONE;
    g_PadValidateCountdown = 0x21;
    g_PadErrorHoldBits = 0;
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
