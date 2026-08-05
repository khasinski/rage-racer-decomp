#include "common.h"
#include "game/vector.h"
#include "psyq/gte.h"
#include "game/render.h"
#include "game/race.h"
#include "game/state.h"
#include "game/menu.h"
#include "game/car.h"
#include "psyq/gpu.h"

extern s32 D_8007FB40;
extern s32 D_8007FB44;
extern s32 g_MenuAltLayout;

void GameDrawTexturedQuadWide() asm("GameDrawTexturedQuad");

void DrawMenuAltPanel(s32 arg0, s32 arg1) {
    s32 step0;
    s32 step1;
    void *scratch;
    register s32 zero asm("$0");
    s32 value;
    s32 offset;
    s32 x0;
    register s32 y0 asm("$6");
    s32 render1;
    s16 y1;
    register void *callScratch asm("$4");
    s32 callX;

    scratch = *(void **)0x1F800004;
    step0 = arg0;
    step1 = arg1;

    if (step0 == 0 && step1 == 0) {
        D_8007FB40 = zero;
        D_8007FB44 = zero;
        return;
    }

    if (step0 < 0) {
        value = D_8007FB40 + step0;
        D_8007FB40 = value;
        if (value < 0) {
            D_8007FB40 = 0;
        }
    }

    if (step1 < 0) {
        value = D_8007FB44 + step1;
        D_8007FB44 = value;
        if (value < 0) {
            D_8007FB44 = 0;
        }
    }

    value = D_8007FB40;
    if (value != 0) {
        offset = (value - 1) * 2;
        x0 = (g_MenuAltLayout != 0) ? 0x69 : 0xA8;
        y0 = 0x9E;
        callScratch = scratch;
        callX = x0;
        y0 = (s16)(y0 - offset);
        x0 += 0x1C;
        y1 = (s16)(offset + 0x9F);
        GameDrawTexturedQuadWide(
            callScratch,
            callX,
            y0,
            x0,
            y0,
            callX,
            y1,
            x0,
            y1,
            0xB0,
            0x38,
            0xCC,
            0x38,
            0xB0,
            0x6C,
            0xCC,
            0x6C,
            0x7F,
            0x7F,
            0x7F,
            0x232,
            0,
            0,
            0x1C);
    }

    render1 = D_8007FB44;
    if (render1 != 0) {
        offset = render1 - 1;
        x0 = (g_MenuAltLayout != 0) ? 0x92 : 0xC0;
        y0 = 0x128;
        callScratch = scratch;
        callX = x0;
        y0 = (s16)(y0 - offset);
        x0 += 0x4E;
        y1 = (s16)(render1 + 0x128);
        GameDrawTexturedQuadWide(
            callScratch,
            callX,
            y0,
            x0,
            y0,
            callX,
            y1,
            x0,
            y1,
            0x61,
            0x38,
            0xAF,
            0x38,
            0x61,
            0x58,
            0xAF,
            0x58,
            0x7F,
            0x7F,
            0x7F,
            0x259,
            0,
            0,
            0x1C);
    }

    if (step0 > 0) {
        value = D_8007FB40 + step0;
        D_8007FB40 = value;
        if (value >= 0xF) {
            D_8007FB40 = 0xE;
        }
    }

    if (step1 > 0) {
        value = D_8007FB44 + step1;
        D_8007FB44 = value;
        if (value >= 0x11) {
            __asm__ volatile("" ::: "memory");
            D_8007FB44 = 0x10;
        }
    }
}

typedef struct UVec {
    u16 x;
    u16 y;
    u16 z;
    u16 pad;
} UVec;

extern SVec D_80011A48[];
extern s32 D_8007FB48;


