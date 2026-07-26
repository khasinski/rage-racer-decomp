#include "common.h"
#include "game/car.h"
#include "game/menu.h"
#include "game/render.h"

extern u8 g_MenuBlankCaption asm("D_80011BA0");
extern u8 D_80082130;
extern u8 g_UiChromeScript asm("D_80082460");
extern u8 g_UiChromeScript2 asm("D_80082790");
extern u8 D_80082898;
extern u8 D_80082A54;
extern s32 D_80082D80[];
extern u8 g_MenuSubCursor asm("D_8009B2F0");
extern s32 g_MenuConfirmTimer asm("D_8009B300");
extern s32 D_8009B31C;
extern s32 g_MenuPlateCarIndex asm("D_8009B320");
extern s32 D_8009B338;
extern s32 D_8009B374;
extern s32 D_8009B378;
extern s32 D_8019C908;
extern s32 D_8019CB0C;
extern u8 *D_801E4188;
extern s32 D_801E4290;
extern GameCarEntry D_801E4388[];

void func_800189E4(s32 arg0);
void func_80046A2C(void *ot, s32 x0, s32 y0, s32 x1, s32 y1, s32 u0, s32 v0,
                   s32 r, s32 g, s32 b, s32 clut, s32 sh, s32 st, s32 flags);
s32 func_800487D8(u8 *commands, s32 *progress, s32 step);
void func_800489AC(s32 arg0, s32 arg1, s32 arg2);
void func_80048B88(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6,
                   s32 a7, s32 a8, s32 a9, void *a10);
void func_80048D64(s32 x0, s32 y0, s32 x1, s32 y1, s32 useFlash);
void func_8004F99C(s32 arg0, s32 arg1, s32 arg2);
void func_8004FCE8(s32 arg0, s32 arg1, s32 arg2);
s32 func_80050FA8(s32 arg0);
void func_8005131C(void);
void GamePlaySoundCue(s32 cue) asm("func_8005D6EC");

