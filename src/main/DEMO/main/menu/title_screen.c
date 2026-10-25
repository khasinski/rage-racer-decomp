#include "common.h"
#include "game/prim.h"
#include "game/asset.h"
#include "game/audio.h"
#include "game/audio_internal.h"
#include "game/cd.h"
#include "game/car.h"
#include "game/frontend_internal.h"
#include "game/menu.h"
#include "game/menu_internal.h"
#include "game/race.h"
#include "game/random.h"
#include "game/render.h"
#include "game/render_internal.h"
#include "game/fmv_internal.h"
#include "game/save_internal.h"
#include "game/scratchpad.h"
#include "game/screens.h"
#include "game/state.h"
#include "psyq/cd.h"
#include "psyq/gpu.h"

extern s32 g_TestMode;

/* Scene 2: the menu-side entry to the front end. Clears the title/menu
 * state words and hands over to scene 4, UpdateFrontend. */
void EnterFrontend(void) {
    SetDispMask(0);
    CloseLoadedAudioSlots();
    ResetTrackTextureSwap();
    UploadLoadBufferImage();

    g_FrameSyncThreshold = 0x80;
    g_SceneId = 4;
    g_SceneTimer = 0;
    g_FrontendIdleTimer = 0;
    g_TitleFadeLevel = 0;
    g_MainMenuSlide = 0;
    g_TitlePulse = 0;
    g_FrontendState = FRONTEND_STATE_TITLE;
    g_TitleExitTimer = 0;
    g_TitleAttractTimer = -1;

    UpdateBgmTrackCount();
    SetDefaultReverbDepth();
}


void EnterTitleScreen(void) {
    SetupDisplay240(0, 0, 0);
    if (g_StreamReturnScene != 0) {
        g_TitleFadeLevel = 0xFF;
        g_TitleAttractTimer = 0x1E0;
        g_TitleExitTimer = 0;
    } else {
        SetDispMask(0);
        UploadLoadBufferImage();
        g_TitleFadeLevel = 0;
        g_TitleAttractTimer = 0;
        g_TitleExitTimer = 0x1E;
    }
    g_FrameSyncThreshold = 0x80;
    g_SceneTimer = 0;
    g_SceneId = 4;
    g_FrontendIdleTimer = 0;
    g_MainMenuSlide = 0;
    g_FrontendState = FRONTEND_STATE_TITLE;
    UpdateBgmTrackCount();
    SetDefaultReverbDepth();
    DrawPressStartPrompt();
}


void DrawTitleFadeOverlay(s32 brightness) {
    void *current;
    void **scratch;
    u8 *base;
    void *next;
    s32 color;

    color = (u8)brightness;
    base = g_DrawBuffer;
    base += 0xCC;
    scratch = &SCRATCH_PRIM_CURSOR_AS(void);
    current = *scratch;
    next = GameQueueTileTrans(base, current, 0, 0x18, 0x140, 0xC0, color, color, color);
    *scratch = QueueDrawModePrim(base, next, 0x29);
}


/* Pre-release: the prompt sprite and its fade overlay are both suppressed
 * while the test harness is driving the build (g_TestMode != 0). Retail
 * (src/main/PAL/main/menu/title_screen.c) dropped the test harness and the
 * guard along with it. */
void DrawPressStartPrompt(void) {
    void **scratch;
    u8 *base;
    void *next;
    s32 sinValue;
    s32 frame;

    if (g_TestMode == 0) {
        if (g_TitleFadeLevel > 0) {
            DrawTitleFadeOverlay((u8)g_TitleFadeLevel);
            g_TitleFadeLevel -= 2;
        }

        sinValue = rsin(((g_AnimTimer * 3) << 5) & 0xFE0);
        frame = (sinValue / 64) + 0x80;

        scratch = &SCRATCH_PRIM_CURSOR_AS(void);
        base = g_DrawBuffer;
        base += 0xCC;
        next = *scratch;
        next = GameQueueShadedSprite(base, next, 0x68, 0xC8, 0x70, 0x10, 0x70, 0xA0, 0x7E84, frame);
        *scratch = QueueDrawModePrim(base, next, 0x39);
    }
}


/* Pre-release: pressing START commits straight to Grand Prix save 0 and the
 * menu-exit fade (retail's UpdateMainMenuInput case 0 confirm handler, run
 * here instead since this build has no row-selection menu yet). */
void UpdateTitleScreen(void) {
    if (g_PadPressed & PAD_START) {
        if (g_TestMode == 0) {
            PlaySoundCue(2);
        }
        if (g_AssetLoadState != 0) {
            ResetAssetLoader();
        }
        g_CarTable = g_GrandPrixCars;
        g_RaceProgress = &g_GrandPrixSave;
        g_CourseProgress = &g_GrandPrixCourseProgress;
        g_SeriesSelection = 0;
        RequestSelectBgmAssetsNoReset();
        g_FrontendState = FRONTEND_STATE_MENU_EXIT;
        g_FrontendIdleTimer = 0;
        g_TitleMenuSelection = 0;
        if (g_TitleAttractTimer > 0) {
            g_TitleAttractTimer = 0;
            StartCdVolumeFade(1);
        }
    }
    DrawPressStartPrompt();
}


