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

/*
 * The 0x1000-byte memory-card payload: a flat dump of live globals, named per
 * field below. checksum = ~sum(u16[0..0x7FE]). Written by func_8005F88C, read
 * by GameLoadSaveStateBlock - both keep raw offsets, see names.md 3b.
 */
typedef struct GameSaveBlock {
    u16 unk00;             /* g_PadMappingIndex */
    u16 unk02;             /* g_NegconMappingIndex */
    u16 unk04;             /* g_NegconSteerNeutral */
    u16 unk06;             /* g_NegconSteerPlay */
    u16 unk08;             /* g_NegconNeutralI */
    u16 unk0A;             /* g_NegconNeutralII */
    u16 unk0C;             /* g_NegconNeutralL */
    u16 unk0E;             /* g_NegconMaxTwist */
    s32 gpFile1[5];        /* +0x10 g_GrandPrixSave..A4, i.e. GameRaceProgress slot 1 */
    s32 gpFile2[5];        /* +0x24 g_ExtraGrandPrixSave..8C, GameRaceProgress slot 2 */
    s32 timeAttack[5];     /* +0x38 g_TimeAttackSave..990, GameRaceProgress slot 3 */
    u16 unk4C;             /* +0x4C g_BgmSelection */
    u16 advancedUnlocked;  /* +0x4E g_AdvancedSeriesUnlocked */
    s32 maxClassReached[2];/* +0x50 g_MaxClassReached */
    u8 carTables[3][0x68]; /* +0x58 the three GameCarEntry tables, 13 rows of 8 */
    u16 unk190[0x16];      /* +0x190 g_ClassRecords */
    u16 unk1BC[0x10];      /* +0x1BC D_801E444C */
    u16 unk1DC[0x400];     /* +0x1DC g_TeamLogoCanvas, the 0x800-byte scroll/VRAM buffer */
    s32 unk9DC[0x10];      /* +0x9DC D_801E4408, [2][4] pairs on an 8-byte stride */
    s32 unkA1C[0x10];      /* +0xA1C D_8019C70C, same shape */
    s32 rankRecords[2][4][5][4]; /* +0xA5C D_801E7744 ranking rows */
    s32 timeRecords[2][4][5][4]; /* +0xCDC D_8019CB78 time rows */
    s32 unkF5C[2][4][3];   /* +0xF5C D_801E41E8 */
    s32 unkFBC;            /* +0xFBC g_BgmVolumeSetting, clamped to 0..0xF on load */
    s32 unkFC0;            /* +0xFC0 g_SfxVolumeSetting, clamped to 0..0xF on load */
    s32 unkFC4;            /* +0xFC4 g_MonoOutput, forced to 0/1 on load */
    u8 courseProgress1[8]; /* +0xFC8 D_801E42EC */
    u8 courseProgress2[8]; /* +0xFD0 D_8009E874 */
    u8 padFD8[0x24];
    u32 checksum;          /* +0xFFC */
} GameSaveBlock;

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

/*
 * Memory card BIOS front end. These were labelled Cd until the event classes
 * were decoded: every one operates on SwCARD/HwCARD, never on the drive.
 */
void GameClearMemoryCardHwEvents(void) asm("func_8005F2AC");
void GameClearMemoryCardSwEvents(void) asm("func_8005F304");
s32 GameWaitMemoryCardHwEvent(void) asm("func_8005F4D8");
s32 GameWaitMemoryCardSwEvent(void) asm("func_8005F55C");
s32 GamePollMemoryCardHwEvent(void) asm("func_8005F35C");
s32 GamePollMemoryCardHwEventLimit(s32 attempts) asm("func_8005F420");
void GameOpenMemoryCardEvents(void) asm("func_8005EFAC");
void GameEnableMemoryCardEvents(void) asm("func_8005F0D4");
void GameDisableMemoryCardEvents(void) asm("func_8005F16C");
void GameCloseMemoryCardEvents(void) asm("func_8005F204");
/* libcard _card_clear (see psyq/): _new_card() + _card_write(chan, 0x3F, 0). */
s32 _card_clear(s32 chan) asm("func_80063DEC");
void GameCardReadAndSetMode(s32 param) asm("func_8005EB20");
void GameCardSeekParam(s32 param) asm("func_8005EB78");
s32 GameCardReadStatusPair(s32 high, s32 low) asm("func_8005EBB0");
s32 GameFormatMemoryCard(s32 arg0, s32 arg1) asm("func_8005EF44");

#endif
