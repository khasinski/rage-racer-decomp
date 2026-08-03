#include "common.h"
#include "game/menu.h"
#include "game/race.h"
#include "game/state.h"
#include "game/render.h"
#include "psyq/gpu.h"
#include "game/cd.h"

/* Second copies of the same three captions -- the ROM holds duplicate literals
 * at 0x80010E1C..0x80010E28 and here; see docs/names.md 18d for why the bytes
 * are lowercase and how each caption is identified. */
extern char g_CaptionLapTime2[] asm("D_80010F90");
extern char g_CaptionTotalTime2[] asm("D_80010FA8");
extern char g_CaptionRanking2[] asm("D_80010F98");
extern char g_FmtRecordName[] asm("D_80010F9C");
extern char g_FmtCarName[] asm("D_80010FA4");
extern s32 g_PlayerLapTimes[] asm("D_8009E858");
extern s32 g_BestLapIndex asm("D_8019CAC8");
extern s32 g_TimeRecordInsertRow asm("D_8019CE10");
extern s32 g_RankingInsertRow asm("D_801E8A48");
extern u8 *g_PlaceSuffixNames[] asm("D_8007D4C0");
extern s32 g_CarClassNames[] asm("D_8007D508");
extern s32 g_CarNames[] asm("D_8007D4D4");
void func_80016EA0(void *dst, s32 len, void *src, s32 arg3);
void func_80016754(void *dst, s32 x, void *src, s32 color);
void *func_80021CD4(void *dst, s32 value);
void LibcSprintf() asm("func_800632F0");
s32 AddTilePrim(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) asm("func_80032F34");
extern volatile s32 g_RaceTotalTime asm("D_801E4BA8");
extern volatile u16 g_PlayerCarIndex asm("D_801E40D4");
extern s32 g_RankingTimes asm("D_801E774C");
/* Split symbols of the two S22 record tables: +0x08 is the time and +0x0C the
 * car index, so g_RankingCars is g_RankingRecords[0][0][0].vC and
 * g_TimeRecordTimes / g_TimeRecordCars the same pair of g_TimeRecords. */
extern u16 g_RankingCars[] asm("D_801E7750");
extern s32 g_TimeRecordTimes asm("D_8019CB80");
extern u16 g_TimeRecordCars[] asm("D_8019CB84");
/* Deliberately raw: see docs/names.md 12d. */
extern s32 g_FrameSyncThreshold asm("D_8019C768");
extern s32 g_RecordEntryState asm("D_801E6C8C");
void InsertRaceRecords(void) asm("func_80022324");
extern u8 g_NameEntryCharset[] asm("D_80010FB0");
extern s32 g_NameEntryCursor asm("D_8019C8F8");
extern s32 g_RecordPanelSlide asm("D_8019CA14");
extern u8 g_RankingNameCodes[] asm("D_801E417C");
extern s32 g_NameEntryChar asm("D_801E6830");
extern u8 g_TimeRecordNameCodes[] asm("D_801F17FC");
void RequestSelectBgmAssets(void) asm("func_80018410");
void func_80021D68(void);
void DrawRankingPanel(u8 *arg0) asm("func_80021DB8");
void DrawTimeRecordPanel(u8 *arg0) asm("func_80022068");
void DrawNameEntryCursor(s32 arg0, s32 arg1) asm("func_8002229C");
void DrawFullscreenFadeTile(s32 arg0, s32 arg1) asm("func_80033AA0");
void PlaySoundCue(s32 cue) asm("func_8005D6EC");
void CdSync(s32 arg0, s32 arg1) asm("func_8006A534");
s32 CdControl(s32 com, void *param, s32 result) asm("func_8006A5A4");

