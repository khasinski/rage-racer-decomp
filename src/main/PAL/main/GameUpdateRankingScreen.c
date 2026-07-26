#include "common.h"
#include "game/car.h"
#include "game/race.h"
#include "game/menu.h"
#include "game/render.h"

s32 GameDrawCarSelectScreen(s32 arg0);
s32 GameDrawCustomizeScreen(s32 arg0);

extern u8 *g_CarModelAsset asm("D_8009E698");


extern s32 D_8009B2CC;
extern s32 D_8009B2C8;
extern s32 D_8009B334;
extern s32 D_8019CDF8;
extern u8 D_80082724;
extern u8 D_80081890;
extern u8 g_UiChromeScript asm("D_80082460");

void func_80047460(void *buf, s32 xa, s32 ya, s32 w, s32 h, s32 r, s32 g, s32 b, s32 code);
void func_80046A2C(void *ot, s32 x0, s32 y0, s32 x1, s32 y1, s32 u0, s32 v0,
                   s32 r, s32 g, s32 b, s32 clut, s32 sh, s32 st, s32 flags);

void func_8005194C(void);
void func_8004CF30(s32 arg0);
void func_800489AC(s32 arg0, s32 arg1, s32 arg2);
s32 func_800487D8(u8 *arg0, s32 *arg1, s32 arg2);
void GamePlaySoundCue(s32 cue) asm("func_8005D6EC");
s32 func_8004D384(s32 *arg0, s32 arg1, s32 arg2);
void func_800509C4(s32 arg0);

void GameUpdateRankingScreen(void) asm("func_80054D10");
void GameUpdateRankingScreen(void) {
    s32 state;

    g_MenuAltLayout = 0;
    func_8005194C();
    func_8004CF30(-9);
    state = GameMenuBusy;
    if (state == 0) {
        g_UiScriptProgress2 = 0;
        GameMenuBusy = -1;
        func_800489AC(0, 2, D_8019CDF8);
        func_800487D8(&D_80082724, &g_UiScriptProgress2, 1);
    } else if (state < 0) {
        switch (state) {
        case -1:
            func_800489AC(g_UiScriptProgress2, 2, D_8019CDF8);
            if (func_800487D8(&D_80082724, &g_UiScriptProgress2, 1) != 0) {
                g_MenuOverlayPattern = -1;
                if (g_PadEdge2 & 0x1000) {
                    GamePlaySoundCue(1);
                    D_8019CDF8 = (D_8019CDF8 > 0) ? D_8019CDF8 - 1 : 2;
                }
                if (g_PadEdge2 & 0x4000) {
                    GamePlaySoundCue(1);
                    D_8019CDF8 = (D_8019CDF8 < 2) ? D_8019CDF8 + 1 : 0;
                }
                {
                    s32 flags = g_PadEdge2;
                    if (flags & 0x860) {
                        s32 x = D_8019CDF8;
                        if (x == 0) {
                            GamePlaySoundCue(2);
                            GameMenuBusy = -2;
                            D_8009B2C8 = -3;
                        } else if (x == 1) {
                            GamePlaySoundCue(2);
                            GameMenuBusy = -2;
                            D_8009B2C8 = -5;
                        } else if (x == 2) {
                            GamePlaySoundCue(3);
                            GameMenuBusy = 1;
                            g_MenuOverlayPattern = x;
                        }
                    } else if (flags & 0x90) {
                        GamePlaySoundCue(3);
                        GameMenuBusy = 1;
                        g_MenuOverlayPattern = 2;
                    }
                }
            }
            break;
        case -2:
            func_800487D8(&D_80082724, &g_UiScriptProgress2, -1);
            func_800489AC(g_UiScriptProgress2, 2, D_8019CDF8);
            if (g_UiScriptProgress2 > 0) {
                break;
            }
            GameMenuBusy = D_8009B2C8;
            break;
        case -3:
            if (func_8004D384(&g_UiScriptProgress2, 1, 0) == 0) {
                break;
            }
            if (!(g_PadEdge2 & 0x8f0)) {
                break;
            }
            GamePlaySoundCue(3);
            GameMenuBusy = -4;
            break;
        case -4:
            func_8004D384(&g_UiScriptProgress2, -1, 0);
            if (g_UiScriptProgress2 > 0) {
                break;
            }
            GameMenuBusy = -1;
            break;
        case -5:
            if (func_8004D384(&g_UiScriptProgress2, 1, 1) == 0) {
                break;
            }
            if (!(g_PadEdge2 & 0x8f0)) {
                break;
            }
            GamePlaySoundCue(3);
            GameMenuBusy = -6;
            break;
        case -6:
            func_8004D384(&g_UiScriptProgress2, -1, 1);
            if (g_UiScriptProgress2 > 0) {
                break;
            }
            GameMenuBusy = -1;
            break;
        }
    } else {
        goto pos;
    }
    func_800487D8(&D_80081890, &g_UiScriptProgress, 0);
    func_800487D8(&g_UiChromeScript, &g_UiScriptProgress, 1);
    return;
pos:
    g_MenuHandlerIndex = -1;
    g_MenuHandlerIndex2 = 2;
    func_800487D8(&D_80082724, &g_UiScriptProgress2, -1);
    func_800489AC(g_UiScriptProgress2, 2, D_8019CDF8);
    func_800487D8(&D_80081890, &g_UiScriptProgress, -1);
    func_800487D8(&g_UiChromeScript, &g_UiScriptProgress, 0);
    if (g_UiScriptProgress > 0) {
        return;
    }
    g_MenuScreen = 1;
    g_MenuHandlerIndex = 1;
    D_8019CDF8 = 0;
    g_UiScriptProgress = 0;
    GameMenuBusy = 0;
    func_800509C4(0);
    if (g_CourseIndex >= 4) {
        D_8009B334 = 1;
    } else {
        D_8009B334 = -1;
    }
}

