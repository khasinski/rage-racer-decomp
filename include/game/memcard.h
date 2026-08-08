#ifndef GAME_MEMCARD_H
#define GAME_MEMCARD_H

#include "common.h"

#include "psyq/gpu.h"
#include "psyq/kernel.h"

/*
 * g_McMenuPhase picks the prompt drawn under the slot list; 0 draws none, and
 * DrawMemoryCardMessage is called with the value minus one. The names come
 * from the retail strings the index reaches through g_McMessageRows, quoted here.
 */
/* Geometry of the payload. The file around it is icon + header + this block
 * + header again; see the layout note above. */
/* The three CarEntry tables inside the block: grand prix, extra grand
 * prix and time attack, 13 rows of 8 bytes each. */
#define MC_GP_CARS_OFS    0x58
#define MC_EXTRA_CARS_OFS 0xC0
#define MC_TIME_CARS_OFS  0x128

#define MC_BLOCK_SIZE         0x1000
#define MC_BLOCK_CHECKSUM_OFS 0xFFC

#define MC_PROMPT_NONE          0x00
#define MC_PROMPT_SELECT_SAVE   0x01  /* "Select file to save." */
#define MC_PROMPT_SELECT_LOAD   0x02  /* "Select file to load." */
#define MC_PROMPT_NO_CARD       0x03  /* "No Memory card." */
#define MC_PROMPT_CARD_FULL     0x04  /* "Memory card full." */
#define MC_PROMPT_NO_DATA       0x05  /* "No data in Memory card." */
#define MC_PROMPT_NEW_CARD      0x06  /* "New Memory card." */
#define MC_PROMPT_FORMAT_ASK    0x07  /* "Format Memory card?" */
#define MC_PROMPT_OVERWRITE_ASK 0x09  /* "Overwrite old file?" */
#define MC_PROMPT_ACCESSING     0x0F  /* "Now accessing Memory card." */
#define MC_PROMPT_CARD_ERROR    0x10  /* "Memory card error." */
#define MC_PROMPT_LOAD_OK       0x11  /* "LOAD DATA OK!" */
#define MC_PROMPT_SAVE_OK       0x12  /* "SAVE DATA OK!" */
#define MC_PROMPT_FORMAT_OK     0x13  /* "FORMAT DATA OK!" */
#define MC_PROMPT_NO_FILE       0x14  /* "No file found." */

typedef struct GameSaveHeaderRow {
    u8 nameLength;
    u8 name[7];
    s32 saveCounter;
    u8 reserved[0x70];
    u32 checksum;
} GameSaveHeaderRow;

typedef struct MemoryCardMessageRow {
    s32 text;
    u8 column;
    u8 reserved[3];
} MemoryCardMessageRow;

typedef struct SavedCarSetup {
    u8 modelVariant;
    u8 tireCompound;
    u8 transmission;
    u8 paintColor1;
    u8 paintColor2;
    u8 enabled;
    u8 reserved[2];
} SavedCarSetup;

typedef struct SavedClassRecord {
    u16 grade;
    u16 clears;
} SavedClassRecord;

/*
 * The 0x1000-byte memory-card payload: a flat dump of live globals, named per
 * field below. checksum = ~sum(u16[0..0x7FE]). Written by StoreSaveStateBlock, read
 * by LoadSaveStateBlock - both keep raw offsets, see names.md 3b.
 *
 * The file WriteMemoryCardSaveFile lays down around it is four writes:
 *
 *     0x0000  0x200  icon block, "SC" + title + CLUT at 0x60 + 16x16 4bpp icon
 *     0x0200  0x080  GameSaveHeaderRow
 *     0x0280  0x1000 this struct
 *     0x1280  0x080  the same header row again
 *
 * 0x1300 total, one memory card block. The three slots are named
 * "bu00:BESCES-00650 RAGE000".."RAGE002" (g_SaveFilePath).
 *
 * The payload is region independent: US (BASLUS-00403) and JP (BISLPS-00600)
 * saves both satisfy the checksum rules above and decode field for field
 * against this struct, so only the directory filename identifies the region.
 */
