#include "common.h"
#include "game/race.h"
#include "game/state.h"
#include "game/render.h"
#include "game/asset.h"
#include "game/menu.h"
#include "psyq/gpu.h"

void GameSetupDisplay240(s32 arg0, s32 arg1, s32 arg2);
void GameSetupDisplay480(s32 arg0, s32 arg1, s32 arg2);

extern s32 g_TitlePulse asm("D_801E429C");

void GameDrawFullscreenFadeTile(s32 arg0, s32 arg1) asm("func_80033AA0");
void GameDrawMainMenuRows(void) asm("func_8001B2D4");

void GameUpdateMainMenuExit(void) asm("func_8001B884");

void GameUpdateMainMenuExit(void) {
    s32 value;
    GameRaceProgress *ptr;

    value = g_TitlePulse + 1;
    g_TitlePulse = value;
    GameDrawFullscreenFadeTile(value * 2, 0x59);

    if (g_TitlePulse >= 0x81) {
        switch (g_TitleMenuSelection) {
        case 0:
        case 1:
            ptr = g_RaceProgress;
            g_GrandPrixMode = 1;
            if (ptr->maxClassReached == -1) {
                ptr->maxClassReached = 0;
                g_SceneId = 0x1F;
                g_GrandPrixSeries = 0;
            } else {
                g_SceneId = 6;
            }
            break;
        case 2:
            g_GrandPrixMode = 0;
            g_SceneId = 6;
            break;
        case 3:
            g_SceneId = 0x19;
            break;
        case 4:
            g_SceneId = 0x16;
            break;
        }
    }

    GameDrawMainMenuRows();
}

void GameUpdateFrontend(void) asm("func_8001BB58");

void GameUpdateTitleAttract(void) asm("func_8001B974");

extern s32 g_MainMenuSlide asm("D_801E6F1C");
extern s32 g_ClassWinCount asm("D_801E4DA8");

void *func_80016F8C(void *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9);
void *func_80017390(void *arg0, void *arg1, s32 arg2);
void *func_800173F4(void *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10);

void GameUpdateTitleAttract(void) {
    s32 alpha;
    void *base;
    s32 color;
    s32 h88;
    /* This pin is load-bearing: removing it changes .text. */
    register s32 scratch asm("$22");
    s32 hF0;
    s32 clut0;
    void *next;
    s32 tmp;
    s32 clamped;
    s32 x28;
    s32 yA0;

    tmp = g_MainMenuSlide;
    tmp <<= 1;
    clamped = 0x7F;
    alpha = clamped - tmp;
    if (alpha >= 0x30) {
        clamped = alpha;
        if (clamped >= 0x80) {
            clamped = 0x7F;
        }
    } else {
        clamped = 0x30;
    }
    alpha = clamped;

    x28 = 0x28;
    yA0 = 0xA0;
    /* This barrier is load-bearing: removing it changes .text. */
    asm("" : "=r"(x28), "=r"(yA0) : "0"(x28), "1"(yA0)); /* Match note: materialize first-call argument registers before the stack-arg temp. */
    color = 0x7E00;
    scratch = 0x1F800000;
    hF0 = 0xF0;
    /* This barrier is load-bearing: removing it changes .text. */
    asm("" : "=r"(scratch), "=r"(hF0) : "0"(scratch), "1"(hF0)); /* Match note: keep scratchpad base and 0xf0 materialized before the first stack-arg temp. */
    tmp = 0x18;
    next = *(void **)scratch;
    h88 = 0x88;
    clut0 = 0x7DC0;

    next = func_80016F8C(base = g_DrawBuffer + 0xD0, next, x28, yA0, hF0, tmp, 0, h88, clut0, alpha);
    next = func_80016F8C(base, next, 0x20, 0xB8, 0x100, 0x10, 0, hF0, 0x7DC1, alpha);
    next = func_80016F8C(base, next, 0x11A, 0xAF, 0xC, 8, 0xE0, 0xB0, clut0, alpha);
    next = func_80017390(base, next, 0x19);

    if (g_ClassWinCount >= 0xB) {
        color = 0x7D80;
    }

    next = func_80016F8C(base, next, 0x34, 0x18, 0x6C, h88, 0, 0, color, alpha);
    *(void **)scratch = func_800173F4(base, next, 0xA0, 0x18, -0x6C, h88, 0, 0, color, 0x99, alpha);
}
extern s32 g_TitleAttractTimer asm("D_8019CB70");
extern s32 g_TitleExitTimer asm("D_8009E880");
extern s32 g_FrontendState asm("D_8009F098");
extern s32 D_8007C744;
extern u32 D_801E8260;
extern void (*g_FrontendDrawHandlers[])(void) asm("D_8007C748");

