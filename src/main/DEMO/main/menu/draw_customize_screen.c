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

s32 DrawCustomizeScreen(s32 step) {
    s32 value;

    if (step == 0) {
        g_CustomizeFadeAccum = 0;
        return;
    }

    if (step > 0) {
        value = step + g_CustomizeFadeAccum;
        g_CustomizeFadeAccum = value;
        if (value >= 0x1FD) {
            g_CustomizeFadeAccum = 0x1FC;
        }
        value = 0;
    } else {
        s32 limit;
        u32 product;

        value = step + g_CustomizeFadeAccum;
        g_CustomizeFadeAccum = value;
        limit = 0x1FC;
        if (value < 0) {
            g_CustomizeFadeAccum = 0;
        }
        limit = limit - g_CustomizeFadeAccum;
        product = limit * limit;
        value = product / 2048;
    }

    DrawCarEngineSpec((s16)value, (g_CustomizeFadeAccum / 4U) & 0xFF, g_PlayerCarIndex);
    return g_CustomizeFadeAccum;
}

/* Verbatim identical to PAL (confirmed via standalone compile + raw-byte
 * comparison against the real demo bytes: every differing word is a
 * genuinely relocatable lui/jal/lw/sw/addiu, none a literal constant). */
void UpdateCustomizeScreen(void) {
    void *ot;
    s32 mode;
    s32 lowMode;
    u8 *cmdList;
    u16 *pad;
    s32 sel;

    ot = SCRATCH_OT_BASE_AS(void);
    g_MenuAltLayout = g_MenuAltLayoutSetting;
    DrawCarNamePlate(g_CarNamePlateStep, g_MenuPlateCarIndex, 0);
    mode = 2;
    DrawMenuCarView();
    if (g_GrandPrixMode != 0) {
        mode = 3;
    }
    cmdList = &g_CustomizeMenuScriptTimeAttack;
    if (g_GrandPrixMode != 0) {
        cmdList = &g_CustomizeMenuScriptGp;
    }

    if (GameMenuBusy == 0) {
        g_CarSpecGraphStep = 3;
        RunTimedDrawScript(g_CustomizePopupScript, &g_UiScriptProgress2, -1);
        lowMode = mode & 0xFF;
        DrawFadingMenuSprites(g_UiScriptProgress, lowMode, g_RankingOption);
        RunTimedDrawScript(cmdList, &g_UiScriptProgress, 0);
        if ((RunTimedDrawScript(&g_UiChromeScript, &g_UiScriptProgress, 1) != 0) && (g_UiScriptProgress2 <= 0)) {
            g_MenuOverlayPattern = -1;
            if (g_PadPressed & PAD_UP) {
                PlaySoundCue(1);
                g_RankingOption = (g_RankingOption > 0) ? g_RankingOption - 1 : lowMode;
            }
            if (g_PadPressed & PAD_DOWN) {
                PlaySoundCue(1);
                g_RankingOption = (g_RankingOption < mode) ? g_RankingOption + 1 : 0;
            }
            if (g_PadPressed & PAD_CONFIRM) {
                u8 carByte;

                sel = g_RankingOption;
                if (sel == 0) {
                    PlaySoundCue(2);
                    carByte = g_CarTable[g_PlayerCarIndex].tireCompound;
                    g_CustomizePopupScript = &g_MenuDialogPanelUpperScript;
                    GameMenuBusy = -1;
                        g_UiScriptProgress2 = 0;
                        g_MenuSubCursor = carByte;
                        return;
                }
                if (sel == 1) {
                    if (g_CarModelAsset->transmissionAvailable != 0) {
                        PlaySoundCue(2);
                        carByte = g_CarTable[g_PlayerCarIndex].transmission;
                        g_CustomizePopupScript = &g_MenuDialogPanelLowerScript;
                        GameMenuBusy = -2;
                        g_UiScriptProgress2 = 0;
                        g_MenuSubCursor = carByte;
                        return;
                    }
                    PlaySoundCue(5);
                    g_CustomizePopupScript = &g_TransmissionUnavailableScript;
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
                    g_CarSpecGraphStep = -3;
                    g_MenuViewOffsetTarget = 0x3D090;
                }
            } else if (g_PadPressed & PAD_CANCEL) {
                PlaySoundCue(3);
                GameMenuBusy = 2;
                g_MenuOverlayPattern = 2;
            }
        }
        return;
    }

    if (GameMenuBusy < 0) {
        if (GameMenuBusy == -1) {
            if (RunTimedDrawScript(g_CustomizePopupScript, &g_UiScriptProgress2, 1) != 0) {
                pad = &g_PadPressed;
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
                if (g_PadPressed & PAD_RIGHT) {
                    if (g_MenuSubCursor != 0) {
                        PlaySoundCue(1);
                        g_MenuSubCursor--;
                    }
                }
                DrawTireCompoundSlider(g_MenuSubCursor, 0);
            }
        } else if (GameMenuBusy == -2) {
            if (RunTimedDrawScript(g_CustomizePopupScript, &g_UiScriptProgress2, 1) != 0) {
                pad = &g_PadPressed;
                if (*pad & 0x860) {
                    PlaySoundCue(2);
                    GameMenuBusy = -6;
                    g_MenuConfirmTimer = 0x23;
                    g_CarTable[g_PlayerCarIndex].transmission = g_MenuSubCursor;
                    g_TimeAttackCarTransmissions[g_PlayerCarIndex * 8] = g_MenuSubCursor;
                }
                if (*pad & 0x90) {
                    PlaySoundCue(3);
                    GameMenuBusy = 0;
                }
                if ((*pad & 0x8000) && (g_MenuSubCursor != 0)) {
                    PlaySoundCue(1);
                    g_MenuSubCursor = 0;
                }
                if (g_PadPressed & PAD_RIGHT) {
                    if (g_MenuSubCursor == 0) {
                        PlaySoundCue(1);
                        g_MenuSubCursor = 1;
                    }
                }
                DrawMenuCursorBox((g_MenuSubCursor != 0) ? 0xDA : 0xB8, 0x68, 0x20, 0x20, 0);
                DrawSprite(ot, 0xC2, 0x70, 0xC, 0x10, 0x60, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                DrawSprite(ot, 0xE3, 0x70, 0x10, 0x10, 0x6C, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                GameDrawMenuButton(0xB8, 0x68, 0x20, 0x20, 0x95, 0x25, 0x1E, 0, 0, 0, &g_MenuBlankCaption);
                GameDrawMenuButton(0xDA, 0x68, 0x20, 0x20, 0x1E, 0x8E, 0x95, 0, 0, 0, &g_MenuBlankCaption);
            }
        } else if (GameMenuBusy == -3) {
            RunTimedDrawScript(g_CustomizePopupScript, &g_UiScriptProgress2, 0);
            if (RunTimedDrawScript(&g_UiChromeScript2, &g_UiScriptProgress2, 1) != 0) {
                if (g_PadPressed & PAD_CONFIRM) {
                    GameMenuBusy = -4;
                }
                if (g_PadPressed & PAD_CANCEL) {
                    GameMenuBusy = -4;
                }
            }
        } else if (GameMenuBusy == -4) {
            RunTimedDrawScript(g_CustomizePopupScript, &g_UiScriptProgress2, -1);
            RunTimedDrawScript(&g_UiChromeScript2, &g_UiScriptProgress2, 0);
            if (g_UiScriptProgress2 <= 0) {
                GameMenuBusy = 0;
            }
        } else if (GameMenuBusy == -5) {
            if (g_MenuConfirmTimer <= 0) {
                RunTimedDrawScript(g_CustomizePopupScript, &g_UiScriptProgress2, -1);
                if (g_UiScriptProgress2 <= 0) {
                    GameMenuBusy = 0;
                    g_CarTable[g_PlayerCarIndex].tireCompound = g_MenuSubCursor;
                    g_TimeAttackCarTires[g_PlayerCarIndex * 8] = g_MenuSubCursor;
                }
            } else {
                g_MenuConfirmTimer -= 1;
                RunTimedDrawScript(g_CustomizePopupScript, &g_UiScriptProgress2, 1);
                DrawTireCompoundSlider(g_MenuSubCursor, 1);
            }
        } else if (GameMenuBusy == -6) {
            if (g_MenuConfirmTimer <= 0) {
                RunTimedDrawScript(g_CustomizePopupScript, &g_UiScriptProgress2, -1);
                if (g_UiScriptProgress2 <= 0) {
                    GameMenuBusy = 0;
                }
            } else {
                g_MenuConfirmTimer -= 1;
                RunTimedDrawScript(g_CustomizePopupScript, &g_UiScriptProgress2, 1);
                DrawMenuCursorBox((g_MenuSubCursor != 0) ? 0xDA : 0xB8, 0x68, 0x20, 0x20, 1);
                DrawSprite(ot, 0xC2, 0x70, 0xC, 0x10, 0x60, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                DrawSprite(ot, 0xE3, 0x70, 0x10, 0x10, 0x6C, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                GameDrawMenuButton(0xB8, 0x68, 0x20, 0x20, 0x95, 0x25, 0x1E, 0, 0, 0, &g_MenuBlankCaption);
                GameDrawMenuButton(0xDA, 0x68, 0x20, 0x20, 0x1E, 0x8E, 0x95, 0, 0, 0, &g_MenuBlankCaption);
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
