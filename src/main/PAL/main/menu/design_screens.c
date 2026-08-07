#include "common.h"
#include "game/audio.h"
#include "game/car.h"
#include "game/menu.h"
#include "game/race.h"
#include "game/render.h"
#include "game/state.h"

extern void ComposeSampleTeamLogo(s32, s32);
extern void RampTeamLogoCanvas(s32, s32);
extern void DrawLogoSamplePanel(s32, s32);

void UpdateLogoSampleScreen(void) {
    s32 v0;
    s32 t;
    s32 pl;

    g_MenuAltLayout = 0;
    ComposeSampleTeamLogo(D_80082EA4, D_80082EA8);
    DrawTeamLogoCanvas(1, 0);
    v0 = GameMenuBusy;
    if (v0 == 0) {
        RampTeamLogoCanvas(-10, 0);
        DrawLogoSamplePanel(-1, D_80082EB0 + 1);
        RunTimedDrawScript(D_8009F0B0, &g_UiScriptProgress2, -1);
        DrawFadingMenuSprites(g_UiScriptProgress, 2, D_8019C770);
        RunTimedDrawScript(&D_80081CA4, &g_UiScriptProgress, 0);
        if (RunTimedDrawScript(&g_UiChromeScript, &g_UiScriptProgress, 1) == 0) return;
        if (g_UiScriptProgress2 > 0) return;
        g_MenuOverlayPattern = -1;
        if (g_PadPressed & PAD_UP) {
            s32 n, c;
            PlaySoundCue(1);
            c = D_8019C770;
            n = 2;
            if (c > 0) n = c - 1;
            D_8019C770 = n;
        }
        if (g_PadPressed & PAD_DOWN) {
            s32 n, c;
            PlaySoundCue(1);
            c = D_8019C770;
            n = 0;
            if (c < 2) n = c + 1;
            D_8019C770 = n;
        }
        if (g_PadPressed & PAD_CONFIRM) {
            pl = D_8019C770;
            if (pl == 0) {
                PlaySoundCue(2);
                GameMenuBusy = -1;
                g_UiScriptProgress2 = 0;
                D_8009F0B0 = &D_80082634;
                D_80082EB0 = D_80082EA4;
            } else if (pl == 1) {
                PlaySoundCue(2);
                GameMenuBusy = -2;
                g_UiScriptProgress2 = 0;
                D_8009F0B0 = &D_80082664;
                D_80082EB0 = D_80082EA8;
            } else if (pl == 2) {
                PlaySoundCue(3);
                GameMenuBusy = 1;
                g_MenuOverlayPattern = 2;
            }
        } else if (g_PadPressed & PAD_CANCEL) {
            PlaySoundCue(3);
            GameMenuBusy = 1;
            g_MenuOverlayPattern = 2;
        }
        return;
    }

    if (v0 < 0) {
        RampTeamLogoCanvas(10, 0);
        if (GameMenuBusy == -1) {
            if (RunTimedDrawScript(D_8009F0B0, &g_UiScriptProgress2, 1) != 0) {
                u16 *p = &g_PadPressed;
                if (*p & 0x860) {
                    PlaySoundCue(2);
                    GameMenuBusy = 0;
                    D_80082EB0 = D_80082EA4;
                }
                if (*p & 0x90) {
                    PlaySoundCue(3);
                    GameMenuBusy = 0;
                    D_80082EA4 = D_80082EB0;
                }
                if (*p & 0x8000) {
                    s32 n, c;
                    PlaySoundCue(1);
                    c = D_80082EA4;
                    n = 0x13;
                    if (c > 0) n = c - 1;
                    D_80082EA4 = n;
                }
                if (g_PadPressed & PAD_RIGHT) {
                    s32 n, c;
                    PlaySoundCue(1);
                    c = D_80082EA4;
                    n = 0;
                    if (c < 19) n = c + 1;
                    D_80082EA4 = n;
                }
            }
            t = D_80082EA4;
        } else {
            if (RunTimedDrawScript(D_8009F0B0, &g_UiScriptProgress2, 1) != 0) {
                u16 *p = &g_PadPressed;
                if (*p & 0x860) {
                    PlaySoundCue(2);
                    GameMenuBusy = 0;
                    D_80082EB0 = D_80082EA8;
                }
                if (*p & 0x90) {
                    PlaySoundCue(3);
                    GameMenuBusy = 0;
                    D_80082EA8 = D_80082EB0;
                }
                if (*p & 0x8000) {
                    s32 n, c;
                    PlaySoundCue(1);
                    c = D_80082EA8;
                    n = 0x13;
                    if (c > 0) n = c - 1;
                    D_80082EA8 = n;
                }
                if (g_PadPressed & PAD_RIGHT) {
                    s32 n, c;
                    PlaySoundCue(1);
                    c = D_80082EA8;
                    n = 0;
                    if (c < 19) n = c + 1;
                    D_80082EA8 = n;
                }
            }
            t = D_80082EA8;
        }
        DrawLogoSamplePanel(1, t + 1);
        DrawFadingMenuSprites(g_UiScriptProgress, 2, D_8019C770);
        RunTimedDrawScript(&D_80081CA4, &g_UiScriptProgress, 0);
        RunTimedDrawScript(&g_UiChromeScript, &g_UiScriptProgress, 1);
        return;
    }

    g_MenuHandlerIndex = -1;
    g_MenuHandlerIndex2 = 8;
    DrawLogoSamplePanel(-1, 0);
    RunTimedDrawScript(&D_80081CA4, &g_UiScriptProgress, -1);
    RunTimedDrawScript(&g_UiChromeScript, &g_UiScriptProgress, 0);
    DrawFadingMenuSprites(g_UiScriptProgress, 2, D_8019C770);
    if (g_UiScriptProgress <= 0) {
        g_MenuScreen = 7;
        g_MenuHandlerIndex = 7;
        D_8019C770 = 0;
        g_UiScriptProgress = 0;
        GameMenuBusy = 0;
    }
}

