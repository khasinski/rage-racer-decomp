#include "common.h"
#include "game/race.h"
#include "game/state.h"
#include "game/render.h"
#include "game/menu.h"
#include "game/car.h"
#include "psyq/gpu.h"

extern s32 D_8019C768;
extern s32 D_8019C908;

extern s32 D_80082568;
extern s32 D_80082D6C;

extern void *D_8019C764;
extern void *D_801E40B4;
extern void *D_8019C794;
extern void *D_801E8A44;
extern void *D_8009F0B0;
extern void *D_8019CB00;
extern void *D_801E4188;

extern s32 D_8009B2FC;
extern s32 g_MenuConfirmTimer asm("D_8009B300");
extern s32 D_8009B304;
extern s32 D_8009B30C;
extern s32 D_8009B310;
extern s32 D_8009B314;
extern s32 D_8009B31C;
extern s32 g_MenuPlateCarIndex asm("D_8009B320");
extern s32 D_8009B324;
extern s32 D_8009B328;
extern s32 D_8009B32C;
extern s32 D_8009B330;
extern s32 D_8009B334;
extern s32 D_8009B33C;
extern s32 D_8009B354;
extern s32 D_8009B360;
extern s32 D_8009B364;
extern s32 D_8009B368;
extern s32 D_8009B36C;
extern s32 D_8009B370;
extern s32 D_8009B374;
extern s32 D_8009B378;

extern s32 D_8019C7AC;
extern s32 D_801E4138;
extern s32 D_8019C7C0;
extern s32 D_8019C758;
extern s32 D_801E4D74;

void GameInitRenderState(s32) asm("func_80017884");
void func_80050B1C(void);
void func_80069728(void *, void *);
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
void func_800509C4(s32);

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
        D_8019C768 = 0x80;
        g_CourseIndex = t0;
        g_PlayerCarIndex = t1;
        g_GrandPrixClass = t2;
        D_8019C908 = t3;
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
    func_80050B1C();

    scratch = (s32 *)0x1F800000;
    scratch[3] = -64;
    scratch[4] = -256;
    scratch[2] = 0;
    scratch[6] = 0x100;
    scratch[7] = 0;
    scratch[8] = 0;
    GameSetCameraRotMatrix();
    func_80069728((void *)0x1F800028, &D_80082D6C);

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
    D_8009B2FC = 0;
    g_MenuConfirmTimer = 0;
    D_8009B304 = 0;
    GameMenuBusy = 0;
    D_8009B30C = 0;
    D_8009B310 = 0;
    D_8009B354 = 0;
    g_MenuViewOffset = 0;
    g_MenuViewOffsetTarget = 0;
    D_8009B360 = 0;
    D_8009B364 = 0;
    D_8009B368 = 0;
    D_8009B370 = -1;
    D_8009B374 = 0;
    D_8009B378 = -1;
    g_MenuOverlayPattern = 0;
    D_8009B31C = 0;
    g_MenuPlateCarIndex = 0;
    D_8009B324 = 0;
    D_8009B328 = 0;
    D_8009B36C = g_CourseIndex;
    D_8009B32C = 0;
    D_8009B330 = 0;
    D_8009B334 = 0;
    D_8009B314 = 1;
    g_MenuHandlerIndex = -1;
    g_MenuHandlerIndex2 = -1;
    g_MenuAltLayoutSetting = 0;
    D_8009B33C = 0;
    g_MenuScreen = 0;
    D_8019C7AC = 0;
    D_801E4138 = 0;
    D_8019C7C0 = 0;
    D_8019C758 = 0;
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