void FlipCourseCard(s32 *p0, s32 *p1, s32 *p2) {
    SVec verts[4];
    UVec out[4];
    Matrix mtx;
    s32 scratch2;
    s32 n;
    s32 v;
    s32 depth;

    verts[0] = D_80011A48[0];
    verts[1] = D_80011A48[1];
    verts[2] = D_80011A48[2];
    verts[3] = D_80011A48[3];

    scratch2 = *(s32 *)0x1F800004 + 4;

    n = *p0 - *p1;
    if (n != 0) {
        if (n > 0)
            n = (n + 12) / 12;
        else
            n = (n - 12) / 12;
    }
    *p1 = *p1 + n;
    n = *p1 / 1000;
    if (n < 11) {
        n = 11;
    }
    if (n < 1024) {
        v = *p2;
        if (v >= 0) {
            D_8007FB48 = v;
            *p2 = -1;
        }
    }

    switch (D_8007FB48) {
    case 1:
        depth = 0x1F8;
        break;
    case 2:
        depth = 0x20B;
        break;
    case 3:
        depth = 0x1F9;
        break;
    default:
        return;
    }

    BuildRotMatrixY(&mtx, n);
    ApplyMatrixSV((s32 *)&mtx, &verts[0], (s16 *)&out[0]);
    ApplyMatrixSV((s32 *)&mtx, &verts[1], (s16 *)&out[1]);
    ApplyMatrixSV((s32 *)&mtx, &verts[2], (s16 *)&out[2]);
    ApplyMatrixSV((s32 *)&mtx, &verts[3], (s16 *)&out[3]);

    {
        register s32 x0 asm("$5");
        register s32 y0 asm("$6");
        register s32 x1 asm("$7");
        register s32 y1 asm("$3");
        register s32 x2 asm("$8");
        register s32 y2 asm("$9");
        register s32 x3 asm("$10");
        register s32 y3 asm("$11");

        x0 = out[0].x;
        x1 = out[1].x;
        x2 = out[2].x;
        x3 = out[3].x;
        y0 = out[0].y;
        y1 = out[1].y;
        y2 = out[2].y;
        y3 = out[3].y;

        x0 = (s16)(x0 + 0xE4);
        y0 = (s16)(y0 + 0x58);
        y1 = (s16)(y1 + 0x58);
        x2 = (s16)(x2 + 0xE4);
        y2 = (s16)(y2 + 0x58);
        x3 = (s16)(x3 + 0xE4);
        y3 = (s16)(y3 + 0x58);
        x1 = (s16)(x1 + 0xE4);

        GameDrawTexturedQuadWide(scratch2,
            x0, y0, x1, y1, x2, y2, x3, y3,
            0xA0, 0x70, 0xDF, 0x70, 0xA0, 0xBF, 0xDF, 0xBF,
            0x7F, 0x7F, 0x7F,
            depth,
            0, 0,
            0x1C);
    }
}

extern volatile s32 D_8007FB4C;

void DrawTimeAttackPlate(s32 arg0) {
    void *scratch = *(void **)0x1F800004;
    s32 step = arg0;
    s32 value;
    s32 renderValue;
    s32 y0;
    s16 y1;

    if (step == 0) {
        D_8007FB4C = 0;
        return;
    }

    if (step < 0) {
        value = D_8007FB4C + step;
        D_8007FB4C = value;
        if (value < 0) {
            D_8007FB4C = 0;
        }
    }

    renderValue = D_8007FB4C;
    y0 = 0xD7;
    if (renderValue != 0) {
        y0 = (s16)(y0 - renderValue);
        y1 = (s16)(renderValue + 0xD8);
        GameDrawTexturedQuadWide(
            scratch,
            0x4C,
            y0,
            0x7C,
            y0,
            0x4C,
            y1,
            0x7C,
            y1,
            0xCC,
            0x38,
            0xFC,
            0x38,
            0xCC,
            0x50,
            0xFC,
            0x50,
            0,
            0,
            0,
            0x20F,
            1,
            0,
            0x1C);
    }

    if (step > 0) {
        value = D_8007FB4C + step;
        D_8007FB4C = value;
        if (value >= 0xD) {
            __asm__ volatile("" ::: "memory");
            D_8007FB4C = 0xC;
        }
    }
}

extern Matrix g_SceneColorMatrix;
extern Matrix D_80082DFC;
extern Matrix g_SceneLightMatrix;
extern Matrix D_80082E1C;
void SetColorMatrix(void *m);

/* The menu-mode twin of InitTrackLighting. */
void InitMenuLighting(void) {
    g_SceneColorMatrix = D_80082DFC;
    g_SceneLightMatrix = D_80082E1C;
    SetColorMatrix(&g_SceneColorMatrix);
    SetLightMatrix(&g_SceneLightMatrix);
    SetBackColor(0x20, 0x20, 0x20);
    SetFarColor(0, 0, 0);
    SetFogNear(0x4E20, 0x140);
}

extern s32 g_FrameSyncThreshold;
extern s32 g_PlayerMoney;

extern s32 D_80082568;
extern Vec4 g_MenuViewScale;

extern void *D_8019C764;
extern void *D_801E40B4;
extern void *D_8019C794;
extern void *D_801E8A44;
extern void *D_8009F0B0;
extern void *D_8019CB00;
extern void *D_801E4188;

extern s32 g_MenuHintBarProgress;
extern s32 g_MenuConfirmTimer;
extern s32 D_8009B304;
extern s32 g_MenuHintBarStep;
extern s32 D_8009B310;
extern s32 g_MenuHintButtonsVisible;
extern s32 g_CarNamePlateStep;
extern s32 g_MenuPlateCarIndex;
extern s32 D_8009B324;
extern s32 D_8009B328;
extern s32 D_8009B32C;
extern s32 D_8009B330;
extern s32 g_TimeAttackPlateStep;
extern s32 D_8009B33C;
extern s32 g_CourseSwapDelay;
extern s32 D_8009B360;
extern s32 D_8009B364;
extern s32 D_8009B368;
extern s32 g_MenuCourseModelIndex;
extern s32 g_MenuPendingCourseIndex;
extern s32 g_CarSwapFromIndex;
extern s32 g_CarSwapToIndex;

