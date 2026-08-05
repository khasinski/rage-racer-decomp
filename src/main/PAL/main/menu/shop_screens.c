#include "common.h"
#include "game/car.h"
#include "game/menu.h"
#include "game/render.h"
#include "game/audio.h"

u32 DrawEngineerShopScreen(s32 arg0);

extern u8 *g_CarModelAsset;

extern u8 D_800820C4;
extern u8 D_800828B0;
extern u8 D_80082904;
extern u8 D_80082958;
extern u8 D_800829AC;
extern u8 D_80082A00;
extern s32 g_CarPriceTable[];
extern u8 *D_8019CB00;
extern s32 D_801E4294;
extern u8 D_801E438D[];

void RequestCarModel(s32 arg0);
void UploadTeamNameTexture(void *arg0, s32 arg1);
void GameDrawSpriteWide(void *ot, s32 x0, s32 y0, s32 x1, s32 y1, s32 u0, s32 v0,
                   s32 r, s32 g, s32 b, s32 clut, s32 sh, s32 st, s32 flags) asm("DrawSprite");
void DrawFadingMenuSprites(s32 arg0, s32 arg1, s32 arg2);
void GameDrawMenuButtonWide(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6,
                   s32 a7, s32 a8, s32 a9, void *a10) asm("GameDrawMenuButton");
