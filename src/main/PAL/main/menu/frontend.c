#include "common.h"
#include "game/race.h"
#include "game/state.h"
#include "game/render.h"
#include "game/asset.h"
#include "game/menu.h"
#include "psyq/gpu.h"
#include "game/audio.h"
#include "game/random.h"
#include "game/screens.h"
#include "psyq/cd.h"

void DrawFullscreenFadeTile(s32 arg0, s32 arg1);

void UpdateMainMenuExit(void) {
    s32 value;
    GameRaceProgress *ptr;

    value = g_TitlePulse + 1;
    g_TitlePulse = value;
    DrawFullscreenFadeTile(value * 2, 0x59);

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

    DrawMainMenuRows();
}

extern s32 g_ClassWinCount;

void *QueueShadedSpriteNine(void *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9) asm("GameQueueShadedSprite");
void *GameQueueDrawModePrimWide(void *arg0, void *arg1, s32 arg2) asm("QueueDrawModePrim");
void *GameQueueShadedTexturedRectWide(void *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10) asm("GameQueueShadedTexturedRect");

void UpdateTitleAttract(void) {
    s32 alpha;
    void *base;
    s32 color;
    s32 h88;
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
    asm("" : "=r"(x28), "=r"(yA0) : "0"(x28), "1"(yA0)); /* Match note: materialize first-call argument registers before the stack-arg temp. */
    color = 0x7E00;
    scratch = 0x1F800000;
    hF0 = 0xF0;
    asm("" : "=r"(scratch), "=r"(hF0) : "0"(scratch), "1"(hF0)); /* Match note: keep scratchpad base and 0xf0 materialized before the first stack-arg temp. */
    tmp = 0x18;
    next = *(void **)scratch;
    h88 = 0x88;
    clut0 = 0x7DC0;

    next = QueueShadedSpriteNine(base = g_DrawBuffer + 0xD0, next, x28, yA0, hF0, tmp, 0, h88, clut0, alpha);
    next = QueueShadedSpriteNine(base, next, 0x20, 0xB8, 0x100, 0x10, 0, hF0, 0x7DC1, alpha);
    next = QueueShadedSpriteNine(base, next, 0x11A, 0xAF, 0xC, 8, 0xE0, 0xB0, clut0, alpha);
    next = GameQueueDrawModePrimWide(base, next, 0x19);

    if (g_ClassWinCount >= 0xB) {
        color = 0x7D80;
    }

    next = QueueShadedSpriteNine(base, next, 0x34, 0x18, 0x6C, h88, 0, 0, color, alpha);
    *(void **)scratch = GameQueueShadedTexturedRectWide(base, next, 0xA0, 0x18, -0x6C, h88, 0, 0, color, 0x99, alpha);
}
extern u32 D_801E8260;
extern void (*g_FrontendDrawHandlers[])(void);


void UpdateFrontend(void) {
    u32 state;
    s32 b, m4;

    g_AnimTimer++;
    Random15();

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
            PlaySoundCue(0x1a);
        }
    }

    state = g_SceneTimer;
    if (state < 0x1cc) {
        g_SceneTimer = state + 1;
    } else {
        if (!(g_FrontendState == 3) && !(D_8007C744 % 2)) {
        if (state == 0x1cc) {
            g_GrandPrixSeries = 0;
            g_GrandPrixClass = (Random15() & 0xfff) % 5;
            b = Random15() & 0xfff;
            m4 = b % 4;
            g_CourseIndex = m4;
            if (g_GrandPrixClass < 2 && m4 == 3) {
                g_CourseIndex = (Random15() & 0xfff) % 3;
            }
            RequestTrackLoad();
            g_SceneTimer++;
        } else if (state == 0x1cd) {
            if (g_AssetLoadState == 0) {
                RequestRaceStart();
                g_SceneTimer++;
            }
        } else if (state == 0x1ce) {
            if (g_AssetLoadState == 0) {
                g_SceneTimer = 0x1cf;
            }
        }
        }
    }
    state = g_SceneTimer;
    if (state == 0xf) {
        SetDispMask(1);
        state = g_SceneTimer;
    }
    if (state == 1) {
        SetupDisplay240(0, 0, 0);
    }

    g_FrontendDrawHandlers[g_FrontendState]();

    if (D_801E8260 < 900) {
        D_801E8260++;
    } else {
        if (D_8007C744 % 2) {
            BeginIntroFmv(3);
            D_8007C744++;
        } else {
            if (g_SceneTimer == 0x1cf) {
                g_GrandPrixMode = 1;
                g_SceneId = 0x1d;
                D_8007C744++;
            }
        }
    }

    UpdateTitleAttract();
}

