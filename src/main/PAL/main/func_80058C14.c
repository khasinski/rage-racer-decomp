#include "common.h"
#include "game/car.h"
#include "game/menu.h"
#include "game/render.h"

extern s32 D_8009B338;
extern s32 D_8019CB0C;
extern s32 D_80082EB4;
extern u8 D_80082010;
extern u8 D_80082460;
extern s32 D_801F17A0;
extern GameCarEntry D_801E4388[];

void func_8005131C(void);
s32 func_8004F048(void *, s32, s32);
void func_80049418(s32, s32, s32, s32);
void func_800489AC(s32, s32, s32);
s32 func_800487D8(void *, void *, s32);
void func_8005D6EC(s32);
void func_8001D8C4(s32);
void func_8001DA74(s32);

void GameUpdatePaintColorScreen(void) asm("func_80058C14");
void GameUpdatePaintColorScreen(void) {
    D_8019CB0C = D_8009B338;
    func_8005131C();

    if (GameMenuBusy == 0) {
        func_8004F048(&g_UiScriptProgress2, -1, D_80082EB4);
        func_80049418(-1, 0, 1, 1);
        func_800489AC(g_UiScriptProgress, 2, D_801F17A0);
        func_800487D8(&D_80082010, &g_UiScriptProgress, 0);
        if (func_800487D8(&D_80082460, &g_UiScriptProgress, 1) == 0) {
            return;
        }
        if (g_UiScriptProgress2 > 0) {
            return;
        }
        g_MenuOverlayPattern = -1;
        if (g_PadEdge2 & 0x1000) {
            func_8005D6EC(1);
            D_801F17A0 = D_801F17A0 > 0 ? D_801F17A0 - 1 : 2;
        }
        if (g_PadEdge2 & 0x4000) {
            func_8005D6EC(1);
            D_801F17A0 = D_801F17A0 < 2 ? D_801F17A0 + 1 : 0;
        }
        {
            u16 f = g_PadEdge2;
            if (f & 0x860) {
                s32 sel = D_801F17A0;
                s32 val;
                if (sel == 0) {
                    func_8005D6EC(2);
                    val = g_CarTable[g_PlayerCarIndex].shapeIndex;
                    GameMenuBusy = -1;
                    g_UiScriptProgress2 = 0;
                    D_80082EB4 = val;
                } else if (sel == 1) {
                    func_8005D6EC(2);
                    val = g_CarTable[g_PlayerCarIndex].textureIndex;
                    GameMenuBusy = -2;
                    g_UiScriptProgress2 = 0;
                    D_80082EB4 = val;
                } else if (sel == 2) {
                    func_8005D6EC(3);
                    GameMenuBusy = 1;
                    g_MenuOverlayPattern = 2;
                    g_MenuViewOffsetTarget = 0x3D090;
                }
            } else if (f & 0x90) {
                func_8005D6EC(3);
                GameMenuBusy = 3;
                g_MenuOverlayPattern = 2;
                g_MenuViewOffsetTarget = 0x3D090;
            }
        }
        return;
    }

    if (GameMenuBusy < 0) {
        if (func_8004F048(&g_UiScriptProgress2, 1, D_80082EB4) != 0) {
            if (g_PadEdge & 0x8000) {
                func_8005D6EC(1);
                D_80082EB4 = D_80082EB4 > 0 ? D_80082EB4 - 1 : 0x11;
            }
            if (g_PadEdge & 0x2000) {
                func_8005D6EC(1);
                D_80082EB4 = D_80082EB4 < 17 ? D_80082EB4 + 1 : 0;
            }
            if (GameMenuBusy == -1) {
                u16 *btn = &g_PadEdge2;
                if (*btn & 0x860) {
                    func_8005D6EC(2);
                    g_CarTable[g_PlayerCarIndex].shapeIndex = D_80082EB4;
                    D_801E4388[g_PlayerCarIndex].shapeIndex = D_80082EB4;
                    D_801E4388[g_PlayerCarIndex].textureIndex = g_CarTable[g_PlayerCarIndex].textureIndex;
                    GameMenuBusy = 0;
                }
                if (*btn & 0x90) {
                    func_8005D6EC(3);
                    D_80082EB4 = g_CarTable[g_PlayerCarIndex].shapeIndex;
                    GameMenuBusy = 0;
                }
                func_8001D8C4(D_80082EB4);
            } else {
                u16 *btn = &g_PadEdge2;
                if (*btn & 0x860) {
                    func_8005D6EC(2);
                    g_CarTable[g_PlayerCarIndex].textureIndex = D_80082EB4;
                    D_801E4388[g_PlayerCarIndex].shapeIndex = g_CarTable[g_PlayerCarIndex].shapeIndex;
                    D_801E4388[g_PlayerCarIndex].textureIndex = D_80082EB4;
                    GameMenuBusy = 0;
                }
                if (*btn & 0x90) {
                    func_8005D6EC(3);
                    D_80082EB4 = g_CarTable[g_PlayerCarIndex].textureIndex;
                    GameMenuBusy = 0;
                }
                func_8001DA74(D_80082EB4);
            }
        }

        func_80049418(1, 0, 1, 1);
        func_800489AC(g_UiScriptProgress, 2, D_801F17A0);
        func_800487D8(&D_80082010, &g_UiScriptProgress, 0);
        func_800487D8(&D_80082460, &g_UiScriptProgress, 1);
        return;
    }

    g_MenuHandlerIndex = -1;
    g_MenuHandlerIndex2 = 10;
    func_800487D8(&D_80082010, &g_UiScriptProgress, -1);
    func_800487D8(&D_80082460, &g_UiScriptProgress, 0);
    func_800489AC(g_UiScriptProgress, 2, D_801F17A0);
    if (g_UiScriptProgress <= 0) {
        g_MenuScreen = 6;
        g_MenuHandlerIndex = 6;
        D_801F17A0 = 0;
        g_UiScriptProgress = 0;
        GameMenuBusy = 0;
    }
}
