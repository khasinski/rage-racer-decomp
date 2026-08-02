#include "common.h"
#include "game/state.h"
#include "game/render.h"
#include "game/menu.h"
#include "game/race.h"
#include "game/asset.h"
#include "psyq/gpu.h"
#include "game/cd.h"

extern s32 g_FrameSyncThreshold asm("D_8019C768");
extern s32 D_801E8260;
extern s32 D_801E6F28;
extern s32 g_MainMenuSlide asm("D_801E6F1C");
extern s32 g_TitlePulse asm("D_801E429C");
extern s32 g_FrontendState asm("D_8009F098");
extern s32 g_TitleExitTimer asm("D_8009E880");
extern s32 g_TitleAttractTimer asm("D_8019CB70");

void GameCloseLoadedAudioSlots(void) asm("func_8005B9CC");
void GameResetTrackTextureSwap(void) asm("func_80019EBC");
void func_8001A498(void);
void func_80021540(void);
void func_8005DBB4(void);

/* Scene 2: the menu-side entry to the front end. Clears the title/menu
 * state words and hands over to scene 4, GameUpdateFrontend. */
void GameEnterFrontend(void) asm("func_8001AF70");
void GameEnterFrontend(void) {
    SetDispMask(0);
    GameCloseLoadedAudioSlots();
    GameResetTrackTextureSwap();
    func_8001A498();

    g_FrameSyncThreshold = 0x80;
    g_SceneId = 4;
    g_SceneTimer = 0;
    D_801E8260 = 0;
    D_801E6F28 = 0;
    g_MainMenuSlide = 0;
    g_TitlePulse = 0;
    g_FrontendState = 0;
    g_TitleExitTimer = 0;
    g_TitleAttractTimer = -1;

    func_80021540();
    func_8005DBB4();
}

extern s32 g_StreamReturnScene asm("D_8019C760");
extern s32 D_801E6F28;
extern s32 g_TitleAttractTimer;
extern s32 g_TitleExitTimer;
extern s32 g_FrameSyncThreshold asm("D_8019C768");
extern s32 D_801E8260;
extern s32 g_MainMenuSlide;
extern s32 g_FrontendState;
void func_8001A498(void);
void func_80021540(void);
void func_8005DBB4(void);
void GameDrawPressStartPrompt(void) asm("func_8001B170");

void GameEnterTitleScreen(void) asm("func_8001B014");

void GameEnterTitleScreen(void) {
    GameSetupDisplay240(0, 0, 0);
    if (g_StreamReturnScene != 0) {
        D_801E6F28 = 0xFF;
        g_TitleAttractTimer = 0x190;
        g_TitleExitTimer = 0;
    } else {
        SetDispMask(0);
        func_8001A498();
        D_801E6F28 = 0;
        g_TitleAttractTimer = 0;
        g_TitleExitTimer = 0x1E;
    }
    g_FrameSyncThreshold = 0x80;
    g_SceneTimer = 0;
    g_SceneId = 4;
    D_801E8260 = 0;
    g_MainMenuSlide = 0;
    g_FrontendState = 0;
    func_80021540();
    func_8005DBB4();
    GameDrawPressStartPrompt();
}


extern u8 *g_DrawBuffer asm("D_8019C900");