void DrawRankingPanel(u8 *arg0) asm("func_80021DB8");
void DrawRankingPanel(u8 *arg0) {
    u8 *panel;
    s32 iter;
    s32 countOrIndex;
    s32 xOrField;
    s32 destination;
    s32 color;
    s32 scoreOrX;
    char text[56];
    s32 mode;
    s32 row;
    s32 doubledRow;
    s32 value;
    s32 scoreValue;
    s32 limit;

    panel = arg0;
    func_80016EA0(panel + 0x10, 0x4C, g_CaptionLapTime2, 0x7852);
    mode = g_CourseIndex;
    text[1] = 0x2F;
    limit = 6;
    if (mode != 3) {
        limit = 3;
    }
    iter = 0;
    if (limit > 0) {
        scoreOrX = (s32)g_PlayerLapTimes;
        do {
            scoreValue = iter + (((u32)iter) >> 31);
            row = scoreValue >> 1;
            doubledRow = row * 2;
            value = iter - doubledRow;
            value <<= 3;
            xOrField = value + 0x58;
            func_80021CD4(
                &text[2],
                ({
                    (void)(*(volatile char *)&text[0] = iter + 0x31);
                    doubledRow = (doubledRow + row) << 5;
                    scoreValue = *((s32 *)scoreOrX);
                    value = (destination = (s32)(panel + 0x14));
                    scoreValue;
                }));
            destination = doubledRow;
            destination = destination + value;
            color = 0x78CC;
            if (g_BestLapIndex == iter) {
                color = 0x780F;
            }
            func_80016754((void *)destination, xOrField, text, color);
            iter++;
            scoreOrX += 4;
        } while (iter < limit);
    }
    func_80016EA0(panel + 0x10, 0x6C, g_CaptionRanking2, 0x7812);
    countOrIndex = 0;
    scoreOrX = 0x82;
    destination = 0x78;
    do {
        text[0] = g_PlaceSuffixNames[countOrIndex][0];
        text[1] = g_PlaceSuffixNames[countOrIndex][1];
        text[2] = g_PlaceSuffixNames[countOrIndex][2];
        text[3] = 0x2F;
        func_80021CD4(&text[4], g_RankingRecords[g_GrandPrixSeries][g_CourseIndex][countOrIndex].v8);
        xOrField = g_RankingRecords[g_GrandPrixSeries][g_CourseIndex][countOrIndex].vC;
        LibcSprintf(&text[0xC], g_FmtRecordName,
                      &g_RankingRecords[g_GrandPrixSeries][g_CourseIndex][countOrIndex],
                      g_CarClassNames[xOrField]);
        color = 0x78CC;
        if (g_RankingInsertRow == countOrIndex) {
            color = 0x780F;
        }
        func_80016754(panel + 0x14, destination, text, color);
        LibcSprintf(text, g_FmtCarName, g_CarNames[xOrField]);
        func_80016754(panel + 0x2C, scoreOrX, text, color);
        destination += 0x14;
        scoreOrX += 0x14;
        countOrIndex++;
    } while (countOrIndex < 5);
}

void DrawTimeRecordPanel(u8 *s5) asm("func_80022068");
void DrawTimeRecordPanel(u8 *s5) {
    char text[48];
    s32 s4, s3;
    s32 s2, color, idx;

    func_80016EA0(s5 + 0x10, 0x4C, g_CaptionTotalTime2, 0x7852);

    text[0] = 0x54;
    text[1] = 0x2F;
    func_80021CD4(&text[2], g_RaceTotalTime);
    func_80016754(s5 + 0x14, 0x58, text, 0x78CC);

    func_80016EA0(s5 + 0x10, 0x6C, g_CaptionRanking2, 0x7812);

    s2 = 0;
    s4 = 0x82;
    s3 = 0x78;
    for (; s2 < 5; s2++) {
        text[0] = g_PlaceSuffixNames[s2][0];
        text[1] = g_PlaceSuffixNames[s2][1];
        text[2] = g_PlaceSuffixNames[s2][2];
        text[3] = 0x2F;
        func_80021CD4(&text[4], g_TimeRecords[g_GrandPrixSeries][g_CourseIndex][s2].v8);

        idx = g_TimeRecords[g_GrandPrixSeries][g_CourseIndex][s2].vC;
        LibcSprintf(&text[0xC], g_FmtRecordName,
                      &g_TimeRecords[g_GrandPrixSeries][g_CourseIndex][s2], g_CarClassNames[idx]);

        color = 0x78CC;
        if (g_TimeRecordInsertRow == s2) {
            color = 0x780F;
        }
        func_80016754(s5 + 0x14, s3, text, color);

        LibcSprintf(text, g_FmtCarName, g_CarNames[idx]);

        func_80016754(s5 + 0x2C, s4, text, color);
        s3 += 0x14;
        s4 += 0x14;
    }
}

void DrawNameEntryCursor(s32 arg0, s32 arg1) asm("func_8002229C");
void DrawNameEntryCursor(s32 arg0, s32 arg1) {
    s32 *scratch;

    if (g_AnimTimer & 8) {
        scratch = (s32 *)0x1F800000;
        *scratch = AddTilePrim(
            g_DrawBuffer + 0xCC,
            *scratch,
            (arg0 * 8) + 0x7C,
            (((arg1 * 4) + arg1) << 2) + 0x7E,
            9,
            2,
            0xC0,
            0x48,
            0x48);
    }

}

