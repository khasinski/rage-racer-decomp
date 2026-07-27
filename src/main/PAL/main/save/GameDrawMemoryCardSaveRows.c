#include "common.h"
#include "game/memcard.h"
#include "game/audio.h"
#include "game/menu.h"
#include "game/state.h"
#include "psyq/gpu.h"
#include "game/render.h"

extern char g_FmtSaveRow[] asm("D_80012FC8");
extern u8 g_SaveNameCharset[] asm("D_80012FD0");
extern char g_FmtSaveRowTail[] asm("D_80012FFC");
extern char g_FmtSaveRowEmpty[] asm("D_80013000");
extern char g_McSlotLabels[] asm("D_80082F7C");
extern char g_McSlotLabelNoFile[] asm("D_80082F86");
extern char g_McSlotLabelError[] asm("D_80082F9A");
extern s32 g_McMenuPage asm("D_80082F50");
extern s32 g_McMenuRowCursor asm("D_80082F54");
extern s32 g_McFreeBlocks asm("D_8009B73C");

void func_80047958(s32, s32, void *, s32, s32, s32, s32, s32);
void LibcSprintf() asm("func_800632F0");

void GameDrawMemoryCardSaveRows(s32 flags, GameSaveHeaderRow *rows) {
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
        if (flags_reg & 1) {
            s32 i;

            LibcSprintf(text, g_FmtSaveRow, row_bit);
            func_80047958(0x48, y, text, 0x7F, color, color, width, height);

            for (i = 0; i < row[0]; i++) {
                text_ptr[i] = g_SaveNameCharset[*((row + i) + 1)];
            }
            while (i < 7) {
                text_ptr[i++] = ' ';
            }
            LibcSprintf(text + 6, g_FmtSaveRowTail);
            func_80047958(0x68, y, text, 0x7F, color, color, width, height);
            func_80047958(0xB0, y, GameFormatSaveElapsedTime(text, *(s32 *)(row + 8)), 0x7F, color, color, width, height);
        } else if (flags_reg & 0x10000) {
            LibcSprintf(text, g_FmtSaveRow, row_bit);
            func_80047958(0x48, y, text, 0x7F, color, color, width, height);
            func_80047958(0x88, y, g_McSlotLabelError, 0x7F, color, color, width, height);
        } else if (g_McFreeBlocks == 0) {
            if (g_McMenuPage == 0) {
                LibcSprintf(text, g_FmtSaveRowEmpty, row_bit);
                func_80047958(0x48, y, text, 0x7F, color, color, width, height);
            } else if (g_McMenuRowCursor == 0) {
                LibcSprintf(text, g_FmtSaveRowEmpty, row_bit);
                func_80047958(0x48, y, text, 0x7F, color, color, width, height);
            } else {
                LibcSprintf(text, g_FmtSaveRow, row_bit);
                func_80047958(0x48, y, text, 0x7F, color, color, width, height);
                func_80047958(0x90, y, g_McSlotLabelNoFile, 0x7F, color, color, width, height);
            }
        } else if (g_McMenuPage == 0) {
            LibcSprintf(text, g_FmtSaveRowEmpty, row_bit);
            func_80047958(0x48, y, text, 0x7F, color, color, width, height);
        } else {
            LibcSprintf(text, g_FmtSaveRow, row_bit);
            func_80047958(0x48, y, text, 0x7F, color, color, width, height);
            func_80047958(0x90, y, g_McSlotLabels + (g_McMenuRowCursor * 10), 0x7F, color, color, width, height);
        }

        row_bit++;
        row += 0x80;
        y += 0x30;
        flags_reg >>= 1;
    } while ((s32)row < (s32)(rows_base + 0x180));
}

void GameAdjustMenuSelectionHorizontal(s32 *value, s32 min, s32 max) {
    u16 input = g_PadEdge;
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

    GamePlaySoundCue(1);
}

void GameSetMenuBinaryChoiceVertical(s32 *value) {
    u16 input = g_PadEdge;

    if (input & 0x8000) {
        if (*value == 0) {
            GamePlaySoundCue(1);
            *value = 1;
        } else {
            *value = 1;
        }
    } else if (input & 0x2000) {
        if (*value == 1) {
            GamePlaySoundCue(1);
        }
        *value = 0;
    }
}

u16 GamePollMenuConfirmInput(void) {
    u16 *state = &g_PadEdge2;
    u16 value;

    value = *state & 0x860;
    if (value != 0) {
        GamePlaySoundCue(2);
    }

    return *state & 0x860;
}

u16 GamePollMenuBackInput(void) {
    u16 *state = &g_PadEdge2;
    u16 value;

    value = *state & 0x90;
    if (value != 0) {
        GamePlaySoundCue(3);
    }

    return *state & 0x90;
}

void func_80023A60(s32 arg0, s32 arg1);

void GameDrawMenuFadeOverlay(s32 arg0) {
    func_80023A60(arg0, 0x40);
}

extern s32 g_McFadeStep asm("D_8009B9A0");

void GameStartMenuExitFade(void) {
    GameStopMemoryCardEvents();
    g_McFadeStep = 8;
}

extern s32 g_McMenuRowCount asm("D_8009B744");
extern s32 g_McMenuPage;
extern s32 g_McMenuRowCursor;
extern s32 g_McFromLoadMenu asm("D_8009B730");
extern s32 g_McFadeStep;
extern s32 g_McFadeLevel asm("D_8009B9A4");
void func_8005EAD0(void);
void GameEnterMemoryCardMenu(void) asm("func_800613B8");
void GameEnterMemoryCardMenu(void) {
    SetDispMask(0);
    GameSetupDisplay480(0, 0, 0);
    g_McMenuRowCount = 2;
    g_McMenuState = -1;
    g_SceneTimer = 0;
    g_McMenuPage = 0;
    g_McMenuRowCursor = 0;
    g_McMenuSubState = 1;
    g_McFromLoadMenu = 0;
    func_8005EAD0();
    g_McFadeStep = -8;
    g_McFadeLevel = 0xFF;
    g_SceneId = 0x1A;
}
