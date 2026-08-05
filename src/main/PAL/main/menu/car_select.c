#include "common.h"
#include "game/car.h"
#include "game/race.h"
#include "game/menu.h"
#include "game/render.h"

s32 DrawCarSelectScreen(s32 arg0);
s32 DrawCustomizeScreen(s32 arg0);

extern u8 *g_CarModelAsset asm("D_8009E698");


extern s32 D_8009B2CC;
extern s32 D_8009B2C8;
extern s32 g_TimeAttackPlateStep;
extern s32 D_8019CDF8;
extern u8 D_80082724;
extern u8 D_80081890;
extern u8 g_UiChromeScript asm("D_80082460");

void func_80047460(void *buf, s32 xa, s32 ya, s32 w, s32 h, s32 r, s32 g, s32 b, s32 code);
void func_80046A2C(void *ot, s32 x0, s32 y0, s32 x1, s32 y1, s32 u0, s32 v0,
                   s32 r, s32 g, s32 b, s32 clut, s32 sh, s32 st, s32 flags);

void DrawMenuCourseView(void);
void DrawMenuLightBurst(s32 arg0);
void DrawFadingMenuSprites(s32 arg0, s32 arg1, s32 arg2);
void PlaySoundCue(s32 cue);
s32 DrawRankingTable(s32 *arg0, s32 arg1, s32 arg2);
void DrawTimeAttackPlate(s32 arg0);

void UpdateRankingScreen(void);
void UpdateRankingScreen(void) {
    s32 state;

    g_MenuAltLayout = 0;
    DrawMenuCourseView();
    DrawMenuLightBurst(-9);
    state = GameMenuBusy;
    if (state == 0) {
        g_UiScriptProgress2 = 0;
        GameMenuBusy = -1;
        DrawFadingMenuSprites(0, 2, D_8019CDF8);
        RunTimedDrawScript(&D_80082724, &g_UiScriptProgress2, 1);
    } else if (state < 0) {
        switch (state) {
        case -1:
            DrawFadingMenuSprites(g_UiScriptProgress2, 2, D_8019CDF8);
            if (RunTimedDrawScript(&D_80082724, &g_UiScriptProgress2, 1) != 0) {
                g_MenuOverlayPattern = -1;
                if (g_PadEdge2 & 0x1000) {
                    PlaySoundCue(1);
                    D_8019CDF8 = (D_8019CDF8 > 0) ? D_8019CDF8 - 1 : 2;
                }
                if (g_PadEdge2 & 0x4000) {
                    PlaySoundCue(1);
                    D_8019CDF8 = (D_8019CDF8 < 2) ? D_8019CDF8 + 1 : 0;
                }
                {
                    s32 flags = g_PadEdge2;
                    if (flags & 0x860) {
                        s32 x = D_8019CDF8;
                        if (x == 0) {
                            PlaySoundCue(2);
                            GameMenuBusy = -2;
                            D_8009B2C8 = -3;
                        } else if (x == 1) {
                            PlaySoundCue(2);
                            GameMenuBusy = -2;
                            D_8009B2C8 = -5;
                        } else if (x == 2) {
                            PlaySoundCue(3);
                            GameMenuBusy = 1;
                            g_MenuOverlayPattern = x;
                        }
                    } else if (flags & 0x90) {
                        PlaySoundCue(3);
                        GameMenuBusy = 1;
                        g_MenuOverlayPattern = 2;
                    }
                }
            }
            break;
        case -2:
            RunTimedDrawScript(&D_80082724, &g_UiScriptProgress2, -1);
            DrawFadingMenuSprites(g_UiScriptProgress2, 2, D_8019CDF8);
            if (g_UiScriptProgress2 > 0) {
                break;
            }
            GameMenuBusy = D_8009B2C8;
            break;
        case -3:
            if (DrawRankingTable(&g_UiScriptProgress2, 1, 0) == 0) {
                break;
            }
            if (!(g_PadEdge2 & 0x8f0)) {
                break;
            }
            PlaySoundCue(3);
            GameMenuBusy = -4;
            break;
        case -4:
            DrawRankingTable(&g_UiScriptProgress2, -1, 0);
            if (g_UiScriptProgress2 > 0) {
                break;
            }
            GameMenuBusy = -1;
            break;
        case -5:
            if (DrawRankingTable(&g_UiScriptProgress2, 1, 1) == 0) {
                break;
            }
            if (!(g_PadEdge2 & 0x8f0)) {
                break;
            }
            PlaySoundCue(3);
            GameMenuBusy = -6;
            break;
        case -6:
            DrawRankingTable(&g_UiScriptProgress2, -1, 1);
            if (g_UiScriptProgress2 > 0) {
                break;
            }
            GameMenuBusy = -1;
            break;
        }
    } else {
        goto pos;
    }
    RunTimedDrawScript(&D_80081890, &g_UiScriptProgress, 0);
    RunTimedDrawScript(&g_UiChromeScript, &g_UiScriptProgress, 1);
    return;
pos:
    g_MenuHandlerIndex = -1;
    g_MenuHandlerIndex2 = 2;
    RunTimedDrawScript(&D_80082724, &g_UiScriptProgress2, -1);
    DrawFadingMenuSprites(g_UiScriptProgress2, 2, D_8019CDF8);
    RunTimedDrawScript(&D_80081890, &g_UiScriptProgress, -1);
    RunTimedDrawScript(&g_UiChromeScript, &g_UiScriptProgress, 0);
    if (g_UiScriptProgress > 0) {
        return;
    }
    g_MenuScreen = 1;
    g_MenuHandlerIndex = 1;
    D_8019CDF8 = 0;
    g_UiScriptProgress = 0;
    GameMenuBusy = 0;
    DrawTimeAttackPlate(0);
    if (g_CourseIndex >= 4) {
        g_TimeAttackPlateStep = 1;
    } else {
        g_TimeAttackPlateStep = -1;
    }
}