void *func_8001720C(void *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
void *func_80017390(void *arg0, void *arg1, s32 arg2);

/* Full-width grey box over the title screen at OT slot 0x29; the caller
 * steps its brightness D_801E6F28 down by 2 a frame. */
void GameDrawTitleFadeOverlay(s32 brightness) asm("func_8001B0F0");
void GameDrawTitleFadeOverlay(s32 arg0) {
    void *current;
    void **scratch;
    u8 *base;
    void *next;
    s32 color;

    color = arg0 & 0xFF;
    base = g_DrawBuffer;
    base += 0xCC;
    scratch = (void **)0x1F800000;
    current = *scratch;
    next = func_8001720C(base, current, 0, 0x18, 0x140, 0xC0, color, color, color);
    *scratch = func_80017390(base, next, 0x29);
}

extern s32 D_801E6F28;

s32 func_80068568(s32 arg0);
void GameDrawTitleFadeOverlay(s32 brightness) asm("func_8001B0F0");
void *func_80016F8C(void *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9);
void *func_80017390(void *arg0, void *arg1, s32 arg2);

void GameDrawPressStartPrompt(void) asm("func_8001B170");

void GameDrawPressStartPrompt(void) {
    void **scratch;
    u8 *base;
    void *next;
    s32 sinValue;
    s32 frame;

    if (D_801E6F28 > 0) {
        GameDrawTitleFadeOverlay((u8)D_801E6F28);
        D_801E6F28 -= 2;
    }

    sinValue = func_80068568(((g_AnimTimer * 3) << 5) & 0xFE0);
    frame = (sinValue / 64) + 0x80;

    scratch = (void **)0x1F800000;
    base = g_DrawBuffer;
    base += 0xCC;
    next = *scratch;
    next = func_80016F8C(base, next, 0x68, 0xC8, 0x70, 0x10, 0x70, 0xA0, 0x7E84, frame);
    *scratch = func_80017390(base, next, 0x39);
}

extern s32 g_TitleAttractTimer;
extern s32 g_FrontendState;
extern s32 D_801E8260;
void GamePlaySoundCue(s32 cue) asm("func_8005D6EC");
void GameDrawPressStartPrompt(void) asm("func_8001B170");

void GameUpdateTitleScreen(void) asm("func_8001B260");

void GameUpdateTitleScreen(void) {
    if (g_PadEdge2 & 0x800) {
        GamePlaySoundCue(2);
        g_FrontendState = 1;
        D_801E8260 = 0;
        g_TitleMenuSelection = 0;
        if (g_TitleAttractTimer > 0) {
            g_TitleAttractTimer = 0;
            GameStartCdVolumeFade(1);
        }
    }
    GameDrawPressStartPrompt();
}

extern s32 g_FrontendState;
extern s32 g_TitlePulse;
extern s32 g_MainMenuSlide;

void *func_800175A4(void *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10, s32 arg11);

void GameDrawMainMenuRows(void) asm("func_8001B2D4");

void GameDrawMainMenuRows(void) {
    void *scratch;
    u8 *base;
    s32 row;
    s32 i;
    s32 one;
    s32 width;
    s32 y;

    base = g_DrawBuffer + 0xCC;
    scratch = *(void **)0x1F800000;
    row = 0;
    i = 0;
    one = 1;
    width = 0x70;
    y = 0x64;

    while (i < 5) {
        s32 code;
        s32 frame;
        s32 delta;

        code = 0x7E85;

        if ((g_AdvancedSeriesUnlocked == 0) && (i == one)) {
            i = 2;
        }

        if (i == g_TitleMenuSelection) {
            code = 0x7E86;
        }

        if (g_FrontendState == one) {
            code = 0x7E85;
        }

        if ((g_TitlePulse & 2) != 0) {
            code = 0x7E85;
        }

        delta = g_MainMenuSlide - (row * 8);
        if (delta >= 0) {
            frame = delta;
            if (frame >= 0x11) {
                frame = 0x10;
            }
        } else {
            frame = 0;
        }

        scratch = func_800175A4(base, scratch, 0x68, y, width, frame, 0, (i << 4) + 0xA0, width, 0x10, code, 0x39);
        y += 0x18;
        i++;
        row++;
    }

    *(void **)0x1F800000 = scratch;
}

extern s32 g_FrontendState;
extern s32 g_MainMenuSlide;

void GameDrawMainMenuRows(void) asm("func_8001B2D4");

void GameUpdateMainMenuOpen(void) asm("func_8001B440");

void GameUpdateMainMenuOpen(void) {
    if (++g_MainMenuSlide == 0x30) {
        g_FrontendState = 2;
    }

    GameDrawMainMenuRows();
}

extern s32 g_BgmTrackCount asm("D_801E40A8");
extern volatile u8 g_BgmShuffleOrder[] asm("D_801E7734");
extern u8 D_801E7733[];
extern s32 g_BgmShuffleIndex asm("D_8009E6CC");

s32 GameRandom15(void) asm("func_800632B0");

/* Refills g_BgmShuffleOrder with a random permutation of the
 * g_BgmTrackCount tracks and rewinds g_BgmShuffleIndex. */
void GameShuffleBgmOrder(void) asm("func_8001B488");
void GameShuffleBgmOrder(void) {
    s32 i;
    s32 count;
    s32 j;
    s32 remaining;

    for (i = 0; i < g_BgmTrackCount; i++) {
        g_BgmShuffleOrder[i] = 0xFF;
    }

    for (i = 0; i < g_BgmTrackCount; i++) {
        count = 0;
        for (j = 0; j < g_BgmTrackCount; j++) {
            if (g_BgmShuffleOrder[j] == 0xFF) {
                count++;
            }
        }

        remaining = ((GameRandom15() & 0xFFF) % count) + 1;
        j = 0;
        while (remaining != 0) {
            if (g_BgmShuffleOrder[j] == 0xFF) {
                remaining--;
            }
            j++;
        }
        D_801E7733[j] = i;
    }

    g_BgmShuffleIndex = 0;
}

extern volatile u16 g_PadEdge2 asm("D_801E436E");
extern s32 D_801E8260;

extern s32 *g_CarTable asm("D_8019C7C8");
extern s32 *g_CourseProgress asm("D_8009E67C");
extern s32 g_ExtraGrandPrixSaveMaxClass asm("D_801E6E88");
extern s32 g_FrontendState;
extern s32 g_OptionMenuCursor asm("D_8019C7B4");

extern s32 g_GrandPrixCars asm("D_801E4F44");
extern s32 g_GrandPrixCourseProgress asm("D_801E42EC");
extern s32 g_ExtraGrandPrixCars asm("D_8019C914");
extern s32 g_ExtraGrandPrixCourseProgress asm("D_8009E874");
extern s32 g_TimeAttackCars asm("D_801E4388");

void GamePlaySoundCue(s32 cue) asm("func_8005D6EC");
extern void GameResetAssetLoader(void) asm("func_80017BE4");
extern void GameShuffleBgmOrder(void) asm("func_8001B488");
s32 GameRequestTrackLoad(void) asm("func_8001965C");
extern void GameRequestSelectBgmAssetsNoReset(void) asm("func_8001839C");
extern void GameRequestSaveScreenAssets(void) asm("func_800182D0");
extern void GameRequestOptionScreenAssets(void) asm("func_80018B98");
extern void GameDrawMainMenuRows(void) asm("func_8001B2D4");

void GameUpdateMainMenuInput(void) asm("func_8001B5DC");

void GameUpdateMainMenuInput(void) {
    volatile u16 *flagp = &g_PadEdge2;
    s32 idx;
    u16 flags;

    if (*flagp != 0) {
        D_801E8260 = 0;
    }
    flags = *flagp;
    idx = g_TitleMenuSelection;

    if (flags & 0x1000) {
        s32 n = idx - 1;
        g_TitleMenuSelection = n;
        if (g_AdvancedSeriesUnlocked == 0 && n == 1) {
            g_TitleMenuSelection = idx - 2;
        }
    } else if (flags & 0x4000) {
        s32 n = idx + 1;
        g_TitleMenuSelection = n;
        if (g_AdvancedSeriesUnlocked == 0 && n == 1) {
            g_TitleMenuSelection = idx + 2;
        }
    }

    {
        s32 m = g_TitleMenuSelection + 5;
        m = m % 5;
        g_TitleMenuSelection = m;
        if (idx != m) {
            GamePlaySoundCue(1);
        }
    }

    if (g_PadEdge2 & 0x860) {
        GamePlaySoundCue(2);
        if (g_AssetLoadState != 0) {
            GameResetAssetLoader();
        }
        func_8001B488();
        switch (g_TitleMenuSelection) {
        case 0:
            g_CarTable = &g_GrandPrixCars;
            g_RaceProgress = &g_GrandPrixSave;
            g_CourseProgress = &g_GrandPrixCourseProgress;
            g_SeriesSelection = 0;
            if (g_GrandPrixSave.maxClassReached == -1) {
                g_GrandPrixClass = 0;
                g_CourseIndex = 3;
                GameRequestTrackLoad();
            } else {
                GameRequestSelectBgmAssetsNoReset();
            }
            break;
        case 1:
            g_CarTable = &g_ExtraGrandPrixCars;
            g_RaceProgress = &g_ExtraGrandPrixSave;
            g_CourseProgress = &g_ExtraGrandPrixCourseProgress;
            g_SeriesSelection = 1;
            if (g_ExtraGrandPrixSaveMaxClass == -1) {
                g_GrandPrixClass = 0;
                g_CourseIndex = 3;
                GameRequestTrackLoad();
            } else {
                GameRequestSelectBgmAssetsNoReset();
            }
            break;
        case 2:
            g_CarTable = &g_TimeAttackCars;
            g_RaceProgress = &g_TimeAttackSave;
            g_SeriesSelection = 0;
            GameRequestSelectBgmAssetsNoReset();
            break;
        case 3:
            GameRequestSaveScreenAssets();
            break;
        case 4:
            GameRequestOptionScreenAssets();
            g_OptionMenuCursor = 0;
            break;
        }
        g_FrontendState = 3;
    }
    GameDrawMainMenuRows();
}