typedef struct GameSaveBlock {
    u16 padMappingIndex;
    u16 negconMappingIndex;
    u16 negconSteerNeutral;
    u16 negconSteerPlay;
    u16 negconNeutralI;
    u16 negconNeutralII;
    u16 negconNeutralL;
    u16 negconMaxTwist;
    s32 grandPrixProgress[5]; /* +0x10 g_GrandPrixSave, GameRaceProgress: course,
                              carIndex, classIndex, maxClassReached, money */
    s32 extraGrandPrixProgress[5]; /* +0x24 g_ExtraGrandPrixSave */
    s32 timeAttackProgress[5]; /* +0x38 g_TimeAttackSave; this slot reuses the
                              money word for g_GrandPrixSeries */
    u16 bgmSelection;             /* +0x4C g_BgmSelection */
    u16 advancedUnlocked;  /* +0x4E g_AdvancedSeriesUnlocked */
    s32 maxClassReached[2];/* +0x50 g_MaxClassReached */
    SavedCarSetup carSetup[3][13];
    SavedClassRecord classRecords[11];
    u16 teamLogoClut[16];
    u16 teamLogoCanvas[0x400];
    s32 bestLapTimes[2][4][2];
    s32 bestTotalTimes[2][4][2];
    s32 rankingRecords[2][4][5][4];
    s32 timeRecords[2][4][5][4]; /* +0xCDC g_TimeRecords time rows */
    s32 bestSectorTimes[2][4][3];
    s32 bgmVolume;            /* +0xFBC g_BgmVolumeSetting, clamped to 0..0xF on load */
    s32 sfxVolume;            /* +0xFC0 g_SfxVolumeSetting, clamped to 0..0xF on load */
    s32 monoOutput;            /* +0xFC4 g_MonoOutput, forced to 0/1 on load */
    u8 grandPrixCourseProgress[8];
    u8 extraGrandPrixCourseProgress[8];
    u8 reserved[0x24];
    u32 checksum;          /* +0xFFC */
} GameSaveBlock;

void AdvanceSaveHeaderCounter(void);
void ClearSaveHeaderRows(GameSaveHeaderRow *rows);
void BuildSaveIconBlock(
    u8 *block,
    char *title,
    s32 iconTile,
    s32 imageX,
    s32 imageY);
void WriteSaveHeaderRow(GameSaveHeaderRow *row);
s32 LoadSaveStateBlock(u8 *block);
s32 WriteMemoryCardSaveFile(
    char *path,
    char *title,
    void *iconBlock,
    GameSaveHeaderRow *header,
    void *saveBlock);
s32 WriteMemoryCardSaveSlot(
    s32 slot,
    GameSaveHeaderRow *header);
s32 ReadVerifiedSaveHeader(
    s32 fd,
    GameSaveHeaderRow *header);
s32 ScanMemoryCardSaveHeaders(GameSaveHeaderRow *headers);
s32 LoadMemoryCardSaveSlot(
    s32 slot,
    GameSaveHeaderRow *header);
s32 CountMemoryCardFiles(s32 device, s32 port);
s32 CalculateMemoryCardFreeBlocks(s32 fileCount);
s32 RefreshMemoryCardSaveStatus(
    s32 unused,
    GameSaveHeaderRow *headers);
char *FormatSaveElapsedTime(char *dst, u32 ticks);
void DrawMemoryCardMessageLine(s32 unused, s32 messageIndex);
void DrawMemoryCardHelpPrompt(s32 promptIndex);
void DrawMemoryCardSaveRows(
    s32 flags,
    GameSaveHeaderRow *rows);

/*
 * Memory card BIOS front end. These were labelled Cd until the event classes
 * were decoded: every one operates on SwCARD/HwCARD, never on the drive.
 */
/* Both bodies take no argument, but two callers hand them the card slot;
 * an empty parameter list lets that stand. */
