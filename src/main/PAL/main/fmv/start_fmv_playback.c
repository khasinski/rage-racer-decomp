#include "common.h"
#include "game/fmv.h"
#include "game/asset.h"
#include "game/state.h"
#include "psyq/gpu.h"
#include "game/render.h"
#include "game/cd.h"
#include "psyq/kernel.h"

#ifndef FMV_RETRY_INITIAL_FRAME
#define FMV_RETRY_INITIAL_FRAME 1
#endif

void StartFmvPlayback(FmvWorkBuffers *buffers) {
#if FMV_RETRY_INITIAL_FRAME
    s32 fail;
    char frame_pad[8];
#endif

    SetDispMask(0);
    g_FmvFrameWidth = 0;
    g_FmvFrameHeight = 0;
    ApplyCdVolume();
    SetupDisplay240(0, 0, 0);
    g_DrawEnv1Dither = 0;
    g_DrawEnv0Dither = 0;
    g_DispEnv1Rgb24 = 1;
    g_DispEnv0Rgb24 = 1;
    SetupFmvBuffers(buffers);
    {
        FmvDecodeContext *buf = &g_FmvDecodeContext;
        InitFmvContext(buf, 0, 0x18, 0, 0x108);
    }
    OpenFmvStream(UploadFmvSlice);
#if FMV_RETRY_INITIAL_FRAME
    fail = -1;
    while (1) {
        FmvDecodeContext *buf = &g_FmvDecodeContext;
        if (PresentFmvFrame(buf) != fail) {
            break;
        }
        StartStreamRead(g_StreamLoc);
    }
#else
    PresentFmvFrame(&g_FmvDecodeContext);
#endif
    g_FmvStreamEnded = 0;
    g_SceneTimer = 0;
    g_FmvState = FMV_PLAYBACK_DECODE;
    VSync(0);
    g_GameClock = 0;
}
