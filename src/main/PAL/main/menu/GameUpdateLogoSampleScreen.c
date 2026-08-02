#include "common.h"
#include "game/menu.h"
#include "game/render.h"
#include "game/audio.h"
#include "game/car.h"
#include "game/race.h"

s32 GameDrawTeamNameScreen(s32 arg0);
s32 GameDrawPaintColorScreen(s32 arg0);
s32 GameDrawCarShopScreen(s32 arg0);

extern s32 D_80082EA4;
extern s32 D_80082EA8;
extern s32 D_80082EB0;
extern s32 D_8019C770;
extern void *D_8009F0B0;
extern u8 D_80082634;
extern u8 D_80082664;
extern u8 D_80081CA4;
extern u8 g_UiChromeScript asm("D_80082460");

extern void func_8001D338(s32, s32);
extern void func_8004A248(s32, s32);
extern void GameRampTeamLogoCanvas(s32, s32) asm("func_8004B8B4");
extern void GameDrawLogoSamplePanel(s32, s32) asm("func_8004E368");
extern s32 func_800487D8(void *, void *, s32);
extern void func_800489AC(s32, s32, s32);
void GamePlaySoundCue(s32 cue) asm("func_8005D6EC");

void GameUpdateLogoSampleScreen(void) asm("func_800580C8");
void GameUpdateLogoSampleScreen(void) {
    s32 v0;
    s32 t;
    s32 pl;

    g_MenuAltLayout = 0;
    func_8001D338(D_80082EA4, D_80082EA8);
    func_8004A248(1, 0);
    v0 = GameMenuBusy;
    if (v0 == 0) {
        GameRampTeamLogoCanvas(-10, 0);
        GameDrawLogoSamplePanel(-1, D_80082EB0 + 1);
        func_800487D8(D_8009F0B0, &g_UiScriptProgress2, -1);
        func_800489AC(g_UiScriptProgress, 2, D_8019C770);
        func_800487D8(&D_80081CA4, &g_UiScriptProgress, 0);
        if (func_800487D8(&g_UiChromeScript, &g_UiScriptProgress, 1) == 0) return;
        if (g_UiScriptProgress2 > 0) return;
        g_MenuOverlayPattern = -1;
        if (g_PadEdge2 & 0x1000) {
            s32 n, c;
            GamePlaySoundCue(1);
            c = D_8019C770;
            n = 2;
            if (c > 0) n = c - 1;
            D_8019C770 = n;
        }
        if (g_PadEdge2 & 0x4000) {
            s32 n, c;
            GamePlaySoundCue(1);
            c = D_8019C770;
            n = 0;
            if (c < 2) n = c + 1;
            D_8019C770 = n;
        }
        if (g_PadEdge2 & 0x860) {
            pl = D_8019C770;
            if (pl == 0) {
                GamePlaySoundCue(2);
                GameMenuBusy = -1;
                g_UiScriptProgress2 = 0;
                D_8009F0B0 = &D_80082634;
                D_80082EB0 = D_80082EA4;
            } else if (pl == 1) {
                GamePlaySoundCue(2);
                GameMenuBusy = -2;
                g_UiScriptProgress2 = 0;
                D_8009F0B0 = &D_80082664;
                D_80082EB0 = D_80082EA8;
            } else if (pl == 2) {
                GamePlaySoundCue(3);
                GameMenuBusy = 1;
                g_MenuOverlayPattern = 2;
            }
        } else if (g_PadEdge2 & 0x90) {
            GamePlaySoundCue(3);
            GameMenuBusy = 1;
            g_MenuOverlayPattern = 2;
        }
        return;
    }

    if (v0 < 0) {
        GameRampTeamLogoCanvas(10, 0);
        if (GameMenuBusy == -1) {
            if (func_800487D8(D_8009F0B0, &g_UiScriptProgress2, 1) != 0) {
                u16 *p = &g_PadEdge2;
                if (*p & 0x860) {
                    GamePlaySoundCue(2);
                    GameMenuBusy = 0;
                    D_80082EB0 = D_80082EA4;
                }
                if (*p & 0x90) {
                    GamePlaySoundCue(3);
                    GameMenuBusy = 0;
                    D_80082EA4 = D_80082EB0;
                }
                if (*p & 0x8000) {
                    s32 n, c;
                    GamePlaySoundCue(1);
                    c = D_80082EA4;
                    n = 0x13;
                    if (c > 0) n = c - 1;
                    D_80082EA4 = n;
                }
                if (g_PadEdge2 & 0x2000) {
                    s32 n, c;
                    GamePlaySoundCue(1);
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
                    GamePlaySoundCue(2);
                    GameMenuBusy = 0;
                    D_80082EB0 = D_80082EA8;
                }
                if (*p & 0x90) {
                    GamePlaySoundCue(3);
                    GameMenuBusy = 0;
                    D_80082EA8 = D_80082EB0;
                }
                if (*p & 0x8000) {
                    s32 n, c;
                    GamePlaySoundCue(1);
                    c = D_80082EA8;
                    n = 0x13;
                    if (c > 0) n = c - 1;
                    D_80082EA8 = n;
                }
                if (g_PadEdge2 & 0x2000) {
                    s32 n, c;
                    GamePlaySoundCue(1);
                    c = D_80082EA8;
                    n = 0;
                    if (c < 19) n = c + 1;
                    D_80082EA8 = n;
                }
            }
            t = D_80082EA8;
        }
        GameDrawLogoSamplePanel(1, t + 1);
        func_800489AC(g_UiScriptProgress, 2, D_8019C770);
        func_800487D8(&D_80081CA4, &g_UiScriptProgress, 0);
        func_800487D8(&g_UiChromeScript, &g_UiScriptProgress, 1);
        return;
    }

    g_MenuHandlerIndex = -1;
    g_MenuHandlerIndex2 = 8;
    GameDrawLogoSamplePanel(-1, 0);
    func_800487D8(&D_80081CA4, &g_UiScriptProgress, -1);
    func_800487D8(&g_UiChromeScript, &g_UiScriptProgress, 0);
    func_800489AC(g_UiScriptProgress, 2, D_8019C770);
    if (g_UiScriptProgress <= 0) {
        g_MenuScreen = 7;
        g_MenuHandlerIndex = 7;
        D_8019C770 = 0;
        g_UiScriptProgress = 0;
        GameMenuBusy = 0;
    }
}

