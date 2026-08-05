#include "common.h"
#include "game/screens.h"
#include "game/race.h"
#include "game/state.h"
#include "game/render.h"
#include "game/cd.h"
#include "game/audio.h"
#include "game/menu.h"

extern s16 g_ClassRecords[];
extern volatile s32 g_ClassWinCount;
void SetReverbDepth(s32 arg0, s32 arg1);
extern void *g_CourseProgress;
extern volatile u16 g_PadEdge2;
void DrawFullscreenFadeTile(s32 arg0, s32 arg1);
void GameDrawSpriteWide(void *arg0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9, s32 a10, s32 a11, s32 a12, s32 a13) asm("DrawSprite");
void DrawRaceEndBanner(s32 arg0);
void ResetCourseProgress(s32 arg0);

void UpdateBgmTrackCount(void) {
    s32 offset;
    s32 current;
    s32 one;
    s32 value;

    g_ClassWinCount = 0;
    one = 1;
    offset = 0;
    do {
        current = *(s16 *)((u8 *)g_ClassRecords + offset);
        offset += 4;
        if (current == one) {
            g_ClassWinCount = g_ClassWinCount + 1;
        }
    } while (offset < 0x2C);

    value = g_ClassWinCount;
    value = value < 5;
    if (value) {
        value = 9;
    } else {
        value = 10;
    }
    g_BgmTrackCount = value;
}

void DrawLostRaceCaption(s32 arg0) {
    if (arg0 >= 0x100) {
        arg0 = 0xFF;
    }
    arg0 >>= 1;
    GameDrawProportionalTextShaded(0x28, 0x40, &g_CaptionLostRace, 0x7812, arg0);
}

void EnterLostRaceScreen(void) {
    g_FrameSyncThreshold = 0x80;
    SetReverbDepth(0x28, 0x28);
    g_SceneId = 0xE;
    g_LostRaceChoice = 0;
    g_SceneTimer = -1;
    DrawLostRaceCaption(0xFF);
}

void DrawRaceEndPrompt(void) {
    s32 color = 0x7812;
    s32 drawColor;
    s16 index;

    if (g_SceneTimer & 4) {
        color = 0x784C;
    }

    drawColor = 0x7812;
    if (g_LostRaceChoice == 0) {
        drawColor = color;
    }
    DrawProportionalText(0x6A, 0x68, g_TextTryAgain, drawColor);

    drawColor = 0x7812;
    if (g_LostRaceChoice != 0) {
        drawColor = color;
    }
    DrawProportionalText(0x70, 0x78, g_TextEndRace, drawColor);

    DrawProportionalText(0x76, 0xB8, g_TextChance, 0x7812);

    index = *(s16 *)((u8 *)g_CourseProgress + 6);
    DrawProportionalText(0xBE, 0xB8, &g_ChanceDigits[index], 0x7812);

    DrawText8x8(0x58, 0xD0, g_TextPressStart, 0x78CC);
    DrawLostRaceCaption(0xFF);
}

void UpdateLostRaceScreen(void) {
    s32 timer;
    s32 old;
    s32 current;
    u8 *ptr;
    u16 value;

    timer = g_SceneTimer;
    if (timer == -1) {
        old = g_LostRaceChoice;
        if ((g_PadEdge2 & 0x1000) && (old == 1)) {
            g_LostRaceChoice = 0;
        }
        if ((g_PadEdge2 & 0x4000) && (g_LostRaceChoice == 0)) {
            g_LostRaceChoice = 1;
        }
        current = g_LostRaceChoice;
        if (old != current) {
            PlaySoundCue(1);
        }
        if (g_PadEdge2 & 0x800) {
            PlaySoundCue(2);
            if (g_LostRaceChoice != 0) {
                RequestSelectBgmAssets();
            }
            ptr = g_CourseProgress;
            value = *(u16 *)(ptr + 6);
            g_SceneTimer = 0;
            *(u16 *)(ptr + 6) = value - 1;
        }
    } else {
        timer += 2;
        g_SceneTimer = timer;
        DrawFullscreenFadeTile(timer, 0x49);
        if (g_SceneTimer == 0x100) {
            if (g_LostRaceChoice != 0) {
                g_SceneId = 6;
            } else {
                g_SceneId = 0xB;
            }
        }
    }

    DrawRaceEndPrompt();
}

void DrawRaceEndBanner(s32 arg0) {
    if (arg0 >= 256) {
        arg0 = 0xFF;
    }
    arg0 >>= 1;
    GameDrawSpriteWide(g_DrawBuffer + 204, 0x50, 0x6C, 0xA0, 0x18, 0, 0x28, arg0, arg0, arg0, 0xC, 0, 1, 0x29);
}

void EnterRaceEndScreen(void) {
    g_FrameSyncThreshold = 0x80;
    g_SceneId = 0x10;
    g_SceneTimer = 0x22B;
    DrawRaceEndBanner(0x22B);
}

void UpdateRaceEndScreen(void) {
    s32 v = g_SceneTimer - 1;
    g_SceneTimer = v;
    if ((g_PadEdge2 & 0x860) && (u32)v >= 261) {
        StartCdVolumeFade(0xFA);
        g_SceneTimer = 0xFF;
    }
    if (g_SceneTimer == 0) {
        RequestSelectBgmAssets();
        ResetCourseProgress(g_GrandPrixClass);
        g_SceneId = 6;
    }
    DrawRaceEndBanner(g_SceneTimer);
}
