#include "common.h"
#include "game/memcard.h"
#include "game/audio.h"
#include "game/menu.h"
#include "game/state.h"
#include "psyq/gpu.h"
#include "game/render.h"


void DrawMemoryCardSaveRows(s32 flags, GameSaveHeaderRow *rows) {
    char text[16];
    u8 *rows_base = (u8 *)rows;
    s32 flags_reg = flags;
    s32 color = 0x7F;
    s32 width = 0x244;
    s32 height = 0xA0;
    char *text_ptr = text;
    s32 y = 0xD8;
    s32 row_bit = 1;
    u8 *row = rows_base;

    do {
        if (flags_reg % 2) {
            s32 i;

            sprintf(text, g_FmtSaveRow, row_bit);
            DrawLargeText(0x48, y, text, 0x7F, color, color, width, height);

            for (i = 0; i < row[0]; i++) {
                text_ptr[i] = g_SaveNameCharset[*((row + i) + 1)];
            }
            while (i < 7) {
                text_ptr[i++] = ' ';
            }
            sprintf(text + 6, g_FmtSaveRowTail);
            DrawLargeText(0x68, y, text, 0x7F, color, color, width, height);
            DrawLargeText(0xB0, y, FormatSaveElapsedTime(text, *(s32 *)(row + 8)), 0x7F, color, color, width, height);
        } else if (flags_reg & 0x10000) {
            sprintf(text, g_FmtSaveRow, row_bit);
            DrawLargeText(0x48, y, text, 0x7F, color, color, width, height);
            DrawLargeText(0x88, y, g_McSlotLabelError, 0x7F, color, color, width, height);
        } else if (g_McFreeBlocks == 0) {
            if (g_McMenuPage == 0) {
                sprintf(text, g_FmtSaveRowEmpty, row_bit);
                DrawLargeText(0x48, y, text, 0x7F, color, color, width, height);
            } else if (g_McMenuRowCursor == 0) {
                sprintf(text, g_FmtSaveRowEmpty, row_bit);
                DrawLargeText(0x48, y, text, 0x7F, color, color, width, height);
            } else {
                sprintf(text, g_FmtSaveRow, row_bit);
                DrawLargeText(0x48, y, text, 0x7F, color, color, width, height);
                DrawLargeText(0x90, y, g_McSlotLabelNoFile, 0x7F, color, color, width, height);
            }
        } else if (g_McMenuPage == 0) {
            sprintf(text, g_FmtSaveRowEmpty, row_bit);
            DrawLargeText(0x48, y, text, 0x7F, color, color, width, height);
        } else {
            sprintf(text, g_FmtSaveRow, row_bit);
            DrawLargeText(0x48, y, text, 0x7F, color, color, width, height);
            DrawLargeText(0x90, y, g_McSlotLabels + (g_McMenuRowCursor * 10), 0x7F, color, color, width, height);
        }

        row_bit++;
        row += 0x80;
        y += 0x30;
        flags_reg >>= 1;
    } while ((s32)row < (s32)(rows_base + 0x180));
}

void AdjustMenuSelectionHorizontal(s32 *value, s32 min, s32 max) {
    u16 input = g_PadPressedRepeat;
    s32 next;

    if (input & 0x4000) {
        next = *value + 1;
        *value = next;
        if (max < next) {
            *value = max;
            return;
        }
    } else if (input & 0x1000) {
        next = *value - 1;
        *value = next;
        if (next < min) {
            *value = min;
            return;
        }
    } else {
        return;
    }

    PlaySoundCue(1);
}

void SetMenuBinaryChoiceVertical(s32 *value) {
    u16 input = g_PadPressedRepeat;

    if (input & 0x8000) {
        if (*value == 0) {
            PlaySoundCue(1);
            *value = 1;
        } else {
            *value = 1;
        }
    } else if (input & 0x2000) {
        if (*value == 1) {
            PlaySoundCue(1);
        }
        *value = 0;
    }
}

u16 PollMenuConfirmInput(void) {
    u16 *state = &g_PadPressed;
    u16 value;

    value = *state & 0x860;
    if (value != 0) {
        PlaySoundCue(2);
    }

    return *state & 0x860;
}

u16 PollMenuBackInput(void) {
    u16 *state = &g_PadPressed;
    u16 value;

    value = *state & 0x90;
    if (value != 0) {
        PlaySoundCue(3);
    }

    return *state & 0x90;
}


void DrawMenuFadeOverlay(s32 level) {
    DrawFullscreenFadeTile480(level, 0x40);
}

void StartMenuExitFade(void) {
    StopMemoryCardEvents();
    g_McFadeStep = 8;
}

void EnterMemoryCardMenu(void) {
    SetDispMask(0);
    SetupDisplay480(0, 0, 0);
    g_McMenuRowCount = 2;
    g_McMenuState = -1;
    g_SceneTimer = 0;
    g_McMenuPage = 0;
    g_McMenuRowCursor = 0;
    g_McMenuSubState = 1;
    g_McFromLoadMenu = 0;
    StartMemoryCardEvents();
    g_McFadeStep = -8;
    g_McFadeLevel = 0xFF;
    g_SceneId = 0x1A;
}