void DrawMainMenuRows(void) {
    void *scratch;
    u8 *base;
    s32 row;
    s32 i;
    s32 one;
    s32 width;
    s32 y;

    base = g_DrawBuffer + 0xCC;
    scratch = SCRATCH_PRIM_CURSOR_AS(void);
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

        scratch = GameQueueTexturedRect(base, scratch, 0x68, y, width, frame, 0, (i * 16) + 0xA0, width, 0x10, code, 0x39);
        y += 0x18;
        i++;
        row++;
    }

    SCRATCH_PRIM_CURSOR_AS(void) = scratch;
}

void UpdateMainMenuOpen(void) {
    if (++g_MainMenuSlide == 0x30) {
        g_FrontendState = FRONTEND_STATE_MENU_INPUT;
    }

    DrawMainMenuRows();
}


/* Refills g_BgmShuffleOrder with a random permutation of the
 * g_BgmTrackCount tracks and rewinds g_BgmShuffleIndex. */
void ShuffleBgmOrder(void) {
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

        remaining = ((Random15() & 0xFFF) % count) + 1;
        j = 0;
        while (remaining != 0) {
            if (g_BgmShuffleOrder[j] == 0xFF) {
                remaining--;
            }
            j++;
        }
        g_BgmShuffleOrder[j - 1] = i;
    }

    g_BgmShuffleIndex = 0;
}


void UpdateMainMenuInput(void) {
    volatile u16 *flagp = &g_PadPressed;
    s32 idx;
    u16 flags;

    if (*flagp != 0) {
        g_FrontendIdleTimer = 0;
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
            PlaySoundCue(1);
        }
    }

    if (g_PadPressed & PAD_CONFIRM) {
        PlaySoundCue(2);
        if (g_AssetLoadState != 0) {
            ResetAssetLoader();
        }
        ShuffleBgmOrder();
        switch (g_TitleMenuSelection) {
        case 0:
            g_CarTable = g_GrandPrixCars;
            g_RaceProgress = &g_GrandPrixSave;
            g_CourseProgress = &g_GrandPrixCourseProgress;
            g_SeriesSelection = 0;
            if (g_GrandPrixSave.maxClassReached == -1) {
                g_GrandPrixClass = 0;
                g_CourseIndex = 3;
                RequestTrackLoad();
            } else {
                RequestSelectBgmAssetsNoReset();
            }
            break;
        case 1:
            g_CarTable = g_ExtraGrandPrixCars;
            g_RaceProgress = &g_ExtraGrandPrixSave;
            g_CourseProgress = &g_ExtraGrandPrixCourseProgress;
            g_SeriesSelection = 1;
            if (g_ExtraGrandPrixSaveMaxClass == -1) {
                g_GrandPrixClass = 0;
                g_CourseIndex = 3;
                RequestTrackLoad();
            } else {
                RequestSelectBgmAssetsNoReset();
            }
            break;
        case 2:
            g_CarTable = g_TimeAttackCars;
            g_RaceProgress = &g_TimeAttackSave;
            g_SeriesSelection = 0;
            RequestSelectBgmAssetsNoReset();
            break;
        case 3:
            RequestSaveScreenAssets();
            break;
        case 4:
            RequestOptionScreenAssets();
            g_OptionMenuCursor = 0;
            break;
        }
        g_FrontendState = FRONTEND_STATE_MENU_EXIT;
    }
    DrawMainMenuRows();
}


/* Pre-release: no row-selection menu to fade into yet, so the exit wipe
 * always commits to Grand Prix mode and race scene 6. Retail's
 * UpdateMainMenuExit (src/main/PAL/main/menu/frontend.c) switches on
 * g_TitleMenuSelection to pick among Grand Prix / Time Attack / save /
 * options instead. */
void UpdateMainMenuExit(void) {
    s32 value;

    value = g_TitlePulse + 1;
    g_TitlePulse = value;
    DrawFullscreenFadeTile(value * 2, 0x59);

    if (g_TitlePulse >= 0x81) {
        g_GrandPrixMode = 1;
        g_SceneId = 6;
    }
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


/* Pre-release: a much shorter state machine than retail's UpdateFrontend
 * (src/main/PAL/main/menu/frontend.c). The scene-timer thresholds are
 * retail's +0x50, the attract-mode race-load picks a fixed class/course
 * instead of calling Random15(), and the idle-timeout path always commits
 * to Grand Prix mode -- there is no FMV intro cycle or g_AttractCycleCount
 * parity check yet. */
void UpdateFrontend(void) {
    u32 state;

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
    if (state < 0x21C) {
        g_SceneTimer = state + 1;
    } else {
        if (g_FrontendState != FRONTEND_STATE_MENU_EXIT) {
            if (state == 0x21C) {
                g_GrandPrixSeries = 0;
                g_GrandPrixClass = 2;
                g_CourseIndex = 0;
                RequestTrackLoad();
                g_SceneTimer++;
            } else if (state == 0x21D) {
                if (g_AssetLoadState == 0) {
                    RequestRaceStart();
                    g_SceneTimer++;
                }
            } else if (state == 0x21E) {
                if (g_AssetLoadState == 0) {
                    g_SceneTimer = 0x21F;
                }
            }
        }
    }
    if (g_SceneTimer == 0xf) {
        SetDispMask(1);
    }
    if (g_SceneTimer == 1) {
        SetupDisplay240(0, 0, 0);
    }

    g_FrontendDrawHandlers[g_FrontendState]();

    if (g_FrontendIdleTimer < 900) {
        g_FrontendIdleTimer++;
    } else {
        if (g_SceneTimer == 0x21F) {
            g_GrandPrixMode = 1;
            g_SceneId = 0x1d;
        }
    }

    UpdateTitleAttract();
}
