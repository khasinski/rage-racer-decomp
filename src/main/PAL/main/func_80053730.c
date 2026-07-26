#include "common.h"
#include "game/race.h"
#include "game/state.h"
#include "game/menu.h"
#include "game/render.h"
extern s32 D_8009B338;
extern s32 D_8019CB0C;
extern s16 D_801E4DAC;
extern s32 D_8009B334;
extern s32 D_8009B31C;
extern s32 D_8009B320;
extern s32 D_8009B30C;
extern s32 D_8009B300;
extern s32 D_8009B310;
extern s32 D_8009B344;
extern s32 D_8009B348;
extern s32 D_8009B34C;
extern s32 D_8009B350;
extern s32 D_8009B354;
extern s32 D_8009B358;
extern s32 D_8009B35C;
extern s32 D_8009B360;
extern s32 D_8009B364;
extern s32 D_8009B368;
extern s32 D_8009B36C;
extern s32 D_8009B370;
extern s32 D_8009B374;
extern s32 D_8009B378;
extern u8 D_8009B2F0;
extern u8 *D_8019C764;
extern s32 D_8019C9F8;
extern s32 D_8019C7AC;
extern s32 D_8019C908;
extern u16 D_8019CABC;
extern u16 D_801E436A;
extern s32 D_801E40D4;
extern GameRaceProgress *D_801E4FAC;
extern u8 *D_8009E67C;
extern u8 D_80081818;
extern u8 D_800817A0;
extern u8 D_80082460;
extern u8 D_800827FC;
extern u8 D_80082790;
extern u8 D_80082604;
extern u8 D_800825A4;
extern u8 D_80011BA0;
void func_800212F0(s32 arg0);
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
s32 func_80053650(void);
s32 GameCanSelectNextCourse(void) asm("func_80053688");
void func_8005D6EC(s32 arg0);
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
    D_8019CB0C = D_8009B338;
    if (D_801E4DAC != 0) {
        func_800506BC(&D_8009B364, &D_8009B360, &D_8009B368);
    } else {
        func_800509C4(D_8009B334);
    }
    func_8004FCE8(D_8009B31C, D_8009B320, 0);
    func_8005194C();
    hdr = &D_80081818;
    if (D_801E4DAC != 0) {
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
        if ((func_800487D8(&D_80082460, &g_UiScriptProgress, 1) != 0) && (g_UiScriptProgress2 <= 0)) {
            g_MenuOverlayPattern = -1;
            if (g_PadEdge2 & 0x1000) {
                func_8005D6EC(1);
                D_8019C7AC = (D_8019C7AC > 0) ? D_8019C7AC - 1 : 2;
            }
            if (g_PadEdge2 & 0x4000) {
                func_8005D6EC(1);
                D_8019C7AC = (D_8019C7AC < 2) ? D_8019C7AC + 1 : 0;
            }
            if ((D_801E436A & 0x8000) && (func_80053650() != 0)) {
                t = D_8009B350;
                u = D_8009B34C;
                if (t < u ? (u - t <= 0x3D08F) : (t - u <= 0x3D08F)) {
                    if (D_8009B370 < 0) {
{
                        s32 llap;
                        s32 lprev;
                        s32 lu;
                        s32 lt;
                        func_8005D6EC(8);
                        lu = D_8009B34C;
                        llap = g_CourseIndex;
                        lprev = D_8009B350;
                        lt = D_8009B364;
                        D_8009B350 = 0;
                        D_8009B354 = 0;
                        D_8009B36C = llap;
                        llap = llap - 1;
                        D_8009B34C = (lu - lprev) + 0x7A120;
                        D_8009B360 = (D_8009B360 - lt) + 0x1F4000;
                        g_CourseIndex = llap;
                        D_8009B370 = llap;
                        D_8009B368 = D_8009E67C[llap & 3];
                        D_8009B334 = (llap < 4) ? -1 : 1;
                    }
                    }
                }
            }
            if ((D_801E436A & 0x2000) && (GameCanSelectNextCourse() != 0)) {
                t = D_8009B350;
                u = D_8009B34C;
                if (t < u ? (u - t <= 0x3D08F) : (t - u <= 0x3D08F)) {
                    if (D_8009B370 < 0) {
{
                            s32 llap;
                            s32 lprev;
                            s32 lu;
                            s32 lt;
                            s32 lbase;
                            func_8005D6EC(8);
                            lu = 0x7A120;
                            llap = g_CourseIndex;
                            lprev = D_8009B350;
                            lt = D_8009B34C;
                            lbase = D_8009B364;
                            D_8009B350 = 0xF4240;
                            D_8009B354 = 0;
                            D_8009B36C = llap;
                            llap = llap + 1;
                            lprev = lprev - lt;
                            lu = lu - lprev;
                            D_8009B360 = (D_8009B360 - lbase) + 0x1F4000;
                            g_CourseIndex = llap;
                            D_8009B370 = llap;
                            D_8009B34C = lu;
                            D_8009B368 = D_8009E67C[llap & 3];
                            D_8009B334 = (llap < 4) ? -1 : 1;
                        }
                    }
                }
            }
            if (g_PadEdge2 & 0x860) {
                sel = D_8019C7AC;
                if (sel == 0) {
                    func_8005D6EC(2);
                    GameMenuBusy = 1;
                    g_MenuOverlayPattern = 1;
                    D_8009B334 = -1;
                    D_8009B35C = 0x3D090;
                    D_8009B368 = 0;
                    D_8009B360 = (D_8009B360 - D_8009B364) + 0x1F4000;
                } else if (sel == 2) {
                    if (D_801E4DAC != 0) {
                        u16 hv;
                        func_8005D6EC(2);
                        hv = 0;
                        if (g_GrandPrixClass < 5) {
                            hv = D_8019CABC;
                        }
                        D_8019C764 = &D_80082604;
                        GameMenuBusy = -1;
                        D_8019CABC = hv;
                        g_UiScriptProgress2 = 0;
                        D_8009B2F0 = 1;
                    } else {
                        func_8005D6EC(3);
                        func_8005E8E0();
                        D_8009B30C = -1;
                        D_8009B334 = -1;
                        D_8009B35C = 0x3D090;
                        GameMenuBusy = sel;
                        D_8009B368 = 0;
                        D_8019CABC = g_CourseIndex >> 2;
                        D_8009B360 = (D_8009B360 - D_8009B364) + 0x1F4000;
                    }
                } else {
                    func_8005D6EC(2);
                    if (D_801E4DAC != 0) {
                        D_8019C764 = &D_800825A4;
                        GameMenuBusy = -2;
                        g_UiScriptProgress2 = 0;
                        D_8009B2F0 = g_GrandPrixClass;
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
            func_800487D8(&D_80082790, &g_UiScriptProgress2, 0);
            if (func_800487D8(D_8019C764, &g_UiScriptProgress2, 1) != 0) {
                if (g_PadEdge2 & 0x860) {
                    func_8005D6EC((D_8009B2F0 != 0) ? 2 : 3);
                    GameMenuBusy = -3;
                    D_8009B300 = 0x23;
                }
                pad = (u16 *)&g_PadEdge2;
                if (*pad & 0x90) {
                    func_8005D6EC(3);
                    GameMenuBusy = -4;
                }
                if (*pad & 0x8000) {
                    func_8005D6EC(1);
                    D_8009B2F0 = 1;
                }
                if (*pad & 0x2000) {
                    func_8005D6EC(1);
                    D_8009B2F0 = 0;
                }
                func_80048D64((D_8009B2F0 != 0) ? 0xB8 : 0xDA, 0x8C, 0x20, 0x20, 0);
                func_80046A2C(ot, 0xC0, 0x94, 0x10, 0x10, 0x9D, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                func_80046A2C(ot, 0xE3, 0x94, 0x10, 0x10, 0xAD, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                func_80048B88(0xB8, 0x8C, 0x20, 0x20, 0x95, 0x25, 0x1E, 0, 0, 0, (s32)&D_80011BA0);
                func_80048B88(0xDA, 0x8C, 0x20, 0x20, 0x1E, 0x4E, 0x95, 0, 0, 0, (s32)&D_80011BA0);
            }
        } else if (state == -2) {
            u16 *pad;
            if (func_800487D8(D_8019C764, &g_UiScriptProgress2, 1) != 0) {
                if (g_PadEdge2 & 0x860) {
                    func_8005D6EC(2);
                    if (D_8009B2F0 == g_GrandPrixClass) {
                        GameMenuBusy = 0;
                    } else {
                        GameMenuBusy = -5;
                        D_8009B310 = 0;
                        D_8009B300 = 0x23;
                        func_8005026C(0);
                    }
                }
                pad = (u16 *)&g_PadEdge2;
                if (*pad & 0x90) {
                    func_8005D6EC(3);
                    GameMenuBusy = 0;
                }
                if (*pad & 0x1000) {
                    func_8005D6EC(1);
                    D_8009B2F0 = (D_8009B2F0 != 0) ? D_8009B2F0 - 1 : D_801E4FAC->progression;
                }
                if (g_PadEdge2 & 0x4000) {
                    func_8005D6EC(1);
                    D_8009B2F0 = (D_8009B2F0 < D_801E4FAC->progression) ? D_8009B2F0 + 1 : 0;
                }
                func_80048D64(0xB8, D_8009B2F0 * 0x1E + 0x6C, 0x38, 0x20, 0);
                for (i = 0; i < D_801E4FAC->progression + 1; i++) {
                    func_80046A2C(ot, 0xC0, i * 0x1E + 0x74, 0x1A, 0x10, 0x60, 0xCC, 0, 0, 0, 0x244, 1, 1, 0x3B);
                    func_80046A2C(ot, 0xE0, i * 0x1E + 0x74, 8, 0x10, i * 8 + 8, 0x18, 0, 0, 0, 0x244, 1, 1, 0x3B);
                    func_80048B88(0xB8, i * 0x1E + 0x6C, 0x38, 0x20, 0x95, 0x25, 0x1E, 0, 0, 0, (s32)&D_80011BA0);
                }
            }
        } else if (state == -3) {
            cnt = D_8009B300;
            if (cnt <= 0) {
                func_800487D8(&D_800827FC, &g_UiScriptProgress2, -1);
                func_800487D8(&D_80082790, &g_UiScriptProgress2, 0);
                func_800487D8(D_8019C764, &g_UiScriptProgress2, 0);
                if (g_UiScriptProgress2 <= 0) {
                    func_8005E8E0();
                    GameMenuBusy = (D_8009B2F0 != 0) ? 4 : 2;
                    D_8009B30C = -1;
                    D_8009B35C = 0x3D090;
                    D_8009B368 = 0;
                    D_8009B360 = (D_8009B360 - D_8009B364) + 0x1F4000;
                }
            } else {
                D_8009B300 = cnt - 1;
                func_800487D8(&D_800827FC, &g_UiScriptProgress2, 0);
                func_800487D8(&D_80082790, &g_UiScriptProgress2, 0);
                func_800487D8(D_8019C764, &g_UiScriptProgress2, 1);
                func_80048D64((D_8009B2F0 != 0) ? 0xB8 : 0xDA, 0x8C, 0x20, 0x20, 1);
                func_80046A2C(ot, 0xC0, 0x94, 0x10, 0x10, 0x9D, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                func_80046A2C(ot, 0xE3, 0x94, 0x10, 0x10, 0xAD, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                func_80048B88(0xB8, 0x8C, 0x20, 0x20, 0x95, 0x25, 0x1E, 0, 0, 0, (s32)&D_80011BA0);
                func_80048B88(0xDA, 0x8C, 0x20, 0x20, 0x1E, 0x4E, 0x95, 0, 0, 0, (s32)&D_80011BA0);
            }
        } else if (state == -4) {
            func_800487D8(&D_800827FC, &g_UiScriptProgress2, -1);
            func_800487D8(&D_80082790, &g_UiScriptProgress2, 0);
            func_800487D8(D_8019C764, &g_UiScriptProgress2, 0);
            if (g_UiScriptProgress2 <= 0) {
                GameMenuBusy = 0;
            }
        } else if (state == -5) {
            cnt = D_8009B300;
            if (cnt <= 0) {
                if (D_8009B310 != 0) {
                    if (func_8005026C(-1) == 0) {
                        GameMenuBusy = 0;
                        g_UiScriptProgress2 = 0;
                    }
                } else {
                    if (func_8005026C(1) >= 0x19) {
                        D_8009B310 = 1;
                        g_GrandPrixClass = D_8009B2F0;
                        func_800212F0(D_8009B2F0);
                        D_8009B34C = 0x7A120;
                        D_8009B350 = 0x7A120;
                        D_8019C7AC = 0;
                        D_8009B370 = -1;
                        D_8009B360 = 0;
                        g_CourseIndex = g_CourseIndex & ~3;
                        D_8009B36C = g_CourseIndex;
                        D_8009B368 = D_8009E67C[0];
                    }
                    func_800487D8(D_8019C764, &g_UiScriptProgress2, 1);
                    func_80048D64(0xB8, D_8009B2F0 * 0x1E + 0x6C, 0x38, 0x20, 1);
                    for (i = 0; i < D_801E4FAC->progression + 1; i++) {
                        func_80046A2C(ot, 0xC0, i * 0x1E + 0x74, 0x1A, 0x10, 0x60, 0xCC, 0, 0, 0, 0x244, 1, 1, 0x3B);
                        func_80046A2C(ot, 0xE0, i * 0x1E + 0x74, 8, 0x10, i * 8 + 8, 0x18, 0, 0, 0, 0x244, 1, 1, 0x3B);
                        func_80048B88(0xB8, i * 0x1E + 0x6C, 0x38, 0x20, 0x95, 0x25, 0x1E, 0, 0, 0, (s32)&D_80011BA0);
                    }
                }
            } else {
                D_8009B300 = cnt - 1;
                func_800487D8(D_8019C764, &g_UiScriptProgress2, 1);
                func_80048D64(0xB8, D_8009B2F0 * 0x1E + 0x6C, 0x38, 0x20, 1);
                for (i = 0; i < D_801E4FAC->progression + 1; i++) {
                    func_80046A2C(ot, 0xC0, i * 0x1E + 0x74, 0x1A, 0x10, 0x60, 0xCC, 0, 0, 0, 0x244, 1, 1, 0x3B);
                    func_80046A2C(ot, 0xE0, i * 0x1E + 0x74, 8, 0x10, i * 8 + 8, 0x18, 0, 0, 0, 0x244, 1, 1, 0x3B);
                    func_80048B88(0xB8, i * 0x1E + 0x6C, 0x38, 0x20, 0x95, 0x25, 0x1E, 0, 0, 0, (s32)&D_80011BA0);
                }
            }
        }
        res = func_80053650();
        func_80049418(1, 1, res, GameCanSelectNextCourse());
        func_800489AC(g_UiScriptProgress, 2, D_8019C7AC);
        func_800487D8(hdr, &g_UiScriptProgress, 0);
        func_800487D8(&D_80082460, &g_UiScriptProgress, 1);
        func_8004CF30(7);
    } else {
        g_MenuHandlerIndex = -1;
        D_8009B344 = 1;
        res = func_80053650();
        func_80049418(-1, 1, res, GameCanSelectNextCourse());
        func_800487D8(hdr, &g_UiScriptProgress, -1);
        func_800487D8(&D_80082460, &g_UiScriptProgress, 0);
        func_800489AC(g_UiScriptProgress, 2, D_8019C7AC);
        func_8004CF30(-9);
        if (g_UiScriptProgress <= 0) {
            switch (GameMenuBusy) {
            case 1:
                if (D_8009B358 > 0x3D08F) {
                    D_8019C9F8 = 3;
                    g_MenuHandlerIndex = 4;
                    func_8004F3EC(0, 0);
                    func_80049418(0, 0, 0, 0);
                    D_8009B378 = -1;
                    D_8009B34C = 0;
                    D_8009B350 = 0;
                    D_8009B358 = 0x3D090;
                    D_8009B35C = 0;
                    D_8009B374 = D_801E40D4;
                    goto clear;
                }
                break;
            case 2:
                if ((D_8009B348 <= 0) && (D_8009B358 > 0x3D08F)) {
                    s32 raw;
                    s32 d;
                    s32 lapc;
                    s32 half;
                    raw = g_CourseIndex;
                    p = D_801E4FAC;
                    d = D_801E40D4;
                    g_SceneId = 2;
                    lapc = g_GrandPrixClass;
                    half = D_801E4DAC;
                    raw = raw & 3;
                    g_CourseIndex = raw;
                    p->state = raw;
                    p->pad4 = d;
                    p->lap = lapc;
                    if (half != 0) {
                        p->elapsedTime = D_8019C908;
                        goto clear;
                    }
                    goto setlast;
                }
                break;
            case 3:
                D_8019C9F8 = 2;
                g_MenuHandlerIndex = 2;
                goto clear;
            case 4:
                if ((D_8009B348 <= 0) && (D_8009B358 > 0x3D08F)) {
                    s32 raw;
                    s32 d;
                    s32 lapc;
                    s32 half;
                    g_SceneId = 0x18;
                    raw = g_CourseIndex & 3;
                    D_801E4FAC->state = (g_CourseIndex = raw);
                    d = D_801E40D4;
                    lapc = g_GrandPrixClass;
                    half = D_801E4DAC;
                    D_801E4FAC->pad4 = d;
                    D_801E4FAC->lap = lapc;
                    if (half != 0) {
                        D_801E4FAC->elapsedTime = D_8019C908;
                    } else {
                        p = D_801E4FAC;
                    setlast:
                        p->elapsedTime = (s16)D_8019CABC;
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