s32 GameRandom15(void) asm("func_800632B0");
s32 CdControl(s32 com, void *param, s32 result) asm("func_8006A5A4");
void GamePlaySoundCue(s32 cue) asm("func_8005D6EC");
s32 GameRequestTrackLoad(void) asm("func_8001965C");
s32 GameRequestRaceStart(void) asm("func_80019580");
void GameBeginIntroFmv(int) asm("func_80019AF0");

void GameUpdateFrontend(void) {
    u32 state;
    s32 b, m4;

    g_AnimTimer++;
    GameRandom15();

    if (g_TitleAttractTimer > 0) {
        g_TitleAttractTimer--;
    }
    if (g_TitleAttractTimer == 0) {
        if (CdControl(9, 0, 0) == 1) {
            g_TitleAttractTimer--;
        }
    }
    if (g_TitleExitTimer != 0) {
        if (--g_TitleExitTimer == 0) {
            GamePlaySoundCue(0x1a);
        }
    }

    state = g_SceneTimer;
    if (state < 0x1cc) {
        g_SceneTimer = state + 1;
    } else {
        if (g_FrontendState == 3) goto Lcheck;
        if (D_8007C744 & 1) goto Lcheck;
        if (state == 0x1cc) {
            g_GrandPrixSeries = 0;
            g_GrandPrixClass = (GameRandom15() & 0xfff) % 5;
            b = GameRandom15() & 0xfff;
            m4 = b % 4;
            g_CourseIndex = m4;
            if (g_GrandPrixClass < 2 && m4 == 3) {
                g_CourseIndex = (GameRandom15() & 0xfff) % 3;
            }
            GameRequestTrackLoad();
            g_SceneTimer++;
        } else if (state == 0x1cd) {
            if (g_AssetLoadState == 0) {
                GameRequestRaceStart();
                g_SceneTimer++;
            }
        } else if (state == 0x1ce) {
            if (g_AssetLoadState == 0) {
                g_SceneTimer = 0x1cf;
            }
        }
    }
Lcheck:
    state = g_SceneTimer;
    if (state == 0xf) {
        SetDispMask(1);
        state = g_SceneTimer;
    }
    if (state == 1) {
        GameSetupDisplay240(0, 0, 0);
    }

    g_FrontendDrawHandlers[g_FrontendState]();

    if (D_801E8260 < 900) {
        D_801E8260++;
    } else {
        if (D_8007C744 & 1) {
            GameBeginIntroFmv(3);
            D_8007C744++;
        } else {
            if (g_SceneTimer == 0x1cf) {
                g_GrandPrixMode = 1;
                g_SceneId = 0x1d;
                D_8007C744++;
            }
        }
    }

    GameUpdateTitleAttract();
}

/*
 * Empty stub; GameSetupDisplay240 and GameSetupDisplay480 both call it with one argument,
 * so the parameter is declared and ignored.
 */
void func_8001BE94(int arg0) {
}

extern volatile u8 g_FrameContexts[] asm("D_8019CE38");
extern u8 D_801C0620[];
extern u8 g_DispEnv1X[] asm("D_801C067C");
extern u8 D_801C0690[];
extern u16 g_ScreenOffsetX asm("D_801E4B8C");
extern u16 g_ScreenOffsetY asm("D_801E4B9C");

void func_80069A58(s32, s32);
void func_80069A78(s32);

