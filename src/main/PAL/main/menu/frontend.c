#include "common.h"
#include "game/prim.h"
#include "game/asset.h"
#include "game/audio.h"
#include "game/menu.h"
#include "game/menu_internal.h"
#include "game/frontend_internal.h"
#include "game/race.h"
#include "game/random.h"
#include "game/render.h"
#include "game/render_internal.h"
#include "game/scratchpad.h"
#include "game/screens.h"
#include "game/state.h"
#include "psyq/cd.h"
#include "psyq/gpu.h"

#define FRAME_CONTEXT_ENVIRONMENT(offset)                                      \
    (*(volatile GameFrameEnvironmentHeader *)(g_FrameContexts[0].bytes + (offset)))


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



void UpdateTitleAttract(void) {
    s32 alpha;
    void *base;
    s32 color;
    s32 h88;
    register RenderBufferAddress scratch asm("$22");
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
    scratch.value = SCRATCHPAD_ADDR;
    hF0 = 0xF0;
    asm("" : "=r"(scratch.value), "=r"(hF0) : "0"(scratch.value), "1"(hF0)); /* Match note: keep scratchpad base and 0xf0 materialized before the first stack-arg temp. */
    tmp = 0x18;
    next = *scratch.pointerLink;
    h88 = 0x88;
    clut0 = 0x7DC0;

    next = GameQueueShadedSprite(base = g_DrawBuffer + 0xD0, next, x28, yA0, hF0, tmp, 0, h88, clut0, alpha);
    next = GameQueueShadedSprite(base, next, 0x20, 0xB8, 0x100, 0x10, 0, hF0, 0x7DC1, alpha);
    next = GameQueueShadedSprite(base, next, 0x11A, 0xAF, 0xC, 8, 0xE0, 0xB0, clut0, alpha);
    next = QueueDrawModePrim(base, next, 0x19);

    if (g_ClassWinCount >= 0xB) {
        color = 0x7D80;
    }

    next = GameQueueShadedSprite(base, next, 0x34, 0x18, 0x6C, h88, 0, 0, color, alpha);
    *scratch.pointerLink = GameQueueShadedTexturedRect(base, next, 0xA0, 0x18, -0x6C, h88, 0, 0, color, 0x99, alpha);
}


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
        if (!(g_FrontendState == FRONTEND_STATE_MENU_EXIT) &&
            !(g_AttractCycleCount % 2)) {
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

    if (g_FrontendIdleTimer < 900) {
        g_FrontendIdleTimer++;
    } else {
        if (g_AttractCycleCount % 2) {
            BeginIntroFmv(3);
            g_AttractCycleCount++;
        } else {
            if (g_SceneTimer == 0x1cf) {
                g_GrandPrixMode = 1;
                g_SceneId = 0x1d;
                g_AttractCycleCount++;
            }
        }
    }

    UpdateTitleAttract();
}


#undef FRAME_CONTEXT_ENVIRONMENT