void ClearMemoryCardHwEvents();
void ClearMemoryCardSwEvents();
s32 WaitMemoryCardHwEvent(void);
s32 WaitMemoryCardSwEvent(void);
s32 PollMemoryCardHwEvent(void);
s32 PollMemoryCardHwEventLimit(s32 attempts);
void OpenMemoryCardEvents(void);
void EnableMemoryCardEvents(void);
void DisableMemoryCardEvents(void);
void CloseMemoryCardEvents(void);
/* libcard _card_clear (see psyq/): _new_card() + _card_write(chan, 0x3F, 0). */
s32 _card_clear(s32 chan);
void CardReadAndSetMode(s32 param);
void CardSeekParam(s32 param);
s32 CardReadStatusPair(s32 high, s32 low);
s32 FormatMemoryCard(s32 port, s32 slot);

/* Moved here from menu.h and audio.h: these belong to the card, not to
 * the menu or the mixer. */
void StartMemoryCardEvents(void);
void StopMemoryCardEvents(void);
void EnterMemoryCardMenu(void);
void EnterMemoryCardMenuFromLoad(void);
void UpdateMemoryCardMenu(void);

/* Declared identically by 20 translation units before this
 * header carried them. */

extern s32 g_McFadeLevel;
extern s32 g_McFadeStep;
extern s32 g_McFreeBlocks;
extern s32 g_McFromLoadMenu;
extern s32 g_McMenuPage;
extern s32 g_McMenuRowCount;
extern s32 g_McMenuRowCursor;

/* Declared identically by 64 translation units before this
 * header carried them. */

extern char g_FmtCardDevice[];
extern char g_FmtCardWildcard[];
extern char g_FmtPlayTime[];
extern char g_FmtSaveChecksum[];
extern char g_FmtSaveRow[];
extern char g_FmtSaveRowEmpty[];
extern char g_FmtSaveRowTail[];
extern char g_FmtString[];
extern s32 g_McActionBusy;
extern s32 g_McActionElapsed;
extern s32 g_McActionOk;
extern s32 g_McActionResult;
extern s32 g_McActionState;
extern s32 g_McActionTimer;
extern s32 g_McCardFileCount;
extern s32 g_McCardOkFrames;
extern s32 g_McConfirmChoice;
extern DirEntry g_McDirEntries[];
extern s32 g_McDrawEnabled;
extern s32 g_McErrorCountdown;
extern s32 g_McErrorPending;
extern s32 g_McErrorTicks;
extern u8 g_McHelpText[];
extern s32 g_McHwEventError;
extern s32 g_McHwEventIoe;
extern s32 g_McHwEventNew;
extern s32 g_McHwEventTimeout;
extern s32 g_McLastCardStatus;
extern s32 g_McLastMenuState;
extern s32 g_McLastSlot;
extern s16 g_McMessageColumnX[];
extern MemoryCardMessageRow *g_McMessageRows[];
extern u8 g_McMessageText[];
extern s32 g_McNoCardTicks;
extern s32 g_McPollStatus;
extern s32 g_McPollTicks;
extern GameSaveHeaderRow g_McSaveHeaders[];
extern s32 g_McSaveMode;
extern s32 g_McSavedLoadPhase;
extern s32 g_McSettleTicks;
extern s32 g_McSlotCursor;
extern char g_McSlotLabelError[];
extern char g_McSlotLabelNoFile[];
extern char g_McSlotLabels[];
extern s32 g_McSlotUsedMask;
extern s32 g_McStateChangeCount;
extern s32 g_McStatusResult;
extern s32 g_McStatusState;
extern s32 g_McSwEventError;
extern s32 g_McSwEventIoe;
extern s32 g_McSwEventNew;
extern s32 g_McSwEventTimeout;
extern char g_MsgSaveChecksumOk[];
extern s32 g_SaveElapsedTicks;
extern char g_SaveFilePath[];
extern u8 g_SaveNameCharset[];
extern char g_SaveTitleSjis[];

void BiosBuInit(void);
void DrawMemoryCardMessage(s32 message);
s32 DrawShadowedTile(s32 base, s32 prim, s32 a, s32 b);
void InitCARD(s32 padEnable);
s32 PollMemoryCardStatus(s32 a, s32 b);
void StartCARD(void);
void StoreSaveStateBlock(u8 *block);
void DrawMemoryCardScreen(s32 showBar, s32 variant, s32 cursor, s32 barRow);

/* Declared identically by 1 translation units before this
 * header carried them. */

extern Rect g_SaveIconRect;

#endif