s32 DrawTeamNameScreen(s32 step) {
    s32 value;

    if (step == 0) {
        D_8009B2E0 = 0;
        return;
    }

    if (step > 0) {
        value = step + D_8009B2E0;
        D_8009B2E0 = value;
        if (value >= 0x1FD) {
            D_8009B2E0 = 0x1FC;
        }
    } else {
        value = step + D_8009B2E0;
        D_8009B2E0 = value;
        if (value < 0) {
            D_8009B2E0 = 0;
        }
    }

    return D_8009B2E0;
}

void UploadTeamNameTexture(void *a, s32 b);

void UpdateTeamNameScreen(void) {
    u16 pad;
    register s32 newdepth asm("$2");

    g_MenuAltLayout = g_MenuAltLayoutSetting;
    DrawTeamNameCharModel();
    if (!(GameMenuBusy != 0)) {

    DrawTeamNameEntry(1, GameMenuCursor);
    if (RunTimedDrawScript(&D_80081D34, &g_UiScriptProgress, 1) == 0) return;
    g_MenuOverlayPattern = -1;

    if (g_TeamNameLength < 6) {
        if ((g_PadPressedRepeat & PAD_DPAD) && GameMenuCursorAnim < 0) {
            if (g_PadPressedRepeat & PAD_UP) { s32 u = GameMenuCursor; GameMenuCursor = (u < 0xB) ? u + 0x21 : u - 0xB; }
            if (g_PadPressedRepeat & PAD_DOWN) { s32 d = GameMenuCursor; GameMenuCursor = (d < 0x21) ? d + 0xB : d - 0x21; }
            if (g_PadPressedRepeat & PAD_LEFT) { s32 l = GameMenuCursor; GameMenuCursor = (l % 11 != 0) ? l - 1 : l + 0xA; }
            if (g_PadPressedRepeat & PAD_RIGHT) {
                s32 r;
                s32 rn;
                r = GameMenuCursor;
                rn = r + 1;
                if (rn % 11 == 0) newdepth = r - 0xA; else newdepth = rn;
                GameMenuCursor = newdepth;
            }
            g_MenuViewAngleTarget = 0;
            g_MenuViewAngle = 0x3E8000;
            GameMenuCursorAnim = GameMenuCursor;
            PlaySoundCue(1);

        }
    } else {
        if ((g_PadPressedRepeat & (PAD_LEFT | PAD_RIGHT)) && GameMenuCursorAnim < 0) {
            s32 nc = (GameMenuCursor == 0x2A) ? 0x2B : 0x2A;
            GameMenuCursor = nc;
            g_MenuViewAngleTarget = 0;
            g_MenuViewAngle = 0x3E8000;
            GameMenuCursorAnim = nc;
            PlaySoundCue(1);
        }
    }
    pad = g_PadPressed;
    if (pad & 0x860) {
    {
        s32 c = GameMenuCursor;
        if (c == 0x2A) goto pop;
        if (!(c != 0x2B)) {
    PlaySoundCue(3);
    GameMenuBusy = 1;
    g_MenuOverlayPattern = 2;
    g_MenuViewOffsetTarget = 0x3D090;
    return;
    }
    }

    {
        u32 d;
        PlaySoundCue(2);
        g_TeamNameChars[g_TeamNameLength] = (u8)GameMenuCursor;
        d = g_TeamNameLength;
        if (d >= 5) GameMenuCursor = 0x2B;
        if (d >= 7) newdepth = d; else newdepth = d + 1;
    }
    } else {

    if (!(pad & 0x90)) return;
pop:
    if (g_TeamNameLength == 0) return;
    PlaySoundCue(4);
    {
        newdepth = 0xA;
        g_TeamNameChars[g_TeamNameLength] = newdepth;
    }
    newdepth = g_TeamNameLength - 1;
    }
    g_TeamNameLength = newdepth;
    return;
    }

    g_MenuHandlerIndex = -1;
    g_MenuHandlerIndex2 = 9;
    DrawTeamNameEntry(-1, GameMenuCursor);
    RunTimedDrawScript(&D_80081D34, &g_UiScriptProgress, -1);
    if (g_UiScriptProgress > 0) return;
    if (0x3D08F < g_MenuViewOffset) {
        g_MenuScreen = 6;
        g_MenuHandlerIndex = 6;
        UploadTeamNameTexture(&g_TeamNameChars, g_TeamNameLength);
        g_UiScriptProgress = 0;
        GameMenuBusy = 0;
    }
}

