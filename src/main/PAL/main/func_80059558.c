#include "common.h"
#include "game/car.h"
#include "game/menu.h"
#include "game/render.h"

extern u8 D_80011BA0;
extern u8 D_8007F460[];
extern u8 D_800820C4;
extern u8 D_80082460;
extern u8 D_80082790;
extern u8 D_800828B0;
extern u8 D_80082904;
extern u8 D_80082958;
extern u8 D_800829AC;
extern u8 D_80082A00;
extern s32 D_80082D7C[];
extern u8 D_8009B2F0;
extern s32 D_8009B300;
extern s32 D_8009B31C;
extern s32 D_8009B320;
extern s32 D_8009B32C;
extern s32 D_8009B330;
extern s32 D_8009B338;
extern s32 D_8009B344;
extern s32 D_8009B34C;
extern s32 D_8009B350;
extern s32 D_8009B374;
extern s32 D_8009B378;
extern u8 *D_8009E698;
extern s32 D_8019C908;
extern s32 D_8019C9F8;
extern s16 D_8019CA18;
extern u8 *D_8019CB00;
extern s32 D_8019CB0C;
extern s32 D_801E40D4;
extern s16 D_801E41A4;
extern s32 D_801E4294;
extern u16 D_801E436A;
extern u8 D_801E438D[];
extern s32 D_801E4B88;

void func_8001882C(s32 arg0);
void func_8001D530(void *arg0, s32 arg1);
void func_80046A2C(void *ot, s32 x0, s32 y0, s32 x1, s32 y1, s32 u0, s32 v0,
                   s32 r, s32 g, s32 b, s32 clut, s32 sh, s32 st, s32 flags);
s32 func_800487D8(u8 *commands, s32 *progress, s32 step);
void func_800489AC(s32 arg0, s32 arg1, s32 arg2);
void func_80048B88(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6,
                   s32 a7, s32 a8, s32 a9, void *a10);
