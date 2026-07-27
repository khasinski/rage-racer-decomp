#include "common.h"
#include "game/state.h"
#include "game/race.h"
#include "game/render.h"
#include "game/menu.h"
#include "game/car.h"

s32 GameDrawRankingScreen(s32 arg0);

extern u8 D_80011B60;
void func_80016754(s32 arg0, s32 arg1, void *arg2, s32 arg3);
void func_80052738(void) {
    if (g_SceneTimer & 8) {
        func_80016754(0x74, 0xEC, &D_80011B60, 0x78CC);
    }
}

extern s32 D_8009B334;
extern s32 D_8009B360;
extern s32 D_8009B364;
extern s32 D_8009B368;
extern s32 D_801E8268;
extern u8 *g_CourseProgress asm("D_8009E67C");
extern s32 g_PlayerCar asm("D_8009E6D4");
extern s32 D_8009E6D8;
extern s32 D_8009E6DC;
extern s32 D_8009E6F4;
extern s32 D_8009E6F8;
extern s32 D_8009E6FC;
extern s32 D_8009E718;
extern s32 D_8009E71C;
extern s32 g_PlayerTrackProgress asm("D_8009E744");
extern u8 g_TeamLogoClutRect[] asm("D_8007BEDC");
extern u8 g_TeamLogoRect[] asm("D_8007BEE4");
extern u8 g_TeamLogoClut[] asm("D_801E444C");
extern u8 g_TeamLogoCanvas[] asm("D_801E6F2C");
s32 GameRequestCarSelectAssets(void) asm("func_80018530");
void func_8005E88C(void);
void func_80049418(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void LoadImage(void *rect, void *data) asm("func_80065B24");
void func_8001D530(void *arg0, s32 arg1);

void func_80052778(void) {
    register s32 one asm("$16");
    register s32 initValue asm("$3");
    register s32 mode asm("$4");
    register s32 largeValue asm("$5");
    register u8 *table asm("$2");
    register s32 eight asm("$2");

    func_80052738();
    if (GameRequestCarSelectAssets() != 0) {
        return;
    }

    one = 1;
    func_8005E88C();
    g_MenuHandlerIndex = one;
    g_MenuScreen = one;
    func_80049418(0, 0, 0, 0);

    initValue = 0x7A120;
    mode = 0x3D090;
    largeValue = 0x1F0000;
    asm volatile("" : "=r"(largeValue) : "0"(largeValue));
    g_MenuViewOffset = mode;
    mode = g_CourseIndex;
    eight = 8;
    D_801E8268 = eight;
    table = g_CourseProgress;
    largeValue |= 0x4000;
    g_UiScriptProgress = 0;
    g_PlayerCar = 0;
    D_8009E6D8 = 0;
    D_8009E6DC = 0;
    D_8009E6F4 = 0;
    D_8009E6F8 = 0;
    D_8009E6FC = 0;
    g_PlayerTrackProgress = 0;
    D_8009E718 = 0;
    D_8009E71C = 0;
    g_MenuViewAngleTarget = 0x7A120;
    g_MenuViewAngle = initValue;
    g_MenuViewOffsetTarget = 0;
    D_8009B360 = largeValue;
    D_8009B364 = 0;
    D_8009B368 = table[mode & 3];

    if (mode >= 4) {
        D_8009B334 = one;
    } else {
        D_8009B334 = -1;
    }

    LoadImage(g_TeamLogoRect, g_TeamLogoCanvas);
    LoadImage(g_TeamLogoClutRect, g_TeamLogoClut);
    func_8001D530(g_TeamNameChars, g_TeamNameLength);
}

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/menu/func_80052738", func_8005290C);

s32 func_80053650(void) {
    s32 v1 = 0;
    if (g_GrandPrixMode != 0) {
        v1 = (g_SeriesSelection != 0) << 2;
    }
    return v1 < g_CourseIndex;
}

s32 GameCanSelectNextCourse(void) asm("func_80053688");

s32 GameCanSelectNextCourse(void) {
    s32 limit;

    if (g_GrandPrixMode != 0) {
        if (g_SeriesSelection != 0) {
            limit = (g_GrandPrixClass < 2) ? 6 : 7;
        } else {
            limit = (g_GrandPrixClass < 2) ? 2 : 3;
        }
    } else if (g_AdvancedSeriesUnlocked != 0) {
        limit = (g_MaxClassReached[1] < 2) ? 6 : 7;
    } else {
        limit = (g_MaxClassReached[0] < 2) ? 2 : 3;
    }

    return g_CourseIndex < limit;
}

extern s32 D_8009B31C;
extern s32 g_MenuPlateCarIndex asm("D_8009B320");
extern s32 D_8009B30C;
extern s32 g_MenuConfirmTimer asm("D_8009B300");
extern s32 D_8009B310;
extern s32 D_8009B348;
extern s32 D_8009B354;
extern s32 D_8009B36C;
extern s32 D_8009B370;
extern s32 D_8009B374;
extern s32 D_8009B378;
extern u8 g_MenuSubCursor asm("D_8009B2F0");
extern u8 *D_8019C764;
extern s32 D_8019C7AC;
extern s32 D_8019C908;
extern u16 D_8019CABC;
extern u8 D_80081818;
extern u8 D_800817A0;
extern u8 g_UiChromeScript asm("D_80082460");
extern u8 D_800827FC;
extern u8 g_UiChromeScript2 asm("D_80082790");
extern u8 D_80082604;
extern u8 D_800825A4;
extern u8 g_MenuBlankCaption asm("D_80011BA0");
void GameResetCourseProgress(s32 arg0) asm("func_800212F0");
void func_80046A2C(void *ot, s32 x0, s32 y0, s32 x1, s32 y1, s32 u0, s32 v0,
                   s32 r, s32 g, s32 b, s32 clut, s32 sh, s32 st, s32 flags);
s32 func_800487D8(u8 *commands, s32 *progress, s32 step);
void func_800489AC(s32 arg0, s32 arg1, s32 arg2);
void func_80048B88(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6,
                   s32 a7, s32 a8, s32 a9, s32 a10);
void func_80048D64(s32 x0, s32 y0, s32 x1, s32 y1, s32 useFlash);
void func_80049418(s32, s32, s32, s32);
void func_8004CF30(s32 arg0);
void func_8004F3EC(s32 arg0, s32 arg1);
void func_8004FCE8(s32 arg0, s32 arg1, s32 arg2);
s32 func_8005026C(s32 arg0);
void func_800506BC(s32 *p0, s32 *p1, s32 *p2);
void func_800509C4(s32 arg0);
void func_8005194C(void);
void GamePlaySoundCue(s32 cue) asm("func_8005D6EC");
void func_8005E8E0(void);
void GameUpdateCourseSelectScreen(void) asm("func_80053730");
void GameUpdateCourseSelectScreen(void) {
    void *ot;
    u8 *hdr;
    s32 state;
    s32 res;
    s32 sel;
    s32 prev;
    s32 cnt;
    s32 t;
    s32 u;
    s32 lap;
    s32 i;
    GameRaceProgress *p;
    ot = *(void **)0x1F800004;
    g_MenuAltLayout = g_MenuAltLayoutSetting;
    if (g_GrandPrixMode != 0) {
        func_800506BC(&D_8009B364, &D_8009B360, &D_8009B368);
    } else {
        func_800509C4(D_8009B334);
    }
    func_8004FCE8(D_8009B31C, g_MenuPlateCarIndex, 0);
    func_8005194C();
    hdr = &D_80081818;
    if (g_GrandPrixMode != 0) {
        hdr = &D_800817A0;
    }
    state = GameMenuBusy;
    if (state == 0) {
        D_8009B30C = 1;
        func_800487D8(D_8019C764, &g_UiScriptProgress2, -1);
        res = func_80053650();
        func_80049418(1, 1, res, GameCanSelectNextCourse());
        func_800489AC(g_UiScriptProgress, 2, D_8019C7AC);
        func_800487D8(hdr, &g_UiScriptProgress, 0);
        func_8004CF30(7);
        if ((func_800487D8(&g_UiChromeScript, &g_UiScriptProgress, 1) != 0) && (g_UiScriptProgress2 <= 0)) {
            g_MenuOverlayPattern = -1;
            if (g_PadEdge2 & 0x1000) {
                GamePlaySoundCue(1);
                D_8019C7AC = (D_8019C7AC > 0) ? D_8019C7AC - 1 : 2;
            }
            if (g_PadEdge2 & 0x4000) {
                GamePlaySoundCue(1);
                D_8019C7AC = (D_8019C7AC < 2) ? D_8019C7AC + 1 : 0;
            }
            if ((g_PadHeld & 0x8000) && (func_80053650() != 0)) {
                t = g_MenuViewAngleTarget;
                u = g_MenuViewAngle;
                if (t < u ? (u - t <= 0x3D08F) : (t - u <= 0x3D08F)) {
                    if (D_8009B370 < 0) {
{
                        s32 llap;
                        s32 lprev;
                        s32 lu;
                        s32 lt;
                        GamePlaySoundCue(8);
                        lu = g_MenuViewAngle;
                        llap = g_CourseIndex;
                        lprev = g_MenuViewAngleTarget;
                        lt = D_8009B364;
                        g_MenuViewAngleTarget = 0;
                        D_8009B354 = 0;
                        D_8009B36C = llap;
                        llap = llap - 1;
                        g_MenuViewAngle = (lu - lprev) + 0x7A120;
                        D_8009B360 = (D_8009B360 - lt) + 0x1F4000;
                        g_CourseIndex = llap;
                        D_8009B370 = llap;
                        D_8009B368 = g_CourseProgress[llap & 3];
                        D_8009B334 = (llap < 4) ? -1 : 1;
                    }
                    }
                }
            }
            if ((g_PadHeld & 0x2000) && (GameCanSelectNextCourse() != 0)) {
                t = g_MenuViewAngleTarget;
                u = g_MenuViewAngle;
                if (t < u ? (u - t <= 0x3D08F) : (t - u <= 0x3D08F)) {
                    if (D_8009B370 < 0) {
{
                            s32 llap;
                            s32 lprev;
                            s32 lu;
                            s32 lt;
                            s32 lbase;
                            GamePlaySoundCue(8);
                            lu = 0x7A120;
                            llap = g_CourseIndex;
                            lprev = g_MenuViewAngleTarget;
                            lt = g_MenuViewAngle;
                            lbase = D_8009B364;
                            g_MenuViewAngleTarget = 0xF4240;
                            D_8009B354 = 0;
                            D_8009B36C = llap;
                            llap = llap + 1;
                            lprev = lprev - lt;
                            lu = lu - lprev;
                            D_8009B360 = (D_8009B360 - lbase) + 0x1F4000;
                            g_CourseIndex = llap;
                            D_8009B370 = llap;
                            g_MenuViewAngle = lu;
                            D_8009B368 = g_CourseProgress[llap & 3];
                            D_8009B334 = (llap < 4) ? -1 : 1;
                        }
                    }
                }
            }
            if (g_PadEdge2 & 0x860) {
                sel = D_8019C7AC;
                if (sel == 0) {
                    GamePlaySoundCue(2);
                    GameMenuBusy = 1;
                    g_MenuOverlayPattern = 1;
                    D_8009B334 = -1;
                    g_MenuViewOffsetTarget = 0x3D090;
                    D_8009B368 = 0;
                    D_8009B360 = (D_8009B360 - D_8009B364) + 0x1F4000;
                } else if (sel == 2) {
                    if (g_GrandPrixMode != 0) {
                        u16 hv;
                        GamePlaySoundCue(2);
                        hv = 0;
                        if (g_GrandPrixClass < 5) {
                            hv = D_8019CABC;
                        }
                        D_8019C764 = &D_80082604;
                        GameMenuBusy = -1;
                        D_8019CABC = hv;
                        g_UiScriptProgress2 = 0;
                        g_MenuSubCursor = 1;
                    } else {
                        GamePlaySoundCue(3);
                        func_8005E8E0();
                        D_8009B30C = -1;
                        D_8009B334 = -1;
                        g_MenuViewOffsetTarget = 0x3D090;
                        GameMenuBusy = sel;
                        D_8009B368 = 0;
                        D_8019CABC = g_CourseIndex >> 2;
                        D_8009B360 = (D_8009B360 - D_8009B364) + 0x1F4000;
                    }
                } else {
                    GamePlaySoundCue(2);
                    if (g_GrandPrixMode != 0) {
                        D_8019C764 = &D_800825A4;
                        GameMenuBusy = -2;
                        g_UiScriptProgress2 = 0;
                        g_MenuSubCursor = g_GrandPrixClass;
                    } else {
                        GameMenuBusy = 3;
                        g_MenuOverlayPattern = 1;
                        D_8009B334 = -1;
                    }
                }
            }
        }
    } else if (state < 0) {
        if (state == -1) {
            u16 *pad;
            func_800487D8(&D_800827FC, &g_UiScriptProgress2, 0);
            func_800487D8(&g_UiChromeScript2, &g_UiScriptProgress2, 0);
            if (func_800487D8(D_8019C764, &g_UiScriptProgress2, 1) != 0) {
                if (g_PadEdge2 & 0x860) {
                    GamePlaySoundCue((g_MenuSubCursor != 0) ? 2 : 3);
                    GameMenuBusy = -3;
                    g_MenuConfirmTimer = 0x23;
                }
                pad = (u16 *)&g_PadEdge2;
                if (*pad & 0x90) {
                    GamePlaySoundCue(3);
                    GameMenuBusy = -4;
                }
                if (*pad & 0x8000) {
                    GamePlaySoundCue(1);
                    g_MenuSubCursor = 1;
                }
                if (*pad & 0x2000) {
                    GamePlaySoundCue(1);
                    g_MenuSubCursor = 0;
                }
                func_80048D64((g_MenuSubCursor != 0) ? 0xB8 : 0xDA, 0x8C, 0x20, 0x20, 0);
                func_80046A2C(ot, 0xC0, 0x94, 0x10, 0x10, 0x9D, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                func_80046A2C(ot, 0xE3, 0x94, 0x10, 0x10, 0xAD, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                func_80048B88(0xB8, 0x8C, 0x20, 0x20, 0x95, 0x25, 0x1E, 0, 0, 0, (s32)&g_MenuBlankCaption);
                func_80048B88(0xDA, 0x8C, 0x20, 0x20, 0x1E, 0x4E, 0x95, 0, 0, 0, (s32)&g_MenuBlankCaption);
            }
        } else if (state == -2) {
            u16 *pad;
            if (func_800487D8(D_8019C764, &g_UiScriptProgress2, 1) != 0) {
                if (g_PadEdge2 & 0x860) {
                    GamePlaySoundCue(2);
                    if (g_MenuSubCursor == g_GrandPrixClass) {
                        GameMenuBusy = 0;
                    } else {
                        GameMenuBusy = -5;
                        D_8009B310 = 0;
                        g_MenuConfirmTimer = 0x23;
                        func_8005026C(0);
                    }
                }
                pad = (u16 *)&g_PadEdge2;
                if (*pad & 0x90) {
                    GamePlaySoundCue(3);
                    GameMenuBusy = 0;
                }
                if (*pad & 0x1000) {
                    GamePlaySoundCue(1);
                    g_MenuSubCursor = (g_MenuSubCursor != 0) ? g_MenuSubCursor - 1 : g_RaceProgress->maxClassReached;
                }
                if (g_PadEdge2 & 0x4000) {
                    GamePlaySoundCue(1);
                    g_MenuSubCursor = (g_MenuSubCursor < g_RaceProgress->maxClassReached) ? g_MenuSubCursor + 1 : 0;
                }
                func_80048D64(0xB8, g_MenuSubCursor * 0x1E + 0x6C, 0x38, 0x20, 0);
                for (i = 0; i < g_RaceProgress->maxClassReached + 1; i++) {
                    func_80046A2C(ot, 0xC0, i * 0x1E + 0x74, 0x1A, 0x10, 0x60, 0xCC, 0, 0, 0, 0x244, 1, 1, 0x3B);
                    func_80046A2C(ot, 0xE0, i * 0x1E + 0x74, 8, 0x10, i * 8 + 8, 0x18, 0, 0, 0, 0x244, 1, 1, 0x3B);
                    func_80048B88(0xB8, i * 0x1E + 0x6C, 0x38, 0x20, 0x95, 0x25, 0x1E, 0, 0, 0, (s32)&g_MenuBlankCaption);
                }
            }
        } else if (state == -3) {
            cnt = g_MenuConfirmTimer;
            if (cnt <= 0) {
                func_800487D8(&D_800827FC, &g_UiScriptProgress2, -1);
                func_800487D8(&g_UiChromeScript2, &g_UiScriptProgress2, 0);
                func_800487D8(D_8019C764, &g_UiScriptProgress2, 0);
                if (g_UiScriptProgress2 <= 0) {
                    func_8005E8E0();
                    GameMenuBusy = (g_MenuSubCursor != 0) ? 4 : 2;
                    D_8009B30C = -1;
                    g_MenuViewOffsetTarget = 0x3D090;
                    D_8009B368 = 0;
                    D_8009B360 = (D_8009B360 - D_8009B364) + 0x1F4000;
                }
            } else {
                g_MenuConfirmTimer = cnt - 1;
                func_800487D8(&D_800827FC, &g_UiScriptProgress2, 0);
                func_800487D8(&g_UiChromeScript2, &g_UiScriptProgress2, 0);
                func_800487D8(D_8019C764, &g_UiScriptProgress2, 1);
                func_80048D64((g_MenuSubCursor != 0) ? 0xB8 : 0xDA, 0x8C, 0x20, 0x20, 1);
                func_80046A2C(ot, 0xC0, 0x94, 0x10, 0x10, 0x9D, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                func_80046A2C(ot, 0xE3, 0x94, 0x10, 0x10, 0xAD, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                func_80048B88(0xB8, 0x8C, 0x20, 0x20, 0x95, 0x25, 0x1E, 0, 0, 0, (s32)&g_MenuBlankCaption);
                func_80048B88(0xDA, 0x8C, 0x20, 0x20, 0x1E, 0x4E, 0x95, 0, 0, 0, (s32)&g_MenuBlankCaption);
            }
        } else if (state == -4) {
            func_800487D8(&D_800827FC, &g_UiScriptProgress2, -1);
            func_800487D8(&g_UiChromeScript2, &g_UiScriptProgress2, 0);
            func_800487D8(D_8019C764, &g_UiScriptProgress2, 0);
            if (g_UiScriptProgress2 <= 0) {
                GameMenuBusy = 0;
            }
        } else if (state == -5) {
            cnt = g_MenuConfirmTimer;
            if (cnt <= 0) {
                if (D_8009B310 != 0) {
                    if (func_8005026C(-1) == 0) {
                        GameMenuBusy = 0;
                        g_UiScriptProgress2 = 0;
                    }
                } else {
                    if (func_8005026C(1) >= 0x19) {
                        D_8009B310 = 1;
                        g_GrandPrixClass = g_MenuSubCursor;
                        GameResetCourseProgress(g_MenuSubCursor);
                        g_MenuViewAngle = 0x7A120;
                        g_MenuViewAngleTarget = 0x7A120;
                        D_8019C7AC = 0;
                        D_8009B370 = -1;
                        D_8009B360 = 0;
                        g_CourseIndex = g_CourseIndex & ~3;
                        D_8009B36C = g_CourseIndex;
                        D_8009B368 = g_CourseProgress[0];
                    }
                    func_800487D8(D_8019C764, &g_UiScriptProgress2, 1);
                    func_80048D64(0xB8, g_MenuSubCursor * 0x1E + 0x6C, 0x38, 0x20, 1);
                    for (i = 0; i < g_RaceProgress->maxClassReached + 1; i++) {
                        func_80046A2C(ot, 0xC0, i * 0x1E + 0x74, 0x1A, 0x10, 0x60, 0xCC, 0, 0, 0, 0x244, 1, 1, 0x3B);
                        func_80046A2C(ot, 0xE0, i * 0x1E + 0x74, 8, 0x10, i * 8 + 8, 0x18, 0, 0, 0, 0x244, 1, 1, 0x3B);
                        func_80048B88(0xB8, i * 0x1E + 0x6C, 0x38, 0x20, 0x95, 0x25, 0x1E, 0, 0, 0, (s32)&g_MenuBlankCaption);
                    }
                }
            } else {
                g_MenuConfirmTimer = cnt - 1;
                func_800487D8(D_8019C764, &g_UiScriptProgress2, 1);
                func_80048D64(0xB8, g_MenuSubCursor * 0x1E + 0x6C, 0x38, 0x20, 1);
                for (i = 0; i < g_RaceProgress->maxClassReached + 1; i++) {
                    func_80046A2C(ot, 0xC0, i * 0x1E + 0x74, 0x1A, 0x10, 0x60, 0xCC, 0, 0, 0, 0x244, 1, 1, 0x3B);
                    func_80046A2C(ot, 0xE0, i * 0x1E + 0x74, 8, 0x10, i * 8 + 8, 0x18, 0, 0, 0, 0x244, 1, 1, 0x3B);
                    func_80048B88(0xB8, i * 0x1E + 0x6C, 0x38, 0x20, 0x95, 0x25, 0x1E, 0, 0, 0, (s32)&g_MenuBlankCaption);
                }
            }
        }
        res = func_80053650();
        func_80049418(1, 1, res, GameCanSelectNextCourse());
        func_800489AC(g_UiScriptProgress, 2, D_8019C7AC);
        func_800487D8(hdr, &g_UiScriptProgress, 0);
        func_800487D8(&g_UiChromeScript, &g_UiScriptProgress, 1);
        func_8004CF30(7);
    } else {
        g_MenuHandlerIndex = -1;
        g_MenuHandlerIndex2 = 1;
        res = func_80053650();
        func_80049418(-1, 1, res, GameCanSelectNextCourse());
        func_800487D8(hdr, &g_UiScriptProgress, -1);
        func_800487D8(&g_UiChromeScript, &g_UiScriptProgress, 0);
        func_800489AC(g_UiScriptProgress, 2, D_8019C7AC);
        func_8004CF30(-9);
        if (g_UiScriptProgress <= 0) {
            switch (GameMenuBusy) {
            case 1:
                if (g_MenuViewOffset > 0x3D08F) {
                    g_MenuScreen = 3;
                    g_MenuHandlerIndex = 4;
                    func_8004F3EC(0, 0);
                    func_80049418(0, 0, 0, 0);
                    D_8009B378 = -1;
                    g_MenuViewAngle = 0;
                    g_MenuViewAngleTarget = 0;
                    g_MenuViewOffset = 0x3D090;
                    g_MenuViewOffsetTarget = 0;
                    D_8009B374 = g_PlayerCarIndex;
                    goto clear;
                }
                break;
            case 2:
                if ((D_8009B348 <= 0) && (g_MenuViewOffset > 0x3D08F)) {
                    s32 raw;
                    s32 d;
                    s32 lapc;
                    s32 half;
                    raw = g_CourseIndex;
                    p = g_RaceProgress;
                    d = g_PlayerCarIndex;
                    g_SceneId = 2;
                    lapc = g_GrandPrixClass;
                    half = g_GrandPrixMode;
                    raw = raw & 3;
                    g_CourseIndex = raw;
                    p->course = raw;
                    p->carIndex = d;
                    p->classIndex = lapc;
                    if (half != 0) {
                        p->unk10 = D_8019C908;
                        goto clear;
                    }
                    goto setlast;
                }
                break;
            case 3:
                g_MenuScreen = 2;
                g_MenuHandlerIndex = 2;
                goto clear;
            case 4:
                if ((D_8009B348 <= 0) && (g_MenuViewOffset > 0x3D08F)) {
                    s32 raw;
                    s32 d;
                    s32 lapc;
                    s32 half;
                    g_SceneId = 0x18;
                    raw = g_CourseIndex & 3;
                    g_RaceProgress->course = (g_CourseIndex = raw);
                    d = g_PlayerCarIndex;
                    lapc = g_GrandPrixClass;
                    half = g_GrandPrixMode;
                    g_RaceProgress->carIndex = d;
                    g_RaceProgress->classIndex = lapc;
                    if (half != 0) {
                        g_RaceProgress->unk10 = D_8019C908;
                    } else {
                        p = g_RaceProgress;
                    setlast:
                        p->unk10 = (s16)D_8019CABC;
                    }
                clear:
                    g_UiScriptProgress = 0;
                    GameMenuBusy = 0;
                }
                break;
            default:
                goto clear;
            }
        }
    }
}

extern s32 D_8009B2C4;

s32 GameDrawRankingScreen(s32 step) asm("func_80054C84");
s32 GameDrawRankingScreen(s32 arg0) {
    s32 value;

    if (arg0 == 0) {
        D_8009B2C4 = 0;
        return;
    }

    if (arg0 > 0) {
        value = arg0 + D_8009B2C4;
        D_8009B2C4 = value;
        if (value >= 0x1FD) {
            D_8009B2C4 = 0x1FC;
        }
    } else {
        value = arg0 + D_8009B2C4;
        D_8009B2C4 = value;
        if (value < 0) {
            D_8009B2C4 = 0;
        }
    }

    return D_8009B2C4;
}