s32 DrawPaintColorScreen(s32 step) {
    s32 value;

    if (step == 0) {
        D_8009B2E4 = 0;
        return;
    }

    if (step > 0) {
        value = step + D_8009B2E4;
        D_8009B2E4 = value;
        if (value >= 0x1FD) {
            D_8009B2E4 = 0x1FC;
        }
    } else {
        value = step + D_8009B2E4;
        D_8009B2E4 = value;
        if (value < 0) {
            D_8009B2E4 = 0;
        }
    }

    return D_8009B2E4;
}

void DrawBrowseArrows(s32, s32, s32, s32);

void UpdatePaintColorScreen(void) {
    g_MenuAltLayout = g_MenuAltLayoutSetting;
    DrawMenuCarView();

    if (GameMenuBusy == 0) {
        DrawPaintColorPalette(&g_UiScriptProgress2, -1, D_80082EB4);
        DrawBrowseArrows(-1, 0, 1, 1);
        DrawFadingMenuSprites(g_UiScriptProgress, 2, D_801F17A0);
        RunTimedDrawScript(&D_80082010, &g_UiScriptProgress, 0);
        if (RunTimedDrawScript(&g_UiChromeScript, &g_UiScriptProgress, 1) == 0) {
            return;
        }
        if (g_UiScriptProgress2 > 0) {
            return;
        }
        g_MenuOverlayPattern = -1;
        if (g_PadPressed & PAD_UP) {
            PlaySoundCue(1);
            D_801F17A0 = D_801F17A0 > 0 ? D_801F17A0 - 1 : 2;
        }
        if (g_PadPressed & PAD_DOWN) {
            PlaySoundCue(1);
            D_801F17A0 = D_801F17A0 < 2 ? D_801F17A0 + 1 : 0;
        }
        {
            u16 f = g_PadPressed;
            if (f & 0x860) {
                s32 sel = D_801F17A0;
                s32 val;
                if (sel == 0) {
                    PlaySoundCue(2);
                    val = g_CarTable[g_PlayerCarIndex].paintColor1;
                    GameMenuBusy = -1;
                    g_UiScriptProgress2 = 0;
                    D_80082EB4 = val;
                } else if (sel == 1) {
                    PlaySoundCue(2);
                    val = g_CarTable[g_PlayerCarIndex].paintColor2;
                    GameMenuBusy = -2;
                    g_UiScriptProgress2 = 0;
                    D_80082EB4 = val;
                } else if (sel == 2) {
                    PlaySoundCue(3);
                    GameMenuBusy = 1;
                    g_MenuOverlayPattern = 2;
                    g_MenuViewOffsetTarget = 0x3D090;
                }
            } else if (f & 0x90) {
                PlaySoundCue(3);
                GameMenuBusy = 3;
                g_MenuOverlayPattern = 2;
                g_MenuViewOffsetTarget = 0x3D090;
            }
        }
        return;
    }

    if (GameMenuBusy < 0) {
        if (DrawPaintColorPalette(&g_UiScriptProgress2, 1, D_80082EB4) != 0) {
            if (g_PadPressedRepeat & PAD_LEFT) {
                PlaySoundCue(1);
                D_80082EB4 = D_80082EB4 > 0 ? D_80082EB4 - 1 : 0x11;
            }
            if (g_PadPressedRepeat & PAD_RIGHT) {
                PlaySoundCue(1);
                D_80082EB4 = D_80082EB4 < 17 ? D_80082EB4 + 1 : 0;
            }
            if (GameMenuBusy == -1) {
                u16 *btn = &g_PadPressed;
                if (*btn & 0x860) {
                    PlaySoundCue(2);
                    g_CarTable[g_PlayerCarIndex].paintColor1 = D_80082EB4;
                    g_TimeAttackCars[g_PlayerCarIndex].paintColor1 = D_80082EB4;
                    g_TimeAttackCars[g_PlayerCarIndex].paintColor2 = g_CarTable[g_PlayerCarIndex].paintColor2;
                    GameMenuBusy = 0;
                }
                if (*btn & 0x90) {
                    PlaySoundCue(3);
                    D_80082EB4 = g_CarTable[g_PlayerCarIndex].paintColor1;
                    GameMenuBusy = 0;
                }
                SetBodyColor1(D_80082EB4);
            } else {
                u16 *btn = &g_PadPressed;
                if (*btn & 0x860) {
                    PlaySoundCue(2);
                    g_CarTable[g_PlayerCarIndex].paintColor2 = D_80082EB4;
                    g_TimeAttackCars[g_PlayerCarIndex].paintColor1 = g_CarTable[g_PlayerCarIndex].paintColor1;
                    g_TimeAttackCars[g_PlayerCarIndex].paintColor2 = D_80082EB4;
                    GameMenuBusy = 0;
                }
                if (*btn & 0x90) {
                    PlaySoundCue(3);
                    D_80082EB4 = g_CarTable[g_PlayerCarIndex].paintColor2;
                    GameMenuBusy = 0;
                }
                SetBodyColor2(D_80082EB4);
            }
        }

        DrawBrowseArrows(1, 0, 1, 1);
        DrawFadingMenuSprites(g_UiScriptProgress, 2, D_801F17A0);
        RunTimedDrawScript(&D_80082010, &g_UiScriptProgress, 0);
        RunTimedDrawScript(&g_UiChromeScript, &g_UiScriptProgress, 1);
        return;
    }

    g_MenuHandlerIndex = -1;
    g_MenuHandlerIndex2 = 10;
    RunTimedDrawScript(&D_80082010, &g_UiScriptProgress, -1);
    RunTimedDrawScript(&g_UiChromeScript, &g_UiScriptProgress, 0);
    DrawFadingMenuSprites(g_UiScriptProgress, 2, D_801F17A0);
    if (g_UiScriptProgress <= 0) {
        g_MenuScreen = 6;
        g_MenuHandlerIndex = 6;
        D_801F17A0 = 0;
        g_UiScriptProgress = 0;
        GameMenuBusy = 0;
    }
}

