#include "common.h"
#include "game/menu.h"
#include "game/render.h"
#include "game/car.h"

s32 DrawDesignModeScreen(s32 arg0);
s32 DrawTeamLogoScreen(s32 arg0);

typedef struct CellMask {
    u8 cells[6][6];
} CellMask;

extern CellMask D_80011BD4;
extern s32 D_8009B2D4;

void func_80046A2C(void *ot, s32 x0, s32 y0, s32 x1, s32 y1, s32 u0, s32 v0,
                   s32 r, s32 g, s32 b, s32 clutX, s32 shadeTex,
                   s32 semiTrans, s32 flags);

s32 DrawDesignModeScreen(s32 step);
s32 DrawDesignModeScreen(s32 arg0) {
    CellMask mask;
    void *ot;
    s32 limit;
    s32 offset;
    s32 intensity;
    s32 y;
    s32 row;
    s32 column;

    ot = (void *)(*(u32 *)0x1F800004 + 4);
    mask = D_80011BD4;

    if (arg0 == 0) {
        D_8009B2D4 = 0;
        return;
    }

    if (arg0 > 0) {
        s32 updated;

        updated = D_8009B2D4 + arg0;
        D_8009B2D4 = updated;
        if (updated >= 0x1FD) {
            D_8009B2D4 = 0x1FC;
        }
        offset = 0;
    } else {
        s32 updated;

        updated = D_8009B2D4 + arg0;
        D_8009B2D4 = updated;
        if (updated < 0) {
            D_8009B2D4 = 0;
        }
        limit = 0x1FC - D_8009B2D4;
        offset = (u32)(limit * limit) / 2048;
    }

    y = 0xB0 - (s16)offset;
    intensity = (u32)D_8009B2D4 / 4;

    func_80046A2C(ot, 0xB4, y, 0x18, 0xC, 0x94, 0xDC,
                  (u8)intensity, (u8)intensity, (u8)intensity,
                  0x244, 0, 1, 0x3B);
    func_80046A2C(ot, 0xCE, y, 0x14, 0xC, 0xE0, 0xDC,
                  (u8)intensity, (u8)intensity, (u8)intensity,
                  0x244, 0, 1, 0x3B);

    for (row = 0; row < 6; row++) {
        for (column = 0; column < 6; column++) {
            s32 clutX;

            if (mask.cells[row][column] != 0) {
                clutX = 0x26F;
            } else {
                clutX = 0x244;
            }

            func_80046A2C(ot, 0xB4 + column * 0x10,
                          0xC0 + row * 0x20 - (s16)offset,
                          0xC, 0x18, 0xF4, 0x60,
                          (u8)intensity, (u8)intensity,
                          (u8)intensity,
                          clutX, 0, 1, 0x39);
        }
    }

    return D_8009B2D4;
}

extern s32 g_TeamNameCharModel;
extern s32 g_DesignModeOption;

extern u8 D_800828EC;
extern u8 g_UiChromeScript2;
extern u8 D_80081B54;
extern u8 g_UiChromeScript;

void DrawMenuCarView(void);
void DrawFadingMenuSprites(s32 a, s32 b, s32 c);
void PlaySoundCue(s32 cue);
void RampTeamLogoCanvas(s32 a, s32 b);
void DrawTeamLogoCanvas(s32 a, s32 b);

