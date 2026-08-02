#include "common.h"
#include "game/state.h"
#include "psyq/gpu.h"
#include "game/render.h"

extern s32 g_FmvFrameWidth asm("D_8009AF6C");
extern s32 g_FmvFrameHeight asm("D_8009AF70");
extern s32 g_FmvStreamEnded asm("D_8009AF74");
extern s32 g_FmvState asm("D_8009F094");
extern s32 g_GameClock asm("D_8009E678");
extern volatile u8 g_FmvVlcBuffers asm("D_8009AF20");
extern u8 g_DrawEnv1Dither asm("D_801C0638");
extern u8 g_DrawEnv0Dither asm("D_8019CE50");
extern u8 g_DispEnv1Rgb24 asm("D_801C068D");
extern u8 g_DispEnv0Rgb24 asm("D_8019CEA5");
extern s32 g_StreamLoc asm("D_801E8A90");

void ApplyCdVolume(void) asm("func_8004310C");
void SetupFmvBuffers(s32 arg0) asm("func_8001EB14");
void InitFmvContext(volatile void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) asm("func_8001EA7C");
void UploadFmvSlice(void) asm("func_8001EBC8");
void OpenFmvStream(void (*arg0)(void)) asm("func_8001EB5C");
s32 PresentFmvFrame(volatile void *arg0) asm("func_8001ED3C");
void StartStreamRead(s32 arg0) asm("func_8001F018");
s32 VSync(s32 mode) asm("func_8006DD30");

void StartFmvPlayback(s32 arg0) asm("func_8001E79C");
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