extern s32 D_8009B2E0;

s32 GameDrawTeamNameScreen(s32 step) asm("func_800586B0");
s32 GameDrawTeamNameScreen(s32 arg0) {
    s32 value;

    if (arg0 == 0) {
        D_8009B2E0 = 0;
        return;
    }

    if (arg0 > 0) {
        value = arg0 + D_8009B2E0;
        D_8009B2E0 = value;
        if (value >= 0x1FD) {
            D_8009B2E0 = 0x1FC;
        }
    } else {
        value = arg0 + D_8009B2E0;
        D_8009B2E0 = value;
        if (value < 0) {
            D_8009B2E0 = 0;
        }
    }

    return D_8009B2E0;
}

extern u32 D_80081D34;

void func_80051D6C(void);
s32 func_8004E724(s32 a, s32 b);
s32 func_800487D8(void *a, void *b, s32 c);
void GameUploadTeamNameTexture(void *a, s32 b) asm("func_8001D530");

void GameUpdateTeamNameScreen(void) asm("func_8005873C");
void GameUpdateTeamNameScreen(void) {
    u16 pad;
    s32 newdepth;

    g_MenuAltLayout = g_MenuAltLayoutSetting;
    func_80051D6C();
    if (GameMenuBusy != 0) goto reopen;

    func_8004E724(1, GameMenuCursor);
    if (func_800487D8(&D_80081D34, &g_UiScriptProgress, 1) == 0) return;
    g_MenuOverlayPattern = -1;

    if (g_TeamNameLength < 6) {
        if ((g_PadEdge & 0xF000) && GameMenuCursorAnim < 0) {
            if (g_PadEdge & 0x1000) { s32 u = GameMenuCursor; GameMenuCursor = (u < 0xB) ? u + 0x21 : u - 0xB; }
            if (g_PadEdge & 0x4000) { s32 d = GameMenuCursor; GameMenuCursor = (d < 0x21) ? d + 0xB : d - 0x21; }
            if (g_PadEdge & 0x8000) { s32 l = GameMenuCursor; GameMenuCursor = (l % 11 != 0) ? l - 1 : l + 0xA; }
            if (g_PadEdge & 0x2000) {
                s32 r;
                /* This pin is load-bearing: removing it changes .text. */
                register s32 res asm("$2");
                s32 rn;
                r = GameMenuCursor;
                rn = r + 1;
                if (rn % 11 == 0) res = r - 0xA; else res = rn;
                GameMenuCursor = res;
            }
            g_MenuViewAngleTarget = 0;
            g_MenuViewAngle = 0x3E8000;
            GameMenuCursorAnim = GameMenuCursor;
            GamePlaySoundCue(1);

        }
    } else {
        if ((g_PadEdge & 0xA000) && GameMenuCursorAnim < 0) {
            s32 nc = (GameMenuCursor == 0x2A) ? 0x2B : 0x2A;
            GameMenuCursor = nc;
            g_MenuViewAngleTarget = 0;
            g_MenuViewAngle = 0x3E8000;
            GameMenuCursorAnim = nc;
            GamePlaySoundCue(1);
        }
    }
after_sound:
    pad = g_PadEdge2;
    if (pad & 0x860) {
    {
        s32 c = GameMenuCursor;
        if (c == 0x2A) goto pop;
        if (c != 0x2B) goto push;
    }
    GamePlaySoundCue(3);
    GameMenuBusy = 1;
    g_MenuOverlayPattern = 2;
    g_MenuViewOffsetTarget = 0x3D090;
    return;

push:
    {
        u32 d;
        GamePlaySoundCue(2);
        g_TeamNameChars[g_TeamNameLength] = (u8)GameMenuCursor;
        d = g_TeamNameLength;
        if (d >= 5) GameMenuCursor = 0x2B;
        if (d >= 7) newdepth = d; else newdepth = d + 1;
    }
    goto set_depth;
    }

    if (!(pad & 0x90)) return;
pop:
    if (g_TeamNameLength == 0) return;
    GamePlaySoundCue(4);
    {
        /* This pin is load-bearing: removing it changes .text. */
        register s32 tv asm("$2");
        tv = 0xA;
        g_TeamNameChars[g_TeamNameLength] = tv;
    }
    newdepth = g_TeamNameLength - 1;
set_depth:
    g_TeamNameLength = newdepth;
    return;

reopen:
    g_MenuHandlerIndex = -1;
    g_MenuHandlerIndex2 = 9;
    func_8004E724(-1, GameMenuCursor);
    func_800487D8(&D_80081D34, &g_UiScriptProgress, -1);
    if (g_UiScriptProgress > 0) return;
    if (0x3D08F < g_MenuViewOffset) {
        g_MenuScreen = 6;
        g_MenuHandlerIndex = 6;
        GameUploadTeamNameTexture(&g_TeamNameChars, g_TeamNameLength);
        g_UiScriptProgress = 0;
        GameMenuBusy = 0;
    }
}

