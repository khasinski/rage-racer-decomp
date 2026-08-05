#include "common.h"
#include "game/asset.h"
#include "game/car.h"
#include "game/race.h"
#include "game/render.h"
#include "game/state.h"
#include "psyq/gpu.h"
#include "game/cd.h"
void UpdateAttractCars(void);

extern s32 g_BgmSelectStep;
extern void (*g_BgmSelectSteps[])(void);
extern s32 g_ImageBlockBuffer;
extern s32 g_FrameSyncThreshold;
extern s32 g_AttractDemoStep;
void UploadImageAsset(void *arg0);
void InstallCourseAssets(void);
s32 RequestTrackDataAssets(void);
extern s16 g_AttractTitleDelays[];
s32 GetAttractTitleFade(s32 arg0);

void func_80046A2C(
    u8 *arg0,
    u32 arg1,
    u32 arg2,
    u32 arg3,
    u32 arg4,
    u32 arg5,
    u32 arg6,
    u32 arg7,
    u32 arg8,
    u32 arg9,
    u32 arg10,
    u32 arg11,
    u32 arg12,
    u32 arg13);

extern u32 g_BgmShuffleIndex;
extern u8 g_BgmShuffleOrder[];
void InitTrackScene(void);
void AdvanceBgmShuffleBag(s32 arg0);
void DrawAttractTitle(void);
extern u32 g_StreamReturnScene;
extern u32 g_CameraViewMode;
extern u8 g_CarTrackSection[];
void DrawFullscreenFadeTile(s32 arg0, u32 arg1);
void ReturnToTitleScene(void);
s32 CycleAttractCameraCar(u32 arg0, s32 arg1);
void RequestTrackTexturePage(s32 arg0);
void UpdateCamera(u32 arg0, GameCarRuntime *arg1);
void DrawSkyBackground(void);
void DrawTerrainCellsWide(void);
void DrawCourseScenery2(u32 arg0, u32 arg1);

void UpdateBgmSelectScene(void);
void UpdateBgmSelectScene(void) {
    void (*func)(void);

    func = g_BgmSelectSteps[g_BgmSelectStep];
    g_SceneTimer++;
    func();
}

void EnterAttractDemo(void);
void EnterAttractDemo(void) {
    s32 initialValue;

    SetDispMask(0);
    SetupDisplay240(0, 0, 0);

    initialValue = 0x80;
    g_FrameSyncThreshold = initialValue;
    UploadImageAsset((void *)g_ImageBlockBuffer);
    InstallCourseAssets();
    RequestTrackDataAssets();

    g_AttractDemoStep = 0;
    g_FadeLevel = initialValue;
    g_SceneTimer = 0;
    g_SceneId = 0x1E;
    g_CameraCarIndex = 0;
}

s32 GetAttractTitleFade(s32 arg0);
s32 GetAttractTitleFade(s32 arg0) {
    s32 value;

    if (g_AttractDemoStep == 0) {
        value = (g_SceneTimer * 4) - g_AttractTitleDelays[arg0];
    } else {
        if (g_FadeLevel > 0) {
            g_FadeLevel--;
        }
        value = g_FadeLevel;
    }

    return value < 0 ? 0 : (value < 0x80 ? value : 0x7F);
}

void DrawAttractTitle(void);
void DrawAttractTitle(void) {
    u8 *ptr;
    s32 value;
    u32 one;
    u32 arg13;

    ptr = g_DrawBuffer + 0xCC;
    value = GetAttractTitleFade(0);
    one = 1;
    arg13 = 0x29;
    func_80046A2C(ptr, 0x74, 0x34, 0x58, 0x38, 0xA8, 0xA8, value, value, value, 0x1F, 0, one, arg13);
    func_80046A2C(ptr, 0x44, 0x70, 0xB8, 0x14, 0x48, 0xE8, value, value, value, 0x80, 0, one, arg13);
    value = GetAttractTitleFade(1);
    func_80046A2C(ptr, 0x5E, 0x90, 0x84, 0xC, 0, (g_CourseIndex * 12) + 0x9C, value, value, value, 0x12, 0, one, arg13);
}

void UpdateAttractDemoStart(void);
void UpdateAttractDemoStart(void) {
    s32 mode;
    u32 timer;

    timer = g_SceneTimer;
    if (timer < 0x2710) {
        g_SceneTimer = timer + 1;
    }

    if (g_SceneTimer == 2) {
        SetDispMask(1);
    }

    if (g_AssetLoadState == 0) {
        InitTrackScene();

        g_AttractDemoStep = 1;
        mode = g_BgmShuffleOrder[g_BgmShuffleIndex];
        AdvanceBgmShuffleBag(mode);

        mode += 3;
        if (mode == 0xC) {
            mode = 0x11;
        }

        RequestCdTrack(mode);
        StartCdAudio();
    }

    DrawAttractTitle();
}

void ReturnToTitleScene(void);
void ReturnToTitleScene(void) {
    g_SceneId = 3;
    g_StreamReturnScene = 0;
    ResetCdAudioState();
}

void UpdateAttractDemoRace(void);
void UpdateAttractDemoRace(void) {
    u32 value;
    u32 timer;
    s32 index;
    s32 offset;

    g_SceneTimer++;
    if ((u32)g_SceneTimer < 0x3D) {
        DrawAttractTitle();
        value = g_SceneTimer - 6;
        DrawFullscreenFadeTile(0xFF - (((value * 3) * 4) - value), 0x49);
    }

    timer = g_SceneTimer;
    if (timer == 0x6CC) {
        StartCdVolumeFade(0x38);
        timer = g_SceneTimer;
    }
    if (timer >= 0x6CD) {
        u32 adjusted;

        adjusted = timer - 0x6CC;
        DrawFullscreenFadeTile(adjusted * 5, 0x49);
    }

    if (g_SceneTimer == 0x708) {
        ReturnToTitleScene();
    }

    g_AnimTimer++;
    g_CameraCarIndex = CycleAttractCameraCar(0xFF, g_CameraCarIndex);
    UpdateAttractCars();

    index = g_CameraCarIndex;
    offset = (((((index * 3) * 4) + index) * 8) - index) * 4;
    RequestTrackTexturePage(*(s16 *)&g_CarTrackSection[offset]);

    UpdateCamera(g_CameraViewMode, &g_Cars[g_CameraCarIndex]);
    DrawCars();
    UpdateEnvironment();
    DrawSkyBackground();
    *(u32 *)0x1F800084 = (u32)g_IsEnvironmentMode4;
    DrawTerrainCellsWide();
    DrawCourseObjects();
    DrawCourseScenery2(g_AnimTimer, 1);
}
