#include "common.h"
#include "game/menu.h"
#include "game/race.h"
#include "game/state.h"
#include "game/render.h"
#include "psyq/gpu.h"
#include "game/cd.h"

extern char D_80010F90[], D_80010FA8[], D_80010F98[], D_80010F9C[], D_80010FA4[];
extern s32 D_8009E858[];
extern s32 D_8019CAC8;
extern s32 D_8019CE10;
extern s32 D_801E8A48;
extern u8 *D_8007D4C0[];
extern s32 D_8007D508[];
extern s32 D_8007D4D4[];
void func_80016EA0(void *dst, s32 len, void *src, s32 arg3);
void func_80016754(void *dst, s32 x, void *src, s32 color);
void *func_80021CD4(void *dst, s32 value);
void func_800632F0();
s32 GameAddTilePrim(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) asm("func_80032F34");
extern volatile s32 D_801E4BA8;
extern volatile u16 g_PlayerCarIndex asm("D_801E40D4");
extern s32 D_801E774C;
extern u16 D_801E7750[];
extern s32 D_8019CB80;
extern u16 D_8019CB84[];
extern s32 D_8019C768;
extern s32 D_801E6C8C;
void func_80022324(void);
extern u8 D_80010FB0[];
extern s32 D_8019C8F8;
extern s32 D_8019CA14;
extern u8 D_801E417C[];
extern s32 D_801E6830;
extern u8 D_801F17FC[];
void GameRequestSelectBgmAssets(void) asm("func_80018410");
void func_80021D68(void);
void func_80021DB8(u8 *arg0);
void func_80022068(u8 *arg0);
void func_8002229C(s32 arg0, s32 arg1);
void func_80033AA0(s32 arg0, s32 arg1);
void GamePlaySoundCue(s32 cue) asm("func_8005D6EC");
void func_8006A534(s32 arg0, s32 arg1);
s32 CdControl(s32 com, void *param, s32 result) asm("func_8006A5A4");

void func_80021DB8(u8 *arg0) {
    register u8 *panel;
    register s32 iter;
    register s32 countOrIndex;
    register s32 xOrField;
    register s32 destination;
    register s32 color;
    register s32 scoreOrX;
    char text[56];
    s32 mode;
    s32 row;
    s32 doubledRow;
    s32 value;
    s32 scoreValue;
    s32 limit;

    panel = arg0;
    func_80016EA0(panel + 0x10, 0x4C, D_80010F90, 0x7852);
    mode = g_CourseIndex;
    text[1] = 0x2F;
    limit = 6;
    if (mode != 3) {
        limit = 3;
    }
    iter = 0;
    if (limit > 0) {
        scoreOrX = (s32)D_8009E858;
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
            if (D_8019CAC8 == iter) {
                color = 0x780F;
            }
            func_80016754((void *)destination, xOrField, text, color);
            iter++;
            scoreOrX += 4;
        } while (iter < limit);
    }
    func_80016EA0(panel + 0x10, 0x6C, D_80010F98, 0x7812);
    countOrIndex = 0;
    scoreOrX = 0x82;
    destination = 0x78;
    do {
        text[0] = D_8007D4C0[countOrIndex][0];
        text[1] = D_8007D4C0[countOrIndex][1];
        text[2] = D_8007D4C0[countOrIndex][2];
        text[3] = 0x2F;
        func_80021CD4(&text[4], g_RankingRecords[g_GrandPrixSeries][g_CourseIndex][countOrIndex].v8);
        xOrField = g_RankingRecords[g_GrandPrixSeries][g_CourseIndex][countOrIndex].vC;
        func_800632F0(&text[0xC], D_80010F9C,
                      &g_RankingRecords[g_GrandPrixSeries][g_CourseIndex][countOrIndex],
                      D_8007D508[xOrField]);
        color = 0x78CC;
        if (D_801E8A48 == countOrIndex) {
            color = 0x780F;
        }
        func_80016754(panel + 0x14, destination, text, color);
        func_800632F0(text, D_80010FA4, D_8007D4D4[xOrField]);
        func_80016754(panel + 0x2C, scoreOrX, text, color);
        destination += 0x14;
        scoreOrX += 0x14;
        countOrIndex++;
    } while (countOrIndex < 5);
}