/*
 * Empty stub; SetupDisplay240 and SetupDisplay480 both call it with one argument,
 * so the parameter is declared and ignored.
 */
void ResetFrameContext(int arg0) {
}

extern volatile u8 g_FrameContexts[];
extern u8 g_DispEnv1X[];
extern u16 g_ScreenOffsetX;
extern u16 g_ScreenOffsetY;

void SetupDisplay240(s32 arg0, s32 arg1, s32 arg2) {
    u8 *base;
    s32 height;
    u16 *src0;
    u16 *src1;
    s32 i;
    s32 offset;
    s32 one;
    s32 stride;
    u16 value;
    u16 value2;

    SetGeomOffset(0xA0, 0x78);
    SetGeomScreen(0x140);

    base = (u8 *)g_FrameContexts;
    height = 0xF0;
    SetDefDrawEnv((DrawEnv *)base, 0, 0, 0x140, height);
    SetDefDrawEnv((DrawEnv *)D_801C0620, 0, 0xF0, 0x140, height);
    SetDefDispEnv(base + 0x5C, 0, 0xF0, 0x140, height);
    SetDefDispEnv(g_DispEnv1X, 0, 0, 0x140, height);

    {
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
        SetDefDrawEnv((DrawEnv *)ptr, arg1, arg2, arg3, small_height);
        SetDefDrawEnv((DrawEnv *)D_801C0690, 0x56, 0x102, 0x94, small_height);
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
        g_FrameContexts[offset + 0x19] = arg0;
        g_FrameContexts[offset + 0x1A] = arg1;
        g_FrameContexts[offset + 0x1B] = arg2;
        value = *src0;
        stride |= 0x37E8;
        *(volatile u16 *)(g_FrameContexts + offset + 0x64) = value;
        value2 = *src1;
        i++;
        g_FrameContexts[offset + 0x86] = one;
        g_FrameContexts[offset + 0x88] = 0;
        g_FrameContexts[offset + 0x89] = arg0;
        g_FrameContexts[offset + 0x8A] = arg1;
        g_FrameContexts[offset + 0x8B] = arg2;
        *(volatile u16 *)(g_FrameContexts + offset + 0x66) = value2 + 0x1D;
        offset += stride;
    } while (i < 2);

    ResetFrameContext(0);
    ResetFrameContext(1);

    asm(".globl func_8001C05C\nfunc_8001C05C = func_8001BE9C + 0x1C0");
}

void SetupDisplay480(s32 arg0, s32 arg1, s32 arg2) {
    u8 *base = (u8 *)g_FrameContexts;
    s32 height;
    u16 *src0;
    u16 *src1;
    s32 i;
    s32 offset;
    s32 one;
    s32 stride;
    u16 value;
    u16 value2;

    SetGeomOffset(0xA0, 0xF0);
    SetGeomScreen(0x140);

    height = 0x1E0;
    SetDefDrawEnv((DrawEnv *)base, 0, 0, 0x140, height);
    SetDefDrawEnv((DrawEnv *)D_801C0620, 0, 0, 0x140, height);
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
        g_FrameContexts[offset + 0x19] = arg0;
        g_FrameContexts[offset + 0x1A] = arg1;
        g_FrameContexts[offset + 0x1B] = arg2;
        value = *src0;
        stride |= 0x37E8;
        *(volatile u16 *)(g_FrameContexts + offset + 0x64) = value;
        value2 = *src1;
        i++;
        g_FrameContexts[offset + 0x86] = one;
        g_FrameContexts[offset + 0x88] = 0;
        g_FrameContexts[offset + 0x89] = arg0;
        g_FrameContexts[offset + 0x8A] = arg1;
        g_FrameContexts[offset + 0x8B] = arg2;
        *(volatile u16 *)(g_FrameContexts + offset + 0x66) = value2 + 0x1D;
        offset += stride;
    } while (i < 2);

    ResetFrameContext(0);
    ResetFrameContext(1);

    SCRATCH_CLIP_Y1 = 0x1E0;
    asm(".globl func_8001C218\nfunc_8001C218 = func_8001C088 + 0x190");
}