void InsertRaceRecords(void) asm("func_80022324");
void InsertRaceRecords(void) {
    s32 count;
    s32 i;
    s32 row_offset;
    s32 best;
    register s32 j asm("$7");
    s32 *score_ptr;
    register s32 entry_addr asm("$5");
    s32 fill_offset;
    register s32 fill_addr asm("$3");
    s32 copy0;
    s32 copy1;
    s32 copy2;
    s32 copy3;
    register s32 score_offset asm("$3");
    s32 score_value;
    s32 *entry;
    s32 mode;
    register s32 base_addr asm("$2");
    register s32 course_addr asm("$3");
    u8 *name_base;
    u8 *name_base2;
    s32 letter;
    s32 code;
    s32 letter2;
    s32 code2;

    count = 3;
    if (g_CourseIndex == 3) {
        count = 6;
    }

    best = 0x927C0;
    i = 0;
    if (i < count) {
        score_ptr = g_PlayerLapTimes;
        while (i < count) {
            if (*score_ptr < best) {
                best = *score_ptr;
                g_BestLapIndex = i;
            }
            i++;
            score_ptr++;
        }
    }

    i = 0;
    name_base = (u8 *)g_RankingRecords;
    letter = 0x41;
    code = 0xB;
    row_offset = 0;
    while (i < 5) {
        score_offset = row_offset + (g_CourseIndex * 0x50);
        score_offset += g_GrandPrixSeries * 0x140;
        if (best < *((s32 *)((u8 *)&g_RankingTimes + score_offset))) {
            if (i < 4) {
                j = 4;
                do {
                    entry_addr = j * 0x10;
                    j--;
                    mode = g_CourseIndex;
                    course_addr = (g_GrandPrixSeries * 0x140) + (s32)name_base;
                    base_addr = (mode * 0x50) + course_addr;
                    entry_addr += base_addr;
                    entry = (s32 *)entry_addr;
                    asm volatile("" : : "r"(j));
                    copy0 = entry[-4];
                    copy1 = entry[-3];
                    copy2 = entry[-2];
                    copy3 = entry[-1];
                    entry[0] = copy0;
                    entry[1] = copy1;
                    entry[2] = copy2;
                    entry[3] = copy3;
                    asm volatile("" : : : "memory");
                } while (i < j);
            }
            score_offset = row_offset + (g_CourseIndex * 0x50);
            score_offset += g_GrandPrixSeries * 0x140;
            *((s32 *)((u8 *)&g_RankingTimes + score_offset)) = best;
            j = 0;
            fill_offset = row_offset;
            for (; j < 6; j++) {
                fill_addr = fill_offset + (g_CourseIndex * 0x50);
                fill_addr += g_GrandPrixSeries * 0x140;
                *((volatile u8 *)((fill_addr + (s32)name_base) + j)) = letter;
                g_RankingNameCodes[j] = code;
            }

            score_offset = row_offset + (g_CourseIndex * 0x50);
            score_offset += g_GrandPrixSeries * 0x140;
            *((u16 *)((u8 *)g_RankingCars + score_offset)) = g_PlayerCarIndex;
            break;
        }
        i++;
        row_offset += 0x10;
    }

    g_RankingInsertRow = i;
    i = 0;
    name_base2 = (u8 *)g_TimeRecords;
    letter2 = 0x41;
    code2 = 0xB;
    row_offset = 0;
    while (i < 5) {
        score_offset = row_offset + (g_CourseIndex * 0x50);
        score_offset += g_GrandPrixSeries * 0x140;
        score_value = *((s32 *)((u8 *)&g_TimeRecordTimes + score_offset));
        if (g_RaceTotalTime < score_value) {
            if (i < 4) {
                j = 4;
                do {
                    entry_addr = j * 0x10;
                    j--;
                    mode = g_CourseIndex;
                    course_addr = (g_GrandPrixSeries * 0x140) + (s32)name_base2;
                    base_addr = (mode * 0x50) + course_addr;
                    entry_addr += base_addr;
                    entry = (s32 *)entry_addr;
                    asm volatile("" : : "r"(j));
                    copy0 = entry[-4];
                    copy1 = entry[-3];
                    copy2 = entry[-2];
                    copy3 = entry[-1];
                    entry[0] = copy0;
                    entry[1] = copy1;
                    entry[2] = copy2;
                    entry[3] = copy3;
                    asm volatile("" : : : "memory");
                } while (i < j);
            }
            score_offset = row_offset + (g_CourseIndex * 0x50);
            score_offset += g_GrandPrixSeries * 0x140;
            *((s32 *)((u8 *)&g_TimeRecordTimes + score_offset)) = g_RaceTotalTime;
            j = 0;
            fill_offset = row_offset;
            for (; j < 6; j++) {
                fill_addr = fill_offset + (g_CourseIndex * 0x50);
                fill_addr += g_GrandPrixSeries * 0x140;
                *((volatile u8 *)((fill_addr + (s32)name_base2) + j)) = letter2;
                g_TimeRecordNameCodes[j] = code2;
            }

            score_offset = row_offset + (g_CourseIndex * 0x50);
            score_offset += g_GrandPrixSeries * 0x140;
            *((u16 *)((u8 *)g_TimeRecordCars + score_offset)) = g_PlayerCarIndex;
            break;
        }
        i++;
        row_offset += 0x10;
    }

    g_TimeRecordInsertRow = i;
}