s32 GameDrawCarSelectScreen(s32 step) asm("func_800551BC");
s32 GameDrawCarSelectScreen(s32 arg0) {
    s32 p = (s32) *(void **) 0x1F800004;
    u8 *buf = (u8 *) p + 4;
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

    v = (u32) D_8009B2CC >> 2;
    col = v & 0xff;
    func_80047460(buf, 0xa3, 0x180, 0x1a, 0x19, col, col, col, 0x20);

    tex = ((u8 *) g_CarTable)[g_PlayerCarIndex * 8 + 2];
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

extern s16 D_8019CA18;
extern s16 D_801E41A4;

void func_80055454(void) {
    s32 index;
    GameCarEntry *ptr;

    D_8019CA18 = -1;
    index = g_PlayerCarIndex - 1;
    if (index >= 0) {
        s32 one = 1;
        s32 offset = index * 8;
        ptr = (GameCarEntry *)(offset + (s32)g_CarTable);
        while (index >= 0) {
            if (ptr->enabled == one) {
                D_8019CA18 = index;
                break;
            }
            index--;
            ptr--;
        }
    }

    D_801E41A4 = -1;
    index = g_PlayerCarIndex + 1;
    if (index < 13) {
        s32 one = 1;
        s32 offset = index * 8;
        ptr = (GameCarEntry *)(offset + (s32)g_CarTable);
        while (index < 13) {
            if (ptr->enabled == one) {
                D_801E41A4 = index;
                break;
            }
            index++;
            ptr++;
        }
    }
}

extern s32 D_8009B33C;
extern s32 D_8019C7B8;

s32 GameGetCarUnlockLevel(s32 model) asm("func_8001785C");

void func_80055524(void) {
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
loop_enabled:
        byte = enabledPtr->enabled;
        enabledPtr--;
        if (byte == 0) {
            D_8019C7B8 = index;
        }
        index--;
        if (index < 0) {
            goto done;
        }
        goto loop_enabled;
    }

    index = 12;
loop_disabled:
    {
        value = GameGetCarUnlockLevel(index);
        {
            s32 offset = index * 8;
            ptr = (GameCarEntry *)(offset + (s32)g_CarTable);
        }
        if (ptr->enabled == 0) {
            if (g_RaceProgress->maxClassReached < 4) {
                if ((g_RaceProgress->maxClassReached + 1) < value) {
                    index--;
                    goto loop_disabled_check;
                }
            } else if (g_RaceProgress->maxClassReached < value) {
                index--;
                goto loop_disabled_check;
            }
            D_8019C7B8 = index;
        }
        index--;
    }
loop_disabled_check:
    if (index >= 0) {
        goto loop_disabled;
    }

done:
}

