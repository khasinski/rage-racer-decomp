#include "common.h"
#include "game/memcard.h"

extern char D_80012FC8[];
extern u8 D_80012FD0[];
extern char D_80012FFC[];
extern char D_80013000[];
extern char D_80082F7C[];
extern char D_80082F86[];
extern char D_80082F9A[];
extern s32 D_80082F50;
extern s32 D_80082F54;
extern s32 D_8009B73C;

void func_80047958(s32, s32, void *, s32, s32, s32, s32, s32);
void func_800632F0();

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

            func_800632F0(text, D_80012FC8, row_bit);
            func_80047958(0x48, y, text, 0x7F, color, color, width, height);

            for (i = 0; i < row[0]; i++) {
                text_ptr[i] = D_80012FD0[*((row + i) + 1)];
            }
            while (i < 7) {
                text_ptr[i++] = ' ';
            }
            func_800632F0(text + 6, D_80012FFC);
            func_80047958(0x68, y, text, 0x7F, color, color, width, height);
            func_80047958(0xB0, y, GameFormatSaveElapsedTime(text, *(s32 *)(row + 8)), 0x7F, color, color, width, height);
        } else if (flags_reg & 0x10000) {
            func_800632F0(text, D_80012FC8, row_bit);
            func_80047958(0x48, y, text, 0x7F, color, color, width, height);
            func_80047958(0x88, y, D_80082F9A, 0x7F, color, color, width, height);
        } else if (D_8009B73C == 0) {
            if (D_80082F50 == 0) {
                func_800632F0(text, D_80013000, row_bit);
                func_80047958(0x48, y, text, 0x7F, color, color, width, height);
            } else if (D_80082F54 == 0) {
                func_800632F0(text, D_80013000, row_bit);
                func_80047958(0x48, y, text, 0x7F, color, color, width, height);
            } else {
                func_800632F0(text, D_80012FC8, row_bit);
                func_80047958(0x48, y, text, 0x7F, color, color, width, height);
                func_80047958(0x90, y, D_80082F86, 0x7F, color, color, width, height);
            }
        } else if (D_80082F50 == 0) {
            func_800632F0(text, D_80013000, row_bit);
            func_80047958(0x48, y, text, 0x7F, color, color, width, height);
        } else {
            func_800632F0(text, D_80012FC8, row_bit);
            func_80047958(0x48, y, text, 0x7F, color, color, width, height);
            func_80047958(0x90, y, D_80082F7C + (D_80082F54 * 10), 0x7F, color, color, width, height);
        }

        row_bit++;
        row += 0x80;
        y += 0x30;
        flags_reg >>= 1;
    } while ((s32)row < (s32)(rows_base + 0x180));
}
