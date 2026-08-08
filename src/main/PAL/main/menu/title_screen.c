#include "common.h"
#include "game/asset.h"
#include "game/audio.h"
#include "game/cd.h"
#include "game/menu.h"
#include "game/race.h"
#include "game/random.h"
#include "game/render.h"
#include "game/scratchpad.h"
#include "game/screens.h"
#include "game/state.h"
#include "psyq/gpu.h"

extern s32 D_801E8260;

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
    D_801E8260 = 0;
    D_801E6F28 = 0;
    g_MainMenuSlide = 0;
    g_TitlePulse = 0;
    g_FrontendState = 0;
    g_TitleExitTimer = 0;
    g_TitleAttractTimer = -1;

    UpdateBgmTrackCount();
    SetDefaultReverbDepth();
}

extern s32 g_StreamReturnScene;

void EnterTitleScreen(void) {
    SetupDisplay240(0, 0, 0);
    if (g_StreamReturnScene != 0) {
        D_801E6F28 = 0xFF;
        g_TitleAttractTimer = 0x190;
        g_TitleExitTimer = 0;
    } else {
        SetDispMask(0);
        UploadLoadBufferImage();
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
    UpdateBgmTrackCount();
    SetDefaultReverbDepth();
    DrawPressStartPrompt();
}

void *GameQueueDrawModePrimWide(void* ot, void* prim, s32 tpage) asm("QueueDrawModePrim");

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
    *scratch = GameQueueDrawModePrimWide(base, next, 0x29);
}

s32 rsin(s32 angle);
void *QueueShadedSpriteNine(void* ot, void* prim, s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 clutIndex, s32 intensity) asm("GameQueueShadedSprite");
void *GameQueueDrawModePrimWide(void *ot, void *prim, s32 tpage);

void DrawPressStartPrompt(void) {
    void **scratch;
    u8 *base;
    void *next;
    s32 sinValue;
    s32 frame;

    if (D_801E6F28 > 0) {
        DrawTitleFadeOverlay((u8)D_801E6F28);
        D_801E6F28 -= 2;
    }

    sinValue = rsin(((g_AnimTimer * 3) << 5) & 0xFE0);
    frame = (sinValue / 64) + 0x80;

    scratch = &SCRATCH_PRIM_CURSOR_AS(void);
    base = g_DrawBuffer;
    base += 0xCC;
    next = *scratch;
    next = QueueShadedSpriteNine(base, next, 0x68, 0xC8, 0x70, 0x10, 0x70, 0xA0, 0x7E84, frame);
    *scratch = GameQueueDrawModePrimWide(base, next, 0x39);
}


void UpdateTitleScreen(void) {
    if (g_PadPressed & PAD_START) {
        PlaySoundCue(2);
        g_FrontendState = 1;
        D_801E8260 = 0;
        g_TitleMenuSelection = 0;
        if (g_TitleAttractTimer > 0) {
            g_TitleAttractTimer = 0;
            StartCdVolumeFade(1);
        }
    }
    DrawPressStartPrompt();
}

/* This call site hands full words where GameQueueTexturedRect's parameters are
 * s16/u8; the alias names the routine, not its address, so the truncation the
 * header would insert never happens here. */
u8 *GameQueueTexturedRectWide(
    void *ot, u8 *prim, s32 x, s32 y, s32 w, s32 h, s32 u, s32 v,
    s32 uSpan, s32 vSpan, s32 clutIndex, s32 tpage) asm("GameQueueTexturedRect");

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

        scratch = GameQueueTexturedRectWide(base, scratch, 0x68, y, width, frame, 0, (i * 16) + 0xA0, width, 0x10, code, 0x39);
        y += 0x18;
        i++;
        row++;
    }

    SCRATCH_PRIM_CURSOR_AS(void) = scratch;
}

void UpdateMainMenuOpen(void) {
    if (++g_MainMenuSlide == 0x30) {
        g_FrontendState = 2;
    }

    DrawMainMenuRows();
}

extern volatile u8 g_BgmShuffleOrder[];
extern s32 g_BgmShuffleIndex;

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
        D_801E7733[j] = i;
    }

    g_BgmShuffleIndex = 0;
}


extern s32 *g_CarTable;
extern s32 *g_CourseProgress;

extern s32 g_GrandPrixCars;
extern s32 g_GrandPrixCourseProgress;
extern s32 g_ExtraGrandPrixCars;
extern s32 g_ExtraGrandPrixCourseProgress;
extern s32 g_TimeAttackCars;

extern void ShuffleBgmOrder(void);
extern void RequestOptionScreenAssets(void);

void UpdateMainMenuInput(void) {
    volatile u16 *flagp = &g_PadPressed;
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
            g_CarTable = &g_GrandPrixCars;
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
            g_CarTable = &g_ExtraGrandPrixCars;
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
            g_CarTable = &g_TimeAttackCars;
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
        g_FrontendState = 3;
    }
    DrawMainMenuRows();
}
