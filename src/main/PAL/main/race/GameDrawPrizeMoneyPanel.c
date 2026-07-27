#include "common.h"
#include "game/race.h"
#include "game/menu.h"
#include "game/car.h"
#include "game/state.h"

extern s32 g_PrizeAmount asm("D_801F17B0");
extern s32 g_PromotionBonus asm("D_8019CE0C");
extern u8 D_80010E30[], D_80010E34[], D_80010E38[], D_80010E40[];
void func_80016EA0(s32 id, void *dst, void *src, s32 arg3);
void func_800632F0(void *dst, void *fmt, s32 val);
extern s32 g_CourseProgress asm("D_8009E67C");
extern s32 g_ClassClearFanfareTimer asm("D_801E4D0C");
extern s32 g_ClassCompleted asm("D_801E4B94");
extern s16 D_8019CB58;
extern s16 D_8019CB54;
extern GameScoreRecord g_ClassRecords[] asm("D_8019CB40");
extern GameScoreRecord D_8019CB42[];
extern s32 g_ClassResultPlace asm("D_8019C7C4");
extern s32 g_SeriesCleared asm("D_8019C8EC");
extern s32 g_ClassPromoted asm("D_801E419C");
s32 GameGetCarUnlockLevel(s32 model) asm("func_8001785C");
s32 func_800214B8(void);
void GameUpdateBgmTrackCount(void) asm("func_80021540");
void GameResetProgressSlot(s32 arg0, s32 arg1) asm("func_80021288");
void GameResetCourseProgress(s32 arg0) asm("func_800212F0");
void GameBeginEndingFmv(s32 arg0) asm("func_80019BB8");
void GameBeginClassFmv(s32 arg0) asm("func_80019B3C");
extern s32 D_8019C768;
extern s32 g_PrizeScreenStep asm("D_8019CB74");
extern s32 g_PrizeAmount asm("D_801F17B0");
extern s32 g_PrizeCountStep asm("D_801E6DA0");
extern s32 g_BonusCountStep asm("D_801E6C78");
extern s32 D_8007BEF4[][6][3];
extern s32 g_PromotionBonusTable[] asm("D_8007C00C");
void GamePlaySoundCue(s32 cue) asm("func_8005D6EC");

void GameDrawPrizeMoneyPanel(u8 *s0) asm("func_800206B8");
void GameDrawPrizeMoneyPanel(u8 *s0) {
    u8 sp[16];
    if (g_RaceProgress->unk10 > 0x3B9AC9FF) {
        g_RaceProgress->unk10 = 0x3B9AC9FF;
    }
    func_80016EA0(0x10, s0 + 128, D_80010E30, 0x7812);
    func_800632F0(sp, D_80010E34, g_PrizeAmount);
    func_80016EA0(0x12, s0 + 140, sp, 0x7812);
    func_80016EA0(0x10, s0 + 160, D_80010E38, 0x7812);
    func_800632F0(sp, D_80010E34, g_RaceProgress->unk10);
    func_80016EA0(0x12, s0 + 172, sp, 0x7812);
    if (g_ClassPromoted != 0) {
        func_80016EA0(0x10, s0 + 192, D_80010E40, 0x7812);
        func_800632F0(sp, D_80010E34, g_PromotionBonus);
        func_80016EA0(0x12, s0 + 204, sp, 0x7812);
    }
}