s32 DrawCarSelectScreen(s32 step) asm("func_800551BC");
s32 DrawCarSelectScreen(s32 arg0) {
    s32 p = (s32) *(void **)0x1F800004;
    u8 *buf = (u8 *)p + 4;
    s32 v;
    s32 col;
    s32 xpos;
    s32 mode;
    u8 tex;

    if (arg0 == 0) {
        D_8009B2CC = 0;
        return p;
    }

    if (arg0 > 0) {
        D_8009B2CC += arg0;
        if (D_8009B2CC >= 509) {
            D_8009B2CC = 508;
        }
    } else {
        D_8009B2CC += arg0;
        if (D_8009B2CC < 0) {
            D_8009B2CC = 0;
        }
    }

    v = (u32) D_8009B2CC / 4;
    col = v & 0xff;
    func_80047460(buf, 0xa3, 0x180, 0x1a, 0x19, col, col, col, 0x20);

    tex = ((u8 *)g_CarTable)[g_PlayerCarIndex * 8 + 2];
    if (tex != 0) {
        func_80046A2C(buf, 0xad, 0x185, 0x10, 0x10, 0x6c, 0x7c, col, col, col,
                      0x244, 0, 1, 0x3b);
        xpos = 0xa5;
    } else {
        func_80046A2C(buf, 0xae, 0x185, 0xc, 0x10, 0x60, 0x7c, col, col, col,
                      0x244, 0, 1, 0x3b);
        xpos = 0xa6;
    }

    mode = g_CarModelAsset[9];
    switch (mode) {
    case 4:
        func_80046A2C(buf, xpos, 0x185, 8, 0x10, 0x20, 0x18, v & 0xff, v & 0xff,
                      v & 0xff, 0x244, 0, 1, 0x3b);
        break;
    case 5:
        func_80046A2C(buf, xpos, 0x185, 8, 0x10, 0x28, 0x18, v & 0xff, v & 0xff,
                      v & 0xff, 0x244, 0, 1, 0x3b);
        break;
    case 6:
        func_80046A2C(buf, xpos, 0x185, 8, 0x10, 0x30, 0x18, v & 0xff, v & 0xff,
                      v & 0xff, 0x244, 0, 1, 0x3b);
        break;
    }

    return D_8009B2CC;
}

extern s16 g_PrevOwnedCarIndex;
extern s16 g_NextOwnedCarIndex;

void UpdateOwnedCarNeighbours(void);
void UpdateOwnedCarNeighbours(void) {
    s32 index;
    GameCarEntry *ptr;

    g_PrevOwnedCarIndex = -1;
    index = g_PlayerCarIndex - 1;
    if (index >= 0) {
        s32 one = 1;
        s32 offset = index * 8;
        ptr = (GameCarEntry *)(offset + (s32)g_CarTable);
        while (index >= 0) {
            if (ptr->enabled == one) {
                g_PrevOwnedCarIndex = index;
                break;
            }
            index--;
            ptr--;
        }
    }

    g_NextOwnedCarIndex = -1;
    index = g_PlayerCarIndex + 1;
    if (index < 13) {
        s32 one = 1;
        s32 offset = index * 8;
        ptr = (GameCarEntry *)(offset + (s32)g_CarTable);
        while (index < 13) {
            if (ptr->enabled == one) {
                g_NextOwnedCarIndex = index;
                break;
            }
            index++;
            ptr++;
        }
    }
}