void func_80022068(u8 *s5) {
    char text[48];
    s32 s4, s3;
    s32 s2, color, idx;

    func_80016EA0(s5 + 0x10, 0x4C, D_80010FA8, 0x7852);

    text[0] = 0x54;
    text[1] = 0x2F;
    func_80021CD4(&text[2], D_801E4BA8);
    func_80016754(s5 + 0x14, 0x58, text, 0x78CC);

    func_80016EA0(s5 + 0x10, 0x6C, D_80010F98, 0x7812);

    s2 = 0;
    s4 = 0x82;
    s3 = 0x78;
    for (; s2 < 5; s2++) {
        text[0] = D_8007D4C0[s2][0];
        text[1] = D_8007D4C0[s2][1];
        text[2] = D_8007D4C0[s2][2];
        text[3] = 0x2F;
        func_80021CD4(&text[4], g_TimeRecords[g_GrandPrixSeries][g_CourseIndex][s2].v8);

        idx = g_TimeRecords[g_GrandPrixSeries][g_CourseIndex][s2].vC;
        func_800632F0(&text[0xC], D_80010F9C,
                      &g_TimeRecords[g_GrandPrixSeries][g_CourseIndex][s2], D_8007D508[idx]);

        color = 0x78CC;
        if (D_8019CE10 == s2) {
            color = 0x780F;
        }
        func_80016754(s5 + 0x14, s3, text, color);

        func_800632F0(text, D_80010FA4, D_8007D4D4[idx]);

        func_80016754(s5 + 0x2C, s4, text, color);
        s3 += 0x14;
        s4 += 0x14;
    }
}

void func_8002229C(s32 arg0, s32 arg1) {
    s32 *scratch;

    if (g_AnimTimer & 8) {
        scratch = (s32 *)0x1F800000;
        *scratch = GameAddTilePrim(
            g_DrawBuffer + 0xCC,
            *scratch,
            (arg0 << 3) + 0x7C,
            (((arg1 << 2) + arg1) << 2) + 0x7E,
            9,
            2,
            0xC0,
            0x48,
            0x48);
    }

}