void GameCommitClassProgress(void) asm("func_800207E0");
void GameCommitClassProgress(void) {
    s32 score_index;
    u8 *slots;
    s32 slot_count;
    s32 filled;
    s32 i;
    s32 done;
    s32 value;
    s32 *state;

    slots = (u8 *)(g_CourseProgress + g_CourseIndex);
    g_ClassClearFanfareTimer = 0;

    if (*slots == 0 || g_RacePosition < *slots) {
        *slots = g_RacePosition;
    }

    value = GameGetCarUnlockLevel(g_PlayerCarIndex);
    slot_count = 4;
    if (g_GrandPrixClass < value) {
        *(s16 *)(g_CourseProgress + 4) = 1;
    }

    if (g_GrandPrixClass < 2) {
        slot_count = 3;
    }

    filled = 0;
    for (i = 0; i < slot_count; i++) {
        if (((u8 *)g_CourseProgress)[i] != 0) {
            filled++;
        }
    }

    done = ((slot_count ^ filled) == 0);
    g_ClassCompleted = done;

    if (done != 0) {
        s16 *record;

        score_index = (g_GrandPrixSeries * 6) + g_GrandPrixClass;

        if (score_index == 4) {
            record = &D_8019CB58;
            goto check_record;
        }
        if (score_index != 10) {
            goto not_special_record;
        }
        record = &D_8019CB54;

check_record:
        if (*record == -1) {
            *record = 0;
        }

        goto after_record_check;

not_special_record:
        if (score_index != 5) {
            if (g_ClassRecords[score_index + 1].value == -1) {
                g_ClassRecords[score_index + 1].value = 0;
            }
        }

after_record_check:
        value = func_800214B8();
        g_ClassResultPlace = value;
        if (value != 0) {
            s32 offset;

            offset = score_index * 4;
            if (g_ClassRecords[score_index].value == 0 || value < g_ClassRecords[score_index].value) {
                g_ClassRecords[score_index].value = (u16)g_ClassResultPlace;
            }
            g_ClassClearFanfareTimer = 0xD2;
        }

        GameUpdateBgmTrackCount();
        if (g_ClassResultPlace == 1) {
            s32 offset;

            offset = score_index * 4;
            if (*(s16 *)((char *)D_8019CB42 + offset) < 99) {
                (*(s16 *)((char *)D_8019CB42 + offset))++;
            }
        }
    } else {
        g_ClassResultPlace = 0;
    }

    g_SeriesCleared = 0;
    if (g_ClassCompleted != 0) {
        if ((g_SeriesSelection == 0 && g_GrandPrixClass == 4) || (g_SeriesSelection == 1 && g_GrandPrixClass == 5)) {
            g_SeriesCleared = 1;
            g_AdvancedSeriesUnlocked = 1;
        }
    }

    g_ClassPromoted = 0;
    if (g_ClassCompleted != 0 && g_SeriesCleared == 0) {
        state = (s32 *)g_RaceProgress;
        if (state[3] < g_GrandPrixClass + 1) {
            g_ClassPromoted = 1;
        }
    }
}

void GameAdvanceGrandPrixClass(void) asm("func_80020B08");
void GameAdvanceGrandPrixClass(void) {
    s32 oldValue;
    GameRaceProgress *ptr;
    s32 *entry;

    if (g_ClassCompleted != 0) {
        if (g_SeriesCleared != 0) {
            s32 magic;
            GameRaceProgress *afterPtr;

            ptr = g_RaceProgress;
            oldValue = ptr->maxClassReached;
            GameResetProgressSlot((s32)g_CarTable, (s32)ptr);
            magic = 0x3B9AC9FF;
            afterPtr = g_RaceProgress;
            afterPtr->unk10 = magic;
            afterPtr->maxClassReached = oldValue;
            GameResetCourseProgress(0);
            GameBeginEndingFmv(0x21);
        } else {
            s32 current;
            s32 next;
            GameRaceProgress *menuPtr;
            s32 enabled;

            GameBeginClassFmv(7);
            current = g_GrandPrixClass;
            menuPtr = g_RaceProgress;
            enabled = g_ClassPromoted;
            next = current + 1;
            g_GrandPrixClass = next;
            menuPtr->classIndex = next;
            menuPtr->course = 0;

            if (enabled != 0) {
                menuPtr->maxClassReached = next;
                entry = &g_MaxClassReached[g_SeriesSelection];
                if (*entry < next) {
                    *entry = next;
                }
            }

            GameResetCourseProgress(g_GrandPrixClass);
        }
    } else {
        g_SceneId = 6;
    }
}

void GameEnterPrizeScreen(void) asm("func_80020C24");
void GameEnterPrizeScreen(void) {
    s32 mode;
    s32 car;
    s32 value;

    g_SceneTimer = 0x100;
    D_8019C768 = 0x80;

    mode = g_CourseIndex;
    car = g_GrandPrixClass;
    g_PrizeScreenStep = 0;
    g_PrizeAmount = g_PrizeMoney[mode][car][g_RacePosition - 1];
    g_SceneId = 0x13;

    if (g_ClassPromoted != 0) {
        g_PromotionBonus = g_PromotionBonusTable[car];
    } else {
        g_PromotionBonus = 0;
    }

    value = D_8007BEF4[g_CourseIndex][g_GrandPrixClass][0] / 80;
    g_PrizeCountStep = value;
    if (value <= 0) {
        g_PrizeCountStep = 1;
    }

    value = g_PromotionBonusTable[g_GrandPrixClass] / 250;
    g_BonusCountStep = value;
    if (value <= 0) {
        g_BonusCountStep = 1;
    }
}

void GameTickClassClearFanfare(void) asm("func_80020D90");
void GameTickClassClearFanfare(void) {
    if (g_ClassClearFanfareTimer > 0) {
        g_ClassClearFanfareTimer--;
    }
    if (g_ClassClearFanfareTimer == 0xB4) {
        GamePlaySoundCue(0x42);
    }
}