extern s32 D_8009B33C;
extern s32 D_8019C7B8;

s32 GetCarUnlockLevel(s32 model);

void RefreshCarUnlockState(void);
void RefreshCarUnlockState(void) {
    s32 index;
    s32 value;
    GameCarEntry *ptr;
    GameCarEntry *enabledPtr;
    s32 byte;
    s32 enabledBase;

    D_8019C7B8 = -1;

    if (D_8009B33C != 0) {
        index = 12;
        enabledBase = (s32)g_CarTable;
        enabledPtr = (GameCarEntry *)(enabledBase + 0x60);
while (1) {
        byte = enabledPtr->enabled;
        enabledPtr--;
        if (byte == 0) {
            D_8019C7B8 = index;
        }
        index--;
        if (index < 0) {
            return;
        }
        }
    }

    index = 12;
do {
    {
        value = GetCarUnlockLevel(index);
        {
            s32 offset = index * 8;
            ptr = (GameCarEntry *)(offset + (s32)g_CarTable);
        }
        if (ptr->enabled == 0) {
            if (g_RaceProgress->maxClassReached < 4) {
                if ((g_RaceProgress->maxClassReached + 1) < value) {
                    index--;
                    continue;
                }
            } else if (g_RaceProgress->maxClassReached < value) {
                index--;
                continue;
            }
            D_8019C7B8 = index;
        }
        index--;
    }
    } while (index >= 0);

}

extern s32 g_CarNamePlateStep;
extern s32 g_MenuPlateCarIndex;

void InstallCarModelSlot(void);
void DrawCarNamePlate(s32 arg0, s32 arg1, s32 arg2);
void DrawMenuCarView(void);

void EnterCarSelectScreen(void);
void EnterCarSelectScreen(void) {
    g_MenuAltLayout = g_MenuAltLayoutSetting;
    InstallCarModelSlot();
    g_MenuScreen = 4;
    g_UiScriptProgress = 0;
    UpdateOwnedCarNeighbours();
    DrawCarNamePlate(g_CarNamePlateStep, g_MenuPlateCarIndex, 0);
    DrawMenuCarView();
    DrawMenuLightBurst(-9);
}

extern u8 D_800818CC;
extern u8 D_800819A4;
extern u8 g_UiChromeScript2 asm("D_80082790");
extern u8 D_8008285C;
extern u8 D_80082874;
extern u8 *D_801E40B4;
extern s32 D_8009B324;
extern s32 D_8009B32C;
extern s32 D_8009B330;
extern s32 D_8009B348;
extern s32 g_MenuHintBarStep;
extern s32 D_8009B360;
extern s32 D_8009B368;
extern s32 g_MenuCourseModelIndex;
extern s32 g_MenuPendingCourseIndex;
extern s32 g_CarSwapFromIndex asm("D_8009B374");
extern s32 g_CarSwapToIndex asm("D_8009B378");
extern s32 D_8019C7B8;
extern s32 g_PlayerMoney;
extern s32 D_801E4138;
extern s32 g_SceneId;
extern u8 *g_CourseProgress asm("D_8009E67C");

void RequestCarModel(s32 carIndex);
s32 RequestRoundAssets(void);
void ClearTeamNameTexture(void);
void DrawBrowseArrows(s32 step, s32 wide, s32 drawLeft, s32 drawRight)
;
void DrawCarShopPricePanel(s32 step, s32 money, s32 price)
;
void DrawEngineerShopPricePanel(s32 step, s32 arg1, s32 arg2)
;
s32 CountOwnedCars(void);
void StartSequenceFadeOut(void);
void RestoreTeamLogoClut(void);
void DrawOwnedCarCounter(s32 step, s32 count);
void DrawMenuAltPanel(s32 arg0, s32 arg1);

