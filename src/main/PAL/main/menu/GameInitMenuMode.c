#include "common.h"
#include "psyq/gte.h"
#include "game/render.h"
#include "game/race.h"
#include "game/state.h"
#include "game/menu.h"
#include "game/car.h"
#include "psyq/gpu.h"

extern s32 D_8007FB40;
extern s32 D_8007FB44;
extern s32 g_MenuAltLayout asm("D_8019CB0C");

void func_80046E00();

void func_80050400(s32 arg0, s32 arg1) {
    s32 step0;
    s32 step1;
    void *scratch;
    /* This pin is load-bearing: removing it changes .text. */
    register s32 zero asm("$0");
    s32 value;
    s32 offset;
    s32 x0;
    /* This pin is load-bearing: removing it changes .text. */
    register s32 y0 asm("$6");
    s32 render1;
    s16 y1;
    /* These pins are load-bearing: removing any one changes .text. */
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
        func_80046E00(
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
        func_80046E00(
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

typedef struct SVec {
    s16 x;
    s16 y;
    s16 z;
    s16 pad;
} SVec;

typedef struct UVec {
    u16 x;
    u16 y;
    u16 z;
    u16 pad;
} UVec;

extern SVec D_80011A48[];
extern s32 D_8007FB48;


void GameFlipCourseCard(s32 *p0, s32 *p1, s32 *p2) asm("func_800506BC");
void GameFlipCourseCard(s32 *p0, s32 *p1, s32 *p2) {
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

    GameBuildRotMatrixY(&mtx, n);
    ApplyMatrixSV((s32 *)&mtx, &verts[0], (s16 *)&out[0]);
    ApplyMatrixSV((s32 *)&mtx, &verts[1], (s16 *)&out[1]);
    ApplyMatrixSV((s32 *)&mtx, &verts[2], (s16 *)&out[2]);
    ApplyMatrixSV((s32 *)&mtx, &verts[3], (s16 *)&out[3]);

    {
        /* These pins are load-bearing: removing any one changes .text. */
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

        func_80046E00(scratch2,
            x0, y0, x1, y1, x2, y2, x3, y3,
            0xA0, 0x70, 0xDF, 0x70, 0xA0, 0xBF, 0xDF, 0xBF,
            0x7F, 0x7F, 0x7F,
            depth,
            0, 0,
            0x1C);
    }
}

extern volatile s32 D_8007FB4C;

void func_800509C4(s32 arg0) {
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
        func_80046E00(
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

extern Matrix g_SceneColorMatrix asm("D_8019CAD4");
extern Matrix D_80082DFC;
extern Matrix g_SceneLightMatrix asm("D_8009E6AC");
extern Matrix D_80082E1C;
void func_800698B8(Matrix *arg0);
void func_80069888(Matrix *arg0);
void func_80069A18(s32 arg0, s32 arg1, s32 arg2);
void func_80069A38(s32 arg0, s32 arg1, s32 arg2);
void func_800686D4(s32 arg0, s32 arg1);

/* The menu-mode twin of GameInitTrackLighting. */
void GameInitMenuLighting(void) asm("func_80050B1C");
void GameInitMenuLighting(void) {
    g_SceneColorMatrix = D_80082DFC;
    g_SceneLightMatrix = D_80082E1C;
    func_800698B8(&g_SceneColorMatrix);
    func_80069888(&g_SceneLightMatrix);
    func_80069A18(0x20, 0x20, 0x20);
    func_80069A38(0, 0, 0);
    func_800686D4(0x4E20, 0x140);
}

extern s32 g_FrameSyncThreshold asm("D_8019C768");
extern s32 g_PlayerMoney asm("D_8019C908");

extern s32 D_80082568;
extern s32 g_MenuViewScale asm("D_80082D6C");

extern void *D_8019C764;
extern void *D_801E40B4;
extern void *D_8019C794;
extern void *D_801E8A44;
extern void *D_8009F0B0;
extern void *D_8019CB00;
extern void *D_801E4188;

extern s32 g_MenuHintBarProgress asm("D_8009B2FC");
extern s32 g_MenuConfirmTimer asm("D_8009B300");
extern s32 D_8009B304;
extern s32 g_MenuHintBarStep asm("D_8009B30C");
extern s32 D_8009B310;
extern s32 g_MenuHintButtonsVisible asm("D_8009B314");
extern s32 g_CarNamePlateStep asm("D_8009B31C");
extern s32 g_MenuPlateCarIndex asm("D_8009B320");
extern s32 D_8009B324;
extern s32 D_8009B328;
extern s32 D_8009B32C;
extern s32 D_8009B330;
extern s32 g_TimeAttackPlateStep asm("D_8009B334");
extern s32 D_8009B33C;
extern s32 g_CourseSwapDelay asm("D_8009B354");
extern s32 D_8009B360;
extern s32 D_8009B364;
extern s32 D_8009B368;
extern s32 g_MenuCourseModelIndex asm("D_8009B36C");
extern s32 g_MenuPendingCourseIndex asm("D_8009B370");
extern s32 g_CarSwapFromIndex asm("D_8009B374");
extern s32 g_CarSwapToIndex asm("D_8009B378");

extern s32 g_CourseSelectOption asm("D_8019C7AC");
extern s32 D_801E4138;
extern s32 g_RankingOption asm("D_8019C7C0");
extern s32 g_DesignModeOption asm("D_8019C758");
extern s32 D_801E4D74;

void GameInitRenderState(s32) asm("func_80017884");
void func_8005290C(s32);
void func_80054C84(s32);
void func_800551BC(s32);
void func_800562C8(s32);
void func_80056E64(s32);
void func_800576BC(s32);
void func_8005803C(s32);
void func_800586B0(s32);
void func_80058B88(s32);
void func_80059248(s32);
void func_8005A2CC(s32);
void func_800496F0(s32);
void func_8004CF30(s32);

void GameInitMenuMode(void) asm("func_80050C18");
void GameInitMenuMode(void) {
    GameRaceProgress *p;
    s32 *scratch;

    SetDispMask(0);
    g_MirrorMode = 0;
    p = g_RaceProgress;
    {
        s32 t0 = p->course;
        s32 t1 = p->carIndex;
        s32 t2 = p->classIndex;
        s32 t3 = p->unk10;
        g_FrameSyncThreshold = 0x80;
        g_CourseIndex = t0;
        g_PlayerCarIndex = t1;
        g_GrandPrixClass = t2;
        g_PlayerMoney = t3;
    }
    GameInitRenderState(1);

    GameSetupDisplay480(0, 0, 0);
    g_SceneId = 8;
    g_SceneTimer = 0;
    if (g_GrandPrixMode != 0) {
        g_GrandPrixSeries = g_SeriesSelection;
    } else {
        g_GrandPrixSeries = *(u16 *)&g_RaceProgress->unk10;
    }
    g_CourseIndex = ((s32)g_GrandPrixSeries << 2) | g_CourseIndex;
    GameInitMenuLighting();

    scratch = (s32 *)0x1F800000;
    scratch[3] = -64;
    scratch[4] = -256;
    scratch[2] = 0;
    scratch[6] = 0x100;
    scratch[7] = 0;
    scratch[8] = 0;
    GameSetCameraRotMatrix();
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

    func_8005290C(0);
    func_80054C84(0);
    func_800551BC(0);
    func_800562C8(0);
    func_80056E64(0);
    func_800576BC(0);
    func_8005803C(0);
    func_800586B0(0);
    func_80058B88(0);
    func_80059248(0);
    func_8005A2CC(0);
    func_800496F0(0);
    func_8004CF30(0);
    func_800509C4(0);
}

/* Counts the enabled entries of g_CarTable. */
s32 GameCountOwnedCars(void) asm("func_80050F68");
s32 GameCountOwnedCars(void) {
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
