#include "common.h"
#include "game/asset.h"
#include "game/audio.h"
#include "game/car.h"
#include "game/asset_internal.h"
#include "game/menu.h"
#include "game/menu_scripts_internal.h"
#include "game/save_internal.h"
#include "game/race.h"
#include "game/render.h"
#include "game/scratchpad.h"
#include "game/state.h"


void UpdateRankingScreen(void) {
    s32 state;

    g_MenuAltLayout = 0;
    DrawMenuCourseView();
    DrawMenuLightBurst(-9);
    state = GameMenuBusy;
    if (state == 0) {
        g_UiScriptProgress2 = 0;
        GameMenuBusy = -1;
        DrawFadingMenuSprites(0, 2, g_RankingCursor);
        RunTimedDrawScript(&g_RankingMenuScript, &g_UiScriptProgress2, 1);
    } else if (state < 0) {
        switch (state) {
        case -1:
            DrawFadingMenuSprites(g_UiScriptProgress2, 2, g_RankingCursor);
            if (RunTimedDrawScript(&g_RankingMenuScript, &g_UiScriptProgress2, 1) != 0) {
                g_MenuOverlayPattern = -1;
                if (g_PadPressed & PAD_UP) {
                    PlaySoundCue(1);
                    g_RankingCursor = (g_RankingCursor > 0) ? g_RankingCursor - 1 : 2;
                }
                if (g_PadPressed & PAD_DOWN) {
                    PlaySoundCue(1);
                    g_RankingCursor = (g_RankingCursor < 2) ? g_RankingCursor + 1 : 0;
                }
                {
                    s32 flags = g_PadPressed;
                    if (flags & 0x860) {
                        s32 x = g_RankingCursor;
                        if (x == 0) {
                            PlaySoundCue(2);
                            GameMenuBusy = -2;
                            g_RankingPendingState = -3;
                        } else if (x == 1) {
                            PlaySoundCue(2);
                            GameMenuBusy = -2;
                            g_RankingPendingState = -5;
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
            RunTimedDrawScript(&g_RankingMenuScript, &g_UiScriptProgress2, -1);
            DrawFadingMenuSprites(g_UiScriptProgress2, 2, g_RankingCursor);
            if (g_UiScriptProgress2 > 0) {
                break;
            }
            GameMenuBusy = g_RankingPendingState;
            break;
        case -3:
            if (DrawRankingTable(&g_UiScriptProgress2, 1, 0) == 0) {
                break;
            }
            if (!(g_PadPressed & (PAD_START | PAD_SQUARE | PAD_CROSS | PAD_CIRCLE | PAD_TRIANGLE))) {
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
            if (!(g_PadPressed & (PAD_START | PAD_SQUARE | PAD_CROSS | PAD_CIRCLE | PAD_TRIANGLE))) {
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
    RunTimedDrawScript(&g_RankingPanelScript, &g_UiScriptProgress, 0);
    RunTimedDrawScript(&g_UiChromeScript, &g_UiScriptProgress, 1);
    return;
pos:
    g_MenuHandlerIndex = -1;
    g_MenuHandlerIndex2 = 2;
    RunTimedDrawScript(&g_RankingMenuScript, &g_UiScriptProgress2, -1);
    DrawFadingMenuSprites(g_UiScriptProgress2, 2, g_RankingCursor);
    RunTimedDrawScript(&g_RankingPanelScript, &g_UiScriptProgress, -1);
    RunTimedDrawScript(&g_UiChromeScript, &g_UiScriptProgress, 0);
    if (g_UiScriptProgress > 0) {
        return;
    }
    g_MenuScreen = 1;
    g_MenuHandlerIndex = 1;
    g_RankingCursor = 0;
    g_UiScriptProgress = 0;
    GameMenuBusy = 0;
    DrawTimeAttackPlate(0);
    if (g_CourseIndex >= 4) {
        g_TimeAttackPlateStep = 1;
    } else {
        g_TimeAttackPlateStep = -1;
    }
}

s32 DrawCarSelectScreen(s32 step) {
    u32 *ot = SCRATCH_OT_BASE_AS(u32);
    OrderingTableAddress otAddress;
    s32 p;
    u32 *buf = ot + 1;
    s32 v;
    s32 col;
    s32 xpos;
    s32 mode;
    u8 tex;

    otAddress.pointer = ot;
    p = otAddress.value;

    if (step == 0) {
        g_CarSelectFadeAccum = 0;
        return p;
    }

    if (step > 0) {
        g_CarSelectFadeAccum += step;
        if (g_CarSelectFadeAccum >= 509) {
            g_CarSelectFadeAccum = 508;
        }
    } else {
        g_CarSelectFadeAccum += step;
        if (g_CarSelectFadeAccum < 0) {
            g_CarSelectFadeAccum = 0;
        }
    }

    v = g_CarSelectFadeAccum / 4U;
    col = v & 0xff;
    DrawRectOutline(buf, 0xa3, 0x180, 0x1a, 0x19, col, col, col, 0x20);

    tex = g_CarTable[g_PlayerCarIndex].transmission;
    if (tex != 0) {
        DrawSprite(buf, 0xad, 0x185, 0x10, 0x10, 0x6c, 0x7c, col, col, col,
                      0x244, 0, 1, 0x3b);
        xpos = 0xa5;
    } else {
        DrawSprite(buf, 0xae, 0x185, 0xc, 0x10, 0x60, 0x7c, col, col, col,
                      0x244, 0, 1, 0x3b);
        xpos = 0xa6;
    }

    mode = g_CarModelAsset->gearCount;
    switch (mode) {
    case 4:
        DrawSprite(buf, xpos, 0x185, 8, 0x10, 0x20, 0x18, v & 0xff, v & 0xff,
                      v & 0xff, 0x244, 0, 1, 0x3b);
        break;
    case 5:
        DrawSprite(buf, xpos, 0x185, 8, 0x10, 0x28, 0x18, v & 0xff, v & 0xff,
                      v & 0xff, 0x244, 0, 1, 0x3b);
        break;
    case 6:
        DrawSprite(buf, xpos, 0x185, 8, 0x10, 0x30, 0x18, v & 0xff, v & 0xff,
                      v & 0xff, 0x244, 0, 1, 0x3b);
        break;
    }

    return g_CarSelectFadeAccum;
}

void UpdateOwnedCarNeighbours(void) {
    s32 index;
    CarEntry *ptr;

    g_PrevOwnedCarIndex = -1;
    index = g_PlayerCarIndex - 1;
    if (index >= 0) {
        s32 one = 1;
        ptr = &g_CarTable[index];
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
        ptr = &g_CarTable[index];
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
void RefreshCarUnlockState(void) {
    s32 index;
    s32 value;
    CarEntry *ptr;
    CarEntry *enabledPtr;
    s32 byte;

    g_ShopCarIndex = -1;

    if (g_CarShopUnlockAll != 0) {
        index = 12;
        enabledPtr = &g_CarTable[12];
while (1) {
        byte = enabledPtr->enabled;
        enabledPtr--;
        if (byte == 0) {
            g_ShopCarIndex = index;
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
        ptr = &g_CarTable[index];
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
            g_ShopCarIndex = index;
        }
        index--;
    }
    } while (index >= 0);

}

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

/* This pre-release build has no car-swap-preview gesture yet (PAL's
 * PAD_UP/PAD_DOWN cursor nudge and PAD_LEFT/PAD_RIGHT held-car-swap blocks,
 * normally inside the same chrome-script-done check below, are both simply
 * absent) -- confirmed by a full instruction-stream alignment against PAL's
 * compiled UpdateCarSelectScreen: everything before and after those two
 * blocks matches PAL exactly, register renumbering aside. */
void UpdateCarSelectScreen(void) {
    s32 mode;
    u8 *cmdList;
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
    cmdList = &g_CarSelectMenuScriptTimeAttack;
    if (g_GrandPrixMode != 0) {
        cmdList = &g_CarSelectMenuScriptGp;
    }

    if (GameMenuBusy == 0) {
        g_CarNamePlateStep = 0x14;
        g_CarSpecGraphStep = 3;
        g_MenuPlateCarIndex = g_PlayerCarIndex;
        RunTimedDrawScript(g_CarSelectPopupScript, &g_UiScriptProgress2, -1);
        RunTimedDrawScript(&g_UiChromeScript2, &g_UiScriptProgress2, 0);
        DrawBrowseArrows(
            1, 0, ~g_PrevOwnedCarIndex != 0, ~g_NextOwnedCarIndex != 0);
        {
            s32 initial;

            initial = -1;
            if (g_GrandPrixMode == 0) {
                DrawOwnedCarCounter(1, CountOwnedCars());
            }
            DrawFadingMenuSprites(g_UiScriptProgress, mode, g_CarSelectCursor);
            RunTimedDrawScript(cmdList, &g_UiScriptProgress, 0);
            if ((RunTimedDrawScript(&g_UiChromeScript, &g_UiScriptProgress, 1) !=
                 0) &&
                (g_UiScriptProgress2 <= 0)) {
                g_MenuOverlayPattern = initial;
                UpdateOwnedCarNeighbours();
                RefreshCarUnlockState();
                t = g_MenuViewAngleTarget;
                u = g_MenuViewAngle;
                if (t < u ? (u - t <= 0x493DF) : (t - u <= 0x493DF)) {
                    if (g_CarSwapToIndex < 0) {
                        if (g_PadPressed & PAD_CONFIRM) {
                            s32 choice;

                            choice = g_CarSelectCursor;
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
                                g_CarSpecGraphStep = -3;
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
                                g_CarSpecGraphStep = -3;
                                g_MenuViewOffsetTarget = 0x3D090;
                                return;
                            }
                            if (choice == 2) {
                                s32 car;

                                car = g_ShopCarIndex;
                                if (car != -1) {
                                    s32 base;
                                    s32 prev;

                                    PlaySoundCue(2);
                                    g_CarListCursor = g_ShopCarIndex;
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
                                g_CarSelectPopupScript = &g_CarShopUnavailableScript;
                                GameMenuBusy = car;
                                g_UiScriptProgress2 = 0;
                                return;
                            }
                            if (choice == 3) {
                                s32 unlockLevel;

                                if (g_CarModelAsset->upgradesAvailable != 0) {
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
                                g_CarSelectPopupScript = &g_EngineerShopUnavailableScript;
                                GameMenuBusy = -2;
                                g_UiScriptProgress2 = 0;
                                return;
                            }
                        } else if ((g_PadPressed & PAD_CANCEL) &&
                                   ((g_MenuViewAngle - 0x2710) >
                                    0x120160U)) {
                            PlaySoundCue(3);
                            GameMenuBusy = 5;
                            g_MenuOverlayPattern = 2;
                            g_CarNamePlateStep = -10;
                            g_CarSpecGraphStep = -3;
                            g_MenuViewOffsetTarget = 0x3D090;
                        }
                    }
                }
            }
        }
        return;
    }

    if (GameMenuBusy < 0) {
        RunTimedDrawScript(g_CarSelectPopupScript, &g_UiScriptProgress2, 0);
        if (RunTimedDrawScript(&g_UiChromeScript2, &g_UiScriptProgress2, 1) != 0) {
            if (g_PadPressed & PAD_CONFIRM) {
                GameMenuBusy = 0;
            }
            if (g_PadPressed & PAD_CANCEL) {
                GameMenuBusy = 0;
            }
        }
        DrawBrowseArrows(
            1, 0, ~g_PrevOwnedCarIndex != 0, ~g_NextOwnedCarIndex != 0);
        if (g_GrandPrixMode == 0) {
            DrawOwnedCarCounter(1, CountOwnedCars());
        }
        DrawFadingMenuSprites(g_UiScriptProgress, mode, g_CarSelectCursor);
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
    DrawFadingMenuSprites(g_UiScriptProgress, mode, g_CarSelectCursor);
    if (g_UiScriptProgress <= 0) {
        switch (GameMenuBusy) {
        case 1:
            if ((g_MenuOutgoingScreenProgress > 0) &&
                (g_MenuViewOffset <= 0x3D08F)) {
                return;
            }
            g_SceneId = 9;
            g_CourseIndex &= 3;
            g_RaceProgress->course = g_CourseIndex;
            g_RaceProgress->carIndex = g_PlayerCarIndex;
            g_RaceProgress->classIndex = g_GrandPrixClass;
            if (g_GrandPrixMode != 0) {
                g_RaceProgress->money.value = g_PlayerMoney;
            } else {
                g_RaceProgress->money.value = g_GrandPrixSeries;
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
            g_MenuAltPanelStep = 0;
            g_MenuAltPanelStep2 = 0;
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
            g_CarSelectCursor = 0;
            g_MenuPendingCourseIndex = minusOne;
            g_MenuViewOffset = offset;
            g_MenuViewOffsetTarget = 0;
            g_CourseCardSpin = largeValue;
            g_MenuCourseModelIndex = course;
            g_CourseCardPendingGrade = g_CourseProgress->bestPlace[course & 3];
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
