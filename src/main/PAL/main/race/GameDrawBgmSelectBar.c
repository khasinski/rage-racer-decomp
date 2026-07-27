#include "common.h"
#include "game/state.h"
#include "game/render.h"
#include "game/menu.h"
#include "game/car.h"
#include "psyq/gpu.h"
#include "game/cd.h"

extern s32 g_BgmSelectCursor asm("D_801E4B84");
extern s32 g_BgmRandomLabelTimer asm("D_8007D6B0");
extern s32 g_BgmSelectTrack asm("D_801E4404");
s32 func_80016EC4(u8 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
s32 func_8001720C(u8 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
s32 func_80017390(u8 *arg0, s32 arg1, s32 arg2);
extern u32 g_BgmShuffleIndex asm("D_8009E6CC");
extern s32 g_BgmTrackCount asm("D_801E40A8");
extern u8 g_BgmShuffleOrder[] asm("D_801E7734");
void func_8001B488(void);
extern s32 g_BgmChangeDelay asm("D_8019CAF4");
extern s32 g_BgmSelectCdTrack asm("D_8019CE00");
extern s32 g_CdTrackEnded asm("D_8019C7BC");
extern s32 g_BgmSelectStep asm("D_8019C99C");
extern s32 g_BgmRandomPlay asm("D_8007D6B4");
extern s32 g_BgmSelectShowUi asm("D_801E412C");
extern s32 g_CameraViewMode asm("D_8009E870");
void GameAdvanceBgmShuffleBag() asm("func_80025E54");
void GameDrawFullscreenFadeTile() asm("func_80033AA0");
void GameRequestOptionScreenAssets() asm("func_80018B98");
void GameDrawBgmSelectBar() asm("func_80025C58");
void func_8003BB50();
void GameRequestTrackTexturePage() asm("func_80019EFC");
void GameUpdateCamera() asm("func_80043BCC");
void func_800418D4();
void GameDrawTerrainCellsWide() asm("func_80041888");
void GameDrawCourseScenery2() asm("func_8003E2E8");
int GameCycleBgmSelectCameraCar() asm("func_8001A0E4");

void GameDrawBgmSelectBar(void) asm("func_80025C58");
void GameDrawBgmSelectBar(void) {
    u8 *base;
    s32 arg4;
    s32 arg5;
    s32 temp;
    s32 product;
    s32 value;
    s32 next;

    base = g_DrawBuffer + 0xD0;
    next = *(s32 *)0x1F800000;
    temp = (g_BgmSelectCursor == 0) ? 0x3FEC : 0x3FEF;
    arg4 = 0x14;
    arg5 = 0x10;

    next = func_80016EC4(base, next, 0x20, 0xC1, arg4, arg5, 0, 0, temp);
    temp = (g_BgmSelectCursor == 1) ? 0x3FEC : 0x3FEF;
    next = func_80016EC4(base, next, 0x36, 0xC1, arg4, arg5, arg4, 0, temp);
    temp = (g_BgmSelectCursor == 2) ? 0x3FEC : 0x3FEF;
    next = func_80016EC4(base, next, 0x4C, 0xC1, arg4, arg5, 0x28, 0, temp);

    if (g_BgmRandomLabelTimer != 0) {
        g_BgmRandomLabelTimer--;
        temp = 0x10;
    } else {
        value = g_BgmSelectTrack;
        product = value << 1;
        product += value;
        product <<= 2;
        temp = product + 0x1C;
    }

    next = func_80016EC4(base, next, 0x64, 0xC2, 0xBA, 0xC, 0, temp, 0x3FED);
    next = func_80016EC4(base, next, 0x62, 0xC0, 0xBE, 0x10, 0x3C, 0, 0x3FEE);
    next = func_8001720C(base, next, 0x14, 0xB8, 0x118, 0x20, 0, 0, 0);
    *(s32 *)0x1F800000 = func_80017390(base, next, 0xB);
}

void GameAdvanceBgmShuffleBag(u32 arg0) asm("func_80025E54");
void GameAdvanceBgmShuffleBag(u32 arg0) {
    u8 *first;
    u8 *before;
    u8 *other;
    u8 value;

    g_BgmShuffleIndex++;
    if (g_BgmShuffleIndex == g_BgmTrackCount) {
        func_8001B488();

        first = g_BgmShuffleOrder;
        before = first - 1;
        if (arg0 == first[0]) {
            other = g_BgmTrackCount + before;
            value = *other;
            first[0] = value;
            *other = arg0;
        }
    }
}

void GameUpdateBgmSelect(void) asm("func_80025ED8");
void GameUpdateBgmSelect(void) {
    s32 t;
    if (g_BgmChangeDelay <= 0) goto L78;
    t = g_BgmChangeDelay - 1;
    g_BgmChangeDelay = t;
    if (t == 4) goto L13c;
    if (t != 0) goto L13c;
    if (g_BgmSelectCdTrack == 12) g_BgmSelectCdTrack = 17;
    GameRequestCdTrack(g_BgmSelectCdTrack);
    GameStartCdAudio();
    g_CdTrackEnded = 0;
    goto L13c;

L78:
    if (g_CdTrackEnded == 0) goto L13c;
    g_BgmChangeDelay = 6;
    if (g_BgmRandomPlay == 0) goto Ld0;
    g_BgmSelectTrack = g_BgmShuffleOrder[g_BgmShuffleIndex];
    GameAdvanceBgmShuffleBag(g_BgmSelectTrack);
    goto L124;
Ld0:
    g_BgmSelectTrack = g_BgmSelectTrack + 1;
    g_BgmSelectTrack = (g_BgmSelectTrack + g_BgmTrackCount) % g_BgmTrackCount;
L124:
    g_BgmSelectCdTrack = g_BgmSelectTrack + 3;

L13c:
    if (g_SceneTimer == 2) SetDispMask(1);
    if (g_FadeStep != 0) goto L424;

    if (g_PadEdge2 & 0x8000) {
        if (g_BgmSelectCursor > 0) g_BgmSelectCursor = g_BgmSelectCursor - 1;
    }
    if (g_PadEdge2 & 0x2000) {
        if (g_BgmSelectCursor < 2) g_BgmSelectCursor = g_BgmSelectCursor + 1;
    }
    if (g_PadEdge2 & 1) {
        s32 p;
        s32 h0;
        func_8001B488();
        h0 = g_BgmShuffleOrder[0];
        p = g_BgmSelectTrack;
        if (p == h0) {
            u8 tmp = g_BgmShuffleOrder[g_BgmTrackCount - 1];
            g_BgmShuffleOrder[0] = tmp;
            g_BgmShuffleOrder[g_BgmTrackCount - 1] = p;
        }
        g_BgmRandomPlay = 1;
        g_BgmRandomLabelTimer = 60;
    }
    {
        u16 f = g_PadEdge2;
        if (f & 2) {
            g_BgmRandomPlay = 0;
            g_BgmRandomLabelTimer = 0;
        }
        if (f & 0x860) {
            switch (g_BgmSelectCursor) {
            case 0:
                if (g_BgmRandomPlay == 0) {
                    g_BgmSelectTrack = g_BgmSelectTrack - 1;
                    g_BgmSelectTrack = (g_BgmSelectTrack + g_BgmTrackCount) % g_BgmTrackCount;
                }
                goto lab380;
            case 2:
                if (g_BgmRandomPlay != 0) {
                    g_BgmSelectTrack = g_BgmShuffleOrder[g_BgmShuffleIndex];
                    GameAdvanceBgmShuffleBag(g_BgmSelectTrack);
                } else {
                    g_BgmSelectTrack = g_BgmSelectTrack + 1;
                    g_BgmSelectTrack = (g_BgmSelectTrack + g_BgmTrackCount) % g_BgmTrackCount;
                }
            lab380:
                if (g_BgmChangeDelay == 0) {
                    GameStartCdVolumeFade(60);
                    g_BgmChangeDelay = 0x40;
                }
                g_BgmSelectCdTrack = g_BgmSelectTrack + 3;
                break;
            case 1:
                GameStartCdVolumeFade(60);
                g_FadeStep = 4;
                break;
            }
        } else if (f & 0x90) {
            GameStartCdVolumeFade(60);
            g_FadeStep = 4;
        }
    }
    {
        u16 f = g_PadEdge2;
        if (f & 4) g_BgmSelectShowUi = 1;
        if (f & 8) g_BgmSelectShowUi = 0;
    }
    goto L48c;

L424:
    GameDrawFullscreenFadeTile(g_FadeLevel, 0x49);
    g_FadeLevel = g_FadeLevel + g_FadeStep;
    if (g_FadeLevel >= 256) {
        GameRequestOptionScreenAssets();
        g_BgmSelectStep = 3;
        g_FadeLevel = 256;
        g_FadeStep = -4;
    }

L48c:
    if (g_BgmSelectShowUi != 0) GameDrawBgmSelectBar();
    g_AnimTimer = g_AnimTimer + 1;
    g_CameraCarIndex = GameCycleBgmSelectCameraCar(0xff, g_CameraCarIndex);
    func_8003BB50();
    GameRequestTrackTexturePage(g_Cars[g_CameraCarIndex].field_78);
    GameUpdateCamera(g_CameraViewMode, &g_Cars[g_CameraCarIndex]);
    GameDrawCars();
    GameUpdateEnvironment();
    func_800418D4();
    *(s32 *)0x1F800084 = g_IsEnvironmentMode4;
    GameDrawTerrainCellsWide();
    GameDrawCourseObjects();
    GameDrawCourseScenery2(g_AnimTimer, 1);
}
