#include "common.h"
#include "game/asset.h"
#include "game/state.h"
#include "psyq/gte.h"
#include "game/render.h"
#include "game/menu.h"
#include "game/race.h"
#include "psyq/gpu.h"

void *func_80016F8C(void *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9);
void *func_80017390(void *arg0, void *arg1, s32 arg2);
extern s32 g_BootLogoState asm("D_8007D53C");
extern s32 g_BootLogoTimer asm("D_8007D540");
extern s32 g_BootLogoHoldTimer asm("D_8007D544");
void GameBeginIntroFmv(s32 arg0) asm("func_80019AF0");
void func_800230B0(void);
void GameDrawBootLogo(void) asm("func_8002317C");
extern Matrix g_SceneColorMatrix asm("D_8019CAD4");
extern Matrix g_DefaultColorMatrix asm("D_8007D548");
extern Matrix g_SceneLightMatrix asm("D_8009E6AC");
extern Matrix g_DefaultLightMatrix asm("D_8007D568");
void func_800698B8(Matrix *arg0);
void func_80069888(Matrix *arg0);
void func_80069A18(s32 arg0, s32 arg1, s32 arg2);
void func_80069A38(s32 arg0, s32 arg1, s32 arg2);
void func_800686D4(s32 arg0, s32 arg1);
extern s32 g_FrameSyncThreshold asm("D_8019C768");
extern s32 g_ImageBlockBuffer asm("D_801E4B30");
extern s32 g_OptionLetterboxHeight asm("D_8009F0A0");
void GameUploadImageAsset(s32 arg0) asm("func_8001A3C0");
void GameInitRenderState(s32 arg0) asm("func_80017884");
void GameInstallSceneLighting(void) asm("func_800234DC");

void GameDrawBootLogo(void) asm("func_8002317C");
void GameDrawBootLogo(void) {
    u8 *base;
    s32 height;
    s32 clut;
    void **scratch;
    s32 fade;
    s32 value;
    void *next;

    fade = g_SceneTimer;
    if (fade >= 0) {
        value = fade;
        if (value >= 0x100) {
            value = 0xFF;
        }
    } else {
        value = 0;
    }
    fade = value;

    base = g_DrawBuffer + 0xCC;
    scratch = (void **)0x1F800000;

    next = *scratch;
    next = func_80016F8C(base, next, 0x64, 0xEC, 0x7C, 0x18, 0x80, 0, 0x3F97, fade);

    height = 0x20;
    clut = 0x3FD7;
    next = func_80016F8C(base, next, 0xDC, 0xC4, 8, 0x10, 0, height, clut, fade);
    next = func_80016F8C(base, next, 0x64, 0xC4, 0x78, height, 0, 0, clut, fade);
    *scratch = func_80017390(base, next, 5);
}

void GameUpdateBootLogoScene(void) asm("func_800232B4");
void GameUpdateBootLogoScene(void) {
    s32 state;

    if (g_BootLogoTimer < 110) {
        if (g_BootLogoTimer >= 10) {
            SetDispMask(1);
        }
        func_800230B0();
        goto inc_timer;
    } else if (g_BootLogoTimer == 110) {
        SetDispMask(0);
        GameSetupDisplay480(0, 0, 0);
inc_timer:
        g_BootLogoTimer++;
        return;
    }

    if (g_BootLogoHoldTimer != 0) {
        g_BootLogoHoldTimer--;
        if ((g_AssetLoadState == 0) && (g_PadHeld != 0)) {
            g_BootLogoHoldTimer = 0;
        }
    }

    state = g_BootLogoState;
    switch (state) {
    case 0:
        if ((u32)g_SceneTimer < 0x100) {
            g_SceneTimer += 8;
        } else {
            g_BootLogoState = 1;
        }
        break;
    case 1:
        if (g_BootLogoHoldTimer == 0) {
            g_BootLogoState = 2;
        }
        break;
    case 2:
        g_SceneTimer -= 8;
        if (g_SceneTimer == 0) {
            g_BootLogoState = 3;
            GameSetupDisplay240(0, 0, 0);
        }
        break;
    case 3:
        g_SceneTimer++;
        if ((u32)g_SceneTimer >= 21) {
            GameBeginIntroFmv(3);
        }
        break;
    }

done:
    if (g_BootLogoState != 3) {
        GameDrawBootLogo();
        if ((u32)g_SceneTimer >= 10) {
            SetDispMask(1);
        }
    }
}

void GameInstallSceneLighting(void) asm("func_800234DC");
void GameInstallSceneLighting(void) {
    g_SceneColorMatrix = g_DefaultColorMatrix;
    g_SceneLightMatrix = g_DefaultLightMatrix;
    func_800698B8(&g_SceneColorMatrix);
    func_80069888(&g_SceneLightMatrix);
    func_80069A18(0x20, 0x20, 0x20);
    func_80069A38(0, 0, 0);
    func_800686D4(0x4E20, 0x140);
}

void GameEnterAttractScene(void) asm("func_800235D8");
void GameEnterAttractScene(void) {
    SetDispMask(0);
    g_FrameSyncThreshold = 0x80;
    if (g_AssetLoadState == 0) {
        GameUploadImageAsset(g_ImageBlockBuffer);
        g_MirrorMode = 0;
        GameInitRenderState(5);
        GameSetupDisplay480(0, 0, 0);
        g_SceneId = 0x17;
        g_SceneTimer = 0;
        GameInstallSceneLighting();
        *(s32 *)0x1F800008 = 0;
        *(s32 *)0x1F80000C = 0;
        *(s32 *)0x1F800010 = -3520;
        *(s32 *)0x1F800018 = 0;
        *(s32 *)0x1F80001C = 0;
        *(s32 *)0x1F800020 = 0;
        GameSetCameraRotMatrix();
        g_OptionLetterboxHeight = 0xF0;
        g_FadeLevel = 0x100;
        g_GameMode = 0;
        g_FadeStep = -8;
    }
}