void func_80048D64(s32 x0, s32 y0, s32 x1, s32 y1, s32 useFlash);
void func_80049418(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_8004CF00(void);
void func_8004F650(s32 arg0, s32 arg1, s32 arg2);
void func_8004FCE8(s32 arg0, s32 arg1, s32 arg2);
void func_80050400(s32 arg0, s32 arg1);
s32 func_80050FA8(s32 arg0);
void func_8005131C(void);
void func_80059320(void);
void func_8005D6EC(s32 arg0);

void GameUpdateCarShopScreen(void) asm("func_80059558");
void GameUpdateCarShopScreen(void) {
    void *ot;
    s32 value;
    s32 res;
    s32 sel;
    s32 t;
    s32 u;

    ot = *(void **)0x1F800004;
    D_8019CB0C = D_8009B338;
    func_80050400(D_8009B32C, D_8009B330);
    func_8004FCE8(D_8009B31C, D_8009B320, 0);
    func_8005131C();
    value = D_80082D7C[func_80050FA8(D_801E4B88)];
    if (GameMenuBusy == 0) {
        D_8009B320 = D_801E4B88;
        func_800487D8(D_8019CB00, &g_UiScriptProgress2, -1);
        func_800487D8(&D_80082790, &g_UiScriptProgress2, 0);
        func_80049418(1, 0, ~D_8019CA18 != 0, ~D_801E41A4 != 0);
        func_8004F650(1, D_8019C908, value);
        func_800489AC(g_UiScriptProgress, 1, D_801E4294);
        func_800487D8(&D_800820C4, &g_UiScriptProgress, 0);
        {
        s32 initial;

        initial = -1;
        res = func_800487D8(&D_80082460, &g_UiScriptProgress, 1);
        if ((res != 0) && (g_UiScriptProgress2 <= 0)) {
            g_MenuOverlayPattern = initial;
            if (g_PadEdge2 & 0x1000) {
                func_8005D6EC(1);
                D_801E4294 = (D_801E4294 > 0) ? D_801E4294 - 1 : 1;
            }
            if (g_PadEdge2 & 0x4000) {
                func_8005D6EC(1);
                D_801E4294 = (D_801E4294 <= 0) ? D_801E4294 + 1 : 0;
            }
            func_80059320();
            sel = D_801E4B88;
            if ((D_801E436A & 0x8000) && (D_8019CA18 != -1)) {
                t = D_8009B350;
                u = D_8009B34C;
                if (t < u ? (u - t <= 0x493DF) : (t - u <= 0x493DF)) {
                    if (D_8009B378 < 0) {
                        s32 lprev;

                        func_8005D6EC(8);
                        D_801E4B88 = (s32) D_8019CA18;
                        func_8001882C(D_8019CA18);
                        lprev = D_8009B350;
                        D_8009B374 = sel;
                        D_8009B350 = 0;
                        D_8009B330 = -1;
                        D_8009B378 = D_801E4B88;
                        D_8009B34C = (D_8009B34C - lprev) + 0x927C0;
                    }
                }
            }
            if ((D_801E436A & 0x2000) && (D_801E41A4 != -1)) {
                t = D_8009B350;
                u = D_8009B34C;
                if (t < u ? (u - t <= 0x493DF) : (t - u <= 0x493DF)) {
                    if (D_8009B378 < 0) {
                        s32 base;
                        s32 lprev;

                        func_8005D6EC(8);
                        D_801E4B88 = (s32) D_801E41A4;
                        func_8001882C(D_801E41A4);
                        base = 0x927C0;
                        lprev = D_8009B350;
                        D_8009B350 = 0x124F80;
                        D_8009B374 = sel;
                        D_8009B330 = -1;
                        D_8009B378 = D_801E4B88;
                        D_8009B34C = base - (lprev - D_8009B34C);
                    }
                }
            }
            if (D_8009E698[8] == 0) {
                D_8009B32C = 1;
            } else {
                D_8009B32C = -1;
            }
            t = D_8009B350;
            u = D_8009B34C;
            if (t < u ? (u - t <= 0x493DF) : (t - u <= 0x493DF)) {
                if (D_8009B378 < 0) {
                    if (g_PadEdge2 & 0x860) {
                        sel = D_801E4294;
                        if (sel == 1) {
                            if (D_801E40D4 != D_801E4B88) {
                                s32 base;
                                s32 current;
                                s32 selected;
                                s32 lu;
                                s32 lprev;

                                func_8001882C(D_801E40D4);
                                base = 0x927C0;
                                current = D_801E4B88;
                                selected = D_801E40D4;
                                lu = D_8009B34C;
                                lprev = D_8009B350;
                                D_8009B350 = 0;
                                D_8009B374 = current;
                                D_8009B378 = selected;
                                D_8009B34C = (lu - lprev) + base;
                            }
                            func_8005D6EC(3);
                            g_MenuOverlayPattern = 2;
                            GameMenuBusy = sel;
                            goto block_51;
                        }
                        if (sel != 0) {
                            return;
                        }
                        if (g_CarTable[D_801E4B88].enabled == 0) {
                            func_8005D6EC(2);
                            GameMenuBusy = -1;
                            g_UiScriptProgress2 = 0;
                            D_8009B2F0 = 0;
                            switch (D_801E4B88) {
                            case 0:
                            case 1:
                            case 2:
                            case 10:
                                D_8019CB00 = &D_80082958;
                                return;
                            case 3:
                                D_8019CB00 = &D_80082904;
                                return;
                            case 4:
                            case 5:
                            case 6:
                            case 11:
                                D_8019CB00 = &D_800829AC;
                                return;
                            case 7:
                            case 8:
                            case 9:
                            case 12:
                                D_8019CB00 = &D_80082A00;
                                return;
                            }
                        }
                        return;
                    } else if (g_PadEdge2 & 0x90) {
                        if (D_801E40D4 != D_801E4B88) {
                            s32 base;
                            s32 current;
                            s32 selected;
                            s32 lu;
                            s32 lprev;

                            func_8001882C(D_801E40D4);
                            base = 0x927C0;
                            current = D_801E4B88;
                            selected = D_801E40D4;
                            lu = D_8009B34C;
                            lprev = D_8009B350;
                            D_8009B350 = 0;
                            D_8009B374 = current;
                            D_8009B378 = selected;
                            D_8009B34C = (lu - lprev) + base;
                        }
                        func_8005D6EC(3);
                        GameMenuBusy = 1;
                        g_MenuOverlayPattern = 2;
block_51:
                        D_8009B32C = -1;
                        D_8009B330 = -1;
                    }
                }
            }
        }
        }
    } else {
        if (GameMenuBusy < 0) {
            if ((u32) (GameMenuBusy + 2) < 2U) {
                u16 *pad;

                func_800487D8(D_8019CB00, &g_UiScriptProgress2, 0);
                if (func_800487D8(&D_80082790, &g_UiScriptProgress2, 1) != 0) {
                    if (GameMenuBusy == -1) {
                        if (g_PadEdge2 & 0x860) {
                            if (D_8009B2F0 != 0) {
                                if (D_8019C908 >= value) {
                                    func_8005D6EC(2);
                                    GameMenuBusy = -3;
                                    D_8009B300 = 0x23;
                                } else {
                                    func_8005D6EC(5);
                                    D_8019CB00 = &D_800828B0;
                                    GameMenuBusy = -2;
                                }
                            } else {
                                func_8005D6EC(3);
                                GameMenuBusy = 0;
                            }
                        }
                        pad = &g_PadEdge2;
                        if (*pad & 0x90) {
                            func_8005D6EC(3);
                            GameMenuBusy = 0;
                        }
                        if ((*pad & 0x8000) && (D_8009B2F0 == 0)) {
                            func_8005D6EC(1);
                            D_8009B2F0 = 1;
                        }
                        if (g_PadEdge2 & 0x2000) {
                            if (D_8009B2F0 != 0) {
                                func_8005D6EC(1);
                                D_8009B2F0 = 0;
                            }
                        }
                    } else {
                        if (g_PadEdge2 & 0x860) {
                            GameMenuBusy = 0;
                        }
                        if (g_PadEdge2 & 0x90) {
                            GameMenuBusy = 0;
                        }
                    }
                    func_80048D64((D_8009B2F0 != 0) ? 0xB8 : 0xDA, 0x44, 0x20, 0x20, 0);
                    func_80046A2C(ot, 0xC0, 0x4C, 0x10, 0x10, 0x9D, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                    func_80046A2C(ot, 0xE3, 0x4C, 0x10, 0x10, 0xAD, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                    func_80048B88(0xB8, 0x44, 0x20, 0x20, 0x95, 0x25, 0x1E, 0, 0, 0, &D_80011BA0);
                    func_80048B88(0xDA, 0x44, 0x20, 0x20, 0x1E, 0x8E, 0x95, 0, 0, 0, &D_80011BA0);
                }
            } else if (GameMenuBusy == -3) {
                if (D_8009B300 <= 0) {
                    func_800487D8(D_8019CB00, &g_UiScriptProgress2, -1);
                    func_800487D8(&D_80082790, &g_UiScriptProgress2, 0);
                    if (g_UiScriptProgress2 <= 0) {
                        g_CarTable[D_801E4B88].enabled = 1;
                        D_801E438D[D_801E4B88 * 8] = 1;
                        GameMenuBusy = 2;
                        D_8009B32C = -1;
                        D_801E40D4 = D_801E4B88;
                    }
                } else {
                    D_8009B300 -= 1;
                    func_800487D8(D_8019CB00, &g_UiScriptProgress2, 0);
                    func_800487D8(&D_80082790, &g_UiScriptProgress2, 1);
                    func_80048D64((D_8009B2F0 != 0) ? 0xB8 : 0xDA, 0x44, 0x20, 0x20, 1);
                    func_80046A2C(ot, 0xC0, 0x4C, 0x10, 0x10, 0x9D, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                    func_80046A2C(ot, 0xE3, 0x4C, 0x10, 0x10, 0xAD, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                    func_80048B88(0xB8, 0x44, 0x20, 0x20, 0x95, 0x25, 0x1E, 0, 0, 0, &D_80011BA0);
                    func_80048B88(0xDA, 0x44, 0x20, 0x20, 0x1E, 0x8E, 0x95, 0, 0, 0, &D_80011BA0);
                }
            }
            func_80049418(1, 0, ~D_8019CA18 != 0, ~D_801E41A4 != 0);
            func_8004F650(1, D_8019C908, value);
            func_800489AC(g_UiScriptProgress, 1, D_801E4294);
            func_800487D8(&D_800820C4, &g_UiScriptProgress, 0);
            func_800487D8(&D_80082460, &g_UiScriptProgress, 1);
            return;
        }
        g_MenuHandlerIndex = -1;
        D_8009B344 = 0xB;
        func_80049418(-1, 0, ~D_8019CA18 != 0, ~D_801E41A4 != 0);
        func_8004F650(-1, D_8019C908, value);
        func_800487D8(&D_800820C4, &g_UiScriptProgress, -1);
        func_800487D8(&D_80082460, &g_UiScriptProgress, 0);
        func_800489AC(g_UiScriptProgress, 1, D_801E4294);
        if (g_UiScriptProgress <= 0) {
            if (GameMenuBusy == 2) {
                D_8019C908 -= value;
            }
            D_8019C9F8 = 4;
            g_MenuHandlerIndex = 4;
            g_UiScriptProgress = 0;
            GameMenuBusy = 0;
            D_801E4294 = 0;
            func_8001D530(&D_8007F460, GameMenuStackDepth);
            func_8004CF00();
        }
    }
}
