#ifndef GAME_MEMCARD_H
#define GAME_MEMCARD_H

#include "common.h"

typedef struct GameSaveHeaderRow {
    u8 nameLength;
    u8 name[7];
    s32 saveCounter;
    u8 reserved[0x70];
    u32 checksum;
} GameSaveHeaderRow;

void GameAdvanceSaveHeaderCounter(void) asm("func_8005F618");
void GameClearSaveHeaderRows(GameSaveHeaderRow *rows) asm("func_8005F65C");
void GameBuildSaveIconBlock(
    u8 *block,
    char *title,
    s32 iconTile,
    s32 imageX,
    s32 imageY) asm("func_8005F6BC");
void GameWriteSaveHeaderRow(GameSaveHeaderRow *row) asm("func_8005F81C");
s32 GameLoadSaveStateBlock(void *block) asm("func_8005FED4");
s32 GameWriteMemoryCardSaveFile(
    char *path,
    char *title,
    void *iconBlock,
    GameSaveHeaderRow *header,
    void *saveBlock) asm("func_80060524");
s32 GameWriteMemoryCardSaveSlot(
    s32 slot,
    GameSaveHeaderRow *header) asm("func_80060724");
s32 GameReadVerifiedSaveHeader(
    s32 fd,
    GameSaveHeaderRow *header) asm("func_800607B8");
s32 GameScanMemoryCardSaveHeaders(GameSaveHeaderRow *headers) asm("func_8006090C");
s32 GameLoadMemoryCardSaveSlot(
    s32 slot,
    GameSaveHeaderRow *header) asm("func_800609E4");
s32 GameCountMemoryCardFiles(s32 device, s32 port) asm("func_80060B68");
s32 GameCalculateMemoryCardFreeBlocks(s32 fileCount) asm("func_80060BE8");
s32 GameRefreshMemoryCardSaveStatus(
    s32 unused,
    GameSaveHeaderRow *headers) asm("func_80060C3C");
void *GameFormatSaveElapsedTime(void *dst, u32 ticks) asm("func_80060CB0");
void GameDrawMemoryCardMessageLine(s32 unused, s32 messageIndex) asm("func_80060D48");
void GameDrawMemoryCardHelpPrompt(s32 promptIndex) asm("func_80060D88");
void GameDrawMemoryCardSaveRows(
    s32 flags,
    GameSaveHeaderRow *rows) asm("func_80060DF0");

#endif