extern s32 g_CourseSelectOption;
extern s32 D_801E4138;
extern s32 g_RankingOption;
extern s32 g_DesignModeOption;
extern s32 D_801E4D74;

void InitRenderState(s32);
s32 DrawCourseSelectScreen(s32 step);
s32 DrawRankingScreen(s32 step);
s32 DrawCarSelectScreen(s32 step);
s32 DrawCustomizeScreen(s32 step);
s32 DrawDesignModeScreen(s32 step);
s32 DrawTeamLogoScreen(s32 step);
s32 DrawLogoSampleScreen(s32 step);
s32 DrawTeamNameScreen(s32 step);
s32 DrawPaintColorScreen(s32 step);
u32 DrawEngineerShopScreen(s32 step);
void DrawCarSpecGraph(s32);
void DrawMenuLightBurst(s32);

void InitMenuMode(void) {
    GameRaceProgress *p;
    s32 *scratch;

    SetDispMask(0);
    g_MirrorMode = 0;
    p = g_RaceProgress;
    {
        s32 t0 = p->course;
        s32 t1 = p->carIndex;
        s32 t2 = p->classIndex;
        s32 t3 = p->money;
        g_FrameSyncThreshold = 0x80;
        g_CourseIndex = t0;
        g_PlayerCarIndex = t1;
        g_GrandPrixClass = t2;
        g_PlayerMoney = t3;
    }
    InitRenderState(1);

    SetupDisplay480(0, 0, 0);
    g_SceneId = 8;
    g_SceneTimer = 0;
    if (g_GrandPrixMode != 0) {
        g_GrandPrixSeries = g_SeriesSelection;
    } else {
        g_GrandPrixSeries = *(u16 *)&g_RaceProgress->money;
    }
    g_CourseIndex = ((s32)g_GrandPrixSeries << 2) | g_CourseIndex;
    InitMenuLighting();

    scratch = (s32 *)0x1F800000;
    scratch[3] = -64;
    scratch[4] = -256;
    scratch[2] = 0;
    scratch[6] = 0x100;
    scratch[7] = 0;
    scratch[8] = 0;
    SetCameraRotMatrix();
    ScaleMatrix((void *)0x1F800028, &g_MenuViewScale);

    D_8019C764 = &D_80082568;
    D_801E40B4 = &D_80082568;
    D_8019C794 = &D_80082568;
    D_801E8A44 = &D_80082568;
    D_8009F0B0 = &D_80082568;
    D_8019CB00 = &D_80082568;
    D_801E4188 = &D_80082568;
    g_MenuViewAngle = 500000;
    g_MenuViewAngleTarget = 500000;
    g_UiScriptProgress = 0;
    g_UiScriptProgress2 = 0;
    g_MenuHintBarProgress = 0;
    g_MenuConfirmTimer = 0;
    D_8009B304 = 0;
    GameMenuBusy = 0;
    g_MenuHintBarStep = 0;
    D_8009B310 = 0;
    g_CourseSwapDelay = 0;
    g_MenuViewOffset = 0;
    g_MenuViewOffsetTarget = 0;
    D_8009B360 = 0;
    D_8009B364 = 0;
    D_8009B368 = 0;
    g_MenuPendingCourseIndex = -1;
    g_CarSwapFromIndex = 0;
    g_CarSwapToIndex = -1;
    g_MenuOverlayPattern = 0;
    g_CarNamePlateStep = 0;
    g_MenuPlateCarIndex = 0;
    D_8009B324 = 0;
    D_8009B328 = 0;
    g_MenuCourseModelIndex = g_CourseIndex;
    D_8009B32C = 0;
    D_8009B330 = 0;
    g_TimeAttackPlateStep = 0;
    g_MenuHintButtonsVisible = 1;
    g_MenuHandlerIndex = -1;
    g_MenuHandlerIndex2 = -1;
    g_MenuAltLayoutSetting = 0;
    D_8009B33C = 0;
    g_MenuScreen = 0;
    g_CourseSelectOption = 0;
    D_801E4138 = 0;
    g_RankingOption = 0;
    g_DesignModeOption = 0;
    D_801E4D74 = 0;

    DrawCourseSelectScreen(0);
    DrawRankingScreen(0);
    DrawCarSelectScreen(0);
    DrawCustomizeScreen(0);
    DrawDesignModeScreen(0);
    DrawTeamLogoScreen(0);
    DrawLogoSampleScreen(0);
    DrawTeamNameScreen(0);
    DrawPaintColorScreen(0);
    DrawCarShopScreen(0);
    DrawEngineerShopScreen(0);
    DrawCarSpecGraph(0);
    DrawMenuLightBurst(0);
    DrawTimeAttackPlate(0);
}

/* Counts the enabled entries of g_CarTable. */
s32 CountOwnedCars(void) {
    s32 count = 0;
    s32 i = 0;
    GameCarEntry *entries = g_CarTable;
    s32 flag;

    do {
        flag = entries[i].enabled;
        if (flag != 0) {
            count++;
        }
        i++;
    } while (i < 0xD);

    return count;
}
