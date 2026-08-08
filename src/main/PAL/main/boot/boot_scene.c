#include "common.h"
#include "game/asset.h"
#include "game/menu.h"
#include "game/race.h"
#include "game/render.h"
#include "game/scratchpad.h"
#include "game/state.h"
#include "psyq/gpu.h"
#include "psyq/gte.h"

void *QueueShadedSpriteNine(void* ot, void* prim, s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 clutIndex, s32 intensity) asm("GameQueueShadedSprite");
void *GameQueueDrawModePrimWide(void* ot, void* prim, s32 tpage) asm("QueueDrawModePrim");
extern Matrix g_SceneLightMatrix;
void InitRenderState(s32 otShift);

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
    scratch = &SCRATCH_PRIM_CURSOR_AS(void);

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
        SCRATCH_VIEW_X = 0;
        SCRATCH_VIEW_Y = 0;
        SCRATCH_VIEW_Z = -3520;
        SCRATCH_VIEW_ANGLE_X = 0;
        SCRATCH_VIEW_ANGLE_Y = 0;
        SCRATCH_VIEW_ANGLE_Z = 0;
        SetCameraRotMatrix();
        g_OptionLetterboxHeight = 0xF0;
        g_FadeLevel = 0x100;
        g_GameMode = 0;
        g_FadeStep = -8;
    }
}