s32 DrawCarShopScreen(s32 step) {
    s32 value;
    s32 limit;
    s32 amount;
    s32 phase;
    s32 channel;

    if (step == 0) {
        D_8009B2E8 = 0;
        return;
    }

    if (step > 0) {
        value = D_8009B2E8 + step;
        D_8009B2E8 = value;
        if (value >= 0x1FD) {
            D_8009B2E8 = 0x1FC;
        }
        value = 0;
    } else {
        value = D_8009B2E8 + step;
        D_8009B2E8 = value;
        if (value < 0) {
            D_8009B2E8 = 0;
        }

        value = D_8009B2E8;
        limit = 0x1FC;
        limit -= value;
        value = (u32)(limit * limit) >> 0xB;
    }

    amount = value << 16;
    amount >>= 16;
    limit = g_PlayerCarIndex;
    phase = (u8)((u32)D_8009B2E8 / 4);
    channel = limit;
    DrawCarEngineSpec(amount, phase, channel);

    return D_8009B2E8;
}
void UpdateCarListCursor(void) {
    s32 index;
    CarEntry *entry;

    switch (0) { default:
    if (D_8009B33C != 0) {
        g_PrevOwnedCarIndex = -1;
        index = g_CarListCursor - 1;
        if (index >= 0) {
            entry = &g_CarTable[index];
            while (index >= 0) {
                if (entry->enabled == 0) {
                    g_PrevOwnedCarIndex = index;
                    break;
                }
                index--;
                entry--;
            }
        }
    } else {
        g_PrevOwnedCarIndex = -1;
        index = g_CarListCursor - 1;
        if (index >= 0) {
        backward_loop:
            {
                s32 value = GetCarUnlockLevel(index);
                if (g_CarTable[index].enabled == 0) {
                    s32 progression = g_RaceProgress->maxClassReached;
                    if (progression < 4) {
                        if ((progression + 1) < value) {
                            index--;
                            goto backward_check;
                        }
                        g_PrevOwnedCarIndex = index;
                        break;
                    }
                    if (progression >= value) {
                        g_PrevOwnedCarIndex = index;
                        break;
                    }
                }
                index--;
            }
        backward_check:
            if (index >= 0) {
                goto backward_loop;
            }
        }
    }

    }
    if (D_8009B33C != 0) {
        g_NextOwnedCarIndex = -1;
        index = g_CarListCursor + 1;
        if (index < 13) {
            entry = &g_CarTable[index];
            while (index < 13) {
                if (entry->enabled == 0) {
                    g_NextOwnedCarIndex = index;
                    break;
                }
                index++;
                entry++;
            }
        }
    } else {
        g_NextOwnedCarIndex = -1;
        index = g_CarListCursor + 1;
        if (index < 13) {
        forward_loop:
            switch (0) { default:
            {
                s32 value = GetCarUnlockLevel(index);
                if (g_CarTable[index].enabled == 0) {
                    s32 progression = g_RaceProgress->maxClassReached;
                    if (progression < 4) {
                        if ((progression + 1) < value) {
                            index++;
                            break;
                        }
                        g_NextOwnedCarIndex = index;
                        return;
                    }
                    if (progression >= value) {
                        g_NextOwnedCarIndex = index;
                        return;
                    }
                }
                index++;
            }
            }
            if (index < 13) {
                goto forward_loop;
            }
        }
    }

    return;
}
