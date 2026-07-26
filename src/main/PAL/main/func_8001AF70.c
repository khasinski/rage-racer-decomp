#include "common.h"
#include "game/state.h"
#include "game/render.h"
#include "game/menu.h"
#include "game/race.h"
#include "game/asset.h"
#include "psyq/gpu.h"
#include "game/cd.h"

extern s32 D_8019C768;
extern s32 D_801E8260;
extern s32 D_801E6F28;
extern s32 D_801E6F1C;
extern s32 D_801E429C;
extern s32 D_8009F098;
extern s32 D_8009E880;
extern s32 D_8019CB70;

void func_8005B9CC(void);
void func_80019EBC(void);
void func_8001A498(void);
void func_80021540(void);
void func_8005DBB4(void);

void func_8001AF70(void) {
    SetDispMask(0);
    func_8005B9CC();
    func_80019EBC();
    func_8001A498();

    D_8019C768 = 0x80;
    g_SceneId = 4;
    g_SceneTimer = 0;
    D_801E8260 = 0;
    D_801E6F28 = 0;
    D_801E6F1C = 0;
    D_801E429C = 0;
    D_8009F098 = 0;
    D_8009E880 = 0;
    D_8019CB70 = -1;

    func_80021540();
    func_8005DBB4();
}

extern s32 g_StreamReturnScene asm("D_8019C760");
extern s32 D_801E6F28, D_8019CB70, D_8009E880, D_8019C768;
extern s32 D_801E8260, D_801E6F1C, D_8009F098;
void func_8001A498(void);
void func_80021540(void);
void func_8005DBB4(void);
void GameDrawPressStartPrompt(void) asm("func_8001B170");

void GameEnterTitleScreen(void) asm("func_8001B014");

void GameEnterTitleScreen(void) {
    GameSetupDisplay240(0, 0, 0);
    if (g_StreamReturnScene != 0) {
        D_801E6F28 = 0xFF;
        D_8019CB70 = 0x190;
        D_8009E880 = 0;
    } else {
        SetDispMask(0);
        func_8001A498();
        D_801E6F28 = 0;
        D_8019CB70 = 0;
        D_8009E880 = 0x1E;
    }
    D_8019C768 = 0x80;
    g_SceneTimer = 0;
    g_SceneId = 4;
    D_801E8260 = 0;
    D_801E6F1C = 0;
    D_8009F098 = 0;
    func_80021540();
    func_8005DBB4();
    GameDrawPressStartPrompt();
}


extern u8 *g_DrawBuffer asm("D_8019C900");

void *func_8001720C(void *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
void *func_80017390(void *arg0, void *arg1, s32 arg2);

void func_8001B0F0(s32 arg0) {
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
void func_8001B0F0(s32 arg0);
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
        func_8001B0F0((u8)D_801E6F28);
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

extern s32 D_8019CB70;
extern s32 D_8009F098;
extern s32 D_801E8260;
void GamePlaySoundCue(s32 cue) asm("func_8005D6EC");
void GameDrawPressStartPrompt(void) asm("func_8001B170");

void GameUpdateTitleScreen(void) asm("func_8001B260");

void GameUpdateTitleScreen(void) {
    if (g_PadEdge2 & 0x800) {
        GamePlaySoundCue(2);
        D_8009F098 = 1;
        D_801E8260 = 0;
        g_TitleMenuSelection = 0;
        if (D_8019CB70 > 0) {
            D_8019CB70 = 0;
            GameStartCdVolumeFade(1);
        }
    }
    GameDrawPressStartPrompt();
}

extern s32 D_8009F098;
extern s32 D_801E429C;
extern s32 D_801E6F1C;

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

        if (D_8009F098 == one) {
            code = 0x7E85;
        }

        if ((D_801E429C & 2) != 0) {
            code = 0x7E85;
        }

        delta = D_801E6F1C - (row * 8);
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

extern s32 D_8009F098;
extern s32 D_801E6F1C;

void GameDrawMainMenuRows(void) asm("func_8001B2D4");

void GameUpdateMainMenuOpen(void) asm("func_8001B440");

void GameUpdateMainMenuOpen(void) {
    if (++D_801E6F1C == 0x30) {
        D_8009F098 = 2;
    }

    GameDrawMainMenuRows();
}

extern s32 g_BgmTrackCount asm("D_801E40A8");
extern volatile u8 D_801E7734[];
extern u8 D_801E7733[];
extern s32 D_8009E6CC;

s32 GameRandom15(void) asm("func_800632B0");

void func_8001B488(void) {
    s32 i;
    s32 count;
    s32 j;
    s32 remaining;

    for (i = 0; i < g_BgmTrackCount; i++) {
        D_801E7734[i] = 0xFF;
    }

    for (i = 0; i < g_BgmTrackCount; i++) {
        count = 0;
        for (j = 0; j < g_BgmTrackCount; j++) {
            if (D_801E7734[j] == 0xFF) {
                count++;
            }
        }

        remaining = ((GameRandom15() & 0xFFF) % count) + 1;
        j = 0;
        while (remaining != 0) {
            if (D_801E7734[j] == 0xFF) {
                remaining--;
            }
            j++;
        }
        D_801E7733[j] = i;
    }

    D_8009E6CC = 0;
}

extern volatile u16 g_PadEdge2 asm("D_801E436E");
extern s32 D_801E8260;

extern s32 *g_CarTable asm("D_8019C7C8");
extern s32 *D_8009E67C;
extern s32 D_801E40A0;
extern s32 D_801E6E88;
extern s32 D_8009F098;
extern s32 D_8019C7B4;

extern s32 D_801E4F44;
extern s32 D_801E4094;
extern s32 D_801E42EC;
extern s32 D_8019C914;
extern s32 D_801E6E7C;
extern s32 D_8009E874;
extern s32 D_801E4388;
extern s32 D_8019C980;

void GamePlaySoundCue(s32 cue) asm("func_8005D6EC");
extern void func_80017BE4(void);
extern void func_8001B488(void);
s32 GameRequestTrackLoad(void) asm("func_8001965C");
extern void func_8001839C(void);
extern void func_800182D0(void);
extern void func_80018B98(void);
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
            func_80017BE4();
        }
        func_8001B488();
        switch (g_TitleMenuSelection) {
        case 0:
            g_CarTable = &D_801E4F44;
            g_RaceProgress = (GameRaceProgress *)&D_801E4094;
            D_8009E67C = &D_801E42EC;
            g_SeriesSelection = 0;
            if (D_801E40A0 == -1) {
                g_GrandPrixClass = 0;
                g_CourseIndex = 3;
                GameRequestTrackLoad();
            } else {
                func_8001839C();
            }
            break;
        case 1:
            g_CarTable = &D_8019C914;
            g_RaceProgress = (GameRaceProgress *)&D_801E6E7C;
            D_8009E67C = &D_8009E874;
            g_SeriesSelection = 1;
            if (D_801E6E88 == -1) {
                g_GrandPrixClass = 0;
                g_CourseIndex = 3;
                GameRequestTrackLoad();
            } else {
                func_8001839C();
            }
            break;
        case 2:
            g_CarTable = &D_801E4388;
            g_RaceProgress = (GameRaceProgress *)&D_8019C980;
            g_SeriesSelection = 0;
            func_8001839C();
            break;
        case 3:
            func_800182D0();
            break;
        case 4:
            func_80018B98();
            D_8019C7B4 = 0;
            break;
        }
        D_8009F098 = 3;
    }
    GameDrawMainMenuRows();
}