void DrawMenuCursorBox(s32 x0, s32 y0, s32 x1, s32 y1, s32 useFlash);
void DrawBrowseArrows(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void DrawCarShopPricePanel(s32 arg0, s32 arg1, s32 arg2);
void UpdateCarListCursor(void);

void UpdateCarShopScreen(void) {
    void *ot;
    s32 value;
    s32 res;
    s32 sel;
    s32 t;
    s32 u;

    ot = *(void **)0x1F800004;
    g_MenuAltLayout = g_MenuAltLayoutSetting;
    DrawMenuAltPanel(D_8009B32C, D_8009B330);
    DrawCarNamePlate(g_CarNamePlateStep, g_MenuPlateCarIndex, 0);
    DrawMenuCarView();
    value = g_CarPriceTable[GetOwnedCarAssetIndex(g_CarListCursor)];
    if (GameMenuBusy == 0) {
        g_MenuPlateCarIndex = g_CarListCursor;
        RunTimedDrawScript(D_8019CB00, &g_UiScriptProgress2, -1);
        RunTimedDrawScript(&g_UiChromeScript2, &g_UiScriptProgress2, 0);
        DrawBrowseArrows(1, 0, ~g_PrevOwnedCarIndex != 0, ~g_NextOwnedCarIndex != 0);
        DrawCarShopPricePanel(1, g_PlayerMoney, value);
        DrawFadingMenuSprites(g_UiScriptProgress, 1, D_801E4294);
        RunTimedDrawScript(&D_800820C4, &g_UiScriptProgress, 0);
        {
        s32 initial;

        initial = -1;
        res = RunTimedDrawScript(&g_UiChromeScript, &g_UiScriptProgress, 1);
        if ((res != 0) && (g_UiScriptProgress2 <= 0)) {
            g_MenuOverlayPattern = initial;
            if (g_PadEdge2 & 0x1000) {
                PlaySoundCue(1);
                D_801E4294 = (D_801E4294 > 0) ? D_801E4294 - 1 : 1;
            }
            if (g_PadEdge2 & 0x4000) {
                PlaySoundCue(1);
                D_801E4294 = (D_801E4294 <= 0) ? D_801E4294 + 1 : 0;
            }
            UpdateCarListCursor();
            sel = g_CarListCursor;
            if ((g_PadHeld & 0x8000) && (g_PrevOwnedCarIndex != -1)) {
                t = g_MenuViewAngleTarget;
                u = g_MenuViewAngle;
                if (t < u ? (u - t <= 0x493DF) : (t - u <= 0x493DF)) {
                    if (g_CarSwapToIndex < 0) {
                        s32 lprev;

                        PlaySoundCue(8);
                        g_CarListCursor = (s32) g_PrevOwnedCarIndex;
                        RequestCarModel(g_PrevOwnedCarIndex);
                        lprev = g_MenuViewAngleTarget;
                        g_CarSwapFromIndex = sel;
                        g_MenuViewAngleTarget = 0;
                        D_8009B330 = -1;
                        g_CarSwapToIndex = g_CarListCursor;
                        g_MenuViewAngle = (g_MenuViewAngle - lprev) + 0x927C0;
                    }
                }
            }
            if ((g_PadHeld & 0x2000) && (g_NextOwnedCarIndex != -1)) {
                t = g_MenuViewAngleTarget;
                u = g_MenuViewAngle;
                if (t < u ? (u - t <= 0x493DF) : (t - u <= 0x493DF)) {
                    if (g_CarSwapToIndex < 0) {
                        s32 base;
                        s32 lprev;

                        PlaySoundCue(8);
                        g_CarListCursor = (s32) g_NextOwnedCarIndex;
                        RequestCarModel(g_NextOwnedCarIndex);
                        base = 0x927C0;
                        lprev = g_MenuViewAngleTarget;
                        g_MenuViewAngleTarget = 0x124F80;
                        g_CarSwapFromIndex = sel;
                        D_8009B330 = -1;
                        g_CarSwapToIndex = g_CarListCursor;
                        g_MenuViewAngle = base - (lprev - g_MenuViewAngle);
                    }
                }
            }
            if (g_CarModelAsset[8] == 0) {
                D_8009B32C = 1;
            } else {
                D_8009B32C = -1;
            }
            t = g_MenuViewAngleTarget;
            u = g_MenuViewAngle;
            if (t < u ? (u - t <= 0x493DF) : (t - u <= 0x493DF)) {
                if (g_CarSwapToIndex < 0) {
                    if (g_PadEdge2 & 0x860) {
                        sel = D_801E4294;
                        if (sel == 1) {
                            if (g_PlayerCarIndex != g_CarListCursor) {
                                s32 base;
                                s32 current;
                                s32 selected;
                                s32 lu;
                                s32 lprev;

                                RequestCarModel(g_PlayerCarIndex);
                                base = 0x927C0;
                                current = g_CarListCursor;
                                selected = g_PlayerCarIndex;
                                lu = g_MenuViewAngle;
                                lprev = g_MenuViewAngleTarget;
                                g_MenuViewAngleTarget = 0;
                                g_CarSwapFromIndex = current;
                                g_CarSwapToIndex = selected;
                                g_MenuViewAngle = (lu - lprev) + base;
                            }
                            PlaySoundCue(3);
                            g_MenuOverlayPattern = 2;
                            GameMenuBusy = sel;
                        D_8009B32C = -1;
                        D_8009B330 = -1;
                                                return;
                        }
                        if (sel != 0) {
                            return;
                        }
                        if (g_CarTable[g_CarListCursor].enabled == 0) {
                            PlaySoundCue(2);
                            GameMenuBusy = -1;
                            g_UiScriptProgress2 = 0;
                            g_MenuSubCursor = 0;
                            switch (g_CarListCursor) {
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
                        if (g_PlayerCarIndex != g_CarListCursor) {
                            s32 base;
                            s32 current;
                            s32 selected;
                            s32 lu;
                            s32 lprev;

                            RequestCarModel(g_PlayerCarIndex);
                            base = 0x927C0;
                            current = g_CarListCursor;
                            selected = g_PlayerCarIndex;
                            lu = g_MenuViewAngle;
                            lprev = g_MenuViewAngleTarget;
                            g_MenuViewAngleTarget = 0;
                            g_CarSwapFromIndex = current;
                            g_CarSwapToIndex = selected;
                            g_MenuViewAngle = (lu - lprev) + base;
                        }
                        PlaySoundCue(3);
                        GameMenuBusy = 1;
                        g_MenuOverlayPattern = 2;
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

                RunTimedDrawScript(D_8019CB00, &g_UiScriptProgress2, 0);
                if (RunTimedDrawScript(&g_UiChromeScript2, &g_UiScriptProgress2, 1) != 0) {
                    if (GameMenuBusy == -1) {
                        if (g_PadEdge2 & 0x860) {
                            if (g_MenuSubCursor != 0) {
                                if (g_PlayerMoney >= value) {
                                    PlaySoundCue(2);
                                    GameMenuBusy = -3;
                                    g_MenuConfirmTimer = 0x23;
                                } else {
                                    PlaySoundCue(5);
                                    D_8019CB00 = &D_800828B0;
                                    GameMenuBusy = -2;
                                }
                            } else {
                                PlaySoundCue(3);
                                GameMenuBusy = 0;
                            }
                        }
                        pad = &g_PadEdge2;
                        if (*pad & 0x90) {
                            PlaySoundCue(3);
                            GameMenuBusy = 0;
                        }
                        if ((*pad & 0x8000) && (g_MenuSubCursor == 0)) {
                            PlaySoundCue(1);
                            g_MenuSubCursor = 1;
                        }
                        if (g_PadEdge2 & 0x2000) {
                            if (g_MenuSubCursor != 0) {
                                PlaySoundCue(1);
                                g_MenuSubCursor = 0;
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
                    DrawMenuCursorBox((g_MenuSubCursor != 0) ? 0xB8 : 0xDA, 0x44, 0x20, 0x20, 0);
                    GameDrawSpriteWide(ot, 0xC0, 0x4C, 0x10, 0x10, 0x9D, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                    GameDrawSpriteWide(ot, 0xE3, 0x4C, 0x10, 0x10, 0xAD, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                    GameDrawMenuButtonWide(0xB8, 0x44, 0x20, 0x20, 0x95, 0x25, 0x1E, 0, 0, 0, &g_MenuBlankCaption);
                    GameDrawMenuButtonWide(0xDA, 0x44, 0x20, 0x20, 0x1E, 0x8E, 0x95, 0, 0, 0, &g_MenuBlankCaption);
                }
            } else if (GameMenuBusy == -3) {
                if (g_MenuConfirmTimer <= 0) {
                    RunTimedDrawScript(D_8019CB00, &g_UiScriptProgress2, -1);
                    RunTimedDrawScript(&g_UiChromeScript2, &g_UiScriptProgress2, 0);
                    if (g_UiScriptProgress2 <= 0) {
                        g_CarTable[g_CarListCursor].enabled = 1;
                        D_801E438D[g_CarListCursor * 8] = 1;
                        GameMenuBusy = 2;
                        D_8009B32C = -1;
                        g_PlayerCarIndex = g_CarListCursor;
                    }
                } else {
                    g_MenuConfirmTimer -= 1;
                    RunTimedDrawScript(D_8019CB00, &g_UiScriptProgress2, 0);
                    RunTimedDrawScript(&g_UiChromeScript2, &g_UiScriptProgress2, 1);
                    DrawMenuCursorBox((g_MenuSubCursor != 0) ? 0xB8 : 0xDA, 0x44, 0x20, 0x20, 1);
                    GameDrawSpriteWide(ot, 0xC0, 0x4C, 0x10, 0x10, 0x9D, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                    GameDrawSpriteWide(ot, 0xE3, 0x4C, 0x10, 0x10, 0xAD, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                    GameDrawMenuButtonWide(0xB8, 0x44, 0x20, 0x20, 0x95, 0x25, 0x1E, 0, 0, 0, &g_MenuBlankCaption);
                    GameDrawMenuButtonWide(0xDA, 0x44, 0x20, 0x20, 0x1E, 0x8E, 0x95, 0, 0, 0, &g_MenuBlankCaption);
                }
            }
            DrawBrowseArrows(1, 0, ~g_PrevOwnedCarIndex != 0, ~g_NextOwnedCarIndex != 0);
            DrawCarShopPricePanel(1, g_PlayerMoney, value);
            DrawFadingMenuSprites(g_UiScriptProgress, 1, D_801E4294);
            RunTimedDrawScript(&D_800820C4, &g_UiScriptProgress, 0);
            RunTimedDrawScript(&g_UiChromeScript, &g_UiScriptProgress, 1);
            return;
        }
        g_MenuHandlerIndex = -1;
        g_MenuHandlerIndex2 = 0xB;
        DrawBrowseArrows(-1, 0, ~g_PrevOwnedCarIndex != 0, ~g_NextOwnedCarIndex != 0);
        DrawCarShopPricePanel(-1, g_PlayerMoney, value);
        RunTimedDrawScript(&D_800820C4, &g_UiScriptProgress, -1);
        RunTimedDrawScript(&g_UiChromeScript, &g_UiScriptProgress, 0);
        DrawFadingMenuSprites(g_UiScriptProgress, 1, D_801E4294);
        if (g_UiScriptProgress <= 0) {
            if (GameMenuBusy == 2) {
                g_PlayerMoney -= value;
            }
            g_MenuScreen = 4;
            g_MenuHandlerIndex = 4;
            g_UiScriptProgress = 0;
            GameMenuBusy = 0;
            D_801E4294 = 0;
            UploadTeamNameTexture(&g_TeamNameChars, g_TeamNameLength);
            UploadTeamLogoClut();
        }
    }
}

extern u32 D_8009B2EC;

u32 DrawEngineerShopScreen(s32 arg0) {
    u32 value;
    s32 amount;

    if (arg0 == 0) {
        D_8009B2EC = 0;
        return;
    }

    if (arg0 > 0) {
        value = arg0 + D_8009B2EC;
        D_8009B2EC = value;
        if ((s32)value >= 0x1FD) {
            D_8009B2EC = 0x1FC;
        }
        amount = 0;
    } else {
        s32 diff = 0x1FC;
        s32 product;

        value = arg0 + D_8009B2EC;
        D_8009B2EC = value;
        if ((s32)value < 0) {
            D_8009B2EC = 0;
        }
        diff -= D_8009B2EC;
        product = diff * diff;
        amount = (u32)product / 2048;
    }

    DrawCarEngineSpec((s16)amount, (u8)(D_8009B2EC >> 2), g_PlayerCarIndex);
    return D_8009B2EC;
}

extern u8 D_80082130;
extern u8 D_80082898;
extern u8 D_80082A54;
extern s32 D_80082D80[];
extern u8 *D_801E4188;
extern s32 D_801E4290;

void RequestUpgradedCarModel(s32 arg0);
void DrawEngineerShopPricePanel(s32 arg0, s32 arg1, s32 arg2);

void UpdateEngineerShopScreen(void) {
    void *ot;
    s32 value;
    s32 res;
    s32 sel;

    ot = *(void **)0x1F800004;
    g_MenuAltLayout = g_MenuAltLayoutSetting;
    DrawCarNamePlate(g_CarNamePlateStep, g_MenuPlateCarIndex, 0);
    DrawMenuCarView();
    g_MenuPlateCarIndex = g_PlayerCarIndex;
    value = D_80082D80[GetOwnedCarAssetIndex(g_PlayerCarIndex)];
    if (GameMenuBusy == 0) {
        RunTimedDrawScript(D_801E4188, &g_UiScriptProgress2, -1);
        RunTimedDrawScript(&g_UiChromeScript2, &g_UiScriptProgress2, 0);
        DrawEngineerShopPricePanel(1, g_PlayerMoney, value);
        DrawFadingMenuSprites(g_UiScriptProgress, 1, D_801E4290);
        RunTimedDrawScript(&D_80082130, &g_UiScriptProgress, 0);
        res = RunTimedDrawScript(&g_UiChromeScript, &g_UiScriptProgress, 1);
        if ((res != 0) && (g_UiScriptProgress2 <= 0)) {
            g_MenuOverlayPattern = -1;
            if (g_PadEdge2 & 0x1000) {
                PlaySoundCue(1);
                D_801E4290 = (D_801E4290 > 0) ? D_801E4290 - 1 : 1;
            }
            if (g_PadEdge2 & 0x4000) {
                PlaySoundCue(1);
                D_801E4290 = (D_801E4290 <= 0) ? D_801E4290 + 1 : 0;
            }
            if (g_PadEdge2 & 0x860) {
                sel = D_801E4290;
                if (sel == 0) {
                    if (g_PlayerMoney >= value) {
                        PlaySoundCue(2);
                        D_801E4188 = &D_80082A54;
                        GameMenuBusy = -1;
                        g_UiScriptProgress2 = 0;
                        g_MenuSubCursor = 0;
                    } else {
                        PlaySoundCue(5);
                        D_801E4188 = &D_80082898;
                        GameMenuBusy = -3;
                        g_UiScriptProgress2 = 0;
                    }
                } else if (sel == 1) {
                    PlaySoundCue(3);
                    GameMenuBusy = sel;
                    g_MenuOverlayPattern = 2;
                }
            } else if (g_PadEdge2 & 0x90) {
                PlaySoundCue(3);
                GameMenuBusy = 1;
                g_MenuOverlayPattern = 2;
            }
        }
    } else {
        if (GameMenuBusy < 0) {
            if (GameMenuBusy == -1) {
                u16 *pad;

                RunTimedDrawScript(D_801E4188, &g_UiScriptProgress2, 0);
                if (RunTimedDrawScript(&g_UiChromeScript2, &g_UiScriptProgress2, 1) != 0) {
                    if (g_PadEdge2 & 0x860) {
                        if (g_MenuSubCursor != 0) {
                            PlaySoundCue(2);
                            GameMenuBusy = -2;
                            g_MenuConfirmTimer = 0x23;
                            RequestUpgradedCarModel(g_PlayerCarIndex);
                        } else {
                            PlaySoundCue(3);
                            GameMenuBusy = 0;
                        }
                    }
                    pad = &g_PadEdge2;
                    if (*pad & 0x90) {
                        PlaySoundCue(3);
                        GameMenuBusy = 0;
                    }
                    if ((*pad & 0x8000) && (g_MenuSubCursor == 0)) {
                        PlaySoundCue(1);
                        g_MenuSubCursor = 1;
                    }
                    if (g_PadEdge2 & 0x2000) {
                        if (g_MenuSubCursor != 0) {
                            PlaySoundCue(1);
                            g_MenuSubCursor = 0;
                        }
                    }
                    DrawMenuCursorBox((g_MenuSubCursor != 0) ? 0xB8 : 0xDA, 0x44, 0x20, 0x20, 0);
                    GameDrawSpriteWide(ot, 0xC0, 0x4C, 0x10, 0x10, 0x9D, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                    GameDrawSpriteWide(ot, 0xE3, 0x4C, 0x10, 0x10, 0xAD, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                    GameDrawMenuButtonWide(0xB8, 0x44, 0x20, 0x20, 0x95, 0x25, 0x1E, 0, 0, 0, &g_MenuBlankCaption);
                    GameDrawMenuButtonWide(0xDA, 0x44, 0x20, 0x20, 0x1E, 0x8E, 0x95, 0, 0, 0, &g_MenuBlankCaption);
                }
            } else if (GameMenuBusy == -2) {
                if (g_MenuConfirmTimer <= 0) {
                    RunTimedDrawScript(D_801E4188, &g_UiScriptProgress2, -1);
                    RunTimedDrawScript(&g_UiChromeScript2, &g_UiScriptProgress2, 0);
                    if (g_UiScriptProgress2 <= 0) {
                        g_MenuViewAngle = 0x927C0;
                        g_MenuViewAngleTarget = 0;
                        GameMenuBusy = 2;
                        g_MenuOverlayPattern = 2;
                        g_CarSwapFromIndex = g_PlayerCarIndex;
                        g_CarSwapToIndex = g_PlayerCarIndex;
                    }
                } else {
                    g_MenuConfirmTimer -= 1;
                    RunTimedDrawScript(D_801E4188, &g_UiScriptProgress2, 0);
                    RunTimedDrawScript(&g_UiChromeScript2, &g_UiScriptProgress2, 1);
                    DrawMenuCursorBox((g_MenuSubCursor != 0) ? 0xB8 : 0xDA, 0x44, 0x20, 0x20, 1);
                    GameDrawSpriteWide(ot, 0xC0, 0x4C, 0x10, 0x10, 0x9D, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                    GameDrawSpriteWide(ot, 0xE3, 0x4C, 0x10, 0x10, 0xAD, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                    GameDrawMenuButtonWide(0xB8, 0x44, 0x20, 0x20, 0x95, 0x25, 0x1E, 0, 0, 0, &g_MenuBlankCaption);
                    GameDrawMenuButtonWide(0xDA, 0x44, 0x20, 0x20, 0x1E, 0x8E, 0x95, 0, 0, 0, &g_MenuBlankCaption);
                }
            } else {
                RunTimedDrawScript(D_801E4188, &g_UiScriptProgress2, 0);
                if (RunTimedDrawScript(&g_UiChromeScript2, &g_UiScriptProgress2, 1) != 0) {
                    if (g_PadEdge2 & 0x860) {
                        GameMenuBusy = 0;
                    }
                    if (g_PadEdge2 & 0x90) {
                        GameMenuBusy = 0;
                    }
                }
            }
            DrawEngineerShopPricePanel(1, g_PlayerMoney, value);
            DrawFadingMenuSprites(g_UiScriptProgress, 1, D_801E4290);
            RunTimedDrawScript(&D_80082130, &g_UiScriptProgress, 0);
            RunTimedDrawScript(&g_UiChromeScript, &g_UiScriptProgress, 1);
            return;
        }
        g_MenuHandlerIndex = -1;
        g_MenuHandlerIndex2 = 0xC;
        DrawEngineerShopPricePanel(-1, g_PlayerMoney, value);
        RunTimedDrawScript(&D_80082130, &g_UiScriptProgress, -1);
        RunTimedDrawScript(&g_UiChromeScript, &g_UiScriptProgress, 0);
        DrawFadingMenuSprites(g_UiScriptProgress, 1, D_801E4290);
        if (g_UiScriptProgress <= 0) {
            if (GameMenuBusy == 2) {
                g_CarTable[g_PlayerCarIndex].modelVariant++;
                if (g_CarTable[g_PlayerCarIndex].modelVariant > g_TimeAttackCars[g_PlayerCarIndex].modelVariant) {
                    g_TimeAttackCars[g_PlayerCarIndex].modelVariant = g_CarTable[g_PlayerCarIndex].modelVariant;
                }
                g_PlayerMoney -= value;
            }
            g_MenuScreen = 4;
            g_MenuHandlerIndex = 4;
            g_UiScriptProgress = 0;
            GameMenuBusy = 0;
            D_801E4290 = 0;
        }
    }
}

void ShopScreenNoOp(void) {
}
