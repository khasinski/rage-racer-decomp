#include "common.h"
#include "game/menu.h"
#include "game/render.h"
#include "game/car.h"

extern s32 D_8009B338;
extern s32 D_8019CB0C;
extern s32 D_8009B37C;
extern s32 D_8019C758;

extern u8 D_800828EC;
extern u8 g_UiChromeScript2 asm("D_80082790");
extern u8 D_80081B54;
extern u8 g_UiChromeScript asm("D_80082460");

void func_8005131C(void);
s32 func_800487D8(void *a, void *b, s32 c);
void func_800489AC(s32 a, s32 b, s32 c);
void func_8005D6EC(s32 a);
void func_8004B8B4(s32 a, s32 b);
void func_8004A248(s32 a, s32 b);
s32 func_8004E724(s32 a, s32 b);

void GameUpdateDesignModeScreen(void) asm("func_80057198");
void GameUpdateDesignModeScreen(void) {
    s32 sel;
    u16 edge;

    D_8019CB0C = D_8009B338;
    func_8005131C();
    if (GameMenuBusy == 0) {
        func_800487D8(&D_800828EC, &g_UiScriptProgress2, -1);
        func_800487D8(&g_UiChromeScript2, &g_UiScriptProgress2, 0);
        func_800489AC(g_UiScriptProgress, 3, D_8019C758);
        func_800487D8(&D_80081B54, &g_UiScriptProgress, 0);
        if (func_800487D8(&g_UiChromeScript, &g_UiScriptProgress, 1) != 0) {
            g_MenuOverlayPattern = -1;
            if (g_PadEdge2 & 0x1000) {
                func_8005D6EC(1);
                D_8019C758 = (D_8019C758 > 0) ? D_8019C758 - 1 : 3;
            }
            if (g_PadEdge2 & 0x4000) {
                func_8005D6EC(1);
                D_8019C758 = (D_8019C758 < 3) ? D_8019C758 + 1 : 0;
            }
            if (g_PadEdge2 & 0x860) {
                sel = D_8019C758;
                if (sel == 0) {
                    func_8005D6EC(2);
                    func_8004B8B4(-256, -256);
                    GameMenuBusy = 1;
                    g_MenuOverlayPattern = 1;
                } else if (sel == 1) {
                    func_8005D6EC(2);
                    GameMenuBusy = 2;
                    g_MenuOverlayPattern = sel;
                } else if (sel == 2) {
                    if (g_PlayerCarIndex < 10) {
                        GameMenuBusy = 3;
                        g_MenuOverlayPattern = 1;
                        func_8005D6EC(2);
                    } else {
                        GameMenuBusy = -1;
                        g_UiScriptProgress2 = 0;
                        func_8005D6EC(5);
                    }
                } else if (sel == 3) {
                    func_8005D6EC(3);
                    GameMenuBusy = 4;
                    g_MenuOverlayPattern = 2;
                }
            } else if (g_PadEdge2 & 0x90) {
                func_8005D6EC(3);
                GameMenuBusy = 4;
                g_MenuOverlayPattern = 2;
            }
        }
    } else if (GameMenuBusy < 0) {
        func_800487D8(&D_800828EC, &g_UiScriptProgress2, 0);
        if (func_800487D8(&g_UiChromeScript2, &g_UiScriptProgress2, 1) != 0) {
            edge = g_PadEdge2;
            if (edge & 0x860) GameMenuBusy = 0;
            if (edge & 0x90) GameMenuBusy = 0;
        }
        func_800489AC(g_UiScriptProgress, 3, D_8019C758);
        func_800487D8(&D_80081B54, &g_UiScriptProgress, 0);
        func_800487D8(&g_UiChromeScript, &g_UiScriptProgress, 1);
    } else {
        g_MenuHandlerIndex = -1;
        g_MenuHandlerIndex2 = 6;
        func_800487D8(&D_80081B54, &g_UiScriptProgress, -1);
        func_800487D8(&g_UiChromeScript, &g_UiScriptProgress, 0);
        func_800489AC(g_UiScriptProgress, 3, D_8019C758);
        if (g_UiScriptProgress <= 0) {
            switch (GameMenuBusy) {
            case 1:
                g_MenuScreen = 7;
                g_MenuHandlerIndex = 7;
                func_8004A248(0, 0);
                break;
            case 2:
                g_MenuScreen = 9;
                g_MenuHandlerIndex = 9;
                func_8004E724(0, 0);
                g_MenuViewOffset = 0x3D090;
                g_MenuViewOffsetTarget = 0;
                g_MenuViewAngleTarget = 0;
                g_MenuViewAngle = 0;
                GameMenuCursor = (g_TeamNameLength >= 6) ? 0x2B : 0;
                D_8009B37C = GameMenuCursor;
                break;
            case 3:
                g_MenuScreen = 10;
                g_MenuHandlerIndex = 10;
                g_UiScriptProgress2 = 0;
                g_MenuViewOffset = 0x3D090;
                g_MenuViewOffsetTarget = 0;
                break;
            case 4:
                g_MenuScreen = 5;
                g_MenuHandlerIndex = 5;
                D_8019C758 = 0;
                g_MenuViewOffset = 0x3D090;
                g_MenuViewOffsetTarget = 0;
                break;
            }
            g_UiScriptProgress = 0;
            GameMenuBusy = 0;
        }
    }
}