void UpdateCarSelectScreen(void);
void UpdateCarSelectScreen(void) {
    s32 mode;
    u8 *cmdList;
    s32 lowMode;
    s32 sel;
    s32 t;
    s32 u;

    g_MenuAltLayout = g_MenuAltLayoutSetting;
    DrawCarNamePlate(g_CarNamePlateStep, g_MenuPlateCarIndex, 0);
    mode = 2;
    DrawMenuCarView();
    DrawMenuLightBurst(-9);
    if (g_GrandPrixMode != 0) {
        mode = 4;
    }
    cmdList = &D_800819A4;
    if (g_GrandPrixMode != 0) {
        cmdList = &D_800818CC;
    }

    if (GameMenuBusy == 0) {
        g_CarNamePlateStep = 0x14;
        D_8009B324 = 3;
        g_MenuPlateCarIndex = g_PlayerCarIndex;
        RunTimedDrawScript(D_801E40B4, &g_UiScriptProgress2, -1);
        RunTimedDrawScript(&g_UiChromeScript2, &g_UiScriptProgress2, 0);
        DrawBrowseArrows(
            1, 0, ~g_PrevOwnedCarIndex != 0, ~g_NextOwnedCarIndex != 0);
        {
            s32 initial;

            initial = -1;
            if (g_GrandPrixMode == 0) {
                DrawOwnedCarCounter(1, CountOwnedCars());
            }
            lowMode = mode & 0xFF;
            DrawFadingMenuSprites(g_UiScriptProgress, lowMode, D_801E4138);
            RunTimedDrawScript(cmdList, &g_UiScriptProgress, 0);
            if ((RunTimedDrawScript(&g_UiChromeScript, &g_UiScriptProgress, 1) !=
                 0) &&
                (g_UiScriptProgress2 <= 0)) {
                g_MenuOverlayPattern = initial;
                if (g_PadEdge2 & 0x1000) {
                    PlaySoundCue(1);
                    D_801E4138 =
                        (D_801E4138 > 0) ? D_801E4138 - 1 : lowMode;
                }
                if (g_PadEdge2 & 0x4000) {
                    PlaySoundCue(1);
                    D_801E4138 =
                        (D_801E4138 < mode) ? D_801E4138 + 1 : 0;
                }
                UpdateOwnedCarNeighbours();
                RefreshCarUnlockState();
                sel = g_PlayerCarIndex;
                if ((g_PadHeld & 0x8000) && (g_PrevOwnedCarIndex != -1)) {
                    t = g_MenuViewAngleTarget;
                    u = g_MenuViewAngle;
                    if (t < u ? (u - t <= 0x493DF) : (t - u <= 0x493DF)) {
                        if (g_CarSwapToIndex < 0) {
                            s32 prev;

                            PlaySoundCue(8);
                            g_PlayerCarIndex = (s32)g_PrevOwnedCarIndex;
                            RequestCarModel(g_PrevOwnedCarIndex);
                            prev = g_MenuViewAngleTarget;
                            g_CarSwapFromIndex = sel;
                            g_MenuViewAngleTarget = 0;
                            D_8009B330 = -1;
                            g_CarSwapToIndex = g_PlayerCarIndex;
                            g_MenuViewAngle =
                                (g_MenuViewAngle - prev) + 0x927C0;
                        }
                    }
                }
                if ((g_PadHeld & 0x2000) && (g_NextOwnedCarIndex != -1)) {
                    t = g_MenuViewAngleTarget;
                    u = g_MenuViewAngle;
                    if (t < u ? (u - t <= 0x493DF) : (t - u <= 0x493DF)) {
                        if (g_CarSwapToIndex < 0) {
                            s32 base;
                            s32 prev;

                            PlaySoundCue(8);
                            g_PlayerCarIndex = (s32)g_NextOwnedCarIndex;
                            RequestCarModel(g_NextOwnedCarIndex);
                            base = 0x927C0;
                            prev = g_MenuViewAngleTarget;
                            g_MenuViewAngleTarget = 0x124F80;
                            g_CarSwapFromIndex = sel;
                            D_8009B330 = -1;
                            g_CarSwapToIndex = g_PlayerCarIndex;
                            g_MenuViewAngle =
                                base - (prev - g_MenuViewAngle);
                        }
                    }
                }
                t = g_MenuViewAngleTarget;
                u = g_MenuViewAngle;
                if (t < u ? (u - t <= 0x493DF) : (t - u <= 0x493DF)) {
                    if (g_CarSwapToIndex < 0) {
                        if (g_PadEdge2 & 0x860) {
                            s32 choice;

                            choice = D_801E4138;
                            if (choice == 0) {
                                u16 series;

                                PlaySoundCue(2);
                                StartSequenceFadeOut();
                                if (g_GrandPrixMode != 0) {
                                    series = 0;
                                    if (g_GrandPrixClass < 5) {
                                        series = (u16)g_GrandPrixSeries;
                                    }
                                    g_GrandPrixSeries = series;
                                } else {
                                    g_GrandPrixSeries = g_CourseIndex >> 2;
                                }
                                RequestRoundAssets();
                                GameMenuBusy = 1;
                                g_MenuHintBarStep = -1;
                                g_CarNamePlateStep = -10;
                                g_MenuOverlayPattern = 0;
                                D_8009B324 = -3;
                                g_MenuViewOffsetTarget = 0x3D090;
                                return;
                            }
                            if (choice == 1) {
                                PlaySoundCue(2);
                                GameMenuBusy = 2;
                                g_MenuOverlayPattern = 1;
                                g_CarNamePlateStep = -10;
                                return;
                            }
                            if (choice == mode) {
                                PlaySoundCue(3);
                                GameMenuBusy = 5;
                                g_MenuOverlayPattern = 2;
                                g_CarNamePlateStep = -10;
                                D_8009B324 = -3;
                                g_MenuViewOffsetTarget = 0x3D090;
                                return;
                            }
                            if (choice == 2) {
                                s32 car;

                                car = D_8019C7B8;
                                if (car != -1) {
                                    s32 base;
                                    s32 prev;

                                    PlaySoundCue(2);
                                    g_CarListCursor = D_8019C7B8;
                                    RequestCarModel(g_CarListCursor);
                                    base = 0x927C0;
                                    prev = g_MenuViewAngleTarget;
                                    g_MenuViewAngleTarget = 0x124F80;
                                    GameMenuBusy = 3;
                                    g_MenuOverlayPattern = 1;
                                    g_CarSwapFromIndex = g_PlayerCarIndex;
                                    g_CarSwapToIndex = g_CarListCursor;
                                    g_MenuViewAngle =
                                        base - (prev - g_MenuViewAngle);
                                    return;
                                }
                                PlaySoundCue(5);
                                D_801E40B4 = &D_8008285C;
                                GameMenuBusy = car;
                                g_UiScriptProgress2 = 0;
                                return;
                            }
                            if (choice == 3) {
                                s32 unlockLevel;

                                if (g_CarModelAsset[0xA] != 0) {
                                    unlockLevel =
                                        GetCarUnlockLevel(g_PlayerCarIndex);
                                    if (g_RaceProgress->maxClassReached >=
                                        unlockLevel) {
                                        GameMenuBusy = 4;
                                        g_MenuOverlayPattern = 1;
                                        PlaySoundCue(2);
                                        return;
                                    }
                                }
                                PlaySoundCue(5);
                                D_801E40B4 = &D_80082874;
                                GameMenuBusy = -2;
                                g_UiScriptProgress2 = 0;
                                return;
                            }
                        } else if ((g_PadEdge2 & 0x90) &&
                                   ((u32)(g_MenuViewAngle - 0x2710) >
                                    0x120160U)) {
                            PlaySoundCue(3);
                            GameMenuBusy = 5;
                            g_MenuOverlayPattern = 2;
                            g_CarNamePlateStep = -10;
                            D_8009B324 = -3;
                            g_MenuViewOffsetTarget = 0x3D090;
                        }
                    }
                }
            }
        }
        return;
    }

    if (GameMenuBusy < 0) {
        RunTimedDrawScript(D_801E40B4, &g_UiScriptProgress2, 0);
        if (RunTimedDrawScript(&g_UiChromeScript2, &g_UiScriptProgress2, 1) != 0) {
            if (g_PadEdge2 & 0x860) {
                GameMenuBusy = 0;
            }
            if (g_PadEdge2 & 0x90) {
                GameMenuBusy = 0;
            }
        }
        DrawBrowseArrows(
            1, 0, ~g_PrevOwnedCarIndex != 0, ~g_NextOwnedCarIndex != 0);
        if (g_GrandPrixMode == 0) {
            DrawOwnedCarCounter(1, CountOwnedCars());
        }
        DrawFadingMenuSprites(g_UiScriptProgress, mode, D_801E4138);
        RunTimedDrawScript(cmdList, &g_UiScriptProgress, 0);
        RunTimedDrawScript(&g_UiChromeScript, &g_UiScriptProgress, 1);
        return;
    }

    g_MenuHandlerIndex = -1;
    g_MenuHandlerIndex2 = 4;
    DrawBrowseArrows(
        -1, 0, ~g_PrevOwnedCarIndex != 0, ~g_NextOwnedCarIndex != 0);
    if (g_GrandPrixMode == 0) {
        DrawOwnedCarCounter(-1, CountOwnedCars());
    }
    RunTimedDrawScript(cmdList, &g_UiScriptProgress, -1);
    RunTimedDrawScript(&g_UiChromeScript, &g_UiScriptProgress, 0);
    DrawFadingMenuSprites(g_UiScriptProgress, mode, D_801E4138);
    if (g_UiScriptProgress <= 0) {
        switch (GameMenuBusy) {
        case 1:
            if ((D_8009B348 > 0) &&
                (g_MenuViewOffset <= 0x3D08F)) {
                return;
            }
            g_SceneId = 9;
            g_CourseIndex &= 3;
            g_RaceProgress->course = g_CourseIndex;
            g_RaceProgress->carIndex = g_PlayerCarIndex;
            g_RaceProgress->classIndex = g_GrandPrixClass;
            if (g_GrandPrixMode != 0) {
                g_RaceProgress->money = g_PlayerMoney;
            } else {
                g_RaceProgress->money = g_GrandPrixSeries;
            }
            break;
        case 2:
            g_MenuScreen = 5;
            g_MenuHandlerIndex = 5;
            break;
        case 3:
            g_MenuScreen = 0xB;
            g_MenuHandlerIndex = 0xB;
            DrawCarShopPricePanel(0, 0, 0);
            DrawBrowseArrows(0, 0, 0, 0);
            DrawMenuAltPanel(0, 0);
            D_8009B32C = 0;
            D_8009B330 = 0;
            ClearTeamNameTexture();
            RestoreTeamLogoClut();
            break;
        case 4:
            g_MenuScreen = 0xC;
            g_MenuHandlerIndex = 0xC;
            DrawEngineerShopPricePanel(0, 0, 0);
            break;
        case 5:
        {
            s32 angle;
            s32 offset;
            s32 largeValue;
            s32 course;
            s32 one;
            s32 minusOne;

            if (g_MenuViewOffset <= 0x3D08F) {
                return;
            }
            angle = 0x7A120;
            offset = 0x3D090;
            largeValue = 0x1F4000;
            course = g_CourseIndex;
            one = 1;
            minusOne = -1;
            g_MenuViewAngle = angle;
            g_MenuViewAngleTarget = angle;
            g_MenuScreen = one;
            g_MenuHandlerIndex = one;
            D_801E4138 = 0;
            g_MenuPendingCourseIndex = minusOne;
            g_MenuViewOffset = offset;
            g_MenuViewOffsetTarget = 0;
            D_8009B360 = largeValue;
            g_MenuCourseModelIndex = course;
            D_8009B368 = g_CourseProgress[course & 3];
            DrawTimeAttackPlate(0);
            if (g_CourseIndex >= 4) {
                g_TimeAttackPlateStep = one;
            } else {
                g_TimeAttackPlateStep = minusOne;
            }
            break;
        }
        }
        g_UiScriptProgress = 0;
        GameMenuBusy = 0;
    }
}