extern s32 D_8009B31C;
extern s32 g_MenuPlateCarIndex asm("D_8009B320");

void func_80051238(void);
void func_8004FCE8(s32 arg0, s32 arg1, s32 arg2);
void func_8005131C(void);

void GameEnterCarSelectScreen(void) asm("func_80055618");
void GameEnterCarSelectScreen(void) {
    g_MenuAltLayout = g_MenuAltLayoutSetting;
    func_80051238();
    g_MenuScreen = 4;
    g_UiScriptProgress = 0;
    func_80055454();
    func_8004FCE8(D_8009B31C, g_MenuPlateCarIndex, 0);
    func_8005131C();
    func_8004CF30(-9);
}

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/GameUpdateRankingScreen", func_8005568C);

extern s32 D_8009B2D0;

void func_80052158(s32 arg0, s32 arg1, s32 arg2);

s32 GameDrawCustomizeScreen(s32 step) asm("func_800562C8");
s32 GameDrawCustomizeScreen(s32 arg0) {
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
        value = product >> 11;
    }

    func_80052158((s16)value, ((u32)D_8009B2D0 >> 2) & 0xFF, g_PlayerCarIndex);
    return D_8009B2D0;
}

extern u8 g_MenuBlankCaption asm("D_80011BA0");
extern u8 D_80081A34;
extern u8 D_80081AD0;
extern u8 D_80082574;
extern u8 D_800825A4;
extern u8 g_UiChromeScript2 asm("D_80082790");
extern u8 D_80082814;
extern u8 g_MenuSubCursor asm("D_8009B2F0");
extern s32 g_MenuConfirmTimer asm("D_8009B300");
extern s32 D_8009B324;
extern u8 *D_8019C794;
extern s32 D_8019C7C0;
extern u8 D_801E4389[];
extern u8 D_801E438A[];
s32 func_800487D8(u8 *commands, s32 *progress, s32 step);
void func_80048ED8(u8 x, s32 useFlag);
void func_80048D64(s32 x0, s32 y0, s32 x1, s32 y1, s32 useFlash);
void func_80048B88(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6,
                   s32 a7, s32 a8, s32 a9, void *a10);