void GameSetupDisplay240(s32 arg0, s32 arg1, s32 arg2) asm("func_8001BE9C");
void GameSetupDisplay240(s32 arg0, s32 arg1, s32 arg2) {
    /* These pins are load-bearing: removing any one changes .text. */
    register s32 a0_save asm("$18") = arg0;
    register s32 a1_save asm("$19") = arg1;
    s32 a2_save = arg2;
    u8 *base;
    s32 height;
    u16 *src0;
    u16 *src1;
    s32 i;
    s32 offset;
    s32 one;
    s32 stride;
    /* This pin is load-bearing: removing it changes .text. */
    register u16 value asm("$2");

    __asm__("" : : "r"(a0_save), "r"(a1_save), "r"(a2_save));
    func_80069A58(0xA0, 0x78);
    func_80069A78(0x140);

    base = (u8 *)g_FrameContexts;
    height = 0xF0;
    SetDefDrawEnv(base, 0, 0, 0x140, height);
    SetDefDrawEnv(D_801C0620, 0, 0xF0, 0x140, height);
    SetDefDispEnv(base + 0x5C, 0, 0xF0, 0x140, height);
    SetDefDispEnv(g_DispEnv1X, 0, 0, 0x140, height);

    {
        /* These pins are load-bearing: removing any one changes .text. */
        register void *ptr;
        register s32 arg1;
        register s32 arg2;
        register s32 arg3;
        s32 small_height;

        ptr = base + 0x70;
        arg1 = 0x56;
        arg2 = 0x12;
        arg3 = 0x94;
        small_height = 0x24;
        SetDefDrawEnv(ptr, arg1, arg2, arg3, small_height);
        SetDefDrawEnv(D_801C0690, 0x56, 0x102, 0x94, small_height);
    }

    i = 0;
    one = 1;
    src0 = &g_ScreenOffsetX;
    src1 = &g_ScreenOffsetY;
    offset = 0;
    do {
        stride = 0x20000;
        g_FrameContexts[offset + 0x16] = one;
        g_FrameContexts[offset + 0x18] = one;
        g_FrameContexts[offset + 0x19] = a0_save;
        g_FrameContexts[offset + 0x1A] = a1_save;
        g_FrameContexts[offset + 0x1B] = a2_save;
        value = *src0;
        stride |= 0x37E8;
        *(volatile u16 *)(g_FrameContexts + offset + 0x64) = value;
        value = *src1;
        i++;
        g_FrameContexts[offset + 0x86] = one;
        g_FrameContexts[offset + 0x88] = 0;
        g_FrameContexts[offset + 0x89] = a0_save;
        g_FrameContexts[offset + 0x8A] = a1_save;
        g_FrameContexts[offset + 0x8B] = a2_save;
        *(volatile u16 *)(g_FrameContexts + offset + 0x66) = value + 0x1D;
        offset += stride;
    } while (i < 2);

    func_8001BE94(0);
    func_8001BE94(1);

    asm(".globl func_8001C05C\nfunc_8001C05C = func_8001BE9C + 0x1C0");
}

void GameSetupDisplay480(s32 arg0, s32 arg1, s32 arg2) asm("func_8001C088");
void GameSetupDisplay480(s32 arg0, s32 arg1, s32 arg2) {
    /* These pins are load-bearing: removing any one changes .text. */
    register s32 a0_save asm("$18") = arg0;
    register s32 a1_save asm("$19") = arg1;
    s32 a2_save = arg2;
    u8 *base = (u8 *)g_FrameContexts;
    s32 height;
    u16 *src0;
    u16 *src1;
    s32 i;
    s32 offset;
    s32 one;
    s32 stride;
    /* This pin is load-bearing: removing it changes .text. */
    register u16 value asm("$2");

    __asm__("" : : "r"(a0_save), "r"(a1_save), "r"(a2_save));
    func_80069A58(0xA0, 0xF0);
    func_80069A78(0x140);

    height = 0x1E0;
    SetDefDrawEnv(base, 0, 0, 0x140, height);
    SetDefDrawEnv(D_801C0620, 0, 0, 0x140, height);
    SetDefDispEnv(base + 0x5C, 0, 0, 0x140, height);
    SetDefDispEnv(g_DispEnv1X, 0, 0, 0x140, height);

    i = 0;
    one = 1;
    src0 = &g_ScreenOffsetX;
    src1 = &g_ScreenOffsetY;
    offset = 0;
    do {
        stride = 0x20000;
        g_FrameContexts[offset + 0x16] = one;
        g_FrameContexts[offset + 0x18] = one;
        g_FrameContexts[offset + 0x19] = a0_save;
        g_FrameContexts[offset + 0x1A] = a1_save;
        g_FrameContexts[offset + 0x1B] = a2_save;
        value = *src0;
        stride |= 0x37E8;
        *(volatile u16 *)(g_FrameContexts + offset + 0x64) = value;
        value = *src1;
        i++;
        g_FrameContexts[offset + 0x86] = one;
        g_FrameContexts[offset + 0x88] = 0;
        g_FrameContexts[offset + 0x89] = a0_save;
        g_FrameContexts[offset + 0x8A] = a1_save;
        g_FrameContexts[offset + 0x8B] = a2_save;
        *(volatile u16 *)(g_FrameContexts + offset + 0x66) = value + 0x1D;
        offset += stride;
    } while (i < 2);

    func_8001BE94(0);
    func_8001BE94(1);

    *(u16 *)0x1F80007E = 0x1E0;
    asm(".globl func_8001C218\nfunc_8001C218 = func_8001C088 + 0x190");
}