extern s32 D_8009B2D0;

void func_80052158(s32 arg0, s32 arg1, s32 arg2);

s32 DrawCustomizeScreen(s32 step) asm("func_800562C8");
s32 DrawCustomizeScreen(s32 arg0) {
    s32 value;

    if (arg0 == 0) {
        D_8009B2D0 = 0;
        return;
    }

    if (arg0 > 0) {
        value = arg0 + D_8009B2D0;
        D_8009B2D0 = value;
        if (value >= 0x1FD) {
            D_8009B2D0 = 0x1FC;
        }
        value = 0;
    } else {
        s32 limit;
        u32 product;

        value = arg0 + D_8009B2D0;
        D_8009B2D0 = value;
        limit = 0x1FC;
        if (value < 0) {
            D_8009B2D0 = 0;
        }
        limit = limit - D_8009B2D0;
        product = (u32)(limit * limit);
        value = product / 2048;
    }

    func_80052158((s16)value, ((u32)D_8009B2D0 / 4) & 0xFF, g_PlayerCarIndex);
    return D_8009B2D0;
}

extern u8 g_MenuBlankCaption asm("D_80011BA0");
extern u8 D_80081A34;
extern u8 D_80081AD0;
extern u8 D_80082574;
extern u8 D_800825A4;
extern u8 g_UiChromeScript2 asm("D_80082790");
extern u8 D_80082814;
extern u8 g_MenuSubCursor;
extern s32 g_MenuConfirmTimer;
extern s32 D_8009B324;
extern u8 *D_8019C794;
extern s32 g_RankingOption;
extern u8 D_801E4389[];
extern u8 D_801E438A[];
void DrawTireCompoundSlider(u8 x, s32 useFlag);
void DrawMenuCursorBox(s32 x0, s32 y0, s32 x1, s32 y1, s32 useFlash);
void func_80048B88(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6,
                   s32 a7, s32 a8, s32 a9, void *a10);