void EnterRecordEntry(void) asm("func_80022748");
void EnterRecordEntry(void) {
    g_SceneTimer = 0x100;
    g_FrameSyncThreshold = 0x80;
    g_RecordEntryState = 0;
    g_SceneId = 0x15;
    InsertRaceRecords();
}

void UpdateRecordEntry(void) asm("func_80022794");
void UpdateRecordEntry(void) {
    u8 *name;
    s32 i;

    g_AnimTimer++;

    switch (g_RecordEntryState) {
    case 0:
        g_SceneTimer -= 8;
        DrawFullscreenFadeTile(g_SceneTimer, 0x49);
        if (g_SceneTimer == 0) {
            if (g_RankingInsertRow < 5 || g_TimeRecordInsertRow < 5) {
                RequestCdTrack(0xE);
                StartCdAudio();
            }
            if (g_RankingInsertRow < 5) {
                g_NameEntryChar = 0xB;
                g_NameEntryCursor = 0;
                g_RecordEntryState = 1;
            } else {
                g_RecordEntryState = 2;
            }
        }
        DrawRankingPanel((u8 *)0);
        break;

    case 1: {
        u8 *timeName;
        u8 *timeRecordBase;
        u8 *rankingRecordBase;
        u8 *record;
        s32 previous;
        u16 buttons;

        previous = g_NameEntryChar;
        if (g_PadEdge & 0x8000) {
            g_NameEntryChar = previous - 1;
        } else if (g_PadEdge & 0x2000) {
            g_NameEntryChar = previous + 1;
        }
        g_NameEntryChar = (g_NameEntryChar + 42) % 42;
        if (previous != g_NameEntryChar) {
            PlaySoundCue(1);
        }

        g_RankingNameCodes[g_NameEntryCursor] = g_NameEntryChar;
        buttons = g_PadEdge2;
        name = (u8 *)g_RankingNameCodes;
        if (buttons & 0x860) {
            PlaySoundCue(2);
            g_NameEntryCursor++;
            if (g_NameEntryCursor == 6) {
                g_RecordEntryState = 2;
                i = 0;
                if (g_TimeRecordInsertRow < 5) {
                    timeRecordBase = g_TimeRecords;
                    timeName = g_TimeRecordNameCodes;
                    do {
                        *timeName = g_RankingNameCodes[i];
                        record = (u8 *)((((g_CourseIndex * 5) + g_TimeRecordInsertRow) *
                                         0x10) +
                                        (g_GrandPrixSeries * 0x140) +
                                        (s32)timeRecordBase + i);
                        i++;
                        *record = g_NameEntryCharset[*timeName];
                        timeName++;
                    } while (i < 6);
                }
            }
            g_NameEntryChar = g_RankingNameCodes[g_NameEntryCursor];
        } else if ((buttons & 0x90) && g_NameEntryCursor > 0) {
            PlaySoundCue(3);
            g_NameEntryCursor--;
            g_NameEntryChar = name[g_NameEntryCursor];
        }

        if (g_RecordEntryState == 1) {
            DrawNameEntryCursor(g_NameEntryCursor, g_RankingInsertRow);
        }
        i = 0;
        rankingRecordBase = g_RankingRecords;
        do {
            record = (u8 *)((((g_CourseIndex * 5) + g_RankingInsertRow) * 0x10) +
                            (g_GrandPrixSeries * 0x140) +
                            (s32)rankingRecordBase + i);
            *record = g_NameEntryCharset[g_RankingNameCodes[i]];
            i++;
        } while (i < 6);
        DrawRankingPanel((u8 *)0);
        break;
    }

    case 2:
        if (g_PadEdge2 & 0x860) {
            g_RecordEntryState = 3;
            g_RecordPanelSlide = 0;
        }
        DrawRankingPanel((u8 *)0);
        break;

    case 3:
        g_RecordPanelSlide -= 8;
        DrawRankingPanel((u8 *)g_RecordPanelSlide);
        DrawTimeRecordPanel((u8 *)(g_RecordPanelSlide + 0x140));
        if (g_RecordPanelSlide < -0x13F) {
            if (g_TimeRecordInsertRow < 5) {
                g_NameEntryCursor = 0;
                g_RecordEntryState = 4;
                g_NameEntryChar = g_TimeRecordNameCodes[0];
            } else {
                g_RecordEntryState = 5;
            }
        }
        break;

    case 4: {
        u8 *recordBase;
        u8 *record;
        s32 previous;
        u16 buttons;

        previous = g_NameEntryChar;
        if (g_PadEdge & 0x8000) {
            g_NameEntryChar = previous - 1;
        } else if (g_PadEdge & 0x2000) {
            g_NameEntryChar = previous + 1;
        }
        g_NameEntryChar = (g_NameEntryChar + 42) % 42;
        if (previous != g_NameEntryChar) {
            PlaySoundCue(1);
        }

        g_TimeRecordNameCodes[g_NameEntryCursor] = g_NameEntryChar;
        buttons = g_PadEdge2;
        name = (u8 *)g_TimeRecordNameCodes;
        if (buttons & 0x860) {
            PlaySoundCue(2);
            g_NameEntryCursor++;
            if (g_NameEntryCursor == 6) {
                g_RecordEntryState = 5;
            }
            g_NameEntryChar = name[g_NameEntryCursor];
        } else if ((buttons & 0x90) && g_NameEntryCursor > 0) {
            PlaySoundCue(3);
            g_NameEntryCursor--;
            g_NameEntryChar = name[g_NameEntryCursor];
        }

        if (g_RecordEntryState == 4) {
            DrawNameEntryCursor(g_NameEntryCursor, g_TimeRecordInsertRow);
        }
        i = 0;
        recordBase = g_TimeRecords;
        do {
            record = (u8 *)((((g_CourseIndex * 5) + g_TimeRecordInsertRow) * 0x10) +
                            (g_GrandPrixSeries * 0x140) + (s32)recordBase + i);
            *record = g_NameEntryCharset[g_TimeRecordNameCodes[i]];
            i++;
        } while (i < 6);
        DrawTimeRecordPanel((u8 *)0);
        break;
    }

    case 5:
        if (g_PadEdge2 & 0x860) {
            if (g_RankingInsertRow < 5 || g_TimeRecordInsertRow < 5) {
                StartCdVolumeFade(0x78);
                StartCdAudio();
            }
            g_RecordEntryState = 6;
            g_RecordPanelSlide = 0;
        }
        DrawTimeRecordPanel((u8 *)0);
        break;

    case 6:
        g_SceneTimer += 2;
        DrawFullscreenFadeTile(g_SceneTimer, 0x49);
        if ((u32)g_SceneTimer >= 0x100) {
            RequestSelectBgmAssets();
            g_SceneId = 6;
        }
        DrawTimeRecordPanel((u8 *)0);
        break;
    }

    func_80021D68();
}

void ReturnFromClassFmv(void) asm("func_80022EE4");
void ReturnFromClassFmv(void) {
    CdSync(0, 0);
    CdControl(9, 0, 0);
    g_SceneId = 6;
    RequestSelectBgmAssets();
}

void ReturnFromEndingFmv(void) asm("func_80022F2C");
void ReturnFromEndingFmv(void) {
    CdSync(0, 0);
    CdControl(9, 0, 0);
    SetDispMask(0);
    SetupDisplay240(0, 0, 0);
    g_FrameSyncThreshold = 0x80;
    g_FadeStep = 4;
    g_FadeLevel = 0;
    g_SceneId = 0x22;
    g_SceneTimer = 0;
}