void GameUpdateEngineerShopScreen(void) asm("func_8005A3A4");
void GameUpdateEngineerShopScreen(void) {
    void *ot;
    s32 value;
    s32 res;
    s32 sel;

    ot = *(void **)0x1F800004;
    D_8019CB0C = D_8009B338;
    func_8004FCE8(D_8009B31C, g_MenuPlateCarIndex, 0);
    func_8005131C();
    g_MenuPlateCarIndex = g_PlayerCarIndex;
    value = D_80082D80[func_80050FA8(g_PlayerCarIndex)];
    if (GameMenuBusy == 0) {
        func_800487D8(D_801E4188, &g_UiScriptProgress2, -1);
        func_800487D8(&g_UiChromeScript2, &g_UiScriptProgress2, 0);
        func_8004F99C(1, D_8019C908, value);
        func_800489AC(g_UiScriptProgress, 1, D_801E4290);
        func_800487D8(&D_80082130, &g_UiScriptProgress, 0);
        res = func_800487D8(&g_UiChromeScript, &g_UiScriptProgress, 1);
        if ((res != 0) && (g_UiScriptProgress2 <= 0)) {
            g_MenuOverlayPattern = -1;
            if (g_PadEdge2 & 0x1000) {
                GamePlaySoundCue(1);
                D_801E4290 = (D_801E4290 > 0) ? D_801E4290 - 1 : 1;
            }
            if (g_PadEdge2 & 0x4000) {
                GamePlaySoundCue(1);
                D_801E4290 = (D_801E4290 <= 0) ? D_801E4290 + 1 : 0;
            }
            if (g_PadEdge2 & 0x860) {
                sel = D_801E4290;
                if (sel == 0) {
                    if (D_8019C908 >= value) {
                        GamePlaySoundCue(2);
                        D_801E4188 = &D_80082A54;
                        GameMenuBusy = -1;
                        g_UiScriptProgress2 = 0;
                        g_MenuSubCursor = 0;
                    } else {
                        GamePlaySoundCue(5);
                        D_801E4188 = &D_80082898;
                        GameMenuBusy = -3;
                        g_UiScriptProgress2 = 0;
                    }
                } else if (sel == 1) {
                    GamePlaySoundCue(3);
                    GameMenuBusy = sel;
                    g_MenuOverlayPattern = 2;
                }
            } else if (g_PadEdge2 & 0x90) {
                GamePlaySoundCue(3);
                GameMenuBusy = 1;
                g_MenuOverlayPattern = 2;
            }
        }
    } else {
        if (GameMenuBusy < 0) {
            if (GameMenuBusy == -1) {
                u16 *pad;

                func_800487D8(D_801E4188, &g_UiScriptProgress2, 0);
                if (func_800487D8(&g_UiChromeScript2, &g_UiScriptProgress2, 1) != 0) {
                    if (g_PadEdge2 & 0x860) {
                        if (g_MenuSubCursor != 0) {
                            GamePlaySoundCue(2);
                            GameMenuBusy = -2;
                            g_MenuConfirmTimer = 0x23;
                            func_800189E4(g_PlayerCarIndex);
                        } else {
                            GamePlaySoundCue(3);
                            GameMenuBusy = 0;
                        }
                    }
                    pad = &g_PadEdge2;
                    if (*pad & 0x90) {
                        GamePlaySoundCue(3);
                        GameMenuBusy = 0;
                    }
                    if ((*pad & 0x8000) && (g_MenuSubCursor == 0)) {
                        GamePlaySoundCue(1);
                        g_MenuSubCursor = 1;
                    }
                    if (g_PadEdge2 & 0x2000) {
                        if (g_MenuSubCursor != 0) {
                            GamePlaySoundCue(1);
                            g_MenuSubCursor = 0;
                        }
                    }
                    func_80048D64((g_MenuSubCursor != 0) ? 0xB8 : 0xDA, 0x44, 0x20, 0x20, 0);
                    func_80046A2C(ot, 0xC0, 0x4C, 0x10, 0x10, 0x9D, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                    func_80046A2C(ot, 0xE3, 0x4C, 0x10, 0x10, 0xAD, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                    func_80048B88(0xB8, 0x44, 0x20, 0x20, 0x95, 0x25, 0x1E, 0, 0, 0, &g_MenuBlankCaption);
                    func_80048B88(0xDA, 0x44, 0x20, 0x20, 0x1E, 0x8E, 0x95, 0, 0, 0, &g_MenuBlankCaption);
                }
            } else if (GameMenuBusy == -2) {
                if (g_MenuConfirmTimer <= 0) {
                    func_800487D8(D_801E4188, &g_UiScriptProgress2, -1);
                    func_800487D8(&g_UiChromeScript2, &g_UiScriptProgress2, 0);
                    if (g_UiScriptProgress2 <= 0) {
                        g_MenuViewAngle = 0x927C0;
                        g_MenuViewAngleTarget = 0;
                        GameMenuBusy = 2;
                        g_MenuOverlayPattern = 2;
                        D_8009B374 = g_PlayerCarIndex;
                        D_8009B378 = g_PlayerCarIndex;
                    }
                } else {
                    g_MenuConfirmTimer -= 1;
                    func_800487D8(D_801E4188, &g_UiScriptProgress2, 0);
                    func_800487D8(&g_UiChromeScript2, &g_UiScriptProgress2, 1);
                    func_80048D64((g_MenuSubCursor != 0) ? 0xB8 : 0xDA, 0x44, 0x20, 0x20, 1);
                    func_80046A2C(ot, 0xC0, 0x4C, 0x10, 0x10, 0x9D, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                    func_80046A2C(ot, 0xE3, 0x4C, 0x10, 0x10, 0xAD, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                    func_80048B88(0xB8, 0x44, 0x20, 0x20, 0x95, 0x25, 0x1E, 0, 0, 0, &g_MenuBlankCaption);
                    func_80048B88(0xDA, 0x44, 0x20, 0x20, 0x1E, 0x8E, 0x95, 0, 0, 0, &g_MenuBlankCaption);
                }
            } else {
                func_800487D8(D_801E4188, &g_UiScriptProgress2, 0);
                if (func_800487D8(&g_UiChromeScript2, &g_UiScriptProgress2, 1) != 0) {
                    if (g_PadEdge2 & 0x860) {
                        GameMenuBusy = 0;
                    }
                    if (g_PadEdge2 & 0x90) {
                        GameMenuBusy = 0;
                    }
                }
            }
            func_8004F99C(1, D_8019C908, value);
            func_800489AC(g_UiScriptProgress, 1, D_801E4290);
            func_800487D8(&D_80082130, &g_UiScriptProgress, 0);
            func_800487D8(&g_UiChromeScript, &g_UiScriptProgress, 1);
            return;
        }
        g_MenuHandlerIndex = -1;
        g_MenuHandlerIndex2 = 0xC;
        func_8004F99C(-1, D_8019C908, value);
        func_800487D8(&D_80082130, &g_UiScriptProgress, -1);
        func_800487D8(&g_UiChromeScript, &g_UiScriptProgress, 0);
        func_800489AC(g_UiScriptProgress, 1, D_801E4290);
        if (g_UiScriptProgress <= 0) {
            if (GameMenuBusy == 2) {
                g_CarTable[g_PlayerCarIndex].modelVariant++;
                if (g_CarTable[g_PlayerCarIndex].modelVariant > D_801E4388[g_PlayerCarIndex].modelVariant) {
                    D_801E4388[g_PlayerCarIndex].modelVariant = g_CarTable[g_PlayerCarIndex].modelVariant;
                }
                D_8019C908 -= value;
            }
            g_MenuScreen = 4;
            g_MenuHandlerIndex = 4;
            g_UiScriptProgress = 0;
            GameMenuBusy = 0;
            D_801E4290 = 0;
        }
    }
}
