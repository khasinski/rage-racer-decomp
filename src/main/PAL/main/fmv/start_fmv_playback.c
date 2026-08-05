#include "common.h"
#include "game/state.h"
#include "psyq/gpu.h"
#include "game/render.h"
#include "game/cd.h"

extern s32 g_FmvFrameWidth;
extern s32 g_FmvFrameHeight;
extern s32 g_FmvStreamEnded;
extern s32 g_FmvState;
extern s32 g_GameClock;
extern volatile u8 g_FmvVlcBuffers;
extern u8 g_DrawEnv1Dither;
extern u8 g_DrawEnv0Dither;
extern u8 g_DispEnv1Rgb24;
extern u8 g_DispEnv0Rgb24;
extern s32 g_StreamLoc;

void SetupFmvBuffers(s32 arg0);
void InitFmvContext(volatile void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void OpenFmvStream(void (*arg0)(void));
s32 PresentFmvFrame(volatile void *arg0);
void StartStreamRead(s32 arg0);
s32 VSync(s32 mode);

void StartFmvPlayback(s32 arg0) {
    s32 fail;
    char frame_pad[8];

    SetDispMask(0);
    g_FmvFrameWidth = 0;
    g_FmvFrameHeight = 0;
    ApplyCdVolume();
    SetupDisplay240(0, 0, 0);
    g_DrawEnv1Dither = 0;
    g_DrawEnv0Dither = 0;
    g_DispEnv1Rgb24 = 1;
    g_DispEnv0Rgb24 = 1;
    SetupFmvBuffers(arg0);
    {
        volatile void *buf = &g_FmvVlcBuffers;
        InitFmvContext(buf, 0, 0x18, 0, 0x108);
    }
    OpenFmvStream(UploadFmvSlice);
    fail = -1;
    while (1) {
        volatile void *buf = &g_FmvVlcBuffers;
        if (PresentFmvFrame(buf) != fail) {
            break;
        }
        StartStreamRead(g_StreamLoc);
    }
    g_FmvStreamEnded = 0;
    g_SceneTimer = 0;
    g_FmvState = 1;
    VSync(0);
    g_GameClock = 0;
}