void UpdateCustomizeScreen(void);
void UpdateCustomizeScreen(void) {
    void *ot;
    s32 mode;
    s32 lowMode;
    u8 *cmdList;
    u16 *pad;
    s32 sel;

    ot = *(void **)0x1F800004;
    g_MenuAltLayout = g_MenuAltLayoutSetting;
    DrawCarNamePlate(g_CarNamePlateStep, g_MenuPlateCarIndex, 0);
    mode = 2;
    DrawMenuCarView();
    if (g_GrandPrixMode != 0) {
        mode = 3;
    }
    cmdList = &D_80081AD0;
    if (g_GrandPrixMode != 0) {
        cmdList = &D_80081A34;
    }

    if (GameMenuBusy == 0) {
        D_8009B324 = 3;
        RunTimedDrawScript(D_8019C794, &g_UiScriptProgress2, -1);
        lowMode = mode & 0xFF;
        DrawFadingMenuSprites(g_UiScriptProgress, lowMode, g_RankingOption);
        RunTimedDrawScript(cmdList, &g_UiScriptProgress, 0);
        if ((RunTimedDrawScript(&g_UiChromeScript, &g_UiScriptProgress, 1) != 0) && (g_UiScriptProgress2 <= 0)) {
            g_MenuOverlayPattern = -1;
            if (g_PadEdge2 & 0x1000) {
                PlaySoundCue(1);
                g_RankingOption = (g_RankingOption > 0) ? g_RankingOption - 1 : lowMode;
            }
            if (g_PadEdge2 & 0x4000) {
                PlaySoundCue(1);
                g_RankingOption = (g_RankingOption < mode) ? g_RankingOption + 1 : 0;
            }
            if (g_PadEdge2 & 0x860) {
                u8 carByte;

                sel = g_RankingOption;
                if (sel == 0) {
                    PlaySoundCue(2);
                    carByte = g_CarTable[g_PlayerCarIndex].tireCompound;
                    D_8019C794 = &D_80082574;
                    GameMenuBusy = -1;
                        g_UiScriptProgress2 = 0;
                        g_MenuSubCursor = carByte;
                        return;
                }
                if (sel == 1) {
                    if (g_CarModelAsset[8] != 0) {
                        PlaySoundCue(2);
                        carByte = g_CarTable[g_PlayerCarIndex].transmission;
                        D_8019C794 = &D_800825A4;
                        GameMenuBusy = -2;
                        g_UiScriptProgress2 = 0;
                        g_MenuSubCursor = carByte;
                        return;
                    }
                    PlaySoundCue(5);
                    D_8019C794 = &D_80082814;
                    GameMenuBusy = -3;
                    g_UiScriptProgress2 = 0;
                    return;
                }
                if (sel == mode) {
                PlaySoundCue(3);
                GameMenuBusy = 2;
                g_MenuOverlayPattern = 2;
                                return;
                }
                if (sel == 2) {
                    PlaySoundCue(2);
                    GameMenuBusy = 1;
                    g_MenuOverlayPattern = 1;
                    D_8009B324 = -3;
                    g_MenuViewOffsetTarget = 0x3D090;
                }
            } else if (g_PadEdge2 & 0x90) {
                PlaySoundCue(3);
                GameMenuBusy = 2;
                g_MenuOverlayPattern = 2;
            }
        }
        return;
    }

    if (GameMenuBusy < 0) {
        if (GameMenuBusy == -1) {
            if (RunTimedDrawScript(D_8019C794, &g_UiScriptProgress2, 1) != 0) {
                pad = &g_PadEdge2;
                if (*pad & 0x860) {
                    PlaySoundCue(2);
                    GameMenuBusy = -5;
                    g_MenuConfirmTimer = 0x23;
                }
                if (*pad & 0x90) {
                    PlaySoundCue(3);
                    GameMenuBusy = 0;
                }
                if ((*pad & 0x8000) && (g_MenuSubCursor < 4)) {
                    PlaySoundCue(1);
                    g_MenuSubCursor++;
                }
                if (g_PadEdge2 & 0x2000) {
                    if (g_MenuSubCursor != 0) {
                        PlaySoundCue(1);
                        g_MenuSubCursor--;
                    }
                }
                DrawTireCompoundSlider(g_MenuSubCursor, 0);
            }
        } else if (GameMenuBusy == -2) {
            if (RunTimedDrawScript(D_8019C794, &g_UiScriptProgress2, 1) != 0) {
                pad = &g_PadEdge2;
                if (*pad & 0x860) {
                    PlaySoundCue(2);
                    GameMenuBusy = -6;
                    g_MenuConfirmTimer = 0x23;
                    g_CarTable[g_PlayerCarIndex].transmission = g_MenuSubCursor;
                    D_801E438A[g_PlayerCarIndex * 8] = g_MenuSubCursor;
                }
                if (*pad & 0x90) {
                    PlaySoundCue(3);
                    GameMenuBusy = 0;
                }
                if ((*pad & 0x8000) && (g_MenuSubCursor != 0)) {
                    PlaySoundCue(1);
                    g_MenuSubCursor = 0;
                }
                if (g_PadEdge2 & 0x2000) {
                    if (g_MenuSubCursor == 0) {
                        PlaySoundCue(1);
                        g_MenuSubCursor = 1;
                    }
                }
                DrawMenuCursorBox((g_MenuSubCursor != 0) ? 0xDA : 0xB8, 0x68, 0x20, 0x20, 0);
                func_80046A2C(ot, 0xC2, 0x70, 0xC, 0x10, 0x60, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                func_80046A2C(ot, 0xE3, 0x70, 0x10, 0x10, 0x6C, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                func_80048B88(0xB8, 0x68, 0x20, 0x20, 0x95, 0x25, 0x1E, 0, 0, 0, &g_MenuBlankCaption);
                func_80048B88(0xDA, 0x68, 0x20, 0x20, 0x1E, 0x8E, 0x95, 0, 0, 0, &g_MenuBlankCaption);
            }
        } else if (GameMenuBusy == -3) {
            RunTimedDrawScript(D_8019C794, &g_UiScriptProgress2, 0);
            if (RunTimedDrawScript(&g_UiChromeScript2, &g_UiScriptProgress2, 1) != 0) {
                if (g_PadEdge2 & 0x860) {
                    GameMenuBusy = -4;
                }
                if (g_PadEdge2 & 0x90) {
                    GameMenuBusy = -4;
                }
            }
        } else if (GameMenuBusy == -4) {
            RunTimedDrawScript(D_8019C794, &g_UiScriptProgress2, -1);
            RunTimedDrawScript(&g_UiChromeScript2, &g_UiScriptProgress2, 0);
            if (g_UiScriptProgress2 <= 0) {
                GameMenuBusy = 0;
            }
        } else if (GameMenuBusy == -5) {
            if (g_MenuConfirmTimer <= 0) {
                RunTimedDrawScript(D_8019C794, &g_UiScriptProgress2, -1);
                if (g_UiScriptProgress2 <= 0) {
                    GameMenuBusy = 0;
                    g_CarTable[g_PlayerCarIndex].tireCompound = g_MenuSubCursor;
                    D_801E4389[g_PlayerCarIndex * 8] = g_MenuSubCursor;
                }
            } else {
                g_MenuConfirmTimer -= 1;
                RunTimedDrawScript(D_8019C794, &g_UiScriptProgress2, 1);
                DrawTireCompoundSlider(g_MenuSubCursor, 1);
            }
        } else if (GameMenuBusy == -6) {
            if (g_MenuConfirmTimer <= 0) {
                RunTimedDrawScript(D_8019C794, &g_UiScriptProgress2, -1);
                if (g_UiScriptProgress2 <= 0) {
                    GameMenuBusy = 0;
                }
            } else {
                g_MenuConfirmTimer -= 1;
                RunTimedDrawScript(D_8019C794, &g_UiScriptProgress2, 1);
                DrawMenuCursorBox((g_MenuSubCursor != 0) ? 0xDA : 0xB8, 0x68, 0x20, 0x20, 1);
                func_80046A2C(ot, 0xC2, 0x70, 0xC, 0x10, 0x60, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                func_80046A2C(ot, 0xE3, 0x70, 0x10, 0x10, 0x6C, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                func_80048B88(0xB8, 0x68, 0x20, 0x20, 0x95, 0x25, 0x1E, 0, 0, 0, &g_MenuBlankCaption);
                func_80048B88(0xDA, 0x68, 0x20, 0x20, 0x1E, 0x8E, 0x95, 0, 0, 0, &g_MenuBlankCaption);
            }
        }
        DrawFadingMenuSprites(g_UiScriptProgress, mode, g_RankingOption);
        RunTimedDrawScript(cmdList, &g_UiScriptProgress, 0);
        RunTimedDrawScript(&g_UiChromeScript, &g_UiScriptProgress, 1);
        return;
    }

    g_MenuHandlerIndex = -1;
    g_MenuHandlerIndex2 = 5;
    RunTimedDrawScript(cmdList, &g_UiScriptProgress, -1);
    RunTimedDrawScript(&g_UiChromeScript, &g_UiScriptProgress, 0);
    DrawFadingMenuSprites(g_UiScriptProgress, mode, g_RankingOption);
    if (g_UiScriptProgress <= 0) {
        switch (GameMenuBusy) {
        case 1:
            if (g_MenuViewOffset <= 0x3D08F) {
                return;
            }
            g_MenuScreen = 6;
            g_MenuHandlerIndex = 6;
            break;
        case 2:
            g_MenuScreen = 4;
            g_MenuHandlerIndex = 4;
            g_RankingOption = 0;
            break;
        }
        g_UiScriptProgress = 0;
        GameMenuBusy = 0;
    }
}
