#include "common.h"
#include "game/screens.h"
#include "game/race.h"
#include "game/state.h"
#include "game/render.h"
#include "game/menu.h"
#include "game/car.h"
#include "game/cd.h"

typedef struct GrandPrixIntroPosition {
    u8 left;
    u8 right;
} GrandPrixIntroPosition;

typedef struct GrandPrixIntroLayout {
    u8 x;
    u8 y;
    u8 width;
} GrandPrixIntroLayout;

typedef union {
    s16 palette;
    s16 layout;
    s16 width;
    s16 color;
} GrandPrixIntroSelection;

void *GameQueueTileTransWide(
    void *ot, void *prim, s32 x, s32 y, s32 w, s32 h, s32 r, s32 g, s32 b) asm("GameQueueTileTrans");

void *GameQueueDrawModePrimWide(void *ot, void *prim, s32 arg2) asm("QueueDrawModePrim");
extern s32 g_SeriesCleared;
extern s32 g_EndingWashLevel;
extern s32 g_ReplayFrameCount;
extern s32 g_ReplayBufferWrapped;
extern u8 g_PlayerCar;
extern s32 g_ReplayReadCursor;
extern s16 g_PlayerTrackSection;
void PlaySoundCue(s32 cue);
void DrawFullscreenFadeTile(s32 arg0, s32 arg1);
void DrawSeriesClearedWash(s32 arg0, s32 arg1);
void UpdateReplayCars(void);
void UpdateCamera(s32 arg0, void *arg1);
void DrawTerrainCellsWide(void);
void DrawPlayerCarOnly(void);
void DrawCourseScenery2(s32 arg0, s32 arg1);
void DrawSkyBackground(void);
void DrawReplayBadge(void);
void SetTrackTexturePageNow(s32 arg0);
void ApplyReplayFrame(s32 arg0, void *arg1, void *arg2);
extern char g_TextResult[];
extern char *g_CourseNames[];
void DrawText8x8Trans(s32 arg0, s32 arg1, void *arg2, s32 arg3);
s32 AddTilePrim(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
extern s32 g_RaceTotalTime;
extern s32 g_ClassResultPlace;
extern s32 g_PrizeScreenState;
extern s32 g_BestTotalTimes[][4][2];
extern s32 g_PlayerLapTimes[];
/* Grand Prix result panel: the finishing-position graphic's per-place sprite
 * geometry and the two clut tables its two sprites pick from. */
extern u16 g_ResultPlaceCluts[];
extern u16 g_ResultPanelCluts[];
extern char g_FmtClassGrandPrix[];
extern char g_FmtRoundIn[];
/*
 * Three captions drawn with the proportional font. Their bytes are lowercase
 * letters, which func_80016B7C routes through a separate word-sprite bank
 * (a..u index the 4-byte records at 0x8007C438), so the glyphs themselves are
 * on the disc. Each is named for the value it labels, which is what the call
 * sites prove: "hai" heads the ranking rows, "hegi" the total time ("T/…")
 * and "hfgi" the per-lap list.
 */
extern char g_CaptionRanking[];
extern char g_CaptionTotalTime[];
extern char g_CaptionLapTime[];
extern GrandPrixIntroLayout g_ResultPlaceSprites[];
extern GrandPrixIntroPosition g_ClassPlaceBarSizes[];

void LibcSprintf(void *dst, void *fmt, ...);
void DrawResultScreen(void);
void *FormatLapTime(void *dst, s32 value);

void DrawSeriesClearedWash(s32 x, s32 y) {
    void *ot;
    void *prim;
    s32 redStack;
    s32 green;
    register s32 temp asm("$3");
    register s32 quotient asm("$2");
    s32 red;
    s32 blue;
    s32 width;
    s32 height;

    ot = *(u8 **)((u8 *)0x801A0000 - 0x3700) + 0xCC;
    prim = *(void **)0x1F800000;

    redStack = y;
    /* Match note: make GCC materialize the red clamp from a3 into a1. */
    temp = x;
    if (x < 0) {
        temp = x + 7;
    }
    quotient = temp >> 3;
    green = quotient + redStack;
    if (x < 0) {
        x += 3;
    }
    blue = (x >> 2) + redStack;

    if (redStack >= 0) {
        red = redStack;
        if (red >= 0x100) {
            red = 0xFF;
        }
    } else {
        red = 0;
    }
    redStack = red;

    if (green >= 0) {
        temp = green;
        if (temp >= 0x100) {
            temp = 0xFF;
        }
    } else {
        temp = 0;
    }
    green = temp;

    if (blue >= 0) {
        temp = blue;
        if (temp >= 0x100) {
            temp = 0xFF;
        }
    } else {
        temp = 0;
    }

    width = 0x140;
    height = 0xF0;
    prim = GameQueueTileTransWide(ot, prim, 0, 0, width, height, redStack, green, temp);
    *(void **)0x1F800000 = GameQueueDrawModePrimWide(ot, prim, 0x49);
}

void UpdateReplayScene(void) {
    g_AnimTimer++;
    g_SceneTimer++;
    if (g_SceneTimer == 0x3C) {
        if (g_GrandPrixMode != 0) {
            if (g_SeriesCleared == 0) {
                PlaySoundCue(g_RacePosition == 1 ? 0x40 : 0x41);
            }
        }
    }

    if (g_FadeStep < 0) {
        g_FadeLevel += g_FadeStep;
        if (g_FadeLevel < 0) {
            g_FadeLevel = 0;
            g_FadeStep = 0;
            g_EndingWashLevel = 0;
        }
        DrawFullscreenFadeTile(g_FadeLevel, 0x29);
    } else {
        if (g_SeriesCleared != 0) {
            s32 cb = g_ReplayFrameCount;
            s32 fc = g_SceneTimer;
            if ((u32)(cb - 600) < (u32)fc) {
                s32 t = fc + 600;
                s32 c;
                t = t - cb;
                g_EndingWashLevel = t;
                if (t >= 0) {
                    c = t;
                    if (t >= 256) {
                        c = 255;
                    }
                } else {
                    c = 0;
                }
                g_EndingWashLevel = c;
            }
        }

        if (g_FadeStep == 0) {
            if ((g_PadEdge2 & 0x860) != 0) {
                g_FadeStep = 4;
                StartCdVolumeFade(0x3C);
            } else if (g_SceneTimer == g_ReplayFrameCount - 68) {
                g_FadeStep = 4;
                if (g_ReplayBufferWrapped == 0) {
                    StartCdVolumeFade(0x3C);
                }
            }
        } else {
            g_FadeLevel += g_FadeStep;
            if (g_FadeLevel >= 257) {
                s32 v = g_GrandPrixMode;
                g_MirrorMode = 0;
                g_SceneId = v == 0 ? 0x14 : 0x12;
            }
        }

        if (g_SeriesCleared != 0) {
            if (((u32)(g_ReplayFrameCount - 600) < (u32)g_SceneTimer) || (g_FadeLevel != 0)) {
                DrawSeriesClearedWash(g_EndingWashLevel, g_FadeLevel);
            }
        } else {
            if (g_FadeLevel != 0) {
                DrawFullscreenFadeTile(g_FadeLevel, 0x49);
            }
        }
    }

    ApplyReplayFrame(g_ReplayReadCursor, &g_PlayerCar, (u8 *)g_Cars);
    g_ReplayReadCursor++;
    if (g_ReplayReadCursor == g_ReplayFrameCount) {
        g_ReplayReadCursor = 0;
    }
    UpdateReplayCars();
    UpdateCamera(2, &g_PlayerCar);
    *(s32 *)0x1F800084 = g_IsEnvironmentMode4;
    DrawTerrainCellsWide();
    if (g_GrandPrixMode != 0) {
        DrawPlayerCarOnly();
    }
    DrawCourseObjects();
    DrawCourseScenery2(g_SceneTimer, 1);
    UpdateEnvironment();
    DrawSkyBackground();
    DrawReplayBadge();
    if (g_SceneTimer == 1) {
        SetTrackTexturePageNow(g_PlayerTrackSection);
    }
}

void DrawResultScreen(void) {
    u8 *base;
    s32 *scratch;
    s32 width;
    volatile s32 pad[12];
    s32 y;
    s32 next;

    (void)pad;
    DrawProportionalText(0xDC, 0x1C, g_TextResult, 0x7812);

    if (g_GrandPrixMode != 0) {
        y = 0x3C;
    } else {
        y = 0x39;
    }
    DrawText8x8Trans(0x60, y, g_CourseNames[g_CourseIndex], 0x78CC);

    width = 0x140;
    base = g_DrawBuffer;
    scratch = (s32 *)0x1F800000;
    base += 0xCC;

    next = *scratch;
    next = AddTilePrim(base, next, 0, 0, width, 0x30, 0x85, 0x15, 0xE);
    *scratch = AddTilePrim(base, next, 0, 0x30, width, 0x18, 0xF0, 0xF0, 0xF0);
}

void DrawGrandPrixResultPanel(void) asm("DrawGrandprixIntro");
void DrawGrandPrixResultPanel(void) {
    u8 *base;
    char text[0x30];
    if ((g_ClassResultPlace != 0) && (g_PrizeScreenState >= 5)) {
        s32 *scratch;
        s32 next;
        s32 height;
        s32 color;

        scratch = (s32 *)0x1F800000;
        base = g_DrawBuffer + 0xCC;
        height = 8;
        color = 0x78CB;
        next = (s32)GameQueueSprite(
            base, *scratch, 0x14, 0x1C, 0x38, height, 0, 0xE8, color);
        next = (s32)GameQueueSprite(
            base,
            next,
            0x4C,
            0x1C,
            g_ClassPlaceBarSizes[g_ClassResultPlace - 1].right,
            height,
            0x84,
            g_ClassPlaceBarSizes[g_ClassResultPlace - 1].left,
            color);
        next = (s32)GameQueueSprite(
            base,
            next,
            g_ClassPlaceBarSizes[g_ClassResultPlace - 1].right + 0x4E,
            0x1C,
            0x30,
            height,
            0,
            0xF0,
            color);
        next = (s32)GameQueueSprite(
            base,
            next,
            g_ClassPlaceBarSizes[g_ClassResultPlace - 1].right + 0x7C,
            0x1C,
            0x20,
            height,
            0,
            0xF8,
            color);
        *scratch = next;
    }

    {
        char *name;
        s32 classNumber;
        s32 current;

        current = g_GrandPrixClass;
        classNumber = current + 1;
        name = g_GrandPrixNames[g_GrandPrixSeries ? current + 6 : current];
        LibcSprintf(text, g_FmtClassGrandPrix, classNumber, name);
    }
    DrawText8x8Trans(0x10, 0x34, text, 0x78CC);

    LibcSprintf(text, g_FmtRoundIn, g_GrandPrixRound);
    DrawText8x8Trans(0x10, 0x3C, text, 0x78CC);

    {
        s32 *scratch;
        GrandPrixIntroSelection *selection;
        s32 next;
        s32 selectionIndex;

        scratch = (s32 *)0x1F800000;
        DrawResultScreen();

        base = g_DrawBuffer + 0xCC;
        selection = (GrandPrixIntroSelection *)&g_RacePosition;
        next = (s32)GameQueueSprite(
            base,
            *scratch,
            0xB4,
            0x60,
            0x58,
            0x38,
            0xA8,
            0xA8,
            g_ResultPanelCluts[selection->palette]);

        selectionIndex = selection->layout;
        selectionIndex -= 1;
        next = (s32)GameQueueSprite(
            base,
            next,
            g_ResultPlaceSprites[selectionIndex].x,
            0x5C,
            g_ResultPlaceSprites[selectionIndex].y,
            0x1C,
            g_ResultPlaceSprites[selection->width - 1].width,
            0xCC,
            g_ResultPlaceCluts[selection->color]);
        *scratch = next;
    }

    DrawProportionalText(0x10, 0x50, g_CaptionRanking, 0x7812);
}

void DrawRaceTimePanel(s32 arg0) {
    s32 base;
    s32 i;
    s32 *times;
    s32 *selectedPtr;
    s32 count;
    s32 x;
    s32 textPos;
    s32 column;
    s32 columnBase;
    s32 drawColor;
    s32 quotient;
    char text[24];
    s32 color;

    base = arg0;
    DrawProportionalText(0x10, base + 0x80, g_CaptionTotalTime, 0x7812);

    text[0] = 0x54;
    text[1] = 0x2F;
    FormatLapTime(&text[2], g_RaceTotalTime);

    color = 0x7812;
    if (g_BestTotalTimes[g_GrandPrixSeries][g_CourseIndex][g_GrandPrixMode] == g_RaceTotalTime) {
        color = 0x784C;
    }
    drawColor = color;
    count = base + 0x90;
    DrawProportionalText(0x14, count, text, drawColor);

    DrawProportionalText(0x10, base + 0xA4, g_CaptionLapTime, 0x7812);

    count = 6;
    if (g_CourseIndex != 3) {
        count = 3;
    }

    i = 0;
    if (count != 0) {
        times = g_PlayerLapTimes;
        selectedPtr = g_PlayerLapTimes;
        do {
            x = 0xB0;
            if (i < 3) {
                x = 0x14;
            }
            quotient = i / 3;
            column = i - quotient * 3;
            text[0] = i + 0x31;
            columnBase = 0xB0;
            textPos = column * 0xC + (base + columnBase);
            FormatLapTime(&text[2], *times);
            color = 0x7812;
            if (*(s16 *)((char *)selectedPtr - 0x22) == i) {
                color = 0x784C;
            }
            DrawProportionalText(x, textPos, text, color);
            i++;
            times++;
        } while (i < count);
    }
}