void UpdateDesignModeScreen(void);
void UpdateDesignModeScreen(void) {
    s32 sel;
    u16 edge;

    g_MenuAltLayout = g_MenuAltLayoutSetting;
    DrawMenuCarView();
    if (GameMenuBusy == 0) {
        RunTimedDrawScript(&D_800828EC, &g_UiScriptProgress2, -1);
        RunTimedDrawScript(&g_UiChromeScript2, &g_UiScriptProgress2, 0);
        DrawFadingMenuSprites(g_UiScriptProgress, 3, g_DesignModeOption);
        RunTimedDrawScript(&D_80081B54, &g_UiScriptProgress, 0);
        if (RunTimedDrawScript(&g_UiChromeScript, &g_UiScriptProgress, 1) != 0) {
            g_MenuOverlayPattern = -1;
            if (g_PadEdge2 & 0x1000) {
                PlaySoundCue(1);
                g_DesignModeOption = (g_DesignModeOption > 0) ? g_DesignModeOption - 1 : 3;
            }
            if (g_PadEdge2 & 0x4000) {
                PlaySoundCue(1);
                g_DesignModeOption = (g_DesignModeOption < 3) ? g_DesignModeOption + 1 : 0;
            }
            if (g_PadEdge2 & 0x860) {
                sel = g_DesignModeOption;
                if (sel == 0) {
                    PlaySoundCue(2);
                    RampTeamLogoCanvas(-256, -256);
                    GameMenuBusy = 1;
                    g_MenuOverlayPattern = 1;
                } else if (sel == 1) {
                    PlaySoundCue(2);
                    GameMenuBusy = 2;
                    g_MenuOverlayPattern = sel;
                } else if (sel == 2) {
                    if (g_PlayerCarIndex < 10) {
                        GameMenuBusy = 3;
                        g_MenuOverlayPattern = 1;
                        PlaySoundCue(2);
                    } else {
                        GameMenuBusy = -1;
                        g_UiScriptProgress2 = 0;
                        PlaySoundCue(5);
                    }
                } else if (sel == 3) {
                    PlaySoundCue(3);
                    GameMenuBusy = 4;
                    g_MenuOverlayPattern = 2;
                }
            } else if (g_PadEdge2 & 0x90) {
                PlaySoundCue(3);
                GameMenuBusy = 4;
                g_MenuOverlayPattern = 2;
            }
        }
    } else if (GameMenuBusy < 0) {
        RunTimedDrawScript(&D_800828EC, &g_UiScriptProgress2, 0);
        if (RunTimedDrawScript(&g_UiChromeScript2, &g_UiScriptProgress2, 1) != 0) {
            edge = g_PadEdge2;
            if (edge & 0x860) GameMenuBusy = 0;
            if (edge & 0x90) GameMenuBusy = 0;
        }
        DrawFadingMenuSprites(g_UiScriptProgress, 3, g_DesignModeOption);
        RunTimedDrawScript(&D_80081B54, &g_UiScriptProgress, 0);
        RunTimedDrawScript(&g_UiChromeScript, &g_UiScriptProgress, 1);
    } else {
        g_MenuHandlerIndex = -1;
        g_MenuHandlerIndex2 = 6;
        RunTimedDrawScript(&D_80081B54, &g_UiScriptProgress, -1);
        RunTimedDrawScript(&g_UiChromeScript, &g_UiScriptProgress, 0);
        DrawFadingMenuSprites(g_UiScriptProgress, 3, g_DesignModeOption);
        if (g_UiScriptProgress <= 0) {
            switch (GameMenuBusy) {
            case 1:
                g_MenuScreen = 7;
                g_MenuHandlerIndex = 7;
                DrawTeamLogoCanvas(0, 0);
                break;
            case 2:
                g_MenuScreen = 9;
                g_MenuHandlerIndex = 9;
                DrawTeamNameEntry(0, 0);
                g_MenuViewOffset = 0x3D090;
                g_MenuViewOffsetTarget = 0;
                g_MenuViewAngleTarget = 0;
                g_MenuViewAngle = 0;
                GameMenuCursor = (g_TeamNameLength >= 6) ? 0x2B : 0;
                g_TeamNameCharModel = GameMenuCursor;
                break;
            case 3:
                g_MenuScreen = 10;
                g_MenuHandlerIndex = 10;
                g_UiScriptProgress2 = 0;
                g_MenuViewOffset = 0x3D090;
                g_MenuViewOffsetTarget = 0;
                break;
            case 4:
                g_MenuScreen = 5;
                g_MenuHandlerIndex = 5;
                g_DesignModeOption = 0;
                g_MenuViewOffset = 0x3D090;
                g_MenuViewOffsetTarget = 0;
                break;
            }
            g_UiScriptProgress = 0;
            GameMenuBusy = 0;
        }
    }
}

extern s32 D_8009B2D8;

s32 DrawTeamLogoScreen(s32 step);
s32 DrawTeamLogoScreen(s32 arg0) {
    s32 value;

    if (arg0 == 0) {
        D_8009B2D8 = 0;
        return;
    }

    if (arg0 > 0) {
        value = arg0 + D_8009B2D8;
        D_8009B2D8 = value;
        if (value >= 0x1FD) {
            D_8009B2D8 = 0x1FC;
        }
    } else {
        value = arg0 + D_8009B2D8;
        D_8009B2D8 = value;
        if (value < 0) {
            D_8009B2D8 = 0;
        }
    }

    return D_8009B2D8;
}