extern s32 D_8009B2E4;

s32 GameDrawPaintColorScreen(s32 step) asm("func_80058B88");
s32 GameDrawPaintColorScreen(s32 arg0) {
    s32 value;

    if (arg0 == 0) {
        D_8009B2E4 = 0;
        return;
    }

    if (arg0 > 0) {
        value = arg0 + D_8009B2E4;
        D_8009B2E4 = value;
        if (value >= 0x1FD) {
            D_8009B2E4 = 0x1FC;
        }
    } else {
        value = arg0 + D_8009B2E4;
        D_8009B2E4 = value;
        if (value < 0) {
            D_8009B2E4 = 0;
        }
    }

    return D_8009B2E4;
}

extern s32 D_80082EB4;
extern u8 D_80082010;
extern s32 D_801F17A0;

void func_8005131C(void);
s32 func_8004F048(void *, s32, s32);
void func_80049418(s32, s32, s32, s32);
void func_800489AC(s32, s32, s32);
s32 func_800487D8(void *, void *, s32);
void func_8001D8C4(s32);
void func_8001DA74(s32);

void GameUpdatePaintColorScreen(void) asm("func_80058C14");
void GameUpdatePaintColorScreen(void) {
    g_MenuAltLayout = g_MenuAltLayoutSetting;
    func_8005131C();

    if (GameMenuBusy == 0) {
        func_8004F048(&g_UiScriptProgress2, -1, D_80082EB4);
        func_80049418(-1, 0, 1, 1);
        func_800489AC(g_UiScriptProgress, 2, D_801F17A0);
        func_800487D8(&D_80082010, &g_UiScriptProgress, 0);
        if (func_800487D8(&g_UiChromeScript, &g_UiScriptProgress, 1) == 0) {
            return;
        }
        if (g_UiScriptProgress2 > 0) {
            return;
        }
        g_MenuOverlayPattern = -1;
        if (g_PadEdge2 & 0x1000) {
            GamePlaySoundCue(1);
            D_801F17A0 = D_801F17A0 > 0 ? D_801F17A0 - 1 : 2;
        }
        if (g_PadEdge2 & 0x4000) {
            GamePlaySoundCue(1);
            D_801F17A0 = D_801F17A0 < 2 ? D_801F17A0 + 1 : 0;
        }
        {
            u16 f = g_PadEdge2;
            if (f & 0x860) {
                s32 sel = D_801F17A0;
                s32 val;
                if (sel == 0) {
                    GamePlaySoundCue(2);
                    val = g_CarTable[g_PlayerCarIndex].paintColor1;
                    GameMenuBusy = -1;
                    g_UiScriptProgress2 = 0;
                    D_80082EB4 = val;
                } else if (sel == 1) {
                    GamePlaySoundCue(2);
                    val = g_CarTable[g_PlayerCarIndex].paintColor2;
                    GameMenuBusy = -2;
                    g_UiScriptProgress2 = 0;
                    D_80082EB4 = val;
                } else if (sel == 2) {
                    GamePlaySoundCue(3);
                    GameMenuBusy = 1;
                    g_MenuOverlayPattern = 2;
                    g_MenuViewOffsetTarget = 0x3D090;
                }
            } else if (f & 0x90) {
                GamePlaySoundCue(3);
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
                GamePlaySoundCue(1);
                D_80082EB4 = D_80082EB4 > 0 ? D_80082EB4 - 1 : 0x11;
            }
            if (g_PadEdge & 0x2000) {
                GamePlaySoundCue(1);
                D_80082EB4 = D_80082EB4 < 17 ? D_80082EB4 + 1 : 0;
            }
            if (GameMenuBusy == -1) {
                u16 *btn = &g_PadEdge2;
                if (*btn & 0x860) {
                    GamePlaySoundCue(2);
                    g_CarTable[g_PlayerCarIndex].paintColor1 = D_80082EB4;
                    g_TimeAttackCars[g_PlayerCarIndex].paintColor1 = D_80082EB4;
                    g_TimeAttackCars[g_PlayerCarIndex].paintColor2 = g_CarTable[g_PlayerCarIndex].paintColor2;
                    GameMenuBusy = 0;
                }
                if (*btn & 0x90) {
                    GamePlaySoundCue(3);
                    D_80082EB4 = g_CarTable[g_PlayerCarIndex].paintColor1;
                    GameMenuBusy = 0;
                }
                func_8001D8C4(D_80082EB4);
            } else {
                u16 *btn = &g_PadEdge2;
                if (*btn & 0x860) {
                    GamePlaySoundCue(2);
                    g_CarTable[g_PlayerCarIndex].paintColor2 = D_80082EB4;
                    g_TimeAttackCars[g_PlayerCarIndex].paintColor1 = g_CarTable[g_PlayerCarIndex].paintColor1;
                    g_TimeAttackCars[g_PlayerCarIndex].paintColor2 = D_80082EB4;
                    GameMenuBusy = 0;
                }
                if (*btn & 0x90) {
                    GamePlaySoundCue(3);
                    D_80082EB4 = g_CarTable[g_PlayerCarIndex].paintColor2;
                    GameMenuBusy = 0;
                }
                func_8001DA74(D_80082EB4);
            }
        }

        func_80049418(1, 0, 1, 1);
        func_800489AC(g_UiScriptProgress, 2, D_801F17A0);
        func_800487D8(&D_80082010, &g_UiScriptProgress, 0);
        func_800487D8(&g_UiChromeScript, &g_UiScriptProgress, 1);
        return;
    }

    g_MenuHandlerIndex = -1;
    g_MenuHandlerIndex2 = 10;
    func_800487D8(&D_80082010, &g_UiScriptProgress, -1);
    func_800487D8(&g_UiChromeScript, &g_UiScriptProgress, 0);
    func_800489AC(g_UiScriptProgress, 2, D_801F17A0);
    if (g_UiScriptProgress <= 0) {
        g_MenuScreen = 6;
        g_MenuHandlerIndex = 6;
        D_801F17A0 = 0;
        g_UiScriptProgress = 0;
        GameMenuBusy = 0;
    }
}