void GameUpdateCustomizeScreen(void) asm("func_800563A0");
void GameUpdateCustomizeScreen(void) {
    void *ot;
    s32 mode;
    s32 lowMode;
    u8 *cmdList;
    u16 *pad;
    s32 sel;

    ot = *(void **)0x1F800004;
    g_MenuAltLayout = g_MenuAltLayoutSetting;
    func_8004FCE8(D_8009B31C, g_MenuPlateCarIndex, 0);
    mode = 2;
    func_8005131C();
    if (g_GrandPrixMode != 0) {
        mode = 3;
    }
    cmdList = &D_80081AD0;
    if (g_GrandPrixMode != 0) {
        cmdList = &D_80081A34;
    }

    if (GameMenuBusy == 0) {
        D_8009B324 = 3;
        func_800487D8(D_8019C794, &g_UiScriptProgress2, -1);
        lowMode = mode & 0xFF;
        func_800489AC(g_UiScriptProgress, lowMode, D_8019C7C0);
        func_800487D8(cmdList, &g_UiScriptProgress, 0);
        if ((func_800487D8(&g_UiChromeScript, &g_UiScriptProgress, 1) != 0) && (g_UiScriptProgress2 <= 0)) {
            g_MenuOverlayPattern = -1;
            if (g_PadEdge2 & 0x1000) {
                GamePlaySoundCue(1);
                D_8019C7C0 = (D_8019C7C0 > 0) ? D_8019C7C0 - 1 : lowMode;
            }
            if (g_PadEdge2 & 0x4000) {
                GamePlaySoundCue(1);
                D_8019C7C0 = (D_8019C7C0 < mode) ? D_8019C7C0 + 1 : 0;
            }
            if (g_PadEdge2 & 0x860) {
                u8 carByte;

                sel = D_8019C7C0;
                if (sel == 0) {
                    func_8005D6EC(2);
                    carByte = g_CarTable[g_PlayerCarIndex].tireCompound;
                    D_8019C794 = &D_80082574;
                    GameMenuBusy = -1;
                    goto set_state;
                }
                if (sel == 1) {
                    if (g_CarModelAsset[8] != 0) {
                        func_8005D6EC(2);
                        carByte = g_CarTable[g_PlayerCarIndex].transmission;
                        D_8019C794 = &D_800825A4;
                        GameMenuBusy = -2;
set_state:
                        g_UiScriptProgress2 = 0;
                        g_MenuSubCursor = carByte;
                        return;
                    }
                    GamePlaySoundCue(5);
                    D_8019C794 = &D_80082814;
                    GameMenuBusy = -3;
                    g_UiScriptProgress2 = 0;
                    return;
                }
                if (sel == mode) {
                    goto block27;
                }
                if (sel == 2) {
                    GamePlaySoundCue(2);
                    GameMenuBusy = 1;
                    g_MenuOverlayPattern = 1;
                    D_8009B324 = -3;
                    g_MenuViewOffsetTarget = 0x3D090;
                }
            } else if (g_PadEdge2 & 0x90) {
block27:
                GamePlaySoundCue(3);
                GameMenuBusy = 2;
                g_MenuOverlayPattern = 2;
            }
        }
        return;
    }

    if (GameMenuBusy < 0) {
        if (GameMenuBusy == -1) {
            if (func_800487D8(D_8019C794, &g_UiScriptProgress2, 1) != 0) {
                pad = &g_PadEdge2;
                if (*pad & 0x860) {
                    GamePlaySoundCue(2);
                    GameMenuBusy = -5;
                    g_MenuConfirmTimer = 0x23;
                }
                if (*pad & 0x90) {
                    GamePlaySoundCue(3);
                    GameMenuBusy = 0;
                }
                if ((*pad & 0x8000) && (g_MenuSubCursor < 4)) {
                    GamePlaySoundCue(1);
                    g_MenuSubCursor++;
                }
                if (g_PadEdge2 & 0x2000) {
                    if (g_MenuSubCursor != 0) {
                        GamePlaySoundCue(1);
                        g_MenuSubCursor--;
                    }
                }
                func_80048ED8(g_MenuSubCursor, 0);
            }
        } else if (GameMenuBusy == -2) {
            if (func_800487D8(D_8019C794, &g_UiScriptProgress2, 1) != 0) {
                pad = &g_PadEdge2;
                if (*pad & 0x860) {
                    GamePlaySoundCue(2);
                    GameMenuBusy = -6;
                    g_MenuConfirmTimer = 0x23;
                    g_CarTable[g_PlayerCarIndex].transmission = g_MenuSubCursor;
                    D_801E438A[g_PlayerCarIndex * 8] = g_MenuSubCursor;
                }
                if (*pad & 0x90) {
                    GamePlaySoundCue(3);
                    GameMenuBusy = 0;
                }
                if ((*pad & 0x8000) && (g_MenuSubCursor != 0)) {
                    GamePlaySoundCue(1);
                    g_MenuSubCursor = 0;
                }
                if (g_PadEdge2 & 0x2000) {
                    if (g_MenuSubCursor == 0) {
                        GamePlaySoundCue(1);
                        g_MenuSubCursor = 1;
                    }
                }
                func_80048D64((g_MenuSubCursor != 0) ? 0xDA : 0xB8, 0x68, 0x20, 0x20, 0);
                func_80046A2C(ot, 0xC2, 0x70, 0xC, 0x10, 0x60, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                func_80046A2C(ot, 0xE3, 0x70, 0x10, 0x10, 0x6C, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                func_80048B88(0xB8, 0x68, 0x20, 0x20, 0x95, 0x25, 0x1E, 0, 0, 0, &g_MenuBlankCaption);
                func_80048B88(0xDA, 0x68, 0x20, 0x20, 0x1E, 0x8E, 0x95, 0, 0, 0, &g_MenuBlankCaption);
            }
        } else if (GameMenuBusy == -3) {
            func_800487D8(D_8019C794, &g_UiScriptProgress2, 0);
            if (func_800487D8(&g_UiChromeScript2, &g_UiScriptProgress2, 1) != 0) {
                if (g_PadEdge2 & 0x860) {
                    GameMenuBusy = -4;
                }
                if (g_PadEdge2 & 0x90) {
                    GameMenuBusy = -4;
                }
            }
        } else if (GameMenuBusy == -4) {
            func_800487D8(D_8019C794, &g_UiScriptProgress2, -1);
            func_800487D8(&g_UiChromeScript2, &g_UiScriptProgress2, 0);
            if (g_UiScriptProgress2 <= 0) {
                GameMenuBusy = 0;
            }
        } else if (GameMenuBusy == -5) {
            if (g_MenuConfirmTimer <= 0) {
                func_800487D8(D_8019C794, &g_UiScriptProgress2, -1);
                if (g_UiScriptProgress2 <= 0) {
                    GameMenuBusy = 0;
                    g_CarTable[g_PlayerCarIndex].tireCompound = g_MenuSubCursor;
                    D_801E4389[g_PlayerCarIndex * 8] = g_MenuSubCursor;
                }
            } else {
                g_MenuConfirmTimer -= 1;
                func_800487D8(D_8019C794, &g_UiScriptProgress2, 1);
                func_80048ED8(g_MenuSubCursor, 1);
            }
        } else if (GameMenuBusy == -6) {
            if (g_MenuConfirmTimer <= 0) {
                func_800487D8(D_8019C794, &g_UiScriptProgress2, -1);
                if (g_UiScriptProgress2 <= 0) {
                    GameMenuBusy = 0;
                }
            } else {
                g_MenuConfirmTimer -= 1;
                func_800487D8(D_8019C794, &g_UiScriptProgress2, 1);
                func_80048D64((g_MenuSubCursor != 0) ? 0xDA : 0xB8, 0x68, 0x20, 0x20, 1);
                func_80046A2C(ot, 0xC2, 0x70, 0xC, 0x10, 0x60, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                func_80046A2C(ot, 0xE3, 0x70, 0x10, 0x10, 0x6C, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                func_80048B88(0xB8, 0x68, 0x20, 0x20, 0x95, 0x25, 0x1E, 0, 0, 0, &g_MenuBlankCaption);
                func_80048B88(0xDA, 0x68, 0x20, 0x20, 0x1E, 0x8E, 0x95, 0, 0, 0, &g_MenuBlankCaption);
            }
        }
        func_800489AC(g_UiScriptProgress, mode, D_8019C7C0);
        func_800487D8(cmdList, &g_UiScriptProgress, 0);
        func_800487D8(&g_UiChromeScript, &g_UiScriptProgress, 1);
        return;
    }

    g_MenuHandlerIndex = -1;
    g_MenuHandlerIndex2 = 5;
    func_800487D8(cmdList, &g_UiScriptProgress, -1);
    func_800487D8(&g_UiChromeScript, &g_UiScriptProgress, 0);
    func_800489AC(g_UiScriptProgress, mode, D_8019C7C0);
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
            D_8019C7C0 = 0;
            break;
        }
        g_UiScriptProgress = 0;
        GameMenuBusy = 0;
    }
}