void func_80022324(void) {
    s32 count;
    register s32 i asm("$8");
    register s32 row_offset asm("$9");
    register s32 best asm("$10");
    register s32 j asm("$7");
    register s32 *score_ptr asm("$4");
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
    register s32 mode asm("$4");
    register s32 base_addr asm("$2");
    register s32 course_addr asm("$3");
    register u8 *name_base asm("$11");
    register u8 *name_base2 asm("$10");
    register s32 letter asm("$13");
    register s32 code asm("$12");
    register s32 letter2 asm("$12");
    register s32 code2 asm("$11");

    count = 3;
    if (g_CourseIndex == 3) {
        count = 6;
    }

    best = 0x927C0;
    i = 0;
    if (i < count) {
        score_ptr = D_8009E858;
        while (i < count) {
            if (*score_ptr < best) {
                best = *score_ptr;
                D_8019CAC8 = i;
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
        if (best < *((s32 *)((u8 *)&D_801E774C + score_offset))) {
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
            *((s32 *)((u8 *)&D_801E774C + score_offset)) = best;
            j = 0;
            fill_offset = row_offset;
            for (; j < 6; j++) {
                fill_addr = fill_offset + (g_CourseIndex * 0x50);
                fill_addr += g_GrandPrixSeries * 0x140;
                *((volatile u8 *)((fill_addr + (s32)name_base) + j)) = letter;
                D_801E417C[j] = code;
            }

            score_offset = row_offset + (g_CourseIndex * 0x50);
            score_offset += g_GrandPrixSeries * 0x140;
            *((u16 *)((u8 *)D_801E7750 + score_offset)) = g_PlayerCarIndex;
            break;
        }
        i++;
        row_offset += 0x10;
    }

    D_801E8A48 = i;
    i = 0;
    name_base2 = (u8 *)g_TimeRecords;
    letter2 = 0x41;
    code2 = 0xB;
    row_offset = 0;
    while (i < 5) {
        score_offset = row_offset + (g_CourseIndex * 0x50);
        score_offset += g_GrandPrixSeries * 0x140;
        score_value = *((s32 *)((u8 *)&D_8019CB80 + score_offset));
        if (D_801E4BA8 < score_value) {
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
            *((s32 *)((u8 *)&D_8019CB80 + score_offset)) = D_801E4BA8;
            j = 0;
            fill_offset = row_offset;
            for (; j < 6; j++) {
                fill_addr = fill_offset + (g_CourseIndex * 0x50);
                fill_addr += g_GrandPrixSeries * 0x140;
                *((volatile u8 *)((fill_addr + (s32)name_base2) + j)) = letter2;
                D_801F17FC[j] = code2;
            }

            score_offset = row_offset + (g_CourseIndex * 0x50);
            score_offset += g_GrandPrixSeries * 0x140;
            *((u16 *)((u8 *)D_8019CB84 + score_offset)) = g_PlayerCarIndex;
            break;
        }
        i++;
        row_offset += 0x10;
    }

    D_8019CE10 = i;
}

void func_80022748(void) {
    g_SceneTimer = 0x100;
    D_8019C768 = 0x80;
    D_801E6C8C = 0;
    g_SceneId = 0x15;
    func_80022324();
}

void func_80022794(void) {
    u8 *name;
    s32 i;

    g_AnimTimer++;

    switch (D_801E6C8C) {
    case 0:
        g_SceneTimer -= 8;
        func_80033AA0(g_SceneTimer, 0x49);
        if (g_SceneTimer == 0) {
            if (D_801E8A48 < 5 || D_8019CE10 < 5) {
                GameRequestCdTrack(0xE);
                GameStartCdAudio();
            }
            if (D_801E8A48 < 5) {
                D_801E6830 = 0xB;
                D_8019C8F8 = 0;
                D_801E6C8C = 1;
            } else {
                D_801E6C8C = 2;
            }
        }
        func_80021DB8((u8 *)0);
        break;

    case 1: {
        u8 *timeName;
        u8 *timeRecordBase;
        u8 *rankingRecordBase;
        u8 *record;
        s32 previous;
        u16 buttons;

        previous = D_801E6830;
        if (g_PadEdge & 0x8000) {
            D_801E6830 = previous - 1;
        } else if (g_PadEdge & 0x2000) {
            D_801E6830 = previous + 1;
        }
        D_801E6830 = (D_801E6830 + 42) % 42;
        if (previous != D_801E6830) {
            GamePlaySoundCue(1);
        }

        D_801E417C[D_8019C8F8] = D_801E6830;
        buttons = g_PadEdge2;
        name = (u8 *)D_801E417C;
        if (buttons & 0x860) {
            GamePlaySoundCue(2);
            D_8019C8F8++;
            if (D_8019C8F8 == 6) {
                D_801E6C8C = 2;
                i = 0;
                if (D_8019CE10 < 5) {
                    timeRecordBase = g_TimeRecords;
                    timeName = D_801F17FC;
                    do {
                        *timeName = D_801E417C[i];
                        record = (u8 *)((((g_CourseIndex * 5) + D_8019CE10) *
                                         0x10) +
                                        (g_GrandPrixSeries * 0x140) +
                                        (s32)timeRecordBase + i);
                        i++;
                        *record = D_80010FB0[*timeName];
                        timeName++;
                    } while (i < 6);
                }
            }
            D_801E6830 = D_801E417C[D_8019C8F8];
        } else if ((buttons & 0x90) && D_8019C8F8 > 0) {
            GamePlaySoundCue(3);
            D_8019C8F8--;
            D_801E6830 = name[D_8019C8F8];
        }

        if (D_801E6C8C == 1) {
            func_8002229C(D_8019C8F8, D_801E8A48);
        }
        i = 0;
        rankingRecordBase = g_RankingRecords;
        do {
            record = (u8 *)((((g_CourseIndex * 5) + D_801E8A48) * 0x10) +
                            (g_GrandPrixSeries * 0x140) +
                            (s32)rankingRecordBase + i);
            *record = D_80010FB0[D_801E417C[i]];
            i++;
        } while (i < 6);
        func_80021DB8((u8 *)0);
        break;
    }

    case 2:
        if (g_PadEdge2 & 0x860) {
            D_801E6C8C = 3;
            D_8019CA14 = 0;
        }
        func_80021DB8((u8 *)0);
        break;

    case 3:
        D_8019CA14 -= 8;
        func_80021DB8((u8 *)D_8019CA14);
        func_80022068((u8 *)(D_8019CA14 + 0x140));
        if (D_8019CA14 < -0x13F) {
            if (D_8019CE10 < 5) {
                D_8019C8F8 = 0;
                D_801E6C8C = 4;
                D_801E6830 = D_801F17FC[0];
            } else {
                D_801E6C8C = 5;
            }
        }
        break;

    case 4: {
        u8 *recordBase;
        u8 *record;
        s32 previous;
        u16 buttons;

        previous = D_801E6830;
        if (g_PadEdge & 0x8000) {
            D_801E6830 = previous - 1;
        } else if (g_PadEdge & 0x2000) {
            D_801E6830 = previous + 1;
        }
        D_801E6830 = (D_801E6830 + 42) % 42;
        if (previous != D_801E6830) {
            GamePlaySoundCue(1);
        }

        D_801F17FC[D_8019C8F8] = D_801E6830;
        buttons = g_PadEdge2;
        name = (u8 *)D_801F17FC;
        if (buttons & 0x860) {
            GamePlaySoundCue(2);
            D_8019C8F8++;
            if (D_8019C8F8 == 6) {
                D_801E6C8C = 5;
            }
            D_801E6830 = name[D_8019C8F8];
        } else if ((buttons & 0x90) && D_8019C8F8 > 0) {
            GamePlaySoundCue(3);
            D_8019C8F8--;
            D_801E6830 = name[D_8019C8F8];
        }

        if (D_801E6C8C == 4) {
            func_8002229C(D_8019C8F8, D_8019CE10);
        }
        i = 0;
        recordBase = g_TimeRecords;
        do {
            record = (u8 *)((((g_CourseIndex * 5) + D_8019CE10) * 0x10) +
                            (g_GrandPrixSeries * 0x140) + (s32)recordBase + i);
            *record = D_80010FB0[D_801F17FC[i]];
            i++;
        } while (i < 6);
        func_80022068((u8 *)0);
        break;
    }

    case 5:
        if (g_PadEdge2 & 0x860) {
            if (D_801E8A48 < 5 || D_8019CE10 < 5) {
                GameStartCdVolumeFade(0x78);
                GameStartCdAudio();
            }
            D_801E6C8C = 6;
            D_8019CA14 = 0;
        }
        func_80022068((u8 *)0);
        break;

    case 6:
        g_SceneTimer += 2;
        func_80033AA0(g_SceneTimer, 0x49);
        if ((u32)g_SceneTimer >= 0x100) {
            GameRequestSelectBgmAssets();
            g_SceneId = 6;
        }
        func_80022068((u8 *)0);
        break;
    }

    func_80021D68();
}

void func_80022EE4(void) {
    func_8006A534(0, 0);
    CdControl(9, 0, 0);
    g_SceneId = 6;
    GameRequestSelectBgmAssets();
}

void func_80022F2C(void) {
    func_8006A534(0, 0);
    CdControl(9, 0, 0);
    SetDispMask(0);
    GameSetupDisplay240(0, 0, 0);
    D_8019C768 = 0x80;
    g_FadeStep = 4;
    g_FadeLevel = 0;
    g_SceneId = 0x22;
    g_SceneTimer = 0;
}