extern s32 D_8009B2E8;

void func_80052158(s32 arg0, s32 arg1, s32 arg2);

s32 GameDrawCarShopScreen(s32 step) asm("func_80059248");
s32 GameDrawCarShopScreen(s32 arg0) {
    s32 value;
    s32 limit;
    s32 amount;
    s32 phase;
    s32 channel;

    if (arg0 == 0) {
        D_8009B2E8 = 0;
        return;
    }

    if (arg0 > 0) {
        value = D_8009B2E8 + arg0;
        D_8009B2E8 = value;
        if (value >= 0x1FD) {
            D_8009B2E8 = 0x1FC;
        }
        value = 0;
    } else {
        value = D_8009B2E8 + arg0;
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
    phase = ((u32)D_8009B2E8 >> 2) & 0xFF;
    channel = limit;
    func_80052158(amount, phase, channel);

    return D_8009B2E8;
}

extern s32 D_8009B33C;
extern s16 g_PrevOwnedCarIndex asm("D_8019CA18");
extern s16 g_NextOwnedCarIndex asm("D_801E41A4");

s32 GameGetCarUnlockLevel(s32 model) asm("func_8001785C");

void func_80059320(void) {
    s32 index;
    GameCarEntry *entry;

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
                s32 value = GameGetCarUnlockLevel(index);
                if (g_CarTable[index].enabled == 0) {
                    s32 progression = g_RaceProgress->maxClassReached;
                    if (progression < 4) {
                        if ((progression + 1) < value) {
                            index--;
                            goto backward_check;
                        }
                        g_PrevOwnedCarIndex = index;
                        goto backward_done;
                    }
                    if (progression >= value) {
                        g_PrevOwnedCarIndex = index;
                        goto backward_done;
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

backward_done:
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
            {
                s32 value = GameGetCarUnlockLevel(index);
                if (g_CarTable[index].enabled == 0) {
                    s32 progression = g_RaceProgress->maxClassReached;
                    if (progression < 4) {
                        if ((progression + 1) < value) {
                            index++;
                            goto forward_check;
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
        forward_check:
            if (index < 13) {
                goto forward_loop;
            }
        }
    }

forward_done:
    return;
}
