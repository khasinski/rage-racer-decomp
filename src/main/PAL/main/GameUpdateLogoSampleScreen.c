#include "common.h"
#include "game/menu.h"
#include "game/render.h"

extern s32 D_80082EA4;
extern s32 D_80082EA8;
extern s32 D_80082EB0;
extern s32 D_8019CB0C;
extern s32 D_8019C770;
extern void *D_8009F0B0;
extern u8 D_80082634;
extern u8 D_80082664;
extern u8 D_80081CA4;
extern u8 D_80082460;

extern void func_8001D338(s32, s32);
extern void func_8004A248(s32, s32);
extern void func_8004B8B4(s32, s32);
extern void func_8004E368(s32, s32);
extern s32 func_800487D8(void *, void *, s32);
extern void func_800489AC(s32, s32, s32);
extern void func_8005D6EC(s32);

void GameUpdateLogoSampleScreen(void) asm("func_800580C8");
void GameUpdateLogoSampleScreen(void) {
    s32 v0;
    s32 t;
    s32 pl;

    D_8019CB0C = 0;
    func_8001D338(D_80082EA4, D_80082EA8);
    func_8004A248(1, 0);
    v0 = GameMenuBusy;
    if (v0 == 0) {
        func_8004B8B4(-10, 0);
        func_8004E368(-1, D_80082EB0 + 1);
        func_800487D8(D_8009F0B0, &g_UiScriptProgress2, -1);
        func_800489AC(g_UiScriptProgress, 2, D_8019C770);
        func_800487D8(&D_80081CA4, &g_UiScriptProgress, 0);
        if (func_800487D8(&D_80082460, &g_UiScriptProgress, 1) == 0) return;
        if (g_UiScriptProgress2 > 0) return;
        g_MenuOverlayPattern = -1;
        if (g_PadEdge2 & 0x1000) {
            s32 n, c;
            func_8005D6EC(1);
            c = D_8019C770;
            n = 2;
            if (c > 0) n = c - 1;
            D_8019C770 = n;
        }
        if (g_PadEdge2 & 0x4000) {
            s32 n, c;
            func_8005D6EC(1);
            c = D_8019C770;
            n = 0;
            if (c < 2) n = c + 1;
            D_8019C770 = n;
        }
        if (g_PadEdge2 & 0x860) {
            pl = D_8019C770;
            if (pl == 0) {
                func_8005D6EC(2);
                GameMenuBusy = -1;
                g_UiScriptProgress2 = 0;
                D_8009F0B0 = &D_80082634;
                D_80082EB0 = D_80082EA4;
            } else if (pl == 1) {
                func_8005D6EC(2);
                GameMenuBusy = -2;
                g_UiScriptProgress2 = 0;
                D_8009F0B0 = &D_80082664;
                D_80082EB0 = D_80082EA8;
            } else if (pl == 2) {
                func_8005D6EC(3);
                GameMenuBusy = 1;
                g_MenuOverlayPattern = 2;
            }
        } else if (g_PadEdge2 & 0x90) {
            func_8005D6EC(3);
            GameMenuBusy = 1;
            g_MenuOverlayPattern = 2;
        }
        return;
    }

    if (v0 < 0) {
        func_8004B8B4(10, 0);
        if (GameMenuBusy == -1) {
            if (func_800487D8(D_8009F0B0, &g_UiScriptProgress2, 1) != 0) {
                u16 *p = &g_PadEdge2;
                if (*p & 0x860) {
                    func_8005D6EC(2);
                    GameMenuBusy = 0;
                    D_80082EB0 = D_80082EA4;
                }
                if (*p & 0x90) {
                    func_8005D6EC(3);
                    GameMenuBusy = 0;
                    D_80082EA4 = D_80082EB0;
                }
                if (*p & 0x8000) {
                    s32 n, c;
                    func_8005D6EC(1);
                    c = D_80082EA4;
                    n = 0x13;
                    if (c > 0) n = c - 1;
                    D_80082EA4 = n;
                }
                if (g_PadEdge2 & 0x2000) {
                    s32 n, c;
                    func_8005D6EC(1);
                    c = D_80082EA4;
                    n = 0;
                    if (c < 19) n = c + 1;
                    D_80082EA4 = n;
                }
            }
            t = D_80082EA4;
        } else {
            if (func_800487D8(D_8009F0B0, &g_UiScriptProgress2, 1) != 0) {
                u16 *p = &g_PadEdge2;
                if (*p & 0x860) {
                    func_8005D6EC(2);
                    GameMenuBusy = 0;
                    D_80082EB0 = D_80082EA8;
                }
                if (*p & 0x90) {
                    func_8005D6EC(3);
                    GameMenuBusy = 0;
                    D_80082EA8 = D_80082EB0;
                }
                if (*p & 0x8000) {
                    s32 n, c;
                    func_8005D6EC(1);
                    c = D_80082EA8;
                    n = 0x13;
                    if (c > 0) n = c - 1;
                    D_80082EA8 = n;
                }
                if (g_PadEdge2 & 0x2000) {
                    s32 n, c;
                    func_8005D6EC(1);
                    c = D_80082EA8;
                    n = 0;
                    if (c < 19) n = c + 1;
                    D_80082EA8 = n;
                }
            }
            t = D_80082EA8;
        }
        func_8004E368(1, t + 1);
        func_800489AC(g_UiScriptProgress, 2, D_8019C770);
        func_800487D8(&D_80081CA4, &g_UiScriptProgress, 0);
        func_800487D8(&D_80082460, &g_UiScriptProgress, 1);
        return;
    }

    g_MenuHandlerIndex = -1;
    g_MenuHandlerIndex2 = 8;
    func_8004E368(-1, 0);
    func_800487D8(&D_80081CA4, &g_UiScriptProgress, -1);
    func_800487D8(&D_80082460, &g_UiScriptProgress, 0);
    func_800489AC(g_UiScriptProgress, 2, D_8019C770);
    if (g_UiScriptProgress <= 0) {
        g_MenuScreen = 7;
        g_MenuHandlerIndex = 7;
        D_8019C770 = 0;
        g_UiScriptProgress = 0;
        GameMenuBusy = 0;
    }
}
