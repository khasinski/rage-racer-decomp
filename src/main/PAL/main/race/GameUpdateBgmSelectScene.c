#include "common.h"
#include "game/asset.h"
#include "game/car.h"
#include "game/race.h"
#include "game/render.h"
#include "game/state.h"
#include "psyq/gpu.h"
#include "game/cd.h"

extern s32 g_BgmSelectStep asm("D_8019C99C");
extern void (*g_BgmSelectSteps[])(void) asm("D_8007D6B8");
extern s32 g_ImageBlockBuffer asm("D_801E4B30");
extern s32 D_8019C768;
extern s32 g_AttractDemoStep asm("D_801E682C");
void GameUploadImageAsset(void *arg0) asm("func_8001A3C0");
void GameInstallCourseAssets(void) asm("func_80019730");
s32 GameRequestTrackDataAssets(void) asm("func_80019844");
extern s16 g_AttractTitleDelays[] asm("D_8007D6CC");
s32 GameGetAttractTitleFade(s32 arg0) asm("func_8002664C");

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

extern u32 g_BgmShuffleIndex asm("D_8009E6CC");
extern u8 g_BgmShuffleOrder[] asm("D_801E7734");
void func_80025940(void);
void GameAdvanceBgmShuffleBag(s32 arg0) asm("func_80025E54");
void GameDrawAttractTitle(void) asm("func_800266D0");
extern u32 g_StreamReturnScene asm("D_8019C760");
extern u32 g_CameraViewMode asm("D_8009E870");
extern u8 D_801F18CC[];
void func_80033AA0(s32 arg0, u32 arg1);
void GameReturnToTitleScene(void) asm("func_800268EC");
s32 func_8001A1F0(u32 arg0, s32 arg1);
void func_8003BB50(void);
void func_80019EFC(s32 arg0);
void GameUpdateCamera(u32 arg0, GameCarRuntime *arg1) asm("func_80043BCC");
void func_800418D4(void);
void GameDrawTerrainCellsWide(void) asm("func_80041888");
void GameDrawCourseScenery2(u32 arg0, u32 arg1) asm("func_8003E2E8");

void GameUpdateBgmSelectScene(void) asm("func_80026570");
void GameUpdateBgmSelectScene(void) {
    void (*func)(void);

    func = g_BgmSelectSteps[g_BgmSelectStep];
    g_SceneTimer++;
    func();
}

void GameEnterAttractDemo(void) asm("func_800265BC");
void GameEnterAttractDemo(void) {
    s32 initialValue;

    SetDispMask(0);
    GameSetupDisplay240(0, 0, 0);

    initialValue = 0x80;
    D_8019C768 = initialValue;
    GameUploadImageAsset((void *)g_ImageBlockBuffer);
    GameInstallCourseAssets();
    GameRequestTrackDataAssets();

    g_AttractDemoStep = 0;
    g_FadeLevel = initialValue;
    g_SceneTimer = 0;
    g_SceneId = 0x1E;
    g_CameraCarIndex = 0;
}

s32 GameGetAttractTitleFade(s32 arg0) asm("func_8002664C");
s32 GameGetAttractTitleFade(s32 arg0) {
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

void GameDrawAttractTitle(void) asm("func_800266D0");
void GameDrawAttractTitle(void) {
    u8 *ptr;
    s32 value;
    u32 one;
    u32 arg13;

    ptr = g_DrawBuffer + 0xCC;
    value = GameGetAttractTitleFade(0);
    one = 1;
    arg13 = 0x29;
    func_80046A2C(ptr, 0x74, 0x34, 0x58, 0x38, 0xA8, 0xA8, value, value, value, 0x1F, 0, one, arg13);
    func_80046A2C(ptr, 0x44, 0x70, 0xB8, 0x14, 0x48, 0xE8, value, value, value, 0x80, 0, one, arg13);
    value = GameGetAttractTitleFade(1);
    func_80046A2C(ptr, 0x5E, 0x90, 0x84, 0xC, 0, (g_CourseIndex * 12) + 0x9C, value, value, value, 0x12, 0, one, arg13);
}

void GameUpdateAttractDemoStart(void) asm("func_80026824");
void GameUpdateAttractDemoStart(void) {
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
        func_80025940();

        g_AttractDemoStep = 1;
        mode = g_BgmShuffleOrder[g_BgmShuffleIndex];
        GameAdvanceBgmShuffleBag(mode);

        mode += 3;
        if (mode == 0xC) {
            mode = 0x11;
        }

        GameRequestCdTrack(mode);
        GameStartCdAudio();
    }

    GameDrawAttractTitle();
}

void GameReturnToTitleScene(void) asm("func_800268EC");
void GameReturnToTitleScene(void) {
    g_SceneId = 3;
    g_StreamReturnScene = 0;
    GameResetCdAudioState();
}

void GameUpdateAttractDemoRace(void) asm("func_80026920");
void GameUpdateAttractDemoRace(void) {
    u32 value;
    u32 timer;
    s32 index;
    s32 offset;

    g_SceneTimer++;
    if ((u32)g_SceneTimer < 0x3D) {
        GameDrawAttractTitle();
        value = g_SceneTimer - 6;
        func_80033AA0(0xFF - (((value * 3) * 4) - value), 0x49);
    }

    timer = g_SceneTimer;
    if (timer == 0x6CC) {
        GameStartCdVolumeFade(0x38);
        timer = g_SceneTimer;
    }
    if (timer >= 0x6CD) {
        u32 adjusted;

        adjusted = timer - 0x6CC;
        func_80033AA0((adjusted * 4) + adjusted, 0x49);
    }

    if (g_SceneTimer == 0x708) {
        GameReturnToTitleScene();
    }

    g_AnimTimer++;
    g_CameraCarIndex = func_8001A1F0(0xFF, g_CameraCarIndex);
    func_8003BB50();

    index = g_CameraCarIndex;
    offset = (((((index * 3) * 4) + index) * 8) - index) * 4;
    func_80019EFC(*(s16 *)&D_801F18CC[offset]);

    GameUpdateCamera(g_CameraViewMode, &g_Cars[g_CameraCarIndex]);
    GameDrawCars();
    GameUpdateEnvironment();
    func_800418D4();
    *(u32 *)0x1F800084 = (u32)g_IsEnvironmentMode4;
    GameDrawTerrainCellsWide();
    GameDrawCourseObjects();
    GameDrawCourseScenery2(g_AnimTimer, 1);
}
