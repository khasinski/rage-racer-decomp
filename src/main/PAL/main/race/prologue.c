#include "common.h"
#include "game/asset.h"
#include "game/car.h"
#include "game/race.h"
#include "game/state.h"
#include "game/render.h"
#include "game/menu.h"
#include "psyq/gpu.h"
#include "game/cd.h"

typedef struct UnkEventPair {
    s16 timer;
    s16 carIndex;
} UnkEventPair;

extern s32 g_AttractDemoStep;
extern u32 g_StreamReturnScene;
extern void (*g_AttractDemoSteps[])(void);
void ReturnToTitleScene(void);
/* Deliberately raw: see docs/names.md 12d. */
extern s32 g_FrameSyncThreshold;
extern s32 g_PrologueStep;
extern s32 g_PrologueCutIndex;
extern u8 g_TextNowLoading[];
void DrawFullscreenFadeTile(s32 arg0, s32 arg1);
void InitTrackScene(void);
void RequestTrackDataAssets(void);
void UpdatePrologueLoad(void);
/* Split symbols of the 14-entry prologue table: 8 bytes each, { s16 x,
 * s16 y, char *text }. The scroll drives y and the alpha ramp. */
extern s16 g_PrologueLineX[];
extern s16 g_PrologueLineY[];
extern s32 g_PrologueLineText[];
void GameDrawText8x8Shaded(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
s32 GameQueueTileTransWide(u8 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) asm("GameQueueTileTrans");
s32 GameQueueDrawModePrimWide(u8 *arg0, s32 arg1, s32 arg2) asm("QueueDrawModePrim");
void RequestSelectBgmAssets(void);
extern u32 g_CameraViewMode;
extern u8 g_CarTrackSection[];
extern UnkEventPair g_PrologueCameraCuts[];
void ExitPrologue(void);
void DrawPrologueText(void);
void RequestTrackTexturePage(s32 arg0);
void UpdateCamera(u32 arg0, GameCarRuntime *arg1);
void DrawTerrainCellsWide(void);
void DrawCourseScenery2(s32 arg0, s32 arg1);

void UpdateAttractDemoScene(void) {
    g_AttractDemoSteps[g_AttractDemoStep]();

    if ((g_SceneId == 0x1E) && ((g_PadEdge2 & 0x860) != 0)) {
        if (g_AssetLoadState != 0) {
            ResetAssetLoader();
            g_SceneId = 3;
            g_StreamReturnScene = 0;
        } else {
            ReturnToTitleScene();
        }
    }
}

void EnterPrologue(void) {
    SetDispMask(0);
    SetupDisplay240(0, 0, 0);

    g_FrameSyncThreshold = 0x80;
    g_FadeLevel = 0x108;
    g_FadeStep = -4;
    g_SceneId = 0x20;
    g_PrologueStep = 0;
    g_PrologueCutIndex = 0;
    g_SceneTimer = 0;
    g_CameraCarIndex = 3;
}

void UpdatePrologueLoad(void) {
    s32 delta;

    if (g_SceneTimer == 2) {
        SetDispMask(1);
    }

    delta = g_FadeStep;
    if (delta < 0) {
        s32 value;

        value = g_FadeLevel;
        value = value + delta;
        g_FadeLevel = value;

        if (g_FadeLevel < 0) {
            g_FadeLevel = 0;
            g_FadeStep = 0;
        }

        DrawFullscreenFadeTile(g_FadeLevel, 0x49);
    } else if (delta > 0) {
        s32 value;

        value = g_FadeLevel;
        value = value + delta;
        g_FadeLevel = value;

        DrawFullscreenFadeTile(g_FadeLevel, 0x49);

        if (g_FadeLevel >= 0x101) {
            SetDispMask(0);
            g_CourseIndex = 0;
            InitTrackScene();
            StartCdAudio();
            g_PrologueStep = 3;
            g_FadeLevel = 0x100;
            g_FadeStep = 0;
        }
    }

    DrawProportionalText(0x5E, 0x72, g_TextNowLoading, 0x7812);
}

void UpdatePrologueLoadStep0(void) {
    if (g_AssetLoadState == 0) {
        InstallCourseAssets();
        RequestTrackDataAssets();
        g_PrologueStep = 1;
    }

    UpdatePrologueLoad();
}

void UpdatePrologueLoadStep1(void) {
    if (g_AssetLoadState == 0) {
        g_FadeStep = 4;
        RequestCdTrack(2);
        g_PrologueStep = 2;
    }

    UpdatePrologueLoad();
}

void UpdatePrologueLoadStep2(void) {
    UpdatePrologueLoad();
}

void DrawPrologueText(void) {
    s32 i;
    s32 offset;
    s32 adjusted;
    s32 delta;
    s32 value;
    s32 clamped;
    s32 next;
    s32 arg1;
    s32 arg7;
    s32 scale_b;
    s32 arg8;

    for (i = 0, offset = 0; i < 14; offset += 8) {
        s32 tableY;

        adjusted = ((u32)g_SceneTimer / 3) - 0xD0;
        tableY = *(s16 *)((u8 *)g_PrologueLineY + offset);
        delta = tableY - adjusted;

        if (delta < 0x60) {
            value = (0x60 - delta) << 1;
        } else if (delta >= 0x91) {
            value = (delta - 0x90) << 1;
        } else {
            value = 0;
        }

        if (value >= 0) {
            clamped = value;
            if (clamped >= 0x80) {
                clamped = 0x7F;
            }
        } else {
            clamped = 0;
        }

        value = 0x7F - clamped;
        i++;
        if (value != 0) {
            GameDrawText8x8Shaded(
                *(s16 *)((u8 *)g_PrologueLineX + offset),
                delta,
                *(s32 *)((u8 *)g_PrologueLineText + offset),
                0x78CC,
                value);
        }
    }

    {
        s32 camera;
        s32 *scratch;
        u8 *ptr;
        s32 scale_a;
        s32 tmp;
        u8 *base;

        camera = g_FadeLevel;
        scratch = (s32 *)0x1F800000;
        tmp = camera * 7;
        scale_a = tmp * 32;
        base = g_DrawBuffer;
        arg1 = *scratch;
        ptr = base + 0xD0;
        arg7 = (scale_a / 0x100) + 0x20;
        scale_b = (camera * 3) << 6;
        arg8 = (scale_b / 0x100) + 0x40;

        next = GameQueueTileTransWide(ptr, arg1, 0, 0, 0x140, 0xF0, camera, arg7, arg8);
        *scratch = GameQueueDrawModePrimWide(ptr, next, 0x49);
    }
}

void ExitPrologue(void) {
    g_SceneId = 6;
    PauseCdAudio();
    RequestSelectBgmAssets();
}

void UpdatePrologue(void) {
    s32 timer;
    s32 active;
    s32 eventIndex;

    if (g_SceneTimer == 2) {
        SetDispMask(1);
    }

    if ((u32)g_SceneTimer >= 0x79 && (g_PadEdge2 & 0x860)) {
        ExitPrologue();
    }

    timer = g_SceneTimer;
    if (timer == 0x3C) {
        g_FadeStep = -4;
    } else if (timer == 0x42E) {
        g_FadeStep = 2;
    } else if (timer == 0x500) {
        ExitPrologue();
    }

    g_FadeLevel += g_FadeStep;
    if (g_FadeLevel < 0) {
        g_FadeLevel = 0;
        g_FadeStep = 0;
    } else if (g_FadeLevel >= 0x100) {
        g_FadeLevel = 0xFF;
        g_FadeStep = 0;
    }

    DrawPrologueText();

    active = (u32)(g_SceneTimer - 0x10) < 0x40F;
    if (active) {
        eventIndex = g_PrologueCutIndex;
        g_AnimTimer++;
        if (g_PrologueCameraCuts[eventIndex].timer == g_SceneTimer) {
            g_PrologueCutIndex = eventIndex + 1;
            g_CameraCarIndex = g_PrologueCameraCuts[eventIndex].carIndex;
        }

        UpdateAttractCars();

        RequestTrackTexturePage(*(s16 *)&g_CarTrackSection[(((((g_CameraCarIndex * 3) * 4) + g_CameraCarIndex) * 8) - g_CameraCarIndex) * 4]);

        UpdateCamera(g_CameraViewMode, &g_Cars[g_CameraCarIndex]);
        UpdateEnvironment();
    }

    DrawCars();
    DrawSkyBackground();
    *(u32 *)0x1F800084 = (u32)g_IsEnvironmentMode4;
    DrawTerrainCellsWide();
    DrawCourseObjects();
    DrawCourseScenery2(g_AnimTimer, active);
}
