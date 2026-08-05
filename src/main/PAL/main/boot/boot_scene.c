#include "common.h"
#include "game/asset.h"
#include "game/state.h"
#include "psyq/gte.h"
#include "game/render.h"
#include "game/menu.h"
#include "game/race.h"
#include "psyq/gpu.h"

void *QueueShadedSpriteNine(void *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9) asm("GameQueueShadedSprite");
void *GameQueueDrawModePrimWide(void *arg0, void *arg1, s32 arg2) asm("QueueDrawModePrim");
extern s32 g_BootLogoState;
extern s32 g_BootLogoTimer;
extern s32 g_BootLogoHoldTimer;
void BeginIntroFmv(s32 arg0);
void DrawEndingStill(void);
void DrawBootLogo(void);
extern Matrix g_SceneColorMatrix;
extern Matrix g_DefaultColorMatrix;
extern Matrix g_SceneLightMatrix;
extern Matrix g_DefaultLightMatrix;
void SetColorMatrix(void *m);
extern s32 g_FrameSyncThreshold;
extern s32 g_ImageBlockBuffer;
extern s32 g_OptionLetterboxHeight;
void UploadImageAsset(s32 arg0);
void InitRenderState(s32 arg0);
void InstallSceneLighting(void);

void DrawBootLogo(void) {
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
    next = QueueShadedSpriteNine(base, next, 0x64, 0xEC, 0x7C, 0x18, 0x80, 0, 0x3F97, fade);

    height = 0x20;
    clut = 0x3FD7;
    next = QueueShadedSpriteNine(base, next, 0xDC, 0xC4, 8, 0x10, 0, height, clut, fade);
    next = QueueShadedSpriteNine(base, next, 0x64, 0xC4, 0x78, height, 0, 0, clut, fade);
    *scratch = GameQueueDrawModePrimWide(base, next, 5);
}

void UpdateBootLogoScene(void) {
    s32 state;

    if (g_BootLogoTimer < 110) {
        if (g_BootLogoTimer >= 10) {
            SetDispMask(1);
        }
        DrawEndingStill();
        g_BootLogoTimer++;
        return;
    } else if (g_BootLogoTimer == 110) {
        SetDispMask(0);
        SetupDisplay480(0, 0, 0);
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
            SetupDisplay240(0, 0, 0);
        }
        break;
    case 3:
        g_SceneTimer++;
        if ((u32)g_SceneTimer >= 21) {
            BeginIntroFmv(3);
        }
        break;
    }

    if (g_BootLogoState != 3) {
        DrawBootLogo();
        if ((u32)g_SceneTimer >= 10) {
            SetDispMask(1);
        }
    }
}

void InstallSceneLighting(void) {
    g_SceneColorMatrix = g_DefaultColorMatrix;
    g_SceneLightMatrix = g_DefaultLightMatrix;
    SetColorMatrix(&g_SceneColorMatrix);
    SetLightMatrix(&g_SceneLightMatrix);
    SetBackColor(0x20, 0x20, 0x20);
    SetFarColor(0, 0, 0);
    SetFogNear(0x4E20, 0x140);
}

void EnterAttractScene(void) {
    SetDispMask(0);
    g_FrameSyncThreshold = 0x80;
    if (g_AssetLoadState == 0) {
        UploadImageAsset(g_ImageBlockBuffer);
        g_MirrorMode = 0;
        InitRenderState(5);
        SetupDisplay480(0, 0, 0);
        g_SceneId = 0x17;
        g_SceneTimer = 0;
        InstallSceneLighting();
        *(s32 *)0x1F800008 = 0;
        *(s32 *)0x1F80000C = 0;
        *(s32 *)0x1F800010 = -3520;
        *(s32 *)0x1F800018 = 0;
        *(s32 *)0x1F80001C = 0;
        *(s32 *)0x1F800020 = 0;
        SetCameraRotMatrix();
        g_OptionLetterboxHeight = 0xF0;
        g_FadeLevel = 0x100;
        g_GameMode = 0;
        g_FadeStep = -8;
    }
}
