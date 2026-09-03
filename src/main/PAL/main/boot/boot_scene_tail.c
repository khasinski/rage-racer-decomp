#include "common.h"
#include "game/prim.h"
#include "game/asset.h"
#include "game/menu.h"
#include "game/race.h"
#include "game/render.h"
#include "game/render_internal.h"
#include "game/scratchpad.h"
#include "game/state.h"
#include "psyq/gpu.h"
#include "psyq/gte.h"


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
